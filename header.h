

//Dimensione schermo
#define SCREEN_X 80     //Dimensione schermo asse x
#define SCREEN_Y 120    //Dimensione schermo asse y

#define ASTRODCHOICE 4   //Numero delle possibili astronavi tra cui scegliere
#define ENEMYDCHOICE 4   //Numero dei possibil nemici tra cui scegliere
#define ASTRODIM 5      //Dimensione dello sprite astronave in lunghezza e larghezza
#define ENEMYDIM 3      //Dimensione dello sprite nemico in lunghezza e larghezza

//Tasti freccia
#define UP 65		/* Cursore sopra */
#define DW 66		/* Cursore sotto */
#define SX 68		/* Cursore sinistra */
#define DX 67		/* Cursore destra */

#define M 3             //Numero dei nemici (come da specifiche del progetto)
int delaytime = 10000;  //Tempo di delay per rallentare/velocizzare l'esperienza di gioco

//Tipologia di oggetto
//typedef enum  {ASTRONAVE = 2, NEMICO1 = 3, NEMICO2 = 4, BOMBA = 5, MISSILE=6} otype;
typedef enum  {ASTRONAVE, NEMICO1, NEMICO2 , BOMBA , MISSILE} otype;

//Stato dell'oggetto
typedef enum {NOTINIZIALIZED, INIZIALIZED} ktype;


//Struct per il singolo oggetto che opera all'interno del gioco
typedef struct {
    //Posizione
    int x;
    int y;
    int oldx;
    int oldy;

    //Processo
    pid_t pid;

    //Aspetto
    int color;
    otype o_type;  //Tipologia di oggetto (astronave, nemico, bomba, missile etc)
    ktype k_type;
    int dtype;   //Tipologia grafica di astronave/nemico. $$ -1 indica l'essere una bomba o un missile per ora
    
}object;
//Cambiare il nome da oggetto a object

/**
 * Nell'array di struct, l'indice in posizione 0 rappresenta l'astronave;
 * quelli successivi, al momento della sua creazione, i nemici,
 * seguono missili e bombe.
 *
 */


//Disegni possibili delle astronavi e nemici per dare più scelta

char astroshipsprites[ASTRODCHOICE][5][6]={ //ben 4 scelte possibili
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


char enemysprites[ENEMYDCHOICE][3][4]={
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


