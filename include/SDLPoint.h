#ifndef SDL_POINT_H
#define SDL_POINT_H

#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_rect.h>  // for SDL_Point


struct SDLPoint: SDL_Point {
    SDLPoint();
    SDLPoint(int x, int y);

    friend std::ostream& operator << (std::ostream& out, const SDLPoint& pt);
};


#endif // SDL_POINT_H