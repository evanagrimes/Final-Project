/* **********************************************************
 * Simple Snake Game                                        *
 * Created by Evan Grimes                                   *
 * Under instruction from Peter Miller and David Housman    *
 * For Programming I at Goshen College                      *
 * Spring 2015                                              *
 ************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "engine.h"

//Macros
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

#define MAX_length 100


//Global Variables
bool end = false;//ends the game
int key;//stores keydown event information

int wallpos[512][512];//array for storing level information from command line unput

int xapple;
int yapple;

int xpos = ((boardwidth / 2)/blockwidth)*blockwidth;
int ypos = ((boardlength / 2)/blocklength)*blocklength;

int wallxpos[512];//holds location information to draw walls
int wallypos[512];//holds location information to draw walls
int length = 0;

//Snake structure
struct node {
    int length;
    int xpos[512];
    int ypos[512];
    int xdir[512];
    int ydir[512];
};

struct node *snake;


//Function Declarations
void init_snake();//..........sets the intial values for the snake structure
void drawlevel();//...........draws the walls sepcified for individual levels
void shift();//...............shifts the nodes of the snake down the snake structure to make room for new head position
void walk();//................makes the snake move in a constant direction set by the direction functions
void addtosnake();//..........adds three nodes to the end of the snake
void renderBoard();//.........draws the various aspects of the board (snake,walls,apple)
void moveleft();//............changes the direction of the head to the left
void moveright();//...........changes the direction of the head to the right
void moveup();//..............changes the direction of the head upward
void movedown();//............changes the direction of the head downward
void stop();//................ends the game
bool iswall();//..............detects if the head has collided with a wall
bool eatapple();//............detects if the head has eaten an apple
void keydown(int key);//......registers a keydown event to a global variable
void newapple();//............generates a new apple in a random location
bool eatsnake();//............detects if the snake has collided with itself
void loadlevel(char *y);//....load the level from command line input


//Functions
void init_snake(){
    snake->length = 5;
    for(int i=snake->length; i>snake->length-5; i--)
        snake->xpos[i] = -50;
    snake->xpos[head] = xpos;
    snake->ypos[head] = ypos;
}

void drawlevel(){
    for(int i=0; i<length; i++){
        drawRect(wallcolor, wallxpos[i], wallypos[i], wallwidth, walllength);
    }
}


void shift(){
    for(int i=snake->length-1; i > 0; i--){  
        snake->xpos[i] = snake->xpos[i-1];
        snake->ypos[i] = snake->ypos[i-1];
    }
}

void walk(){
    shift();
    snake->xpos[head] = snake->xpos[head] + snake->xdir[head]*blockwidth;
    snake->ypos[head] = snake->ypos[head] + snake->ydir[head]*blocklength;
}


void addtosnake(){
    snake->length += 3;
    for(int i=snake->length; i>snake->length-3; i--)
        snake->xpos[i] = -50;
    snake->xpos[snake->length-1] = snake->xpos[snake->length - 2];
    snake->ypos[snake->length-1] = snake->ypos[snake->length - 2];
}

void renderBoard(){
    drawlevel();
    drawRect(applecolor, xapple, yapple, applewidth, applelength);
    drawRect(blockheadcolor, snake->xpos[head], snake->ypos[head], blockwidth, blocklength);
    for(int i = 1; i < snake->length; i++){
        drawRect(blockcolor, snake->xpos[i], snake->ypos[i], blockwidth, blocklength);
    }
}


void moveleft(){
    if(snake->xdir[head] != 1){
        snake->xdir[head] = -1;
        snake->ydir[head] = 0;
        shift();
        renderBoard();
    }
}

void moveright(){
    if(snake->xdir[head] != -1){
        shift();
        snake->xdir[head] = 1;
        snake->ydir[head] = 0;
        renderBoard();
    }
}

void moveup(){
    if(snake->ydir[head] != 1){
        shift();
        snake->xdir[head] = 0;
        snake->ydir[head] = -1;
        renderBoard();
    }
}

void movedown(){
    if(snake->ydir[head] != -1){
        shift();
        snake->xdir[head] = 0;
        snake->ydir[head] = 1;
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
    if(snake->xpos[head] >= boardwidth - blockwidth)
        return true;
    if(snake->ypos[head]  >= boardlength - blocklength)
        return true;
    if(snake->xpos[head] <= 0)
        return true;
    if(snake->ypos[head]  <= 0)
        return true;
    if(wallpos[snake->xpos[head]][snake->ypos[head]] == 7)
        return true; 
    return false;
}

bool eatapple(){
    if(snake->xpos[head] == xapple && snake->ypos[head] == yapple)
        return true;
    return false;
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
}

bool eatsnake(){
    for(int j = 1; j < snake->length; j++){
        if(snake->xpos[head] == snake->xpos[j] && snake->ypos[head] == snake->ypos[j]){
            return true;
        }
    }
    return false;
}

void loadlevel(char *y){
    int x,flag=0;
    char *file = y;
    FILE *fp = fopen(file,"r");
    if(ferror(fp) || fp == NULL){
        fclose(fp);
        printf("Level file error\n");
        flag = -1;
    }
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




int main(int argc, char *argv[]) {
    snake = malloc(MAX_length * sizeof(struct node));
    if (snake == NULL) {
        printf("Cant allocate enough space.\n");
        exit(0);
    }

    if(argv[1] != NULL){
        loadlevel(argv[1]);
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
        if(eaten > 25){
            printf("You Win! Move on to the next level.\n");
            stop();
        }
    }

    SDL_Delay(700);


    printf("Eaten: %d\n", eaten);
    printf("Closing program...\n");
    obliterate();

    return 0;
}
