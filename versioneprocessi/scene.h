/* -  LIBRERIE - */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <time.h>
#include <signal.h>
#include <ncurses.h>

/* -  MACRO - */

/* Macro funzionali */
#define SCREEN_W getmaxx(stdscr)
#define SCREEN_H getmaxy(stdscr)

#define DIM_COLORS 8                /* Numero di colori presenti nel gioco */

#define ASTRODCHOICE    4           //Possibili scelte di sprite per la astronave
#define ENEMYDCHOICE    4           //Possibili scelte di sprite per i nemici
#define ASTRODIM        5           //Dimensioni (in entrambi gli assi) dello sprite astronave
#define ENEMYDIM        3           //Dimensioni (in entrambi gli assi) dello sprite nemico (lv. 1 e lv. 2)

#define ENEMYSLEEP1 100000
#define ENEMYSLEEP2 300000
#define BOMBSLEEP 80000
#define MISSILESLEEP 5000
#define SHOT_ANGLE_CORRECTION 0.05
//0.625


#define ENEMY_GENERATION_WIDTH 0.75     //Percentuala dal bordo di schermo a cui cominciare la generazione
#define ENEMY_GENERATION_DISTANCE_COLUMN 5   //Distanza tra linee di nemici per colonna
#define ENEMY_GENERATION_DISTANCE_LINES 7    //Distanza tra colonne di nemici generati
#define ENEMY_GENERATION_LINES 4            //Numero di linee di nemici


/* Macro interne di supporto */
#define DELAY_ARRSTART 1000000

#define DIM_ARRSTART_Y 37
#define DIM_ARRSTART_X 285
#define DIM_ARRSTART_Y_FIRSTLINE 20
#define DIM_ARRSTART_Y_SECONDLINE 35
#define DIM_ARRSTART_X_COLUMNS 142

#define DIM_ARRMENU_Y 16
#define DIM_ARRMENU_X 80
#define DIM_ARRMENU 4

#define DIM_ARRMENU_ 14


/* -  VARIABILI GLOBALI - */

/**
 * @brief Tipologia di oggetto nel gioco
 * 
 */
typedef enum TYPE {ASTROSHIP, ENEMY1, ENEMY2, BOMB, MISSILE} TYPE;

/**
 * @brief Condizione in cui si trova l'oggetto nel gioco
 * 
 */
typedef enum {NOT_INITIALIZED, INITIALIZED, ALIVE, DEAD, KILLED} STATE; 

/**
 * @brief Struttura di ogni oggetto nel gioco
 * 
 */
typedef struct Object{
    int x;
    float y;
    
    int id;                     //usato per le bombe e i missili
    bool hasShot;               //Usato dall'astronave e dai nemici
    int color;                  //Colore oggetto (dalla tavolozza offerta dalla librerai ncurses)
    TYPE type;                  //tipo entità
    STATE state;                //stato oggetto
    int appearance;             //Aspetto degli oggetti (usato da astronave e nemici)

    int dir;                    //Direzione dell'oggetto (usato solo dai nemici)
    pid_t pid;                  //Process id
}Object;



char arrStart [DIM_ARRSTART_Y][DIM_ARRSTART_X];
char arrMenu[DIM_ARRMENU][DIM_ARRMENU_Y][DIM_ARRMENU_X];
char astroship_sprites[ASTRODCHOICE][5][6];
char enemy_sprites[ENEMYDCHOICE][3][4];
char win[13][103+1];
char lose[14][121+1];



/* -  FUNZIONI - */

/* Funzioni di visualizzazione a schermo */

int kbhit(void);            /* Funzione che simula il comportamento della kbhit in ambiente Windows. Permette di leggere un carattere dal flusso di input */

void initScreen();          /* Inizializzazione schermo e attributi per usare la libreria ncurses */
void clearScreen();         /* Pulizia dello schermo */

void drawSplashScreen();    /* Disegno della schermata iniziale */
int drawMenu();            /* Disegno del menu di gioco */

void drawScene(
    Object *astroship,
    Object *enemies, int enemiesCount,
    Object *enemies2, int enemies2Count,
    Object *missiles, int missilesCount,
    Object *bombs, int bombsCount);


void drawObject(Object entity);

bool range(int min, int max, int value);

int astroCollided(Object astroship, Object dangerousObj);

int missileCollided(Object* enemy, Object missiles, int enemiesCount);

void drawFinalScene(int status);

void gameWin();

void gameLose();

