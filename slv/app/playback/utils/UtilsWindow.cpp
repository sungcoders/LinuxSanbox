#include "UtilsWindow.h"

UtilsWindow::UtilsWindow()
{
}

UtilsWindow::~UtilsWindow()
{
}

bool UtilsWindow::isInsideArea(int mouseX, int mouseY, SDL_Rect& buttonRect)
{
    return (mouseX >= buttonRect.x && mouseX <= buttonRect.x + buttonRect.w &&
            mouseY >= buttonRect.y && mouseY <= buttonRect.y + buttonRect.h);
}

void UtilsWindow::drawPlayIcon(SDL_Renderer* renderer, const SDL_Rect& box)
{
    SDL_RenderDrawRect(renderer, &box);
    int w = 25;             // width
    int h = 20;             // height
    int x = box.x + 15;     // x position
    int y = box.y + 10;     // y position

    SDL_Point p1 = {x, y};
    SDL_Point p2 = {x, y + h};
    SDL_Point p3 = {x + w, y + h / 2};

    SDL_RenderDrawLine(renderer, p1.x, p1.y, p2.x, p2.y);
    SDL_RenderDrawLine(renderer, p2.x, p2.y, p3.x, p3.y);
    SDL_RenderDrawLine(renderer, p3.x, p3.y, p1.x, p1.y);
}

void UtilsWindow::drawPauseIcon(SDL_Renderer* renderer, const SDL_Rect& box)
{
    SDL_RenderDrawRect(renderer, &box);

    int barWidth = box.w * 0.15;   // 15% width
    int gap      = box.w * 0.10;   // 10% distance

    int totalWidth = 2 * barWidth + gap;

    int startX = box.x + (box.w - totalWidth) / 2;  // blance
    int startY = box.y + box.h * 0.2;               // 2% height
    int barH   = box.h * 0.6;                       // 6% height

    SDL_Rect left  = {startX, startY, barWidth, barH};
    SDL_Rect right = {startX + barWidth + gap, startY, barWidth, barH};

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
