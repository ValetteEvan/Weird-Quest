#pragma once 
#include "List.h"
#include "tools.h"
#include "windowManager.h"

typedef struct{
	sfVector2f pos;
	sfBool isTouched;
	sfIntRect rect;
	sfFloatRect bound;
	
}SpidersTreeStruct;

stdList* spidersTreesList;

void stree_initSpidersTrees();
void stree_addSpidersTrees(sfVector2f _pos);
void stree_updateSpidersTrees(Window* _window);
void stree_displaySpidersTreesBehindPlayer(Window* _window);
void stree_displaySpiderTreesInFrontOfPlayer(Window* _window);
void stree_clearSpidersTrees();