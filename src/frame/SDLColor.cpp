
#include "SDLColor.h"
#include <zlog.h>


SDLColor::SDLColor() {
    r = 0x00;
    g = 0x00;
    b = 0x00;
    a = 0xFF;
}

SDLColor::SDLColor(Uint32 color) {
    r = (color >> 16) & 0xFF;
    g = (color >> 8) & 0xFF;
    b = color & 0xFF;
    a = (color >> 24) & 0xFF;
}

SDLColor::SDLColor(Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha) {
    r = red;
    g = green;
    b = blue;
    a = alpha;
}

bool SDLColor::operator == (const SDL_Color &other) const {
    return r == other.r && g == other.g && b == other.b && a == other.a;
}

std::ostream &operator << (std::ostream &out, const SDLColor &c) {
    // out << std::hex << std::setiosflags(std::ios::showbase|std::ios::uppercase);
    out << "SDLColor(" << (int)c.r << ", " << (int)c.g << ", " << (int)c.b << ", " << (int)c.a << ")";
    // out << std::dec << std::resetiosflags(std::ios::showbase|std::ios::uppercase);
    return out;
}
