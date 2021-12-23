#pragma once
#include <ncurses.h>
//Dimensione schermo
#define SCREEN_X 80     //Dimensione schermo asse x
#define SCREEN_Y 120    //Dimensione schermo asse y

#define ASTRODCHOICE 4   //Numero delle possibili astronavi tra cui scegliere
#define ENEMYDCHOICE 4   //Numero dei possibil nemici tra cui scegliere
#define ASTRODIM 5      //Dimensione dello sprite astronave in lunghezza e larghezza
#define ENEMYDIM 3      //Dimensione dello sprite nemico in lunghezza e larghezza


#define M 3             //Numero dei nemici (come da specifiche del progetto)
#define delaytime 10000  //Tempo di delay per rallentare/velocizzare l'esperienza di gioco

#define SCREEN_W getmaxx(stdscr)
#define SCREEN_H getmaxy(stdscr)

/**
 * Nell'array di struct, l'indice in posizione 0 rappresenta l'astronave;
 * quelli successivi, al momento della sua creazione, i nemici,
 * seguono missili e bombe.
 *
 */

