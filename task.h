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
void pAstroship(void* obj);
void pEnemy1(void* obj);
void pEnemy2(void *obj);
void pBomb(void* obj);
void pMissile(void* obj);
void tEngine(int life);
void pEnd();    



void debugPositions(
    object astroship,
    object *enemies, int enemiesCount,
    object *missiles, int missilesCount,
    object *bombs, int bombsCount);


