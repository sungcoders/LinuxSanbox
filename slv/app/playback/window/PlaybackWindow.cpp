#include "PlaybackWindow.h"

PlaybackWindow::PlaybackWindow()
: window(nullptr)
, renderer(nullptr)
, texture(nullptr)
, m_bIsPaused(false)
, m_iTimeSeek(-1)
{
}

PlaybackWindow::~PlaybackWindow()
{
    LOGI("Destroying PlaybackWindow...\n");
    destroyWindow();
}

void PlaybackWindow::createWindow(int width, int height)
{
    LOGI("Creating window with resolution: {}x{}", width, height);
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow(
        "Playback (SungLV author)",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        width,
        height,
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
    );

    renderer = SDL_CreateRenderer(window, -1, 0);
    texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_IYUV,
        SDL_TEXTUREACCESS_STREAMING,
        width,
        height
    );
}

void PlaybackWindow::resizeWindow(int width, int height)
{
    if (window)
    {
        SDL_SetWindowSize(window, width, height);
    }
}

void PlaybackWindow::renderFrame(const FrameInfo& sframe)
{
    SDL_UpdateYUVTexture(
        texture,
        NULL,
        sframe.frame->data[0], sframe.frame->linesize[0],
        sframe.frame->data[1], sframe.frame->linesize[1],
        sframe.frame->data[2], sframe.frame->linesize[2]
    );
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);

    if(isHoldBar())
    {
        drawTimelineBackground(renderer);
        renderBar(renderer, sframe.timestamp, 435);
    }

    SDL_RenderPresent(renderer);
}

void PlaybackWindow::renderBar(SDL_Renderer* renderer, double pos, int lengh)
{
    // 1750  -- x
    // lengh -- pos
    // x = pos * 1750 / lengh
    // propose: rate = 1/length
    // x = pos * 1750 * rate
    int xPos = std::floor(pos * 1750 / lengh);
    m_button = {50, 1030, 60, 40};
    m_point = {xPos+120, 1040, 20, 20};
    m_timelinebar = {120, 1045, 1750, 10};
    m_timelineprogress = {120, 1045, xPos, 10};

    // Icon pause/resume
    if (m_bIsPaused.load()) utilsWindow.drawPlayIcon(renderer, m_button);
    else                    utilsWindow.drawPauseIcon(renderer, m_button);
    
    // timeline bar
    SDLColor sWhite = {255, 255, 255, 255};
    SDLColor sDarkBlue = {0, 0, 255, 255};
    utilsWindow.drawTimeline(renderer, m_timelinebar, sWhite);
    utilsWindow.drawTimeline(renderer, m_timelineprogress, sDarkBlue);
    utilsWindow.drawTimeline(renderer, m_point, sDarkBlue);
}

void PlaybackWindow::drawTimelineBackground(SDL_Renderer* renderer)
{
    SDL_Rect panel = {40, 1025, 1840, 50};

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 150);
    SDL_RenderFillRect(renderer, &panel);
}

void PlaybackWindow::delay(int ms)
{
    SDL_Delay(ms);
}

void PlaybackWindow::destroyWindow()
{
    if (texture) {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }
    if (renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }
    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
    SDL_Quit();
}

void PlaybackWindow::WindowEvent(SDL_Event& eventType)
{
    while (SDL_PollEvent(&event))
    {
        // if(event.type != 1024)
        // {
        //     LOGI("-------------------- Received event: {} --------------------", event.type);
        // }

        eventType = event;
        if(event.type == SDL_MOUSEBUTTONDOWN)
        {
            if (utilsWindow.isInsideArea(event.button.x, event.button.y, m_button))
            {
                m_bIsPaused.store(!m_bIsPaused.load());
                LOGI("Pause button clicked, new state: {}", m_bIsPaused.load());
            }
            else if (utilsWindow.isInsideArea(event.button.x, event.button.y, m_timelinebar))
            {
                int length = 1750;
                double ratio = (double)(event.button.x - 120) / length;
                int sec = ratio * 435;
                m_iTimeSeek.store(sec);
                LOGI("bar timeline clicked x:{}, y:{}, lenght={}, sec={}", event.button.x, event.button.y, length, sec);
            }
        }
        // if(event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
        // {
        //     winWidth = event.window.data1;
        //     winHeight = event.window.data2;

        //     pauseBtn.y = winHeight - 70;
        //     LOGI("Window resized: {}x{}", winWidth, winHeight);
        // }
    }
}

bool PlaybackWindow::isPaused()
{
    return m_bIsPaused.load();
}

bool PlaybackWindow::isHoldBar()
{
    // uitl time check time mouse is not in screen 100ms
    return true;
}

int PlaybackWindow::getTimeSeek()
{
    int iseek = m_iTimeSeek.load();
    m_iTimeSeek.store(-1);
    return iseek;
}
