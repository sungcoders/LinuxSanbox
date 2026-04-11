#ifndef PLAYBACKFRAME_H
#define PLAYBACKFRAME_H

#include <queue>
#include <mutex>
#include <condition_variable>

extern "C" {
#include <libavcodec/avcodec.h>
}

#include "PlaybackClock.h"
#include "UtilsLog.h"

struct FrameInfo
{
    AVFrame* frame;
    double timestamp;
    double duration;
};

class PlaybackFrame
{
public:
    PlaybackFrame();
    ~PlaybackFrame() = default;

    void push(FrameInfo& sFrame);
    void pop(FrameInfo& sFrame);
    FrameInfo& getLastFrame();

    int size();
    void waitFrame();
    void abortFrame();
    void flushFrame();

private:
    FrameInfo sLastFrame;
    std::queue<FrameInfo> queue;
    std::mutex m_mutex;
    std::condition_variable cv;

};

#endif // PLAYBACKFRAME_H