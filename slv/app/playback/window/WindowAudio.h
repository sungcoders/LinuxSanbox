#ifndef WINDOW_AUDIO_H
#define WINDOW_AUDIO_H

#include <iostream>

extern "C" {
#include <libavformat/avformat.h>
#include <libswresample/swresample.h>
#include <libavcodec/avcodec.h>
#include <libavutil/opt.h>
}

#include <SDL2/SDL.h>
#include "PlaybackFrame.h"
#include "UtilsLog.h"
#include "UtilsEvent.h"

class WindowAudio
{
public:
    WindowAudio();
    ~WindowAudio();

    void audioInit(AVCodecContext* codecCtx, std::shared_ptr<PlaybackClock> clock);
    void getDeviceIDAudio();
    void resampleAudio();
    void sendFrameOutput(const FrameInfo& sframe);
    void destroyAudio();

private:
    SwrContext* m_swrCtx;
    AVCodecContext* m_codecCtx;
    SDL_AudioDeviceID m_deviceID;
    std::shared_ptr<PlaybackClock> m_pClock;

};

#endif // WINDOW_AUDIO_H
