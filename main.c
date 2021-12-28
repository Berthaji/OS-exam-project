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

    //init della libreria grafica
    initScreen();
    
    //motore di gioco basato su processi
    pEngine(LIFES);  

    //ripristino della finestra del terminale
    endwin();
    printf("\n Game Over \n");
    return 0;
}

