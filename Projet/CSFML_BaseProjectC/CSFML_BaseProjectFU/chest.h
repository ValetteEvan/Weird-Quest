#pragma once

#include "tools.h"
#include "windowManager.h"
#include "List.h"

#define CHEST_DATA STD_LIST_GETDATA(chest_ListChest, Chest_ChestList, i)

typedef enum
{
	CHEST_LITTLE,
	CHEST_BIG
}ChestType;

typedef enum
{
	CHEST_ITEM_KEY_DUNGEON_JOY,
	CHEST_ITEM_KEY_DUNGEON_SADNESS,
	CHEST_ITEM_KEY_DUNGEON_FEAR,
	CHEST_ITEM_KEY_DUNGEON_ANGER,
	CHEST_ITEM_RING_JOY,
	CHEST_ITEM_RING_SADNESS,
	CHEST_ITEM_RING_FEAR,
	CHEST_ITEM_RING_ANGER
}ItemsInChest;

typedef struct 
{
	ChestType type;
	ItemsInChest item;
	sfVector2f pos;
	float timerAnim;
	int frameX;
	sfBool isOpen;
	sfBool isOpenAnimDone;
}Chest_ChestList;

stdList* chest_ListChest;

void chest_loadTexture();

void chest_InitChest();

void chest_AddChest(ChestType _type, sfVector2f _pos);

void chest_UpdateChest(Window* _window);

void chest_DisplayChest(Window* _window);

void chest_Deinit();

void chest_clearChestList();