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
#include "PlaybackDefine.h"

class PlaybackPlayer
{
public:
    PlaybackPlayer();
    ~PlaybackPlayer();

    void start(void);
    void SetConfig(void);
    void SetPlayInfo(const std::string& filename, const Playbackdef::PlaybackType& eType);
    void PlayStart(void);
    void Seek(const int64_t& position);
    void Pause(void);
    void Resume(void);
    void PlayStop(void);

    void outPutView(void);
    void handleEvent(void);

private:
    std::atomic<bool> m_bIsExit;
    std::unique_ptr<PlaybackDemux> m_pCdemux;
    std::shared_ptr<PlaybackDecodeVideo> m_pCdecodeVideo;
    std::shared_ptr<PlaybackDecodeAudio> m_pCdecodeAudio;
    std::shared_ptr<PlaybackFrame> m_pCFrameVideo;
    std::shared_ptr<PlaybackClock> m_pCClock;
    std::atomic<Playbackdef::PlaybackState> m_ePlaybackState;
    std::atomic<Playbackdef::PlaybackType> m_ePlaybackType;
    std::thread inputThread;
    std::thread outPutThreadVideo;
    PlaybackWindow win;

};


#endif