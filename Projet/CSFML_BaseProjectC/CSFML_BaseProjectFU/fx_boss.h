#pragma once
#include "stateManager.h"
#include "textureManager.h"
#include "List.h"
#include "math.h"
#include "tools.h"

stdList* fxList;
stdList* fxList2; 

typedef enum {
	EXPLOSION_CENTER= 1,
	EXPLOSION_DOWN,
}FX_Type;

typedef struct {
	sfVector2f position;
	sfVector2f origin;
	sfVector2f size;
	sfTexture* texture;
	FX_Type type; 
	float timeToDie; 
	float timeAnim;
	int frameX;
}FX;

typedef enum {
	IMPACT,
}FX_Type2;

typedef struct {
	sfVector2f position;
	sfVector2f origin;
	sfVector2f size;
	sfTexture* texture;
	FX_Type2 type; 
	float timeToDie; 
	float timeAnim; 
	int frameX; 
}FX2; 
 
void fx_Init1(Window* _window); 
void fx_Init2(Window* _window); 

void fx_Update1(Window* _window); 
void fx_Update2(Window* _window);

void fx_DisplayLayer1(Window* _window);
void fx_DisplayLayer2(Window* _window);

void fx_Create1(FX_Type _type, sfVector2f _pos);
void fx_Create2(FX_Type2 _type, sfVector2f _pos);  

stdList* fx_GetList();
stdList* fx_GetList2(); 

void fx_Deinit();