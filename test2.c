#include <stdbool.h>
#include <time.h>
#include <signal.h>


#include <ncurses.h>
#include <time.h>

//#include <termios.h>

//LIBRERIE PERSONALI
#include "task.h"
#include "Scene.h"

#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#define SCREEN_W getmaxx(stdscr)
#define SCREEN_H getmaxy(stdscr)

int kbhit(void);
//int kbhit(fd_set readfds);


int mains(){
    
    initscr();
    cbreak();

    int fs[2];
    pipe(fs);

    int * j;
    int k = 100;
    j = &k;


    // write(fs[1], j, sizeof(int) );
    
    // //int i = kbhit(fs[0]);
    // int c,*d;
    // while(1){
    //     read(fs[0],&d,sizeof(int));
    //     printf("CARATTERE: %c", d);
    // }

    int c;
    while (!((c = getch()) != '\n' || c != EOF)) { 
        printf("\nciao");
    }

    return 0;
}

/*
* Return 1 (true) if there are any unread characters, 0 (false) otherwise
*/
int kbhit(void){
    int ch,r;

    /* turn off getch() blocking and echo */
    nodelay(stdscr,TRUE);
    noecho();

    /* check for input */
    ch = getch();
    if( ch == ERR) /* no input */
        r = FALSE;

    else{ /* input */
        r = TRUE;
        ungetch(ch);
    }

    /* restore block and echo */
    echo();
    nodelay(stdscr,FALSE);
    return(r);
}

 int main(void){
    initscr();
    //Chediamo di allargare propriamente lo schermo prima di iniziare
    //Una riga al centro dice le dimensioni da raggiungere e quelle raggiunte



    
    addstr("Press any key to end this program:");
    while(!kbhit()){
        printf("\n ciao");
    }

    

    endwin();
    return 0;
 }