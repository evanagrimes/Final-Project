#include <SDL2/SDL.h>
#include "graphics.h"

static Graphics g;

void init_graphics(int x, int y, int width, int height, Color c){
    SDL_Init(SDL_INIT_VIDEO);
    g.win = SDL_CreateWindow("Evan's Snake", x, y, width, height, (Uint32)0);
    g.renderer = SDL_CreateRenderer(g.win, -1, SDL_RENDERER_ACCELERATED);
    g.background = c;
    clear_graphics();
    render_graphics();
}

void obliterate_graphics(){
    SDL_DestroyRenderer(g.renderer);
    SDL_DestroyWindow(g.win);
    SDL_Quit();
}

void drawRect(Color c, int x, int y, int width, int height){
    SDL_SetRenderDrawColor(g.renderer, c.red, c.green, c.blue, (Uint8)255);
    SDL_Rect r = {x, y, width, height};
    SDL_RenderFillRect(g.renderer, &r);
} 

void clear_graphics(){
    SDL_SetRenderDrawColor(g.renderer, g.background.red, 
                            g.background.green, g.background.blue, (Uint8)255);
    SDL_RenderClear(g.renderer);
}

void render_graphics(){
    SDL_RenderPresent(g.renderer);
}

Color color(int r, int g, int b){
    return (Color) { (Uint8)r, (Uint8)g, (Uint8)b};
}

int getKeyB(){
    SDL_Event e;

    while(1){
        while(SDL_PollEvent(&e) != 0){
            if(e.type == SDL_QUIT){
                return -1;
            }

            if(e.type == SDL_KEYDOWN){
                return e.key.keysym.scancode;
            }
        }
    }
}

int getKey(){
    SDL_Event e;

    while(SDL_PollEvent(&e) != 0){
        if(e.type == SDL_QUIT){
            return -1;
        }

        if(e.type == SDL_KEYDOWN){
            return e.key.keysym.scancode;
        }
    }

    return 0;
}

Framerate initFramerate(int fps){
    Framerate f;
    f.start = SDL_GetTicks();
    f.lastTick = SDL_GetTicks();
    f.frames = 0;
    f.fps = fps;
    f.tpf = 100 / fps;
    return f;
}

Framerate capFrame(Framerate f){
    int ticksSince = SDL_GetTicks() - f.lastTick;
    if( ticksSince < f.tpf){
        SDL_Delay( f.tpf - ticksSince); 
    }
    f.frames++;
    f.lastTick = SDL_GetTicks();
    return f;
}

float avgFPS(Framerate f){
    return f.frames / (SDL_GetTicks() - f.start) / 1000.f;
}
