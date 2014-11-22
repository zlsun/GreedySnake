#include <zlog.h>

#include "SDLWindow.h"
#include "SDLError.h"


int SDLWindow::window_count = 0;

SDLWindow::SDLWindow(): timeout(10) {
    zlog "SDLWindow::SDLWindow()";
    if (window_count++ == 0) {
        initSDL();
    }
}

SDLWindow::~SDLWindow() {
    zlog "SDLWindow::~SDLWindow()";
    if (--window_count == 0) {
        quitSDL();
    }
}

void SDLWindow::initSDL() {
    zlog "SDLWindow::initSDL";
    if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO) < 0 ) {
        throw SDLError("Unable to init SDL");
    }
    if (TTF_Init() < 0) {
        throw SDLError("Unable to init TTF");
    }
}

void SDLWindow::quitSDL() {
    zlog "SDLWindow::quitSDL";
    TTF_Quit();
    SDL_Quit();
}

void SDLWindow::create(const std::string& title, const SDLRect& rc) {
    zlog "SDLWindow::create";
    rect = rc;
    wnd = SDL_CreateWindow(title.c_str(), rc.x, rc.y, rc.w, rc.h, SDL_WINDOW_SHOWN);
    if (wnd == nullptr) {
        throw SDLError("Unable to create window: ");
    }
    renderer.create(wnd);
    onCreate();
}

void SDLWindow::processEvent(SDL_Event* e) {
    switch (e->type) {
        case SDL_QUIT:
            onQuitEvent(&e->quit);
            break;
        case SDL_KEYDOWN:
            onKeyDown(&e->key);
            break;
        case SDL_KEYUP:
            onKeyUp(&e->key);
            break;
        case SDL_MOUSEMOTION:
            onMouseMotion(&e->motion);
            break;
        case SDL_MOUSEBUTTONDOWN:
            onMouseButtonDown(&e->button);
            break;
        case SDL_MOUSEBUTTONUP:
            onMouseButtonUp(&e->button);
            break;
        default:
            break;
    }
}

void SDLWindow::mainLoop() {
    zlog "SDLWindow::mainLoop";

    SDL_Event e;
    Uint32 start_time, pass_time;
    running = true;
    zlog "mainLoop start";
    while (running) {
        start_time = SDL_GetTicks();
        while (SDL_PollEvent(&e)) {
            processEvent(&e);
        }
        onIdle();
        onPaint(&renderer);
        pass_time = SDL_GetTicks() - start_time;
        SDL_Delay(pass_time > timeout ? 0 : timeout - pass_time);
    }
    zlog "mainLoop end";

    onClose();
    renderer.destroy();
    SDL_DestroyWindow(wnd);
}

void SDLWindow::close() {
    zlog "SDLWindow::close";
    running = false;
}

int SDLWindow::getTimeout() {
    return timeout;
}
void SDLWindow::setTimeout(int tm) {
    timeout = tm;
}

void SDLWindow::onKeyDown(SDL_KeyboardEvent* key) {}
void SDLWindow::onKeyUp(SDL_KeyboardEvent* key) {}
void SDLWindow::onMouseMotion(SDL_MouseMotionEvent* motion) {}
void SDLWindow::onMouseButtonDown(SDL_MouseButtonEvent* button) {}
void SDLWindow::onMouseButtonUp(SDL_MouseButtonEvent* button) {}
void SDLWindow::onPaint(SDLRenderer* renderer) {}
void SDLWindow::onQuitEvent(SDL_QuitEvent* quit) {}

void SDLWindow::onCreate() {}
void SDLWindow::onClose() {}
void SDLWindow::onIdle() {}
