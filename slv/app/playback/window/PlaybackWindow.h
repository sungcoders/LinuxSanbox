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
    void renderFrame(AVFrame* frame);
    void resizeWindow(int width, int height);
    void delay(int ms);
    void destroyWindow();
    bool isPaused();

    void WindowEvent(SDL_Event& eventType);
    
private:
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_Texture* texture = nullptr;
    SDL_Rect m_button;
    SDL_Rect m_point;
    SDL_Rect m_timelinebar;
    SDL_Rect m_timelineprogress;
    std::atomic<bool> m_bIsPaused;
    // int winWidth;
    // int winHeight;
    SDL_Event event;
    std::thread eventThread;
    UtilsWindow utilsWindow;
};

#endif // WINDOW_H
