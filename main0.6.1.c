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


/* Prototipi funzioni adoperate */
void Astronave(int pipeout, int pipein);
void Missile(int pipeout, int x, int y);
void Nemico(int pipeout);
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
int i, status;	      

initscr();			/* Inizializza schermo di output */
noecho();				/* Imposta modalità della tastiera */
curs_set(0);		/* Nasconde il cursore */
pipe(p);				/* Creazione pipe */


		/* Inizializza generatore di numeri casuali */
		srand(time(NULL)); 

		
		pidA = fork();	
  
		
		if(pidA==0) {
				/*for(i=0; i<5; i++){
					mvprintw(starty,0,(char*)spriteLine);
					starty -= 1;}*/
					
				
    				Astronave(p[1], p[0]); 
  }		
  if(getpid()!=0) {		
		pidN=fork();
		
  			if(pidN==0){
  				mvaddch(MAXY/2,MAXX/2+1,'n');
  				printf("okkk");
  				close(p[0]);
  				Nemico(p[1]);
  				}
			else{
  				waitpid(pidN, &status);
				close(p[1]); /* chiusura del descrittore di scrittura */
			    	Area(p[0]);  /* invocazione funzione area */  
		    	    }
     		
		}
		
		
		/* Termino i processi Vespa e Contadino */
  	kill(pidA,1);	
  	kill(pidN,1); 
     		
	
  	//kill(pidN,1);	

		/* Ripristino la modalità di funzionamento usuale */
		endwin();	    	

		/* Termino il gioco ed esco dal programma */
		printf("\n\n\nGAME OVER\n\n\n");	

		return 0;		
}



/*
----------------------------------------------------------------------
 Funzione 'Missile'
---------------------------------------------------------------------- 
*/
void Missile(int pipeout, int x, int y)
{
struct position missile1, missile2;
int deltax;		
int deltay1, deltay2;	
char c;	

	    deltax= 1;
	    deltay1= 1; missile1.y = y+5;
	    deltay2= -1; missile2.y = y-1;
	    missile1.x = x+3;  /* Coordinata X iniziale */
  	    missile2.x = x+3;
 
	 
	 missile1.c ='o';	/* Carattere identificativo */
	 missile2.c = 'o';
	 
	 write(pipeout,&missile1,sizeof(missile1));
	 write(pipeout,&missile2,sizeof(missile2));
	 
    c = getch();
    bool flag=false;
    
   	
   	do{	//questo ciclo serve a non far premere backspace per ogni movimento del missile
   	//al momento servono due backspace per far funzionare il missile: uno per farlo comparire e uno per lanciarlo

		
	    missile1.y += deltay1;
	    missile2.y += deltay2;
	    missile1.x += deltax;
	    missile2.x += deltax;
	    
	     /* Se supero area X schermo esco dal processo*/
	    if(missile1.x + deltax < 1 && missile2.x + deltax == MAXX){
		exit(0);
	    }

	    /* Se supero area Y schermo esco dal processo*/
	    if(missile1.y + deltay1 < 1 || missile2.y + deltay2 == MAXY){
	      exit(0);
	    }
		
	    /* Comunico le coordinate correnti al processo padre */
	    write(pipeout,&missile1,sizeof(missile1));
	    write(pipeout,&missile2,sizeof(missile2));

			/* Inserisco una pausa per rallentare il movimento */
	    usleep(DELAY);
	    
    }while(missile1.y + deltay1 != 0 || missile2.y + deltay2 != MAXY);	//doppio controllo
    
    exit(0);
   
  
}



void Nemico(int pipeout){
	struct position nemico;
	
	nemico.x=MAXX/2+1;
	nemico.y=MAXY/2;
	nemico.x='n';
	int deltax, deltay;
	
	write(pipeout,&nemico,sizeof(nemico));
	
	deltay=1;
	deltax=-1;
	
	while(true){
	if(nemico.y + deltay < 1 || nemico.y + deltay > MAXX)
		deltay=-deltax;
		
	nemico.x-=deltax;
	nemico.y+=deltay;
	
	write(pipeout,&nemico,sizeof(nemico));
	
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
    int pidM1, status;
    pidM1=fork();
					
				
     		if(pidM1==0) {
						
					
					close(pipein); 
					Missile(pipeout, apos.x, apos.y); 
					
					
     		}
     		else {
     			waitpid(pidM1, &status);
			close(pipeout); 
	    		Area(pipein);  
			
     		}
     		
     		kill(pidM1,1);
     		
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
struct position missile, astronave, dato_letto, nemico;
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
 	if(dato_letto.c=='#') {
        mvprintw(astronave.y, astronave.x, (char*)blankSprite);
        mvprintw(astronave.y+1, astronave.x, (char*)blankSprite);
        mvprintw(astronave.y+2, astronave.x, (char*)blankSprite);
        mvprintw(astronave.y+3, astronave.x, (char*)blankSprite);
        mvprintw(astronave.y+4, astronave.x, (char*)blankSprite);
        
        astronave=dato_letto;
        }
        
        //nemico
        if(dato_letto.c=='n'){
        mvaddch(nemico.y, nemico.x, ' ');
        nemico=dato_letto;}
       

			
      
   	}
		
		/* Visualizzo il carattere dell'entità sulle nuove coordinate */
   if(missile.y !=20 && missile.y!=0 && dato_letto.c=='o') mvaddch(dato_letto.y,dato_letto.x,dato_letto.c);
   
   if(dato_letto.c=='#') {
   	mvprintw(dato_letto.y, dato_letto.x, (char*)spriteLine);
   	mvprintw(dato_letto.y+1, dato_letto.x, (char*)spriteLine);
   	mvprintw(dato_letto.y+2, dato_letto.x, (char*)spriteLine);
   	mvprintw(dato_letto.y+3, dato_letto.x, (char*)spriteLine);
   	mvprintw(dato_letto.y+4, dato_letto.x, (char*)spriteLine);}
   	
   
   if(dato_letto.c=='n')
   	{ 
   		mvaddch(dato_letto.y, dato_letto.x, 'n');
   	}
   				
  
	
		

			 

	curs_set(0);		 /* Aggiorno lo schermo di output per visualizzare le modifiche */
       refresh();

 } while(true);
 
}






