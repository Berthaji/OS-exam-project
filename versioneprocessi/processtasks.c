/**
 * @file processtasks.c
 * @brief Libreria del motore grafico del gioco in versione processi/pipe
 */

#include "processtasks.h"

/**
 * @brief Gestore delle coordinate del player e della condizione di sparo
 * 
 * @param pipeOut pipe in cui scrivere i dati
 * @param o oggetto su cui scrivere
 */
void pAstroship(int pipeOut, Object o){
    char c = 'q';
    bool loop = true;
    while (loop){
        c = getch(); /* carattere inserito e quindi controllato */
        switch (c){
            case KEY_UP & A_CHARTEXT:
                if (o.y > 0)
                    o.y--;
                break;

            case KEY_DOWN & A_CHARTEXT:
                if (o.y < SCREEN_H - 5)
                    o.y++;
                break;
        
            case ' ': /* Barra spaziatrice */
                o.hasShot = true;
                break;
        }
        write(pipeOut, &o, sizeof(o)); /* Comunico al processo padre le coordinate */
        o.hasShot = false;
    }
    exit(0);
}


/**
 * @brief Gestore dei nemici di primo livello
 * 
 * @param pipeOut pipe in cui scrivere i dati
 * @param o oggetto su cui scrivere
 */
void pEnemy1(int pipeOut, Object o){
    bool loop = true;
    o.pid = getpid();

    while (loop){
        o.x--;                   /* sposto il nemico verso destra */
        if(o.dir)
            o.dir = 0;
        else
            o.dir = 1;
        
        o.y += o.dir ? -1 : 1; /* sposto il nemico in basso o in alto di continuo per dare più movimento*/

        /**
         * se il nemico è fuori dallo schermo dobbiamo terminare il processo
         * perciò terminiamo il loop, comunichiamo al processo principale che il nemico 
         * è "morto" (segnale di exit) e chiudiamo il processo nemico
         */

        /*se il nemico non è ai bordi si può muovere tranquillamente, altrimenti lo spostiamo più dentro */
        if (o.y >= SCREEN_H -2){    /* Bordo inferiore superato */
            o.y--;
            o.dir = 1;
        }
        if (o.y <= 0 ){    /* Bordo superiore superato */
            o.y++;
            o.dir = 0;
        }
        if (o.x < -5){
            loop = false;
            o.state = KILLED;
        }

        write(pipeOut, &o, sizeof(o)); /* comunichiamo il risultato dell'eleborazione */
        usleep(ENEMYSLEEP1);                 /*  un delay per evitare che il nemico vada troppo veloce */       
    }
    /* harakiri */
    o.state = KILLED;
    write(pipeOut, &o, sizeof(o));
    exit(0);
}

/**
 * @brief Gestore dei nemici di secondo livello
 * 
 * @param pipeOut pipe in cui scrivere i dati
 * @param o oggetto su cui scrivere
 */
void pEnemy2(int pipeOut, Object o){
    bool loop = true;
    o.pid = getpid();
    while (loop){
        o.x--;                   /* sposto il nemico verso destra*/
        
        if(o.dir)
            o.dir = 0;
        else
            o.dir = 1;
        
        /**
         * se il nemico è fuori dallo schermo dobbiamo terminare il processo
         * perciò terminiamo il loop, comunichiamo al processo principale che il nemico 
         * è "morto" (segnale di exit) e chiudiamo il processo nemico
         */

        /* se il nemico non è ai bordi si può muovere tranquillamente, altrimenti lo spostiamo più dentro */
        if (o.y >= SCREEN_H -2){    /* Bordo inferiore superato */
            o.y--;
            o.dir = 1;
        }
        if (o.y <= 0 ){    /* Bordo superiore superato */
            o.y++;
            o.dir = 0;
        }
        if (o.x < -5){ /* Bordo laterale superato */
            loop = false;
            o.state = KILLED;
        }

        write(pipeOut, &o, sizeof(o)); 
        usleep(ENEMYSLEEP2);                 /*  un delay per evitare che il nemico vada troppo veloce  */
    }
    /* harakiri  */
    exit(0);
}


/**
 * @brief Gestore bombe dei nemici
 * 
 * @param pipeOut pipe in cui scrivere i dati
 * @param o oggetto su cui scrivere
 */
void pBomb(int pipeOut, Object o){
    bool loop = true;

    while (loop){
        o.x--;                     /* sposto la bomba verso sinistra */
       
        /**
         * se il missile è fuori dallo schermo dobbiamo terminare il processo
         * perciò terminiamo il loop, comunichiamo al processo principale che il proiettile 
         * è "morto" e chiudiamo il processo missile
         */
        if (o.y < 0|| o.y > SCREEN_H || o.x < 0){
            loop = false;
            o.state = DEAD;
        }

        write(pipeOut, &o, sizeof(o)); /* comunichiamo il risultato dell'eleborazione */
        usleep(BOMBSLEEP);                 /*  un delay per evitare che la bomba vada troppo veloce */
    }
    /* harakiri */
    exit(0);
}

/**
 * @brief Questa funzione si occupa di gestire un missile lanciato dal player
 * 
 * @param pipeOut pipe in scrittura 
 * @param o i dati iniziali del proiettile
 */
void pMissile(int pipeOut, Object o){
    bool loop = true;
    while (loop){
        o.x++;                     /* sposto il missile verso sinistra */
       
        o.y += (o.dir ? -1 : 1) * SHOT_ANGLE_CORRECTION;      

        /**
         * se il missile è fuori dallo schermo dobbiamo terminare il processo
         * perciò terminiamo il loop, comunichiamo al processo principale che il proiettile 
         * è "morto" e chiudiamo il processo missile
         */
        if (o.y < -2 || o.y > SCREEN_H + 2 || o.x > SCREEN_W){
            loop = false;
            o.state = DEAD;
        }

        write(pipeOut, &o, sizeof(o)); /* comunichiamo il risultato dell'eleborazione */
        usleep(MISSILESLEEP);                 /*  un delay per evitare che il missile vada troppo veloce */
    }
    /* harakiri */
    o.state = KILLED;
    write(pipeOut, &o, sizeof(o));
    exit(0);
}

/**
 * @brief uesta funzione è il cuore del gioco nella versione processi, 
 * in essa vengono lette le informazioni prodotte dai processi,
 * gestita la logica di gioco e predisposto tutto per il disegno su schermo
 * 
 * @param life Contatore numero di vite
 * @param enemiesdim Numero di nemici da mostrare
 * @param shotProb Probabilità di lancio di una bomba da parte di un qualunque nemico
 * @param color Colore usato nel gioco
 */
void pEngine(int life, int enemiesdim, int shotProb, int color){
    attron(COLOR_PAIR(color));

    /* variabili di supporto */
    bool missilesShooted = false;
    int enemies1Count = enemiesdim;
    int enemies2Count = 0;
    int missile2Count = 1;      //Controllo secondo sparo missile per nemico lv.2
    int bombsCount = 0;
    int missilesCount = 0;

    int status = -1;
    bool loop = true;           /* Regolatore delle condizioni di giocabilità */
    
    Object *astroship = (Object *)malloc(sizeof(Object) * 1);
    Object *enemies1 = (Object *)malloc(sizeof(Object) * enemiesdim);
    Object *enemies2 = (Object *)malloc(sizeof(Object) * 1);
    Object *bombs    = (Object *)malloc(sizeof(Object) * 1);
    Object *missiles = (Object *)malloc(sizeof(Object) * missilesCount);

    bool *doubleMissile = (bool *) malloc(sizeof(bool)* missile2Count);      /* Array per contenere il fatto che il nemico di secondo livello sia stato attaccato 2 volte */
    doubleMissile[0] = false;   /* Prima inizializzazione */

    Object message; /* qui viene salvato il messaggio letto dalla pipe dei processi */
   
    /**
     * L'array seguente conterrà il file descriptor della pipe
     * Tenteremo un inizializzazione tramite la funzione pipe(), 
     * in caso di errore, il programma termina 
     */
    int fs[2];
    int pipeIn, pipeOut;
    if (pipe(fs) == -1)
        exit(-1);

    pipeIn = fs[0];         /* pipeIn -> Lettura  */
    pipeOut = fs[1];        /* pipeOut -> Scrittura  */



    /* Inizializzazione della astronave   */
    astroship->x = 1;
    astroship->y = (SCREEN_H - 3) / 2;
    astroship->type = ASTROSHIP;
    astroship->hasShot = false;
    astroship->color = color;
    astroship->state = INITIALIZED;
    astroship->appearance = rand() % ASTRODCHOICE;
    
    astroship->pid = fork();

    if (astroship->pid == 0){
        pAstroship(fs[1], *astroship);
    }
    else{   /* Siamo ancora nel processo padre */
        /* Generazione dei nemici di primo livello */
        int enemyappereace = rand()% (ENEMYDCHOICE -1);  /* L'ultimo aspetto è riservato ai nemici di secondo livello */
        int i,x,y;
        for (i = 0, x = 0, y = 1; i < enemies1Count; i++){
            enemies1[i].type = ENEMY1;
            enemies1[i].appearance = enemyappereace;
            enemies1[i].id = i;
            enemies1[i].state = INITIALIZED;
           
            /* Disegno dei nemici sullo schermo in modo  */
            enemies1[i].x = SCREEN_W * ENEMY_GENERATION_WIDTH + (x * ENEMY_GENERATION_DISTANCE_COLUMN);      
            enemies1[i].y = ENEMY_GENERATION_DISTANCE_LINES * y;
            if (i % ENEMY_GENERATION_LINES == ENEMY_GENERATION_LINES -1 && i != 0){     /* Impostiamo 4 file di nemici */
                y = 0;
                x++;
            }
            y++;
            
            enemies1[i].pid = fork();

            if (enemies1[i].pid == 0){
                enemies1[i].dir = i % 2;      /* Tutti in direzioni diverse  */
                pEnemy1(pipeOut, enemies1[i]);
            }
        }
    }
    
    sleep(1);
    while (loop){
        read(pipeIn, &message, sizeof(Object));
        /* in questo switch va gestita la logica del gioco */
        switch (message.type){
            case ASTROSHIP:{
                bool conditions = false;
                if(missilesShooted == false){
                    /* Non sono mai stati sparati missili: ha senso farli sparare */
                    conditions = true;
                    missilesShooted = true;
                }
                else{ /* Non  è la prima votla che si spara: vediamo se i precedenti sono morti*/
                     if(missiles[missilesCount-1].state != INITIALIZED 
                        && missiles[missilesCount-2].state != INITIALIZED ){
                         conditions = true;
                     }
                }

                astroship->type = message.type;
                astroship->x = message.x;
                astroship->y = message.y;
                astroship->appearance = message.appearance;
                
                /* Lancio dei due missili */
                if (conditions && message.hasShot){               /* Lancio dei due missili*/
                    missilesCount += 2;
                    missiles = (Object *)realloc(missiles, sizeof(Object) * missilesCount);
                    int i;
                    for (i = missilesCount - 2; i < missilesCount; i++){    /* Aggiunta dei due missili all'elenco*/
                        missiles[i].x = message.x + 5; 
                        missiles[i].y = message.y + 2;
                        missiles[i].type = MISSILE;
                        missiles[i].id = i;
                        missiles[i].state = INITIALIZED;
                        missiles[i].pid = fork();
                        if (missiles[i].pid == 0){
                            missiles[i].dir = i % 2;
                            pMissile(pipeOut, missiles[i]);
                        }
                    }
                }
                break;
            }

            case ENEMY1:{
                int id = message.id;

                if(enemies1[id].state != DEAD){
                    enemies1[id].x = message.x;
                    enemies1[id].y = message.y;
                    /* Blocco per la gestione del lancio delle bombe*/
                    if( (rand()%1000) < shotProb){        
                        bombsCount++;
                        bombs = (Object *)realloc(bombs, sizeof(Object) * bombsCount);
                        int i;
                        for (i = bombsCount - 1; i < bombsCount; i++){
                            bombs[i].x = message.x - 1; 
                            bombs[i].y = message.y + 1;
                            bombs[i].type = BOMB;
                            bombs[i].id = i;
                            bombs[i].pid = fork();
                            if (bombs[i].pid == 0){
                                bombs[i].dir = i % 2;
                                pBomb(pipeOut, bombs[i]);
                            }
                        }
                    }
                    
                    /* Controllo dell'incrocio con l'oggetto astronave */
                    int match = astroCollided(*astroship,enemies1[id]); 
                    if(match > -1){  /* Match nemico astronave*/
                        life--;
                        
                        /* Ammazzo il nemico colpito*/
                        enemies1[id].y = 0;
                        enemies1[id].x = 0;
                        enemies1[id].state = DEAD;
                    }
                }
                
                break;
            }

            case BOMB:{
                int id = message.id;
                bombs[id].x = message.x;
                bombs[id].y = message.y;
                bombs[id].state = message.state;

                /* Controllo dell'incrocio con l'oggetto astronave*/
                int match = astroCollided(*astroship,bombs[id]); 
                if(match > -1){  /* Match nemico astronave */
                    life--;
                    /* Ammazzo il nemico colpito */
                    if(bombs[id].state != DEAD){  
                        bombs[id].y = -1;
                        bombs[id].x = -1;
                        bombs[id].state = DEAD;
                    }
                }
                break;
            }
            
            case MISSILE:{
                /* Lancia i missili*/
                
                int id = message.id;
                missiles[id].x = message.x;
                missiles[id].y = message.y;

                bool isMissileAlive = false;
                if(missiles[id].state > ALIVE && message.state < ALIVE){
                    //abbiamo in entrata un missile inizializzato
                    //me ne abbiamo salvato uno morto
                    //Quindi lo ignoriamo
                    isMissileAlive = false;
                }
                else{
                    isMissileAlive = true;
                    missiles[id].state = message.state;
                }

                if(isMissileAlive){
                    /* Controllo delle collisioni coi nemici di primo livello */
                    int enemyid = missileCollided(enemies1,missiles[id],enemies1Count); 
                    if(enemyid > -1 ){  /* Match nemico missile*/
                        if(enemies1[enemyid].state != DEAD){  
                            /* Ammazzo il nemico solo se viene colpito una volta (ossia dal primo missile)*/
                            
                            /* Ammazzo il missile stesso*/
                            missiles[id].y = -1;
                            missiles[id].x = -1;
                            missiles[id].state = DEAD;

                            /* Gwnerazione nemico lv 2 */
                            enemies2Count++;
                            enemies2 = (Object *)realloc(enemies2, sizeof(Object) * enemies2Count);

                            missile2Count++;
                            doubleMissile = (bool *)realloc(doubleMissile, sizeof(bool) * missile2Count);
                            doubleMissile[missile2Count-1] = false;

                            int i;
                            for (i = enemies2Count - 1; i < enemies2Count; i++){
                                enemies2[i].x = enemies1[enemyid].x-1;
                                enemies2[i].y = enemies1[enemyid].y; 
                                enemies2[i].type = ENEMY2;
                                enemies2[i].state = INITIALIZED;
                                enemies2[i].appearance = 3;
                                enemies2[i].id = i;
                                enemies2[i].pid = fork();       
                                if (enemies2[i].pid == 0){
                                    enemies2[i].dir = 0;
                                    pEnemy2(pipeOut, enemies2[i]);
                                }
                            }
                                                    
                            /* Ammazzo il nemico colpito */
                            enemies1[enemyid].y = 0;
                            enemies1[enemyid].x = 0;
                            enemies1[enemyid].state = DEAD;
                        }
                    }

                    /* Controllo delle collisioni coi nemici di secondo livello */
                    /* Controlliamo se è stato colpito 2 volte */
                    int enemy2id = missileCollided(enemies2,missiles[id],enemies2Count);

                    if(doubleMissile[enemy2id] == true && enemy2id > -1 ){  
                        /* Match nemico2 missile (colpito almeno 2 volte) */
                        /* Ammazzo il nemico solo se viene colpito una volta (ossia dal primo missile) */
                        enemies2[enemy2id].y = 0;
                        enemies2[enemy2id].x = 0;
                        enemies2[enemy2id].state = DEAD;

                        /* Ammazzo il missile stesso */
                        missiles[id].y = -1;
                        missiles[id].x = -1;
                        missiles[id].state = DEAD;
                    }
                    if(doubleMissile[enemy2id] == false && enemy2id > -1 && enemies2[enemy2id].state == INITIALIZED){         
                        /*  Un nemico lv. 2 Non è mai stato colpito prima, impostiamo su true */
                        doubleMissile[enemy2id] = true;

                        /* Ammazzo il missile stesso */
                        missiles[id].y = -1;
                        missiles[id].x = -1;
                        missiles[id].state = DEAD;
                    }
                }
                break;
            }

            case ENEMY2:{
                int id = message.id;

                /* Test per vedere se nella pipe è rimasto un nemico 2 per sbaglio */
                if(!(enemies2[id].pid == -1)){ 
                    /* Dovresti essere morto, sono gli ultimi elementi rimasti nella pipe, ti ignoro */
                    enemies2[id].x = message.x;
                    enemies2[id].y = message.y; 

                    /* Blocco per la gestione del lancio delle bombe */
                    if( (rand()%1000) < shotProb){        
                        bombsCount += 1;

                        bombs = (Object *)realloc(bombs, sizeof(Object) * bombsCount);
                        int i;
                        for (i = bombsCount - 1; i < bombsCount; i++){
                            bombs[i].x = message.x - 1; 
                            bombs[i].y = message.y + 1;
                            bombs[i].type = BOMB;
                            bombs[i].id = i;
                            bombs[i].pid = fork();
                            if (bombs[i].pid == 0){
                                bombs[i].dir = i % 2;
                                pBomb(pipeOut, bombs[i]);
                            }
                        }
                    }

                    /* Controllo dell'incrocio con l'oggetto astronave */
                    int match = astroCollided(*astroship,enemies2[id]); 
                    if(match > -1){  /* Match nemico astronave*/
                        life--;

                        /* Kill del nemico stesso*/
                        enemies2[id].y = 0; 
                        enemies2[id].x = 0;
                        enemies2[id].state = DEAD;

                        if(astroship->state != DEAD){  /* Ammazzo astronave solo se viene colpito una volta*/
                            /* Ammazzo l'astronave stessa */
                            astroship->y = -1;
                            astroship->x = -1;
                            astroship->state = DEAD;                                            
                        }   
                    }
                }
            }
        }
        
        /* Controllo delle confizioni di giocabilità    
            LEGGENDA     
         Status = -1 => si gioca ancora
         Status = 0 => 
         Status = 1 => si esce dal gioco come vincitori
         Status = 2 => si esce perdenti (navicelle a sx)
         Status = 3 => si esce perdenti (life == 0) */

        /* Funzioni di disegno */
        drawScene(astroship, enemies1, enemies1Count, enemies2, enemies2Count, missiles, missilesCount, bombs, bombsCount);
        mvprintw(0,0, "VITE: %d", life);

        refresh();
        
        status = statusConditions(life, enemies1, enemies1Count, enemies2, enemies2Count);
        if(status > 0){
            loop = false;
            clearScreen();
            refresh();
            sleep(1);
            drawFinalScene(status);
        }
        
        /* Liberazione eventuali risorse */
        pClean(enemies1, enemies1Count, enemies2, enemies2Count, missiles, missilesCount, bombs, bombsCount);    
    }

    /* funzione killa robe */
    pEnd(astroship, enemies1, enemies1Count, enemies2, enemies2Count, missiles, missilesCount, bombs, bombsCount, fs);

}

/**
 * @brief Funzione per la pulizia delle risorse ad ogni ciclo
 * 
 * @param enemies1 Nemici di primo livello
 * @param enemies1Count Contatore nemici primo livello
 * @param enemies2 Nemici di secondo livello
 * @param enemies2Count Contatore nemici secondo livello
 * @param missiles Missili
 * @param missilesCount Contatore nemici
 * @param bombs Bombe
 * @param bombsCount Contatore bombe
 */
void pClean( 
    Object *enemies1, int enemies1Count,
    Object *enemies2, int enemies2Count,
    Object *missiles, int missilesCount,
    Object *bombs, int bombsCount){

    /* Clean dei missili morti (funziona a parte da richiamare!) */
    int i;
    for (i = 0; i < missilesCount; i++)
        if(missiles[i].state == KILLED)
            if(!kill(missiles[i].pid,SIGKILL)){
                waitpid(-1, NULL, WNOHANG);
                wait(NULL);
            }

    /* Bombe        */
    for (i = 0; i < bombsCount; i++)
        if(bombs[i].state == KILLED)
            if(!kill(bombs[i].pid,SIGKILL)){
                waitpid(-1, NULL, WNOHANG);
                wait(NULL);
            }

    /* Nemici lv.1*/
    for (i = 0; i < enemies1Count; i++)
        if(enemies1[i].state == KILLED)
            if(!kill(enemies1[i].pid,SIGKILL)){
                waitpid(-1, NULL, WNOHANG);
                wait(NULL);
            }

    /* Nemici lv.2*/
    for (i = 0; i < enemies2Count; i++)
        if(enemies2[i].state == KILLED)
            if(!kill(enemies2[i].pid,SIGKILL)){
                waitpid(-1, NULL, WNOHANG);
                wait(NULL);
            }
  
}

/**
 * @brief Funzione per chiudere ogni cosa aperta
 * 
 * @param astroship oggetto astronave
 * @param enemies1 lista nemici lv. 1
 * @param enemies1Count contatore nemici lv. 1
 * @param enemies2 lista nemici lv. 1
 * @param enemies2Count contatore nemici lv. 2
 * @param missiles lista missili
 * @param missilesCount contatore missili
 * @param bombs lista bombe
 * @param bombsCount contatore bombe
 * @param fs file descriptor
 */
void pEnd( 
    Object *astroship,
    Object *enemies1, int enemies1Count,
    Object *enemies2, int enemies2Count,
    Object *missiles, int missilesCount,
    Object *bombs, int bombsCount, 
    int fs[])
{
    
    clearScreen();
    close(fs[0]);
    close(fs[1]);

    if(!kill(astroship->pid,SIGKILL)){
        waitpid(-1, NULL, WNOHANG);
        wait(NULL);
    }
    refresh();

    /* Ammazzare in areaGioco ciò che è già morto */
    int i;   
    for (i = 0; i < missilesCount; i++){
        if(!kill(missiles[i].pid,SIGKILL)){
            waitpid(-1, NULL, WNOHANG);
            wait(NULL);
        }
        refresh();
    }

    
    for (i = 0; i < enemies1Count; i++){
        if(!kill(enemies1[i].pid,SIGKILL)){
            waitpid(-1, NULL, WNOHANG);
            wait(NULL);
        }
        refresh();
    }

    for (i = 0; i < enemies2Count; i++){
        if(!kill(enemies2[i].pid,SIGKILL)){
            waitpid(-1, NULL, WNOHANG);
            wait(NULL);
        }
        refresh();
    }

    for (i = 0; i < bombsCount; i++){
        if(!kill(bombs[i].pid,SIGKILL)){
            waitpid(-1, NULL, WNOHANG);
            wait(NULL);
        }
        refresh();
    }
       
    free(astroship);
    free(enemies1);
    free(enemies2);
    free(bombs);
    free(missiles);

}


/**
 * @brief Funzioni per il controllo delle condizioni di uscita o continuo del gioco
 * 
 * @param life Contatore vire
 * @param enemies1 Nemici di primo livello
 * @param enemies1Count Contatore nemici di secondo livello
 * @param enemies2 Nemici di secondo livello
 * @param enemies2Count Contatore nemici di secondo livello
 * @return int Indice di ritorno della codnzioni di gioco
 */
int statusConditions(bool life,
    Object *enemies1, int enemies1Count,
    Object *enemies2, int enemies2Count){

    int status = -1;

    /* Tutti i nemici sono morti ? */
    bool enemiesDead = false;
    int i;
    for (i = 0; i < enemies1Count; i++)
        if (enemies1[i].state == INITIALIZED)
            enemiesDead = true;
    for (i = 0; i < enemies2Count; i++)
        if (enemies2[i].state == INITIALIZED)
            enemiesDead = true;

    if(!enemiesDead)        /* Se neanche un nemico è vivo, status == 1 */
         status = 1;


    /* 2. nelle condizioni, se life (perchè legato a doppia mandata col numero di astroship) è == 0, allora status = 3 (e loop false) */
    if(life < 1)
        status = 3;

    /* 3.  nelle condizioni, se almeno un nemico arriva a bordo schermo, allora loop == false e status == 2
      arrivare a abordo schermo => ciclo su coordinate dei nemici (x < 0 e  0 < y > DIMSCHERMO ) */

    for (i = 0; i < enemies1Count; i++)
        if (enemies1[i].x < 1 && enemies1[i].y > 0 && enemies1[i].y < SCREEN_H && enemies1[i].state==INITIALIZED)
            status = 2;
    for (i = 0; i < enemies2Count; i++)
        if (enemies2[i].x < 1 && enemies2[i].y > 0 && enemies2[i].y < SCREEN_H && enemies2[i].state==INITIALIZED)
            status = 2; 

        /* LEGGENDA:
            Status = -1 => si gioca ancora
          Status = 0 => 
          Status = 1 => si esce dal gioco come vincitori
          Status = 2 => si esce perdenti (navicelle a sx)
          Status = 3 => si esce perdenti (life == 0) */
    return status;
}



