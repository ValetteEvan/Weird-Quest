/// Function prefix: brz_

#pragma once
#include "tools.h"
#include "windowManager.h"
#include "List.h"

#define BRZ_GETDATA STD_LIST_GETDATA(brz_List, BrazierData, i)

typedef struct {
	sfVector2f pos;
	int frameX;
	float timerprt;
	float timeranim;
	sfBool isOnScreen;
} BrazierData;

stdList* brz_List;

void brz_TextureOnLoad();

void brz_Init();

void brz_CreateBrazier(sfVector2f _pos);

void brz_Update();

void brz_Display(Window* _window);

void brz_Clear();
