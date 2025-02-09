#pragma once
#include "tools.h"
#include "windowManager.h"
#include "List.h"


typedef struct{
	sfVector2f pos;
	sfBool isOnScreen;
	sfBool isCut;
	float timerAnim;
	int frameX;
	int type;
	sfBool hasSpawnedSnake;
}Grass;

stdList* listGrass;

void getGrassTexture();

void initGrass();
void updateGrass();
void displayGrass(Window* _window);
void addGrass(sfVector2f _pos, int _type);
void deleteGrass(sfVector2f _pos);
void clearGrass();
void shouldGrassBeCut(sfVector2f _posSlash);
sfBool isPlayerOnGrass(sfVector2i _posPlayerTile);