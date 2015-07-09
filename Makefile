
include common.mk

CXXFLAGS += -g
LDFLAGS +=
LDLIBS += -lstdc++ -lm
LDLIBS += `pkg-config --libs sdl2 SDL2_image SDL2_ttf`

