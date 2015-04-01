#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "engine.h"

#define boardwidth 500
#define boardlength 500
#define fps 60
#define blockcolor color(255,0,0)
#define blockwidth 25
#define blocklength 25
#define left 80
#define right 79
#define up 82
#define down 81


int quit = 0;
int key;
int time = 1;

int xvel = 1;
int yvel = 1;

#define xpos  xvel * time
#define ypos  yvel * time

//int cur_xpos = boardwidth / 2;
//int cur_ypos = boardlength / 2;

//int xvel = 1;//unit/time
//int yvel = ypos / time;

void renderBoard(){
    drawRect(blockcolor, xpos, ypos, blockwidth, blocklength);
}

void moveleft(){
    xvel = -1;
    yvel = 1/time;
    renderBoard();
}

void moveright(){
    time = 0;
    xvel = 1;
    yvel = 1/time;
    renderBoard();
}

void moveup(){
    xvel = 0;
    yvel = 1;
    renderBoard();
}

void movedown(){
    xvel = 0;
    yvel = -1;
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
        printf("time: %1d  xpos: %1d  ypos %1d\n",time,xpos,ypos);
    }


    printf("Closing program...\n");
    obliterate();

    return 0;
}
