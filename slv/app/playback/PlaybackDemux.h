#ifndef PLAYBACKDEMUX_H
#define PLAYBACKDEMUX_H

extern "C" {
#include <libavformat/avformat.h>
}

#include "PlaybackPacket.h"
#include "PlaybackDecodeVideo.h"
#include "PlaybackDecodeAudio.h"

class PlaybackDemux
{
public:
    PlaybackDemux(std::shared_ptr<PlaybackDecodeVideo> decodeVideo, std::shared_ptr<PlaybackDecodeAudio> decodeAudio, std::shared_ptr<PlaybackClock> clock);
    ~PlaybackDemux();

    void Init(const std::string& filename);
    void Start(void);
    void Stop(void);

private:
    std::atomic<bool> m_bExit;
    AVFormatContext* m_fmtCtx;
    AVCodecContext* m_codecCtxVideo;
    AVCodecContext* m_codecCtxAudio;
    int m_idxvideoStream;
    int m_idxaudioStream;
    double m_dVideoTimeBase;
    double m_dAudioTimeBase;
    std::shared_ptr<PlaybackPacket> m_pCpacketVideo;
    std::shared_ptr<PlaybackPacket> m_pCpacketAudio;
    std::shared_ptr<PlaybackDecodeVideo> m_pCdecodeVideo;
    std::shared_ptr<PlaybackDecodeAudio> m_pCdecodeAudio;
    std::shared_ptr<PlaybackClock> m_pCClock;
    std::thread demuxThread;

    void Demux(void);
    void pushPacketAV(AVPacket* avpacket);
    void handleEnoughPacket(AVPacket* avpacket);

};

#endif // PLAYBACKDEMUX_H
