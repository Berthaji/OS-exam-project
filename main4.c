//Librerie necessarie
#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/wait.h>

#include <stdbool.h>
#include <time.h>
#include <signal.h>

#include "header.h"



//Funzione per avvio di tutto quello che serve, ncurses, oggetti, contropipe...
//Funzione per il singolo oggetto, una per astronave, una per missile, bomba, nemico1 e forse nemico2, 
//Funzione per disegnare (forse)
//Funzione di areagioco per controllare gli spostamenti e le collisioni

void startgame();       //Per avviare l'interfaccia di gioco e strutture necessarie
void astroship(int filedescriptor);
void enemy(int filedescriptor[2]);
void areagioco(int filedescriptor);
void drawingobject(object obj);

int main(){
        //Preparazione dell'area di gioco
        startgame();

    
        
        sleep(1);
        endwin();       //Chiusura ordinata del gioco

        printf("\nEND\n");
        return 0;
}


void startgame(){
    

    // //-- SETTAGGIO IMPOSTAZIONI NCURSES --//
    // initscr();
    // noecho();
    // keypad(stdscr,1);       //Attenzione lavoriamo sullo schermo principale
    // curs_set(0);
    //-- SETTAGGIO IMPOSTAZIONI NCURSES --//
    initscr();
    noecho();
    keypad(stdscr,1);       //Attenzione lavoriamo sullo schermo principale
    curs_set(0);
    start_color();          //Per la gestione dei colori

    srand(time(NULL));      //Inizializza generatore di numeri casuali
    

    //-- CREAZIONE DEL DATABASE DEGLI OGGETTI --//
    object* objs = NULL;       //Creazione dell'array di struct per contenere tutti i dati di tutti gli oggetti
    objs = (object*) malloc(sizeof(object)*(M+1)); //Per ora allochiamo solo astronave e gli M nemici

    
    if(objs == NULL)         //Controllo di routine per la corretta allocazione in memoria
        exit(-1);

    //-- CREAZIONE PROCESSI --//

    //Pipe
    int mainpid = getpid(); // PID del processo padre 

    int fs[2];              // File descriptor della pipe
    int valpipe = pipe(fs);	            // Creazione pipe

    if(valpipe == -1)               // Controllo di routine per la corretta generazione
        exit(-1);                   //TODO fare creazione e controllo assieme

    
    //Fork
    int astropid = fork();
    
    if(astropid == 0){         // Gestiamo i vari casi una volta fatta la prima fork!  (!obj[0].pid)
        //Posso allora far partire la relativa funzione (quindi il processo) di  gestione   - //$$ attenzione al non aver messo == /Siamo dentro il processo astronave

        int maxx, maxy;         //Capire a chi servano queste due variabili $$
        getmaxyx(stdscr, maxy, maxx);  /* Ottiene dimensioni schermo */

        close(fs[0]);
        astroship(fs[1]);      //Non sono sicuro di passargli il pid corretto con obj[0].pid
    }
    else{                   //Siamo ancora nel processo padre

        //Possiamo già passare il pid del processo astronave alla sua struttura oggetto
        if(mainpid==getpid()){                          //Controllo per far eseguire la porzione di codice solo al vero padre
                //obj[0].pid = astropid;                      //il padre ha (ritornato da prima) il pid del figlio che gli possiamo già passare          
        }

        int i;

        for (i = 1; i < M+1; i++){                      //Trasforamre in un so while perchè magari dei 4 nemici solo 3 sono figli: il nipote viene killato ma non rifatto $$
            int enemypid = fork();
            if(enemypid == 0 && getppid()==mainpid){//Voglio che si "realizzino" solo i processi direttamente figli del padre
                enemy(fs); 
                
            }
            else                                        //Non sono figli diretti
                if(enemypid == 0 && getppid()!=mainpid) //non è il padre && il proprio padre non coincide con quello stabilito, sono degli inutili nipoti
                    kill(getpid(),SIGTERM);                       //Non è un processo direttamente figlio -> Killare

            
            
            //refresh();
            if(mainpid==getpid()){                          //Controllo per far eseguire la porzione di codice solo al vero padre
                //obj[i].pid = enemypid;                      //il padre ha (ritornato da prima) il pid del figlio che gli possiamo già passare           
            }


        }
        
        //Continuo funzione padre principale
        if(mainpid==getpid()){                          //Controllo per far eseguire la porzione di codice solo al vero padre (controllo di sicurezza)

            
            //SETTAGGIO NCURSE

            close(fs[1]);
            areagioco(fs[0]);   

            //Se arriviamo qua come flusso, il gioco si sta chiudendo -> chiudere tutti i processi
            int i;
            for (i = 0; i < M+1; i++){
                //mvprintw(i,1,"PID: %d\n", obj[i].pid);
                //kill(obj[i].pid,SIGKILL);
                //wait(0);
            }
        }
    }  

    

    //-- CHIUSURA DEL GIOCO --//
    //Devo chiudere tutti i processi: grazie alla struttura precedentemente definita, so chi deve essere killato
    //PER ORA LO LASCIO COMMENTATO, FUNZIONA DENTRO IL PRECEDENTE IF MAINPID == GETPID


    //Dobbiamo far partire l'ultima funzione, quella di areagioco

}

//Processo astronave
void astroship(int filedescriptor){
    //TIP: per alleggerire la pipe, potremo usarla SOLO per mandare le coordinate e gedtire tutto da area gioco

    object starship;
    //Inizializziamo inanzitutto l'astronave, che passeremo di continuo
    starship.k_type = NOTINIZIALIZED;                           //Ridondante ma necessario (? evitiamo controlli di deffault sbagliati?)
    starship.x = 0;
    starship.y = 0;
    starship.oldx = 0;
    starship.oldy = 0;
    starship.o_type = ASTRONAVE;
    //starship->pid = getpid();
    starship.dtype = rand()%ASTRODCHOICE;
    starship.color = rand()%7;

    //Ultimo comando prima di far partire il resto
    starship.k_type = INIZIALIZED;

    if(starship.k_type == INIZIALIZED){
           /* chiusura del descrittore di lettura -> best pratice?*/
        
        write(filedescriptor, &starship, sizeof(starship));           /* Comunico al processo padre le coordinate iniziali del contadino */
        int c;

        while(true){
            //Riprendo le dimensioni dello schermo
            //int maxx =0, maxy=0;
            //getmaxyx(stdscr, maxy,maxx);
            starship.oldy = starship.y;
            
            c = getch();                               //carattere inserito e quindi controllato
            switch(c) {
                case KEY_UP: //Keyup
                    //Controllo: evitare che vada oltre il bordo alto dello schermo
                    //if(starship.y-1 < 0)
                        //starship.o_type= ASTRONAVE;
                    //starship.y-=1;
                    
                    starship.y -= 1;
                    
                    break;
                
                case KEY_DOWN: //Key dn //Controllo: evitare che vada oltre il bordo basso dello schermo
                    //if(starship.y+1 > maxy)
                    //starship.o_type= ASTRONAVE;
                    //starship.y+=1;
                    starship.y += 1;
                    break;
                
                case 'q': /* Tasto 'q' */           //Abolire questo comando perchè non voglio uscire dal programma in questo modo
                    endwin();
                    exit(0);
                    break; //Ridondante (e caso inutile)

                case ' ': //Barra spaziatrice
                    //Lancio dei missili

                    break;

                default:
                    //starship.newy= 0;
                    break;
            }
            //starship.newy= 65;
        
            write(filedescriptor, &starship, sizeof(starship));      /* Comunico al processo padre le coordinate del contadino */
        }
    }

 

    //Il flusso di esecuzione non deve mai arrivare qua
    exit(-130);
    kill(getpid(),1);
}

//Processo nemico (per ora solo lui di livello 1)
void enemy(int filedescriptor[2]){
    //Iniziamo l'i-esimo nemico, che passeremo di continuo

    //enemy->pid = getpid();

    while(true){
        //close(pipeout); /* chiusura del descrittore di lettura -> best pratice?*/
    }

    //Il flusso di esecuzione non deve mai arrivare qua
    exit(-131);
    kill(getpid(),1);
}

void missile(int filedescriptor){
    //Date delle coordinate che ha da astronave, lancia due mini oggetti, o meglio fa scrivere di continuo sulla pipe due oggetti
    //Il problema è fare in un certo senso due fork con due direzioni diveerse, quindi fare una sorta di if else e poi if else  
}

void bomb(int filedescriptor){

}



void areagioco(int filedescriptor){
    
    bool game = true; //Variabile di supporto per la gestione del senso di areagioco

    int adchoice, edchoice; //Astro Drawing Choice, Enemy Drawing Choice
    adchoice = rand() % ASTRODCHOICE;
    edchoice = rand() % ENEMYDCHOICE;


    //Primi disegni / disegni iniziali: astronave e gli M nemici

    init_pair(1,COLOR_RED,COLOR_BLACK);   /* Colore oggetto */
    init_pair(2,COLOR_BLACK,COLOR_BLACK); /* Colore per cancellare */
    init_pair(3,COLOR_GREEN,COLOR_BLACK); /* Colore per cancellare */

    //attron(COLOR_PAIR(3)); 

    //Disegno iniziale della astronave TODO: scegliere dove posizionarla all'inizio
    int j;
    for(j = 0; j < 5; j++){                                 //Attenzione al magic number 5 necessario $$
        mvprintw(j,0, astroshipsprites[adchoice][j]);       //Stampa in posizione (x,y) = (0,0)
    }
    sleep(1);
    refresh();

    object obj;                                         //Variabile temporanea per l'oggetto in entrata
    //object astronave;                                      //Variabile temporanea per lastronave
    object nemico;                                         //Attenzione: fare una malloc di M nemici e poi fare una realloc $$
    object missile;                                        //Stesso discorso $$
    object bomba;                                          //Stesso discorso $$

    
    int i = 0;                                              //cONTATORE per la exit

    
    
    attron(COLOR_PAIR(1)); 

    //mvprintw(12,9,"LETTURAx IN CORSO");
    //sleep(1);
    refresh();

    read(filedescriptor, &obj, sizeof(object));

    do{
        //game = true;

        //Area gioco si occupa di disegnare le figure e gestire le collisioni tra i vari oggetti
        //DISEGNARE I VARI OGGETTI
        //GESTIRE LE VARIE COLLISIONI PREVISTE DALLE SPECIFICHE

        //Lettura delle coordinate di astronave, quindi disegno
        

        //Lettura dalla pipe!
        
        /* Leggo dalla pipe */
		read(filedescriptor, &obj, sizeof(object));
        
        
        //VEspa e contadino fa un controllo sulla prima lettura, che per ora ignoro
        
        //Devo capire quale oggetto ho di fronte: uso il parametro otype
        //RIDISEGNO OGNI SINGOLO OGGETTO CHE MI ARRIVA ALLA PIPE
            //Richiamo una funzione apposita che, dato in input l'area da cancellare, la cancella e scrive nella nuova area lo sprite adatto
            

        drawingobject(obj);
       
        refresh();

        //Goloso codice per dare "tempo a questa funzione", che altrimenti chiuderebbe subito visto che le collisioni non sono state implementate
        if(i == 3000000)
            game = false;  
        //Fine del codice goloso     
        i++;

        refresh();
        
    }while(game);
}

/**
 * @brief Getting the object and re-painting into main window (with ncurse function)
 * 
 * @param obj The object to (re)draw
 */
void drawingobject(object obj){

    //First step: getting the area to erase
    
    int i; //indica il numero di colonne per cui iterare la cancellazione: ogni oggetto in realtà ha dimensioni diverse (nemici a parte)
    switch(obj.o_type){
        case ASTRONAVE:
            i = ASTRODIM;
            break;

        case NEMICO1:
            i = ENEMYDIM;
            break;
        
        case NEMICO2:
            i = ENEMYDIM;
            break;

        case BOMBA:
            i = 1;
            break;
        
        case MISSILE:
            i = 1;
            break;
    }
    //mvprintw(0,0, "VALORE: %d", i);

    //TODO: mettere in una sotto funzione questo switch, cosi aumento modularità e leggerezza dell flusso logico di esecuzione
    int j,k;                    //Indica j per le colonne, indice k per le righe: NOTA potrebbe essere più intuitivo chiamarel deltax e delta y
    for(j =0; j < i; j++)
        for(k =0; k < i; k++)             //+ 1 non sicuro
            mvprintw(obj.oldy + j, obj.oldx + k, " ");
        


    //Second step: drawing the new area
    //We need to pass not cthe space character but the i-th from the correct array

    //j=3;
    for(j =0; j < i; j++)
        mvprintw(obj.y + j, obj.x, astroshipsprites[obj.dtype][j]);

    //In teoria ora dovrei salvare le nuove coordinare di x e y, ma è da fare da un'altra parte
    //Questo non vale per l'oggetto astronave
    //obj.x += obj.newx;
    //obj.y += obj.newy;
    //Attenzione: abbiamo deciso cosi il funzionamento del movimento degli oggetti, per cui newx e newy rappresentano dei delta da sommare
    

}