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
#include <pthread.h>    //Per l'uso dei thread
#include <semaphore.h> //Per l'uso dei semaforini

#include "task.h"
#include "Scene.h"


//Define PERSONALI
#define SCREEN_W getmaxx(stdscr)
#define SCREEN_H getmaxy(stdscr)

#include <pthread.h>
#include <stdio.h>

//Imposta la VM con un solo processore

int megaMiao;

/* Struttura parametri per la print_function */
typedef struct char_print_parms{
    char character; /* The character to print. */
    int count; /* The number of times to print it. */ 
    pthread_t threadId;
}char_print_parms;

/* Stampa un numero di caratteri prefissato su stderr, determinato dai parametri passati 
   Con un puntatore alla struttura char_print_parms */
void* char_print (void* parameters){
    /* Casting del puntatore con il corretto tipo, cioè (struct char_print_parms*) */
    struct char_print_parms* p;                     //Noi non possiamo usare direttamente parameters perchè di tipo void*
    p  = (struct char_print_parms*) parameters;     //Ricorda che il cast è obbligatorio!
    
    int i;
    for (i = 0; i < p->count; i++)
        printf("\n %c", p->character);
        //fputc(p->character, stderr);
        
    return NULL;
    //exit(0);
}

int main(){
    
    srand(time(NULL));
    /* Variabili per memorizzare i thread ID */

    pthread_t thread2_id, thread1_id;

    /* Strutture per il passaggio dei dati ai thread */
    struct char_print_parms thread1_args;
    struct char_print_parms thread2_args;

    /* Crea un nuovo thread che stampa 30.000 volte il carattere x */
    thread1_args.character = 'x';       //Inizializzo la struttura prima di passarla!
    thread1_args.count = 30000;
    //pthread_create(&thread1_id, NULL, &char_print, &thread1_args);

    /* Crea un nuovo thread che stampa 20.000 volte il carattere o */
    
    //pthread_create(&thread2_id, NULL, &char_print, &thread2_args);

    //
    pthread_t thread_id[100];
    char_print_parms * cpp1 = (char_print_parms*) malloc(sizeof(char_print_parms));

    for(int i = 1; i < 100; i++){
        if(i % 2 == 0){
            thread2_args.character = 'o';
            thread2_args.count = 20000;
            pthread_create(&cpp1[i].threadId, NULL, &char_print, &thread2_args);
            cpp1 = (char_print_parms *)realloc(cpp1, sizeof(object) * i);
            
        }
        if(i % 2 == 1){
            pthread_create(&thread_id[i], NULL, &char_print, &thread1_args);
        }
    }

    /* Attende la terminazione dei 2 thread precedentemente creati, quindi termina il programma */
    // pthread_join (thread1_id, NULL);
    // pthread_join (thread2_id, NULL);

    // for(int i = 0; i < 100; i++){
    //     pthread_join(thread_id[i], NULL);
    // }

    while(1){
        printf("FINE");
    }



    return 0; 
}

//Gestire la condizione per cui il main NON termini prima delle altre funzioni!
/* pthread_join 
 *  -> INPUT thread ID del thread da attendere, una variabile void* che riceve il valore di ritorno di tale thread
 *  -> ACTION: Permette di deallocare le risorse del thread specificato, cosicche non saranno più adoperati e chiude il thread
 *  -> OUTPUT: 
 */


/**
 * Buffer a coda circolare!
 * 
 * Problema delle code circolari => Riempe il buffer se pieno  => Usiamo una variabile globale contatore che è un problema
 * 
 * problema a sua volta della corsa critica nell'uso della stessa risorsa / ancora un problrma deterministico
 * Necessità di assicurare la mutua esclusione!
 * Come fare? Inanzitutto capire quale sia l'area di codice interessata dall'area critica
 * Soluzione: Semafori
 * 
 * 
 */

//SEMAFORI
void sus(){
    int S;
}

/**
 * Semaforo S
 *  intero (int)
 * 
 * Meccanismo di accesso: 
 *  -> wait(S)
 *  -> signal(S)
 * 
 */

void sus2(){
    // int Semaforo;           //Costruzione di un semaforo

    // mutex : Semaforo;       //Inizializzazione di un semaforo

    // int buffer[33];

    // do{
    //     // wait(mutex);
    //     // //... sezione critica ...
    //     // signal(mutex);
    //     //sezione non critica
    // }while(true);

    
}

/**
 * Semaforo contatore: intero che può assumere valori in un dominio non limitato
 * Semaforo binario: intero che può assumere soltanto valori booleani (zero o uno) e che prende il nome di mutex
 * 
 * Semafori presenti : inizializzato a 0, mantiene il numero degli elementi inseriti
 * Semafori disponibili : inizializzato a n, mantiene il numero delle posizioni libere
 * Semafori mutex : inizializzato ad 1, sincronizza gli accessi al buffer
 */

/**
 * Possiamo usare i seamfori e il mutex per risolvere il problema del contatore
 * 
 */


//Curiosità: Con 1 produttore e N consumatori, dovremo implementare una linked list


/**
 * MUTEX
 *  -> Se si lock() il mutex un altro thread che lo vuole usare si blocca
 *  -> Se si unblock(), l'altro thrad viene riattivato
 * 
 */

void sus30(){
    //NB Il mutex è da inizializzare una sola volta
    pthread_mutex_t mutex;
    pthread_mutex_init (&mutex, NULL);

    //oppure monolinea nel caso di inizializzazioni di default:
    //pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

    //Bloccaggio di un mutex
    pthread_mutex_lock(&mutex);

    //Sbloccaggio di un mutex
    pthread_mutex_unlock(&mutex);

    //Attenzione a non abusare del mutex per evitaer la situazione in cui uun thread non riesca mai ad essere sbloccato!

    //vedere i vslori di ritorno di un mutex per metterlo nel while true oppure direttamente nella condizione del while


}


//Di nuovo semafori, vediamp come imèplementarli meglio


/**
 * wait: decrementa il valore del semaforo di 1                                 wait--
    l'operazione blocca il thread che l'ha
    eseguita fino a quando il semaforo non assume valore positivo
    (ovviamente ciò sarà dovuto all'operazione di un altro thread)

    Non appena il valore del semaforo ritorna positivo, l'operazione di wait
    bloccata si sblocca, il semaforo viene decrementato di 1, e
    l'operazione di wait ritorna al thread chiamante

 * post: incrementa il valore del semaforo di 1,                                post++
    e se il semaforo assumeva precedentemente al post il valore zero e altri
    thread erano bloccati su di esso con una operazione di wait, uno dei
    thread viene sbloccato, la sua operazione di wait viene completata e il
    semaforo ritorna a zero (ergo il continuo della descrizione di prima per wait)
    
 */



void sus99(){
    sem_t semaforo;            //Dichirazione
    sem_init(&semaforo,0,1);   //Inizializzazione prima dell'uso; ripeto, meglio usare una define per capire meglio il suo uso

        //0 da specifiche; 1 il valore iniziale che vogliamo dargli

    sem_destroy(&semaforo);     //Per deallocarlo (alla fine nella tEnd)

    sem_wait(&semaforo);        //Implementa la wait
    sem_post(&semaforo);        //Post

    sem_trywait(&semaforo);     //Wait non bloccante
        /*
           Se il semaforo non può essere bloccato, il thread non resta in attesa! (magari per aragioco?)
        */  
    int val;   
    sem_getvalue(&semaforo, &val); 
    //restituisce il valore corrente del semaforo nella variabile int puntata dal suo secondo parametro
    //In accordo con le specifiche NON usare questo valore per prendere delle decisioni su fare wait/post
    //problema di corsa critica!

}