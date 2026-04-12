#include "PlaybackClock.h"

PlaybackClock::PlaybackClock()
: m_bIsPaused(false)
, m_PlaybackInfo()
, m_mutexInfo()
, m_mutexAudio()
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

void PlaybackClock::setClock(double timestamp, bool isAudio)
{
    std::unique_lock<std::mutex> lock(m_mutexAudio);
    PlaybackSync& ps_sync = isAudio ? sAudioSync : sVideoSync;
    ps_sync.dTimestamp = timestamp;
}

double PlaybackClock::getClock(bool isAudio)
{
    std::unique_lock<std::mutex> lock(m_mutexAudio);
    PlaybackSync& ps_sync = isAudio ? sAudioSync : sVideoSync;
    return ps_sync.dTimestamp;
}

int PlaybackClock::getDelayTime(double timestamp)
{
    int delay = 0;
    double audio_clock = getClock(true);
    double diff = timestamp - audio_clock;

    const double AV_SYNC_THRESHOLD = 0.04;      // 
    const double AV_NOSYNC_THRESHOLD = 10.0;    // 

    if (std::fabs(diff) < AV_NOSYNC_THRESHOLD)
    {
        if (diff > AV_SYNC_THRESHOLD)
        {
            // video soon → delay
            delay = static_cast<int>(diff * 1000);
        }
        else if (diff < -AV_SYNC_THRESHOLD)
        {
            // video late → drop frame
            delay = 0;
        }
    }
    
    LOGD("{:.3f} - {:.3f} = {:.3f} ({}s)", timestamp, audio_clock, diff, delay);
    return delay;
}
