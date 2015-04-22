#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "engine.h"

#define boardwidth 500
#define boardlength 500
#define fps 120
#define blockcolor color(0,255,0)
#define blockwidth 25
#define blocklength 25
#define applecolor color(255,0,0)
#define applewidth 10
#define applelength 10
#define left 80
#define right 79
#define up 82
#define down 81
#define head 0
#define ticks  vel * steps


bool end = false;
int key;
int xdir;
int ydir;
int steps = 1;
int vel = 1;

int xapple;
int yapple;

int xpos = boardwidth / 2;
int ypos = boardlength / 2;



struct stack {
    //    int link[512];
    int digit;
    int xpos[512];
    int ypos[512];
}snake;

void init_snake(){
    snake.digit = 0;
    //    snake.link[snake.digit] = 1;
    snake.xpos[head] = xpos;
    snake.ypos[head] = ypos;
}





void addtosnake(){
    snake.digit += 1;
    //    snake.link[snake.digit] = 1;
    //    snake.xpos[snake.digit] = snake.xpos[snake.digit - 1];
    //    snake.ypos[snake.digit] = snake.ypos[snake.digit - 1];
}

void renderBoard(){
    drawRect(applecolor, xapple, yapple, applewidth, applelength);
    drawRect(blockcolor, snake.xpos[head], snake.ypos[head], blockwidth, blocklength);
    //Issue in this for loop
    for(int i = 0; i < snake.digit+1; i+=1){
        drawRect(blockcolor, snake.xpos[snake.digit], snake.ypos[snake.digit], blockwidth, blocklength);
//        printf("digit: %d  xpos: %d   ypos: %d\n",i,snake.xpos[snake.digit],snake.ypos[snake.digit]);
    }
}


void moveleft(){
    steps = 0;
    xdir = -1;
    ydir = 0;
    renderBoard();
}

void moveright(){
    steps = 0;
    xdir = 1;
    ydir = 0;
    renderBoard();
}

void moveup(){
    steps = 0;
    xdir = 0;
    ydir = -1;
    renderBoard();
}

void movedown(){
    steps = 0;
    xdir = 0;
    ydir = 1;
    renderBoard();
}

void stop(){
    xdir = 0;
    ydir = 0;
    end = true;
}

bool iswall(){
    if(snake.xpos[head] >= boardwidth - blockwidth)
        return true;
    if(snake.ypos[head]  >= boardlength - blocklength)
        return true;
    if(snake.xpos[head] <= 0)
        return true;
    if(snake.ypos[head]  <= 0)
        return true;
    return false;
}

bool eatapple(){
    if((snake.xpos[head] < xapple) && (xapple + applewidth < snake.xpos[head] + blockwidth)){
        if((snake.ypos[head] < yapple) && (yapple + applewidth < snake.ypos[head] + blocklength))
            return true;
        else if((snake.ypos[head] < yapple + applelength) && (yapple + applelength < snake.ypos[head] + blocklength))
            return true;
    }
    //Error somewhere in the next 10 lines
    if((snake.xpos[head] < xapple + applewidth) && (xapple + applewidth < snake.xpos[head] + blockwidth)){
        if((snake.ypos[head] < yapple) && (yapple + applewidth < snake.ypos[head] + blocklength))
            return true;
        else if((snake.ypos[head] < yapple + applelength) && (yapple + applelength < snake.ypos[head] + blocklength))
            return true;
    }
    return false;

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


void newapple(){
    xapple = (rand() + snake.xpos[head]) % (boardwidth - blockwidth/2);
    yapple = (rand() + snake.ypos[head]) % (boardlength - blocklength/2);
}




int main(int argc, char *args[]) {
    int eaten = 0;


    srand (time(NULL));
    printf("Start test...\n");

    initEngine(0, 0, boardwidth, boardlength, color(255,255,255), fps, &renderBoard);
    init_snake();

    newapple();
    registerKey(&keydown);

    while(stepEngine() && end == false){
        for(int i = 1; i < snake.digit+1; i++){
            snake.xpos[snake.digit] = snake.xpos[snake.digit - 1]+blockwidth;
            snake.ypos[snake.digit] = snake.ypos[snake.digit - 1]+blocklength;
        } 
        
        
        snake.xpos[head] = snake.xpos[head] + (xdir)*vel;
        snake.ypos[head] = snake.ypos[head] + (ydir)*vel;


        if(iswall()){
            stop();
        }
        if(eatapple()){
            newapple();
            addtosnake();
            eaten++;
        }
        steps += 1;

        printf("digit.xpos: %d  digit.ypos: %d\n",snake.xpos[snake.digit],snake.ypos[snake.digit]);
        printf("head.xpos %d  head.ypos:  %d\n",snake.xpos[head],snake.ypos[head]);
    }

    printf("Eaten: %d\n", eaten);
    printf("Closing program...\n");
    obliterate();

    return 0;
}
