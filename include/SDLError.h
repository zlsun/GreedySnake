#ifndef SDL_ERROR_H
#define SDL_ERROR_H

#include <string>
#include <stdexcept>

#include <SDL2/SDL.h>

class SDLError: public std::runtime_error {
public:
    SDLError(std::string info): std::runtime_error(info + ": " + SDL_GetError()) {}
};

#endif // SDL_ERROR_H