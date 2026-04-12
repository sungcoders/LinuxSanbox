#include "PlaybackOutput.h"

PlaybackOutput::PlaybackOutput()
{
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
