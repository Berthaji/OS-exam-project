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
int *doubleMissile;      //Array per contenere il fatto che il nemico di secondo livello sia stato attaccato 2 volte

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

void* tBombe2(void* parameters);

void drawScenes(
    // object *astroship,
    // object *enemies, int enemiesCount,
    // object *enemies2, int enemies2Count,
    // object *missiles, int missilesCount,
    // object *bombs, int bombsCount
    );

int statusConditionsThread(bool life,
    object *enemies1, int enemies1Count,
    object *enemies2, int enemies2Count);

void tEnd(int i);


bool ttenemy(object * o, bool bo);


void myInitScreen();
void clearScreens();

void checkCollision();



int main(){

    srand(time(NULL));
    myInitScreen();
    clearScreens();

    

//     int* enemies1Count = 4;                                  
// int* enemies2Count = 0;
        
    
    missilesCount =  (int*) malloc (sizeof(int)); // * 1 è ridondante
    *missilesCount = 0;         //nb _,0; __,1

    enemies1Count =  (int*) malloc (sizeof(int));
    *enemies1Count = 3;         //nb _,0; __,1

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
    missiles = (object*) malloc (sizeof(object) * 3); //Ho solo due missili

    //Allocazione spazio per gli M nemici di primo livello
    enemies1 = (object*) malloc (sizeof(object) * *enemies1Count);

    //Allocazione spazio per gli M nemici di secondo livello
    enemies2 = (object*) malloc (sizeof(object) * *enemies1Count); //-1 //Allochiamo sempre M spazi

    //Allocazione spazio per le bombe
    bombs = (object*) malloc (sizeof(object) * *enemies1Count);// (*bombsCount) ); //-1

    //Allocazione spazio per il controllo sul secondo missile contro il nemico di secondo livello
    doubleMissile = (int *) malloc(sizeof(int) * *enemies1Count);



    pthread_mutex_init (&tMutex, NULL);


    
    //In analogia con quanto fatto con la versione processi potrei modificare i valori prima di inviarli ad astroship, ma 
    //stavolta potrei prima del loop impostrare le corrdinate correttamente! $$

    ///Creazione dei thread per ogni elemento
    
    //Per astronave
    pthread_create(&astroship->tid, NULL, tastroship, (void *)astroship);

    //Per nemici liv. 1    
    int i;
    int x=0, y=1;
    for(i = 0; i < *enemies1Count; i++){
        pthread_mutex_lock(&tMutex);                    //$Forzare il terminale grande almeno 32 per i nemici
        //inizializzo qui le coordinate di partenza per non far casino con variabili simili a static in tEnemy1
        enemies1[i].x = SCREEN_W * 0.75 + (x * 5); 
             
        enemies1[i].y = 7 * y;    
        enemies1[i].type = ENEMY1;
        enemies1[i].state = INITIALIZED;   
        enemies1[i].appearance = 2;    
        enemies1[i].dir = i % 2;      
        if (i % 4 == 3 && i != 0){
            y = 0;
            x++;
        }
        y++;
        pthread_create(&enemies1[i].tid, NULL, tEnemy1, (void*)(&enemies1[i])); 
        pthread_mutex_unlock(&tMutex);
    }

    //Per i nemici di secondo livello
    for(i = 0; i < *enemies1Count; i++){
        pthread_mutex_lock(&tMutex);                    //$Forzare il terminale grande almeno 32 per i nemici
        //inizializzo qui le coordinate di partenza per non far casino con variabili simili a static in tEnemy1
        enemies2[i].x = -1; 
        enemies2[i].y = -1;    
        enemies2[i].type = ENEMY2;
        enemies2[i].state = NOT_INITIALIZED;   //in teroia non dovrebbe
        enemies2[i].appearance = 3;    
        //enemies2[i].dir = i % 2;      
        doubleMissile[i] = 0;           //Contatore per il doppio colpo
        pthread_create(&enemies2[i].tid, NULL, tEnemy2, (void*)(&enemies2[i])); 
        pthread_mutex_unlock(&tMutex);
    }

    //Per la generazione delle bombe dei nemici (inizializzazione dei valori)

    for(i = 0; i < *enemies1Count; i++){
        pthread_mutex_lock(&tMutex); 

        bombs[i].state = NOT_INITIALIZED;
        bombs[i].type = BOMB;
        bombs[i].x = -1;
        bombs[i].x = -1;  
        pthread_create(&(bombs[i].tid), NULL, tBombe2, (void*)(&bombs[i]));
            
        pthread_mutex_unlock(&tMutex);   
    }

    //Per la generazione dei missili
    for(i = 0; i < 2; i++){             //Questo 2 deve viventare una macro per tutto $$
        pthread_mutex_lock(&tMutex); 

        missiles[i].type = MISSILE;
        missiles[i].state = NOT_INITIALIZED;
        missiles[i].x = -1;
        missiles[i].y = -1;
        missiles[i].dir = i % 2;//% additional;//2;
        

        pthread_create(&(missiles[i].tid), NULL, tMissile, (void*)(&missiles[i]));
            
        pthread_mutex_unlock(&tMutex);   
    }

    
    //int kk = 1500000000;

    //loops = true;
    
    while(loops){
        usleep(1000);       //Sleep per dare un effetto di scattosità al gioco

        clearScreens();

        //Collisioni tra oggetti
        pthread_mutex_lock(&tMutex);
        checkCollision();
        pthread_mutex_unlock(&tMutex);

        //Funzione disegno
        pthread_mutex_lock(&tMutex);
        drawScenes();
        pthread_mutex_unlock(&tMutex);

        //Abilitazione condizioni di giocabilità (game Win o Game Over)
        pthread_mutex_lock(&tMutex); 
        *status = statusConditionsThread(*life, enemies1,*enemies1Count,enemies2, *enemies2Count);
        if(*status > 0)
            loops = false;
        pthread_mutex_unlock(&tMutex);


        //Pulizia dai missili incastonati
        /*
            1111333333333311
            3333333311111
            13111111

            trovare la prima sequenza di 3
            se fino alla fine è 3 ok
            altrimenti cambia in 3

        */

        pthread_mutex_lock(&tMutex); 
        // pthread_mutex_unlock(&tMutex);


        // int msec = 0, trigger = 5; /* 5 sec */
        // clock_t difference = 2;
        // clock_t before = clock();


        //GENERAZIONE BOMBE
        int shot = 0;
        if(rand()%100000 < 5000){ //$$ oltre 30 da problemi di oggetti non cancellati
        //if(difference - msec >= 2){
            //difference = 0;
            //pthread_mutex_lock(&tMutex); 

            if(bombs[*bombsCount].state != INITIALIZED){
                //La bomba non sta "camminando": puoi agire

                //Prepariamo la bomba per essere sparata

                /*Scegliamo il nemico da cui partire*/
                /*
                    Prtiamo da quello di primo livello, se morto quello di secondo
                    se entrambi morti allora non stampare
                */

                int randEnemy;
                randEnemy = rand() % *enemies1Count;

                if(enemies1[randEnemy].state == INITIALIZED){ //Facciamo sul nemico di primo livello
                    bombs[*bombsCount].x = (enemies1[randEnemy].x) - 1;//o->x - 1; 
                    bombs[*bombsCount].y = (enemies1[randEnemy].y) + 1;//o->y + 1;

                    bombs[*bombsCount].id = *bombsCount; //
                    bombs[*bombsCount].state = INITIALIZED;
                }
                else if (enemies2[randEnemy].state == INITIALIZED){ //Facciamo sul nemico di 2o livello
                    bombs[*bombsCount].x = (enemies2[randEnemy].x) - 1;//o->x - 1; 
                    bombs[*bombsCount].y = (enemies2[randEnemy].y) + 1;//o->y + 1;

                    bombs[*bombsCount].id = *bombsCount; //
                    bombs[*bombsCount].state = INITIALIZED;
                }
                else{       //Entrambi morti aa quell'indice -> Nulla da fare in particolare

                }

                
                //Comandi sempre importanti
                *bombsCount += 1;
                
                if(*bombsCount >= *enemies1Count){
                    *bombsCount = 0;
                    //bombs[*bombsCount].hasShot = true;
                }
            }



            //bombs = (object *)realloc(bombs, sizeof(object) * (*bombsCount));
            //int i;
            

            //Manca inizializzazione del primo elemento quando faccio malloc

            
            
            //pthread_create(&bombs->tid, NULL, tBombe, (void*)(&bombs[*bombsCount - 1]));
            
            //bombs[i].state = DEAD;
             
            // 

            // for (i = *bombsCount - 1; i < *bombsCount; i++){
            //     int randEnemy;
            //     do{
            //         randEnemy = rand() % *enemies1Count;
            //     }while(enemies1[randEnemy].state != INITIALIZED);
            //     bombs[i].x = (enemies1[randEnemy].x) - 1;//o->x - 1; 
            //     bombs[i].y = (enemies1[randEnemy].y) + 1;//o->y + 1;
            //     bombs[i].type = BOMB;
            //     bombs[i].id = i;
            //     bombs[i].state = INITIALIZED;
            //     pthread_create(&bombs[i].tid, NULL, tBombe, (void*)(&bombs[i]));
            //     //bombs[i].state = DEAD;
            // }  


            //sleep(2);
            // difference = clock() - before;
            // msec = difference * 1000 / CLOCKS_PER_SEC;
            //iterations++;
        }
        //shot = shot++;

        pthread_mutex_unlock(&tMutex);

        // int l = *bombsCount-1;
        // bool bo = true;
        // while(bo && l >= 0){
        //     if(bombs[l].state == INITIALIZED)
        //         l--;
        //     else
        //         bo = false;
        // }
        // l--; //opzionale anche se logicamente corretto
        // bo = true;
        // while(bo){
        //     if(bombs[l].state == INITIALIZED)
        //         bombs[l].state = DEAD;
        //     l--;
        //     if(l < 0)
        //         bo = false;
        // }
        // pthread_mutex_unlock(&tMutex); 
        /*
        if iniziale sul primo elemento
            se == 1, ciclo finchè non trovo 3
            da quel momento in poi qualuneu cosa diversa da 3 è da impostare a 3
        */
        // pthread_mutex_lock(&tMutex);
        // if(*missilesCount > 6){
        //     int l = *missilesCount-1;
        //     while(l > -1){
        //         if(missiles[l].state != DEAD){
        //             l--;
        //         }
        //     }
        //     //Qualsiasi elemento che non sia DEAD lo impostiamo come tale
        //     while(l > -1){
        //         if(missiles[l].state != DEAD){
        //             missiles[l].state = DEAD;
        //         }
        //         l--;
        //     }
        // }
        // pthread_mutex_unlock(&tMutex);
        // Join dei nemici eventualmente morti //
        // pthread_mutex_lock(&tMutex);
        // for(i = 0; i < *enemies1Count; i++ ){   //nemici lv.1
        //     if(enemies1[i].state == DEAD){      //Se DEAD è stato inviato un segnale cancel
        //         enemies1[i].x = 0;
        //         enemies1[i].y = 0;
        //         enemies1[i].state = KILLED;
        //         pthread_join(enemies1[i].tid, NULL);
        //     }
        // }
        // pthread_mutex_unlock(&tMutex);
        // pthread_mutex_lock(&tMutex);
        // for(i = 0; i < *enemies2Count; i++ ){   //nemici lv.2
        //     if(enemies2[i].state == DEAD){               
        //         enemies2[i].x = -1; //0
        //         enemies2[i].y = -1; //0
        //         //enemies2[i].appearance = 2;
        //         enemies2[i].state = KILLED; //Forse 
        //         pthread_join(enemies2[i].tid, NULL);
        //     }
        // }
        // pthread_mutex_unlock(&tMutex); 
        // pthread_mutex_lock(&tMutex);
        // if(*bombsCount > 0){
        //     for(i = 0; i < *bombsCount; i++ ){   //bombe
        //         if(bombs[i].state == DEAD){               
        //             bombs[i].x = -1; //0
        //             bombs[i].y = -1; //0            
        //             //enemies2[i].appearance = 2;
        //             //pthread_cancel(bombs[i].tid);   //Forse $$ da mettere sempre cosi
        //             bombs[i].state = KILLED; //Forse 
        //             pthread_join(bombs[i].tid, NULL);
        //         }
        //     }
        // }
        // Mettiamo anche i mnissili?
        // pthread_mutex_unlock(&tMutex);


        //kill(getpid(), SIGCONT);  //Last hope
    }

    // //Abbozzo di tEnd
    tEnd(*enemies1Count);

    //sleep(15);
    
    
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

    obj->pid = getpid();

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
                pthread_mutex_lock(&tMutex);
                if (obj->y > 0){
                    obj->y -= 1;
                }
                pthread_mutex_unlock(&tMutex);
                //obj->y++; funziona
                break;

            case KEY_DOWN:
                pthread_mutex_lock(&tMutex);
                if (obj->y < SCREEN_H - 5){
                    obj->y++;
                }
                pthread_mutex_unlock(&tMutex);
                //obj->y--;
                break;

            case ' ': //Barra spaziatrice
                pthread_mutex_lock(&tMutex);
                //Puoi sparare solo se entrambi i missili sono in status DEAD
                if(
                    missiles[0].state != INITIALIZED //oppure dead or killed
                    && missiles[1].state != INITIALIZED //oppure dead or killed
                ){
                    //Sono "morti": li reinizializziamo
                    missiles[0].x =  astroship->x + 5;//message.x + 5; 
                    missiles[0].y = astroship->y + 2;//message.y + 2;
                    missiles[0].state = INITIALIZED;

                    missiles[1].x =  astroship->x + 5;//message.x + 5; 
                    missiles[1].y = astroship->y + 2;//message.y + 2;
                    missiles[1].state = INITIALIZED;
                }
                //obj->hasShot = true;

                //Togliere additional e rimettere una macro o un magic number
                //int additional = 2;

                //Lancio dei due missili
                //if (obj->hasShot){                   //Lancio dei due missili
                    //*missilesCount += additional;
                    

                    // // if(missiles == NULL)
                    // //     missiles = (object *)malloc(sizeof(object) * missilesCount);
                    // // else
                    // missiles = (object *)realloc(missiles, sizeof(object) * *missilesCount);
                    //pthread_mutex_unlock(&tMutex); 
                    // int i;
                    // for (i = *missilesCount - additional; i < *missilesCount; i++){    //Aggiunta dei due missili all'elenco
                    //     missiles[i].x =  astroship->x + 5;//message.x + 5; 
                    //     missiles[i].y = astroship->y + 2;//message.y + 2;
                    //     missiles[i].type = MISSILE;
                    //     missiles[i].state = INITIALIZED;                        
                    //     missiles[i].id = i;
                    //     missiles[i].dir = i % 2;//% additional;//2;
                    //     pthread_create(&missiles[i].tid, NULL, tMissile, (void *) &missiles[i]);
                    // }
                    //pthread_mutex_lock(&tMutex);
                //Attenzione che hasSHot non viene riposto a false e continua a funzionare
                //}
                
                
                // pthread_mutex_unlock(&tMutex); 
                
                // pthread_mutex_lock(&tMutex);                 
                //obj->hasShot = false;
                pthread_mutex_unlock(&tMutex);
                break;
        }//Fine switch

        //Controlliamo per missili morti da ammazzare $$ forse da spostare in areaGioco
        // pthread_mutex_lock(&tMutex);
        // int i;
        // for(i = 0; i < *missilesCount; i++ ){
        //     if(missiles[i].state == DEAD){
        //         missiles[i].x = -1;
        //         missiles[i].y = -1;
        //         missiles[i].state = KILLED;

        //         pthread_join(missiles[i].tid, NULL);
        //     }
        // }
        // pthread_mutex_unlock(&tMutex);

  
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

    //bool loop = true;
     // while (loop){
        // pthread_mutex_lock(&tMutex);
        //loop = gMissile(o, loop);
        // // o->x = 10;
        // // o->y = 10;
        // pthread_mutex_unlock(&tMutex); //Sblocchiamo il risultato della elaborazione
        //

        //
    while(o->state != KILLED){
    
        while(o->state == INITIALIZED){
                pthread_mutex_lock(&tMutex);

                //Collisione col nemico di secondo livello
                int i;
                for(i = 0; i < *enemies1Count; i++ ){
                    if( range(enemies2[i].x, enemies2[i].x , o->x) &&
                        range(enemies2[i].y, enemies2[i].y +3 , o->y)       //$$ 3 e 3 sono parametri della dimensione da dare con define
                        //&& o->state == INITIALIZED
                        && enemies2[i].state == INITIALIZED
                        ){
                            //Match raggiunto
                            //Incremento opportuno
                            if(doubleMissile[i] == 1){
                                doubleMissile[i] = 2;
                            
                            }
                            else if(doubleMissile[i] == 0){
                                doubleMissile[i] = 1;
                            }   

                            //condizione ulteriore di uscita dal ciclo
                            o->state = DEAD;   //altrimenti o->x = -3; 
                            o->x = -1;
                            o->y = -1;
                            //loop = false;
                    }
                }


                o->x++;
                o->y += (o->dir ? -1 : 1)*0.05;  

                //Bordo schermo         $$ forse da spostare nelle collisioni
                if (o->y < -2 || o->y > SCREEN_H + 2){
                    o->x = -1;
                    o->x = -1;
                    o->state = DEAD;
                }

                if(o->x > SCREEN_W){
                    o->x = -1;
                    o->x = -1;
                    o->state = DEAD;
                }

                //STatus DEAD --> $$ non dovrebbe mai entrare qua
                if (o->state == DEAD){
                    o->y = -1;
                    o->x = -1;
                    o->state = DEAD;
                    //o->state = DEAD;  //$$ ??
                }

                // if(o->x < 0){
                //     o->x = -1;
                //     o->x = -1;
                //     o->state = DEAD;
                // }




                pthread_mutex_unlock(&tMutex);

                usleep(20000);   
            }
        //}



        

       
        
        
        //
        usleep(20000);                 // un delay per evitare che il nemico vada troppo veloce  //usleep(ENEMYSLEEP);
    }
    //harakiri
    //pthread_cancel(o->tid);
    //pthread_exit(NULL);     //forse mettere una istruzione prima per dire che il nemico è morto?
    o->state = DEAD;
    o->x = -1;
    o->y = -1;
    
    return NULL;
}


void* tEnemy1 (void* parameters){
    object* o;                                    //Noi non possiamo usare direttamente parameters perchè di tipo void*
    o = (object*) parameters;                     //Ricorda che il cast è obbligatorio!  

    // pthread_mutex_lock(&tMutex);
    // pthread_mutex_unlock(&tMutex);

    int loop=true;
    while (loop){
        //o.dir = direction;
        pthread_mutex_lock(&tMutex);

        //GENERAZIONE BOMBE
        //SPOSTATO IN AREAGIOCO $$
        //Forse ci sta un altro blocco/sblocco mutex $$

        loop = gEnemy1(o, loop);
        pthread_mutex_unlock(&tMutex);

        usleep(600000);
    }     //forse mettere una istruzione prima per dire che il nemico è morto?
    //pthread_exit(NULL);
    //pthread_cancel(o->tid);
    //o->state = DEAD;

    o->x = -1;
    o->y = -1;
    //sleep(1);

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
        
        while(o->state != KILLED){
            while(o->state == INITIALIZED){
                o->x--;
                if(o->x < 0){
                    o->x = -1;
                    o->x = -1;
                    o->state = DEAD;
                }
                usleep(80000);   
            }
        }

        // o->pid = getpid();
        // loop = gBomb(o, loop);
        // o->x--;
        // //o->hasShot = loop;
        pthread_mutex_unlock(&tMutex); //Sblocchiamo il risultato della elaborazione
                      // un delay per evitare che il nemico vada troppo veloce  //usleep(ENEMYSLEEP);
    }
    //harakiri
    //pthread_exit(NULL);     //forse mettere una istruzione prima per dire che il nemico è morto?
    //pthread_cancel(o->tid);
    //o->state = DEAD;

    //Posizioniamo a -1, -1? il parametro == INITIALIZED dovrebbe evitare di renderlo necessario
    //o->state = DEAD;
    o->x = -1;
    o->y = -1;

    //printf("\a");
   


    return NULL;
}


//Bombe//
void* tBombe2(void* parameters){
    object* o;                                    //Noi non possiamo usare direttamente parameters perchè di tipo void*
    o = (object*) parameters;                     //Ricorda che il cast è obbligatorio!  

    //Impostiamo ogni parametro del nemico, ma in realtà dovrei usare il mutex perchè o è comunque un riferimento 
    // pthread_mutex_lock(&tMutex);
    // // o->x = 10;
    // // o->y = 10;       
    // // o->dir = 1;    
    // // o->y = rand()%30;
    // pthread_mutex_unlock(&tMutex); //Sblocchiamo il risultato della elaborazione



    // bool loop = true;
    // bool test;
    // while (loop){
    //     pthread_mutex_lock(&tMutex);
    //     int i;
    //     for(i = 0; i < *bombsCount; i++ ){
    //         if(bombs[i].state == INITIALIZED){
    //              o->x--;
    //         }
    //         if(bombs[i].x < 0){
    //             loop = false;
    //             bombs[i].state = DEAD;

    //         }
    //             //loop = gBomb(&bombs[i], test);
    //     }
    //     pthread_mutex_unlock(&tMutex);

    //     // pthread_mutex_lock(&tMutex);
    //     // o->pid = getpid();
    //     // //loop = gBomb(o, loop);
    //     // o->x--;
    //     // o->hasShot = loop;
    //     // pthread_mutex_unlock(&tMutex); //Sblocchiamo il risultato della elaborazione
    //     usleep(80000);                 // un delay per evitare che il nemico vada troppo veloce  //usleep(ENEMYSLEEP);
    // }
    // //harakiri
    // //pthread_exit(NULL);     //forse mettere una istruzione prima per dire che il nemico è morto?
    // //pthread_cancel(o->tid);
    // //o->state = DEAD;

    // //Posizioniamo a -1, -1? il parametro == INITIALIZED dovrebbe evitare di renderlo necessario
    // //o->state = DEAD;
    // o->x = -1;
    // o->y = -1;

    //printf("\a");
   bool loop;
//    while(1){
//        pthread_mutex_lock(&tMutex);
//        bombs[o->id].pid = rand()%100;
//        bombs[o->id].x--;
//        pthread_mutex_unlock(&tMutex);
//        usleep(200000);
//    }

   while(o->state != KILLED){
        while(o->state == INITIALIZED){
            o->x--;
            if(o->x < 0){
                o->x = -1;
                o->x = -1;
                o->state = DEAD;
            }
            usleep(80000);   
        }
    }


    return NULL;
}

//Enemies 2 //
void* tEnemy2(void* parameters){
    object* o;                                    //Noi non possiamo usare direttamente parameters perchè di tipo void*
    o = (object*) parameters;                     //Ricorda che il cast è obbligatorio!  

    //Impostiamo ogni parametro del nemico, ma in realtà dovrei usare il mutex perchè o è comunque un riferimento 
    // pthread_mutex_lock(&tMutex);
    while(o->state != KILLED){
        while(o->state == INITIALIZED){
            o->x--;
            if(o->x < 0){
                o->x = -1;
                o->x = -1;
                o->state = DEAD;
            }
            usleep(200000);   
        }
    }


    // int loop=true;
    // while (loop){
    //     //o.dir = direction;
    //     pthread_mutex_lock(&tMutex);

    //     //GENERAZIONE BOMBE
    //     //SPOSTATO IN AREAGIOCO $$
    //     //Forse ci sta un altro blocco/sblocco mutex $$

    //     loop = gEnemy1(o, loop);
    //     pthread_mutex_unlock(&tMutex);

    //     usleep(600000);
    // }     //forse mettere una istruzione prima per dire che il nemico è morto?
    // //pthread_exit(NULL);
    // //pthread_cancel(o->tid);
    // //o->state = DEAD;

    // o->x = -1;
    // o->y = -1;
    //sleep(1);

    return NULL;

    // pthread_mutex_unlock(&tMutex); //Sblocchiamo il risultato della elaborazione

    // bool loop = true;
    // while (loop){
    //     pthread_mutex_lock(&tMutex);


        //GENERAZIONE BOMBE
        // if(rand()%1000 < 150){
        //     *bombsCount += 1;

        //     bombs = (object *)realloc(bombs, sizeof(object) * *bombsCount);
        //     int i;
        //     for (i = *bombsCount - 1; i < *bombsCount; i++){
        //         bombs[i].x = o->x - 1; 
        //         bombs[i].y = o->y + 1;
        //         bombs[i].type = BOMB;
        //         bombs[i].id = i;
        //         bombs[i].state = INITIALIZED;
        //         pthread_create(&bombs[i].tid, NULL, tBombe, (void*)&bombs[i]);
        //     }  
        // }


        
    //     loop = gEnemy2(o, loop);
        
    //     pthread_mutex_unlock(&tMutex); //Sblocchiamo il risultato della elaborazione
    //     usleep(600000);                 // un delay per evitare che il nemico vada troppo veloce  //usleep(ENEMYSLEEP);
    // }
    // //harakiri
    // //pthread_exit(NULL);     //forse mettere una istruzione prima per dire che il nemico è morto?
    // //pthread_cancel(o->tid);
    // //o->state = DEAD;
    // return NULL;
}


//Bisogna reinserire tutti i precedenti parametri della pEnd
void tEnd(int i){

    mvprintw(0,0,"PID: %d", astroship->pid);
    refresh();
    pthread_cancel(astroship->tid);
    pthread_join(astroship->tid, NULL);
    // for(i = 0; i < 1; i++){
    //     pthread_cancel(missiles[i].tid);
    //     pthread_join (missiles[i].tid, NULL);
    //     pthread_cancel(bombs[i].tid);
    //     pthread_join (bombs[i].tid, NULL);
    //     pthread_cancel(enemies1[i].tid);
    //     pthread_join (enemies1[i].tid, NULL);
    //     pthread_cancel(enemies2[i].tid);
    //     pthread_join (enemies2[i].tid, NULL);
    // }

    //free
    free(astroship);
    free(enemies1);
    free(missiles);
    free(bombs);
    free(enemies2);

    free(doubleMissile);
    free(missilesCount);
    free(enemies1Count);
    free(enemies2Count);
    free(bombsCount);
    free(missile2Count);
    free(status);
    free(life);

    
    
    sleep(6);
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

void clearScreens(){
    int y,x;
    for (y = 0; y < SCREEN_H; y++)
        for (x = 0; x < SCREEN_W; x++)
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

    if(*enemies1Count > 0)
        for (i = 0; i < *enemies1Count; i++)
            if (enemies1[i].state == INITIALIZED)  //if (enemies1[i].state != DEAD && enemies1[i].state != KILLED)
                drawObject(enemies1[i]);

    // pthread_mutex_unlock(&tMutex); 
    // pthread_mutex_lock(&tMutex); 

    //if(*enemies2Count > 0)
        for (i = 0; i < *enemies1Count; i++)
            if (enemies2[i].state == INITIALIZED)//!= DEAD && enemies2[i].state != KILLED)
                drawObject(enemies2[i]);

    // pthread_mutex_unlock(&tMutex); 
    // pthread_mutex_lock(&tMutex); 

    //if(*bombsCount > 0)
        for (i = 0; i < *enemies1Count; i++)
            //if (bombs[i].state != DEAD && bombs[i].state != KILLED)
            if (bombs[i].state == INITIALIZED)
                drawObject(bombs[i]);

    // pthread_mutex_unlock(&tMutex); 
    // pthread_mutex_lock(&tMutex); 

    //if(*missilesCount > 0)
        for (i = 0; i < 2; i++)         //$$ il 2 deve diventare una macro
            if (missiles[i].state == INITIALIZED)//!= DEAD && missiles[i].state != KILLED)
                drawObject(missiles[i]);

    // pthread_mutex_unlock(&tMutex); 
    // pthread_mutex_lock(&tMutex); 

    //pthread_mutex_unlock(&tMutex);
    //Contatore vite
    mvprintw(0,0, "VITE: %d , STATUS: %d, RAND: %d", *life, *status, rand()%100);


    //
    //clearScreens();
    int kk;
    // for(kk = 0; kk < *enemies1Count ;kk++){
    //     mvprintw(kk+1,20, "m2count: %d: %d", kk, doubleMissile[kk]);
    // }

    for(kk = 0; kk < *enemies1Count ;kk++){
        mvprintw(kk+1,35, "e1status: %d", enemies1[kk].state);
    }

    for(kk = 0; kk < *enemies1Count ;kk++){
        mvprintw(kk+1,50, "dM: %d X", doubleMissile[kk]);
    }

    // for(kk = 0; kk < *bombsCount ;kk++){
    //     mvprintw(kk+1,65, "bstatus: %d, bID:%d, pID: %d, loop: %d", bombs[kk].state, bombs[kk].id, bombs[kk].pid, bombs[kk].hasShot);
    // }
    //sleep(1);

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
        for(j = 0; j < 2; j++){
            //pthread_mutex_lock(&tMutex); 

            if( range(enemies1[i].x, enemies1[i].x +3 , missiles[j].x) &&
                range(enemies1[i].y, enemies1[i].y +3 , missiles[j].y)       //$$ 3 e 3 sono parametri della dimensione da dare con define
                // &&
                // enemies1[i].state == INITIALIZED &&
                && missiles[j].state == INITIALIZED
                && missiles[j].x != -1
                ){
               

                missiles[j].state = DEAD;

                enemies1[i].state = DEAD;

                //Uccisione nemico1, uccisione missile e spawn nemico livello 2
                //Uccisione nemico1
                // if (enemies1[i].state == INITIALIZED){
                // // if (enemies1[i].state == INITIALIZED){
                //     enemies1[i].state = DEAD;

                //     //sleep(2);
                //     //pthread_cancel(enemies1[i].tid); //$$$$


                //Aggiunta di un solo nemico lv. 2 all'elenco
                //*enemies2Count += 1;    //indica l'ultimo elemento aggiunto
                enemies2[i].state = INITIALIZED;
                enemies2[i].x = enemies1[i].x-1; //message.x; 
                enemies2[i].y = enemies1[i].y; //message.y; 
                //enemies2[i].type = ENEMY2;
                //enemies2[i].appearance = 3;
                //enemies2[i].hasShot = false;     //$$ da rimuovere
                
                //pthread_create(&enemies2[i].tid, NULL, tEnemy2, (void *) &enemies2[i]);               
                
                //Aggiunta di un elemento all'array conytatore doppio colpo nemico lv. 2
                //*missile2Count += 1;
                //doubleMissile = (int *)realloc(doubleMissile, sizeof(int) * *missile2Count);
                //doubleMissile[*enemies2Count-1] = 0;
                
                //:::

                //Uccisione missile --> $$ oppure la deve fare astroship?
                // if (missiles[j].state == INITIALIZED){ // && missiles[i].state != KILLED
                //     missiles[j].x = -1;
                //     missiles[j].y = -1;

                //     missiles[j].state = DEAD;
                //     //sleep(1);
                //     //pthread_cancel(missiles[j].tid); //$$ ci pensa astroship
                // }

                //Spawn nemico livello 2 (non essendo chiamata con un thread a parte ricorda che questo è sempre flusso di areagioco)
                //enemies2 = (object *)realloc(enemies2, sizeof(object) * ((*enemies2Count)));


                
                //Attenzione che hasSHot non viene riposto a false e continua a funzionare
                
                // pthread_mutex_unlock(&tMutex); 
                // pthread_mutex_lock(&tMutex); 
                
                //
            } 
            //pthread_mutex_unlock(&tMutex); 
        }
    }

    //pthread_mutex_unlock(&tMutex); 
    // pthread_mutex_lock(&tMutex); 

     
     


    //2. Missili-Nemici lv.2 -> Muore Nemico
    for(i = 0; i < *enemies1Count; i++){
        if(doubleMissile[i] == 2){
            //Killa nemico e missile
            if (enemies2[i].state == INITIALIZED){
                enemies2[i].state = DEAD;
                 
                
                 
                //pthread_cancel(enemies2[i].tid); //$$$$
            }
            // //Uccisione missile
            // if (missiles[j].state != DEAD && missiles[j].state != KILLED){ // && missiles[i].state != KILLED
            //     missiles[j].state = DEAD; // perchè non j??
            //     pthread_cancel(missiles[j].tid);
            // }
        }
    }

     
              
                 

    // for(i = 0; i < *enemies2Count; i++){
    //     for(j = 0; j < *missile2Count; j++){
    //         //pthread_mutex_lock(&tMutex); 
    //         if( range(enemies2[i].x, enemies2[i].x , missiles[j].x) &&
    //             range(enemies2[i].y, enemies2[i].y +3 , missiles[j].y)   //match - nemico colpito
    //             //enemies2[i].state == INITIALIZED
                
    //             ){
                
    //             if(doubleMissile[i] == 2){   //prima volta //puntatore * da mettere? $$
    //                //0 vergine
    //                 //1 colpito una volta
    //                 //Uccisione nemico livellSo 2
    //                 //doubleMissile[i] = 0;
    //                 enemies2[i].state = DEAD;
    //                 pthread_cancel(enemies2[i].tid); //$$$$
    //             }
    //             doubleMissile[i] = rand()%100;


    //             //Uccisione nemico livello 2
    //             //enemies2[i].state = DEAD;
    //             //pthread_cancel(enemies2[i].tid); //$$$$

    //             //Uccisione missile
    //             if (missiles[j].state != DEAD && missiles[j].state != KILLED){ // && missiles[i].state != KILLED
    //                 missiles[j].state = DEAD; // perchè non j??

    //                 pthread_cancel(missiles[j].tid);
    //             }
                
                
    //         }
    //         //pthread_mutex_unlock(&tMutex); 
    //     }
    // }

    // pthread_mutex_unlock(&tMutex); 
    // pthread_mutex_lock(&tMutex); 


    // //3. Bombe-Astronave -> Vite--
    for(i = 0; i < *enemies1Count; i++){
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
            //pthread_cancel(bombs[i].tid);
        }    
    }
    //pthread_mutex_unlock(&tMutex); 
    
    // pthread_mutex_unlock(&tMutex); 
    // pthread_mutex_lock(&tMutex); 

    // //5. Astronave - Nemici lv. 2 -> Vite --
    for(i = 0; i < *enemies1Count; i++){
        //$$ cambiare + 3 e +5 con le macro
        //pthread_mutex_lock(&tMutex); 
        if( range(astroship->x, astroship->x +5 , enemies2[i].x) &&
            range(astroship->y, astroship->y +5 , enemies2[i].y)       //$$ 3 e 3 sono parametri della dimensione da dare con define
            
            && enemies2[i].state == INITIALIZED
            
            ){
            *life -= 1;   

            //Distruzione nemico lv. 2
            //pthread_cancel(enemies2[i].tid);
            enemies2[i].state = DEAD;

        }
        
    }

    // pthread_mutex_unlock(&tMutex); 
    // pthread_mutex_lock(&tMutex); 

    //$$ 5 e 4 si possono unificare?

    //4. Nemico-Astronave -> Muore Astronave (life--)
    for(i = 0; i < *enemies1Count; i++){
        //$$ cambiare + 3 e +5 con le macro
        //pthread_mutex_lock(&tMutex); 
        if( range(astroship->x, astroship->x +5 , enemies1[i].x) &&
            range(astroship->y, astroship->y +5 , enemies1[i].y)       //$$ 3 e 3 sono parametri della dimensione da dare con define
            
            && enemies1[i].state == INITIALIZED
            
            ){
            *life -= 1;   

            //Distruzione nemico lv. 2
            //pthread_cancel(enemies2[i].tid);
            enemies1[i].state = DEAD;

        }
        
    }
    // for(i = 0; i < *enemies1Count; i++){
    //     //$$ cambiare + 3 e +5 con le macro
    //     //pthread_mutex_lock(&tMutex); 
    //     if( range(astroship->x, astroship->x +5 , enemies1[i].x) &&
    //         range(astroship->y, astroship->y +5 , enemies1[i].y)       //$$ 3 e 3 sono parametri della dimensione da dare con define
    //         ){
    //         *life -= 1;   

    //         //Distruzione nemico lv. 1
    //         //pthread_cancel(enemies1[i].tid);
    //         enemies1[i].state = DEAD;
    //     }
        
    // }
    

    //pthread_mutex_unlock(&tMutex);

}


/*

                                                                                                                        bstatus: 3, bID:333
       ||                               m2count: 1: 0                           e1status: 1                             bstatus: 1, bID:1
      <>===                             m2count: 2: 0                           e1status: 1 O                           bstatus: 1, bID:2
       ||                               m2count: 3: 0                           e1status: 1                             bstatus: 3, bID:3
      ]                                                                                                                 bstatus: 1, bID:4
                                                                                                                        bstatus: 1, bID:5
                                                                                                                        bstatus: 3, bID:6
                                                                                O                                       bstatus: 3, bID:7
                                       <=             O                                                                 bstatus: 1, bID:8
                                       oo-                                                                              bstatus: 1, bID:9
                                       <=                                                                               bstatus: 3, bID:10
                                       <=                                                                               bstatus: 3, bID:11
                                       oo-                                                                              bstatus: 1, bID:12

*/



int statusConditionsThread(bool life,
    object *enemies1, int enemies1Count,
    object *enemies2, int enemies2Count){

    int status = -1;
    //1. nelle condizioni, se l'array dei nemici hanno tutti pid = -1, allora imposta >> status = 1 (e loop = false)
    bool enemiesDead = false;//true;           
    int i;
    for (i = 0; i < enemies1Count; i++)
        if (enemies1[i].state == INITIALIZED) //nuova condizione da aggiungere //!(enemies1[i].state == DEAD )
            enemiesDead = true;

    for (i = 0; i < enemies1Count; i++)
        if (enemies2[i].state == INITIALIZED)
            enemiesDead = true;


    if(!enemiesDead)        //Se neanche un nemico è vivo, status == 1
        status = 1;

    //status = -1;
    //2. nelle condizioni, se life (perchè legato a doppia mandata col numero di astroship) è == 0, allora status = 3 (e loop false)
    if(life < 1)
        status = 3;

    //3.  nelle condizioni, se almeno un nemico arriva a bordo schermo, allora loop == false e status == 2
    // arrivare a abordo schermo => ciclo su coordinate dei nemici (x < 0 e  0 < y > DIMSCHERMO )
    for (i = 0; i < enemies1Count; i++)
        if (enemies1[i].x < 1 && enemies1[i].y > 0 && enemies1[i].y < SCREEN_H)
            status = 2;//status = i + 100;
    for (i = 0; i < enemies1Count; i++)
        if (enemies2[i].x < 1 && enemies2[i].y > 0 && enemies2[i].y < SCREEN_H)
            status = 2; //status = i + 1000000;

        //Status = -1 => si gioca ancora
        //Status = 0 => 
        //Status = 1 => si esce dal gioco come vincitori
        //Status = 2 => si esce perdenti (navicelle a sx)
        //Status = 3 => si esce perdenti (life == 0)
    return status;
}