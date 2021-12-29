#include "task.h"

pthread_mutex_t	mtx;    //mutex per la gestione dei thread

int enemies1Count = M;
int enemies2Count = 0;
int bombsCount = 0;
int missilesCount = 0;

object astroship;
object *enemies1 = (object *)malloc(sizeof(object) * M);
object *enemies2 = (object *)malloc(sizeof(object) * 1);
object *bombs    = (object *)malloc(sizeof(object) * 1);
object *missiles = (object *)malloc(sizeof(object) * missilesCount);


void *pAstroship(void* obj){


    char c = 'q';
    astroship.hasShot = false;

    while (true){
        c = getch(); //carattere inserito e quindi controllato
        switch (c){
            case KEY_UP & A_CHARTEXT:
                if (astroship.y > 0)
                    astroship.y--;
                break;

            case KEY_DOWN & A_CHARTEXT:
                if (astroship.y < SCREEN_H - 5)
                    astroship.y++;
                break;

            //$$ URGENTE - NON COMMITTARE
            case KEY_LEFT  & A_CHARTEXT:
            if (astroship.x > 0)
                astroship.x--;

            break;

            case KEY_RIGHT  & A_CHARTEXT:
            if (astroship.x < SCREEN_W - 5)
                astroship.x++;
            break;
        
            case ' ': //Barra spaziatrice
                //alarm(1);
                astroship.hasShot = true;
                break;

            default:
                break;
        }
        //Scrivo solo se ho sparato o mi sono mosso, cosi risparmio sull'uso della pipe
        //if(o.hasShot || c != 'q')
           // write(pipeOut, &o, sizeof(o)); /* Comunico al processo padre le coordinate del contadino */
        
        


        astroship.hasShot = false;  //??????
        c = 'q';


    }

}



void *pEnemy1(void* obj){
    bool loop = true;
    int k;
    k = (int)obj;   //casto a int l'indice passato da tEngine per poterlo usare

    while (loop){
        //o.dir = direction;

        enemies1[k].x--;                   //sposto il nemico verso destra
        enemies1[k].y += enemies1[k].dir ? -1 : 1; //sposto il missile in basso o in alto a seconda della sua direzione (i nemici vanno su e giu)
        //o.y += 0;               //Si spostano nello stesso modo

        //o.dir == 0 --> 1
        //o.dir == 1 --> -1

        /**
         * se il nemico è fuori dallo schermo dobbiamo terminare il processo
         * perciò terminiamo il loop, comunichiamo al processo principale che il nemico 
         * è "morto" e chiudiamo il processo nemico
         */

        //se il nemico non è ai bordi si può muovere tranquillamente, altrimenti lo spostiamo più dentro
        if (enemies1[k].y >= SCREEN_H -2){    //Bordo inferiore superato
            enemies1[k].y--;
            enemies1[k].dir = 1;
        }
        if (enemies1[k].y <= 0 ){    //Bordo superiore superato
            enemies1[k].y++;
            enemies1[k].dir = 0;
        }

        if (enemies1[k].x < -5){
            loop = false;
            enemies1[k].state = KILLED;
        }

        //o.dir = direction;

        //write(pipeOut, &o, sizeof(o)); //comunichiamo il risultato dell'eleborazione
        usleep(100000);                 // un delay per evitare che il nemico vada troppo veloce
    }
    //harakiri
    //kill(getpid(), SIGKILL);
}



void *pEnemy2(void* obj){
    bool loop = true;
    int k;
    k = (int)obj;   //casto a int l'indice passato da tEngine per poterlo usare

    while (loop){
        //o.dir = direction;

        enemies2[k].x--;                   //sposto il nemico verso destra
        enemies2[k].y += 0;               //Si spostano nello stesso modo
        //o.y += o.dir ? -1 : 1; //sposto il nemico in basso o in alto a seconda della sua direzione

        //o.dir == 0 --> 1
        //o.dir == 1 --> -1

        /**
         * se il nemico è fuori dallo schermo dobbiamo terminare il processo
         * perciò terminiamo il loop, comunichiamo al processo principale che il nemico 
         * è "morto" e chiudiamo il processo nemico
         */

        //se il nemico non è ai bordi si può muovere tranquillamente, altrimenti lo spostiamo più dentro
        if (enemies2[k].y >= SCREEN_H -2){    //Bordo inferiore superato
            enemies2[k].y--;
            enemies2[k].dir = 1;
        }
        if (enemies2[k].y <= 0 ){    //Bordo superiore superato
            enemies2[k].y++;
            enemies2[k].dir = 0;
        }

        if (enemies2[k].x < -5){
            loop = false;
            enemies2[k].state = DEAD;
        }

        //o.dir = direction;

       // write(pipeOut, &o, sizeof(o)); //comunichiamo il risultato dell'eleborazione
        usleep(100000);                 // un delay per evitare che il nemico vada troppo veloce
    }
    //harakiri
    //kill(getpid(), SIGKILL);


}



void *pBomb(void* obj){
    bool loop = true;
    int k;
    k = (int)obj;   //casto a int l'indice passato da tEngine per poterlo usare

    while (loop){
        bombs[k].x--;                     //sposto la bomba verso sinistra
        //o.y += o.dir ? -1 : 1; //sposto il missile in basso o in alto a seconda della sua direzione
        bombs[k].y += 0;
        /**
         * se il missile è fuori dallo schermo dobbiamo terminare il processo
         * perciò terminiamo il loop, comunichiamo al processo principale che il proiettile 
         * è "morto" e chiudiamo il processo missile
         */
        if (bombs[k].y < 0|| bombs[k].y > SCREEN_H || bombs[k].x < 0){
            loop = false;
            bombs[k].state = DEAD;
        }

        //write(pipeOut, &o, sizeof(o)); //comunichiamo il risultato dell'eleborazione
        usleep(50000);                 // un delay per evitare che la bomba vada troppo veloce
    }
    //harakiri
    kill(getpid(), SIGKILL);
}



void *pMissile(void* obj){

    bool loop = true;
    int k;
    k = (int)obj;   //casto a int l'indice passato da tEngine per poterlo usare
    while (loop){

        missiles[k].x++;                     //sposto il missile verso sinistra
        //o.y += 0; //o.dir ? -1 : 1; //sposto il missile in basso o in alto a seconda della sua direzione
        missiles[k].y += (missiles[k].dir ? -1 : 1)*0.625;      //$$diagonale vera



        /**
         * se il missile è fuori dallo schermo dobbiamo terminare il processo
         * perciò terminiamo il loop, comunichiamo al processo principale che il proiettile 
         * è "morto" e chiudiamo il processo missile
         */
        if (missiles[k].y < -2 || missiles[k].y > SCREEN_H + 2){
            loop = false;
            missiles[k].state = DEAD;
        }

        //write(pipeOut, &o, sizeof(o)); //comunichiamo il risultato dell'eleborazione
        usleep(30000);                 // un delay per evitare che il missile vada troppo veloce
    }
    //harakiri
    //kill(getpid(), SIGKILL);
}



void tEngine(int life){

    //variabili di supporto
    bool loop = true; //controlla l'esecuzione del while principale
    int status = -1; //Tipologia di uscita (abbiamo vinto, perso per nemici a bordo schermo...)make all
   
    /**
     Variabili corrispondenti ai thread da utilizzare
     */
    pthread_t t_astroship;
    pthread_t t_enemy1;
    pthread_t t_enemy2;
    pthread_t t_bomb;
    pthread_t t_missile;

    pthread_mutex_init(&mtx, NULL);

    
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


    /* pthread_create ritorna 1 se il thread non è stato creato correttamente
     * mentre, se ritorna 0, avvia subito il thread nella subroutine (terzo
     * parametro) */
    if (pthread_create(&t_astroship, NULL, pAstroship, NULL)){
        endwin();
        exit(-1);
    }

    sleep(1);
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
        if (i % 4 == 3 && i != 0){
            y = 0;
            x++;
        }
        y++;


            enemies1[i].dir = i % 2;      //Tutti in direzioni diverse
            //enemies1[i].dir = 1;            //Tutti nella stessa direzione

            if(pthread_create(&t_enemy1, NULL, pEnemy1, (void*)i)){     //passo i camuffato da void al thread
                endwin();
                exit(-1);
            }

    }



    while(loop){

        //ASTRONAVE
            if (astroship.hasShot){                   //Lancio dei due missili
                    missilesCount += 2;
                    // if(missiles == NULL)
                    //     missiles = (object *)malloc(sizeof(object) * missilesCount);
                    // else
                    missiles = (object *)realloc(missiles, sizeof(object) * missilesCount);

                    for (int i = missilesCount - 2; i < missilesCount; i++){    //Aggiunta dei due missili all'elenco
                        missiles[i].x = astroship.x + 5; 
                        missiles[i].y = astroship.y + 2;
                        missiles[i].type = MISSILE;
                        missiles[i].id = i;
                        missiles[i].dir = i % 2;

                        if(pthread_create(&t_missile, NULL, pMissile, (void*)i)){
                            endwin();
                            exit(-1);
                        }

                    }
                }

            

        //ENEMY1
            for(int k=0; k<enemies1Count; k++){   //scorre ogni nemico di primo livello della run
                if(enemies1[k].state == KILLED)
                status = 2;


                //Blocco per la gestione del lancio delle bombe
                if( (rand()%1000) < SHOT_PROB){        
                    bombsCount += 1;

                    bombs = (object *)realloc(bombs, sizeof(object) * bombsCount);

                    for (int i = bombsCount - 1; i < bombsCount; i++){
                        bombs[i].x = enemies1[k].x - 1; 
                        bombs[i].y = enemies1[k].y + 1;
                        bombs[i].type = BOMB;
                        bombs[i].id = i;
                            bombs[i].dir = i % 2;
                            if(pthread_create(&t_bomb, NULL, pBomb, (void*)i)){
                                endwin();
                                exit(-1);       
                            }
                    }

                }

                int match = AstroCollided(astroship,enemies1[id]); 
                if(match > -1){  //Match nemico astronave
                    life--;
                    
                    //Ammazzo il nemico colpito
                    kill(enemies1[i].pid, SIGKILL);
                    enemies1[i].y = 0;
                    enemies1[i].x = 0;
                    enemies1[i].state = DEAD;

                    //Ammazzo l'astronave stessa
                    if(astroship.state != DEAD){  //Ammazzo astronave solo se viene colpito una volta
                        //Ammazzo l'astronave stessa
                        kill(astroship.pid, SIGKILL);
                        astroship.y = -1;
                        astroship.x = -1;
                        astroship.state = DEAD;                                            

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

            }



        //BOMB
            for(int i=0; i<bombsCount; i++){

                 //Controllo dell'incrocio con l'oggetto astronave
                int match = AstroCollided(astroship,bombs[id]); 
                if(match > -1){  //Match nemico astronave
                    life--;
                    //Ammazzo il nemico colpito
                    if(bombs[i].state != DEAD){  
                        kill(bombs[i].pid, SIGKILL);
                        bombs[i].y = -1;
                        bombs[i].x = -1;
                        bombs[i].state = DEAD;
                    }

                    //Ammazzo l'astronave stessa
                    if(astroship.state != DEAD){  //Ammazzo astronave solo se viene colpito una volta
                        //Ammazzo l'astronave stessa
                        kill(astroship.pid, SIGKILL);
                        astroship.y = -1;
                        astroship.x = -1;
                        astroship.state = DEAD;                                            

                    }  

                    // if(life == 0){
                    //     pEnd(astroship, enemies1, enemies1Count, enemies2, enemies2Count, missiles, missilesCount, bombs, bombsCount);
                    //     pEngine(life);
                    // }

                }

                if (bombs[i].state == DEAD){
                    bombs[i].pid = -1;
                }
                
            }



        //MISSILE
            for(int k=0; i<missilesCount; i++){

                //Controllo delle collisioni coi nemici di primo livello
                int enemyid = MissileCollided(enemies1,missiles[k],enemies1Count); 
                if(enemyid > -1){  //Match nemico missile
                    if(enemies1[enemyid].state != DEAD){  //Ammazzo il nemico solo se viene colpito una volta (ossia dal primo missile)
                        //Ammazzo il missile stesso
                        kill(missiles[k].pid, SIGKILL);
                        missiles[k].y = -1;
                        missiles[k].x = -1;
                        missiles[k].state = DEAD;

                        enemies2Count += 1;
                        enemies2 = (object *)realloc(enemies2, sizeof(object) * enemies2Count);

                        for (int i = enemies2Count - 1; i < enemies2Count; i++){
                            enemies2[i].x = enemies1[enemyid].x-1; //message.x; 
                            enemies2[i].y = enemies1[enemyid].y; //message.y; 
                            enemies2[i].type = ENEMY2;
                            enemies2[i].appearance = 3;
                            enemies2[i].id = i;
                                enemies2[i].dir = 0;
                                if(pthread_create(&t_enemy2, NULL, pEnemy2, (void*)i)){
                                endwin();
                                exit(-1);       
                                }
                            
                        }
                        
                                               
                        //Ammazzo il nemico colpito
                        //kill(enemies1[enemyid].pid, SIGKILL);
                        enemies1[enemyid].y = -5;
                        enemies1[enemyid].x = 0;
                        enemies1[enemyid].state = DEAD;
                    }

                }

                //Controllo delle collisioni coi nemici di secondo livello
                int enemy2id = MissileCollided(enemies2,missiles[id],enemies2Count);
                if(enemy2id > -1){  //Match nemico2 missile
                    if(enemies2[enemy2id].state != DEAD){  //Ammazzo il nemico solo se viene colpito una volta (ossia dal primo missile)
                        //Ammazzo il missile stesso
                        kill(missiles[i].pid, SIGKILL);
                        missiles[i].y = -1;
                        missiles[i].x = -1;
                        missiles[i].state = DEAD;

                        //kill(enemies2[enemy2id].pid, SIGKILL);
                        enemies2[enemy2id].y = -5;
                        enemies2[enemy2id].x = 0;
                        enemies2[enemy2id].state = DEAD;

                    }
                }

                if (missiles[i].state == DEAD){
                    missiles[i].pid = -1;
                }
            }



        
        //ENEMY2
            for(int k=0; i<missilesCount; i++){
                
                if((rand()%1000) < SHOT_PROB){        
                    bombsCount += 1;

                    bombs = (object *)realloc(bombs, sizeof(object) * bombsCount);

                    for (int i = bombsCount - 1; i < bombsCount; i++){
                        bombs[i].x = enemies1[k].x - 1; 
                        bombs[i].y = enemies1[k].y + 1;
                        bombs[i].type = BOMB;
                        bombs[i].id = i;
                            bombs[i].dir = i % 2;
                            if(pthread_create(&t_bomb, NULL, pBomb, (void*)i)){
                                endwin();
                                exit(-1);       
                            }
                    }

                }

                 //Controllo dell'incrocio con l'oggetto astronave
                int match = AstroCollided(astroship,enemies2[id]); 
                if(match > -1){  //Match nemico astronave
                    life--;

                    //Kill del nemico stesso
                    kill(enemies2[i].pid, SIGKILL);
                    enemies2[i].y = -5;
                    enemies2[i].x = 0;
                    enemies2[i].state = DEAD;

                    
                    if(astroship.state != DEAD){  //Ammazzo astronave solo se viene colpito una volta
                        //Ammazzo l'astronave stessa
                        kill(astroship.pid, SIGKILL);
                        astroship.y = -1;
                        astroship.x = -1;
                        astroship.state = DEAD;                                            

                    }
                    
                    // if(life == 0){
                    //     pEnd(astroship, enemies1, enemies1Count, enemies2, enemies2Count, missiles, missilesCount, bombs, bombsCount);
                    //     pEngine(life);
                    // }
                }

                //Controllo per vedere se ha evaso l'area di gioco
                if(enemies2[i].x < 0 && enemies2[i].y > 0 &&
                    enemies2[i].x != 0 && enemies2[i].y != -5 )
                    loop = false;


                if (enemies2[i].state == DEAD){
                    enemies2[i].pid = -1;
                }

            }


            //Aspetta ogni thread
            pthread_join(&t_astroship, NULL);
            pthread_join(&t_enemy1, NULL);
            pthread_join(&t_enemy2, NULL);
            pthread_join(&t_bomb, NULL);
            pthread_join(&t_missile, NULL);

            pthread_mutex_lock(&mtx);
            drawScene(astroship, enemies1, enemies1Count, enemies2, enemies2Count, missiles, missilesCount, bombs, bombsCount);
            pthread_mutex_unlock(&mtx);
            
            //Condizioni di game over
        //1. Astronave colpita da un nemico (se ho solo una vita, gia gestito)
        //2. Tutte le navicelle nemiche sono state distrutte (win)
        //Faccio un ciclo su tutti i nemici
        bool enemiesDead = true;
        int i;
        for (i = 0; i < enemies1Count; i++)
            if (enemies1[i].state != DEAD)
                enemiesDead = false;
        for (i = 0; i < enemies2Count; i++)
            if (enemies2[i].state != DEAD)
                enemiesDead = false; 

        
        //status = enemiesDead ? 1 : -1;           
        
        //Status = -1 => si gioca ancora
        //Status = 0 => 
        //Status = 1 => si esce dal gioco come vincitori
        //Status = 2 => si esce perdenti (navicelle a sx)
        //Status = 3 => 


        if(status > 0 || enemiesDead)
            loop = false;
        
        mvprintw(0,0, "VITE: %d - STATUS %d", life, status);
        refresh();


    }





}