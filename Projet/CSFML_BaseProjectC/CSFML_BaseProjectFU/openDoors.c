#include "math.h"
#include "openDoors.h"
#include "map.h"
#include "enemies.h"
#include "game_HUD.h"
#include "bossJoie.h"
#include "controller.h"
#include "player.h"
#define DOOR_DATA STD_LIST_GETDATA(opDoors_listDoors, opDoors_DoorsList, i)

sfRectangleShape* opDoors_Shape;
sfTexture* opDoors_Texture_dungeon;
sfTexture* opDoors_Texture_joy;
sfBool opDoors_onePass;

void opDoors_loadTexture()
{
	opDoors_Texture_dungeon = GetSfTexture("assets_dungeon");
	if (getCurrentBiome() != BIOME_ALL) 
		opDoors_Texture_joy = GetSfTexture("assets_dungeon_biome"); 
}

void opDoors_InitDoor(Window* _window)
{
	opDoors_listDoors = stdList_Create(sizeof(opDoors_DoorsList), 0);
	opDoors_Shape = sfRectangleShape_create();
	opDoors_onePass = sfFalse;
	opDoors_loadTexture();
}

void opDoors_UpdateDoor(Window* _window)
{
	float dt = getDeltaTime();
	static float timerInput = 0.f;
	timerInput += dt;
	if (!opDoors_onePass && getState() == GAME)
	{
		for (int i = 0; i < opDoors_listDoors->size(opDoors_listDoors); i++)
		{
			opDoors_DoorsList* doorOrientation = STD_LIST_GETDATA(opDoors_listDoors, opDoors_DoorsList, i);
			if (doorOrientation->type == DOOR_TOP)
			{
				for (int c = 0; c < opDoors_listDoors->size(opDoors_listDoors); c++)
				{
					opDoors_DoorsList* doorCheck = STD_LIST_GETDATA(opDoors_listDoors, opDoors_DoorsList, c);
					if (doorOrientation->position.x < doorCheck->position.x)
					{
						doorOrientation->doorOrientation = DOOR_AXE_LEFT;
						break;
					}
					else if(doorOrientation->position.x > doorCheck->position.x)
					{
						doorOrientation->doorOrientation = DOOR_AXE_RIGHT;
						break;
					}
				}
			}
			else if (doorOrientation->type == DOOR)
			{
				for (int c = 0; c < opDoors_listDoors->size(opDoors_listDoors); c++)
				{
					opDoors_DoorsList* doorCheck = STD_LIST_GETDATA(opDoors_listDoors, opDoors_DoorsList, c);
					if (doorOrientation->position.y < doorCheck->position.y)
					{
						doorOrientation->doorOrientation = DOOR_AXE_TOP;
						break;
					}
					else if (doorOrientation->position.y > doorCheck->position.y)
					{
						doorOrientation->doorOrientation = DOOR_AXE_DOWN;
						break;
					}
				}
			}
		}
		opDoors_onePass = sfTrue;
	}

	if (map_GetState() == MAP_STATE_DUNGEON_JOY_1) 
	{
		if (sfKeyboard_isKeyPressed(sfKeyNumpad7) && DEBUG_STATE) {
			HUD_addKey();
		}
		if (getEnAmount() <= 0)
		{
			for (int i = 0; i < opDoors_listDoors->size(opDoors_listDoors); i++)
			{
				opDoors_DoorsList* tmp = STD_LIST_GETDATA(opDoors_listDoors, opDoors_DoorsList, i);
				if (tmp->type == DOOR_TOP && tmp->doorOrientation == DOOR_AXE_LEFT)
				{
					removeCollision(vector2i((tmp->position.x) / 64, (tmp->position.y+128) / 64));
					removeCollision(vector2i((tmp->position.x) / 64, (tmp->position.y+192) / 64));
					tmp->isOpen = sfTrue;
				}
				else if (tmp->type == DOOR_BOSS)
				{
					if (getNbJoyKey() >= 2 && getSqrMagnitudeBetween2Vectorsf(vector2f(tmp->position.x + 192.f, tmp->position.y + 64.f), plr_GetPos()) <= 144.f*144.f && Gamepad_isButtonPressedNF(0, A)) 
					{
						for (int i = 0; i < 2; i++)
						{
							tmp->position.y += 64.f;
						}
						tmp->isOpen = sfTrue;
						if (tmp->isOpen)
						{
							opDoors_listDoors->erase(&opDoors_listDoors, i);
							for (int p = 0; p < opDoors_listDoors->size(opDoors_listDoors); p++)
							{
								opDoors_DoorsList* tmpDoorBossTop = STD_LIST_GETDATA(opDoors_listDoors, opDoors_DoorsList, p);
								if (tmpDoorBossTop->type == DOOR_BOSS_TOP)
								{
									removeCollision(vector2i((tmpDoorBossTop->position.x) / 64, (tmpDoorBossTop->position.y + 128) / 64));
									removeCollision(vector2i((tmpDoorBossTop->position.x + 64) / 64, (tmpDoorBossTop->position.y + 128) / 64));
									removeCollision(vector2i((tmpDoorBossTop->position.x + 128) / 64, (tmpDoorBossTop->position.y + 128) / 64));
									removeCollision(vector2i((tmpDoorBossTop->position.x + 192) / 64, (tmpDoorBossTop->position.y + 128) / 64));
									tmpDoorBossTop->position.y += 128.f;
								}
							}
							break;
						}
					}
				}
				else if (tmp->type == DOOR && tmp->doorOrientation == DOOR_AXE_DOWN)
				{
					if (entity_BossJoie.isDead)
					{
						tmp->isOpen = sfTrue;
						removeCollision(vector2i((tmp->position.x) / 64, (tmp->position.y) / 64));
						removeCollision(vector2i((tmp->position.x + 64) / 64, (tmp->position.y) / 64));
						removeCollision(vector2i((tmp->position.x + 128) / 64, (tmp->position.y) / 64));
						removeCollision(vector2i((tmp->position.x + 192) / 64, (tmp->position.y) / 64));
					}
				}
			}
		}
	}
	if (map_GetState() == MAP_STATE_DUNGEON_JOY_2) 
	{
		if (sfKeyboard_isKeyPressed(sfKeyF2)) {
			HUD_addKey();
		}
		if (getNbJoyKey() >= 1)
		{
			for (int i = 0; i < opDoors_listDoors->size(opDoors_listDoors); i++)
			{
				opDoors_DoorsList* tmp = STD_LIST_GETDATA(opDoors_listDoors, opDoors_DoorsList, i); 
				if (tmp->type == DOOR || tmp->type == DOOR_TOP) 
				{
					tmp->isOpen = sfTrue;
					if (tmp->doorOrientation == DOOR_AXE_RIGHT) {
						removeCollision(vector2i((tmp->position.x) / 64, (tmp->position.y + 128) / 64));
						removeCollision(vector2i((tmp->position.x) / 64, (tmp->position.y + 192) / 64));
					}
					else if (tmp->doorOrientation == DOOR_AXE_TOP) {
						removeCollision(vector2i((tmp->position.x) / 64, (tmp->position.y + 128) / 64));
						removeCollision(vector2i((tmp->position.x+64) / 64, (tmp->position.y + 128) / 64));
						removeCollision(vector2i((tmp->position.x+128) / 64, (tmp->position.y + 128) / 64));
						removeCollision(vector2i((tmp->position.x+192) / 64, (tmp->position.y + 128) / 64)); 
					}
				}
			}
		}
	}
	if (map_GetState() == MAP_STATE_DUNGEON_JOY_3)
	{
		if (getEnAmount() <= 0 || customKeyboardIsKeyPressed(sfKeyF8, _window->renderWindow))
		{
			for (int i = 0; i < opDoors_listDoors->size(opDoors_listDoors); i++)
			{
				opDoors_DoorsList* tmp = STD_LIST_GETDATA(opDoors_listDoors, opDoors_DoorsList, i);
				if (tmp->type == DOOR_TOP && tmp->doorOrientation == DOOR_AXE_RIGHT) {
					tmp->isOpen = sfTrue;
					removeCollision(vector2i((tmp->position.x) / 64, (tmp->position.y + 128) / 64));
					removeCollision(vector2i((tmp->position.x) / 64, (tmp->position.y + 192) / 64));
				}
				else if (tmp->type == DOOR && tmp->doorOrientation == DOOR_AXE_DOWN) {
					tmp->isOpen = sfTrue;
					removeCollision(vector2i((tmp->position.x) / 64, (tmp->position.y) / 64));
					removeCollision(vector2i((tmp->position.x + 64) / 64, (tmp->position.y) / 64));
					removeCollision(vector2i((tmp->position.x + 128) / 64, (tmp->position.y) / 64));
					removeCollision(vector2i((tmp->position.x + 192) / 64, (tmp->position.y) / 64));
				}
				else if (getNbJoyKey() >= 2 && tmp->type == DOOR && tmp->doorOrientation == DOOR_AXE_TOP)
				{
					tmp->isOpen = sfTrue;
					removeCollision(vector2i((tmp->position.x) / 64, (tmp->position.y + 128) / 64));
					removeCollision(vector2i((tmp->position.x + 64) / 64, (tmp->position.y + 128) / 64));
					removeCollision(vector2i((tmp->position.x + 128) / 64, (tmp->position.y + 128) / 64));
					removeCollision(vector2i((tmp->position.x + 192) / 64, (tmp->position.y + 128) / 64));
				}
			}
		}
	}
	if (map_GetState() == MAP_STATE_DUNGEON_JOY_4)
	{
		for (int i = 0; i < opDoors_listDoors->size(opDoors_listDoors); i++)
		{
			opDoors_DoorsList* tmp = STD_LIST_GETDATA(opDoors_listDoors, opDoors_DoorsList, i);
			if (tmp->type == DOOR_TOP && tmp->doorOrientation == DOOR_AXE_LEFT) {
				tmp->isOpen = sfTrue;
				removeCollision(vector2i((tmp->position.x) / 64, (tmp->position.y + 128) / 64));
				removeCollision(vector2i((tmp->position.x) / 64, (tmp->position.y + 192) / 64));
			}
			else if (tmp->type == DOOR && tmp->doorOrientation == DOOR_AXE_TOP) {
				tmp->isOpen = sfTrue;
				removeCollision(vector2i((tmp->position.x) / 64, (tmp->position.y + 128) / 64));
				removeCollision(vector2i((tmp->position.x + 64) / 64, (tmp->position.y + 128) / 64));
				removeCollision(vector2i((tmp->position.x + 128) / 64, (tmp->position.y + 128) / 64));
				removeCollision(vector2i((tmp->position.x + 192) / 64, (tmp->position.y + 128) / 64));
			}
		}
	}
	if (map_GetState() == MAP_STATE_DUNGEON_JOY_5)
	{
		for (int i = 0; i < opDoors_listDoors->size(opDoors_listDoors); i++)
		{
			opDoors_DoorsList* tmp = STD_LIST_GETDATA(opDoors_listDoors, opDoors_DoorsList, i);
			if (tmp->type == DOOR_TOP && tmp->doorOrientation == DOOR_AXE_LEFT) {
				tmp->isOpen = sfTrue;
				removeCollision(vector2i((tmp->position.x) / 64, (tmp->position.y + 128) / 64));
				removeCollision(vector2i((tmp->position.x) / 64, (tmp->position.y + 192) / 64));
			}
			else if (tmp->type == DOOR && tmp->doorOrientation == DOOR_AXE_DOWN) {
				tmp->isOpen = sfTrue;
				removeCollision(vector2i((tmp->position.x) / 64, (tmp->position.y) / 64));
				removeCollision(vector2i((tmp->position.x + 64) / 64, (tmp->position.y) / 64));
				removeCollision(vector2i((tmp->position.x + 128) / 64, (tmp->position.y) / 64));
				removeCollision(vector2i((tmp->position.x + 192) / 64, (tmp->position.y) / 64));
			}
		}
	}
	if (map_GetState() == MAP_STATE_DUNGEON_JOY_6)
	{
		for (int i = 0; i < opDoors_listDoors->size(opDoors_listDoors); i++)
		{
			opDoors_DoorsList* tmp = STD_LIST_GETDATA(opDoors_listDoors, opDoors_DoorsList, i);
			if (tmp->type == DOOR_TOP && tmp->doorOrientation == DOOR_AXE_RIGHT) {
				tmp->isOpen = sfTrue;
				removeCollision(vector2i((tmp->position.x) / 64, (tmp->position.y + 128) / 64));
				removeCollision(vector2i((tmp->position.x) / 64, (tmp->position.y + 192) / 64));
			}
			else if (getNbJoyKey() >= 2 && tmp->type == DOOR && tmp->doorOrientation == DOOR_AXE_DOWN) {
				tmp->isOpen = sfTrue;
				removeCollision(vector2i((tmp->position.x) / 64, (tmp->position.y) / 64));
				removeCollision(vector2i((tmp->position.x + 64) / 64, (tmp->position.y) / 64));
				removeCollision(vector2i((tmp->position.x + 128) / 64, (tmp->position.y) / 64));
				removeCollision(vector2i((tmp->position.x + 192) / 64, (tmp->position.y) / 64));
			}
		}
	}
	if (map_GetState() == MAP_STATE_DUNGEON_JOY_BOSS)
	{
		if (entity_BossJoie.isDead)
		{
			for (int i = 0; i < opDoors_listDoors->size(opDoors_listDoors); i++)
			{
				opDoors_DoorsList* tmp = STD_LIST_GETDATA(opDoors_listDoors, opDoors_DoorsList, i);
				if (tmp->type == DOOR && tmp->doorOrientation == DOOR_AXE_TOP) {
					tmp->isOpen = sfTrue;
					removeCollision(vector2i((tmp->position.x) / 64, (tmp->position.y + 128) / 64));
					removeCollision(vector2i((tmp->position.x + 64) / 64, (tmp->position.y + 128) / 64));
					removeCollision(vector2i((tmp->position.x + 128) / 64, (tmp->position.y + 128) / 64));
					removeCollision(vector2i((tmp->position.x + 192) / 64, (tmp->position.y + 128) / 64));
				}
				else if (tmp->type == DOOR && tmp->doorOrientation == DOOR_AXE_DOWN) {
					tmp->isOpen = sfTrue;
					removeCollision(vector2i((tmp->position.x) / 64, (tmp->position.y) / 64));
					removeCollision(vector2i((tmp->position.x + 64) / 64, (tmp->position.y) / 64));
					removeCollision(vector2i((tmp->position.x + 128) / 64, (tmp->position.y) / 64));
					removeCollision(vector2i((tmp->position.x + 192) / 64, (tmp->position.y) / 64));
				}
			}
		}
	}

	if (customKeyboardIsKeyPressed(sfKeyF8, _window->renderWindow) && timerInput > 0.2f)
	{ 
		for (int i = 0; i < opDoors_listDoors->size(opDoors_listDoors); i++)
		{
			opDoors_DoorsList* tmp = STD_LIST_GETDATA(opDoors_listDoors, opDoors_DoorsList, i);
			if (tmp->type == DOOR_BOSS)
			{
				for (int i = 0; i < 2; i++)
				{
					tmp->position.y += 64.f;
				}
				tmp->isOpen = sfTrue;
				if (tmp->isOpen)
				{
					opDoors_listDoors->erase(&opDoors_listDoors, i);
					for (int p = 0; p < opDoors_listDoors->size(opDoors_listDoors); p++)
					{
						opDoors_DoorsList* tmpDoorBossTop = STD_LIST_GETDATA(opDoors_listDoors, opDoors_DoorsList, p);
						if (tmpDoorBossTop->type == DOOR_BOSS_TOP)
						{
							tmpDoorBossTop->position.y += 128.f;
						}
					}
					break;
				}
			}
		}
		timerInput = 0.f;
	}
	if (customKeyboardIsKeyPressed(sfKeyF6, _window->renderWindow) && timerInput > 0.2f)
	{
		for (int i = 0; i < opDoors_listDoors->size(opDoors_listDoors); i++)
		{
			opDoors_DoorsList* tmp = STD_LIST_GETDATA(opDoors_listDoors, opDoors_DoorsList, i);
			if (tmp->type == DOOR && tmp->doorOrientation == DOOR_AXE_DOWN)
			{
				tmp->isOpen = sfTrue;
			}
		}
		timerInput = 0.f;
	}
	if (customKeyboardIsKeyPressed(sfKeyF7, _window->renderWindow) && timerInput > 0.2f)
	{
		for (int i = 0; i < opDoors_listDoors->size(opDoors_listDoors); i++)
		{
			opDoors_DoorsList* tmp = STD_LIST_GETDATA(opDoors_listDoors, opDoors_DoorsList, i);
			if (tmp->type == DOOR && tmp->doorOrientation == DOOR_AXE_TOP)
			{
				tmp->isOpen = sfTrue;
			}
		}
		timerInput = 0.f;
	}
	if (customKeyboardIsKeyPressed(sfKeyF9, _window->renderWindow) && timerInput > 0.2f)
	{
		for (int i = 0; i < opDoors_listDoors->size(opDoors_listDoors); i++)
		{
			opDoors_DoorsList* tmp = STD_LIST_GETDATA(opDoors_listDoors, opDoors_DoorsList, i);
			if (tmp->type == DOOR_TOP && tmp->doorOrientation == DOOR_AXE_RIGHT)
			{
				tmp->isOpen = sfTrue;
			}
		}
		timerInput = 0.f;
	}
	if (customKeyboardIsKeyPressed(sfKeyF10, _window->renderWindow) && timerInput > 0.2f)
	{
		for (int i = 0; i < opDoors_listDoors->size(opDoors_listDoors); i++)
		{
			opDoors_DoorsList* tmp = STD_LIST_GETDATA(opDoors_listDoors, opDoors_DoorsList, i);
			if (tmp->type == DOOR_TOP && tmp->doorOrientation == DOOR_AXE_LEFT)
			{
				tmp->isOpen = sfTrue;
			}
		}
		timerInput = 0.f;
	}
}

void opDoors_DisplayDoor(Window* _window)
{
	for (int i = 0; i < opDoors_listDoors->size(opDoors_listDoors); i++)
	{
		sfRectangleShape_setSize(opDoors_Shape, vector2f(64.f, 64.f));
		switch (DOOR_DATA->type)
		{
		case DOOR:	
			sfRectangleShape_setTexture(opDoors_Shape, opDoors_Texture_dungeon, sfFalse);
			if (!DOOR_DATA->isOpen)
			{
				if (DOOR_DATA->doorOrientation == DOOR_AXE_TOP)
				{
					sfRectangleShape_setTextureRect(opDoors_Shape, IntRect(448, 64, 64, 64));
					sfRectangleShape_setSize(opDoors_Shape, vector2f(64.f, 64.f));
					for (int d = 4; d > 0; d--)
					{
						sfRectangleShape_setPosition(opDoors_Shape, addVectorsrf(DOOR_DATA->position, vector2f(64.f * (d - 1), 64.f)));
						sfRenderTexture_drawRectangleShape(_window->rdrt, opDoors_Shape, NULL);
					}
					for (int d2 = 4; d2 > 0; d2--)
					{
						sfRectangleShape_setPosition(opDoors_Shape, addVectorsrf(DOOR_DATA->position, vector2f(64.f * (d2 - 1), 128.f)));
						sfRenderTexture_drawRectangleShape(_window->rdrt, opDoors_Shape, NULL);
					}
					sfRectangleShape_setTextureRect(opDoors_Shape, IntRect(448, 0, 64, 64));
					for (int d3 = 4; d3 > 0; d3--)
					{
						sfRectangleShape_setPosition(opDoors_Shape, addVectorsrf(DOOR_DATA->position, vector2f(64.f * (d3 - 1), 0.f)));
						sfRenderTexture_drawRectangleShape(_window->rdrt, opDoors_Shape, NULL);
					}
				}
				else if (DOOR_DATA->doorOrientation == DOOR_AXE_DOWN)
				{
					sfRectangleShape_setTextureRect(opDoors_Shape, IntRect(448, 0, 64, 64));
					for (int d3 = 4; d3 > 0; d3--)
					{
						sfRectangleShape_setPosition(opDoors_Shape, addVectorsrf(DOOR_DATA->position, vector2f(64.f * (d3 - 1), 0.f)));
						sfRenderTexture_drawRectangleShape(_window->rdrt, opDoors_Shape, NULL);
					}
				}
			}
			else
			{
				if (DOOR_DATA->doorOrientation == DOOR_AXE_TOP)
				{
					sfRectangleShape_setTextureRect(opDoors_Shape, IntRect(448, 0, 64, 64));
					sfRectangleShape_setSize(opDoors_Shape, vector2f(64.f, 64.f));
					for (int d = 4; d > 0; d--)
					{
						sfRectangleShape_setPosition(opDoors_Shape, addVectorsrf(DOOR_DATA->position, vector2f(64.f * (d - 1), 128.f)));
						sfRenderTexture_drawRectangleShape(_window->rdrt, opDoors_Shape, NULL);
					}
				}
			}
			break;
		case DOOR_TOP:
			sfRectangleShape_setTexture(opDoors_Shape, opDoors_Texture_dungeon, sfFalse);
			sfRectangleShape_setTextureRect(opDoors_Shape, IntRect(448, 0, 64, 64));
			if (!DOOR_DATA->isOpen)
			{
				for (int d = 4; d > 0; d--)
				{
					sfVector2f tmpPos = addVectorsrf(DOOR_DATA->position, vector2f(0.f, 192.f - 64.f * (d - 1)));
					sfRectangleShape_setPosition(opDoors_Shape, tmpPos);
					sfRectangleShape_setSize(opDoors_Shape, vector2f(64.f, 64.f));
					sfRenderTexture_drawRectangleShape(_window->rdrt, opDoors_Shape, NULL);
				}
			}
			else
			{
				for (int d = 2; d > 0; d--)
				{
					sfVector2f tmpPos = addVectorsrf(DOOR_DATA->position, vector2f(0.f, 192.f - 64.f * (d - 1)));
					sfRectangleShape_setPosition(opDoors_Shape, tmpPos);
					sfRectangleShape_setSize(opDoors_Shape, vector2f(64.f, 64.f));
					sfRenderTexture_drawRectangleShape(_window->rdrt, opDoors_Shape, NULL);
				}
			}
			break;
		case DOOR_BOSS:
			sfRectangleShape_setTexture(opDoors_Shape, opDoors_Texture_joy, sfFalse);
			sfRectangleShape_setTextureRect(opDoors_Shape, IntRect(0, 320, 384, 128));
			sfRectangleShape_setPosition(opDoors_Shape, DOOR_DATA->position);
			sfRectangleShape_setSize(opDoors_Shape, vector2f(384.f, 128.f));
			sfRenderTexture_drawRectangleShape(_window->rdrt, opDoors_Shape, NULL);
			break;
		case DOOR_BOSS_TOP:
			sfRectangleShape_setTexture(opDoors_Shape, opDoors_Texture_joy, sfFalse);
			sfRectangleShape_setTextureRect(opDoors_Shape, IntRect(0, 256, 64, 64));
			sfRectangleShape_setPosition(opDoors_Shape, DOOR_DATA->position);
			sfRectangleShape_setSize(opDoors_Shape, vector2f(64.f, 64.f));
			sfRenderTexture_drawRectangleShape(_window->rdrt, opDoors_Shape, NULL);
			break;
		}
		
	}
}

void opDoors_DeinitDoor()
{
	sfRectangleShape_destroy(opDoors_Shape);
}

void opDoors_addDoors(PropsDoor _type, sfVector2f _pos)
{
	opDoors_DoorsList tmp;
	tmp.type = _type;
	tmp.position = _pos;
	tmp.doorOrientation = DOOR_AXE_NEUTRAL;
	tmp.isOpen = sfFalse;
	tmp.timer = 0.f;
	
	opDoors_listDoors->push_back(&opDoors_listDoors, &tmp);
}

void opDoors_delDoors(int _index)
{
	opDoors_listDoors->erase(&opDoors_listDoors, _index);
}

void opDoors_clearDoorsList()
{
	if (!opDoors_listDoors) return;
	opDoors_listDoors->clear(&opDoors_listDoors);
}

void setOnePassOPDoors()
{
	opDoors_onePass = sfFalse;
}
