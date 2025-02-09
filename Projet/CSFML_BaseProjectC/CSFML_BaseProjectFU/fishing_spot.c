#include "fishing_spot.h"
#include "tools.h"
#include "windowManager.h"
#include "textureManager.h"
#include "controller.h"
#include "List.h"
#include "fish.h"
#include "player.h"
#include "game.h"

stdList* fishspot_List;

sfRectangleShape* fishspot_RenderRect;
sfFloatRect fishspot_rect;
float fishspot_timer;
int fishspot_frame;

void fishspot_Init()
{
	fishspot_List = STD_LIST_CREATE(sfVector2f, 0);
	fishspot_RenderRect = sfRectangleShape_create();
	sfRectangleShape_setSize(fishspot_RenderRect, vector2f(252, 192));
	sfRectangleShape_setTexture(fishspot_RenderRect, GetSfTexture("assets_general"), sfTrue);
	fishspot_timer = 0.f;
	fishspot_frame = 0;
}

void fishspot_Update(Window* _window)
{
	for (int i = 0; i < fishspot_List->size(fishspot_List); i++)
	{
		sfVector2f* tmp = STD_LIST_GETDATA(fishspot_List, sfVector2f, i);
		sfRectangleShape_setPosition(fishspot_RenderRect, *tmp);
		fishspot_rect = sfRectangleShape_getGlobalBounds(fishspot_RenderRect);
		sfFloatRect temp_player_rect = plr_getPlayerBounds();
		if (sfFloatRect_intersects(&temp_player_rect, &fishspot_rect, NULL)) {
			plr_toggleFX(sfTrue);
		}
		if (sfFloatRect_intersects(&temp_player_rect, &fishspot_rect, NULL) && Gamepad_isButtonPressed(0, A, _window->renderWindow))
		{
			fish_IdFishingSpot = i;
			game_ChangeFromGameToFish();
		}
	}
}

void fishspot_Display(Window* _window)
{
	sfRectangleShape_setTextureRect(fishspot_RenderRect, animatorAnim("general_fishing_spot", &fishspot_frame, &fishspot_timer, 0));

	for (int i = 0; i < fishspot_List->size(fishspot_List); i++)
	{
		sfVector2f tmp = *STD_LIST_GETDATA(fishspot_List, sfVector2f, i);
		sfRectangleShape_setPosition(fishspot_RenderRect, tmp);
		sfRenderTexture_drawRectangleShape(_window->rdrt, fishspot_RenderRect, NULL);
	}
}

void fishspot_AddSplash(sfVector2f _pos)
{
	STD_LIST_PUSHBACK(fishspot_List, _pos);
}

void fishspot_Delete(int _index)
{
	if (_index < 0 || _index >= fishspot_List->size(fishspot_List)) return;
	fishspot_List->erase(&fishspot_List, _index);
}

void fishspot_Clear()
{
	if (!fishspot_List) return;
	fishspot_List->clear(&fishspot_List);
}

void fishspot_Deinit()
{
	fishspot_Clear();
	sfRectangleShape_destroy(fishspot_RenderRect);
}