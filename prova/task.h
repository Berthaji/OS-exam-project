//#pragma once


#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/wait.h>

#include <stdbool.h>
#include <time.h>
#include <signal.h>

#include "Scene.h"
#include "Utils.h"


// void player();
// void game();
// void enemy();

// #define ENEMYSLEEP      150000      //Timer per i nemici

// #define M               4           //Enemy quantity (according to project's specifications)
// #define delaytime       10000       //Delay time in the game

// #define SHOT_PROB       0          //Probabilità di rilasciare la bomba (su 1000)
// #define LIFES           3          //Numero di vite
#define ENEMYSLEEP1 100000
#define ENEMYSLEEP2 100000
#define BOMBSLEEP 80000
#define MISSILESLEEP 80000
#define SHOT_ANGLE_CORRECTION 0.625

#define ASTRODCHOICE    4           //Possibili scelte di sprite per la astronave
#define ENEMYDCHOICE    4           //Possibili scelte di sprite per i nemici
#define ASTRODIM        5           //Dimensioni (in entrambi gli assi) dello sprite astronave
#define ENEMYDIM        3           //Dimensioni (in entrambi gli assi) dello sprite nemico (lv. 1 e lv. 2)

#define ENEMY_GENERATION_WIDTH 0.75     //Percentuala dal bordo di schermo a cui cominciare la generazione
#define ENEMY_GENERATION_DISTANCE_COLUMN 5   //Distanza tra linee di nemici per colonna
#define ENEMY_GENERATION_DISTANCE_LINES 7    //Distanza tra colonne di nemici generati
#define ENEMY_GENERATION_LINES 4            //Numero di linee di nemici



void initProcTask();
void pAstroship(int pipeOut, Object o);
void pEnemy1(int pipeOut, Object o);
void pEnemy2(int pipeOut, Object o);
void pBomb(int pipeOut, Object o);
void pMissile(int pipeOut, Object o);
//void pEngine(int life);
void pEngine(int life, int enemiesdim, int shotProb);


void pEnd( 
    Object *astroship,
    Object *enemies, int enemiesCount,
    Object *enemies2, int enemies2Count,
    Object *missiles, int missilesCount,
    Object *bombs, int bombsCount, int fs[]);   


void pPID( 
    Object *astroship,
    Object *enemies, int enemiesCount,
    Object *enemies2, int enemies2Count,
    Object *missiles, int missilesCount,
    Object *bombs, int bombsCount, 
    int fs[]);


int statusConditions(bool life,
    Object *enemies1, int enemies1Count,
    Object *enemies2, int enemies2Count);



