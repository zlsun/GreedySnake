#ifndef SDL_COLOR_H
#define SDL_COLOR_H

#include <iostream>
#include <iomanip>

#include <SDL2/SDL.h>
#include <SDL2/SDL_pixels.h>    // for SDL_Color

struct SDLColor: SDL_Color {
    SDLColor();
    SDLColor(Uint32 color);
    SDLColor(Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha = 0xFF);

    bool operator == (const SDL_Color &other) const;

    friend std::ostream &operator << (std::ostream &out, const SDLColor &c);
};


#endif // SDL_COLOR_H