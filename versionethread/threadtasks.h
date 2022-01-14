#include "scene.h"



Object * astroship;
Object * enemies1;
Object * enemies2;
Object * missiles;
Object * bombs;
int *doubleMissile;      //Array per contenere il fatto che il nemico di secondo livello sia stato attaccato 2 volte

int* missilesCount;      //Contatore numero di missili
int* missile2Count;    
int* enemies1Count;                                  
int* enemies2Count;
int* bombsCount;


int* status;
int* life;

pthread_mutex_t tMutex; 




void* tastroship (void* parameters);
void* tEnemy1 (void* parameters);
void* tEnemy2(void* parameters);
void* tMissile(void* parameters);

void* tBombe2(void* parameters);

void drawScenes();

int statusConditionsThread(bool life,
    Object *enemies1, int enemies1Count,
    Object *enemies2);

void tEnd();

void clearScreens();

void checkCollision();

void tEngine(int lifes, int enemiesdim, int shotProb, int color);




