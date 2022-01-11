#pragma once
#include <sys/types.h>
#include <pthread.h>

//Possible object types
typedef enum TYPE {ASTROSHIP, ENEMY1, ENEMY2, BOMB, MISSILE} TYPE;

//Stato dell'oggetto
typedef enum {NOT_INITIALIZED, INITIALIZED, ALIVE, DEAD, KILLED} STATE; //$$ Bisognerebbe togliere i primi due parametri ormai inutili

typedef struct Point{
    int x;
    int y;
} Point;

/**
 * @brief Struttura di ogni oggetto nel gioco
 * 
 */
typedef struct Object{
    int x;
    float y;
    
    int id;                     //usato per le bombe e i missili
    bool hasShot;               //Usato dall'astronave e dai nemici
    int color;
    TYPE type;                  //tipo entità
    STATE state;                //stato oggetto
    int appearance;             //Aspetto degli oggetti (usato da astronave e nemici)

    int dir;                    //Direzione dell'oggetto (usato solo dai nemici)
    //int invmov;                 //Inversione del movimento (per lo scontro tra nemici) $$deprecata, da togliere

    pid_t pid;                  //Process id
    pthread_t tid;              //Thread id
}Object;

