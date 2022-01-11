#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Structs.h"

bool HasEnemyCollided(Object enemy, Object* missiles, int missilesCount);
int MissileCollided(Object* enemy, Object missiles, int enemiesCount);
int AstroCollided(Object astroship, Object dangerousObj);


bool range(int min, int max, int value);