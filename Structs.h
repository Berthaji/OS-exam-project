#pragma once
#include <sys/types.h>

//Possible object types
typedef enum TYPE {ASTROSHIP, ENEMY1, ENEMY2, BOMB, MISSILE} TYPE;

//Stato dell'oggetto
typedef enum {NOT_INITIALIZED, INITIALIZED, ALIVE, DEAD} STATE; //$$ Bisognerebbe togliere i primi due parametri ormai inutili

typedef struct Point{
    int x;
    int y;
} Point;

//Structure of any object in the game
typedef struct object{
    int x;
    float y;
    
    pid_t pid;                  //Process id
    int id;                     //usato per le bombe e i missili
    bool hasShot;               //Usato dall'astronave e dai nemici
    int color;
    TYPE type;                  //tipo entità
    STATE state;                //stato oggetto
    int appearance;             //Aspetto degli oggetti (usato da astronave e nemici)
                                //$$ -1 indica l'essere una bomba o un missile per ora

    int dir;                    //Direzione dell'oggetto (usato solo dai nemici)
    int invmov;                 //Inversione del movimento (per lo scontro tra nemici) $$deprecata, da togliere
    
}object;

