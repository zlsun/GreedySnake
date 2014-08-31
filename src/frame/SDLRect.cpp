
#include "SDLRect.h"


SDLRect::SDLRect() {
    x = 0;
    y = 0;
    w = 0;
    h = 0;
}
SDLRect::SDLRect(int _w, int _h) {
    x = 0;
    y = 0;
    w = _w;
    h = _h;
}
SDLRect::SDLRect(int _x, int _y, int _w, int _h) {
    x = _x;
    y = _y;
    w = _w;
    h = _h;
}

bool SDLRect::isEmpty() const {
    return (w <= 0) || (h <= 0);
}

bool SDLRect::hasIntersection(const SDLRect &rhs) const {
    return SDL_HasIntersection(this, &rhs);
}

SDLRect SDLRect::intersectRect(const SDLRect &rhs) const {
    SDLRect result;
    if (SDL_IntersectRect(this, &rhs, &result)) {
        return result;
    } else {
        return SDLRect();
    }
}

SDLRect SDLRect::unionRect(const SDLRect &rhs) const {
    SDLRect result;
    SDL_UnionRect(this, &rhs, &result);
    return result;
}

bool SDLRect::operator == (const SDLRect &rhs) const {
    return (x == rhs.x) && (y == rhs.y) && (w == rhs.w) && (h == rhs.h);
}

std::ostream &operator << (std::ostream &out, const SDLRect &rc) {
    out << "SDLRect(" << rc.x << ", " << rc.y << ", " << rc.w << ", " << rc.h << ")";
    return out;
}

SDLRect enclosePoints(const SDLPoint *pts, int cnt) {
    SDLRect result;
    if (SDL_EnclosePoints(pts, cnt, NULL, &result)) {
        return result;
    } else {
        return SDLRect();
    }
}

SDLRect enclosePoints(const SDLPoint *pts, int cnt, const SDLRect &clip) {
    SDLRect result;
    if (SDL_EnclosePoints(pts, cnt, &clip, &result)) {
        return result;
    } else {
        return SDLRect();
    }
}
