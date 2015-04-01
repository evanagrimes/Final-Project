#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "engine.h"

#define boardwidth 500
#define boardlength 500
#define fps 60
#define blockcolor color(0,255,0)
#define blockwidth 25
#define blocklength 25
#define left 80
#define right 79
#define up 82
#define down 81


int quit = 0;
int key;
int xdir;
int ydir;
int time = 1;

int vel = 1;

#define ticks  vel * time

//int xpos = boardwidth / 2;
//int ypos = boardlength / 2;

int xpos = xpos + (xdir)*ticks;
int ypos = ypos + (ydir)*ticks;


void renderBoard(){
    drawRect(blockcolor, xpos, ypos, blockwidth, blocklength);
}

void moveleft(){
    time = 0;
    xdir = -1;
    ydir = 0;
    renderBoard();
}

void moveright(){
    time = 0;
    xdir = 1;
    ydir = 0;
    renderBoard();
}

void moveup(){
    time = 0;
    xdir = 0;
    ydir = -1;
    renderBoard();
}

void movedown(){
    time = 0;
    xdir = 0;
    ydir = 1;
    renderBoard();
}


void keydown(int key){
    printf("Key: %i\n", key);

    if(key == left)
        moveleft();
    if(key == right)
        moveright();
    if(key == up)
        moveup();
    if(key == down)
        movedown();


}



int main(int argc, char *args[]) {
    printf("Start test...\n");

    initEngine(0, 0, boardwidth, boardlength, color(255,255,255), fps, &renderBoard);

    registerKey(&keydown);
    while(stepEngine()){
        time += 1;
        //        time = time/60;
        printf("time: %1d  xpos: %1d  ypos: %1d  ticks:  %1d\n",time,xpos,ypos,ticks);
    }


    printf("Closing program...\n");
    obliterate();

    return 0;
}
