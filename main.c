#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/wait.h>

#include <stdbool.h>
#include <time.h>
#include <signal.h>


//LIBRERIE PERSONALI
#include "task.h"




int main(){
    //Inizializzo il seed del generatore casuale
    srand(getpid());

    //Parametri di gioco 
    int ENEMYSLEEP = 150000;        //Timer per i nemici
    int M = 4;                      //Enemy quantity (according to project's specifications)
    //int delaytime = 10000;          //Delay time in the game
    int SHOT_PROB =  0;             //Probabilità di rilasciare la bomba (su 1000)
    int LIFES = 3;                  //Numero di vite

    //Inclinazione tiro astronave e navicelle nemiche liv. 1 e liv. 2

    /**
     * Seleziona la modalità di gioco (spazio per confermare)
        FACILE -> 250k,     3     3     4
        MEDIA  ->  150k,    4     4     3   
        DIFFICILE -> 100k,  6     7     1 
        ABBASTANZA CASUALE -> tutto rand
     * 
     */

    //init della libreria grafica
    initScreen();
    
    //Menu di gioco
    //...

    //motore di gioco basato su processi
    pEngine(3);
    
    refresh();
    sleep(3);


    // int i = LIFES; 
    // while(i > 0){
    //     pEngine(i); 
    //     i--;
    // }

    //ripristino della finestra del terminale
    endwin();
    printf("\n Game Over \n");
    return 0;
}


/**
 * Condizioni per l'uscita dal gioco 
 *  1. Il giocatore riesce ad uccidere tutti i nemici (Game Win), status == 1
 *          nelle condizioni, se l'array dei nemici hanno tutti pid = -1, allora imposta >> status = 1 e loop = false
 *  2. Tutte le astronavi vengono distrutte (quindi avrò #vite astronavi)
 *          nelle condizioni, se life (perchè legato a doppia mandata col numero di astroship) è == 0, allora status = 3 e loop false
 *  3. Un nemico arriva a bordo dello schermo
 *          nelle condizioni, se almeno un nemico arriva a bordo schermo, allora loop == false e status == 2
 *                              arrivare a abordo schermo => ciclo su coordinate dei nemici (x < 0 e  0 < y > DIMSCHERMO )
 * 
 * Condizioni per il decremento delle vite (quindi nuova astronave, ma ricomincia o rimane li?)
 *  1. Se l’astronave viene colpita da una bomba o entra in contatto con una navicella nemica esplode distruggendosi.
 *          Quindi
 *              life--
 *          
 * da passare...
 *      array dei nemici di primo e secondo livello
 *      life
 *      
 * 
 * //NB da togliere il nuovo status KILLED
 */