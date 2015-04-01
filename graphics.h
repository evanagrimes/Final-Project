#ifndef FOTUS_GRAPHICS
#define FOTUS_GRAPHICS

#include <SDL2/SDL.h>

typedef struct {
    Uint8 red;
    Uint8 green;
    Uint8 blue;
} Color;

typedef struct {
    SDL_Window *win;
    SDL_Renderer *renderer;
    Color background;
} Graphics;

typedef struct {
    int start;
    int lastTick;
    int frames;
    int fps;
    int tpf;
} Framerate;

void init_graphics(int x, int y, int width, int height, Color c);

void obliterate_graphics();
void render_graphics();

void drawRect(Color c, int x, int y, int width, int height); 

void clear_graphics();

Color color(int r, int g, int b);

int getKeyB();
int getKey();

Framerate initFramerate(int fps);
Framerate capFrame(Framerate f);
float avgFPS(Framerate f);

#endif
