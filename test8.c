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

object * astroship;
object * enemies1;
object * enemies2;
object * missiles;
object * bombs;
bool *doubleMissile;      //Array per contenere il fatto che il nemico di secondo livello sia stato attaccato 2 volte

int* missilesCount;      //Contatore numero di missili

pthread_mutex_t tMutex; 

void* tastroship (void* parameters);
void* tEnemy1 (void* parameters);
void* tEnemy2(void* parameters);
void* tBombe(void* parameters);
void* tMissile(void* parameters);

void drawScenes(
    object *astroship,
    //object *enemies, int enemiesCount,
    //object *enemies2, int enemies2Count,
    object *missiles, int missilesCount
    //object *bombs, int bombsCount
    );

void tEnd(int i);


bool ttenemy(object * o, bool bo);


void myInitScreen();
void clearScreens();



int main(){

    myInitScreen();
    clearScreens();

    int enemies1Count = 4;
    int enemies2Count = 0;
        
    
    missilesCount =  (int*) malloc (sizeof(int) * 1);
    *missilesCount = 0;         //nb _,0; __,1


    int bombsCount = 0;
    int missile2Count = 0;      //Contatore del controllo del secondo missile contro il nemico di secondo livello
    

    int status = -1;
    bool loop = true;           //Regolatore della condizione per per il continuo del loop e quindi delle condizioni di giocabilità
    
    ///Allocazione spazio delle variabili globali
    //Allocazione spazio per la astronave
    astroship = (object*) malloc (sizeof(object) * 1);

    //Allocazione spazio per i missili
    missiles = (object*) malloc (sizeof(object) * *missilesCount);

    //Allocazione spazio per gli M nemici di primo livello
    enemies1 = (object*) malloc (sizeof(object) * enemies1Count);
    //Allocazione spazio per gli M nemici di secondo livello
    enemies2 = (object*) malloc (sizeof(object) * enemies2Count);
    //Allocazione spazio per le bombe
    bombs = (object*) malloc (sizeof(object) * bombsCount);

    //Allocazione spazio per il controllo sul secondo missile contro il nemico di secondo livello
    doubleMissile = (bool *) malloc(sizeof(bool) * missile2Count);



    pthread_mutex_init (&tMutex, NULL);


    
    //In analogia con quanto fatto con la versione processi potrei modificare i valori prima di inviarli ad astroship, ma 
    //stavolta potrei prima del loop impostrare le corrdinate correttamente! $$

    ///Creazione dei thread per ogni elemento
    //pthread_t thread_id[10];        //Inizializzo un po di pthread id
    
    //Per astronave
    pthread_create(&astroship->tid, NULL, tastroship, (void *)astroship);


    
    //Per i missili   
    // for(int i = 0; i < *missilesCount; i++){
    //     //pthread_create(&thread_id[i], NULL, &char_print, &thread2_args);
    //     pthread_create(&missiles[i].tid, NULL, tMissile, (void *) &missiles[i]);
    // }

    // //Per nemici liv. 1    
    // for(int i = 0; i < enemies1Count; i++){
    //     //pthread_create(&thread_id[i], NULL, &char_print, &thread2_args);
    //     pthread_create(&enemies1[i].tid, NULL, &tEnemy1, &enemies1[i]);
    // }

    
    // for(int i = 1; i < 4; i++){
    //     //pthread_create(&thread_id[i], NULL, &char_print, &thread2_args);
    //     //pthread_create(&thread_id[i], NULL, &tastroship, astroship);
    //     pthread_create(&thread_id[i], NULL, &tEnemy1, &enemies1[i]); //Da rivedere assolutamente $$
    // }

    // sleep(2);

    // //Trovare il modo di fare la ex-read
    int i = 1500000000;
    
    while(i > 0){
        usleep(1000);
        i--;

        clearScreens();

        pthread_mutex_lock(&tMutex);  //sto cercando di leggere, blocco l'accesso

        //Funzione collisione e poi funzione disegno e poi ancora statusCondition

        //Controlliamo le collisioni tra oggetti

        //Stampa di ogni oggetto
        //drawObject(*astroship);
        //drawObject(missiles[0]);

        //drawScene(astroship, enemies1, enemies1Count, enemies2, enemies2Count, missiles, missilesCount, bombs, bombsCount);
        
        //drawScene(astroship, enemies1, 0, enemies2, 0, missiles, 1, bombs, 0);
        
        drawScenes(astroship,missiles,*missilesCount);
        

        // mvprintw(0,0, "CORDINATE: %d", astroship->x);
        // for(int i = 1; i < 4; i++){
        //     //pthread_create(&thread_id[i], NULL, &char_print, &thread2_args);
        //     //pthread_create(&thread_id[i], NULL, &tastroship, astroship);
        //     mvprintw(enemies1[i].x, enemies1[i].y, "O");
        // }
        refresh();

        //Sbloccaggio di un mutex
        pthread_mutex_unlock(&tMutex);

    }

    // //Abbozzo di tEnd
    //tEnd(1);
    
    
    endwin();

    return 0;
}

void* tastroship (void* parameters){
    object* obj;                                    //Noi non possiamo usare direttamente parameters perchè di tipo void*
    obj = (object*) parameters;                     //Ricorda che il cast è obbligatorio!
    int c;
    //astroship->pid = parameters;

    //Magari prima di metterci dati da rispedire, faccio un Mutex!
    //Bloccaggio di un mutex
    pthread_mutex_lock(&tMutex);
    obj->type = ASTROSHIP;
    obj->appearance = 2;
    obj->hasShot = false;
    obj->x = 6;
    obj->y = 1;
    //Mettere gli altri parametri $$


    //Sbloccaggio di un mutex
    pthread_mutex_unlock(&tMutex);
    //Fine del mutex

    while (true){
        c = getch();
        //mvprintw(6,6,"okchar"); //si ferma qui -> da risolvere

        switch (c){
            case KEY_UP:
                if (obj->y > 0){
                    pthread_mutex_lock(&tMutex);
                    obj->y -= 1;
                    pthread_mutex_unlock(&tMutex);
                }
                //obj->y++; funziona
                break;

            case KEY_DOWN:
                if (obj->y < SCREEN_H - 5){
                    pthread_mutex_lock(&tMutex);
                    obj->y++;
                    pthread_mutex_unlock(&tMutex);
                }
                //obj->y--;
                break;

            case ' ': //Barra spaziatrice
                pthread_mutex_lock(&tMutex);
                obj->hasShot = true;

                //Lancio dei due missili
                if (obj->hasShot){                   //Lancio dei due missili
                    *missilesCount += 2;
                    

                // // if(missiles == NULL)
                // //     missiles = (object *)malloc(sizeof(object) * missilesCount);
                // // else
                    missiles = (object *)realloc(missiles, sizeof(object) * *missilesCount);

                    for (int i = *missilesCount - 2; i < *missilesCount; i++){    //Aggiunta dei due missili all'elenco
                        missiles[i].x =  astroship->x + 5;//message.x + 5; 
                        missiles[i].y = astroship->y + 2;//message.y + 2;
                        missiles[i].type = MISSILE;
                        missiles[i].state = INITIALIZED;
                        
                        missiles[i].id = i; //
                        //missiles[i].pid = fork();
                        //missiles[i].tid = 0; //Dovrà essere fatto nella funzione missile!
                        missiles[i].dir = i % 2;
                        pthread_create(&missiles[i].tid, NULL, tMissile, (void *) &missiles[i]);
                        //tMissile(&missiles[i]);

                        // if (missiles[i].pid == 0){
                        //     missiles[i].dir = i % 2;
                        //     tMissile(&missiles[i]);
                        // }
                    }
                }
                

                pthread_mutex_unlock(&tMutex);
                break;

            // default:
            //     break;
        }//Fine switch

        //Controlliamo per missili morti da ammazzare
        int i;
        for(i = 0; i < *missilesCount; i++ ){
            if(missiles[i].state == DEAD)
                pthread_join(missiles[i].tid, NULL);
                missiles[i].state = KILLED;
        }

  
    }
    return NULL;
    //exit(0);
}

//Missile
void* tMissile(void* parameters){
    object* o;                                    //Noi non possiamo usare direttamente parameters perchè di tipo void*
    o = (object*) parameters;                     //Ricorda che il cast è obbligatorio!  

    //Impostiamo ogni parametro del nemico, ma in realtà dovrei usare il mutex perchè o è comunque un riferimento 
    pthread_mutex_lock(&tMutex);
    o->type = MISSILE;
    // o->x = 10;
    // o->y = 10;       
    //o->dir = 1;    
    //o->y = rand()%30;
    pthread_mutex_unlock(&tMutex); //Sblocchiamo il risultato della elaborazione

    bool loop = true;
    while (loop){
        pthread_mutex_lock(&tMutex);
        loop = gMissile(o, loop);
        // o->x = 10;
        // o->y = 10;
        pthread_mutex_unlock(&tMutex); //Sblocchiamo il risultato della elaborazione
        usleep(200000);                 // un delay per evitare che il nemico vada troppo veloce  //usleep(ENEMYSLEEP);
    }
    //harakiri
    pthread_cancel(o->tid);
    //pthread_exit(NULL);     //forse mettere una istruzione prima per dire che il nemico è morto?
}


void* tEnemy1 (void* parameters){
    object* o;                                    //Noi non possiamo usare direttamente parameters perchè di tipo void*
    o = (object*) parameters;                     //Ricorda che il cast è obbligatorio!  

    //Impostiamo ogni parametro del nemico, ma in realtà dovrei usare il mutex perchè o è comunque un riferimento 
    pthread_mutex_lock(&tMutex);
    //o->type = ENEMY1;
    // o->x = 10;
    // o->y = 10;       
    // o->dir = 1;    
    // o->y = rand()%30;
    // pthread_mutex_unlock(&tMutex); //Sblocchiamo il risultato della elaborazione

    // bool loop = true;
    // while (loop){
    //     pthread_mutex_lock(&tMutex);
    //     loop = gEnemy1(o, loop);
    //     pthread_mutex_unlock(&tMutex); //Sblocchiamo il risultato della elaborazione
    //     usleep(300000);                 // un delay per evitare che il nemico vada troppo veloce  //usleep(ENEMYSLEEP);
    // }
    //harakiri
    pthread_exit(NULL);     //forse mettere una istruzione prima per dire che il nemico è morto?
}




//Bombe//
void* tBombe(void* parameters){
    object* o;                                    //Noi non possiamo usare direttamente parameters perchè di tipo void*
    o = (object*) parameters;                     //Ricorda che il cast è obbligatorio!  

    //Impostiamo ogni parametro del nemico, ma in realtà dovrei usare il mutex perchè o è comunque un riferimento 
    pthread_mutex_lock(&tMutex);
    o->x = 10;
    o->y = 10;       
    o->dir = 1;    
    o->y = rand()%30;
    pthread_mutex_unlock(&tMutex); //Sblocchiamo il risultato della elaborazione

    bool loop = true;
    while (loop){
        pthread_mutex_lock(&tMutex);
        //loop = gMissile(o, loop);
        pthread_mutex_unlock(&tMutex); //Sblocchiamo il risultato della elaborazione
        usleep(300000);                 // un delay per evitare che il nemico vada troppo veloce  //usleep(ENEMYSLEEP);
    }
    //harakiri
    pthread_exit(NULL);     //forse mettere una istruzione prima per dire che il nemico è morto?
}

//Enemies 2 //
void* tEnemy2(void* parameters){
    object* o;                                    //Noi non possiamo usare direttamente parameters perchè di tipo void*
    o = (object*) parameters;                     //Ricorda che il cast è obbligatorio!  

    //Impostiamo ogni parametro del nemico, ma in realtà dovrei usare il mutex perchè o è comunque un riferimento 
    pthread_mutex_lock(&tMutex);
    o->x = 10;
    o->y = 10;       
    o->dir = 1;    
    o->y = rand()%30;
    pthread_mutex_unlock(&tMutex); //Sblocchiamo il risultato della elaborazione

    bool loop = true;
    while (loop){
        pthread_mutex_lock(&tMutex);
        //loop = gMissile(o, loop);
        pthread_mutex_unlock(&tMutex); //Sblocchiamo il risultato della elaborazione
        usleep(300000);                 // un delay per evitare che il nemico vada troppo veloce  //usleep(ENEMYSLEEP);
    }
    //harakiri
    pthread_exit(NULL);     //forse mettere una istruzione prima per dire che il nemico è morto?
}


//Bisogna reinserire tutti i precedenti parametri della pEnd
void tEnd(int i){
    for(int i = 0; i < 1; i++){
        //pthread_join (thread_id[i], NULL);
    }
}



void myInitScreen(){
    initscr();
    noecho();
    keypad(stdscr, 1);
    curs_set(0);
    start_color();

    init_pair(1, COLOR_RED, COLOR_BLACK);     /* Colore oggetto */
    init_pair(2, COLOR_WHITE, COLOR_BLACK);   /* Colore per cancellare */
    init_pair(3, COLOR_GREEN, COLOR_BLACK);   /* Colore per cancellare */
    init_pair(4, COLOR_BLUE, COLOR_BLACK);    /* Colore per cancellare */
    init_pair(5, COLOR_CYAN, COLOR_BLACK);    /* Colore per cancellare */
    init_pair(6, COLOR_MAGENTA, COLOR_BLACK); /* Colore per cancellare */
    init_pair(7, COLOR_RED, COLOR_BLACK);     /* Colore per cancellare */
    init_pair(8, COLOR_YELLOW, COLOR_BLACK);  /* Colore per cancellare */
}

void clearScreens()
{
    for (int y = 0; y < SCREEN_H; y++)
        for (int x = 0; x < SCREEN_W; x++)
            mvaddch(y, x, ' ');
}



void drawScenes(
    object *astroship,
    //object *enemies, int enemiesCount,
    //object *enemies2, int enemies2Count,
    object *missiles, int missilesCount
    //object *bombs, int bombsCount
    )
{
    clearScreen();

    drawObject(*astroship);

    int i;
    // for (i = 0; i < enemiesCount; i++)
    //     if (enemies[i].state != DEAD)
    //         drawObject(enemies[i]);

    // for (i = 0; i < enemies2Count; i++)
    //     if (enemies2[i].state != DEAD)
    //         drawObject(enemies2[i]);

    // for (i = 0; i < bombsCount; i++)
    //     if (bombs[i].state != DEAD)
    //         drawObject(bombs[i]);

    for (i = 0; i < missilesCount; i++)
        if (missiles[i].state != DEAD)
            drawObject(missiles[i]);

    refresh();
}

