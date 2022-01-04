#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAXX 80
#define MAXY 24

void astro(int pipeout);
void area(int pipein);
void missile1(int pipeout);
void missile2(int pipeout);

struct posizione{
	int x;
	int y;
	char c;
};

char sprite[5][5]={
    "|_|> ",
    "|_|> ",
    "|_|> ",
    "|_|> ",
    "|_|> "
};


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
            mvprintw(0,MAXY/2,"|_|>");
            mvprintw(0,(MAXY/2)-1,"|_|>");
            mvprintw(0,(MAXY/2)-2,"|_|>");
            mvprintw(0,(MAXY/2)-3,"|_|>");


			close(p[0]);
			astro(p[1]);
            break;

        default: //processo padre
            close(p[1]);
			area(p[0]);
			//wait(&status);
            break;


    }



		/*
		if(pidA==0){
			mvprintw(0,MAXY/2,"|_|>");
			close(p[0]);
			astro(p[1]);
		} 
		else {
			close(p[1]);
			area(p[0]);
			//wait(&status);
		}*/
			
	kill(pidA,1);
	endwin();
	
}


void astro(int pipeout)
{
	struct posizione oggetto;
	oggetto.x= 0;
	oggetto.y= MAXY/2;
	int dimsprite=sizeof(sprite);
	
	write(pipeout,&oggetto,sizeof(oggetto));
	
	
	while(true){
	
	int c= getch();
	
	switch(c){
		case KEY_UP:
			if(oggetto.y > 0) 
				oggetto.y-=1; 
			break;
		
		case KEY_DOWN: 
			if(oggetto.y < MAXY-1) 
				oggetto.y+=1; 
			break;
		
		case KEY_LEFT: 
			endwin(); 
			exit(0);	//non funziona il tasto per uscire
            break; //ridondante


        //da fare caso in cui arrivi al bordo dello schermo
	}
		
	
	write(pipeout,&oggetto,sizeof(oggetto));
	}
	
	int pidM1, pidM2, status;
	int p[2];
	pipe(p);
	
	pidM1= fork();



        /*
		if(pidM1==0){
			close(p[0]);
			missile1(p[1]);
		} else{
			wait(&status);
			pidM1= fork();
			close(p[0]);
			missile2(p[1]);
		}*/


     switch(pidM1){

        case -1: //processo non creato correttamente
            exit(-1);
            break;
        
        case 0: //processo figlio
            close(p[0]);
			missile1(p[1]);
            break;

        default: //processo padre
            wait(&status);
			pidM1= fork();
			close(p[0]);
			missile2(p[1]);
            break;


    }

		
	kill(pidM1,1);	
	kill(pidM2,1);

}




 
void missile1(int pipeout){
	struct posizione oggetto;
	oggetto.y= MAXY/2;
    
	int dimsprite=sizeof(sprite);
	oggetto.x=dimsprite + dimsprite/4;
	oggetto.c='+';
	
	write(pipeout,&oggetto,sizeof(oggetto));
	
	while(true){
	
	int c= getch();
	
	switch(c){
		case 32:
            while(oggetto.y<MAXY){
                oggetto.y += 0.25;
                oggetto.x += 0.25;
                write(pipeout,&oggetto,sizeof(oggetto));
                clear();
                mvaddch(oggetto.y, oggetto.x, oggetto.c);
                curs_set(0);
                refresh(); 
            }
            break;

		default: 
            endwin(); 
            exit(0);
		
		
	}

}


void area(int pipein)
{	struct posizione oggetto, dato_letto;
	int dimsprite=sizeof(sprite);
	

	while(true){
		read(pipein,&dato_letto,sizeof(dato_letto));
		clear();
		oggetto=dato_letto;


        //prima riga
		mvaddch(oggetto.y, oggetto.x, '|');
		mvaddch(oggetto.y, (oggetto.x+dimsprite/4), '_');
		mvaddch(oggetto.y, (oggetto.x+dimsprite/2), '|');
		mvaddch(oggetto.y, (oggetto.x+dimsprite*3/4), '>');

        //seconda riga
        mvaddch(oggetto.y-1, oggetto.x, '|');
		mvaddch(oggetto.y-1, (oggetto.x+dimsprite/4), '_');
		mvaddch(oggetto.y-1, (oggetto.x+dimsprite/2), '|');
		mvaddch(oggetto.y-1, (oggetto.x+dimsprite*3/4), '>');

        //terza riga
        mvaddch(oggetto.y-2, oggetto.x, '|');
		mvaddch(oggetto.y-2, (oggetto.x+dimsprite/4), '_');
		mvaddch(oggetto.y-2, (oggetto.x+dimsprite/2), '|');
		mvaddch(oggetto.y-2, (oggetto.x+dimsprite*3/4), '>');

        //quarta riga
        mvaddch(oggetto.y-3, oggetto.x, '|');
		mvaddch(oggetto.y-3, (oggetto.x+dimsprite/4), '_');
		mvaddch(oggetto.y-3, (oggetto.x+dimsprite/2), '|');
		mvaddch(oggetto.y-3, (oggetto.x+dimsprite*3/4), '>');


		curs_set(0);
		refresh();
		
	}

}