#pragma once

#include "scene.h"


Object * astroship;
Object * enemies1;
Object * enemies2;
Object * missiles;
Object * bombs;
int *doubleMissile;      //Array per contenere il fatto che il nemico di secondo livello sia stato attaccato 2 volte

int* missilesCount;      //Contatore numero di missili
int* enemies1Count;                                  
int* enemies2Count;
int* bombsCount;

int* missile2Count;    //$$ da togliere?


int* status;
int* life;

pthread_mutex_t tMutex; 



void* tastroship (void* parameters);
void* tEnemy1 (void* parameters);
void* tEnemy2(void* parameters);
void* tBombe(void* parameters);
void* tMissile(void* parameters);

void* tBombe2(void* parameters);

void drawScenes(
    // Object *astroship,
    // Object *enemies, int enemiesCount,
    // Object *enemies2, int enemies2Count,
    // Object *missiles, int missilesCount,
    // Object *bombs, int bombsCount
    );

int statusConditionsThread(bool life,
    Object *enemies1, int enemies1Count,
    Object *enemies2, int enemies2Count);

void tEnd(int i);


bool ttenemy(Object * o, bool bo);


void myInitScreen();
void clearScreens();

void checkCollision();


int mains();


