#pragma once
#include "tools.h"
#include "windowManager.h"
#include "List.h"

typedef enum {
	FX_STARS,
	FX_EXPLOSION,
	FX_NULL,
}FXenemies_enum;

typedef struct {
	sfVector2f pos;
	float animTime;
	int frameX;
	int id;
}FXenemies_struct;

stdList* FXe_ennemiesList;

void FXe_initFXenemies();
void FXe_addFXenemies(sfVector2f _pos, FXenemies_enum _id);
void FXe_updateFXenemies();
void FXe_displayFXenemies(Window* _window);