#include "PlaybackFrame.h"

PlaybackFrame::PlaybackFrame()
: sLastFrame{}
, queue()
, m_mutex()
, cv()
{
}

void PlaybackFrame::push(FrameInfo& sFrame)
{
    std::unique_lock<std::mutex> lock(m_mutex);

    FrameInfo sFrameInfo{};
    sFrameInfo.frame = av_frame_alloc();
    
    sFrameInfo.timestamp = sFrame.timestamp;
    sFrameInfo.duration  = sFrame.duration;
    av_frame_move_ref(sFrameInfo.frame, sFrame.frame);

    queue.push(sFrameInfo);

    av_frame_free(&sFrame.frame);

    cv.notify_one();
}

void PlaybackFrame::pop(FrameInfo& sFrame)
{
    std::unique_lock<std::mutex> lock(m_mutex);

    cv.wait(lock, [this]() {
        return !queue.empty();
    });

    FrameInfo sFrameInfo = queue.front();
    queue.pop();

    sFrame.frame = sFrameInfo.frame;
    sFrame.timestamp = sFrameInfo.timestamp;
    sFrame.duration  = sFrameInfo.duration;

    sLastFrame = sFrameInfo;
    
    sFrameInfo.frame = nullptr;

    cv.notify_one();
}

FrameInfo& PlaybackFrame::getLastFrame()
{
    return sLastFrame;
}

int PlaybackFrame::size()
{
    std::unique_lock<std::mutex> lock(m_mutex);
    return queue.size();
}

void PlaybackFrame::waitFrame()
{
    std::unique_lock<std::mutex> lock(m_mutex);
    cv.wait(lock, [this]()
    {
        return queue.size() < 10;
    });
    return;
}

void PlaybackFrame::abortFrame()
{
    flushFrame();
    LOGI("flushed frame");
}

void PlaybackFrame::flushFrame()
{
    std::unique_lock<std::mutex> lock(m_mutex);
    while (!queue.empty())
    {
        FrameInfo sFrameInfo = queue.front();
        av_frame_free(&sFrameInfo.frame);
        queue.pop();
    }
    cv.notify_all();
}

