#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "engine.h"

#define boardwidth 500
#define boardlength 500
#define fps 15
#define blockcolor color(0,255,0)
#define blockheadcolor color(0,0,255)
#define wallcolor color(0,0,0)
#define wallwidth 15
#define walllength 15
#define blockwidth 15
#define blocklength 15
#define applecolor color(255,0,0)
#define applewidth 15
#define applelength 15
#define left 80
#define right 79
#define up 82
#define down 81
#define head 0

bool end = false;
int key;
int wallpos[512][512];

int xapple;
int yapple;

int xpos = ((boardwidth / 2)/blockwidth)*blockwidth;
int ypos = ((boardlength / 2)/blocklength)*blocklength;

int wallxpos[512];
int wallypos[512];
int length = 0;


struct stack {
    int length;
    int xpos[512];
    int ypos[512];
    int xdir[512];
    int ydir[512];
}snake;

void init_snake(){
    snake.length = 5;
    snake.xpos[head] = xpos;
    snake.ypos[head] = ypos;
}

void drawlevel(){
    for(int i=0; i<length; i++){
        drawRect(wallcolor, wallxpos[i], wallypos[i], wallwidth, walllength);
    }
}


void shift(){
    for(int i=snake.length-1; i > 0; i--){  
        snake.xpos[i] = snake.xpos[i-1];
        snake.ypos[i] = snake.ypos[i-1];
    }
}

void walk(){
    shift();
    snake.xpos[head] = snake.xpos[head] + snake.xdir[head]*blockwidth;
    snake.ypos[head] = snake.ypos[head] + snake.ydir[head]*blocklength;
}


void addtosnake(){
    snake.length += 1;
    snake.xpos[snake.length-1] = snake.xpos[snake.length - 2];
    snake.ypos[snake.length-1] = snake.ypos[snake.length - 2];
}

void renderBoard(){
    drawlevel();
    drawRect(applecolor, xapple, yapple, applewidth, applelength);
    drawRect(blockheadcolor, snake.xpos[head], snake.ypos[head], blockwidth, blocklength);
    for(int i = 1; i < snake.length; i++){
        drawRect(blockcolor, snake.xpos[i], snake.ypos[i], blockwidth, blocklength);
        //        printf("length: %d  xpos: %d   ypos: %d\n",i,snake.xpos[snake.length],snake.ypos[snake.length]);
    }
}


void moveleft(){
    if(snake.xdir[head] != 1){
        snake.xdir[head] = -1;
        snake.ydir[head] = 0;
        shift();
        renderBoard();
    }
}

void moveright(){
    if(snake.xdir[head] != -1){
        shift();
        snake.xdir[head] = 1;
        snake.ydir[head] = 0;
        renderBoard();
    }
}

void moveup(){
    if(snake.ydir[head] != 1){
        shift();
        snake.xdir[head] = 0;
        snake.ydir[head] = -1;
        renderBoard();
    }
}

void movedown(){
    if(snake.ydir[head] != -1){
        shift();
        snake.xdir[head] = 0;
        snake.ydir[head] = 1;
        renderBoard();
    }
}

void stop(){
    end = true;
}

bool iswall(){
    for(int i=0; i<length; i++){
        wallpos[wallxpos[i]][wallypos[i]] = 7;
    }
    if(snake.xpos[head] >= boardwidth - blockwidth)
        return true;
    if(snake.ypos[head]  >= boardlength - blocklength)
        return true;
    if(snake.xpos[head] <= 0)
        return true;
    if(snake.ypos[head]  <= 0)
        return true;
    if(wallpos[snake.xpos[head]][snake.ypos[head]] == 7)
        return true; 
    return false;
}

bool eatapple(){
    if(snake.xpos[head] == xapple && snake.ypos[head] == yapple)
        return true;
    return false;
    //    printf("head.xpos: %d   xapple: %d   head.ypos: %d  yapple: %d\n",snake.xpos[head],xapple,snake.ypos[head],yapple);
}

void keydown(int key){
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
    xapple = ((rand() % (boardwidth-blockwidth))/blockwidth)*blockwidth+blockwidth;
    yapple = ((rand() % (boardlength-blocklength))/blocklength)*blocklength+blocklength;
    if(wallpos[xapple][yapple] == 7)
        newapple();
    //    printf("xapple:  %d   yapple:  %d\n",xapple,yapple);
}

bool eatsnake(){
    for(int j = 1; j < snake.length; j++){
        if(snake.xpos[head] == snake.xpos[j] && snake.ypos[head] == snake.ypos[j]){
            return true;
        }
    }
    return false;
}



int main(int argc, char *argv[]) {
    int x,flag=0;
    if(argv[1] != NULL){
        char *file = argv[1];
        FILE *fp = fopen(file,"r");
        if(ferror(fp) || fp == NULL){
            fclose(fp);
            printf("Level file error\n");
            flag = -1;
        }
        printf("W\n");
        if(flag != -1){
            while((x = getc(fp)) != EOF){
                if(x == ':')
                    while((x = getc(fp)) != '*'){
                        fscanf(fp, "%d", &wallxpos[length]); 
                        fscanf(fp, "%d", &wallypos[length]); 
                        length++;
                    }
            }
        }
    }

    srand (time(NULL));
    printf("Start test...\n");

    initEngine(0, 0, boardwidth, boardlength, color(255,255,255), fps, &renderBoard);
    init_snake();

    newapple();
    registerKey(&keydown);
    int eaten = 0;

    while(stepEngine() && end == false){
        walk();
        if(eaten > 1){  
            if(eatsnake())
                stop();
        }
        if(iswall())
            stop();

        if(eatapple()){
            newapple();
            addtosnake();
            eaten++;
        } 
    }

    SDL_Delay(700);


    printf("Eaten: %d\n", eaten);
    printf("Closing program...\n");
    obliterate();

    return 0;
}
