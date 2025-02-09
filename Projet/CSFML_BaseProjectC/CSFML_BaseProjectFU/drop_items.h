#pragma once
#include "stateManager.h"
#include "textureManager.h"
#include "List.h"
#include "math.h"
#include "tools.h" 

stdList* itemsList;

typedef enum {
	HEART = 1,
}ITEM_Type;

typedef struct {
	sfVector2f position; 
	sfVector2f size;   
	sfVector2f origin;  
	sfTexture* texture;  
	ITEM_Type type;   
	float timeToDie; 
	float timerAnim; 
	float timerPrt; 
	int frameX; 
	float timerFlash; 
	float timerTempFlash; 
	sfBool bool_repeat; 
}ITEM; 

void initItemsDrop(Window* _window); 
void updateItemsDrop(Window* _window); 
void displayItemsDrop(Window* _window);
void createItemsDrop(ITEM_Type _type, sfVector2f _pos); 
void clearItemsDrop(); 
stdList* getItemList();    