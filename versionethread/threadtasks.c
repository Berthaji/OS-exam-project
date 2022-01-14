#include "threadtasks.h"

void tEngine(int lifes, int enemiesdim, int shotProb, int color){

    attron(COLOR_PAIR(color));

    clearScreens();
    
    missilesCount =  (int*) malloc (sizeof(int)); // * 1 è ridondante
    *missilesCount = 0;         //nb _,0; __,1

    enemies1Count =  (int*) malloc (sizeof(int));
    *enemies1Count = enemiesdim;         //nb _,0; __,1

    enemies2Count =  (int*) malloc (sizeof(int));
    *enemies2Count = 0;         //nb _,0; __,1      --0

    bombsCount =  (int*) malloc (sizeof(int));
    *bombsCount = 0;         //nb _,0; __,1

     missile2Count =  (int*) malloc (sizeof(int));       //Contatore del controllo del secondo missile contro il nemico di secondo livello
    *missile2Count = 0;         //nb _,0; __,1    
    
    status =  (int*) malloc (sizeof(int));
    *status = -1; 

    life =  (int*) malloc (sizeof(int));
    *life = lifes; 


    bool loops = true;           //Regolatore della condizione per per il continuo del loop e quindi delle condizioni di giocabilità
    

    /* - Allocazione spazio delle variabili globali - */
    //Allocazione spazio per la astronave
    astroship = (Object*) malloc (sizeof(Object) * 1);

    //Allocazione spazio per i missili
    missiles = (Object*) malloc (sizeof(Object) * 3); //Ho solo due missili

    //Allocazione spazio per gli M nemici di primo livello
    enemies1 = (Object*) malloc (sizeof(Object) * *enemies1Count);

    //Allocazione spazio per gli M nemici di secondo livello
    enemies2 = (Object*) malloc (sizeof(Object) * *enemies1Count); 
    //Allocazione spazio per le bombe
    bombs = (Object*) malloc (sizeof(Object) * *enemies1Count);

    //Allocazione spazio per il controllo sul secondo missile contro il nemico di secondo livello
    doubleMissile = (int *) malloc(sizeof(int) * *enemies1Count);

    pthread_mutex_init (&tMutex, NULL);

    ///Creazione dei thread per ogni elemento
    
    //Per astronave
    pthread_create(&astroship->tid, NULL, tastroship, (void *)astroship);

    //Per nemici liv. 1    
    int i;
    int x=0, y=1;
    for(i = 0; i < *enemies1Count; i++){
        pthread_mutex_lock(&tMutex);                   
        //inizializzo qui le coordinate di partenza per non far casino con variabili simili a static in tEnemy1
        enemies1[i].x = SCREEN_W * ENEMY_GENERATION_WIDTH + (x * ENEMY_GENERATION_DISTANCE_COLUMN);    
        enemies1[i].y = ENEMY_GENERATION_DISTANCE_LINES * y;
        enemies1[i].type = ENEMY1;
        enemies1[i].state = INITIALIZED;   
        enemies1[i].appearance = 2;   
        enemies1[i].dir = i % 2;      

        if (i % ENEMY_GENERATION_LINES == ENEMY_GENERATION_LINES -1 && i != 0){     /* Impostiamo 4 file di nemici */
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
    for(i = 0; i < N_MISSILES; i++){             //Questo 2 deve viventare una macro per tutto $$
        pthread_mutex_lock(&tMutex); 

        missiles[i].type = MISSILE;
        missiles[i].state = NOT_INITIALIZED;
        missiles[i].x = -1;
        missiles[i].y = -1;
        missiles[i].dir = i % 2;//% additional;//2;
        

        pthread_create(&(missiles[i].tid), NULL, tMissile, (void*)(&missiles[i]));
            
        pthread_mutex_unlock(&tMutex);   
    }


    
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

        


        pthread_mutex_lock(&tMutex); 
        //GENERAZIONE BOMBE
        //int shot = 0;
        if(rand()%100000 < shotProb){ //$$ oltre 30 da problemi di oggetti non cancellati
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
        }
        pthread_mutex_unlock(&tMutex);


        //Abilitazione condizioni di giocabilità (game Win o Game Over)
        pthread_mutex_lock(&tMutex); 
        *status = statusConditionsThread(*life, enemies1,*enemies1Count,enemies2);
        //pthread_mutex_unlock(&tMutex);
        if(*status > 0){
            loops = false;
            drawFinalScene(*status);
        }
        pthread_mutex_unlock(&tMutex);


    }

    // //Abbozzo di tEnd
    tEnd();
    
}

void* tastroship (void* parameters){
    Object* obj;                                    //Noi non possiamo usare direttamente parameters perchè di tipo void*
    obj = (Object*) parameters;                     //Ricorda che il cast è obbligatorio!
    int c;

    //Bloccaggio di un mutex
    pthread_mutex_lock(&tMutex);
    obj->type = ASTROSHIP;
    obj->appearance = 2;
    obj->hasShot = false;
    obj->state = INITIALIZED;
    obj->x = 1;
    obj->y = (SCREEN_H - 3) / 2;
    //Mettere gli altri parametri $$
    pthread_mutex_unlock(&tMutex);


    while (*status < 0){    //Se si chiude il resto del gioco, ha senso che anche il thread astronave si chiuda 
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
                
                pthread_mutex_unlock(&tMutex);
                break;
        }//Fine switch

  
    }
    return NULL;
    //exit(0);
}

//Missile
void* tMissile(void* parameters){
    Object* o;                                    //Noi non possiamo usare direttamente parameters perchè di tipo void*
    o = (Object*) parameters;                     //Ricorda che il cast è obbligatorio!  


    while(o->state != KILLED && *status < 0){
    
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
                }
            }

            o->x++;

            o->y += (o->dir ? -1 : 1) * SHOT_ANGLE_CORRECTION;  

            //Bordo schermo
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

            pthread_mutex_unlock(&tMutex);

            usleep(MISSILESLEEP);                 // un delay per evitare che il nemico vada troppo veloce  //usleep(ENEMYSLEEP);
        }
    }
    
    o->state = DEAD;
    o->x = -1;
    o->y = -1;
    
    return NULL;
}


void* tEnemy1 (void* parameters){
    Object* o;                                    //Noi non possiamo usare direttamente parameters perchè di tipo void*
    o = (Object*) parameters;                     //Ricorda che il cast è obbligatorio!  

    int loop=true;
    while (loop  && *status < 0){
        pthread_mutex_lock(&tMutex);

        o->x--;                   //sposto il nemico verso destra
        //o->y += 0;               //Si spostano nello stesso modo
        if(o->dir)
            o->dir = 0;
        else
            o->dir = 1;

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

        if (o->x < 0){
            loop = false;
        
            //pthread_mutex_lock(&tMutex);
            o->state = DEAD;
        }

        if (o->state == DEAD){      //Serve per quando la modificano al'esterno della funzione tBonb
            loop = false;
        }
        pthread_mutex_unlock(&tMutex);

        usleep(ENEMYSLEEP1);
    }     


    o->x = -1;
    o->y = -1;

    return NULL;
}




//Bombe//
void* tBombe(void* parameters){
    Object* o;                                    //Noi non possiamo usare direttamente parameters perchè di tipo void*
    o = (Object*) parameters;                     //Ricorda che il cast è obbligatorio!  

    bool loop = true;
    while (loop  && *status < 0){
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
        pthread_mutex_unlock(&tMutex); //Sblocchiamo il risultato della elaborazione
        usleep(BOMBSLEEP);
    }

    o->x = -1;
    o->y = -1;

   


    return NULL;
}


//Bombe//
void* tBombe2(void* parameters){
    Object* o;                                    //Noi non possiamo usare direttamente parameters perchè di tipo void*
    o = (Object*) parameters;                     //Ricorda che il cast è obbligatorio!  

   while(o->state != KILLED  && *status < 0){
        while(o->state == INITIALIZED){
            o->x--;
            if(o->x < 0){
                o->x = -1;
                o->x = -1;
                o->state = DEAD;
            }
            usleep(BOMBSLEEP);   
        }
    }


    return NULL;
}

//Enemies 2 //
void* tEnemy2(void* parameters){
    Object* o;                                    //Noi non possiamo usare direttamente parameters perchè di tipo void*
    o = (Object*) parameters;                     //Ricorda che il cast è obbligatorio!  

    //Impostiamo ogni parametro del nemico, ma in realtà dovrei usare il mutex perchè o è comunque un riferimento 
    // pthread_mutex_lock(&tMutex);
    while(o->state != KILLED  && *status < 0){
        while(o->state == INITIALIZED){
            o->x--;
            
            if(o->dir)
                o->dir = 0;
            else
                o->dir = 1;
            o->y += o->dir ? -1 : 1; //sposto il nemico in basso o in alto a seconda della sua direzione
            
            if(o->x < 0){
                o->x = -1;
                o->x = -1;
                o->state = DEAD;
            }
            usleep(ENEMYSLEEP2);   
        }
    }
    return NULL;
}



void tEnd(){
    refresh();
    pthread_cancel(astroship->tid);
    pthread_join(astroship->tid, NULL);
    
    sleep(2);   //Delay per permettere agli altri thread di chiudersi correttamente
    free(astroship);
    
    //free
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


void drawScenes(){

    clearScreens();
    drawObject(*astroship);

    int i;
    if(*enemies1Count > 0){
        for (i = 0; i < *enemies1Count; i++)
            if (enemies1[i].state == INITIALIZED) 
                drawObject(enemies1[i]);

        for (i = 0; i < *enemies1Count; i++)
            if (enemies2[i].state == INITIALIZED)
                drawObject(enemies2[i]);

        for (i = 0; i < *enemies1Count; i++)
            if (bombs[i].state == INITIALIZED)
                drawObject(bombs[i]);
    }

    for (i = 0; i < 2; i++)         //$$ il 2 deve diventare una macro
        if (missiles[i].state == INITIALIZED)//!= DEAD && missiles[i].state != KILLED)
            drawObject(missiles[i]);

    //Contatore vite
    mvprintw(0,0, "VITE: %d ", *life);

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

    //1. Missili-Nemici -> Nemici lv.2
    int i,j;
    for(i = 0; i < *enemies1Count; i++){
        for(j = 0; j < 2; j++){
            if( range(enemies1[i].x, enemies1[i].x +3 , missiles[j].x) &&
                range(enemies1[i].y, enemies1[i].y +3 , missiles[j].y) 
                && missiles[j].state == INITIALIZED
                && missiles[j].x != -1
                ){
                missiles[j].state = DEAD;

                enemies1[i].state = DEAD;

                enemies2[i].state = INITIALIZED;
                enemies2[i].x = enemies1[i].x-1;
                enemies2[i].y = enemies1[i].y;
            } 
        }
    }

    //2. Missili-Nemici lv.2 -> Muore Nemico
    for(i = 0; i < *enemies1Count; i++){
        if(doubleMissile[i] == 2){
            //Killa nemico e missile
            if (enemies2[i].state == INITIALIZED){
                enemies2[i].state = DEAD;
            }
        }
    }

    //3. Bombe-Astronave -> Vite--
    for(i = 0; i < *enemies1Count; i++){
        int shoting = astroCollided(*astroship,bombs[i]);
        if(bombs[i].state == INITIALIZED && shoting > 0){
            *life -= 1;   

            //Distruzione bomba
            bombs[i].state = DEAD;
        }    
    }

    //5. Astronave - Nemici lv. 2 -> Vite --
    for(i = 0; i < *enemies1Count; i++){
        if( range(astroship->x, astroship->x +ASTRODIM , enemies2[i].x) &&
            range(astroship->y, astroship->y +ASTRODIM , enemies2[i].y)      
            && enemies2[i].state == INITIALIZED){
            *life -= 1;   

            //Distruzione nemico lv. 2
            enemies2[i].state = DEAD;
        }
    }

    //4. Nemico-Astronave -> Muore Astronave (life--)
    for(i = 0; i < *enemies1Count; i++){
        if( range(astroship->x, astroship->x +ASTRODIM , enemies1[i].x) &&
            range(astroship->y, astroship->y +ASTRODIM , enemies1[i].y)
            && enemies1[i].state == INITIALIZED){
            *life -= 1;   

            //Distruzione nemico lv. 2
            enemies1[i].state = DEAD;
        }   
    }
}



int statusConditionsThread(bool life,
    Object *enemies1, int enemies1Count,
    Object *enemies2){

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
            status = 2; 

    //Status = -1 => si gioca ancora
    //Status = 0 => 
    //Status = 1 => si esce dal gioco come vincitori
    //Status = 2 => si esce perdenti (navicelle a sx)
    //Status = 3 => si esce perdenti (life == 0)
    return status;
}