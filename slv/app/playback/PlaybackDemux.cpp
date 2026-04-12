#include "PlaybackDemux.h"

PlaybackDemux::PlaybackDemux(std::shared_ptr<PlaybackDecodeVideo> decodeVideo, std::shared_ptr<PlaybackDecodeAudio> decodeAudio, std::shared_ptr<PlaybackClock> clock)
: m_pCdecodeVideo(decodeVideo)
, m_pCdecodeAudio(decodeAudio)
, m_pCClock(clock)
, m_bExit(false)
, m_fmtCtx(nullptr)
, m_codecCtxVideo(nullptr)
, m_codecCtxAudio(nullptr)
, m_idxvideoStream(-1)
, m_idxaudioStream(-1)
, m_dVideoTimeBase(0.0)
, m_dAudioTimeBase(0.0)
, m_isSeek(false)
, m_pCpacketVideo(nullptr)
, m_pCpacketAudio(nullptr)
{
}

PlaybackDemux::~PlaybackDemux()
{
    demuxThread.join();
}

void PlaybackDemux::Start(void)
{
    LOGD("Starting demuxing process...");
    demuxThread = std::thread(&PlaybackDemux::Demux, this);
}

void PlaybackDemux::Stop(void)
{
    m_pCdecodeVideo->Stop();
    m_pCdecodeAudio->Stop();
    m_pCpacketVideo->abortPacket();
    m_pCpacketAudio->abortPacket();
    m_bExit.store(true);
}

void PlaybackDemux::Init(const std::string& filename)
{
    avformat_network_init();
    if (avformat_open_input(&m_fmtCtx, filename.c_str(), nullptr, nullptr) != 0)
    {
        LOGE("can not open file {} {}", filename, errno);
        return;
    }
    avformat_find_stream_info(m_fmtCtx, nullptr);
    m_idxvideoStream = av_find_best_stream(m_fmtCtx, AVMEDIA_TYPE_VIDEO, -1, -1, nullptr, 0);
    m_idxaudioStream = av_find_best_stream(m_fmtCtx, AVMEDIA_TYPE_AUDIO, -1, -1, nullptr, 0);
    m_dVideoTimeBase = av_q2d(m_fmtCtx->streams[m_idxvideoStream]->time_base);
    m_dAudioTimeBase = av_q2d(m_fmtCtx->streams[m_idxaudioStream]->time_base);

    AVCodecParameters* codecParVideo = m_fmtCtx->streams[m_idxvideoStream]->codecpar;
    AVCodecParameters* codecParAudio = m_fmtCtx->streams[m_idxaudioStream]->codecpar;

    const AVCodec* codecVideo = avcodec_find_decoder(codecParVideo->codec_id);
    const AVCodec* codecAudio = avcodec_find_decoder(codecParAudio->codec_id);
    
    m_codecCtxVideo = avcodec_alloc_context3(codecVideo);
    m_codecCtxAudio = avcodec_alloc_context3(codecAudio);

    avcodec_parameters_to_context(m_codecCtxVideo, codecParVideo);
    avcodec_parameters_to_context(m_codecCtxAudio, codecParAudio);

    avcodec_open2(m_codecCtxVideo, codecVideo, nullptr);
    avcodec_open2(m_codecCtxAudio, codecAudio, nullptr);

    av_dump_format(m_fmtCtx, 0, filename.c_str(), 0);
    
    PlaybackInfo sInfo
    {
        m_fmtCtx->duration / AV_TIME_BASE,
        codecParVideo->width,
        codecParVideo->height
    };
    m_pCClock->setPlaybackInfo(sInfo);
}

void PlaybackDemux::Demux(void)
{
    AVPacket* packet = av_packet_alloc();
    m_pCpacketVideo = std::make_shared<PlaybackPacket>();
    m_pCpacketAudio = std::make_shared<PlaybackPacket>();
    m_pCdecodeVideo->Init(m_codecCtxVideo, m_dVideoTimeBase, m_pCpacketVideo);
    m_pCdecodeAudio->Init(m_codecCtxAudio, m_dAudioTimeBase, m_pCpacketAudio);
    m_pCdecodeVideo->Start();
    m_pCdecodeAudio->Start();

    while(!m_bExit.load())
    {
        if(m_pCClock->isPaused())
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            continue;
        }
        int ret = av_read_frame(m_fmtCtx, packet);
        if (ret < 0)
        {
            // Handle error or end of file
        }
        else
        {
            handleEnoughPacket(packet);
            pushPacketAV(packet);
            av_packet_unref(packet);
        }
    }

    av_packet_free(&packet);
    avcodec_free_context(&m_codecCtxVideo);
    avcodec_free_context(&m_codecCtxAudio);
    avformat_close_input(&m_fmtCtx);
    LOGE("Demuxing process finished");
}

void PlaybackDemux::pushPacketAV(AVPacket* avpacket)
{
    AVPacket* pkt = av_packet_alloc();
    av_packet_ref(pkt, avpacket);

    if(avpacket->stream_index == m_idxvideoStream)
    {
        if(m_isSeek.load() && avpacket->flags & AV_PKT_FLAG_KEY)
        {
            LOGD("push video {} packet: {:.3f}s size {}", (avpacket->flags & AV_PKT_FLAG_KEY ) ? "key" : "", m_dVideoTimeBase * avpacket->pts, m_pCpacketVideo->size());
            m_pCpacketVideo->push(avpacket);
            m_isSeek.store(false);
            return;
        }
        // LOGD("push video packet: {:.3f}s size {}", m_dVideoTimeBase * avpacket->pts, m_pCpacketVideo->size());
        m_pCpacketVideo->push(avpacket);
    }
    else if(avpacket->stream_index == m_idxaudioStream)
    {
        // LOGD("push audio packet: {:.3f}s size {}", m_dAudioTimeBase * avpacket->pts, m_pCpacketAudio->size());
        m_pCpacketAudio->push(avpacket);
    }
    else
    {
        LOGD("Unknown stream index: {} type {}", avpacket->stream_index, static_cast<int>(m_fmtCtx->streams[avpacket->stream_index]->codecpar->codec_type));
    }
}

void PlaybackDemux::handleEnoughPacket(AVPacket* avpacket)
{
    if(avpacket->stream_index == m_idxvideoStream)
    {
        m_pCpacketVideo->waitPacket();
    }
    if(avpacket->stream_index == m_idxaudioStream)
    {
        m_pCpacketAudio->waitPacket();
    }
}

void PlaybackDemux::SeekStream(int64_t sec)
{
    int64_t targetSeek = (int64_t)(sec / m_dVideoTimeBase);

    // Seek to pts
    int ret = avformat_seek_file(
        m_fmtCtx,
        m_idxvideoStream,
        INT64_MIN,
        targetSeek,
        INT64_MAX,
        AVSEEK_FLAG_BACKWARD
    );

    if (ret < 0)
    {
        printf("Seek failed\n");
        return;
    }

    m_isSeek.store(true);
    // Flush decoder
    avcodec_flush_buffers(m_codecCtxVideo);
    avcodec_flush_buffers(m_codecCtxAudio);

    // Flush packet queue
    m_pCpacketVideo->abortPacket();
    m_pCpacketAudio->abortPacket();

    // Flush frame queue
    m_pCdecodeVideo->FlushRemainFrame();
    m_pCdecodeAudio->FlushRemainFrame();

    LOGD("Seek to {} sec done\n", sec);
}
