#pragma once 
#include "tools.h"
#include "windowManager.h"
#include "List.h"

typedef enum {
    BLT_DIRT,
    BLT_STONE,
    BLT_CRYSTAL,
    BLT_ENERGY
}TypeBulletsEnum;

typedef struct {
    sfVector2f position;
    sfVector2f forward;
    sfFloatRect colRect;
    sfBool isBack;
    TypeBulletsEnum type;
    float velocity;
    float rotation;
    sfIntRect rect;
	sfBool isHoming;
}Bullets_struct;

stdList* bulletsList;

void blt_initBullets();
void blt_addBullets(sfVector2f _position, sfVector2f _forward, TypeBulletsEnum _type);
void blt_updateBullets(sfVector2f _playerPos);

void blt_displayBullets(Window* _window);
/// <summary>
/// is there a nut 
/// </summary>
/// <returns> if there is a nut</returns>
sfBool isNutHere();

/// <summary>
/// get the float rect of nuts
/// </summary>
/// <returns> the float rect</returns>
sfFloatRect* blt_getNutFloatRect();

sfFloatRect blt_GetNutBounds(Bullets_struct* _nut);

void blt_deinitBullets();