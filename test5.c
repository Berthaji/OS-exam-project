#include <malloc.h> 
#include <pthread.h> 
#include <semaphore.h>

struct job {
    struct job* next; /* Link field for linked list. */
    /* Other fields describing work to be done... */ 
};

struct job* job_queue; /* A linked list of pending jobs */

/* A mutex protecting job_queue */
pthread_mutex_t job_queue_mutex = PTHREAD_MUTEX_INITIALIZER;

/* A semaphore counting the number of jobs in the queue */
sem_t job_queue_count;

void initialize_job_queue (){/* Perform one­time initialization of the job queue */
    job_queue = NULL;  /* The queue is initially empty. */
    /*  Initialize  the  semaphore  which  counts  jobs  in  the  queue.  Its  initial  value 
    should be zero */
    sem_init (&job_queue_count, 0, 0); 
}

/* Process queued jobs until the queue is empty */
void* thread_function (void* arg) {
    while (1) {
        struct job* next_job;
        /*  Wait  on  the  job  queue  semaphore.  If  its  value  is  positive,  indicating  that 
        the queue is not empty, decrement the count by 1. If the queue is empty, block 
        until a new job is enqueued */
        sem_wait (&job_queue_count); 

        pthread_mutex_lock(&job_queue_mutex); /* Lock the mutex on the job queue */

        /* Because of the semaphore, we know the queue is not empty. Get the next 
        available job */
        next_job = job_queue; 
        /* Remove this job from the list */
        job_queue = job_queue->next;

        /* Unlock the mutex on the job queue because we’re done with the queue for now */
        pthread_mutex_unlock (&job_queue_mutex); 
        process_job (next_job);  /* Carry out the work */
        free (next_job); /* Clean up */
    }

    return NULL;
}
void enqueue_job (/* Pass job­specific data here... */) { //attenzione
    /* Add a new job to the front of the job queue */

    struct job* new_job;
    new_job = (struct job*) malloc (sizeof (struct job)); 
    /* Allocate a new job object */


    /* Set the other fields of the job struct here... */
    /* Lock the mutex on the job queue before accessing it */
    pthread_mutex_lock (&job_queue_mutex); 
    
    new_job->next = job_queue; /* Place the new job at the head of the queue */
    job_queue = new_job;

    sem_post (&job_queue_count); /* Post to the semaphore to indicate that another 
    job  is  available.  If  threads  are  blocked,  waiting  on  the  semaphore,  one  will 
    become unblocked so it can process the job */
    /* Unlock the job queue mutex. */
    
    pthread_mutex_unlock (&job_queue_mutex); 
}


