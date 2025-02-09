#include "fish_tank.h"
#include "game.h"
#include "player.h"
#include "collisionManager.h"
#include "math.h"
#include "controller.h"
#include "map.h"

sfRectangleShape* tank_Rectangle;
sfRectangleShape* tank_FishRectangle;
sfIntRect tank_Rect;
sfTexture* tank_Text;
sfBool tank_HasAnimStart;
sfVector2f tank_FishPos;
float tank_timer;
int tank_NbFish;

void tank_Init()
{
	tank_List = STD_LIST_CREATE(tank, 0);
	tank_Rectangle = sfRectangleShape_create();
	tank_FishRectangle = sfRectangleShape_create();
	sfRectangleShape_setSize(tank_Rectangle, vector2f(636.f, 598.f));
	sfRectangleShape_setSize(tank_FishRectangle, vector2f(64.f, 42.f));
	sfRectangleShape_setPosition(tank_Rectangle, vector2f(300.f, 332.f));
	tank_Text = GetSfTexture("assets_general");
	tank_NbFish = 0;

}

void tank_CreateFish(int _fish_type)
{
	tank tmp;
	tmp.wichfish = _fish_type;
	tmp.origin = vector2f(32.f, 21.f);
	tmp.timerAnim = 0.f;
	tmp.frameX = 0;
	switch (tmp.wichfish)
	{
	case 0:
		tmp.pos = vector2f(120.f, 198.f); //+120 en x et +198 en y 
		tmp.spawnPoint = vector2f(120.f, 198.f);//+120 en x et +198 en y 
		tmp.maxPos = vector2f(120.f, 398.f);//+120 en x et +398 en y 
		tmp.rotation = 90.f;
		tmp.toSpawn = sfFalse;
		break;
	case 1:
		tmp.pos = vector2f(170.f, 438.f); //+170 en x et +338 en y 
		tmp.spawnPoint = vector2f(170.f, 438.f); //+170 en x et +338 en y 
		tmp.maxPos = vector2f(470.f, 438.f); //+470 en x et +338 en y 
		tmp.rotation = 180.f;
		tmp.toSpawn = sfFalse;
		break;
	case 2:
		tmp.pos = vector2f(520.f, 198.f); //+520 en x et +198 en y 
		tmp.spawnPoint = vector2f(520.f, 198.f); //+520 en x et +198 en y 
		tmp.maxPos = vector2f(520.f, 398.f); //+520 en x et +398 en y 
		tmp.rotation = 90.f;
		tmp.toSpawn = sfFalse;
		break;
	}
	tank_List->push_back(&tank_List, &tmp);
}


void tank_Update(Window* _window)
{
	tank_timer += getDeltaTime();

	if (tank_HasAnimStart) plr_StopMoving();
	if (tank_HasAnimStart && tank_timer >= 0.75f)//&& tank_NbFish <=2)
	{
		tank_CreateFish(tank_NbFish);
		tank_NbFish++;
		tank_timer = 0.f;
	}
	for (int i = 0; i < tank_List->size(tank_List); i++)
	{
		tank* tmp = TANK_GETDATA; /// Slightly more optimized than having to call TANK_GETDATA each time, as this command might be a bit CPU-heavy for longer lists

		if (tmp->wichfish == 0)
		{
			if (tmp->pos.y >= tmp->maxPos.y)
				tmp->toSpawn = sfTrue;
			else if (tmp->pos.y <= tmp->spawnPoint.y)
				tmp->toSpawn = sfFalse;
			if (!tmp->toSpawn)
			{
				addVectorsf(&tmp->pos, multiplyVectorsrf(vector2f(0.f, 50.f), getDeltaTime()));
				tmp->rotation = -90;
			}
			else
			{
				addVectorsf(&tmp->pos, multiplyVectorsrf(vector2f(0.f, -50.f), getDeltaTime()));
				tmp->rotation = 90;
			}
		}
		if (tmp->wichfish == 1)
		{
			if (tmp->pos.x >= tmp->maxPos.x)
				tmp->toSpawn = sfTrue;
			else if (tmp->pos.x <= tmp->spawnPoint.x)
				tmp->toSpawn = sfFalse;
			if (!tmp->toSpawn)
			{
				addVectorsf(&tmp->pos, multiplyVectorsrf(vector2f(50.f, 0.f), getDeltaTime()));
				tmp->rotation = 180.f;
			}
			else
			{
				addVectorsf(&tmp->pos, multiplyVectorsrf(vector2f(-50.f, 0.f), getDeltaTime()));
				tmp->rotation -= tmp->rotation;
			}
		}
		if (tmp->wichfish == 2)
		{
			if (tmp->pos.y >= tmp->maxPos.y)
				tmp->toSpawn = sfTrue;
			else if (tmp->pos.y <= tmp->spawnPoint.y)
				tmp->toSpawn = sfFalse;
			if (!tmp->toSpawn)
			{
				addVectorsf(&tmp->pos, multiplyVectorsrf(vector2f(0.f, 50.f), getDeltaTime()));
				tmp->rotation = -90;
			}
			else
			{
				addVectorsf(&tmp->pos, multiplyVectorsrf(vector2f(0.f, -50.f), getDeltaTime()));
				tmp->rotation = 90;
			}
		}
	}
}

void tank_Display(Window* _window)
{
	for (int i = 0; i < map_GetPropsList()->size(map_GetPropsList()); i++) {
		LayerProps* tmp = STD_LIST_GETDATA(map_GetPropsList(), LayerProps, i);
		if (tmp->type == PROPS_TYPE_GENERAL && tmp->general == PROPS_GENERAL_POOL) {
			for (int i = 0; i < tank_List->size(tank_List); i++) {
				sfRectangleShape_setOrigin(tank_FishRectangle, TANK_GETDATA->origin);
				sfRectangleShape_setPosition(tank_FishRectangle, addVectorsrf(tmp->position, addVectorsrf(TANK_GETDATA->pos, vector2f(0.f, 0.f))));
				sfRectangleShape_setRotation(tank_FishRectangle, TANK_GETDATA->rotation);
				sfRectangleShape_setTexture(tank_FishRectangle, tank_Text, sfFalse);
				sfRectangleShape_setTextureRect(tank_FishRectangle, animatorWithResize("general_fish", &TANK_GETDATA->frameX, &TANK_GETDATA->timerAnim, 0, tank_Rectangle));
				sfRenderTexture_drawRectangleShape(_window->rdrt, tank_FishRectangle, NULL);
			}
		}
	}
}

void tank_SetReleaseStatus(sfBool _status) {
	tank_HasAnimStart = _status;
	if (_status) tank_timer = 0.f;
}