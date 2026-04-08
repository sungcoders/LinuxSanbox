#include "UtilsWindow.h"

UtilsWindow::UtilsWindow()
{
}

UtilsWindow::~UtilsWindow()
{
}

void UtilsWindow::drawButton(SDL_Rect& button, ButtonInfo& info)
{
    button = {info.x, info.y, info.width, info.height};
}

void UtilsWindow::renderButton(SDL_Renderer* renderer, SDL_Rect& buttonRect, bool isPaused)
{
    if (isPaused) SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // {render, r, g, b, a} đỏ
    else          SDL_SetRenderDrawColor(renderer, 0, 200, 0, 255); // {render, r, g, b, a} xanh

    SDL_RenderFillRect(renderer, &buttonRect);
}

bool UtilsWindow::isInsideArea(int mouseX, int mouseY, SDL_Rect& buttonRect)
{
    return (mouseX >= buttonRect.x && mouseX <= buttonRect.x + buttonRect.w &&
            mouseY >= buttonRect.y && mouseY <= buttonRect.y + buttonRect.h);
}

void UtilsWindow::drawPlayIcon(SDL_Renderer* renderer, const SDL_Rect& box)
{
    SDL_RenderDrawRect(renderer, &box);
    int x = box.x + 30;
    int y = box.y + 5;
    SDL_RenderDrawLine(renderer, x, y, x, y + 40);
    SDL_RenderDrawLine(renderer, x, y, x + 30, y + 20);
    SDL_RenderDrawLine(renderer, x, y + 40, x + 30, y + 20);
}

void UtilsWindow::drawPauseIcon(SDL_Renderer* renderer, const SDL_Rect& box)
{
    SDL_RenderDrawRect(renderer, &box);
    int x = box.x + 30;
    int y = box.y + 5;
    SDL_Rect left = {x, y, 5, 40};
    SDL_Rect right = {x + 20, y, 5, 40};

    SDL_RenderFillRect(renderer, &left);
    SDL_RenderFillRect(renderer, &right);
}

void UtilsWindow::drawNextIcon(SDL_Renderer* renderer, int x, int y)
{
    // tam giác 1
    SDL_RenderDrawLine(renderer, x, y, x, y + 40);
    SDL_RenderDrawLine(renderer, x, y, x + 20, y + 20);
    SDL_RenderDrawLine(renderer, x, y + 40, x + 20, y + 20);

    // tam giác 2
    SDL_RenderDrawLine(renderer, x + 20, y, x + 20, y + 40);
    SDL_RenderDrawLine(renderer, x + 20, y, x + 40, y + 20);
    SDL_RenderDrawLine(renderer, x + 20, y + 40, x + 40, y + 20);

    // thanh đứng
    SDL_RenderDrawLine(renderer, x + 45, y, x + 45, y + 40);
}

void UtilsWindow::drawTimeline(SDL_Renderer* renderer, const SDL_Rect& box, const SDLColor& color)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, &box);
    SDL_RenderDrawRect(renderer, &box);
}
