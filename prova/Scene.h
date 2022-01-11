#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <sys/wait.h>


#include "Structs.h"
#include "task.h"

#include <ncurses.h>

//Screen resolution
// #define SCREEN_X        80          //X-axis screen resolution
// #define SCREEN_Y        120         //Y-axis screen resolution

// #define ASTRODCHOICE    4           //Possible astroship sprites
// #define ENEMYDCHOICE    4           //Possible enemies sprites
// #define ASTRODIM        5           //Dimensions (on both axis) of astroship sprite
// #define ENEMYDIM        3           //Dimensions (on both axis) of enemy sprite

// #define M               4           //Enemy quantity (according to project's specifications)
// #define delaytime       10000       //Delay time in the game

// #define SHOT_PROB       0          //Probabilità di rilasciare la bomba (su 1000)
// #define LIFES           3          //Numero di vite



#define SCREEN_W getmaxx(stdscr)
#define SCREEN_H getmaxy(stdscr)


void initScreen();
void clearScreen();


void drawScene(
    Object *astroship,
    Object *enemies, int enemiesCount, 
    Object *enemies2, int enemies2Count, 
    Object *missiles, int missilesCount,
    Object *bombs, int bombsCount
);

void drawObject(Object entity);


bool gEnemy1(Object * o, bool bo);
bool gEnemy2(Object * o, bool bo);
bool gMissile(Object * o, bool bo);
bool gBomb(Object * o, bool bo);

