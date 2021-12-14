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

struct posizione{
	int x;
	int y;
	char c;
};


//Si implementa il movimento principale dell'astronave '+' del giocatore
int main()
{
	//inizializzazione delle varie parti
	int p[2];
	int status;
	initscr();
	noecho();
	keypad(stdscr,1);
	curs_set(0);
	pipe(p);
	
	int pidA;
	pidA=fork();

		
		if(pidA==0){
			mvprintw(0,MAXY/2,"+");
			close(p[0]);
			astro(p[1]);
		} else {
			close(p[1]);
			//area(p[0]);
			wait(&status);
			}
			
	kill(pidA,1);
	endwin();
	return 0;
}


void astro(int pipeout)
{
	struct posizione oggetto;
	oggetto.x= 0;
	oggetto.y= MAXY/2;
	oggetto.c='+';
	
	
	write(pipeout,&oggetto,sizeof(oggetto));
	
	
	while(true){
	int c= getch();
	
	switch(c){
		case KEY_UP:
		if(oggetto.y > 0) oggetto.y-=1; break;
		
		case KEY_DOWN: 
		if(oggetto.y < MAXY-1) oggetto.y+=1; break;
		
		case 113: endwin(); exit(0);
		}
		
	clear();
	mvaddch(oggetto.y, oggetto.x, '+');
	refresh();
	write(pipeout,&oggetto,sizeof(oggetto));
	}
}
	
 
