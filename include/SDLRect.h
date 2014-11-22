#ifndef SDL_RECT_H
#define SDL_RECT_H

#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_rect.h>  // for SDL_Rect

#include "SDLPoint.h"


struct SDLRect: SDL_Rect {
    SDLRect();
    SDLRect(int w, int h);
    SDLRect(int x, int y, int w, int h);

    bool isEmpty() const;
    bool hasIntersection(const SDLRect& rhs) const;

    SDLRect intersectRect(const SDLRect& rhs) const;
    SDLRect unionRect(const SDLRect& rhs) const;

    bool operator == (const SDLRect& rhs) const;

    friend std::ostream& operator << (std::ostream& out, const SDLRect& rc);
};

SDLRect enclosePoints(const SDLPoint* pts, int cnt);

SDLRect enclosePoints(const SDLPoint* pts, int cnt, const SDLRect& clip);


#endif // SDL_RECT_H