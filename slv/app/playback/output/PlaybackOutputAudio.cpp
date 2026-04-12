#include "PlaybackOutputAudio.h"

PlaybackOutputAudio::PlaybackOutputAudio()
: m_bIsExit(false)
, m_codecCtx(nullptr)
, m_pCClock(nullptr)
{
}

PlaybackOutputAudio::~PlaybackOutputAudio()
{
}

void PlaybackOutputAudio::Init(AVCodecContext* codecCtx, std::shared_ptr<PlaybackClock> clock, std::shared_ptr<PlaybackFrame> frame)
{
    m_codecCtx = codecCtx;
    m_pCClock = clock;
    m_pCFrame = frame;
    audio.audioInit(m_codecCtx, m_pCClock);
}

void PlaybackOutputAudio::Stop()
{
    m_bIsExit.store(true);
}

void PlaybackOutputAudio::Output()
{
    while(!m_bIsExit.load())
    {
        if(m_pCClock->isPaused())
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            continue;
        }
        FrameInfo sFrame = {};
        m_pCFrame->pop(sFrame);
        // LOGE("Output frame audio: {:.3f}s", sFrame.timestamp);
        audio.sendFrameOutput(sFrame);
        av_frame_free(&sFrame.frame);
    }
    LOGE("Outputing audio process finished");
}
