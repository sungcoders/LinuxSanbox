#include "PlaybackClock.h"

PlaybackClock::PlaybackClock()
: m_bIsPaused(false)
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
