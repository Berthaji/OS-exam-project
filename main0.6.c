#include <stdio.h>
#include <curses.h>
#include <stdlib.h>
#include <unistd.h>

#define	UP			65		/* Cursore sopra */
#define DW			66		/* Cursore sotto */
#define	SX 			68		/* Cursore sinistra */
#define DX			67		/* Cursore destra */
#define	MAXX		80		/* Dimensione dello schermo di output (colonne) */
#define MAXY		20		/* Dimensione dello schermo di output (righe)   */
#define DELAY 	90000	/* Ritardo nel movimento delle vespe (da adattare) */

typedef enum{ GOES_UP, GOES_DOWN} type_m;

/* Prototipi funzioni adoperate */
void Astronave(int pipeout, int pipein);
void Missile(int pipeout, int x, int y, type_m tipo);
void Missile2(int pipeout, int x, int y);
void Area(int pipein);

char sprite[5][5]={
	"|_|> ",
	"|_|> ",
	"|_|> ",
	"|_|> ",
	"|_|> "};
	
char spriteLine[5]={"|_|> "};
char blankSprite[5]={"     "};


/* Struttura adoperata per veicolare le coordinate */
struct position {
  char c; 	/* Identificatore dell'entità che invia i dati */
  int  x;	 /* Coordinata X */
  int  y;	 /* Coordinata Y */
};


/*
----------------------------------------------------------------------
 Funzione principale del programma 
----------------------------------------------------------------------
*/
int main()
{
int p[2];				/* Descrittori pipe */
int pidA;   		/* Pid processo figlio Astronave*/
int pidN;		/* Pid al processo figlio Nemico*/
int i;	      
int starty=MAXY/2;

initscr();			/* Inizializza schermo di output */
noecho();				/* Imposta modalità della tastiera */
curs_set(0);		/* Nasconde il cursore */
pipe(p);				/* Creazione pipe */


		/* Inizializza generatore di numeri casuali */
		srand(time(NULL)); 

		
		pidA = fork();	
  
		
		if(pidA==0) {
				for(i=0; i<5; i++){
					mvprintw(starty,0,(char*)spriteLine);
					starty -= 1;}
					
				
    				Astronave(p[1], p[0]); 
  }		
  else {		
	
					close(p[1]); /* chiusura del descrittore di scrittura */
			    Area(p[0]);  /* invocazione funzione area */  
     		
		}

		
  	kill(pidA,1);	
  	//kill(pidN,1);	

		/* Ripristino la modalità di funzionamento usuale */
		endwin();	    	

		/* Termino il gioco ed esco dal programma */
		printf("\n\n\nGAME OVER\n\n\n");	

		return 0;		
}



/*
----------------------------------------------------------------------
 Funzione 'Vespa'
---------------------------------------------------------------------- 
*/
void Missile(int pipeout, int x, int y, type_m tipo)
{
struct position missile;
int deltax;		
int deltay;	
char c;	

	    deltax= 1;
	    if(tipo==GOES_DOWN) {deltay= 1; missile.y = y+5;}
	    if(tipo==GOES_UP) {deltay= -1; missile.y = y-1;}
	    missile.x = x+3;  /* Coordinata X iniziale */
  	    //missile.y = y+5;  /* Coordinata Y iniziale */
 
	 
	 missile.c ='o';	/* Carattere identificativo */
	 
	 write(pipeout,&missile,sizeof(missile));
	 
	 
    c = getch();
    bool flag=false;
    
    
   
   	flag=true;
   	
   	while(flag==true){	//questo ciclo serve a non far premere backspace per ogni movimento del missile
   	//al momento servono due backspace per far funzionare il missile: uno per farlo comparire e uno per lanciarlo
	   	
	    /* Se supero area X schermo esco dal processo*/
	    if(missile.x + deltax < 1 || missile.x + deltax > MAXX){
		exit(0);
	    }

	    /* Se supero area Y schermo esco dal processo*/
	    if(missile.y + deltay < 1 || missile.y + deltay > MAXY){
	      exit(0);
	    }

		
	    missile.y += deltay;
	    missile.x += deltax;
		
	    /* Comunico le coordinate correnti al processo padre */
	    write(pipeout,&missile,sizeof(missile));

			/* Inserisco una pausa per rallentare il movimento */
	    usleep(DELAY);
    }
   
  
}


void Missile2(int pipeout, int x, int y)
{
struct position missile;
int deltax;		
int deltay;	
char c;	

	    deltax= 1;
	    deltay= -1;
	    missile.x = x+3;  /* Coordinata X iniziale */
  	    missile.y = y-1;  /* Coordinata Y iniziale */
 
	 
	 missile.c ='o';	/* Carattere identificativo */
	 
	 write(pipeout,&missile,sizeof(missile));
	 
	 
    c = getch();
    bool flag=false;
    
    
   
   	flag=true;
   	
   	while(flag==true){	//questo ciclo serve a non far premere backspace per ogni movimento del missile
   	//al momento servono due backspace per far funzionare il missile: uno per farlo comparire e uno per lanciarlo
	   	
	    /* Se supero area X schermo esco dal processo*/
	    if(missile.x + deltax < 1 || missile.x + deltax > MAXX){
		exit(0);
	    }

	    /* Se supero area Y schermo esco dal processo*/
	    if(missile.y + deltay < 1 || missile.y + deltay > MAXY){
	      exit(0);
	    }

		
	    missile.y += deltay;
	    missile.x += deltax;
		
	    /* Comunico le coordinate correnti al processo padre */
	    write(pipeout,&missile,sizeof(missile));

			/* Inserisco una pausa per rallentare il movimento */
	    usleep(DELAY);
    }
   
  
}




/*
----------------------------------------------------------------------
 Funzione 'Astronave' - Movimento tramite i tasti cursore 
----------------------------------------------------------------------
*/
void Astronave(int pipeout, int pipein)
{
struct position apos;

  apos.x = 0;   /* Coordinata X iniziale */
  apos.y = MAXY/2;   /* Coordinata Y iniziale */
  apos.c='#';			   /* Carattere identificativo*/

  /* Comunico al processo padre le coordinate iniziali*/
  write(pipeout,&apos,sizeof(apos));

	/* Lettura dei tasti cursore */
  while(1)
  {
  		char c;
    c = getch();

    if (c==UP && apos.y > 0){
      apos.y-=1;				
    }

    if(c==DW  && apos.y < MAXY - 1){
      apos.y+=1;		
    }


		/* Comunico al processo padre le coordinate */
    write(pipeout,&apos,sizeof(apos));  
    
    
    if(c==32){
    int pidM1, pidM2, status;
    pidM1=fork();
    pidM2=fork();
					
				
     		if(pidM1==0) {
						

					/* ed eseguo quindi la relativa funzione di gestione */
					close(pipein); /* chiusura del descrittore di lettura */
					Missile(pipeout, apos.x, apos.y, GOES_DOWN); /* invocazione funzione col missile che va giu'*/  
     		}
     		else {
     			wait(&status);
     			pidM2=fork();
     			
     			if(pidM2==0){
     					close(pipein); /* chiusura del descrittore di lettura */
					Missile(pipeout, apos.x, apos.y, GOES_UP); /* invocazione funzione col missile che va su */ 
			}
			else{
					/* Sono ancora nel processo padre */
					wait(status);
					close(pipeout); /* chiusura del descrittore di scrittura */
			    		Area(pipein);  /* invocazione funzione area */  
			}
     		}
     		
     		kill(pidM1,1);
     		kill(pidM2,1);
	}
  }
		

}




/*
----------------------------------------------------------------------
 Funzione relativa al processo di visualizzazione e controllo
----------------------------------------------------------------------
*/
void Area(int pipein)
{
struct position missile, astronave, dato_letto;
struct position trappola1, told1;		
struct position trappola2, told2;	
struct position trappola3, told3;	
int i=0, collision=0;


  do {
		/* Leggo dalla pipe */
    read(pipein,&dato_letto,sizeof(dato_letto));

    /* Vespa */
		if(dato_letto.c=='o') {

			/* Cancello il precedente carattere visualizzato */
       mvaddch(missile.y,missile.x, ' ');
			/* Aggiorno le coordinate relative alla nuova posizione */
      missile=dato_letto;
 			}
     else {
			
			/* astronave */
			 /* Cancello il precedente carattere visualizzato */
        mvprintw(astronave.y, astronave.x, (char*)blankSprite);
        mvprintw(astronave.y+1, astronave.x, (char*)blankSprite);
        mvprintw(astronave.y+2, astronave.x, (char*)blankSprite);
        mvprintw(astronave.y+3, astronave.x, (char*)blankSprite);
        mvprintw(astronave.y+4, astronave.x, (char*)blankSprite);
       

			/* Aggiorno le coordinate relative alla nuova posizione */
      astronave=dato_letto;
   	}
		
		/* Visualizzo il carattere dell'entità sulle nuove coordinate */
   if(missile.y !=20 && dato_letto.c=='o') mvaddch(dato_letto.y,dato_letto.x,dato_letto.c);
   
   if(dato_letto.c=='#') {
   	mvprintw(dato_letto.y, dato_letto.x, (char*)spriteLine);
   	mvprintw(dato_letto.y+1, dato_letto.x, (char*)spriteLine);
   	mvprintw(dato_letto.y+2, dato_letto.x, (char*)spriteLine);
   	mvprintw(dato_letto.y+3, dato_letto.x, (char*)spriteLine);
   	mvprintw(dato_letto.y+4, dato_letto.x, (char*)spriteLine);}
   				
  

		

			 

			 /* Aggiorno lo schermo di output per visualizzare le modifiche */
       refresh();

 } while(true);
}






