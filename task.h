#pragma once

#include <stdio.h>

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

// void player();
// void game();
// void enemy();

void initProcTask();
void pAstroship(int pipeOut, object o);
void pEnemy(int pipeOut, object o);
void pBomb(int filedescriptor);
void pMissile(int filedescriptor, int direction, object o);
void pEngine();
void pEnd();    


void debugPositions(
    object astroship,
    object *enemies, int enemiesCount,
    object *missiles, int missilesCount,
    object *bombs, int bombsCount);
