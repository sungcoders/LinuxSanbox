#ifndef PLAYBACKWINDOW_H
#define PLAYBACKWINDOW_H

#include <atomic>
#include "UtilsWindow.h"

class PlaybackWindow
{
public:
    PlaybackWindow();
    ~PlaybackWindow();

    void createWindow(int width, int height);
    void renderFrame(const FrameInfo& sframe);
    void resizeWindow(int width, int height);
    void delay(int ms);
    void setTotalTimehBar(int length);
    void renderBar(SDL_Renderer* renderer, double pos, int lengh);
    void drawTimelineBackground(SDL_Renderer* renderer);
    void destroyWindow();
    bool isPaused();
    bool isHoldBar();
    int getTimeSeek();

    void WindowEvent(SDL_Event& eventType);
    
private:
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_Texture* texture = nullptr;
    SDL_Rect m_button;
    SDL_Rect m_point;
    SDL_Rect m_timelinebar;
    SDL_Rect m_timelineprogress;
    SDL_Rect m_panel;
    std::atomic<bool> m_bIsPaused;
    std::atomic<int> m_iTimeSeek;
    std::atomic<int> m_iLengthVideo;
    std::atomic<int> m_iScreenW;
    std::atomic<int> m_iScreenH;
    SDL_Event event;
    std::thread eventThread;
    UtilsWindow utilsWindow;
};

#endif // WINDOW_H
