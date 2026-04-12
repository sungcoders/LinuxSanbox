#include "PlaybackWindow.h"

PlaybackWindow::PlaybackWindow()
: window(nullptr)
, renderer(nullptr)
, texture(nullptr)
, m_bIsPaused(false)
, m_iTimeSeek(-1)
, m_iScreenW(0)
, m_iScreenH(0)
{
}

PlaybackWindow::~PlaybackWindow()
{
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
    m_iScreenW.store(width);
    m_iScreenH.store(height);
}

void PlaybackWindow::resizeWindow(int width, int height)
{
    if (window)
    {
        SDL_SetWindowSize(window, width, height);
        m_iScreenW.store(width);
        m_iScreenH.store(height);
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
    // sample size screen 1920x1080
    // 1750  -- x
    // lengh -- pos
    // x = pos * 1750 / lengh
    // propose: rate = 1/length
    // x = pos * 1750 * rate

    // old data
    // int xPos = std::floor(pos * 1750 / lengh);
    // m_button = {50, 1030, 60, 40};
    // m_point = {xPos+120, 1040, 20, 20};
    // m_timelinebar = {120, 1045, 1750, 10};
    // m_timelineprogress = {120, 1045, xPos, 10};

    // SDL_Rect panel = {40, 1025, 1840, 50};

    // gốc: {50, 1030, 60, 40}
    float btn_x = 0.0054;   // button cách mép trái panel (50 - 40) / 1840.0f = 0.54% chiều rộng panel
    float btn_y = 0.1;      // button cách trên panel (1030 - 1025) / 50.0f = 10% chiều cao
    float btn_w = 0.0326;   // button rộng 60 / 1840.0f = 3.26% panel
    float btn_h = 0.8;      // button cao 40 / 50.0f = 80% panel

    // gốc: {120, 1045, 1750, 10}
    float bar_x = 0.0435;   // timeline bắt đầu cách trái panel một đoạn (120 - 40) / 1840.0f = 4.35%
    float bar_y = 0.4;      // timeline nằm ở (1045 - 1025) / 50.0f = 40% chiều cao panel (gần giữa)
    float bar_w = 0.95;     // 1750 / 1840.0f = 95% panel → gần full chiều ngang
    float bar_h = 0.2;      // 10 / 50.0f = 20% chiều cao panel

    // gốc: {xPos+120, 1040, 20, 20}
    float point_y = 0.3;    // point nằm ở (1040 - 1025) / 50.0f = 30% chiều cao panel (hơi cao hơn timeline)
    float point_w = 0.01;   // nhỏ, 20 / 1840.0f = 1% panel
    float point_h = 0.4;    // 20 / 50.0f = 40% chiều cao panel

    // Button pause/resume
    m_button.x = m_panel.x + btn_x * m_panel.w;
    m_button.y = m_panel.y + btn_y * m_panel.h;
    m_button.w = btn_w * m_panel.w;
    m_button.h = btn_h * m_panel.h;

    // Bar timeline
    m_timelinebar.x = m_panel.x + bar_x * m_panel.w;
    m_timelinebar.y = m_panel.y + bar_y * m_panel.h;
    m_timelinebar.w = bar_w * m_panel.w;
    m_timelinebar.h = bar_h * m_panel.h;

    // PROGRESS (xPos là pixel trong timeline gốc → phải scale)
    int progressW = (pos / lengh) * m_timelinebar.w;

    m_timelineprogress = {
        m_timelinebar.x,
        m_timelinebar.y,
        progressW,
        m_timelinebar.h
    };

    // Point
    m_point.w = point_w * m_panel.w;
    m_point.h = point_h * m_panel.h;
    m_point.y = m_panel.y + point_y * m_panel.h;

    // vị trí theo progress
    m_point.x = m_timelinebar.x + progressW - m_point.w / 2;

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
    m_panel.x = 0.021 * m_iScreenW.load();  // 2% left/right
    m_panel.y = 0.949 * m_iScreenH.load();  // 95% top top/bottom
    m_panel.w = 0.958 * m_iScreenW.load();  // 95% width screen
    m_panel.h = 0.046 * m_iScreenH.load();  // 5% height screen
    // old design with screen 1920x1080
    // SDL_Rect panel = {40, 1025, 1840, 50};

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 150);             // Black alpha 150 color
    SDL_RenderFillRect(renderer, &m_panel);
}

void PlaybackWindow::delay(int ms)
{
    SDL_Delay(ms);
}

void PlaybackWindow::setTotalTimehBar(int length)
{
    m_iLengthVideo.store(length);
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
    LOGE("Destroy window video");
}

void PlaybackWindow::WindowEvent(SDL_Event& eventType)
{
    while (SDL_PollEvent(&event))
    {
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
                int length = m_timelinebar.w;                                           // width of bar timline
                double ratio = (double)(event.button.x - m_timelinebar.x) / length;     // raito between x click and width of bar timeline
                int sec = ratio * m_iLengthVideo.load();                                // seconds seek = raito * total length video
                m_iTimeSeek.store(sec);
                LOGI("bar timeline clicked x:{}, y:{}, lenght={}, sec={}", event.button.x, event.button.y, length, sec);
            }
        }
        else if(event.window.event == SDL_WINDOWEVENT_RESIZED)
        {
            int screenW, screenH;
            SDL_GetWindowSize(window, &screenW, &screenH);
            m_iScreenW.store(screenW);
            m_iScreenH.store(screenH);
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
    // until mouse is not in screen
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    return !(mouseX == 0 || mouseX == m_iScreenW.load() -1 ||
            mouseY == 0 || mouseY == m_iScreenH.load() -1);
}

int PlaybackWindow::getTimeSeek()
{
    int iseek = m_iTimeSeek.load();
    m_iTimeSeek.store(-1);
    return iseek;
}
