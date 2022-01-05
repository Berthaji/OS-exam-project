
#include "Scene.h"

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

/**
 * @brief Replaces every character on the screen with a whitespace
 * 
 */
void clearScreen()
{
    for (int y = 0; y < SCREEN_H; y++)
        for (int x = 0; x < SCREEN_W; x++)
            mvaddch(y, x, ' ');
}

/**
 * @brief Initialize display settings 
 * 
 */
void initScreen()
{

    initscr();
    noecho();
    keypad(stdscr, 1);
    curs_set(0);
    start_color();

    init_pair(1, COLOR_RED, COLOR_BLACK);     /* Colore oggetto */
    init_pair(2, COLOR_WHITE, COLOR_BLACK);   /* Colore per cancellare */
    init_pair(3, COLOR_GREEN, COLOR_BLACK);   /* Colore per cancellare */
    init_pair(4, COLOR_BLUE, COLOR_BLACK);    /* Colore per cancellare */
    init_pair(5, COLOR_CYAN, COLOR_BLACK);    /* Colore per cancellare */
    init_pair(6, COLOR_MAGENTA, COLOR_BLACK); /* Colore per cancellare */
    init_pair(7, COLOR_RED, COLOR_BLACK);     /* Colore per cancellare */
    init_pair(8, COLOR_YELLOW, COLOR_BLACK);  /* Colore per cancellare */

  //  attron(COLOR_PAIR(8));
    // attroff(COLOR_PAIR(8));
}

/**
 * @brief Drawing objects in the scene
 * 
 */
void drawScene(
    object *astroship,
    object *enemies, int enemiesCount,
    object *enemies2, int enemies2Count,
    object *missiles, int missilesCount,
    object *bombs, int bombsCount)
{
    clearScreen();

    drawObject(*astroship);

    int i;
    for (i = 0; i < enemiesCount; i++)
        if (enemies[i].state != DEAD)
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
void drawObject(object entity){
    //attron(COLOR_PAIR(entity.color)); //Colore del singolo oggetto

    switch (entity.type){
        case ASTROSHIP:{
            for (int j = 0; j < ASTRODIM; j++)
                mvprintw(entity.y + j, entity.x, astroship_sprites[entity.appearance][j]);
            break;
        }

        case ENEMY1:{
            for (int j = 0; j < ENEMYDIM; j++)
                mvprintw(entity.y + j, entity.x, enemy_sprites[entity.appearance][j]);
            break;
        }

        case ENEMY2:{
            for (int j = 0; j < ENEMYDIM; j++)
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

//Ridimensionamento iniziale
void drawMenu1(){
    clearScreen();

    //Per ora impostiamo un range ottimale per la visione del gioco, ma lo facciamo impostare al giocatore
    // 30<y<40 e 80<x<90
    bool setScreen = false;

    while(!setScreen){
        //Dimensioni attuali dello schermo
        int y = SCREEN_H; 
        int x = SCREEN_W;
        clearScreen();
        
        if( (30 < y && y < 40)  && (80 < y && y < 90) ){ //Dimensioni raggiunte
            mvprintw(y/2, x/2 - 11, "Dimensioni nel range!"); // 11 è la metà della scritta "Dimensioni nel range"
            setScreen = true;
        }
        else{   //Dimensioni non raggiunte
            mvprintw(y/2, x/2 - 17, "Dimensioni attuali: y: %d e x: %d", y, x);
            mvprintw((y/2)+1, x/2 - 22, "Dimensioni ideali: 30 < y < 40, 80 < x < 90"); //22 è la metà della scritta
        }
        refresh();
    }

}

//Schermata home iniziale
void drawMenu2(){

}

//Impostazioni di gioco
void drawMenu3(){

}

//Ricorda i punti di gioco $$ e la possibilità di rigiocare dal menu 3 
void gameWin(){
    //Vedi se lasciare la schermata statica e un whileTrue per il getch oppure copiare test2
}

void gameLose(){

}

//-- Funzioni comuni sia alla processi thread che alla versione  --//

//Da rivedere...
bool gEnemy1(object * o, bool bo){
    o->y += -1;
    o->x = 10;

    /**
     * se il nemico è fuori dallo schermo dobbiamo terminare il processo
     * perciò terminiamo il loop, comunichiamo al processo principale che il nemico 
     * è "morto" e chiudiamo il processo nemico
     */

    //se il nemico non è ai bordi si può muovere tranquillamente, altrimenti lo spostiamo più dentro
    if (o->y >= SCREEN_H -2){    //Bordo inferiore superato
        o->y = o->y--;
        o->dir = 1;
    }
    if (o->y <= 0 ){    //Bordo superiore superato
        o->y = o->y++;
        o->dir = 0;
    }

    if (o->x < -5){
        bo = false;
        o->state = KILLED;
    }
    return bo;
}


bool gMissile(object * o, bool bo){
    
    o->x++;                     //sposto il missile verso destra 
    o->y += (o->dir ? -1 : 1)*0.625;      //$$diagonale vera - //sposto il missile in basso o in alto a seconda della sua direzione


    /**
     * se il missile è fuori dallo schermo dobbiamo terminare il processo
     * perciò terminiamo il loop, comunichiamo al processo principale che il missile 
     * è "morto" e chiudiamo il thread missile
     */

    //se il nemico non è ai bordi si può muovere tranquillamente, altrimenti lo spostiamo più dentro
    /**
     * se il missile è fuori dallo schermo dobbiamo terminare il processo
     * perciò terminiamo il loop, comunichiamo al processo principale che il proiettile 
     * è "morto" e chiudiamo il processo missile
     */
    if (o->y < -2 || o->y > SCREEN_H + 2){
        bo = false;
        o->state = DEAD;
    }

    return bo;        
}



