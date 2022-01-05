/**
 * @file task.c
 * @author Concas Filippo (f.concas24@studenti.unica.it)
 * @brief Manage the game in both process and thread versions
 * @version 0.1
 * @date 2021-12-21
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "task.h"

/**
 * @brief pAstroship è la funzione che gestisce il player
 * prende in ingresso la pipe in scrittura poichè deve solo produrre dati
 */
void pAstroship(int pipeOut, object o){

    char c = 'q';
    while (true){
        c = getch(); //carattere inserito e quindi controllato
        switch (c){
            case KEY_UP & A_CHARTEXT:
                if (o.y > 0)
                    o.y--;
                break;

            case KEY_DOWN & A_CHARTEXT:
                if (o.y < SCREEN_H - 5)
                    o.y++;
                break;

            //$$ URGENTE - NON COMMITTARE
            case KEY_LEFT  & A_CHARTEXT:
            if (o.x > 0)
                o.x--;

            break;

            case KEY_RIGHT  & A_CHARTEXT:
            if (o.x < SCREEN_W - 5)
                o.x++;
            break;
        
            case ' ': //Barra spaziatrice
                //alarm(1);
                
                o.hasShot = true;
                break;

            default:
                break;
        }
        //Scrivo solo se ho sparato o mi sono mosso, cosi risparmio sull'uso della pipe
        if(o.hasShot || c != 'q')
            write(pipeOut, &o, sizeof(o)); /* Comunico al processo padre le coordinate del contadino */
        
        


        o.hasShot = false;
        c = 'q';


    }

}

void pEnemy1(int pipeOut, object o){
    bool loop = true;
    o.pid = getpid();

    while (loop){
        o.x--;                   //sposto il nemico verso destra
        o.y += o.dir ? -1 : 1; //sposto il nemico in basso o in alto a seconda della sua direzione (i nemici vanno su e giu)
        //o.y += 0;               //Si spostano nello stesso modo

        //o.dir == 0 --> 1
        //o.dir == 1 --> -1

        /**
         * se il nemico è fuori dallo schermo dobbiamo terminare il processo
         * perciò terminiamo il loop, comunichiamo al processo principale che il nemico 
         * è "morto" e chiudiamo il processo nemico
         */

        //se il nemico non è ai bordi si può muovere tranquillamente, altrimenti lo spostiamo più dentro
        if (o.y >= SCREEN_H -2){    //Bordo inferiore superato
            o.y--;
            o.dir = 1;
        }
        if (o.y <= 0 ){    //Bordo superiore superato
            o.y++;
            o.dir = 0;
        }

        if (o.x < -5){
            loop = false;
            o.state = KILLED;
        }

        //o.dir = direction;
        write(pipeOut, &o, sizeof(o)); //comunichiamo il risultato dell'eleborazione
        usleep(100000);                 // un delay per evitare che il nemico vada troppo veloce           //usleep(ENEMYSLEEP);
    }
    //harakiri
    kill(getpid(), SIGKILL);
}


void pEnemy2(int pipeOut, object o){
    bool loop = true;
    o.pid = getpid();
    while (loop){
        //o.dir = direction;

        o.x--;                   //sposto il nemico verso destra
        o.y += 0;               //Si spostano nello stesso modo
        //o.y += o.dir ? -1 : 1; //sposto il nemico in basso o in alto a seconda della sua direzione

        //o.dir == 0 --> 1
        //o.dir == 1 --> -1

        /**
         * se il nemico è fuori dallo schermo dobbiamo terminare il processo
         * perciò terminiamo il loop, comunichiamo al processo principale che il nemico 
         * è "morto" e chiudiamo il processo nemico
         */

        //se il nemico non è ai bordi si può muovere tranquillamente, altrimenti lo spostiamo più dentro
        if (o.y >= SCREEN_H -2){    //Bordo inferiore superato
            o.y--;
            o.dir = 1;
        }
        if (o.y <= 0 ){    //Bordo superiore superato
            o.y++;
            o.dir = 0;
        }

        if (o.x < -5){
            loop = false;
            o.state = DEAD;
        }

        //o.dir = direction;

        write(pipeOut, &o, sizeof(o)); //comunichiamo il risultato dell'eleborazione
        //usleep(100000);                 // un delay per evitare che il nemico vada troppo veloce
        //sleep(2);
        
        //usleep(ENEMYSLEEP);
        usleep(100000);
        
        
    }
    //harakiri
    kill(getpid(), SIGKILL);


}


/**
 * @brief 
 * 
 * @param filedescriptor 
 * @param o 
 */
void pBomb(int pipeOut, object o){
    bool loop = true;

    while (loop){
        o.x--;                     //sposto la bomba verso sinistra
        //o.y += o.dir ? -1 : 1; //sposto il missile in basso o in alto a seconda della sua direzione
        o.y += 0;
        /**
         * se il missile è fuori dallo schermo dobbiamo terminare il processo
         * perciò terminiamo il loop, comunichiamo al processo principale che il proiettile 
         * è "morto" e chiudiamo il processo missile
         */
        if (o.y < 0|| o.y > SCREEN_H || o.x < 0){
            loop = false;
            o.state = DEAD;
        }

        write(pipeOut, &o, sizeof(o)); //comunichiamo il risultato dell'eleborazione
        usleep(50000);                 // un delay per evitare che la bomba vada troppo veloce
    }
    //harakiri
    kill(getpid(), SIGKILL);
}

/**
 * @brief Questa funzione si occupa di gestire un missile lanciato dal player
 * 
 * @param pipeOut pipe in scrittura 
 * @param o i dati iniziali del proiettile
 */
void pMissile(int pipeOut, object o){

    bool loop = true;
    while (loop){

        o.x++;                     //sposto il missile verso sinistra
        //o.y += 0; //o.dir ? -1 : 1; //sposto il missile in basso o in alto a seconda della sua direzione
        o.y += (o.dir ? -1 : 1)*0.625;      //$$diagonale vera

        /**
         * se il missile è fuori dallo schermo dobbiamo terminare il processo
         * perciò terminiamo il loop, comunichiamo al processo principale che il proiettile 
         * è "morto" e chiudiamo il processo missile
         */
        if (o.y < -2 || o.y > SCREEN_H + 2){
            loop = false;
            o.state = DEAD;
        }

        write(pipeOut, &o, sizeof(o)); //comunichiamo il risultato dell'eleborazione
        usleep(30000);                 // un delay per evitare che il missile vada troppo veloce
    }
    //harakiri
    kill(getpid(), SIGKILL);
}

/**
 * @brief Questa funzione è il cuore del gioco nella versione processi, 
 * in essa vengono lette le informazioni prodotte dai processi,
 * gestita la logica di gioco e predisposto tutto per il disegno su schermo
 */
void pEngine(int life, int enemiesdim, int shotProb){
    //contatori
    //life = 3;
    int enemies1Count = enemiesdim;
    int enemies2Count = 0;
    int missile2Count = 0;
    int bombsCount = 0;
    int missilesCount = 0;

    int status = -1;
    bool loop = true;           //Regolatore della condizione per per il continuo del loop e quindi delle condizioni di giocabilità
    
    object *astroship = (object *)malloc(sizeof(object) * 1);
    object *enemies1 = (object *)malloc(sizeof(object) * enemiesdim);
    object *enemies2 = (object *)malloc(sizeof(object) * 1);
    object *bombs    = (object *)malloc(sizeof(object) * 1);
    object *missiles = (object *)malloc(sizeof(object) * missilesCount);

    bool *doubleMissile = (bool *) malloc(sizeof(bool)*1);      //Array per contenere il fatto che il nemico di secondo livello sia stato attaccato 2 volte

    //variabili di supporto
    object message; //qui viene salvato il messaggio letto dalla pipe dei processi
    //bool loop = true; //controlla l'esecuzione del while principale
    //int status = -1; //Tipologia di uscita (abbiamo vinto, perso per nemici a bordo schermo...)make all
   
    /**
     * L'array seguente conterrà il file descriptor della pipe
     * Tenteremo un inizializzazione tramite la funzione pipe(), 
     * in caso di errore, il programma termina 
     */
    int fs[2];
    int pipeIn, pipeOut;
    if (pipe(fs) == -1)
        exit(-1);

    pipeIn = fs[0];         //pipeIn -> Lettura
    pipeOut = fs[1];        //pipeOut -> Scrittura

    
    //Inizializzare un colore per il gioco
    int color = (rand()%7)+1;
    attron(COLOR_PAIR(color));

    //Inizializzazione della astronave  
   

    astroship->x = 1;
    astroship->y = (SCREEN_H - 3) / 2;
    astroship->type = ASTROSHIP;
    astroship->hasShot = false;
    astroship->color = color;
    astroship->appearance = rand() % ASTRODCHOICE;
    
    /**
     * Creiamo il processo del giocatore utilizzando la fork
     * se la fork restituisce 0 significa che siamo nel processo figlio
     * per cui dirigiamo l'esecuzione verso la funzione relativa
     * Altrimenti continuiamo con il codice principale
     */
    //pid_t player_pid = fork(); PID: 7823
                        //PID: 7836

    //bombs[i].pid = fork();


    astroship->pid = fork();

    if (astroship->pid == 0){
        //astroship->pid = getpid();
        pAstroship(fs[1], *astroship);
    }

    else{
        //sleep(1);
        //Processo padre

        //Generazione dei nemici di primo livello (Stesso aspetto per tutti i nemici)
        
        int enemyappereace = rand()% (ENEMYDCHOICE -1); //L'ultimo aspetto è riservato ai nemici di secondo livello
        for (int i = 0, x = 0, y = 1; i < enemies1Count; i++){
            enemies1[i].type = ENEMY1;
            enemies1[i].appearance = enemyappereace;
            enemies1[i].id = i;
           
            //Disegno dei nemici sullo schermo in modo ordinato
            enemies1[i].x = SCREEN_W * 0.75 + (x * 5);      //$$ Forzare il terminale grande almeno 32 per i nemici
            enemies1[i].y = 7 * y;
            if (i % 4 == 3 && i != 0){                      //Impostiamo 4 file di nemici $$
                y = 0;
                x++;
            }
            y++;
            
            enemies1[i].pid = fork();

            if (enemies1[i].pid == 0){
                enemies1[i].dir = i % 2;      //Tutti in direzioni diverse
                //enemies1[i].dir = 1;            //Tutti nella stessa direzione
                
                pEnemy1(pipeOut, enemies1[i]);
            }
        }
    }


    sleep(1);

    //loop del gioco, you dont say
    while (loop){
        
        read(pipeIn, &message, sizeof(object));

        //in questo switch va gestita la logica del gioco
        switch (message.type){
            case ASTROSHIP:{
                astroship->type = message.type;
                astroship->x = message.x;
                astroship->y = message.y;
                astroship->appearance = message.appearance;
                
                //Lancio dei due missili
                if (message.hasShot){                   //Lancio dei due missili
                    missilesCount += 2;
                    // if(missiles == NULL)
                    //     missiles = (object *)malloc(sizeof(object) * missilesCount);
                    // else
                    missiles = (object *)realloc(missiles, sizeof(object) * missilesCount);

                    for (int i = missilesCount - 2; i < missilesCount; i++){    //Aggiunta dei due missili all'elenco
                        missiles[i].x = message.x + 5; 
                        missiles[i].y = message.y + 2;
                        missiles[i].type = MISSILE;
                        missiles[i].id = i;
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
                //Controllo per vedere se ha evaso l'area di gioco
                //if(message.state == KILLED)
                    //status = 2;

                int id = message.id;
                
                enemies1[id].x = message.x;
                enemies1[id].y = message.y;

                //Blocco per la gestione del lancio delle bombe
                if( (rand()%1000) < shotProb){        
                    bombsCount ++;

                    bombs = (object *)realloc(bombs, sizeof(object) * bombsCount);

                    for (int i = bombsCount - 1; i < bombsCount; i++){
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
            
                
                //Controllo dell'incrocio con l'oggetto astronave
                int match = AstroCollided(*astroship,enemies1[id]); 
                if(match > -1){  //Match nemico astronave
                    life--;
                    
                    //Ammazzo il nemico colpito
                    //mvprintw(10+match,0, "PID AS: %d - STATE: %d", enemies1[id].pid, enemies1[id].state);
                    refresh();

                    kill(enemies1[id].pid, SIGKILL);
                    //wait(NULL);
                    enemies1[id].y = 0;
                    enemies1[id].x = 0;
                    enemies1[id].state = DEAD;

                    //Ammazzo l'astronave stessa
                    if(astroship->state != DEAD){  //Ammazzo astronave solo se viene colpito una volta
                        //Ammazzo l'astronave stessa
                        kill(astroship->pid, SIGKILL);
                        astroship->y = -1;
                        astroship->x = -1;
                        astroship->state = DEAD;                                            

                    }  

                    //Da reinizializzare tutto
                    // if(life == 0){
                    //     pEnd(astroship, enemies1, enemies1Count, enemies2, enemies2Count, missiles, missilesCount, bombs, bombsCount);
                    //     pEngine(life);
                    // }

                    
                }


                if (message.state == DEAD){
                    enemies1[id].pid = -1;
                }

                break;
            }

            case BOMB:{
                int id = message.id;
                bombs[id].x = message.x;
                bombs[id].y = message.y;
                bombs[id].state = message.state;


                //Controllo dell'incrocio con l'oggetto astronave
                int match = AstroCollided(*astroship,bombs[id]); 
                if(match > -1){  //Match nemico astronave
                    life--;
                    //Ammazzo il nemico colpito
                    if(bombs[id].state != DEAD){  
                        kill(bombs[id].pid, SIGKILL);
                        bombs[id].y = -1;
                        bombs[id].x = -1;
                        bombs[id].state = DEAD;
                    }
                    
                    // if(astroship->state != DEAD){  //Ammazzo astronave solo se viene colpito una volta
                    //     //Ammazzo l'astronave stessa
                    //     kill(astroship->pid, SIGKILL);
                    //     astroship->y = -1;
                    //     astroship->x = -1;
                    //     astroship->state = DEAD;                                            

                    // }

                    // if(life == 0){
                    //     pEnd(astroship, enemies1, enemies1Count, enemies2, enemies2Count, missiles, missilesCount, bombs, bombsCount);
                    //     pEngine(life);
                    // }
                }




                //
                if (message.state == DEAD){
                    bombs[id].pid = -1;
                }
                break;
            }
            
            case MISSILE:{
                int id = message.id;
                missiles[id].x = message.x;
                missiles[id].y = message.y;
                missiles[id].state = message.state;

                //Controllo delle collisioni coi nemici di primo livello
                int enemyid = MissileCollided(enemies1,missiles[id],enemies1Count); 
                if(enemyid > -1){  //Match nemico missile
                    if(enemies1[enemyid].state != DEAD){  //Ammazzo il nemico solo se viene colpito una volta (ossia dal primo missile)
                        //Ammazzo il missile stesso
                        kill(missiles[id].pid, SIGKILL);
                        missiles[id].y = -1;
                        missiles[id].x = -1;
                        missiles[id].state = DEAD;

                        

                        enemies2Count++;
                        enemies2 = (object *)realloc(enemies2, sizeof(object) * enemies2Count);

                        missile2Count++;
                        doubleMissile = (bool *)realloc(doubleMissile, sizeof(bool) * missile2Count);
                        doubleMissile[missile2Count] = false;


                        for (int i = enemies2Count - 1; i < enemies2Count; i++){
                            enemies2[i].x = enemies1[enemyid].x-1; //message.x; 
                            enemies2[i].y = enemies1[enemyid].y; //message.y; 
                            enemies2[i].type = ENEMY2;
                            enemies2[i].appearance = 3;
                            enemies2[i].id = i;
                            enemies2[i].pid = fork();
                            if (enemies2[i].pid == 0){
                                enemies2[i].dir = 0;
                                pEnemy2(pipeOut, enemies2[i]);
                            }
                        }
                        
                                               
                        //Ammazzo il nemico colpito
                        kill(enemies1[enemyid].pid, SIGKILL);
                        enemies1[enemyid].y = 0;
                        enemies1[enemyid].x = 0;
                        enemies1[enemyid].state = DEAD;

                        // printf("\n");
                        // printf("\a");
                        // printf("\n");

                    }
                }


                //Controllo delle collisioni coi nemici di secondo livello
                //Controlliamo se è stato colpito 2 volte
                int enemy2id = MissileCollided(enemies2,missiles[id],enemies2Count);


                if(doubleMissile[enemy2id] == true && enemy2id > -1){  //Match nemico2 missile (colpito almeno 2 volte)
                    if(enemies2[enemy2id].state != DEAD){  //Ammazzo il nemico solo se viene colpito una volta (ossia dal primo missile)

                        kill(enemies2[enemy2id].pid, SIGKILL);
                        enemies2[enemy2id].y = 0;
                        enemies2[enemy2id].x = 0;
                        enemies2[enemy2id].state = DEAD;
                        
                        //Se noti che il nemico2 muore subito, forse nella pipe è rimasto ancora qualche oggetto missile

                        printf("\n");
                        printf("\a");
                        printf("\n");
                    }
                }

                if(doubleMissile[enemy2id] == false && enemy2id > -1){         // Un nemico lv. 2 Non è mai stato colpito prima, impostiamo su true
                    doubleMissile[enemy2id] = true;

                    //Ammazzo il missile stesso
                    kill(missiles[id].pid, SIGKILL);
                    missiles[id].y = -1;
                    missiles[id].x = -1;
                    missiles[id].state = DEAD;
                }

                if (message.state == DEAD){
                    missiles[id].pid = -1;
                }
                break;
            }

            case ENEMY2:{
                int id = message.id;

                //Test per vedere se nella pipe è rimasto un nemico 2 per sbaglio
                if(!(enemies2[id].pid == -1)){ //Dovresti essere morto, sono gli ultimi elementi rimasti nella pipe, ti ignoro
                
                    enemies2[id].x = message.x;
                    enemies2[id].y = message.y; 

                    //Blocco per la gestione del lancio delle bombe
                    if( (rand()%1000) < shotProb){        
                        bombsCount += 1;

                        bombs = (object *)realloc(bombs, sizeof(object) * bombsCount);

                        for (int i = bombsCount - 1; i < bombsCount; i++){
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


                    //Controllo dell'incrocio con l'oggetto astronave
                    int match = AstroCollided(*astroship,enemies2[id]); 
                    if(match > -1){  //Match nemico astronave
                        life--;

                        //Kill del nemico stesso
                        kill(enemies2[id].pid, SIGKILL);
                        enemies2[id].y = 0; //-5;
                        enemies2[id].x = 0;
                        enemies2[id].state = DEAD;

                        
                        if(astroship->state != DEAD){  //Ammazzo astronave solo se viene colpito una volta
                            //Ammazzo l'astronave stessa
                            kill(astroship->pid, SIGKILL);
                            astroship->y = -1;
                            astroship->x = -1;
                            astroship->state = DEAD;                                            

                        }
                        
                        // if(life == 0){
                        //     pEnd(astroship, enemies1, enemies1Count, enemies2, enemies2Count, missiles, missilesCount, bombs, bombsCount);
                        //     pEngine(life);
                        // }
                    }


                    //Controllo per vedere se ha evaso l'area di gioco (da rivedere $$)
                    // if(enemies2[id].x < 0 && enemies2[id].y > 0 &&
                    //     enemies2[id].x != 0 && enemies2[id].y != -5 )
                    //     //loop = false;
                }


                if (message.state == DEAD){
                    enemies2[id].pid = -1;
                }
            
            }
            // default:
            //     break;
        }
        

        

        
        //Controllo delle confizioni di giocabilità
        //status = enemiesDead ? 1 : -1;           
        //Status = -1 => si gioca ancora
        //Status = 0 => 
        //Status = 1 => si esce dal gioco come vincitori
        //Status = 2 => si esce perdenti (navicelle a sx)
        //Status = 3 => si esce perdenti (life == 0)

        //loop = loopConditions();
        
        status = statusConditions(life, enemies1, enemies1Count, enemies2, enemies2Count);
        
        if(status > 0)
            loop = false;
        

        //Funzioni di disegno
        drawScene(astroship, enemies1, enemies1Count, enemies2, enemies2Count, missiles, missilesCount, bombs, bombsCount);
        mvprintw(0,0, "VITE: %d - STATUS: %d", life, status);
        refresh();
        //pPID(astroship, enemies1, enemies1Count, enemies2, enemies2Count, missiles, missilesCount, bombs, bombsCount, fs);
        //debugPositions(astroship, enemies1, enemies1Count, missiles, missilesCount, bombs, bombsCount);
        

    }
    //Usciti dal loop, abbiamo vinto o perso
    //funzione killa robe
    //pPID(astroship, enemies1, enemies1Count, enemies2, enemies2Count, missiles, missilesCount, bombs, bombsCount, fs);

    pEnd(astroship, enemies1, enemies1Count, enemies2, enemies2Count, missiles, missilesCount, bombs, bombsCount, fs);
    
    mvprintw(0,0, "STATUS: %d", status);
    refresh();
    sleep(3);

}

void pEnd( 
    object *astroship,
    object *enemies, int enemiesCount,
    object *enemies2, int enemies2Count,
    object *missiles, int missilesCount,
    object *bombs, int bombsCount, 
    int fs[])
{
    //The game is closing, so let's close all the remaining process!
    clearScreen();

    if (astroship->state != DEAD)
        kill(astroship->pid,SIGKILL);

    int i;
    for (i = 0; i < enemiesCount; i++)
        if (enemies[i].state != DEAD)
            kill(enemies[i].pid,SIGKILL);

    for (i = 0; i < enemies2Count; i++)
        if (enemies2[i].state != DEAD)
            kill(enemies2[i].pid,SIGKILL);

    for (i = 0; i < bombsCount; i++)
        if (bombs[i].state != DEAD)
            kill(bombs[i].pid,SIGKILL);

    for (i = 0; i < missilesCount; i++)
        if (missiles[i].state != DEAD)
            kill(missiles[i].pid,SIGKILL);

    free(astroship);         //In teoria astroship dovrebbe diventare un puntatore e quindi nel progetto avrò un bel po da cambiare
    free(enemies);
    free(enemies2);
    free(bombs);
    free(missiles);

    close(fs[0]);
    close(fs[1]);
 
}

void pPID( 
    object *astroship,
    object *enemies, int enemiesCount,
    object *enemies2, int enemies2Count,
    object *missiles, int missilesCount,
    object *bombs, int bombsCount, 
    int fs[]){

    clearScreen();

    //PID Astronave
    int i, j = 1;
    mvprintw(j,0, "PID AS: %d - STATE: %d", astroship->pid, astroship->state);

    //PID Nemici 1
    for (i = 0; i < enemiesCount; i++){
        j++;
        mvprintw(j,0, "PID N1: %d - STATE: %d", enemies[i].pid, enemies[i].state);
    }
    //PID Nemici 2
    for (i = 0; i < enemies2Count; i++){
        j++;
        mvprintw(j,0, "PID N2: %d - STATE: %d", enemies2[i].pid, enemies2[i].state);
    }
    //PID Bombe
    for (i = 0; i < bombsCount; i++){
        j++;
        mvprintw(j,0, "PID BM: %d - STATE: %d", bombs[i].pid, bombs[i].state);
    }
    //PID Missili
    for (i = 0; i < missilesCount; i++){
        j++;
        mvprintw(j,0, "PID MS: %d - STATE: %d", missiles[i].pid, missiles[i].state);
    }

    refresh();
    sleep(1);
}


int statusConditions(bool life,
    object *enemies1, int enemies1Count,
    object *enemies2, int enemies2Count){

    int status = -1;
    //1. nelle condizioni, se l'array dei nemici hanno tutti pid = -1, allora imposta >> status = 1 (e loop = false)
    bool enemiesDead = false;//true;           
    int i;
    for (i = 0; i < enemies1Count; i++)
        if (!(enemies1[i].state == DEAD))
            enemiesDead = true;

    for (i = 0; i < enemies2Count; i++)
        if (!(enemies2[i].state == DEAD))
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
    for (i = 0; i < enemies2Count; i++)
        if (enemies2[i].x < 1 && enemies2[i].y > 0 && enemies2[i].y < SCREEN_H)
            status = 2; //status = i + 1000000;

        //Status = -1 => si gioca ancora
        //Status = 0 => 
        //Status = 1 => si esce dal gioco come vincitori
        //Status = 2 => si esce perdenti (navicelle a sx)
        //Status = 3 => si esce perdenti (life == 0)
    return status;
}