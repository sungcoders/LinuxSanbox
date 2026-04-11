#ifndef PlAYBACKCLOCK_H
#define PlAYBACKCLOCK_H

#include <atomic>

struct PlaybackInfo
{
    int iLength;
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

private:
    std::atomic<bool> m_bIsPaused;

};

#endif // PlAYBACKCLOCK_H
