
include common.mk

CXXFLAGS += -g
LDFLAGS += -mwindows
LDLIBS += -lstdc++ -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf
