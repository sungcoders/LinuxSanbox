#ifndef UTILS_WINDOW_H
#define UTILS_WINDOW_H

#include <SDL2/SDL.h>
#include "PlaybackFrame.h"
#include "UtilsLog.h"

struct ButtonInfo
{
    int x;
    int y;
    int width;
    int height;
};

class UtilsWindow
{
public:
    UtilsWindow();
    ~UtilsWindow();

    void drawButton(SDL_Rect& button, ButtonInfo& info);
    void renderButton(SDL_Renderer* renderer, SDL_Rect& buttonRect, bool isPaused);
    bool isInsideArea(int mouseX, int mouseY, SDL_Rect& buttonRect);
    void drawPlayIcon(SDL_Renderer* renderer, const SDL_Rect& box);
    void drawPauseIcon(SDL_Renderer* renderer, const SDL_Rect& box);
    void drawNextIcon(SDL_Renderer* renderer, int x, int y);
    void drawTimeline(SDL_Renderer* renderer, int x, int y);

private:
    
};

#endif // UTILS_WINDOW_H
