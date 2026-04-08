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

class WindowAudio
{
public:
    WindowAudio();
    ~WindowAudio();

    void audioInit(AVCodecContext* codecCtx);
    void getDeviceIDAudio();
    void resampleAudio();
    void sendFrameOutput(AVFrame* frame);
    void destroyAudio();

private:
    SwrContext* m_swrCtx;
    AVCodecContext* m_codecCtx;
    SDL_AudioDeviceID m_deviceID;

};

#endif // WINDOW_AUDIO_H
