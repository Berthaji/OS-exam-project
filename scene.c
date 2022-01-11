/**
 * @file scene.c
 * @brief Libreria visiva del gioco
 */

#include "scene.h"

/* Schermate di gioco */
char arrStart [DIM_ARRSTART_Y][DIM_ARRSTART_X] = {
{"                             /\\\\\\\\\\\\\\\\\\\\\\                                                                                                               "},                
{"                            /\\\\\\/////////\\\\\\                                                                                                            "},                
{"                             \\//\\\\\\      \\///    /\\\\\\\\\\\\\\\\\\                                                                                             "},                
{"                              \\////\\\\\\          /\\\\\\/////\\\\\\  /\\\\\\\\\\\\\\\\\\        /\\\\\\\\\\\\\\\\     /\\\\\\\\\\\\\\\\                                                 "},                
{"                                  \\////\\\\\\      \\/\\\\\\\\\\\\\\\\\\\\  \\////////\\\\\\     /\\\\\\//////    /\\\\\\/////\\\\\\                                               "},                
{"                                      \\////\\\\\\   \\/\\\\\\//////     /\\\\\\\\\\\\\\\\\\\\   /\\\\\\          /\\\\\\\\\\\\\\\\\\\\\\                                               "},                
{"                                /\\\\\\      \\//\\\\\\  \\/\\\\\\          /\\\\\\/////\\\\\\  \\//\\\\\\        \\//\\\\///////                                               "},                
{"                                \\///\\\\\\\\\\\\\\\\\\\\\\/   \\/\\\\\\         \\//\\\\\\\\\\\\\\\\/\\\\  \\///\\\\\\\\\\\\\\\\  \\//\\\\\\\\\\\\\\\\\\\\                                            "},                
{"                                   \\///////////     \\///           \\////////\\//     \\////////    \\//////////                                            "},                
{"                                                                                                                                              "},                
{"               /\\\\\\                        /\\\\\\\\\\                                        /\\\\\\                                                       "},                
{"               \\/\\\\\\                      /\\\\\\///                                        \\/\\\\\\                                                      "},                
{"                \\/\\\\\\                     /\\\\\\                                            \\/\\\\\\                                                     "},                
{"                 \\/\\\\\\       /\\\\\\\\\\\\\\\\    /\\\\\\\\\\\\\\\\\\     /\\\\\\\\\\\\\\\\    /\\\\/\\\\\\\\\\\\           \\/\\\\\\       /\\\\\\\\\\\\\\\\    /\\\\/\\\\\\\\\\\\\\                         "},                
{"             /\\\\\\\\\\\\\\\\\\     /\\\\\\/////\\\\\\  \\////\\\\\\//    /\\\\\\/////\\\\\\  \\/\\\\\\////\\\\\\     /\\\\\\\\\\\\\\\\\\     /\\\\\\/////\\\\\\  \\/\\\\\\/////\\\\\\                       "},                
{"             /\\\\\\////\\\\\\    /\\\\\\\\\\\\\\\\\\\\\\      \\/\\\\\\     /\\\\\\\\\\\\\\\\\\\\\\   \\/\\\\\\  \\//\\\\\\   /\\\\\\////\\\\\\    /\\\\\\\\\\\\\\\\\\\\\\   \\/\\\\\\   \\///                       "},                
{"             \\/\\\\\\  \\/\\\\\\   \\//\\\\///////       \\/\\\\\\    \\//\\\\///////    \\/\\\\\\   \\/\\\\\\  \\/\\\\\\  \\/\\\\\\   \\//\\\\///////    \\/\\\\\\                              "},                
{"              \\//\\\\\\\\\\\\\\/\\\\   \\//\\\\\\\\\\\\\\\\\\\\     \\/\\\\\\     \\//\\\\\\\\\\\\\\\\\\\\  \\/\\\\\\   \\/\\\\\\  \\//\\\\\\\\\\\\\\/\\\\   \\//\\\\\\\\\\\\\\\\\\\\  \\/\\\\\\                            "},                
{"                \\///////\\//     \\//////////      \\///       \\//////////   \\///    \\///    \\///////\\//     \\//////////   \\///                            "},   
{"                                                                                                                                            "},                
{" ______   __  __     ______        ______     ______     __    __     ______     ______     ______   ______     ______     ______     _____    "}, 
{"/\\__  _\\ /\\ \\_\\ \\   /\\  ___\\      /\\  == \\   /\\  ___\\   /\\ \"-./  \\   /\\  __ \\   /\\  ___\\   /\\__  _\\ /\\  ___\\   /\\  == \\   /\\  ___\\   /\\  __-.     "},                
{"\\/_/\\ \\/ \\ \\  __ \\  \\ \\  __\\      \\ \\  __<   \\ \\  __\\   \\ \\ \\-./\\ \\  \\ \\  __ \\  \\ \\___  \\  \\/_/\\ \\/ \\ \\  __\\   \\ \\  __<   \\ \\  __\\   \\ \\ \\/\\ \\    "}, 
{"   \\ \\_\\  \\ \\_\\ \\_\\  \\ \\_____\\     \\ \\_\\ \\_\\  \\ \\_____\\  \\ \\_\\ \\ \\_\\  \\ \\_\\ \\_\\  \\/\\_____\\    \\ \\_\\  \\ \\_____\\  \\ \\_\\ \\_\\  \\ \\_____\\  \\ \\____-    "},                
{"    \\/_/   \\/_/\\/_/   \\/_____/      \\/_/ /_/   \\/_____/   \\/_/  \\/_/   \\/_/\\/_/   \\/_____/     \\/_/   \\/_____/   \\/_/ /_/   \\/_____/   \\/____/ "}, 
{"                                                                                                                                                "},                
{"                                     __   __   ______     ______     ______     __     ______     __   __                                       "},     
{"                                    /\\ \\ / /  /\\  ___\\   /\\  == \\   /\\  ___\\   /\\ \\   /\\  __ \\   /\\ \"-.\\ \\                                      "}, 
{"                                    \\ \\ \\'/   \\ \\  __\\   \\ \\  __<   \\ \\___  \\  \\ \\ \\  \\ \\ \\/\\ \\  \\ \\ \\-.  \\                                    "},                
{"                                     \\ \\__|    \\ \\_____\\  \\ \\_\\ \\_\\  \\/\\_____\\  \\ \\_\\  \\ \\_____\\  \\ \\_\\\\\"\\_\\                                   "}, 
{"                                      \\/_/      \\/_____/   \\/_/ /_/   \\/_____/   \\/_/   \\/_____/   \\/_/ \\/_/                                    "},                
{"                                                                                                                                               "}, 
{"                                                                                                                                               "},                
{"                                                        Premi un tasto per giocare...                                                              "},            
{"                                                                                                                                               "}                                                            
};


char arrMenu[DIM_ARRMENU][DIM_ARRMENU_Y][DIM_ARRMENU_X] = {
 {
  {"Beh, vedi, come dire, cioè ecco...                                            "},
  {"La razza umana ha deciso di popolare troppi pianeti, motivo per cui tu,       "},
  {"squattrinato come sempre, oltre a lavorare 12 ore al giorno come pizzaiolo,   "},
  {"nel tempo libero devi anche prendere la tua navicella baracca,                "},
  {"andare nello spazio e minacciare l'esistenza di poveri alieni.                "},
  {"Del resto sono dei poracci che non hanno fatto niente di male nella loro vita,"},
  {"a parte averti incontrato.                                                    "},
  {"Divertiti!                                                                    "},
  {"                                                                              "},
  {"Seleziona la modalità di gioco (spazio per confermare)                        "},
  {"> FACILE                                                                      "},
  {"  MEDIA                                                                       "},
  {"  DIFFICILE                                                                   "},
  {"  ABBASTANZA CASUALE                                                          "},
  {"                                                                              "},
  {"                                                                              "}
 },
 {
  {"Beh, vedi, come dire, cioè ecco...                                            "},
  {"La razza umana ha deciso di popolare troppi pianeti, motivo per cui tu,       "},
  {"squattrinato come sempre, oltre a lavorare 12 ore al giorno come pizzaiolo,   "},
  {"nel tempo libero devi anche prendere la tua navicella baracca,                "},
  {"andare nello spazio e minacciare l'esistenza di poveri alieni.                "},
  {"Del resto sono dei poracci che non hanno fatto niente di male nella loro vita,"},
  {"a parte averti incontrato.                                                    "},
  {"Divertiti!                                                                    "},
  {"                                                                              "},
  {"Seleziona la modalità di gioco (spazio per confermare)                        "},
  {"  FACILE                                                                      "},
  {"> MEDIA                                                                       "},
  {"  DIFFICILE                                                                   "},
  {"  ABBASTANZA CASUALE                                                          "},
  {"                                                                              "},
  {"                                                                              "}
 },
 {
  {"Beh, vedi, come dire, cioè ecco...                                            "},
  {"La razza umana ha deciso di popolare troppi pianeti, motivo per cui tu,       "},
  {"squattrinato come sempre, oltre a lavorare 12 ore al giorno come pizzaiolo,   "},
  {"nel tempo libero devi anche prendere la tua navicella baracca,                "},
  {"andare nello spazio e minacciare l'esistenza di poveri alieni.                "},
  {"Del resto sono dei poracci che non hanno fatto niente di male nella loro vita,"},
  {"a parte averti incontrato.                                                    "},
  {"Divertiti!                                                                    "},
  {"                                                                              "},
  {"Seleziona la modalità di gioco (spazio per confermare)                        "},
  {"  FACILE                                                                      "},
  {"  MEDIA                                                                       "},
  {"> DIFFICILE                                                                   "},
  {"  ABBASTANZA CASUALE                                                          "},
  {"                                                                              "},
  {"                                                                              "}
 },
 {
  {"Beh, vedi, come dire, cioè ecco...                                            "},
  {"La razza umana ha deciso di popolare troppi pianeti, motivo per cui tu,       "},
  {"squattrinato come sempre, oltre a lavorare 12 ore al giorno come pizzaiolo,   "},
  {"nel tempo libero devi anche prendere la tua navicella baracca,                "},
  {"andare nello spazio e minacciare l'esistenza di poveri alieni.                "},
  {"Del resto sono dei poracci che non hanno fatto niente di male nella loro vita,"},
  {"a parte averti incontrato.                                                    "},
  {"Divertiti!                                                                    "},
  {"                                                                              "},
  {"Seleziona la modalità di gioco (spazio per confermare)                        "},
  {"  FACILE                                                                      "},
  {"  MEDIA                                                                       "},
  {"  DIFFICILE                                                                   "},
  {"> ABBASTANZA CASUALE                                                          "},
  {"                                                                              "},
  {"                                                                              "}
 }
};

char astroship_sprites[ASTRODCHOICE][5][6] = { //4 possibile choices
    //Astroship 1
    {
        {"-    "},
        {" --> "},
        {"<!>--"},
        {" --> "},
        {"-    "}},
    //Astroship 2
    {
        {"<=-  "},
        {" --- "},
        {" oooo"},
        {" --- "},
        {"<=-  "}},
    //Astroship 3
    {
        {"]    "},
        {" ||  "},
        {"<>==="},
        {" ||  "},
        {"]    "}},
    //Astroship 4
    {
        {"==>  "},
        {" ==> "},
        {"  ]=="},
        {" ==> "},
        {"==>  "}
    }

};

char enemy_sprites[ENEMYDCHOICE][3][4] = {
    //Enemy 1
    {
        {" --"},
        {"<=="},
        {" --"}},

    //Enemy 2
    {
        {"  x"},
        {"<=="},
        {"  x"}},

    //Enemy 3
    {
        {"<= "},
        {"oo-"},
        {"<= "}},

    //Nemico 4 (il nemico di secondo livello)
    {
        {"X X"},
        {"   "},
        {"X X"}
    }
};



//Schermate di vittoria e sconfitta
char win[13][103+1] = {

                                                                                                                  
{"oooooo     oooo ooooo ooooooooooooo ooooooooooooo   .oooooo.   ooooooooo.   ooooo       .o.       .o. "},
{" `888.     .8'  `888' 8'   888   `8 8'   888   `8  d8P'  `Y8b  `888   `Y88. `888'      .888.      888 "},
{"  `888.   .8'    888       888           888      888      888  888   .d88'  888      .8\"888.     888"},
{"   `888. .8'     888       888           888      888      888  888ooo88P'   888     .8' `888.    Y8P "},
{"    `888.8'      888       888           888      888      888  888`88b.     888    .88ooo8888.   `8' "},
{"     `888'       888       888           888      `88b    d88'  888  `88b.   888   .8'     `888.  .o. "},
{"      `8'       o888o     o888o         o888o      `Y8bood8P'  o888o  o888o o888o o88o     o8888o Y8P "},
{"                                                                                                      "},
{"                Complimenti!                                                                          "},
{"                Ora che abbiamo spazio, posso sistemare la mia collezione di teschi alieni            "},
{"                                                                                                      "},
{"                Se vuoi rigiocare, premi la barra spaziatrice, altrimenti q per uscire                "},                                                                           
{"                                                                                                      "}                                                                                                                    


};


char lose[14][121+1] = {


{" .oooooo..o   .oooooo.     .oooooo.   ooooo      ooo oooooooooooo ooooo ooooooooooooo ooooooooooooo       .o.       .o. "},
{"d8P'    `Y8  d8P'  `Y8b   d8P'  `Y8b  `888b.     `8' `888'     `8 `888' 8'   888   `8 8'   888   `8      .888.      888 "},
{"Y88bo.      888          888      888  8 `88b.    8   888          888       888           888          .8\"888.      888"},
{" `\"Y8888o.  888         888      888  8   `88b.  8   888oooo8     888       888           888         .8' `888.    Y8P "},
{"     `\"Y88b 888          888      888  8     `88b.8   888    \"     888       888           888        .88ooo8888.   `8' "},
{"oo     .d8P `88b    ooo  `88b    d88'  8       `888   888          888       888           888       .8'     `888.  .o. "},
{"8""88888P'   `Y8bood8P'   `Y8bood8P'  o8o        `8  o888o        o888o     o888o         o888o     o88o     o8888o Y8P "},
{"                                                                                                                        "},
{"                         Peccato, sarà per un'altra volta                                                               "},
{"                         Nel frattempo torna a consegnare pizze, va'                                                    "},
{"                                                                                                                        "},
{"                                                                                                                        "},
{"                         Se vuoi rigiocare, premi la barra spaziatrice, altrimenti q per uscire                         "},                                                                              
{"                                                                                                                        "}

};


/* Funzioni di supporto per la stampa */

 /**
  * @brief Inizializza lo schermo per usare ncurses
  * 
  */
void initScreen(){

    initscr();
    noecho();
    keypad(stdscr, 1);
    curs_set(0);
    start_color();
    cbreak();

    init_pair(1, COLOR_RED, COLOR_BLACK);     /* Colore oggetto */
    init_pair(2, COLOR_WHITE, COLOR_BLACK);   /* Colore per cancellare */
    init_pair(3, COLOR_GREEN, COLOR_BLACK);   /* Colore per cancellare */
    init_pair(4, COLOR_BLUE, COLOR_BLACK);    /* Colore per cancellare */
    init_pair(5, COLOR_CYAN, COLOR_BLACK);    /* Colore per cancellare */
    init_pair(6, COLOR_MAGENTA, COLOR_BLACK); /* Colore per cancellare */
    init_pair(7, COLOR_RED, COLOR_BLACK);     /* Colore per cancellare */
    init_pair(8, COLOR_YELLOW, COLOR_BLACK);  /* Colore per cancellare */
}

/**
 * @brief Restituisce 1 (vero) se ci non ci sono caratteri da leggere, 0 (falso) altrimenti
 * 
 * @return int 
 */
int kbhit(void){
    int ch,r;

    /* disattiva il blocco getch() e echo */
    nodelay(stdscr,TRUE);
    noecho();

    /* controlla per input */
    ch = getch();
    if( ch == ERR) /* nessun input */
        r = FALSE;

    else{ /* input */
        r = TRUE;
        ungetch(ch);
    }

    /* ripristina il blocco getch() e echo */
    echo();
    nodelay(stdscr,FALSE);
    return(r);
}

/**
 * @brief Pulizia schermo
 * 
 */
void clearScreen(){
    for (int y = 0; y < SCREEN_H; y++)
        for (int x = 0; x < SCREEN_W; x++)
            mvaddch(y, x, ' ');
}


/* Stampa a video delle varie sezioni di gioco (es. menu, schemrate di vittoria, sconfitta...) */

/**
 * @brief Disegno della schermata principale
 * 
 */
void drawSplashScreen(){
    bool bo = false;
    int i = 1;

    attron(A_BOLD);
    while(!kbhit()){
        i++;
        if(i % DIM_COLORS == 0)
            i = 1;
    
        attron(COLOR_PAIR(i));

        /* Prima parte della scritta - space defender */
        
        /* Effetto scattoso */
        for (int y = 0; y < 40; y++)
            for (int x = 0; x < DIM_ARRSTART_X_COLUMNS; x++)
                mvaddch(y, x, ' ');
            

        for (int y = 0; y < DIM_ARRSTART_Y_FIRSTLINE; y++)
            for (int x = 0; x < DIM_ARRSTART_X_COLUMNS; x++){
                if(arrStart[y][x] == '\\' && bo)
                    mvaddch(y, x, arrStart[y][x]);
                if(arrStart[y][x] == '/' && !bo)
                    mvaddch(y, x, arrStart[y][x]);
            }

        /*Seconda parte della scritta - the remastered version */
        for (int y = DIM_ARRSTART_Y_FIRSTLINE; y < DIM_ARRSTART_Y_SECONDLINE; y++)
            for (int x = 0; x < DIM_ARRSTART_X_COLUMNS; x++)
                    mvaddch(y, x, arrStart[y][x]);
            

        refresh();
        bo = !bo;
        attroff(COLOR_PAIR(i));
        usleep(DELAY_ARRSTART);
    }
    attroff(COLOR_PAIR(i));
}

int drawMenu(){
    int choice = 0;
    bool chosen = false;

    for (int y = 0; y < DIM_ARRMENU_Y; y++)
        mvprintw(y,0, arrMenu[0][y]);

    while(!chosen){
        refresh();
        char ch = getch();
        switch (ch){
            case KEY_UP & A_CHARTEXT:
                if(!(choice == 0)){ /* Non siamo al bordo superiore */
                    choice--;
                    for (int y = 0; y < DIM_ARRMENU_Y; y++)
                        mvprintw(y,0, arrMenu[choice][y]);
                }
                break;

            case KEY_DOWN & A_CHARTEXT:
                if(!(choice == 3)){ //Non siamo al bordo inferiore
                    choice++;
                    for (int y = 0; y < DIM_ARRMENU_Y; y++)
                        mvprintw(y,0, arrMenu[choice][y]);
                }
                break;

            case ' ':
                chosen = true;
                break;
        }
        clearScreen();
        for (int y = 0; y < DIM_ARRMENU_Y; y++)
            mvprintw(y,0, arrMenu[choice][y]);
        refresh();
    }
    return choice;
}

/**
 * @brief Funzione per il disegno degli oggetti nella scena
 * 
 */
void drawScene(
    Object *astroship,
    Object *enemies, int enemiesCount,
    Object *enemies2, int enemies2Count,
    Object *missiles, int missilesCount,
    Object *bombs, int bombsCount)
{
    clearScreen();

    drawObject(*astroship);

    int i;
    for (i = 0; i < enemiesCount; i++)
        if (enemies[i].state != DEAD && enemies[i].state != KILLED)
            drawObject(enemies[i]);

    for (i = 0; i < enemies2Count; i++)
        if (enemies2[i].state != DEAD)
            drawObject(enemies2[i]);

    for (i = 0; i < bombsCount; i++)
        if (bombs[i].state != DEAD)
            drawObject(bombs[i]);

    for (i = 0; i < missilesCount; i++)
        if (missiles[i].state != DEAD)
            drawObject(missiles[i]);

    refresh();
}

/**
 * @brief Disegno del singolo oggetto
 * 
 * @param entity Oggetto da disegnare
 */
void drawObject(Object entity){

    switch (entity.type){
        case ASTROSHIP:{
            int j;
            for (j = 0; j < ASTRODIM; j++)
                mvprintw(entity.y + j, entity.x, astroship_sprites[entity.appearance][j]);
            break;
        }

        case ENEMY1:{
            int j;
            for (j = 0; j < ENEMYDIM; j++)
                mvprintw(entity.y + j, entity.x, enemy_sprites[entity.appearance][j]);
            break;
        }

        case ENEMY2:{
            int j;
            for (j = 0; j < ENEMYDIM; j++)
                mvprintw(entity.y + j, entity.x, enemy_sprites[3][j]);
            break;
        }

        case MISSILE:
            mvprintw(entity.y, entity.x, "#");
            break;

        case BOMB:
            mvprintw(entity.y, entity.x, "O");
            break;
    }
    //attroff(COLOR_PAIR(entity.color));
}


/**
 * @brief Calcolo del rientro in un range specificato tra due coordinate
 * 
 * @param min 
 * @param max 
 * @param value 
 * @return true Vi è stato un match (quindi una collisione)
 * @return false altrimenti
 */
bool range(int min, int max, int value) {
    return (min <= value && value <= max);
}



/**
 * @brief Verifica collisioni con missile
 * 
 * @param enemy 
 * @param missiles 
 * @param enemiesCount 
 * @return int -1 Se non vi è stata collisione, l'id del nemico altrimenti
 */
int missileCollided(Object* enemy, Object missiles, int enemiesCount){
    int i;
    for(i = 0; i < enemiesCount; i++){
        if( range(enemy[i].x, enemy[i].x +3 , missiles.x) &&
            range(enemy[i].y, enemy[i].y +3 , missiles.y)       /*$$ 3 e 3 sono parametri della dimensione da dare con define*/
            ){
                return i;    
            }
    }
    return -1;    
}

/**
 * @brief Verifica le collisioni degli oggetti con la astronave
 * 
 * @param astroship 
 * @param dangerousObj 
 * @return int -1 Se non vi è stata collisione, l'id dell'oggetto altrimenti
 */
int astroCollided(Object astroship, Object dangerousObj){
    int i = 1;
    if( dangerousObj.x - astroship.x == 5 &&  
            dangerousObj.y - astroship.y < 5 && 
            dangerousObj.y - astroship.y >= 0)
        /* range(astroship.x, astroship.x , dangerousObj.x) &&
        // range(astroship.y, astroship.y +5 , dangerousObj.y)) */
        {
            return i;    
        }
    
    return -1;    
}






