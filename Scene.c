
#include "Scene.h"

char astroship_sprites[ASTRODCHOICE][5][6]={ //ben 4 scelte possibili
    //Astronave 1
    {
        {"-    "},
        {" --> "},
        {"<!>--"},
        {" --> "},
        {"-    "} 
    },
    //Astronave 2
    {
        {"<=-  "},
        {" --- "},
        {" oooo"},
        {" --- "},
        {"<=-  "}  
    },
    //Astronave 3
    {
        {"]    "},
        {" ||  "},
        {"<>==="},
        {" ||  "},
        {"]    "}  
    },
    //Astronave 4
    {
        {"  +++"},
        {"<-   "},
        {"  ->>"},
        {"<-   "},
        {"  +++"}  
    }
    
};


char enemy_sprites[ENEMYDCHOICE][3][4]={
    //Nemico 1
    {
        {" --"},
        {"<=="},
        {" --"}
    },

    //Nemico 2
    {
        {"   "},
        {"<=="},
        {"   "}
    },

    //Nemico 3
    {
        {"<= "},
        {"oo-"},
        {"<= "}
    },
    //Nemico 4 -> Nemico di secondo livello?
    {
        {"X X"},
        {"   "},
        {"X X"}
    }
};



void clearScreen()
{
    for (int y = 0; y < SCREEN_H; y++)
        for (int x = 0; x < SCREEN_W; x++)
            mvaddch(y, x, ' ');
}
void initScreen(){

    initscr();
    noecho();
    keypad(stdscr, 1); //Attenzione lavoriamo sullo schermo principale
    curs_set(0);
    start_color(); //Per la gestione dei colori

    init_pair(1, COLOR_RED, COLOR_BLACK);   /* Colore oggetto */
    init_pair(2, COLOR_BLACK, COLOR_BLACK); /* Colore per cancellare */
    init_pair(3, COLOR_GREEN, COLOR_BLACK); /* Colore per cancellare */
}

void drawScene(){
    clearScreen();
    object starship;
    //Inizializziamo inanzitutto l'astronave, che passeremo di continuo
    starship.k_type = NOT_INIZIALIZED; //Ridondante ma necessario (? evitiamo controlli di deffault sbagliati?)
    starship.x = 0;
    starship.y = 0;
    starship.o_type = ASTRONAVE;
    //starship->pid = getpid();
    starship.dtype = rand() % ASTRODCHOICE;
    starship.color = rand() % 7;

    //Ultimo comando prima di far partire il resto
    starship.k_type = INIZIALIZED;

    drawObject(starship);
}
void drawObject(object entity)
{
    int i = 1;

    switch (entity.o_type)
    {
    case ASTRONAVE:
        i = ASTRODIM;
        break;
    case NEMICO1:
    case NEMICO2:
        i = ENEMYDIM;
        break;
    }

    for (int j = 0; j < i; j++)
        mvprintw(entity.y + j, entity.x, astroship_sprites[entity.dtype][j]);
}
