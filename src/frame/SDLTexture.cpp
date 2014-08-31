
#include "SDLTexture.h"
#include <zlog.h>


SDLTexture::SDLTexture() {

}

SDLTexture::SDLTexture(SDL_Texture *texture): tex(texture, &destroyTexture) {
    zlog "create texture", texture;
}

SDL_Texture *SDLTexture::get() const {
    return tex.get();
}

bool SDLTexture::query(Uint32 *format, int *access, int *w, int *h) const {
    return SDL_QueryTexture(get(), format, access, w, h) == 0;
}

SDLColor SDLTexture::getColorMod() const {
    SDLColor color;
    SDL_GetTextureColorMod(get(), &color.r, &color.g, &color.b);
    return color;
}
void SDLTexture::setColorMod(Uint8 red, Uint8 green, Uint8 blue) {
    SDL_SetTextureColorMod(get(), red, green, blue);
}
void SDLTexture::setColorMod(const SDLColor &color) {
    setColorMod(color.r, color.g, color.b);
}

Uint8 SDLTexture::getAlphaMod() const {
    Uint8 alpha;
    SDL_GetTextureAlphaMod(get(), &alpha);
    return alpha;
}
void SDLTexture::setAlphaMod(Uint8 alpha) {
    SDL_SetTextureAlphaMod(get(), alpha);
}

SDL_BlendMode SDLTexture::getBlendMode() const {
    SDL_BlendMode blend;
    SDL_GetTextureBlendMode(get(), &blend);
    return blend;
}
void SDLTexture::setBlendMode(SDL_BlendMode blend) {
    SDL_SetTextureBlendMode(get(), blend);
}

void SDLTexture::destroyTexture(SDL_Texture *texture) {
    zlog "destroy texture", texture;
    SDL_DestroyTexture(texture);
}