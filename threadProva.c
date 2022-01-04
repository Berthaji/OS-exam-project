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
//#include "Scene.h"


//Define PERSONALI
#define SCREEN_W getmaxx(stdscr)
#define SCREEN_H getmaxy(stdscr)

object * astroship;
object * enemies1;
object * enemies2;
object * missile;
object * bomb;

pthread_mutex_t mutex_write;
pthread_mutex_t mutex_read; //Inutile

void* tastroship (void* parameters);
void myInitScreen();
void clearScreens();
void area();

int main(){

    myInitScreen();
    
    clearScreens();
    
    astroship = (object*) malloc (sizeof(object) * 1);

    srand(time(NULL));


    pthread_t thread_id[10];        //Inizializzo un po di pthread id
    refresh();

    pthread_mutex_init (&mutex_read, NULL); //Inutile
    pthread_mutex_init (&mutex_write, NULL);

    pthread_mutex_lock(&mutex_write);
    astroship->appearance = rand() % ASTRODCHOICE;
    astroship->y = SCREEN_H/2;
    astroship->x= 0;
    pthread_mutex_unlock(&mutex_write);

    
    // //In analogia con quanto fatto con la versione processi potrei modificare i valori prima di inviarli ad astroship, ma 
    // //stavolta potrei prima del loop impostrare le corrdinate correttamente!
    
    


    for(int i = 0; i < 1; i++){
        //pthread_create(&thread_id[i], NULL, &char_print, &thread2_args);
        pthread_create(&thread_id[i], NULL, tastroship, (void*)astroship);
        pthread_mutex_lock(&mutex_write);
        mvprintw(9,1, "APPEARANCE %d", astroship->appearance);
        pthread_mutex_unlock(&mutex_write);
        refresh();
    }


    
    while(true){
        

        clearScreens();

        pthread_mutex_lock(&mutex_write);  //sto cercando di leggere, blocco l'accesso
    
        //mvprintw(7,1, "QUA2");
        //refresh();
        //sleep(2);

        //mvprintw(0,0, "CORDINATE: %d", astroship->x);
        drawObject(*astroship);
        refresh();

        //Sbloccaggio di un mutex
        pthread_mutex_unlock(&mutex_write);

    }

    // //Abbozzo di tEnd

    for(int i = 0; i < 1; i++){
       pthread_join (thread_id[i], NULL);
    }
    
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
    pthread_mutex_lock(&mutex_write);
    obj->x = 6;
    obj->y = 1;
    
    
    //Sbloccaggio di un mutex
    pthread_mutex_unlock(&mutex_write);
    //Fine del mutex

    while (true){
        c = getch(); 
        mvprintw(6,6,"okchar"); //si ferma qui
        
        
        switch (c){
            case KEY_UP:
                if (obj->y > 0){
                    pthread_mutex_lock(&mutex_write);
                    obj->y -= 1;
                    pthread_mutex_unlock(&mutex_write);
                }
                break;

            case KEY_DOWN:
                if (obj->y < SCREEN_H - 5){
                    pthread_mutex_lock(&mutex_write);
                    obj->y++;
                    pthread_mutex_unlock(&mutex_write);
                }
                   
                break;
        
            case ' ': //Barra spaziatrice
                pthread_mutex_lock(&mutex_write);
                obj->hasShot = true;
                pthread_mutex_unlock(&mutex_write);
                break;

            default:
                break;
        }
        

        //o.hasShot = false;
        //c = 'q';
        

    /*int i = 100;
    while(i > 0){
        i--;
        sleep(1);
    }*/
}
    return NULL;
    //exit(0);
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




