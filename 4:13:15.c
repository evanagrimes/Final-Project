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
    int digit;
    int xpos[512];
    int ypos[512];
    int xdir[512];
    int ydir[512];
}snake;

void init_snake(){
    snake.digit = 0;
    snake.xpos[head] = xpos;
    snake.ypos[head] = ypos;
}





void addtosnake(){
    snake.digit += 1;
    //    snake.xpos[snake.digit] = snake.xpos[snake.digit - 1];
    //    snake.ypos[snake.digit] = snake.ypos[snake.digit - 1];
}

void renderBoard(){
    drawRect(applecolor, xapple, yapple, applewidth, applelength);
    //Issue in this for loop
    for(int i = 0; i < snake.digit+1; i++){
        drawRect(blockcolor, snake.xpos[i], snake.ypos[i], blockwidth, blocklength);
        //        printf("digit: %d  xpos: %d   ypos: %d\n",i,snake.xpos[snake.digit],snake.ypos[snake.digit]);
    }
}


void moveleft(){
    steps = 0;
    snake.xdir[head] = -1;
    snake.ydir[head] = 0;
    renderBoard();
}

void moveright(){
    steps = 0;
    snake.xdir[head] = 1;
    snake.ydir[head] = 0;
    renderBoard();
}

void moveup(){
    steps = 0;
    snake.xdir[head] = 0;
    snake.ydir[head] = -1;
    renderBoard();
}

void movedown(){
    steps = 0;
    snake.xdir[head] = 0;
    snake.ydir[head] = 1;
    renderBoard();
}

void stop(){
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
            snake.xpos[i] = snake.xpos[i - 1]-(snake.xdir[i - 1]) * blockwidth;
            snake.ypos[i] = snake.ypos[i - 1]-(snake.ydir[i - 1]) * blocklength;
        }
        if(snake.xdir[head] != snake.xdir[head+1] || snake.ydir[head] != snake.ydir[head+1]){ 
            for(int i = 1; i < snake.digit+1; i++){
                snake.xdir[i] = snake.xdir[i - 1];
                snake.ydir[i] = snake.ydir[i - 1];
            }
        }

        snake.xpos[head] = snake.xpos[head] + (snake.xdir[head])*vel;
        snake.ypos[head] = snake.ypos[head] + (snake.ydir[head])*vel;


        if(iswall()){
            stop();
        }
        if(eatapple()){
            newapple();
            addtosnake();
            eaten++;
        } 


        //        printf("snake.digit:%d\n",snake.digit);
        printf("head.xpos   %d  head.ypos:  %d\n",snake.xpos[head],snake.ypos[head]);
        for(int i = 1; i < snake.digit+1; i++){
            printf("digit(%d).xpos: %d  digit(%d).ypos: %d  xdir: %d  ydir: %d\n",i,snake.xpos[i],i,snake.ypos[i],snake.xdir[i],snake.ydir[i]);
        }
    }

    printf("Eaten: %d\n", eaten);
    printf("Closing program...\n");
    obliterate();

    return 0;
}
