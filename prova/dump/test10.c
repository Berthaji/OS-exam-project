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

#include <signal.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

#include <ncurses.h>

#include <pthread.h>

#include "task.h"
#include "Scene.h"
#include "Utils.h"

object * astroship;
pthread_mutex_t tMutex;

void* tastroship (void* parameters);

int main(){
    pthread_mutex_init (&tMutex, NULL);

    //Allocazione spazio per la astronave
    astroship = (object*) malloc (sizeof(object) * 1);

    pthread_create(&astroship->tid, NULL, tastroship, (void *)astroship);

    sleep(10);
    pthread_mutex_lock(&tMutex);
    astroship->state = DEAD;
    pthread_mutex_unlock(&tMutex);

    sleep(10);

}

void* tastroship (void* parameters){
    object* obj;                                    //Noi non possiamo usare direttamente parameters perchè di tipo void*
    obj = (object*) parameters;                     //Ricorda che il cast è obbligatorio!
    
    bool loop = true;
    while(loop){
        pthread_mutex_lock(&tMutex);

        if(obj->state == DEAD)
            loop = false;

        pthread_mutex_unlock(&tMutex);
    }

    return NULL;
}