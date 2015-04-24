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
struct snake{
    int length;
    int xpos;
    int ypos;
    int xdir;
    int ydir;
    struct snake *next;
};

struct snake *head = NULL;


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
    struct snake *cur, *prev, *next, *new_snake;
    new_snake = malloc(sizeof(struct snake));
    new_snake->length = 5;
    while(cur != NULL){
        cur->xpos = -50;
        cur = cur->next;
    }
    new_snake->xpos = xpos;

    head = new_snake;
}

void drawlevel(){
    for(int i=0; i<length; i++){
        drawRect(wallcolor, wallxpos[i], wallypos[i], wallwidth, walllength);
    }
}


void shift(){
    struct snake *cur, *prev, *next;
    //    prev = cur->prev;
    while(cur != NULL){
        prev = cur;
        cur = cur->next;    
        cur->xpos = prev->xpos;
        cur->ypos = prev->ypos;
    }
}

void walk(){
    struct snake *cur, *prev, *next;
    shift();
    head->xpos = head->xpos + head->xdir*blockwidth;
    head->ypos = head->ypos + head->ydir*blocklength;
}


void addtosnake(){
    struct snake *cur, *prev, *next, *new_snake;
    new_snake = malloc(sizeof(struct snake));

    cur = head -> next;
    while(cur -> next != NULL)
        cur = cur -> next;
    //    snake->length += 3;
    new_snake->xpos = cur->xpos;
    new_snake->ypos = cur->ypos;
    next = new_snake;
}

void renderBoard(){
    struct snake *cur,*prev,*next;
    drawlevel();
    drawRect(applecolor, xapple, yapple, applewidth, applelength);
    drawRect(blockheadcolor, head->xpos, head->ypos, blockwidth, blocklength);
    while(cur != NULL){
        drawRect(blockcolor, cur->xpos, cur->ypos, blockwidth, blocklength);
        cur = cur-> next;
    }
}


void moveleft(){
    if(head->xdir != 1){
        head->xdir = -1;
        head->ydir = 0;
        shift();
        renderBoard();
    }
}

void moveright(){
    if(head->xdir != -1){
        shift();
        head->xdir = 1;
        head->ydir = 0;
        renderBoard();
    }
}

void moveup(){
    if(head->ydir != 1){
        shift();
        head->xdir = 0;
        head->ydir = -1;
        renderBoard();
    }
}

void movedown(){
    if(head->ydir != -1){
        shift();
        head->xdir = 0;
        head->ydir = 1;
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
    if(head->xpos >= boardwidth - blockwidth)
        return true;
    if(head->ypos  >= boardlength - blocklength)
        return true;
    if(head->xpos <= 0)
        return true;
    if(head->ypos  <= 0)
        return true;
    if(wallpos[head->xpos][head->ypos] == 7)
        return true; 
    return false;
}

bool eatapple(){
    if(head->xpos == xapple && head->ypos == yapple)
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
    struct snake *cur, *next, *prev;
    cur = head -> next;
    for(cur = head->next; cur != NULL; cur = cur->next){
        if(head->xpos == cur->xpos && head->ypos == cur->ypos){
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
