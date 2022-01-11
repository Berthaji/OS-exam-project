#pragma once

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

#include "scene.h"



#define ENEMYSLEEP1 100000
#define ENEMYSLEEP2 100000
#define BOMBSLEEP 80000
#define MISSILESLEEP 80000
#define SHOT_ANGLE_CORRECTION 0.625


#define ENEMY_GENERATION_WIDTH 0.75     //Percentuala dal bordo di schermo a cui cominciare la generazione
#define ENEMY_GENERATION_DISTANCE_COLUMN 5   //Distanza tra linee di nemici per colonna
#define ENEMY_GENERATION_DISTANCE_LINES 7    //Distanza tra colonne di nemici generati
#define ENEMY_GENERATION_LINES 4            //Numero di linee di nemici



void pAstroship(int pipeOut, Object o);
void pEnemy1(int pipeOut, Object o);
void pEnemy2(int pipeOut, Object o);
void pBomb(int pipeOut, Object o);
void pMissile(int pipeOut, Object o);
void pEngine(int life, int enemiesdim, int shotProb);


void pEnd( 
    Object *astroship,
    Object *enemies, int enemiesCount,
    Object *enemies2, int enemies2Count,
    Object *missiles, int missilesCount,
    Object *bombs, int bombsCount, int fs[]);   



int statusConditions(bool life,
    Object *enemies1, int enemies1Count,
    Object *enemies2, int enemies2Count);


int missileCollided(Object* enemy, Object missiles, int enemiesCount);
int astroCollided(Object astroship, Object dangerousObj);
bool range(int min, int max, int value);

