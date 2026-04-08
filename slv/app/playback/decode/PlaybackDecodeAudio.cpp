#include "PlaybackDecodeAudio.h"

PlaybackDecodeAudio::PlaybackDecodeAudio(std::shared_ptr<PlaybackClock> clock)
: m_bExit(false)
, m_pCPacket(nullptr)
, m_pCClock(clock)
, m_pCOutputAudio(nullptr)
, m_dTimebase(0.0)
, m_codecCtx(nullptr)
{
}

PlaybackDecodeAudio::~PlaybackDecodeAudio()
{
}

void PlaybackDecodeAudio::Init(AVCodecContext* codecCtx, double timebase, std::shared_ptr<PlaybackPacket> packet)
{
    this->m_codecCtx = codecCtx;
    this->m_dTimebase = timebase;
    this->m_pCPacket = packet;
    
    m_pCFrame = std::make_shared<PlaybackFrame>();
    m_pCOutputAudio = std::make_unique<PlaybackOutputAudio>();
    LOGW("Audio decode thread started");
}

void PlaybackDecodeAudio::Stop()
{
    m_pCOutputAudio->Stop();
    m_pCFrame->abortFrame();
    m_bExit.store(true);
}

void PlaybackDecodeAudio::Decode()
{
    AVPacket* avpacket = av_packet_alloc();
    AVFrame* avframe = av_frame_alloc();
    m_pCOutputAudio->Init(m_codecCtx, m_pCClock, m_pCFrame);
    m_pCOutputAudio->Start();

    while(!m_bExit.load())
    {
        if(m_pCClock->isPaused())
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            continue;
        }
        FrameInfo sFrame = {};
        m_pCPacket->pop(avpacket);

        int ret = avcodec_send_packet(m_codecCtx, avpacket);
        if (ret < 0)
        {
            av_packet_unref(avpacket);
            continue;
        }

        while (ret >= 0)
        {
            ret = avcodec_receive_frame(m_codecCtx, avframe);
            if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF || ret < 0)
            {
                break;
            }
            
            sFrame.frame = av_frame_alloc();
            sFrame.timestamp = avframe->pts * m_dTimebase;
            av_frame_move_ref(sFrame.frame, avframe);

            // LOGW("[{}]push audio frame: {:.3f}s size {}", avpacket->stream_index, sFrame.timestamp, m_pCFrame->size());
            m_pCFrame->push(sFrame);

            avframe = av_frame_alloc();
        }
        av_packet_unref(avpacket);
    }
    av_packet_free(&avpacket);
    av_frame_free(&avframe);
    LOGE("Decoding audio process finished");
}
