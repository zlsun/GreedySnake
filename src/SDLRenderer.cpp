
#include <cassert>

#include "SDLRenderer.h"
#include "SDLError.h"
#include <zlog.h>


SDLRenderer::SDLRenderer() {
    // ren = nullptr;
}

void SDLRenderer::create(SDL_Window* wnd) {
    zlog "SDLRenderer::create";
    ren = SDL_CreateRenderer(wnd, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (ren == nullptr) {
        throw SDLError("Unable to create renderer: ");
    }
}

SDLRenderer::~SDLRenderer() {
}

void SDLRenderer::destroy() {
    zlog "SDLRenderer::destroy";
    SDL_DestroyRenderer(ren);
}

void SDLRenderer::clear() {
    SDL_RenderClear(ren);
}
void SDLRenderer::clear(Uint8 red, Uint8 green, Uint8 blue) {
    setDrawColor(red, green, blue);
    SDL_RenderClear(ren);
}
void SDLRenderer::clear(const SDLColor& color) {
    setDrawColor(color);
    SDL_RenderClear(ren);
}
void SDLRenderer::present() {
    SDL_RenderPresent(ren);
}

void SDLRenderer::setViewport() {
    SDL_RenderSetViewport(ren, NULL);
}
void SDLRenderer::setViewport(int x1, int y1, int x2, int y2) {
    setViewport(SDLRect(x1, y1, x2, y2));
}
void SDLRenderer::setViewport(const SDLRect& rc) {
    SDL_RenderSetViewport(ren, &rc);
}

SDLTexture SDLRenderer::loadImage(const std::string& file) {
    zlog "SDLRenderer::loadImage", file;
    SDL_Texture* tex = nullptr;
    tex = IMG_LoadTexture(ren, file.c_str());
    if (tex == nullptr)
        throw SDLError("Failed to load image \"" + file);
    return SDLTexture(tex);
}

SDLTexture SDLRenderer::loadText(const std::string& message, const std::string& fontFile, const SDLColor& color, int fontSize) {
    // zlog "loadText", message;
    // Open the font
    TTF_Font* font = nullptr;
    font = TTF_OpenFont(fontFile.c_str(), fontSize);
    if (font == nullptr)
        throw SDLError("Failed to load font: " + fontFile);
    // Render the message to an SDL_Surface, as that's what TTF_RenderText_X returns
    SDL_Surface* surf = TTF_RenderText_Blended(font, message.c_str(), color);
    SDL_Texture* tex = SDL_CreateTextureFromSurface(ren, surf);
    // Clean up unneeded stuff
    SDL_FreeSurface(surf);
    TTF_CloseFont(font);
    return SDLTexture(tex);
}

void SDLRenderer::setDrawColor(const SDLColor& color) {
    setDrawColor(color.r, color.g, color.b, color.a);
}
void SDLRenderer::setDrawColor(Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha) {
    SDL_SetRenderDrawColor(ren, red, green, blue, alpha);
}

void SDLRenderer::drawPoint(int x, int y) {
    SDL_RenderDrawPoint(ren, x, y);
}
void SDLRenderer::drawPoint(const SDLPoint& pt) {
    drawPoint(pt.x, pt.y);
}
void SDLRenderer::drawPoints(const SDLPoint* pts, int cnt) {
    SDL_RenderDrawPoints(ren, pts, cnt);
}

void SDLRenderer::drawLine(int x1, int y1, int x2, int y2) {
    SDL_RenderDrawLine(ren, x1, y1, x2, y2);
}
void SDLRenderer::drawLine(const SDLPoint& pt1, const SDLPoint& pt2) {
    drawLine(pt1.x, pt1.y, pt2.x, pt2.y);
}
void SDLRenderer::drawLines(const SDLPoint* pts, int cnt) {
    SDL_RenderDrawLines(ren, pts, cnt);
}

void SDLRenderer::drawCurve(int x1, int y1, int x2, int y2, int x3, int y3, int num) {
    SDLPoint* pts = new SDLPoint[num];
    double cx = 3 * (x2 - x1);
    double bx = 3 * (x3 - x2) - cx;
    double ax = x3 - x1 - cx - bx;
    double cy = 3 * (y2 - y1);
    double by = 3 * (y3 - y2) - cy;
    double ay = y3 - y1 - cy - by;
    double dt = 1.0 / (num - 1);
    for (int i = 0; i < num; ++i) {
        double t = i * dt;
        double t2 = t * t;
        double t3 = t2 * t;
        pts[i].x = (int)(ax * t3 + bx * t2 + cx * t) + x1;
        pts[i].y = (int)(ay * t3 + by * t2 + cy * t) + y1;
    }
    drawLines(pts, num);
    delete [] pts;
}
void SDLRenderer::drawCurve(const SDLPoint& pt1, const SDLPoint& pt2, const SDLPoint& pt3, int num) {
    drawCurve(pt1.x, pt1.y, pt2.x, pt2.y, pt3.x, pt3.y, num);
}
void SDLRenderer::drawCurves(const SDLPoint* pts, int cnt, int num) {
    assert(cnt >= 3 && cnt % 2 == 1);
    for (int i = 0; i < cnt - 2; i += 2) {
        drawCurve(pts[i], pts[i + 1], pts[i + 2], num);
    }
}

void SDLRenderer::drawRect(const SDLRect& rc) {
    SDL_RenderDrawRect(ren, &rc);
}
void SDLRenderer::drawRects(const SDLRect* rcs, int cnt) {
    SDL_RenderDrawRects(ren, rcs, cnt);
}
void SDLRenderer::fillRect(const SDLRect& rc) {
    // zlog "SDLRenderer::fillRect(", rc, ")";
    SDL_RenderFillRect(ren, &rc);
}
void SDLRenderer::fillRects(const SDLRect* rcs, int cnt) {
    SDL_RenderFillRects(ren, rcs, cnt);
}

void SDLRenderer::drawTexture(const SDLTexture& tex, const SDLRect& dst) {
    // zlog "drawTexture in", dst->x, dst->y;
    SDL_RenderCopy(ren, tex.get(), NULL, &dst);
}
void SDLRenderer::drawTexture(const SDLTexture& tex, int x, int y, int w, int h) {
    SDLRect dst(x, y, w, h);
    drawTexture(tex, dst);
}
void SDLRenderer::drawTexture(const SDLTexture& tex, int x, int y, int align) {
    int w, h;
    tex.query(NULL, NULL, &w, &h);
    if ((align & EAlignRight) == EAlignRight) {
        x -= w;
    } else if ((align & EAlignHCenter) == EAlignHCenter) {
        x -= w / 2;
    }
    if ((align & EAlignBottom) == EAlignBottom) {
        y -= h;
    } else if ((align & EAlignVCenter) == EAlignVCenter) {
        y -= h / 2;
    }
    drawTexture(tex, x, y, w, h);
}
void SDLRenderer::drawTexture(const SDLTexture& tex, const SDLRect& dst, const SDLRect& clip) {
    // zlog "drawTexture in", dst->x, dst->y;
    SDL_RenderCopy(ren, tex.get(), &clip, &dst);
}
void SDLRenderer::drawTexture(const SDLTexture& tex, int x, int y, int w, int h, const SDLRect& clip) {
    SDLRect dst(x, y, w, h);
    drawTexture(tex, dst, clip);
}
void SDLRenderer::drawTexture(const SDLTexture& tex, int x, int y, const SDLRect& clip, int align) {
    int w, h;
    w = clip.w;
    h = clip.h;
    if ((align & EAlignRight) == EAlignRight) {
        x -= w;
    } else if ((align & EAlignHCenter) == EAlignHCenter) {
        x -= w / 2;
    }
    if ((align & EAlignBottom) == EAlignBottom) {
        y -= h;
    } else if ((align & EAlignVCenter) == EAlignVCenter) {
        y -= h / 2;
    }
    drawTexture(tex, x, y, w, h, clip);
}

void SDLRenderer::drawText(const std::string& message, const std::string& fontFile, const SDLColor& color, int fontSize, int x, int y, int align) {
    SDLTexture tex = loadText(message, fontFile, color, fontSize);
    drawTexture(tex, x, y, align);
}
