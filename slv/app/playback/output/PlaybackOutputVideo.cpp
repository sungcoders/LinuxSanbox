#include "PlaybackOutputVideo.h"

PlaybackOutputVideo::PlaybackOutputVideo()
: m_pCFrame(nullptr)
{
}

void PlaybackOutputVideo::Init(std::shared_ptr<PlaybackFrame> frame)
{
    this->m_pCFrame = frame;
    // shareFrame(frame);
    // win.createWindow(1920, 1044);
    LOGE("Video output thread initialized");
}

void PlaybackOutputVideo::Output()
{
    while (true)
    {
        // win.WindowEvent();
        if(m_pCFrame == nullptr)
        {
            LOGE("Frame queue is not initialized");
            usleep(100000);
            continue;
        }
        FrameInfo sFrame = {};
        m_pCFrame->pop(sFrame);

        LOGE("Display frame video: {:.3f}s", sFrame.timestamp);
        // win.renderFrame(sFrame.frame);
        // win.delay(33);

        av_frame_free(&sFrame.frame);
        // LOGE("Frame displayed, waiting for next frame...");
    }
    LOGE("Video output thread finished");
}
