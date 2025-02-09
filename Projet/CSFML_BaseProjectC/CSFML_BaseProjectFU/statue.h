#pragma once
#include "tools.h"
#include "ressourcesManager.h"
#include "textureManager.h"
#include "windowManager.h" 
#include "List.h" 
#include "map.h"
#include "editor.h"

stdList* stt_List;


typedef enum {
	STATUE_TYPE_1 = 0, 
}StatueType; 

typedef struct {
	sfVector2f pos;
	sfVector2f size; 
	sfColor color; 
	StatueType type;  
}Statue;

typedef struct {
	sfVector2f pos;
	StatueType type;
}StatueToSave;

void stt_Init(Window* _window);
void stt_Update(Window* _window);
void stt_Display(Window* _window);
void stt_DisplayMenu(Window* _window);
void stt_Create(StatueType _type, sfVector2f _pos);
sfFloatRect stt_GetBounds(Statue* _status);  
stdList* stt_GetList();  
Statue* stt_GetToSave();    
float stt_GetInteractCooldown();
void stt_SetInteractCooldown(float _cd);
void stt_clear();