#include <stdio.h>
#include <curses.h>
#include <stdlib.h>
#include <unistd.h>

#define	UP	 	 65						/* Cursore sopra */
#define DW	 	 66						/* Cursore sotto */
#define	SX 		 68						/* Cursore sinistra */
#define DX	 	 67						/* Cursore destra */
#define	MAXX	 80						/* Dimensione dello schermo di output (colonne) */
#define MAXY	 20						/* Dimensione dello schermo di output (righe)   */
#define DELAY 500000  /* Ritardo nel movimento delle vespe (da adattare) */

/* Prototipi delle funzioni adoperate */
void *Vespa(void *arg);
void *Contadino(void *arg);
void Area(void);


/* Struttura adoperata per veicolare le coordinate */
struct position {
  int  x;	 /* Coordinata X */
  int  y;	 /* Coordinata Y */
};

/* Variabili globali per passaggio dati tra threads e segnalazione collisione */
struct position pVespa;	
struct position pContadino;	
int collision = 0;

/* Mutex per la gestione dei thread */
pthread_mutex_t	mtx;	

/*
----------------------------------------------------------------------
 Funzione principale del programma 
----------------------------------------------------------------------
*/
int main()
{
pthread_t tvespa;
pthread_t tcontadino;

  /* Inizializzo e configuro la finestra di output */
  initscr();					
  noecho();			
  curs_set(0);

		/* Inizializzo mutex */
  pthread_mutex_init(&mtx, NULL); 

		/* Inizializza generatore di numeri casuali */
		srand(time(NULL)); 

 /* Creo il thread vespa */
  if(pthread_create(&tvespa, NULL, Vespa, NULL))
  {
    endwin();
    exit;
  }

  /* Creo il thread contadino */
  if(pthread_create(&tcontadino, NULL, Contadino, NULL))
  {
    endwin();
    exit;
  }

	/* Avvio la funzione Area */
  Area();	

  /* Attendo la terminazione dei thread */
  pthread_join (tvespa, NULL);		
		pthread_join (tcontadino, NULL);		

	/* Elimino mutex */
  pthread_mutex_destroy (&mtx);		

  /* Ripristino la modalità di funzionamento usuale */
		endwin();	    	

		/* Segnalo la fine del gioco e termino il programma */
		printf("\n\n\nGAME OVER\n\n\n");	

  return 0;		
}



/*
----------------------------------------------------------------------
 Funzione 'Vespa'
---------------------------------------------------------------------- 
*/
void *Vespa(void *arg)
{
int deltax;		/* Spostamento orizzontale */
int deltay;		/* Spostamento orizzontale */

  pVespa.x = 1;  /* Coordinata X iniziale */
  pVespa.y = 1;  /* Coordinata Y iniziale */
  
  while(!collision)   
  {
			 /* Blocco mutex, cancello ultimo carattere e sblocco mutex */
				pthread_mutex_lock(&mtx);			
    mvaddch(pVespa.y,pVespa.x,' ');
	   pthread_mutex_unlock(&mtx);

    if(random() < RAND_MAX/2) deltax= 1; else deltax=-1;

     /* Se supero area X schermo inverto il movimento */
    if(pVespa.x + deltax < 1 || pVespa.x + deltax > MAXX/2) deltax = -deltax;
			 pVespa.x += deltax;

				if(random() < RAND_MAX/2) deltay= 1; else deltay=-1;

    /* Se supero area Y schermo inverto il movimento */
    if(pVespa.y + deltay < 1 || pVespa.y + deltay > MAXY) deltay = -deltay;
				pVespa.y += deltay;

	   /* Blocco mutex, disegno carattere, aggiorno schermo e sblocco mutex */
				pthread_mutex_lock(&mtx);			
    mvaddch(pVespa.y,pVespa.x,'^');	
				refresh();																							
	   pthread_mutex_unlock(&mtx); 

				/* Inserisco una pausa per rallentare il movimento */
    usleep(DELAY);
  }
}


/*
----------------------------------------------------------------------
 Funzione 'Contadino' - Movimento tramite i tasti cursore 
----------------------------------------------------------------------
*/
void *Contadino(void *arg)
{
char key;

  		/* Posizione iniziale contadino */
  		pContadino.x = MAXX/2;
  		pContadino.y = MAXY/2;

		 	/* Blocco mutex, disegno carattere, aggiorno schermo e sblocco mutex */		
				pthread_mutex_lock(&mtx);									
    mvaddch(pContadino.y,pContadino.x,'#');	
				refresh();			
	   pthread_mutex_unlock(&mtx); 						

  while(!collision)
  {
				key = getch();

		 	/* Blocco mutex, disegno carattere e sblocco mutex */																							
				pthread_mutex_lock(&mtx);									
    mvaddch(pContadino.y,pContadino.x,' ');	
	   pthread_mutex_unlock(&mtx); 						

				/* Gestisco movimento tasti cursore */    
				if(key==UP && pContadino.y > 0)						pContadino.y-=1;		
    if(key==DW && pContadino.y < MAXY-1)	pContadino.y+=1;		 
    if(key==SX && pContadino.x > 0)						pContadino.x-=1;		
    if(key==DX && pContadino.x < MAXX-1)	pContadino.x+=1;
   
		 	/* Blocco mutex, disegno carattere, aggiorno schermo e sblocco mutex */
    pthread_mutex_lock(&mtx);									
    mvaddch(pContadino.y,pContadino.x,'#');	
				refresh();																														
    pthread_mutex_unlock(&mtx); 	
  }
  return NULL;
}


/*
----------------------------------------------------------------------
 Funzione relativa al processo di visualizzazione trappole e controllo 
 di collisione
----------------------------------------------------------------------
*/
void Area()
{
struct position trappola1, told1;		
struct position trappola2, told2;	
struct position trappola3, told3;	
int i=0, vite=3;

		/* Inizializza valori delle strutture coordinate 
		trappola1.x=trappola2.x=trappola3.x=told1.x=told2.x=told3.x=0;
		trappola1.y=trappola2.y=trappola3.y=told1.y=told2.y=told3.y=0;*/

 do{

				/* Ogni 50 cicli genero 3 trappole in posizione casuale */
			 if(!(i++%10))
				{

		 	/* Blocco mutex, cancello le precedenti trappole visualizzate, aggiorno schermo e sblocco mutex */
    pthread_mutex_lock(&mtx);									
    mvaddch(told1.y,told1.x,' ');
				mvaddch(told2.y,told2.x,' ');
				mvaddch(told3.y,told3.x,' ');
				refresh();																														
    pthread_mutex_unlock(&mtx); 	

					 /* Genero casualmente le nuove coordinate delle 3 trappole verificando che
									le nuove coordinate non siano uguali alle precedenti o alle altre trappole */
 				 do{
									trappola1.x = rand()%MAXX;
				 			 trappola1.y = rand()%MAXY;
								 } while(trappola1.x == told1.x && trappola1.y == told1.y);

 				 do{
									trappola2.x = rand()%MAXX;
				 			 trappola2.y = rand()%MAXY;
								 } while( trappola2.x == told2.x && trappola2.y == told2.y ||
																		trappola2.x == trappola1.x && trappola2.y == trappola1.y	); 

 				 do{
									trappola3.x = rand()%MAXX;
				 			 trappola3.y = rand()%MAXY;
								 } while( trappola3.x == told3.x && trappola3.y == told3.y ||
																		trappola3.x == trappola1.x && trappola3.y == trappola1.y	|| 
																		trappola3.x == trappola2.x && trappola3.y == trappola2.y);  

 
		 	/* Blocco mutex, visualizzo le nuove trappole, aggiorno schermo e sblocco mutex */
        pthread_mutex_lock(&mtx);									
				mvaddch(trappola1.y,trappola1.x,'X');
				mvaddch(trappola2.y,trappola2.x,'X');
				mvaddch(trappola3.y,trappola3.x,'X');
				refresh();																														
        pthread_mutex_unlock(&mtx); 	

				 	/* Memorizzo le coordinate delle trappole visualizzate */
 				 told1.x = trappola1.x;
				  told1.y = trappola1.y;
 				 told2.x = trappola2.x;
				  told2.y = trappola2.y;
 				 told3.x = trappola3.x;
				  told3.y = trappola3.y;
				}

 	  /* Segnalo collisione e tipo (vespa/contadino oppure vespa/trappola) */
				if(pContadino.x == pVespa.x && pContadino.y == pVespa.y) vite--;
				if(	pVespa.x == trappola1.x && pVespa.y == trappola1.y || 
								pVespa.x == trappola2.x && pVespa.y == trappola2.y || 
								pVespa.x == trappola3.x && pVespa.y == trappola3.y )  if(vite < 6) vite++; 

				/* Esce quando terminano le vite del contadino */
				if(vite < 1) collision = 1; 

			 /* Visualizzo le vite del contadino */
			 pthread_mutex_lock(&mtx);	
			 mvprintw(0,1,"%3d",vite);
			 pthread_mutex_unlock(&mtx); 		

			 /* Inserisco una pausa per rallentare la creazione delle trappole */
       usleep(DELAY);

 /* Il ciclo si ripete finchè non si verifica una collisione contadino/vespa */
 } while(!collision);

}



