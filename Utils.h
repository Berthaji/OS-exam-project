#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Structs.h"

bool HasEnemyCollided(object enemy, object* missiles, int missilesCount);
int MissileCollided(object* enemy, object missiles, int enemiesCount);
int AstroCollided(object astroship, object dangerousObj);


bool range(int min, int max, int value);