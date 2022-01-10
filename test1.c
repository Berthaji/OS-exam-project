/******************************************************************************

                            Online C Compiler.
                Code, Compile, Run and Debug C program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <stdbool.h>
#include <time.h>
#include <signal.h>


#include <ncurses.h>
#include <time.h>

//#include <termios.h>

//LIBRERIE PERSONALI
#include "task.h"
#include "Scene.h"

#define SCREEN_W getmaxx(stdscr)
#define SCREEN_H getmaxy(stdscr)

void clearScreens();



char arr [37][285] = {
{"                           /\\\\\\\\\\\\\\\\\\\\\\                                                                                                                  "},                
{"                          /\\\\\\/////////\\\\\\                                                                                                               "},                
{"                           \\//\\\\\\      \\///    /\\\\\\\\\\\\\\\\\\                                                                                                "},                
{"                            \\////\\\\\\          /\\\\\\/////\\\\\\  /\\\\\\\\\\\\\\\\\\        /\\\\\\\\\\\\\\\\     /\\\\\\\\\\\\\\\\                                                    "},                
{"                                \\////\\\\\\      \\/\\\\\\\\\\\\\\\\\\\\  \\////////\\\\\\     /\\\\\\//////    /\\\\\\/////\\\\\\                                                  "},                
{"                                    \\////\\\\\\   \\/\\\\\\//////     /\\\\\\\\\\\\\\\\\\\\   /\\\\\\          /\\\\\\\\\\\\\\\\\\\\\\                                                  "},                
{"                              /\\\\\\      \\//\\\\\\  \\/\\\\\\          /\\\\\\/////\\\\\\  \\//\\\\\\        \\//\\\\///////                                                  "},                
{"                              \\///\\\\\\\\\\\\\\\\\\\\\\/   \\/\\\\\\         \\//\\\\\\\\\\\\\\\\/\\\\  \\///\\\\\\\\\\\\\\\\  \\//\\\\\\\\\\\\\\\\\\\\                                               "},                
{"                                 \\///////////     \\///           \\////////\\//     \\////////    \\//////////                                               "},                
{"                                                                                                                                               "},                
{"             /\\\\\\                        /\\\\\\\\\\                                        /\\\\\\                                                          "},                
{"             \\/\\\\\\                      /\\\\\\///                                        \\/\\\\\\                                                         "},                
{"              \\/\\\\\\                     /\\\\\\                                            \\/\\\\\\                                                        "},                
{"               \\/\\\\\\       /\\\\\\\\\\\\\\\\    /\\\\\\\\\\\\\\\\\\     /\\\\\\\\\\\\\\\\    /\\\\/\\\\\\\\\\\\           \\/\\\\\\       /\\\\\\\\\\\\\\\\    /\\\\/\\\\\\\\\\\\\\                            "},                
{"           /\\\\\\\\\\\\\\\\\\     /\\\\\\/////\\\\\\  \\////\\\\\\//    /\\\\\\/////\\\\\\  \\/\\\\\\////\\\\\\     /\\\\\\\\\\\\\\\\\\     /\\\\\\/////\\\\\\  \\/\\\\\\/////\\\\\\                          "},                
{"           /\\\\\\////\\\\\\    /\\\\\\\\\\\\\\\\\\\\\\      \\/\\\\\\     /\\\\\\\\\\\\\\\\\\\\\\   \\/\\\\\\  \\//\\\\\\   /\\\\\\////\\\\\\    /\\\\\\\\\\\\\\\\\\\\\\   \\/\\\\\\   \\///                          "},                
{"           \\/\\\\\\  \\/\\\\\\   \\//\\\\///////       \\/\\\\\\    \\//\\\\///////    \\/\\\\\\   \\/\\\\\\  \\/\\\\\\  \\/\\\\\\   \\//\\\\///////    \\/\\\\\\                                 "},                
{"            \\//\\\\\\\\\\\\\\/\\\\   \\//\\\\\\\\\\\\\\\\\\\\     \\/\\\\\\     \\//\\\\\\\\\\\\\\\\\\\\  \\/\\\\\\   \\/\\\\\\  \\//\\\\\\\\\\\\\\/\\\\   \\//\\\\\\\\\\\\\\\\\\\\  \\/\\\\\\                               "},                
{"              \\///////\\//     \\//////////      \\///       \\//////////   \\///    \\///    \\///////\\//     \\//////////   \\///                               "},   
{"                                                                                                                                               "},                
{" ______   __  __     ______        ______     ______     __    __     ______     ______     ______   ______     ______     ______     _____    "}, 
{"/\\__  _\\ /\\ \\_\\ \\   /\\  ___\\      /\\  == \\   /\\  ___\\   /\\ \"-./  \\   /\\  __ \\   /\\  ___\\   /\\__  _\\ /\\  ___\\   /\\  == \\   /\\  ___\\   /\\  __-.  "},                
{"\\/_/\\ \\/ \\ \\  __ \\  \\ \\  __\\      \\ \\  __<   \\ \\  __\\   \\ \\ \\-./\\ \\  \\ \\  __ \\  \\ \\___  \\  \\/_/\\ \\/ \\ \\  __\\   \\ \\  __<   \\ \\  __\\   \\ \\ \\/\\ \\ "}, 
{"   \\ \\_\\  \\ \\_\\ \\_\\  \\ \\_____\\     \\ \\_\\ \\_\\  \\ \\_____\\  \\ \\_\\ \\ \\_\\  \\ \\_\\ \\_\\  \\/\\_____\\    \\ \\_\\  \\ \\_____\\  \\ \\_\\ \\_\\  \\ \\_____\\  \\ \\____- "},                
{"    \\/_/   \\/_/\\/_/   \\/_____/      \\/_/ /_/   \\/_____/   \\/_/  \\/_/   \\/_/\\/_/   \\/_____/     \\/_/   \\/_____/   \\/_/ /_/   \\/_____/   \\/____/ "}, 
{"                                                                                                                                               "},                
{"                                     __   __   ______     ______     ______     __     ______     __   __                                      "},     
{"                                    /\\ \\ / /  /\\  ___\\   /\\  == \\   /\\  ___\\   /\\ \\   /\\  __ \\   /\\ \"-.\\ \\                                     "}, 
{"                                    \\ \\ \\'/   \\ \\  __\\   \\ \\  __<   \\ \\___  \\  \\ \\ \\  \\ \\ \\/\\ \\  \\ \\ \\-.  \\                                    "},                
{"                                     \\ \\__|    \\ \\_____\\  \\ \\_\\ \\_\\  \\/\\_____\\  \\ \\_\\  \\ \\_____\\  \\ \\_\\\\\"\\_\\                                   "}, 
{"                                      \\/_/      \\/_____/   \\/_/ /_/   \\/_____/   \\/_/   \\/_____/   \\/_/ \\/_/                                   "},                
{"                                                                                                                                               "}, 
{"                                                                                                                                               "},                
{"                                                        Preparazione del gioco...                                                       "},            
{"                                                                                                                                               "}                                                            
};


char arr2[4][16][80] = {
 {
  {"Beh, vedi, come dire, cioè ecco...                                            "},
  {"La razza umana ha deciso di popolare troppi pianeti, motivo per cui tu,       "},
  {"squattrinato come sempre, oltre a lavorare 12 ore al giorno come pizzaiolo,   "},
  {"nel tempo libero devi anche prendere la tua navicella baracca,                "},
  {"andare nello spazio e minacciare l'esistenza di poveri alieni.                "},
  {"Del resto sono dei poracci che non hanno fatto niente di male nella loro vita,"},
  {"a parte averti incontrato.                                                    "},
  {"Divertiti!                                                                    "},
  {"                                                                              "},
  {"Seleziona la modalità di gioco (spazio per confermare)                        "},
  {"> FACILE                                                                      "},
  {"  MEDIA                                                                       "},
  {"  DIFFICILE                                                                   "},
  {"  ABBASTANZA CASUALE                                                          "},
  {"                                                                              "},
  {"                                                                              "}
 },
 {
  {"Beh, vedi, come dire, cioè ecco...                                            "},
  {"La razza umana ha deciso di popolare troppi pianeti, motivo per cui tu,       "},
  {"squattrinato come sempre, oltre a lavorare 12 ore al giorno come pizzaiolo,   "},
  {"nel tempo libero devi anche prendere la tua navicella baracca,                "},
  {"andare nello spazio e minacciare l'esistenza di poveri alieni.                "},
  {"Del resto sono dei poracci che non hanno fatto niente di male nella loro vita,"},
  {"a parte averti incontrato.                                                    "},
  {"Divertiti!                                                                    "},
  {"                                                                              "},
  {"Seleziona la modalità di gioco (spazio per confermare)                        "},
  {"  FACILE                                                                      "},
  {"> MEDIA                                                                       "},
  {"  DIFFICILE                                                                   "},
  {"  ABBASTANZA CASUALE                                                          "},
  {"                                                                              "},
  {"                                                                              "}
 },
 {
  {"Beh, vedi, come dire, cioè ecco...                                            "},
  {"La razza umana ha deciso di popolare troppi pianeti, motivo per cui tu,       "},
  {"squattrinato come sempre, oltre a lavorare 12 ore al giorno come pizzaiolo,   "},
  {"nel tempo libero devi anche prendere la tua navicella baracca,                "},
  {"andare nello spazio e minacciare l'esistenza di poveri alieni.                "},
  {"Del resto sono dei poracci che non hanno fatto niente di male nella loro vita,"},
  {"a parte averti incontrato.                                                    "},
  {"Divertiti!                                                                    "},
  {"                                                                              "},
  {"Seleziona la modalità di gioco (spazio per confermare)                        "},
  {"  FACILE                                                                      "},
  {"  MEDIA                                                                       "},
  {"> DIFFICILE                                                                   "},
  {"  ABBASTANZA CASUALE                                                          "},
  {"                                                                              "},
  {"                                                                              "}
 },
 {
  {"Beh, vedi, come dire, cioè ecco...                                            "},
  {"La razza umana ha deciso di popolare troppi pianeti, motivo per cui tu,       "},
  {"squattrinato come sempre, oltre a lavorare 12 ore al giorno come pizzaiolo,   "},
  {"nel tempo libero devi anche prendere la tua navicella baracca,                "},
  {"andare nello spazio e minacciare l'esistenza di poveri alieni.                "},
  {"Del resto sono dei poracci che non hanno fatto niente di male nella loro vita,"},
  {"a parte averti incontrato.                                                    "},
  {"Divertiti!                                                                    "},
  {"                                                                              "},
  {"Seleziona la modalità di gioco (spazio per confermare)                        "},
  {"  FACILE                                                                      "},
  {"  MEDIA                                                                       "},
  {"  DIFFICILE                                                                   "},
  {"> ABBASTANZA CASUALE                                                          "},
  {"                                                                              "},
  {"                                                                              "}
 }
};
//NB si potrebbe aggiungere una breve descrizione di ogni modalità cosi i 4 "sprite" hanno più senso $$




int main(){
  beep();
  printf("\a");
  
  initscr();
  cbreak();

  noecho();
  keypad(stdscr, 1);
  curs_set(0);
  start_color();

  attron(A_BOLD   );
  
  // for(int i = 0; i < 40; i++){
  //   //mvprintw(i,0, arr[i]);
  //   addch(,arr[i][1]);
  // }
  bool bo = false;
  bool timer = true;

  int msec = 0, trigger = 500; /* 5s */ //50
  clock_t before = clock();
  int i = 0;

  while(msec < trigger){//true){//(msec < trigger){
    clock_t difference = clock() - before;
    msec = difference * 1000 / CLOCKS_PER_SEC;
    //iterations++;//
    //attron(A_REVERSE);
    

    for (int y = 0; y < 40; y++)
          for (int x = 0; x < 142; x++){
              mvaddch(y, x, ' ');
          }

    for (int y = 0; y < 20; y++)
          for (int x = 0; x < 142; x++){

              if(arr[y][x] == '\\' && bo){
                  mvaddch(y, x, arr[y][x]);
              }
              if(arr[y][x] == '/' && !bo){
                  mvaddch(y, x, arr[y][x]);
              }
          }

    //The remastered version

    for (int y = 20; y < 35; y++)
          for (int x = 0; x < 142; x++){
                  mvaddch(y, x, arr[y][x]);

              // if(arr[y][x] == '\\' && bo){
              // }
              // if(arr[y][x] == '/' && !bo){
              //     mvaddch(y, x, arr[y][x]);
              // }
          }

    refresh();
    //usleep(50000);
    bo = !bo;
    //attroff(A_REVERSE);
    

    // if(getch() != 'q'){
    //   clearScreen();
    //   usleep(1000);
    // }
    i++;
    
  }

  //Schermata iniziale conclusa
  //Seconda parte
  clearScreens();
  int choice = 0;
  bool chosen = false;


 

  for (int y = 0; y < 14; y++)
              mvprintw(y,0, arr2[0][y]);

  while(!chosen){
    refresh();
    char ch = getch();
    switch (ch){
      case KEY_UP & A_CHARTEXT:
        //NB Con gestione del bordo superiore
        
        if(!(choice == 0)){ //Non siamo al bordo superiore
          choice--;
          for (int y = 0; y < 14; y++)
              mvprintw(y,0, arr2[choice][y]);
        }
       
            
        break;

      case KEY_DOWN & A_CHARTEXT:
        
        if(!(choice == 3)){ //Non siamo al bordo inferiore
          choice++;
          for (int y = 0; y < 14; y++)
              mvprintw(y,0, arr2[choice][y]);
        }
        break;

      case ' ':
        mvprintw(0,0, "%d", choice);
        refresh();
        sleep(3);
        
        break;

      /*
      NB
      choice = 0 -> facile
      choice = 1 -> media
      choice = 2 -> difficile
      choice = 3 -> molto casuale
      */
    }
    
    clearScreens();
    for (int y = 0; y < 14; y++)
              mvprintw(y,0, arr2[choice][y]);
  }

          

  refresh();
  
  //Menù vero e proprio con selezione

}

void clearScreens()
{
    for (int y = 0; y < SCREEN_H; y++)
        for (int x = 0; x < SCREEN_W; x++)
            mvaddch(y, x, ' ');
}