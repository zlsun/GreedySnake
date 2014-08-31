#ifndef SDL_WINDOW_H
#define SDL_WINDOW_H

#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "SDLRect.h"
#include "SDLRenderer.h"


class SDLWindow {
public:
    SDLWindow();
    ~SDLWindow();

    void create(const std::string &title, const SDLRect &rc);
    void mainLoop();
    void close();

    int getTimeout();
    void setTimeout(int tm);

    virtual void initSDL();
    virtual void quitSDL();

    virtual void onKeyDown(SDL_KeyboardEvent *key);
    virtual void onKeyUp(SDL_KeyboardEvent *key);
    virtual void onMouseMotion(SDL_MouseMotionEvent *motion);
    virtual void onMouseButtonDown(SDL_MouseButtonEvent *button);
    virtual void onMouseButtonUp(SDL_MouseButtonEvent *button);
    virtual void onPaint(SDLRenderer *renderer);
    virtual void onQuitEvent(SDL_QuitEvent *quit_event);

    virtual void onCreate();
    virtual void onClose();
    virtual void onIdle();

protected:
    void processEvent(SDL_Event *e);

    static int window_count;
    bool running;
    Uint32 timeout;
    SDL_Window *wnd;
    SDLRenderer renderer;
    SDLRect rect;
};


#endif // SDL_WINDOW_H