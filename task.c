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
void pAstroship(int pipeIn, object o)
{

    char c;
    while (true)
    {
        c = getch(); //carattere inserito e quindi controllato
        switch (c)
        {
        case KEY_UP & A_CHARTEXT:
            if (o.y > 0)
                o.y--;

            break;

        case KEY_DOWN & A_CHARTEXT:
            if (o.y < SCREEN_H - 5)
                o.y++;
            break;

       
        case ' ': //Barra spaziatrice
            o.hasShot = true;
            break;

        default:
            break;
        }

        write(pipeIn, &o, sizeof(o)); /* Comunico al processo padre le coordinate del contadino */
        o.hasShot = false;
    }

}

void pEnemy(int pipeOut, object o)
{
    while (true)
    {
        write(pipeOut, &o, sizeof(object));
        usleep(30000);
    }
}

void pBomb(int filedescriptor)
{
}

/**
 * @brief Questa funzione si occupa di gestire un missile lanciato dal player
 * 
 * @param pipeOut pipe in scrittura 
 * @param direction indica se il proiettile "sale" o "scende" (in 2D)
 * @param o i dati iniziali del proiettile
 */
void pMissile(int pipeOut, int direction, object o)
{
    bool loop = true;

    while (loop)
    {
        o.x++;                     //sposto il missile verso sinistra
        o.y += direction ? -1 : 1; //sposto il missile in basso o in alto a seconda della sua direzione

        /**
         * se il missile è fuori dallo schermo dobbiamo terminare il processo
         * perciò terminiamo il loop, comunichiamo al processo principale che il proiettile 
         * è "morto" e chiudiamo il processo missile
         */
        if (o.y < -2 || o.y > SCREEN_H + 2)
        {
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
void pEngine()
{
    object astroship;
    object *enemies = (object *)malloc(sizeof(object) * 0);
    object *bombs = (object *)malloc(sizeof(object) * 0);
    object *missiles = (object *)malloc(sizeof(object) * 0);

    //contatori
    int enemiesCount = M;
    int bombsCount = 0;
    int missilesCount = 0;

    bool loop = true; //controlla l'esecuzione del while principale

    //variabili di supporto
    //int i = 0;
    object message; //qui viene salvato il messaggio letto dalla pipe dei processi
   
    /**
     * L'array seguente conterrà il file descriptor della pipe
     * Tenteremo un inizializzazione tramite la funzione pipe(), 
     * in caso di errore, il programma termina 
     */
    int fs[2];
    int pipeIn, pipeOut;
    if (pipe(fs) == -1)
        exit(-1);

    pipeIn = fs[0];
    pipeOut = fs[1];

    //inizializzazione della variabile astroship che conterrà i dati del player
    
    //Inizializziamo inanzitutto l'astronave, che passeremo di continuo
   
    astroship.x = 1;
    astroship.y = (SCREEN_H - 3) / 2;
    astroship.type = ASTROSHIP;
    astroship.hasShot = false;
    astroship.appearance = rand() % ASTRODCHOICE;
    //starship.color = rand()%7;



    /**
     * Creiamo il processo del giocatore utilizzando la fork
     * se la fork restituisce 0 significa che siamo nel processo figlio
     * per cui dirigiamo l'esecuzione verso la funzione relativa
     * Altrimenti continuiamo con il codice principale
     */
    pid_t player_pid = fork();
    if (player_pid == 0)
        pAstroship(fs[1], astroship);
    else
    {
       
        enemies = (object *)realloc(enemies, sizeof(object) * enemiesCount);
        for (int i = 0, x = 0, y = 1; i < M, x < 4; i++)
        {
            enemies[i].x = SCREEN_W / 2 + 1 + (x * 5);
            enemies[i].y = 7 * y;
            enemies[i].type = ENEMY1;
            enemies[i].state = INITIALIZED;
            enemies[i].appearance = 0;
            enemies[i].id = i;
            enemies[i].pid = fork();
            if (i % 4 == 3)
            {
                y = 1;
                x++;
            }
            if (enemies[i].pid == 0)
            {
                pEnemy(pipeOut, enemies[i]);
            }
        }
    }

    //loop del gioco, you dont say
    while (loop)
    {
        read(pipeIn, &message, sizeof(object));

        //in questo switch va gestita la logica del gioco
        switch (message.type)
        {
        case ASTROSHIP:
        {
            astroship.type = message.type;
            astroship.x = message.x;
            astroship.y = message.y;
            astroship.appearance = message.appearance;

            if (message.hasShot)
            {
                missilesCount += 2;
                missiles = (object *)realloc(missiles, sizeof(object) * missilesCount);
                for (int i = missilesCount - 2; i < missilesCount; i++)
                {
                    missiles[i].x = message.x + 5; //Initial coordinates must be derivated form the astroship process
                    missiles[i].y = message.y + 2;
                    missiles[i].type = MISSILE;
                    missiles[i].state = INITIALIZED;
                    missiles[i].appearance = -1;
                    missiles[i].id = i;
                    missiles[i].pid = fork();
                    if (missiles[i].pid == 0)
                    {
                        pMissile(pipeOut, i % 2, missiles[i]);
                    }
                }
            }
            break;
        }

        case ENEMY1:
        case ENEMY2:
        {
            int id = message.id;
            enemies[id].x = message.x;
            enemies[id].y = message.y;
            break;
        }
        case BOMB:
        case MISSILE:
        {
            int id = message.id;
            missiles[id].x = message.x;
            missiles[id].y = message.y;
            missiles[id].state = message.state;

            if (message.state == DEAD)
            {
                missiles[id].pid = -1;
            }
            break;
        }

            // default:
            //     break;
        }
        drawScene(astroship, enemies, enemiesCount, missiles, missilesCount, bombs, bombsCount);
        // debugPositions(astroship, enemies, enemiesCount, missiles, missilesCount, bombs, bombsCount);
    }
}

void pEnd()
{
    //The game is closing, so let's close all the remaining process!

    int i;
    for (i = 0; i < M + 1; i++)
    {
        //mvprintw(i,1,"PID: %d\n", obj[i].pid);
        //kill(obj[i].pid,SIGKILL);
        //wait(0);
    }
}

void debugPositions(
    object astroship,
    object *enemies, int enemiesCount,
    object *missiles, int missilesCount,
    object *bombs, int bombsCount)
{
    clearScreen();

    //Astronave
    mvprintw(0, 0, "OBJ: %d \t\t\tx: %d \t\t\ty: %d \t\t\tpid: %d \n",
             astroship.type,
             astroship.x,
             astroship.y,
             astroship.pid);

    int j = 1;
    //Nemici
    for (int i = 0; i < enemiesCount; i++)
    {
        mvprintw(j, 0, "OBJ: %d \t\t\tx: %d \t\t\ty: %d \t\t\tpid: %d \n",
                 enemies[i].type,
                 enemies[i].x,
                 enemies[i].y,
                 enemies[i].pid);

        j++;
    }

    //Bombe
    for (int i = 0; i < bombsCount; i++)
    {
        mvprintw(j, 0, "OBJ: %d \t\t\tx: %d \t\t\ty: %d \t\t\tpid: %d \n",
                 bombs[i].type,
                 bombs[i].x,
                 bombs[i].y,
                 bombs[i].pid);

        j++;
    }

    //Missili
    for (int i = 0; i < missilesCount; i++)
    {
        mvprintw(j, 0, "OBJ: %d \t\t\tx: %d \t\t\ty: %d \t\t\tpid: %d \n",
                 missiles[i].type,
                 missiles[i].x,
                 missiles[i].y,
                 missiles[i].pid);

        j++;
    }

    refresh();
    //sleep(3);
}