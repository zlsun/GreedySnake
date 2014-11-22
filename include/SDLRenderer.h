#ifndef SDL_RENDERER_H
#define SDL_RENDERER_H

#include <string>
#include <memory>
#include <vector>
#include <stdexcept>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "SDLRect.h"
#include "SDLPoint.h"
#include "SDLColor.h"
#include "SDLTexture.h"


enum Alignment {
    EAlignNone    = 0,
    EAlignLeft    = 1,
    EAlignHCenter = 2,
    EAlignRight   = 3,
    EAlignTop     = 4,
    EAlignVCenter = 8,
    EAlignBottom  = 12,
};

class SDLRenderer {
public:
    SDLRenderer();
    ~SDLRenderer();

    void create(SDL_Window* wnd);
    void destroy();

    void clear();
    void clear(Uint8 red, Uint8 green, Uint8 blue);
    void clear(const SDLColor& color);
    void present();

    void setViewport();
    void setViewport(int x1, int y1, int x2, int y2);
    void setViewport(const SDLRect& rc);

    SDLTexture loadImage(const std::string& file);
    SDLTexture loadText(const std::string& message, const std::string& fontFile, const SDLColor& color, int fontSize);

    void setDrawColor(const SDLColor& color);
    void setDrawColor(Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha = 0xFF);

    void drawPoint(int x, int y);
    void drawPoint(const SDLPoint& pt);
    void drawPoints(const SDLPoint* pts, int cnt);

    void drawLine(int x1, int y1, int x2, int y2);
    void drawLine(const SDLPoint& pt1, const SDLPoint& pt2);
    void drawLines(const SDLPoint* pts, int cnt);

    void drawCurve(int x1, int y1, int x2, int y2, int x3, int y3, int num = 300);
    void drawCurve(const SDLPoint& pt1, const SDLPoint& pt2, const SDLPoint& pt3, int num = 300);
    void drawCurves(const SDLPoint* pts, int cnt, int num = 300);

    void drawRect(const SDLRect& rc);
    void drawRects(const SDLRect* rcs, int cnt);
    void fillRect(const SDLRect& rc);
    void fillRects(const SDLRect* rcs, int cnt);

    void drawTexture(const SDLTexture& tex,
                     const SDLRect& dst);
    void drawTexture(const SDLTexture& tex,
                     int x, int y, int w, int h);
    void drawTexture(const SDLTexture& tex,
                     int x, int y,
                     int align = EAlignLeft | EAlignTop);
    void drawTexture(const SDLTexture& tex,
                     const SDLRect& dst,
                     const SDLRect& clip);
    void drawTexture(const SDLTexture& tex,
                     int x, int y, int w, int h,
                     const SDLRect& clip);
    void drawTexture(const SDLTexture& tex,
                     int x, int y,
                     const SDLRect& clip,
                     int align = EAlignLeft | EAlignTop);

    void drawText(const std::string& message,
                  const std::string& fontFile,
                  const SDLColor& color,
                  int fontSize,
                  int x, int y,
                  int align = EAlignLeft | EAlignTop);

private:
    SDL_Renderer* ren;
};


#endif // SDL_RENDERER_H