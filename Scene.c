
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
        {"==>  "}}

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
        {"X X"}}};

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
}

/**
 * @brief Drawing objects in the scene
 * 
 */
void drawScene(
    object astroship,
    object *enemies, int enemiesCount,
    object *enemies2, int enemies2Count,
    object *missiles, int missilesCount,
    object *bombs, int bombsCount)
{
    clearScreen();

    drawObject(astroship);

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
