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

#include <signal.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

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
int* missile2Count;    //$$ da togliere?

int* status;
int* life;

pthread_mutex_t tMutex; 

void* tastroship (void* parameters);
void* tEnemy1 (void* parameters);
void* tEnemy2(void* parameters);
void* tBombe(void* parameters);
void* tMissile(void* parameters);

void drawScenes(
    // object *astroship,
    // object *enemies, int enemiesCount,
    // object *enemies2, int enemies2Count,
    // object *missiles, int missilesCount,
    // object *bombs, int bombsCount
    );

void tEnd(int i);


bool ttenemy(object * o, bool bo);


void myInitScreen();
void clearScreens();

void checkCollision();



int main(){

    myInitScreen();
    clearScreens();

//     int* enemies1Count = 4;                                  
// int* enemies2Count = 0;
        
    
    missilesCount =  (int*) malloc (sizeof(int)); // * 1 è ridondante
    *missilesCount = 0;         //nb _,0; __,1

    enemies1Count =  (int*) malloc (sizeof(int));
    *enemies1Count = 4;         //nb _,0; __,1

    enemies2Count =  (int*) malloc (sizeof(int));
    *enemies2Count = 0;         //nb _,0; __,1      --0

    bombsCount =  (int*) malloc (sizeof(int));
    *bombsCount = 0;         //nb _,0; __,1

    missile2Count =  (int*) malloc (sizeof(int));       //Contatore del controllo del secondo missile contro il nemico di secondo livello
    *missile2Count = 0;         //nb _,0; __,1    
    
    status =  (int*) malloc (sizeof(int));
    *status = -1; 

    life =  (int*) malloc (sizeof(int));
    *life = 3; 


    bool loops = true;           //Regolatore della condizione per per il continuo del loop e quindi delle condizioni di giocabilità
    
    ///Allocazione spazio delle variabili globali
    //Allocazione spazio per la astronave
    astroship = (object*) malloc (sizeof(object) * 1);

    //Allocazione spazio per i missili
    missiles = (object*) malloc (sizeof(object) * *missilesCount);

    //Allocazione spazio per gli M nemici di primo livello
    enemies1 = (object*) malloc (sizeof(object) * *enemies1Count);

    //Allocazione spazio per gli M nemici di secondo livello
    enemies2 = (object*) malloc (sizeof(object) * (*enemies2Count)-1); //-1 

    //Allocazione spazio per le bombe
    bombs = (object*) malloc (sizeof(object) * (*bombsCount-1));

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
        enemies1[i].state = INITIALIZED;                         
        enemies1[i].dir = i % 2;      
        if (i % 4 == 3 && i != 0){
            y = 0;
            x++;
        }
        y++;
        pthread_create(&enemies1[i].tid, NULL, tEnemy1, (void*)(&enemies1[i])); 
        pthread_mutex_unlock(&tMutex);
    }


    
    //int kk = 1500000000;

    //loops = true;
    
    while(loops){
        usleep(1000);       //Sleep per dare un effetto di scattosità al gioco

        clearScreens();

        //Collisioni tra oggetti
        pthread_mutex_lock(&tMutex);
        //checkCollision();
        pthread_mutex_unlock(&tMutex);

        //Funzione disegno
        pthread_mutex_lock(&tMutex);
        drawScenes();
        pthread_mutex_unlock(&tMutex);

        //Abilitazione condizioni di giocabilità (game Win o Game Over)
        pthread_mutex_lock(&tMutex); 
        // *status = statusConditions(*life, enemies1,*enemies1Count,enemies2, *enemies2Count);
        // if(*status > 0)
        //     loops = false;
        pthread_mutex_unlock(&tMutex);

        /* Join dei nemici eventualmente morti */
        pthread_mutex_lock(&tMutex);
        for(i = 0; i < *enemies1Count; i++ ){   //nemici lv.1
            if(enemies1[i].state == DEAD){      //Se DEAD è stato inviato un segnale cancel
                enemies1[i].x = 0;
                enemies1[i].y = 0;
                enemies1[i].state = KILLED;
                pthread_join(enemies1[i].tid, NULL);
            }
        }
        pthread_mutex_unlock(&tMutex);

        pthread_mutex_lock(&tMutex);
        for(i = 0; i < *enemies2Count; i++ ){   //nemici lv.2
            if(enemies2[i].state == DEAD){               
                enemies2[i].x = -1; //0
                enemies2[i].y = -1; //0
                //enemies2[i].appearance = 2;
                enemies2[i].state = KILLED; //Forse 
                pthread_join(enemies2[i].tid, NULL);
            }
        }
        pthread_mutex_unlock(&tMutex);
        
        pthread_mutex_lock(&tMutex);
        for(i = 0; i < *bombsCount; i++ ){   //bombe
            if(bombs[i].state == DEAD){               
                bombs[i].x = -1; //0
                bombs[i].y = -1; //0
                //enemies2[i].appearance = 2;
                bombs[i].state = KILLED; //Forse 
                pthread_join(bombs[i].tid, NULL);
            }
        }
        //Mettiamo anche i mnissili?
        pthread_mutex_unlock(&tMutex);


        //SIGCONT);  //Last hope
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
    pthread_mutex_unlock(&tMutex);



    while (true){
        c = getch();
        //mvprintw(6,6,"okchar"); //si ferma qui -> da risolvere
        //$$ da rivedere la posizione del mutex, magari prima degli if
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

                //Togliere additional e rimettere una macro o un magic number
                int additional = 2;

                //Lancio dei due missili
                if (obj->hasShot){                   //Lancio dei due missili
                    *missilesCount += additional;
                    

                    // // if(missiles == NULL)
                    // //     missiles = (object *)malloc(sizeof(object) * missilesCount);
                    // // else
                    missiles = (object *)realloc(missiles, sizeof(object) * *missilesCount);
                    pthread_mutex_unlock(&tMutex); 

                    for (int i = *missilesCount - additional; i < *missilesCount; i++){    //Aggiunta dei due missili all'elenco
                        missiles[i].x =  astroship->x + 5;//message.x + 5; 
                        missiles[i].y = astroship->y + 2;//message.y + 2;
                        missiles[i].type = MISSILE;
                        missiles[i].state = INITIALIZED;                        
                        missiles[i].id = i; //
                        //missiles[i].pid = fork();
                        //missiles[i].tid = 0; //Dovrà essere fatto nella funzione missile!
                        missiles[i].dir = i % 2;//% additional;//2;
                        pthread_create(&missiles[i].tid, NULL, tMissile, (void *) &missiles[i]);
                        //tMissile(&missiles[i]);

                        // if (missiles[i].pid == 0){
                        //     missiles[i].dir = i % 2;
                        //     tMissile(&missiles[i]);
                        // }
                    }
                    pthread_mutex_lock(&tMutex);
                //Attenzione che hasSHot non viene riposto a false e continua a funzionare
                }
                pthread_mutex_unlock(&tMutex); 
                
                pthread_mutex_lock(&tMutex);                 
                obj->hasShot = false;
                pthread_mutex_unlock(&tMutex);
                break;

            // default:
            //     break;
        }//Fine switch

        //Controlliamo per missili morti da ammazzare $$ forse da spostare in areaGioco
        pthread_mutex_lock(&tMutex);
        int i;
        for(i = 0; i < *missilesCount; i++ ){
            if(missiles[i].state == DEAD){
                missiles[i].x = -1;
                missiles[i].y = -1;
                missiles[i].state = KILLED;

                pthread_join(missiles[i].tid, NULL);
            }
        }
        pthread_mutex_unlock(&tMutex);

  
    }
    return NULL;
    //exit(0);
}

//Missile
void* tMissile(void* parameters){
    object* o;                                    //Noi non possiamo usare direttamente parameters perchè di tipo void*
    o = (object*) parameters;                     //Ricorda che il cast è obbligatorio!  

    //Impostiamo ogni parametro del nemico, ma in realtà dovrei usare il mutex perchè o è comunque un riferimento 
    // pthread_mutex_lock(&tMutex);
    // //o->type = MISSILE;
    // // o->x = 10;
    // // o->y = 10;       
    // //o->dir = 1;    
    // //o->y = rand()%30;
    // pthread_mutex_unlock(&tMutex); //Sblocchiamo il risultato della elaborazione

    bool loop = true;
    while (loop){
        pthread_mutex_lock(&tMutex);
        loop = gMissile(o, loop);
        // o->x = 10;
        // o->y = 10;
        pthread_mutex_unlock(&tMutex); //Sblocchiamo il risultato della elaborazione
        usleep(20000);                 // un delay per evitare che il nemico vada troppo veloce  //usleep(ENEMYSLEEP);
    }
    //harakiri
    pthread_cancel(o->tid);
    //pthread_exit(NULL);     //forse mettere una istruzione prima per dire che il nemico è morto?
    return NULL;
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

        //GENERAZIONE BOMBE
        if(rand()%1000 < 20){ //$$ oltre 30 da problemi di oggetti non cancellati
            *bombsCount += 1;

            bombs = (object *)realloc(bombs, sizeof(object) * *bombsCount);

            for (int i = *bombsCount - 1; i < *bombsCount; i++){
                bombs[i].x = o->x - 1; 
                bombs[i].y = o->y + 1;
                bombs[i].type = BOMB;
                bombs[i].id = i;
                bombs[i].state = INITIALIZED;
                pthread_create(&bombs[i].tid, NULL, tBombe, (void*)&bombs[i]);
            }  
        }
        //Forse ci sta un altro blocco/sblocco mutex $$
        pthread_mutex_unlock(&tMutex); 
        pthread_mutex_lock(&tMutex); 

        loop = gEnemy1(o, loop);
        pthread_mutex_unlock(&tMutex);

        usleep(600000);
    }     //forse mettere una istruzione prima per dire che il nemico è morto?
    //pthread_exit(NULL);
    pthread_cancel(o->tid);
    return NULL;
}




//Bombe//
void* tBombe(void* parameters){
    object* o;                                    //Noi non possiamo usare direttamente parameters perchè di tipo void*
    o = (object*) parameters;                     //Ricorda che il cast è obbligatorio!  

    //Impostiamo ogni parametro del nemico, ma in realtà dovrei usare il mutex perchè o è comunque un riferimento 
    // pthread_mutex_lock(&tMutex);
    // // o->x = 10;
    // // o->y = 10;       
    // // o->dir = 1;    
    // // o->y = rand()%30;
    // pthread_mutex_unlock(&tMutex); //Sblocchiamo il risultato della elaborazione

    bool loop = true;
    while (loop){
        pthread_mutex_lock(&tMutex);
        loop = gBomb(o, loop);
        pthread_mutex_unlock(&tMutex); //Sblocchiamo il risultato della elaborazione
        usleep(80000);                 // un delay per evitare che il nemico vada troppo veloce  //usleep(ENEMYSLEEP);
    }
    //harakiri
    //pthread_exit(NULL);     //forse mettere una istruzione prima per dire che il nemico è morto?
    pthread_cancel(o->tid);
    return NULL;
}

//Enemies 2 //
void* tEnemy2(void* parameters){
    object* o;                                    //Noi non possiamo usare direttamente parameters perchè di tipo void*
    o = (object*) parameters;                     //Ricorda che il cast è obbligatorio!  

    //Impostiamo ogni parametro del nemico, ma in realtà dovrei usare il mutex perchè o è comunque un riferimento 
    // pthread_mutex_lock(&tMutex);

    // pthread_mutex_unlock(&tMutex); //Sblocchiamo il risultato della elaborazione

    bool loop = true;
    while (loop){
        pthread_mutex_lock(&tMutex);


        //GENERAZIONE BOMBE
        if(rand()%1000 < 150){
            *bombsCount += 1;

            bombs = (object *)realloc(bombs, sizeof(object) * *bombsCount);

            for (int i = *bombsCount - 1; i < *bombsCount; i++){
                bombs[i].x = o->x - 1; 
                bombs[i].y = o->y + 1;
                bombs[i].type = BOMB;
                bombs[i].id = i;
                bombs[i].state = INITIALIZED;
                pthread_create(&bombs[i].tid, NULL, tBombe, (void*)&bombs[i]);
            }  
        }
        pthread_mutex_unlock(&tMutex); 
        pthread_mutex_lock(&tMutex); 
        loop = gEnemy2(o, loop);
        
        pthread_mutex_unlock(&tMutex); //Sblocchiamo il risultato della elaborazione
        usleep(600000);                 // un delay per evitare che il nemico vada troppo veloce  //usleep(ENEMYSLEEP);
    }
    //harakiri
    //pthread_exit(NULL);     //forse mettere una istruzione prima per dire che il nemico è morto?
    pthread_cancel(o->tid);
    return NULL;
}


//Bisogna reinserire tutti i precedenti parametri della pEnd
void tEnd(int i){
    for(i = 0; i < 1; i++){
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
    // object *astroship,
    // object *enemies1, int enemiesCount,
    // object *enemies2, int enemies2Count,
    // object *missiles, int missilesCount,
    // object *bombs, int bombsCount
    )
{
    clearScreens();

    //pthread_mutex_lock(&tMutex);  
    drawObject(*astroship);

    int i;
    for (i = 0; i < *enemies1Count; i++)
        //if (enemies1[i].state != DEAD && enemies1[i].state != KILLED)
        if (enemies1[i].state == INITIALIZED)
            drawObject(enemies1[i]);

    pthread_mutex_unlock(&tMutex); 
    pthread_mutex_lock(&tMutex); 

    for (i = 0; i < *enemies2Count; i++)
        if (enemies2[i].state != DEAD && enemies2[i].state != KILLED)
            drawObject(enemies2[i]);

    pthread_mutex_unlock(&tMutex); 
    pthread_mutex_lock(&tMutex); 

    for (i = 0; i < *bombsCount; i++)
        //if (bombs[i].state != DEAD && bombs[i].state != KILLED)
        if (bombs[i].state == INITIALIZED)
            drawObject(bombs[i]);

    pthread_mutex_unlock(&tMutex); 
    pthread_mutex_lock(&tMutex); 

    for (i = 0; i < *missilesCount; i++)
        if (missiles[i].state != DEAD && missiles[i].state != KILLED)
            drawObject(missiles[i]);

    pthread_mutex_unlock(&tMutex); 
    pthread_mutex_lock(&tMutex); 

    //pthread_mutex_unlock(&tMutex);
    //Contatore vite
    mvprintw(0,0, "VITE: %d, STATUS: %d, RAND: %d", *life, *status, rand()%100);

    refresh();
}


//Check delle collisioni

void checkCollision(){
    //Collisioni da controllare:
    //1. Missili-Nemici -> Nemici lv.2 OK
    //2. Missili-Nemici lv.2 -> Muore Nemico

    //3. Bombe-Astronave -> Vite--
    //4. Nemico-Astronave -> Muore Astronave
    //5. Astronave - Nemici lv. 2 -> Vite --

    //pthread_mutex_lock(&tMutex);  //sto cercando di leggere, blocco l'accesso

    //1. Missili-Nemici -> Nemici lv.2
    int i,j;
    //if() //controllo per evitare accessi in memoria fuori dai range
    for(i = 0; i < *enemies1Count; i++){
        for(j = 0; j < *missilesCount; j++){
            //pthread_mutex_lock(&tMutex); 
            if( range(enemies1[i].x, enemies1[i].x +3 , missiles[j].x) &&
                range(enemies1[i].y, enemies1[i].y +3 , missiles[j].y)       //$$ 3 e 3 sono parametri della dimensione da dare con define
                ){
                //Collisione nemico1 - Missile
                //Uccisione nemico1, uccisione missile e spawn nemico livello 2
                //Uccisione nemico1
                if (enemies1[i].state != DEAD && enemies1[i].state != KILLED){
                    enemies1[i].state = DEAD;
                    pthread_cancel(enemies1[i].tid); //$$$$
                }
                //:::

                //Uccisione missile --> $$ oppure la deve fare astroship?
                if (missiles[j].state != DEAD && missiles[j].state != KILLED){ // && missiles[i].state != KILLED
                    
                    missiles[j].state = DEAD; // perchè non j??
                    pthread_cancel(missiles[j].tid); //$$ ci pensa astroship
                }
                //pthread_cancel(missiles[j].tid);
                //missiles[i].state = DEAD;
                //mvprintw(0,0, "EHILA");
                //refresh();
                //sleep(1);

                //Spawn nemico livello 2 (non essendo chiamata con un thread a parte ricorda che questo è sempre flusso di areagioco)
                *enemies2Count += 1;    //0
                enemies2 = (object *)realloc(enemies2, sizeof(object) * ((*enemies2Count)));

                //Aggiunta di un solo nemico lv. 2 all'elenco

                enemies2[*enemies2Count-1].state = INITIALIZED;
                enemies2[*enemies2Count-1].x = enemies1[i].x-1; //message.x; 
                enemies2[*enemies2Count-1].y = enemies1[i].y; //message.y; 
                enemies2[*enemies2Count-1].type = ENEMY2;
                enemies2[*enemies2Count-1].appearance = 3;
                
                pthread_create(&enemies2[*enemies2Count-1].tid, NULL, tEnemy2, (void *) &enemies2[*enemies2Count-1]);               
                //Attenzione che hasSHot non viene riposto a false e continua a funzionare
                
                pthread_mutex_unlock(&tMutex); 
                pthread_mutex_lock(&tMutex); 
                
                //
            }
            //pthread_mutex_unlock(&tMutex); 
        }
    }


    //2. Missili-Nemici lv.2 -> Muore Nemico
    // for(i = 0; i < *enemies2Count; i++){
    //     for(int j = 0; j < *missilesCount; j++){
    //         //pthread_mutex_lock(&tMutex); 
    //         if( range(enemies2[i].x, enemies2[i].x +3 , missiles[j].x) &&
    //             range(enemies2[i].y, enemies2[i].y +3 , missiles[j].y)       //$$ 3 e 3 sono parametri della dimensione da dare con define
    //             ){
    //             //Collisione
    //             //Se HasShot == true, allora uccidiamo nemico2 e missile
    //             //Se prima collisione, impostiamo hasShot true 
    //             //inoltre uccidiamo sempre missile
    //             if(enemies2[i].hasShot == true &&
    //                 enemies1[i].state != DEAD &&  //$$ == INITIALIZED ?
    //                 enemies1[i].state != KILLED){

    //                 //Uccisione nemico livello 2
    //                 enemies2[i].state = DEAD;
    //                 pthread_cancel(enemies2[i].tid); //$$$$
    //             }

    //             if(enemies2[i].hasShot == false){
    //                 //Uccisione nemico livello 2
    //                 enemies2[i].hasShot = true;
    //             }
                

                
                

    //             //Uccisione missile
    //             if (missiles[j].state != DEAD && missiles[j].state != KILLED){ // && missiles[i].state != KILLED
    //                 missiles[j].state = DEAD; // perchè non j??

    //                 pthread_cancel(missiles[j].tid); //$$ ci pensa astroship
    //             }
                
                
    //         }
    //         //pthread_mutex_unlock(&tMutex); 
    //     }
    // }

    pthread_mutex_unlock(&tMutex); 
    pthread_mutex_lock(&tMutex); 
    //3. Bombe-Astronave -> Vite--
    for(i = 0; i < *bombsCount; i++){
        //$$ cambiare + 3 e +5 con le macro
        //pthread_mutex_lock(&tMutex); 
        int shoting = AstroCollided(*astroship,bombs[i]);

        // if( range(astroship->x, astroship->x +5 , bombs[i].x) &&
        //     range(astroship->y, astroship->y +5 , bombs[i].y)       //$$ 3 e 3 sono parametri della dimensione da dare con define
        //     )
        if(bombs[i].state == INITIALIZED && shoting > 0)
        {
            *life -= 1;   
            //sleep(2);

            //Distruzione bomba
            bombs[i].state = DEAD;
            pthread_cancel(bombs[i].tid);
        }    
    }
    pthread_mutex_unlock(&tMutex); 

    // //5. Astronave - Nemici lv. 2 -> Vite --
    // for(i = 0; i < *enemies2Count; i++){
    //     //$$ cambiare + 3 e +5 con le macro
    //     //pthread_mutex_lock(&tMutex); 
    //     if( range(astroship->x, astroship->x +5 , enemies2[i].x) &&
    //         range(astroship->y, astroship->y +5 , enemies2[i].y)       //$$ 3 e 3 sono parametri della dimensione da dare con define
    //         ){
    //         *life -= 1;   

    //         //Distruzione nemico lv. 2
    //         pthread_cancel(enemies2[i].tid);

    //     }
        
    // }

    // //$$ 5 e 4 si possono unificare?

    // //4. Nemico-Astronave -> Muore Astronave (life--)
    // for(i = 0; i < *enemies1Count; i++){
    //     //$$ cambiare + 3 e +5 con le macro
    //     //pthread_mutex_lock(&tMutex); 
    //     if( range(astroship->x, astroship->x +5 , enemies1[i].x) &&
    //         range(astroship->y, astroship->y +5 , enemies1[i].y)       //$$ 3 e 3 sono parametri della dimensione da dare con define
    //         ){
    //         *life -= 1;   

    //         //Distruzione nemico lv. 1
    //         pthread_cancel(enemies1[i].tid);
    //     }
        
    // }
    

    //pthread_mutex_unlock(&tMutex);

}

