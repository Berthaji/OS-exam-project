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

#include "Scene.h"
#include "Utils.h"
// void player();
// void game();
// void enemy();

void initProcTask();
void pAstroship(int pipeOut, object o);
void pEnemy1(int pipeOut, object o);
void pEnemy2(int pipeOut, object o);
void pBomb(int pipeOut, object o);
void pMissile(int pipeOut, object o);
void pEngine(int life);

void pEnd( 
    object *astroship,
    object *enemies, int enemiesCount,
    object *enemies2, int enemies2Count,
    object *missiles, int missilesCount,
    object *bombs, int bombsCount, int fs[]);   


void pPID( 
    object *astroship,
    object *enemies, int enemiesCount,
    object *enemies2, int enemies2Count,
    object *missiles, int missilesCount,
    object *bombs, int bombsCount, 
    int fs[]);


int statusConditions(bool life,
    object *enemies1, int enemies1Count,
    object *enemies2, int enemies2Count);



