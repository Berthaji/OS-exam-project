#pragma once
#include <sys/types.h>

//Tipologia di oggetto
//typedef enum  {ASTRONAVE = 2, NEMICO1 = 3, NEMICO2 = 4, BOMBA = 5, MISSILE=6} otype;
typedef enum  {ASTRONAVE, NEMICO1, NEMICO2 , BOMBA , MISSILE} otype;

//Stato dell'oggetto
typedef enum {NOT_INIZIALIZED, INIZIALIZED} ktype;

// typedef struct Point{
//     int x;
//     int y;
// } Point;

//Struct per il singolo oggetto che opera all'interno del gioco
typedef struct object{
    //Posizione
    int x;
    int y;
    
    pid_t pid;  //Processo

    //Aspetto
    int color;
    otype o_type;  //Tipologia di oggetto (astronave, nemico, bomba, missile etc)
    ktype k_type;
    int dtype;   //Tipologia grafica di astronave/nemico. $$ -1 indica l'essere una bomba o un missile per ora
    
}object;
//Cambiare il nome da oggetto a object
