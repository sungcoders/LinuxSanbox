#ifndef PLAYBACKPLAYER_H
#define PLAYBACKPLAYER_H

#include <thread>
#include <atomic>

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/imgutils.h>
}

#include "PlaybackDemux.h"
#include "PlaybackClock.h"
#include "PlaybackDecodeAudio.h"
#include "PlaybackDecodeVideo.h"
#include "PlaybackWindow.h"
#include "WindowAudio.h"

class PlaybackPlayer
{
public:
    PlaybackPlayer();
    ~PlaybackPlayer();

    void start();
    void SetConfig();
    void SetPlayInfo(std::string filename);
    void PlayStart();
    void Seek(int64_t position);
    void Pause();
    void Resume();
    void PlayStop();

    void outPutView();
    void handleEvent();

private:
    enum class PlaybackState
    {
        PRE_INIT_E,
        INIT_E,
        PLAY_E,
        PAUSE_E,
        STOP_E,
        COMPLETED_E
    };

    std::atomic<bool> m_bIsExit;
    std::unique_ptr<PlaybackDemux> m_pCdemux;
    std::shared_ptr<PlaybackDecodeVideo> m_pCdecodeVideo;
    std::shared_ptr<PlaybackDecodeAudio> m_pCdecodeAudio;
    std::shared_ptr<PlaybackFrame> m_pCFrameVideo;
    std::shared_ptr<PlaybackClock> m_pCClock;
    std::atomic<PlaybackState> m_ePlaybackState;
    std::thread inputThread;
    std::thread outPutThreadVideo;
    PlaybackWindow win;

};


#endif