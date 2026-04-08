#ifndef PlAYBaCKOUTPUTAUDIO_H
#define PlAYBaCKOUTPUTAUDIO_H

#include "PlaybackOutput.h"
#include "WindowAudio.h"
#include "PlaybackClock.h"
#include "PlaybackFrame.h"

class PlaybackOutputAudio : public PlaybackOutput
{
public:
    PlaybackOutputAudio();
    ~PlaybackOutputAudio();

    void Init(AVCodecContext* codecCtx, std::shared_ptr<PlaybackClock> clock, std::shared_ptr<PlaybackFrame> frame);
    void Stop();
    void Output() override;

private:
    std::atomic<bool> m_bIsExit;
    AVCodecContext* m_codecCtx;
    std::shared_ptr<PlaybackClock> m_pCClock;
    std::shared_ptr<PlaybackFrame> m_pCFrame;
    WindowAudio audio;

};

#endif // PlAYBACKOUTPUTAUDIO_H
