#ifndef SDL_TEXTURE_H
#define SDL_TEXTURE_H

#include <memory>

#include <SDL2/SDL.h>

#include "SDLColor.h"

class SDLTexture {
public:
    typedef std::shared_ptr<SDL_Texture> SDLTexturePtr;

    SDLTexture();
    SDLTexture(SDL_Texture* tex);

    SDL_Texture* get() const;

    bool query(Uint32* format, int* access, int* w, int* h) const;

    SDLColor getColorMod() const;
    void setColorMod(Uint8 red, Uint8 green, Uint8 blue);
    void setColorMod(const SDLColor& color);

    Uint8 getAlphaMod() const;
    void setAlphaMod(Uint8 alpha);

    SDL_BlendMode getBlendMode() const;
    void setBlendMode(SDL_BlendMode blend);
private:
    static void destroyTexture(SDL_Texture* texture);

private:
    SDLTexturePtr tex;
};



#endif // SDL_TEXTURE_H