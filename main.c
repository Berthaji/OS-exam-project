#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/wait.h>

#include <stdbool.h>
#include <time.h>
#include <signal.h>


//LIBRERIE PERSONALI
#include "header.h"
#include "Structs.h"
#include "Scene.h"
#include "task.h"
int main(){
    // initScreen();
    // drawScene();
    // getch();
    // endwin();
    player();

    return 0;
}