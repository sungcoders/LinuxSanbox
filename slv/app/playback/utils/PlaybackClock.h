#ifndef PlAYBACKCLOCK_H
#define PlAYBACKCLOCK_H

#include <atomic>
#include <mutex>

struct PlaybackInfo
{
    int64_t iLength;
    int iHResolution;
    int iWResolution;
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

private:
    std::atomic<bool> m_bIsPaused;
    PlaybackInfo m_PlaybackInfo;
    std::mutex m_mutexInfo;

};

#endif // PlAYBACKCLOCK_H
