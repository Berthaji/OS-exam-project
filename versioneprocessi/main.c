
//#include "scene.h"
#include "processtasks.h"
//#include "threadtasks.h"


int main(){
    srand(time(NULL));

    initScreen();

    /* Visualizzazione della splash screen */       
    drawSplashScreen();
    
    /* Visualizzazione del menu e presa della scelta*/
    clearScreen();
    int choice = drawMenu();
    
    clearScreen();

    /* Inizializzare un colore per il gioco  */
    int color = (rand()%(DIM_COLORS-1))+1;
    attron(COLOR_PAIR(color));
   
    switch (choice){
        case 0: /* Facile */
            pEngine(3,4,7, color);
            break;

        case 1: /* Media */
            //tEngine(3,4,7);
            break;

        case 2: /* Difficile */
            exit(0);
            break;

        case 3: /* Molto casuale */
            exit(0);
            break;
    }
    
    attroff(COLOR_PAIR(color));
    sleep(10);
    endwin();
    return 0;

    
}