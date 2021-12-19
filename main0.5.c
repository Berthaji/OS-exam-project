#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAXX 80
#define MAXY 24

struct posizione{
	int x;
	int y;
	char c;
};

void astro(int pipeout, int pipein);
void area(int pipein);
void missile1(int pipeout, struct posizione oggetto);
void missile2(int pipeout);


char sprite[5][5]={
    "|_|> ",
    "|_|> ",
    "|_|> ",
    "|_|> ",
    "|_|> "
};

char spriteLine[5]={"|_|> "};


//Si implementa il movimento principale dell'astronave sottoforma di sprite
int main(){

	int p[2];
	int status;

    //Inizializzazione del gioco
	initscr();
	noecho();
	keypad(stdscr,1);
	curs_set(0);


	pipe(p);
	
	int pidA;
	int dimsprite=sizeof(sprite);

	pidA=fork();

    switch(pidA){

        case -1: //processo non creato correttamente
            exit(-1);
            break;
        
        case 0: //processo figlio
            
            //prima stampa dello sprite per inizializzarlo
            mvprintw(MAXY/2,0,(char*)spriteLine);
            mvprintw((MAXY/2)-1,0,(char*)spriteLine);
            mvprintw((MAXY/2)-2,0,(char*)spriteLine);
            mvprintw((MAXY/2)-3,0,(char*)spriteLine);
            mvprintw((MAXY/2)-4,0,(char*)spriteLine);


			//close(p[0]);
			astro(p[1], p[0]);
            break;

        default: //processo padre
            close(p[1]);
			area(p[0]);
			//wait(&status);
            break;


    }

			
	kill(pidA,1);
	endwin();
	
}


void astro(int pipeout, int pipein)	//passo anche pipein per chiuderlo quando si usa il processo missile1 e usare la stessa pipe 
{
	struct posizione oggetto;
	oggetto.x= 0;
	oggetto.y= MAXY/2;
	int dimspriteLine = sizeof(spriteLine);
	oggetto.c='a';
	
	write(pipeout,&oggetto,sizeof(oggetto));
	
	
	while(true){
	
	int c= getch();
	
	switch(c){
		case KEY_UP:
			if(oggetto.y >= dimspriteLine) 
				oggetto.y-=1; 
			break;
		
		case KEY_DOWN: 
			if(oggetto.y < MAXY-1) 
				oggetto.y+=1; 
			break;
		
		case KEY_LEFT: 
			/*passo un carattere come identificativo dell'astronave per poterlo mettere nella pipe
			  e leggerlo nella funzione area() per la terminazione*/
			oggetto.c='e';	 
            break; 

	}
		
	
	write(pipeout,&oggetto,sizeof(oggetto));
	}
	
	
	
	
	
	int pidM1, pidM2, status;
	
	pidM1= fork();


     switch(pidM1){

        case -1: //processo non creato correttamente
            exit(-1);
            break;
        
        case 0: //processo figlio
            close(pipein);
			missile1(pipeout, oggetto);
            break;

        default: //processo padre
            wait(&status);
			pidM2= fork();
			close(pipein);
			missile2(pipeout);
            break;


    }

		
	kill(pidM1,1);	
	kill(pidM2,1);

}




 
void missile1(int pipeout, struct posizione oggetto){
	struct posizione missile;
	missile.y= oggetto.y+1;
	int dimsprite=sizeof(sprite);
	missile.x=dimsprite + dimsprite/4;
	missile.c='+';
	
	write(pipeout,&missile,sizeof(missile));
	
	while(true){
	
	int c= getch();
	
	switch(c){
		case 32:
            int delta=-1;
            if(missile.x + delta < 0){
        	clear();
    		}
    	    missile.x+=delta;
    	    missile.y+=delta;
            break;
           
			
	}
	
	write(pipeout,&missile,sizeof(missile));
	}
	
	endwin();
	exit(0);
}




void area(int pipein)
{	struct posizione oggetto, dato_letto;
	int dimsprite=sizeof(sprite);
	

	while(true){
		read(pipein,&dato_letto,sizeof(dato_letto));
		clear();
		oggetto=dato_letto;

	if(oggetto.c == 'a'){
        
		int i;
		
		for(i=0; i<5; i++){
			mvprintw(oggetto.y,0,(char*)spriteLine);
			oggetto.y -= 1;
		}


		curs_set(0);
		refresh();
		
	}
		
		
	if(oggetto.c == 'e'){	//terminazione
		endwin();
		exit(0);
	}
	
	if(oggetto.c == '+'){
	while(true){
		//clear();
		mvaddch(oggetto.y, oggetto.x, oggetto.c);
		curs_set(0);
		refresh(); }
	}
	
	}

}

