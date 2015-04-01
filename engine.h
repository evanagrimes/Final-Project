#ifndef FOTUS_GRAPHICS
#define FOTUS_GRAPHICS
#include <SDL2/SDL.h>
#include <stdbool.h>

typedef struct {
    int red;
    int green;
    int blue;
} Color;

typedef struct {
    SDL_Window *win;
    SDL_Renderer *renderer;
    Color background;
    int fps;
    int mspf;
    void (*render)();
} Engine;


typedef enum {KEY, MOUSE} FType;

typedef struct{
    union {
        void (*keyListener)(int);
        void (*mouseListener)(int,int);
    } callback;
    FType type;
} Listener;

void initEngine(int x, int y, int width, int height, Color c, int fps, void (*)());
void obliterate();
Color color(int r, int g, int b);
bool stepEngine();
void drawRect(Color c, int x, int y, int width, int height);

void registerKey(void (*f) (int));
void registerMouse(void (*f) (int,int));
#endif
