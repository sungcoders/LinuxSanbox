#ifndef PLAYBACKDECODEVIDEO_H
#define PLAYBACKDECODEVIDEO_H

#include "PlaybackDecode.h"
#include "PlaybackWindow.h"

class PlaybackDecodeVideo : public PlaybackDecode
{
public:
    PlaybackDecodeVideo(std::shared_ptr<PlaybackFrame> frame, std::shared_ptr<PlaybackClock> clock);
    ~PlaybackDecodeVideo() = default;

    void Init(AVCodecContext* codecCtx, double timebase, std::shared_ptr<PlaybackPacket> packet);
    void Stop();
    void FlushRemainFrame() override;

private:
    std::atomic<bool> m_bExit;
    std::shared_ptr<PlaybackFrame> m_pCFrame;
    std::shared_ptr<PlaybackPacket> m_pCPacket;
    std::shared_ptr<PlaybackClock> m_pCClock;
    double m_dTimebase;
    AVCodecContext* m_codecCtx;
    std::thread decodeThread;

    void Decode() override;
    void handleEnoughFrame();
};

#endif // PLAYBACKDECODEVIDEO_H
