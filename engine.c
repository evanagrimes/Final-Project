#include <SDL2/SDL.h>
#include "engine.h"
#define NEVENTS 100

int numListeners = 0;
Listener listeners[NEVENTS];
Engine eng;
long lastFrame;

void registerKey(void (*kcb) (int)){
    listeners[numListeners].callback.keyListener = kcb;
    listeners[numListeners].type = KEY;
    numListeners++;
}

void drawRect(Color c, int x, int y, int width, int height){
    SDL_SetRenderDrawColor(eng.renderer, c.red, c.green, c.blue, 255);
    SDL_Rect r = {x, y, width, height};
    SDL_RenderFillRect(eng.renderer, &r);
}

void clear(){
    SDL_SetRenderDrawColor(eng.renderer, eng.background.red, 
                            eng.background.green, eng.background.blue, 255);
    SDL_RenderClear(eng.renderer);
}


bool stepEngine(){
    int i;
    SDL_Event e;
    clear();

    while(SDL_PollEvent(&e) != 0){
        if(e.type == SDL_QUIT){
            return false;
        }
        for(i = 0; i<numListeners; i++){
            if(e.type == SDL_KEYDOWN && listeners[i].type == KEY){
                listeners[i].callback.keyListener(e.key.keysym.scancode);
                continue;
            }
        }
    }

    eng.render();
    
    //Cap FPS
    long interval = SDL_GetTicks() - lastFrame;
    if(interval < eng.mspf){
        SDL_Delay(eng.mspf - interval);
    }
    lastFrame = SDL_GetTicks();

       
    SDL_RenderPresent(eng.renderer);

    return true;
}

void initEngine(int x, int y, int width, int height, Color c, int fps, void (*render)()){

    SDL_Init(SDL_INIT_VIDEO);
    eng.win = SDL_CreateWindow("Simple Snake", x, y, width, height, 0);
    eng.renderer = SDL_CreateRenderer(eng.win, -1, SDL_RENDERER_ACCELERATED);
    eng.background = c;
    eng.fps = fps;
    eng.mspf = 1000 / fps;
    eng.render = render;
    lastFrame = SDL_GetTicks();
}

void obliterate(){
    SDL_DestroyRenderer(eng.renderer);
    SDL_DestroyWindow(eng.win);
    SDL_Quit();
}

Color color(int r, int g, int b){
    return (Color) { r, g, b};
}
