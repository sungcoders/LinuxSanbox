#include "PlaybackPlayer.h"
#include <iostream>

PlaybackPlayer::PlaybackPlayer()
: m_bIsExit(false)
, m_pCdemux(nullptr)
, m_pCdecodeVideo(nullptr)
, m_pCdecodeAudio(nullptr)
, m_pCFrameVideo(nullptr)
, m_pCClock(nullptr)
, m_ePlaybackState(PlaybackState::PRE_INIT_E)
{
}

PlaybackPlayer::~PlaybackPlayer()
{
    outPutThreadVideo.join();
}

void PlaybackPlayer::start()
{
    m_pCFrameVideo = std::make_shared<PlaybackFrame>();
    m_pCClock = std::make_shared<PlaybackClock>();
    m_pCdecodeVideo = std::make_shared<PlaybackDecodeVideo>(m_pCFrameVideo, m_pCClock);
    m_pCdecodeAudio = std::make_shared<PlaybackDecodeAudio>(m_pCClock);
    m_pCdemux = std::make_unique<PlaybackDemux>(m_pCdecodeVideo, m_pCdecodeAudio, m_pCClock);

    if(m_pCdemux == nullptr || m_pCdecodeVideo == nullptr || m_pCFrameVideo == nullptr)
    {
        LOGE("Failed to start MediaPlayer");
        return;
    }
    // system("pwd");
    SetPlayInfo("/workspace/slv/app/playback/Media/mp4/video3.mp4");
    PlayStart();
}

void PlaybackPlayer::outPutView()
{
    win.createWindow(1920, 1080);
    while(!m_bIsExit.load())
    {
        handleEvent();
        if(m_pCClock->isPaused())
        {
            win.renderFrame(m_pCFrameVideo->getLastFrame());
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            continue;
        }
        FrameInfo sFrame = {};
        m_pCFrameVideo->pop(sFrame);
        // LOGE("video timstamp: {:.3f}s", sFrame.timestamp);
        win.renderFrame(sFrame);
        win.delay(40);
        av_frame_free(&sFrame.frame);
    }
    LOGE("Outputing process finished");
}

void PlaybackPlayer::handleEvent()
{
    SDL_Event event;
    win.WindowEvent(event);
    switch (event.type)
    {
    case SDL_QUIT:
        LOGI("Quit event received, exiting...");
        PlayStop();
        return;
    case SDL_KEYDOWN:
        LOGI("Key down event: {}", event.key.keysym.sym);
        return;
    case SDL_MOUSEBUTTONDOWN:
        if(win.isPaused()) { Pause();  }
        else               { Resume(); }
        return;
    }
}

void PlaybackPlayer::SetPlayInfo(std::string filename)
{
    m_pCdemux->Init(filename);
    m_ePlaybackState.store(PlaybackState::INIT_E);
    LOGD("PlaybackPlayer SetPlayInfo");
}

void PlaybackPlayer::SetConfig()
{
    // Code để thiết lập cấu hình phát video
}

void PlaybackPlayer::PlayStart()
{
    m_pCdemux->Start();
    m_ePlaybackState.store(PlaybackState::PLAY_E);
    outPutThreadVideo = std::thread(&PlaybackPlayer::outPutView, this);
    LOGD("PlaybackPlayer started");
}

void PlaybackPlayer::Seek(int64_t position)
{
    // Code để thực hiện seek đến vị trí mong muốn
    m_pCClock->setPause();
    m_pCdemux->SeekStream(position);
    m_pCClock->setPlay();
}

void PlaybackPlayer::Pause()
{
    m_ePlaybackState.store(PlaybackState::PAUSE_E);
    m_pCClock->setPause();
    LOGD("PlaybackPlayer paused");
}

void PlaybackPlayer::Resume()
{
    m_ePlaybackState.store(PlaybackState::PLAY_E);
    m_pCClock->setPlay();
    LOGD("PlaybackPlayer resumed");
}

void PlaybackPlayer::PlayStop()
{
    m_ePlaybackState.store(PlaybackState::STOP_E);
    m_pCdemux->Stop();
    win.destroyWindow();
    m_bIsExit.store(true);
}

