#pragma once
#include <sys/types.h>

//Possible object types
typedef enum TYPE {ASTROSHIP, ENEMY1, ENEMY2, BOMB, MISSILE} TYPE;

//State of the object
typedef enum {NOT_INITIALIZED, INITIALIZED, ALIVE, DEAD} STATE;



//Structure of any object in the game
typedef struct object{
    int x;
    int y;
    
    pid_t pid;                  //Process id
    int id;                     //usato per le bombe e i missili
    bool hasShot;
    int color;
    TYPE type;                  //tipo entità
    STATE state;                //stato oggetto
    int appearance;             //Graphic KIND of objects (astroship and enemies). 
                                //$$ -1 indica l'essere una bomba o un missile per ora
    
}object;

