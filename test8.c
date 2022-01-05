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
int* enemies1Count;                                  
int* enemies2Count;
int* bombsCount;
int* missile2Count;     

pthread_mutex_t tMutex; 

void* tastroship (void* parameters);
void* tEnemy1 (void* parameters);
void* tEnemy2(void* parameters);
void* tBombe(void* parameters);
void* tMissile(void* parameters);

void drawScenes(
    object *astroship,
    object *enemies, int enemiesCount,
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

//     int* enemies1Count = 4;                                  
// int* enemies2Count = 0;
        
    
    missilesCount =  (int*) malloc (sizeof(int) * 1);
    *missilesCount = 0;         //nb _,0; __,1

    enemies1Count =  (int*) malloc (sizeof(int) * 1);
    *enemies1Count = 4;         //nb _,0; __,1

    enemies2Count =  (int*) malloc (sizeof(int) * 1);
    *enemies2Count = 0;         //nb _,0; __,1

    bombsCount =  (int*) malloc (sizeof(int) * 1);
    *bombsCount = 0;         //nb _,0; __,1

    missile2Count =  (int*) malloc (sizeof(int) * 1);       //Contatore del controllo del secondo missile contro il nemico di secondo livello
    *missile2Count = 0;         //nb _,0; __,1    
    

    int status = -1;
    bool loop = true;           //Regolatore della condizione per per il continuo del loop e quindi delle condizioni di giocabilità
    
    ///Allocazione spazio delle variabili globali
    //Allocazione spazio per la astronave
    astroship = (object*) malloc (sizeof(object) * 1);

    //Allocazione spazio per i missili
    missiles = (object*) malloc (sizeof(object) * *missilesCount);

    //Allocazione spazio per gli M nemici di primo livello
    enemies1 = (object*) malloc (sizeof(object) * *enemies1Count);

    //Allocazione spazio per gli M nemici di secondo livello
    enemies2 = (object*) malloc (sizeof(object) * *enemies2Count);
    //Allocazione spazio per le bombe
    bombs = (object*) malloc (sizeof(object) * *bombsCount);

    //Allocazione spazio per il controllo sul secondo missile contro il nemico di secondo livello
    doubleMissile = (bool *) malloc(sizeof(bool) * *missile2Count);



    pthread_mutex_init (&tMutex, NULL);


    
    //In analogia con quanto fatto con la versione processi potrei modificare i valori prima di inviarli ad astroship, ma 
    //stavolta potrei prima del loop impostrare le corrdinate correttamente! $$

    ///Creazione dei thread per ogni elemento
    //pthread_t thread_id[10];        //Inizializzo un po di pthread id
    
    //Per astronave
    pthread_create(&astroship->tid, NULL, tastroship, (void *)astroship);

    // //Per nemici liv. 1    
    // for(int i = 0; i < enemies1Count; i++){
    //     //pthread_create(&thread_id[i], NULL, &char_print, &thread2_args);
    //     pthread_create(&enemies1[i].tid, NULL, &tEnemy1, &enemies1[i]);
    // }
    int i;
    int x=0, y=1;
    for(i = 0; i < *enemies1Count; i++){
        pthread_mutex_lock(&tMutex);                    //$Forzare il terminale grande almeno 32 per i nemici
        //inizializzo qui le coordinate di partenza per non far casino con variabili simili a static in tEnemy1
        enemies1[i].x = SCREEN_W * 0.75 + (x * 5); 
             
        enemies1[i].y = 7 * y;    
        enemies1[i].type = ENEMY1;                         
        enemies1[i].dir = i % 2;      
        if (i % 4 == 3 && i != 0){
            y = 0;
            x++;
        }
        y++;
        pthread_create(&enemies1[i].tid, NULL, tEnemy1, (void*)(&enemies1[i])); 
        pthread_mutex_unlock(&tMutex);
    }


    
    int kk = 1500000000;
    
    while(kk > 0){
        usleep(1000);
        kk--;

        clearScreens();

        //Funzione collisione e poi funzione disegno e poi ancora statusCondition
        
        //Controlliamo le collisioni tra oggetti
        pthread_mutex_lock(&tMutex);  //sto cercando di leggere, blocco l'accesso
        //checkCollision()
        pthread_mutex_unlock(&tMutex);

        //Funzione disegno
        drawScenes(astroship, enemies1, *enemies1Count, missiles,*missilesCount);      //Stampa di ogni oggetto
        
        //Abilitazione condizioni di giocabilità (game Win o Game Over)
        pthread_mutex_lock(&tMutex); 
        
        refresh();

        //Sbloccaggio di un mutex
        pthread_mutex_unlock(&tMutex);


        //Join dei nemici eventualmente morti
         for(i = 0; i < *enemies1Count; i++ ){
            if(enemies1[i].state == DEAD)               
                pthread_join(enemies1[i].tid, NULL);
                pthread_mutex_lock(&tMutex);
                enemies1[i].state = KILLED;
                pthread_mutex_unlock(&tMutex);
        }

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
    int loop=true;

    pthread_mutex_lock(&tMutex);
    o->appearance = 2;
    o->type = ENEMY1;
    pthread_mutex_unlock(&tMutex);

    while (loop){
        //o.dir = direction;

        pthread_mutex_lock(&tMutex);
        o->x--;                   //sposto il nemico verso destra
        //o->y += 0;               //Si spostano nello stesso modo
        o->y += o->dir ? -1 : 1; //sposto il nemico in basso o in alto a seconda della sua direzione
        //pthread_mutex_unlock(&tMutex);

        //o.dir == 0 --> 1
        //o.dir == 1 --> -1

        /**
         * se il nemico è fuori dallo schermo dobbiamo terminare il processo
         * perciò terminiamo il loop, comunichiamo al processo principale che il nemico 
         * è "morto" e chiudiamo il processo nemico
         */

        //se il nemico non è ai bordi si può muovere tranquillamente, altrimenti lo spostiamo più dentro
        if (o->y >= SCREEN_H -2){    //Bordo inferiore superato
            //pthread_mutex_lock(&tMutex);
            o->y--;
            o->dir = 1;
            //pthread_mutex_unlock(&tMutex);
        }

        if (o->y <= 0 ){    //Bordo superiore superato
            //pthread_mutex_lock(&tMutex);
            o->y++;
            o->dir = 0;
            //pthread_mutex_unlock(&tMutex);
        }

        if (o->x < -5){
            loop = false;
            //pthread_mutex_lock(&tMutex);
            o->state = DEAD;
        }
        pthread_mutex_unlock(&tMutex);
        usleep(200000);
    }     //forse mettere una istruzione prima per dire che il nemico è morto?
    //pthread_exit(NULL);
    pthread_cancel(o->tid);
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


//tDraw
void drawScenes(
    object *astroship,
    object *enemies, int enemiesCount,
    //object *enemies2, int enemies2Count,
    object *missiles, int missilesCount
    //object *bombs, int bombsCount
    )
{
    clearScreen();

    pthread_mutex_lock(&tMutex);  
    drawObject(*astroship);

    int i;
    for (i = 0; i < enemiesCount; i++)
        if (enemies[i].state != DEAD)       //&& != KILLED
            drawObject(enemies[i]);

    // for (i = 0; i < enemies2Count; i++)
    //     if (enemies2[i].state != DEAD)
    //         drawObject(enemies2[i]);

    // for (i = 0; i < bombsCount; i++)
    //     if (bombs[i].state != DEAD)
    //         drawObject(bombs[i]);

    for (i = 0; i < missilesCount; i++)
        if (missiles[i].state != DEAD)
            drawObject(missiles[i]);

    pthread_mutex_unlock(&tMutex);

    refresh();
}


//Check delle collisioni

void checkCollision(
    int enemiesCount,
    int enemies2Count,
    int missilesCount,
    int bombsCount
    ){
    //Collisioni da controllare:
    //1. Missili-Nemici -> Nemici lv.2
    //2. Bombe-Astronave -> Vite--
    //3. Nemico-Astronave -> Muore Astronave
    //4. Missili-Nemici lv.2 -> Muore Nemico
    //5. Astronave - Nemici lv. 2 -> Vite --

    pthread_mutex_lock(&tMutex);  //sto cercando di leggere, blocco l'accesso

    //1. Missili-Nemici -> Nemici lv.2
    int i,j;
    for(i = 0; i < enemiesCount; i++){
        for(int j = 0; j < missilesCount; j++){
            if( range(enemies1[i].x, enemies1[i].x +3 , missiles[j].x) &&
                range(enemies1[i].y, enemies1[i].y +3 , missiles[j].y)       //$$ 3 e 3 sono parametri della dimensione da dare con define
                ){
                //Collisione nemico1 - Missile
                //Uccisione nemico1, uccisione missile e spawn nemico livello 2

                //Uccisione nemico1
                pthread_cancel(enemies1[i].tid);
                
                //Uccisione missile
                pthread_cancel(missiles[j].tid);

                //Spawn nemico livello 2 (non essendo chiamata con un thread a parte ricorda che questo è sempre flusso di areagioco)


            }
        }

       
    }


    //2. Bombe-Astronave -> Vite--
    //3. Nemico-Astronave -> Muore Astronave
    //4. Missili-Nemici lv.2 -> Muore Nemico
    //5. Astronave - Nemici lv. 2 -> Vite --

    pthread_mutex_unlock(&tMutex);

}

