#ifndef PlAYBACKCLOCK_H
#define PlAYBACKCLOCK_H

#include <atomic>
#include <mutex>
#include <cmath>
#include "UtilsLog.h"

struct PlaybackInfo
{
    int64_t iLength;
    int iHResolution;
    int iWResolution;
};

struct PlaybackSync
{
    double dTimestamp;
    long long llLastTime;
};

class PlaybackClock
{
public:
    PlaybackClock();
    ~PlaybackClock();

    void setPause();
    void setPlay();
    bool isPaused();
    void getPlaybackInfo(PlaybackInfo& info);
    void setPlaybackInfo(const PlaybackInfo& info);

    void setClock(double timestamp, bool isAudio);
    double getClock(bool isAudio);
    int getDelayTime(double timestamp);

private:
    std::atomic<bool> m_bIsPaused;
    PlaybackInfo m_PlaybackInfo;
    std::mutex m_mutexInfo;
    std::mutex m_mutexAudio;
    PlaybackSync sAudioSync;
    PlaybackSync sVideoSync;

};

#endif // PlAYBACKCLOCK_H
