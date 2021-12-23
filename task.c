#include "task.h"
#include <stdio.h>
void player(){
    #if THREADED //logica multithread
        printf("ciao thread");
    #else  //logica multiprocesso
        printf("ciao processi")
    #endif
}