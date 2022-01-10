#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <stdbool.h>
#include <time.h>
#include <signal.h>
#include <time.h>
#include <pthread.h>
#include <stdio.h>

//#include <termios.h>
#include <ncurses.h>
#include <pthread.h>

#include "task.h"
#include "Scene.h"


//Define PERSONALI
#define SCREEN_W getmaxx(stdscr)
#define SCREEN_H getmaxy(stdscr)



int main(){
    // clone();
    
    /*
        CLONE_FS: Le informazioni sul file system sono condivise
        CLONE_VM: Condivisione dello stesso spazio di memoria
        CLONE_SIGHAND: Condivisione dei gestori dei segnali
        CLONE_FILES: Condivisione dell’insieme dei file aperti

        senza nessuno di questi è come fare una fork()

        Il thread che crea e quello creato condividono lo stesso spazio di
        memoria, gli stessi file descriptors, e le altre risorse del sistema del
        processo originale

        Tutti fanno le stesse cose...?

    */
    //pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine)(void*), void *arg);




    return 0;
}


/* Stampa continuamente delle 'x' sullo standard error e non ritorna */
void* print_xs(void* unused){
    while(1)
        fputc('x', stderr);

    return NULL; //Obbligatorio
}

int main(){
    pthread_t thread_id;
    /* Crea un nuovo thread che esegue la funzione print_xs function */
    pthread_create(&thread_id, NULL, &print_xs, NULL);  //usare una define per rendere più comprensibile questa create del thread


    /* Il thread principale stampa continuamente delle 'o' sullo standard error */
    while(1)
        fputc('o', stderr);

    return 0;
}


/**
 * Modalità di uscita dai thread:
 *  1. mediante return della funzione del thread
 *  2. uscita esplicita mediante funzione pthread_exit [valore di questa == valore di ritorno della f. thread]
 * 
 * Modalità di passaggio dati:
 *  al thread il puntatore ad una struct o ad un array di dati, visto che accetta il tipo void*
 * 
 *  Una tecnica comune è quella di definire una struct per ogni thread
    function, ciascuna con i parametri da passare alla thread function
    
    Tale approccio consente di passare al thread un numero arbitrario di
    dati, ed è possibile invocare più volte la stessa thread function con
    dati sempre diversi
 * 
 */