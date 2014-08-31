
#include "SDLPoint.h"


SDLPoint::SDLPoint() {
    x = 0;
    y = 0;
}

SDLPoint::SDLPoint(int _x, int _y) {
    x = _x;
    y = _y;
}

std::ostream &operator << (std::ostream &out, const SDLPoint &pt) {
    out << "SDLPoint(" << pt.x << ", " << pt.y << ")";
    return out;
}
