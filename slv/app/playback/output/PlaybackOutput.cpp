#include "PlaybackOutput.h"

PlaybackOutput::PlaybackOutput()
: m_pCFrame(nullptr)
{
    m_pCFrame = std::make_shared<PlaybackFrame>();
}

PlaybackOutput::~PlaybackOutput()
{
    outputThread.join();
}

void PlaybackOutput::Start()
{
    LOGE("Playback output thread started");
    outputThread = std::thread(&PlaybackOutput::Output, this);
}

// void PlaybackOutput::shareFrame(std::shared_ptr<PlaybackFrame> frame)
// {
//     if(mediator == nullptr)
//     {
//         LOGE("Mediator is not set, cannot share frame");
//         return;
//     }
//     LOGE("Sending frame to output");
//     mediator->fwdFrameOut(frame);
//     LOGE("after Sending frame to output");
// }