#ifndef PLAYBACKDECODEAUDIO_H
#define PLAYBACKDECODEAUDIO_H

#include "PlaybackDecode.h"
#include "PlaybackOutputAudio.h"

class PlaybackDecodeAudio: public PlaybackDecode
{
public:
    PlaybackDecodeAudio(std::shared_ptr<PlaybackClock> clock);
    ~PlaybackDecodeAudio();

    void Init(AVCodecContext* codecCtx, double timebase, std::shared_ptr<PlaybackPacket> packet);
    void Stop();
    void Decode() override;

private:
    std::atomic<bool> m_bExit;
    std::shared_ptr<PlaybackPacket> m_pCPacket;
    std::shared_ptr<PlaybackFrame> m_pCFrame;
    std::shared_ptr<PlaybackClock> m_pCClock;
    std::unique_ptr<PlaybackOutputAudio> m_pCOutputAudio;
    double m_dTimebase;
    AVCodecContext* m_codecCtx;

};

#endif // PLAYBACKDECODEAUDIO_H
