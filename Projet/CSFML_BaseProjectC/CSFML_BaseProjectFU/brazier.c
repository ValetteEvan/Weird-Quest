#include "brazier.h"
#include "game.h"
#include "player.h"
#include "collisionManager.h"
#include "viewManager.h"
#include "map.h"
#include "particlesManager.h"

sfSprite* brz_Sprite;

void brz_TextureOnLoad()
{
	sfSprite_setTexture(brz_Sprite, GetSfTexture("assets_dungeon_biome"), sfTrue);
}

void brz_Init()
{
	brz_List = STD_LIST_CREATE(BrazierData, 0);

	brz_Sprite = sfSprite_create();

	brz_CreateBrazier(vector2f(500.f, 30.f));
}

void brz_CreateBrazier(sfVector2f _pos)
{
	if (brz_List == NULL) return;

	BrazierData tmp;
	tmp.pos = _pos;
	tmp.timeranim = 0.f;
	tmp.timerprt = 0.f;
	tmp.frameX = 0;
	tmp.isOnScreen = sfTrue;

	brz_List->push_back(&brz_List, &tmp);
	sfSprite_setTexture(brz_Sprite, GetSfTexture("assets_dungeon_biome"), sfTrue);
}

void brz_Update()
{
	for (int i = 0; i < brz_List->size(brz_List); i++) 
	{
		BRZ_GETDATA->timerprt += getDeltaTime();
		BRZ_GETDATA->timeranim += getDeltaTime();
		if (BRZ_GETDATA->timerprt > 0.3f)
		{
			prt_CreateFireParticlesBrazier(vector2f(BRZ_GETDATA->pos.x + 32.f, BRZ_GETDATA->pos.y + 20.f));
			BRZ_GETDATA->timerprt = 0.f;
		}
	}
}

void brz_Display(Window* _window)
{
	for (int i = 0; i < brz_List->size(brz_List); i++)
	{
		BrazierData* tmp = STD_LIST_GETDATA(brz_List, BrazierData, i);

		if (BRZ_GETDATA->pos.x < mainView->viewPos.x - (mainView->size.x / 2.f) - 64.f || BRZ_GETDATA->pos.x > mainView->viewPos.x + (mainView->size.x / 2.f) + 64.f ||
		BRZ_GETDATA->pos.y < mainView->viewPos.y - (mainView->size.y / 2.f) - 64.f || BRZ_GETDATA->pos.y > mainView->viewPos.y + (mainView->size.y / 2.f) + 64.f) 
		{
			BRZ_GETDATA->isOnScreen = sfFalse;
			continue;
		}
		else if (!BRZ_GETDATA->isOnScreen)
			BRZ_GETDATA->isOnScreen = sfTrue;
			
		sfSprite_setPosition(brz_Sprite, BRZ_GETDATA->pos);
		sfSprite_setTextureRect(brz_Sprite, animatorAnim("dungeonPlcH_Brazier", &BRZ_GETDATA->frameX, &BRZ_GETDATA->timeranim, 0));
		sfRenderTexture_drawSprite(_window->rdrt, brz_Sprite, NULL);
	}
}

void brz_Clear() 
{
	if (brz_List == NULL) return;
	brz_List->clear(&brz_List);
}