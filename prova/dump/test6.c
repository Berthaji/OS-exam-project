#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <semaphore.h>

pthread_mutex_t muta = PTHREAD_MUTEX_INITIALIZER;
int value=0;
sem_t job_count;
int counter=0;
char* dati[10] ;
void* elimina(void*);
void inizializza_coda(void);
void aggiungi_job (char*);


int main (){
    int i;
    pthread_t thread[3];
    srand(time(NULL));
    inizializza_coda();
    aggiungi_job ("rosso"); 
    aggiungi_job ("verde");
    aggiungi_job ("giallo"); 
    aggiungi_job ("blu");

    for(i=0;i<3;i++){
        pthread_create(&thread[i], NULL, &elimina, NULL); /* 3 threads per eliminare */
    }

    aggiungi_job ("nero");
    aggiungi_job ("grigio");
    aggiungi_job ("amaranto");

    for(i=0;i<3;i++)
        pthread_join (thread[i], NULL);

    return 0; 
}

void* elimina(void* arg){
    int i,j;
    pthread_t tr;
    char nome[100];

    while(1) {
        sem_wait (&job_count);  /* Nessun job: attendo */

        pthread_mutex_lock(&muta);  /* Blocco mutex */
            strcpy(nome,dati[counter-1]);
            dati[counter-1] = NULL;
            tr = pthread_self();
            sleep( (int) (5.0*rand() / (RAND_MAX + 1.0)) );
            printf("Eliminato %s da thread %lu\n", nome, (unsigned long)tr);
            counter++;/* Aggiorno contatore */
        pthread_mutex_unlock(&muta); /* Sblocco mutex */
    }
    return NULL; 
}


void inizializza_coda(){
    counter = 0;
    sem_init(&job_count, 0, 0); /* Inizio da zero (nessun job) */
}

void aggiungi_job (char* nome){
    pthread_mutex_lock(&muta); /* Blocco mutex */
    if(counter < 10){
        dati[counter] = nome;
        counter++;
        sem_post(&job_count); 
    }
    pthread_mutex_unlock(&muta); /* Sblocco mutex */
}

