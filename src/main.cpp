
#include <iostream>
#include <stdexcept>
#include <cstdio>
#include <cstdlib>

#include <zlog.h>

#include "GameWindow.h"
#include "SDLError.h"

int main(int argc, char* argv[]) {
    zlog "start";
    GameWindow wnd;
    wnd.create("Greedy Snake", SDLRect(SCREEN_X, SCREEN_Y, SCREEN_WIDTH, SCREEN_HEIGHT));
    wnd.mainLoop();
    return 0;
}
