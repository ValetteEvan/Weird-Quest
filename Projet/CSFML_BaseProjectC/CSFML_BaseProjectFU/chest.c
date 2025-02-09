#include "chest.h"
#include "langManager.h"
#include "pause.h"
#include "player.h"
#include "game_HUD.h"
#include "textureManager.h"
#include "viewManager.h"
#include "map.h"
#include "controller.h"
#include "game_HUD.h"
#include "particlesManager.h"

sfRectangleShape* chest_Shape;
sfTexture* chest_texture;
sfIntRect chest_rect;

void chest_loadTexture()
{
	chest_texture = GetSfTexture("dungeonPlcH_Press_plate");
}

void chest_InitChest()
{
	chest_ListChest = stdList_Create(sizeof(Chest_ChestList), 0);
	chest_Shape = sfRectangleShape_create();
	chest_loadTexture();
}

void chest_AddChest(ChestType _type, sfVector2f _pos)
{
	Chest_ChestList tmp;
	tmp.type = _type;
	tmp.pos = _pos;
	tmp.frameX = 0;
	tmp.timerAnim = 0.f;
	tmp.isOpen = sfFalse;
	tmp.isOpenAnimDone = sfFalse;
	switch (getCurrentBiome())
	{
		case BIOME_JOY_DUNGEON:
			if (tmp.type == CHEST_LITTLE)
				tmp.item = CHEST_ITEM_KEY_DUNGEON_JOY;
			else 
				tmp.item = CHEST_ITEM_RING_JOY;
				break;
		case BIOME_SADNESS_DUNGEON:
			if (tmp.type == CHEST_LITTLE)
				tmp.item = CHEST_ITEM_KEY_DUNGEON_SADNESS;
			else
				tmp.item = CHEST_ITEM_RING_SADNESS;
			break;
		case BIOME_FEAR_DUNGEON:
			if (tmp.type == CHEST_LITTLE)
				tmp.item = CHEST_ITEM_KEY_DUNGEON_FEAR;
			else
				tmp.item = CHEST_ITEM_RING_FEAR;
			break;
		case BIOME_ANGRY_DUNGEON:
			if (tmp.type == CHEST_LITTLE)
				tmp.item = CHEST_ITEM_KEY_DUNGEON_ANGER;
			else
				tmp.item = CHEST_ITEM_RING_ANGER;
			break;
		default: return;
	}
	chest_ListChest->push_back(&chest_ListChest, &tmp);
}

void chest_UpdateChest(Window* _window)
{
	for (int i = 0; i < chest_ListChest->size(chest_ListChest); i++)
	{
		sfFloatRect chestRect = FlRect(CHEST_DATA->pos.x - 20.f, CHEST_DATA->pos.y - 20.f, 128.f + 40.f, 128.f + 40.f);
		if (sfFloatRect_intersects(getPointerPlayerBounds(), &chestRect, NULL))
			plr_toggleFX(sfTrue);
		
		if (Gamepad_isButtonPressed(0, A, _window->renderWindow))
		{
			sfFloatRect chestRect = FlRect(CHEST_DATA->pos.x - 20.f, CHEST_DATA->pos.y - 20.f, 128.f + 40.f, 128.f + 40.f);
			if (sfFloatRect_intersects(getPointerPlayerBounds(), &chestRect, NULL))
			{
				
				if (CHEST_DATA->isOpen == sfFalse)
				{
					CHEST_DATA->isOpen = sfTrue;
					//pause_AddItem(CHEST_DATA->type);
					switch(CHEST_DATA->item)
					{
						case CHEST_ITEM_KEY_DUNGEON_JOY:
							HUD_addKey();
							playSoundFX("SFX_pick_item");
							prt_CreateGainParticles(vector2f(CHEST_DATA->pos.x + 32.f, CHEST_DATA->pos.y + 32.f));
							break;
						case CHEST_ITEM_KEY_DUNGEON_SADNESS:
							HUD_addKey();
							playSoundFX("SFX_pick_item");
							prt_CreateGainParticles(vector2f(CHEST_DATA->pos.x + 32.f, CHEST_DATA->pos.y + 32.f));
							break;
						case CHEST_ITEM_KEY_DUNGEON_FEAR:
							HUD_addKey(); 
							playSoundFX("SFX_pick_item");
							prt_CreateGainParticles(vector2f(CHEST_DATA->pos.x + 32.f, CHEST_DATA->pos.y + 32.f));
							break;
						case CHEST_ITEM_KEY_DUNGEON_ANGER:
							HUD_addKey();
							playSoundFX("SFX_pick_item");
							prt_CreateGainParticles(vector2f(CHEST_DATA->pos.x + 32.f, CHEST_DATA->pos.y + 32.f));
							break;
						case CHEST_ITEM_RING_JOY:
							plr_SetUnlockState(PLRUNL_ATTRACT);
							HUD_toggleAddedDataAppered(sfTrue);
							HUD_typeOfAddedData = HUD_INVENTORY;
							break;
						case CHEST_ITEM_RING_SADNESS:
							plr_SetUnlockState(PLRUNL_REPULSE);
							HUD_toggleAddedDataAppered(sfTrue);
							HUD_typeOfAddedData = HUD_INVENTORY;
							break;
						case CHEST_ITEM_RING_FEAR:
							plr_SetUnlockState(PLRUNL_STASIS);
							HUD_toggleAddedDataAppered(sfTrue);
							HUD_typeOfAddedData = HUD_INVENTORY;
							break;
						case CHEST_ITEM_RING_ANGER:
							plr_SetUnlockState(PLRUNL_DESTRUCT);
							HUD_toggleAddedDataAppered(sfTrue);
							HUD_typeOfAddedData = HUD_INVENTORY;
							break;
						default: return;
					}
				}
			}
		}
	}
}

void chest_DisplayChest(Window* _window)
{
	if (getIsFreeingMap()) return;

	for (int i = 0; i < chest_ListChest->size(chest_ListChest); i++)
	{
		sfRectangleShape_setPosition(chest_Shape, CHEST_DATA->pos);
		if (CHEST_DATA->type == CHEST_BIG && !CHEST_DATA->isOpen)
		{
			if (CHEST_DATA->pos.x < mainView->viewPos.x - (mainView->size.x / 2.f) - 64.f || CHEST_DATA->pos.x > mainView->viewPos.x + (mainView->size.x / 2.f) + 64.f || CHEST_DATA->pos.y < mainView->viewPos.y - (mainView->size.y / 2.f) - 64.f || CHEST_DATA->pos.y > mainView->viewPos.y + (mainView->size.y / 2.f) + 64.f)
				continue;
			sfRectangleShape_setSize(chest_Shape, vector2f(128.f, 128.f));
			sfRectangleShape_setTexture(chest_Shape, chest_texture, sfFalse);
			sfRectangleShape_setTextureRect(chest_Shape, IntRect(0, 704, 128, 128));
		}
		else if (CHEST_DATA->type == CHEST_LITTLE && !CHEST_DATA->isOpen)
		{
			sfRectangleShape_setSize(chest_Shape, vector2f(64.f, 64.f));
			sfRectangleShape_setTexture(chest_Shape, chest_texture, sfFalse);
			sfRectangleShape_setTextureRect(chest_Shape, IntRect(0, 640, 64, 64));
		}
		else if (CHEST_DATA->isOpen && !CHEST_DATA->isOpenAnimDone)
		{
			if (CHEST_DATA->type == CHEST_BIG)
			{
				sfRectangleShape_setSize(chest_Shape, vector2f(128.f, 128.f));
				sfRectangleShape_setTexture(chest_Shape, chest_texture, sfFalse);
				sfRectangleShape_setTextureRect(chest_Shape, animatorWithResize("dungeonPlcH_bigChest", &CHEST_DATA->frameX, &CHEST_DATA->timerAnim, 0, chest_Shape));
			}
			else
			{
				sfRectangleShape_setSize(chest_Shape, vector2f(64.f, 64.f));
				sfRectangleShape_setTexture(chest_Shape, chest_texture, sfFalse);
				sfRectangleShape_setTextureRect(chest_Shape, animatorWithResize("dungeonPlcH_litlChest", &CHEST_DATA->frameX, &CHEST_DATA->timerAnim, 0, chest_Shape));
			}
			if (CHEST_DATA->frameX == 3)
				CHEST_DATA->isOpenAnimDone = sfTrue;
		}
		else if (CHEST_DATA->isOpenAnimDone)
		{
			if (CHEST_DATA->type == CHEST_BIG)
			{
				sfRectangleShape_setSize(chest_Shape, vector2f(128.f, 128.f));
				sfRectangleShape_setTexture(chest_Shape, chest_texture, sfFalse);
				sfRectangleShape_setTextureRect(chest_Shape, IntRect(3 * 128, 704, 128, 128));
			}
			else
			{
				sfRectangleShape_setTexture(chest_Shape, chest_texture, sfFalse);
				sfRectangleShape_setTextureRect(chest_Shape, IntRect(3 * 64, 640, 64, 64));
			}
		}
		sfRenderTexture_drawRectangleShape(_window->rdrt, chest_Shape, NULL);
	}
}

void chest_Deinit() {
	if (!chest_Shape) return;
	sfRectangleShape_destroy(chest_Shape);
}

void chest_clearChestList()
{
	if (!chest_ListChest) return;
	chest_ListChest->clear(&chest_ListChest);
}
