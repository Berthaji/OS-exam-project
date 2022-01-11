
#include "scene.h"
#include "processtasks.h"


int main(){
    initScreen();

    /* Visualizzazione della splash screen */       
    drawSplashScreen();
    
    
    /* Visualizzazione del menu e presa della scelta*/
    clearScreen();
    int choice = drawMenu();
    
    clearScreen();
   
    switch (choice){
        case 0: /* Facile */
            pEngine(3,4,7);
            break;

        case 1: /* Media */
            
            break;

        case 2: /* Difficile */
            
            break;

        case 3: /* Molto casuale */
            
            break;
        
        default:
            break;
    }
    
    endwin();
    return 0;

    
}