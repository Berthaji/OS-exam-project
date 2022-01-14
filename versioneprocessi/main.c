/**
 * @file main.c
 * @brief File principale da cui far eseguire il gioco
 */

#include "processtasks.h"

int main(){
    srand(time(NULL));

    initScreen();

    /* Visualizzazione della splash screen */       
    drawSplashScreen();
    getch();

    /* Visualizzazione del menu e presa della scelta*/
    clearScreen();
    int choice = drawMenu();
    
    clearScreen();

    /* Inizializzare un colore per il gioco  */
    int color = (rand()%(DIM_COLORS-1))+1;
    
   
    switch (choice){
        case 0: /* Facile */
            pEngine(3,2,7, color);
            break;

        case 1: /* Media */
            pEngine(3,4,7, color);
            break;

        case 2: /* Difficile */
            pEngine(3,8,10, color);
            break;

        case 3: /* Molto casuale */
            pEngine(rand()%5+1,rand()%10+1,rand()%15+1, color);
            break;
    }
    
    attroff(COLOR_PAIR(color)); /* Disabilitazione colori del gioco */
    endwin();
    return 0;
}