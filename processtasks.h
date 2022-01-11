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

void pClean( 
    Object *astroship,
    Object *enemies1, int enemies1Count,
    Object *enemies2, int enemies2Count,
    Object *missiles, int missilesCount,
    Object *bombs, int bombsCount);

int statusConditions(bool life,
    Object *enemies1, int enemies1Count,
    Object *enemies2, int enemies2Count);

int missileCollided(Object* enemy, Object missiles, int enemiesCount);
int astroCollided(Object astroship, Object dangerousObj);
bool range(int min, int max, int value);

