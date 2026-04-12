#include "PlaybackPacket.h"

PlaybackPacket::PlaybackPacket()
: queue()
, m_mutex()
, cv()
{
}

void PlaybackPacket::push(AVPacket* pkt)
{
    std::unique_lock<std::mutex> lock(m_mutex);
    AVPacket* packet = av_packet_alloc();
    
    if (pkt == nullptr || packet == nullptr)
    {
        LOGE("Failed to push packet");
        return;
    }

    av_packet_move_ref(packet, pkt);
    queue.push(packet);

    cv.notify_one();
}

void PlaybackPacket::pop(AVPacket* pkt)
{
    std::unique_lock<std::mutex> lock(m_mutex);
    cv.wait(lock, [this]()
    {
        return !queue.empty();
    });

    AVPacket* packet = queue.front();
    queue.pop();

    av_packet_move_ref(pkt, packet);
    av_packet_free(&packet);
    
    cv.notify_one();
}

int PlaybackPacket::size()
{
    std::unique_lock<std::mutex> lock(m_mutex);
    return queue.size();
}

void PlaybackPacket::waitPacket()
{
    std::unique_lock<std::mutex> lock(m_mutex);
    cv.wait(lock, [this]()
    {
        return queue.size() < 30;
    });
    return;
}

void PlaybackPacket::abortPacket()
{
    flushPacket();
    LOGI("flushed packet");
}

void PlaybackPacket::flushPacket()
{
    std::unique_lock<std::mutex> lock(m_mutex);
    while (!queue.empty())
    {
        AVPacket* packet = queue.front();
        av_packet_free(&packet);
        queue.pop();
    }
    cv.notify_all();
}
