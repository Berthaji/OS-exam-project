#include "Utils.h"

#include <ncurses.h>

/* Da eliminare
*/
bool HasEnemyCollided(object enemy, object* missiles, int missilesCount){
    int i;
    for(i = 0; i < missilesCount; i++){
        if( 
            /* 
            range(-4,3, enemy.x - missiles[i].x) && *7-4,2
             range(0,3, enemy.y - missiles[i].y)     0,3
             */
            range(enemy.x, enemy.x +3 , missiles[i].x) &&
            range(enemy.y, enemy.y +3 , missiles[i].y)

            ){
                /* mvprintw(0,0, "x1: %d, y1:%d; x2: %d, y2:%d; diffx: %d, diffy: %d", enemy.x, enemy.y, missiles[i].x, missiles[i].y, enemy.x - missiles[i].x,enemy.y - missiles[i].y);
                 refresh();
                 sleep(10);*/
                return true;

            }

    }
    return false;    
}

int MissileCollided(object* enemy, object missiles, int enemiesCount){
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


int AstroCollided(object astroship, object dangerousObj){
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



bool range(int min, int max, int value) {
    return (min <= value && value <= max);
}