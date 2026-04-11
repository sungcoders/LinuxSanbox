#include "PlaybackClock.h"

PlaybackClock::PlaybackClock()
: m_bIsPaused(false)
, m_PlaybackInfo()
, m_mutexInfo()
{
}

PlaybackClock::~PlaybackClock()
{
}

void PlaybackClock::setPause()
{
    m_bIsPaused.store(true);
}

void PlaybackClock::setPlay()
{
    m_bIsPaused.store(false);
}

bool PlaybackClock::isPaused()
{
    return m_bIsPaused.load();
}

void PlaybackClock::getPlaybackInfo(PlaybackInfo& info)
{
    std::unique_lock<std::mutex> lock(m_mutexInfo);
    info = m_PlaybackInfo;
}

void PlaybackClock::setPlaybackInfo(const PlaybackInfo& info)
{
    std::unique_lock<std::mutex> lock(m_mutexInfo);
    m_PlaybackInfo = info;
}
