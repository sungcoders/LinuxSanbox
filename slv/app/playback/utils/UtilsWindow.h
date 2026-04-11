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

struct SDLColor
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
};

class UtilsWindow
{
public:
    UtilsWindow();
    ~UtilsWindow();

    bool isInsideArea(int mouseX, int mouseY, SDL_Rect& buttonRect);
    void drawPlayIcon(SDL_Renderer* renderer, const SDL_Rect& box);
    void drawPauseIcon(SDL_Renderer* renderer, const SDL_Rect& box);
    void drawNextIcon(SDL_Renderer* renderer, int x, int y);
    void drawTimeline(SDL_Renderer* renderer, const SDL_Rect& box, const SDLColor& color);

private:
    
};

#endif // UTILS_WINDOW_H
