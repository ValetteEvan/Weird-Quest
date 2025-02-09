#include "game_HUD.h"
#include "textureManager.h"
#include "fontManager.h"
#include "soundManager.h"
#include "viewManager.h"
#include "langManager.h"
#include "controller.h"
#include "player.h"
#include "map.h"

// position
#define HUD_HEART_POS vector2f(74.f, 22.f)
#define HUD_HEART_2_POS vector2f(74.f + 82.f, 22.f)
#define HUD_HEART_3_POS vector2f(74.f + 82.f * 2.f, 22.f)
#define HUD_DOWN_SELECT_POS vector2f(1726.f, 950.f)
#define HUD_LEFT_SELECT_POS vector2f(1700.f, 924.f)
#define HUD_TOP_SELECT_POS vector2f(1726.f, 898.f)
#define HUD_RIGHT_SELECT_POS vector2f(1752.f, 924.f)
#define HUD_NO_POWER_HAPPINESS_POS vector2f(1707.f, 988.f)
#define HUD_NO_POWER_SADNESS_POS vector2f(1794.f, 905.f)
#define HUD_NO_POWER_SCARE_POS vector2f(1620.f, 905.f)
#define HUD_NO_POWER_ANGER_POS vector2f(1707.f, 822.f)
#define HUD_JOY_RING_POS vector2f(1699.f, 982.f)
#define HUD_SADNESS_RING_POS vector2f(1784.f, 899.f)
#define HUD_SCARE_RING_POS vector2f(1614.f, 899.f)
#define HUD_ANGER_RING_POS vector2f(1699.f, 816.f)
#define HUD_ZONE_BANNER_POS vector2f(1096.f, 100.f)
#define HUD_ZONE_BANNER_TXT_POS vector2f(1592.f, 216.f)
#define HUD_FEEDBACK_NO_ITEM_POS1 vector2f(1592.f, 104.f)
#define HUD_FEEDBACK_NO_ITEM_POS2 vector2f(1700.f, 124.f)
#define HUD_FEEDBACK_NO_ITEM_POS3 vector2f(1584.f, 98.f)
#define HUD_KEY_POS vector2f(80.f, 120.f)
#define HUD_KEY_X_POS vector2f(152.f, 122.f)
#define HUD_NUMBER_KEY_POS vector2f(184.f, 122.f)

#define HUD_FEEDBACK_NO_ITEM_SIZE vector2f(84.f, 84.f);

// texture rect
#define HUD_FULL_HEART (sfIntRect){0, 0, 68, 60}
#define HUD_THREE_QUARTER_HEART (sfIntRect){68, 0, 68, 60}
#define HUD_HALF_HEART (sfIntRect){68 * 2, 0, 68, 60}
#define HUD_ONE_QUARTER_HEART (sfIntRect){68 * 3, 0, 68, 60}
#define HUD_EMPTY_HEART (sfIntRect){68 * 4, 0, 68, 60}
		
#define HUD_DOWN_ARROW (sfIntRect){0, 60, 30, 30}
#define HUD_DOWN_ARROW_SELECT (sfIntRect){30, 60, 30, 30}
#define HUD_LEFT_ARROW (sfIntRect){60, 60, 30, 30}
#define HUD_LEFT_ARROW_SELECT (sfIntRect){90, 60, 30, 30}
#define HUD_TOP_ARROW (sfIntRect){120, 60, 30, 30}
#define HUD_TOP_ARROW_SELECT (sfIntRect){150, 60, 30, 30}
#define HUD_RIGHT_ARROW (sfIntRect){180, 60, 30, 30}
#define HUD_RIGHT_ARROW_SELECT (sfIntRect){210, 60, 30, 30}

#define HUD_NO_POWER (sfIntRect){0, 90, 68, 68}
#define HUD_JOY_RING (sfIntRect){0, 158, 84, 80}
#define HUD_SADNESS_RING (sfIntRect){84, 158, 84, 80}
#define HUD_SCARE_RING (sfIntRect){84 * 2, 158, 84, 80}
#define HUD_ANGER_RING (sfIntRect){84 * 3, 158, 84, 80}
#define HUD_JOY_RING_STOPPED (sfIntRect){84 * 4, 158, 84, 80}
#define HUD_SADNESS_RING_STOPPED (sfIntRect){84 * 5, 158, 84, 80}
#define HUD_SCARE_RING_STOPPED (sfIntRect){84 * 6, 158, 84, 80}
#define HUD_ANGER_RING_STOPPED (sfIntRect){84 * 7, 158, 84, 80}

#define HUD_STAMINA_BASE (sfIntRect){0, 238, 52, 12}
#define HUD_STAMINA_BAR (sfIntRect){0, 250, 44, 4}
#define HUD_ZONE_BANNER (sfIntRect){0, 254, 864, 162}
#define HUD_A_INTERACT (sfIntRect){0, 416, 26, 26}
#define HUD_EXCLAMATION_POINT1 (sfIntRect){0, 442, 64, 64}
#define HUD_EXCLAMATION_POINT2 (sfIntRect){64, 442, 64, 64}
#define HUD_EXCLAMATION_POINT3 (sfIntRect){128, 442, 64, 64}
#define HUD_EXCLAMATION_POINT4 (sfIntRect){192, 442, 64, 64}
#define HUD_DIALOG_BOX (sfIntRect){0, 506, 234, 74}
#define HUD_FEEDBACK_NO_ITEM (sfIntRect){0, 654, 328, 74}
#define HUD_TALK_INTERACT (sfIntRect){0, 1148, 64, 64}
#define HUD_KEY (sfIntRect){0, 1212, 56, 28}
#define HUD_KEY_X (sfIntRect){0, 1240, 22, 26}

#define HUD_0 (sfIntRect){22, 1240, 22, 26}
#define HUD_1 (sfIntRect){44, 1240, 22, 26}
#define HUD_2 (sfIntRect){66, 1240, 22, 26}
#define HUD_3 (sfIntRect){88, 1240, 22, 26}
#define HUD_4 (sfIntRect){110, 1240, 22, 26}
#define HUD_5 (sfIntRect){132, 1240, 22, 26}
#define HUD_6 (sfIntRect){154, 1240, 22, 26}
#define HUD_7 (sfIntRect){176, 1240, 22, 26}
#define HUD_8 (sfIntRect){198, 1240, 22, 26}
#define HUD_9 (sfIntRect){220, 1240, 22, 26}

sfRectangleShape* HUD_Rect;
sfTexture* HUD_tex;
int nbHeart;
int HUD_FrameX;
float HUD_TimerAnim;
float HUD_timer;
float HUD_timerAddedData;
float time_to_lerp = 0.0f;
sfTexture* HUDTexture;
int HUD_NbJoyKey;
sfText* HUDTxt;
sfSprite* HUD_spr;
sfBool HUD_addedDataAppeared;
sfVector2f startPos;
sfVector2f endPos;
sfVector2f pos;
static sfBool isOnepass;
int HUD_NbHeart;

void HUD_loadTexture()
{
	HUD_tex = GetSfTexture("HUD");
	HUD_Rect = sfRectangleShape_create();
	sfRectangleShape_setTexture(HUD_Rect, HUD_tex, sfTrue);
}

void HUD_InitGame() {
	HUD_NbJoyKey = 0;
	HUD_spr = sfSprite_create();
	isOnepass = sfFalse;
	startPos = vector2f(1920.0f, 100.0f);
	endPos = vector2f(1200.0f, 100.0f);
	pos = vector2f(1920.0f, 100.0f);
	HUDTxt = sfText_create();
	HUD_addedDataAppeared = sfFalse;
	HUD_FrameX = 0;
	HUD_TimerAnim = 0.f;
	HUD_NbHeart = 0;
	HUD_loadTexture();

	sfText_setCharacterSize(HUDTxt, 38);
	sfText_setColor(HUDTxt, color(92, 199, 180, 255));
	sfText_setOutlineThickness(HUDTxt, 1.5f);
	sfText_setOutlineColor(HUDTxt, sfWhite);
}

void HUD_UpdateGame(Window* _window) {

	float dt = getDeltaTime();

	HUD_NbHeart = plr_GetMaxHeart() / 4;
	if (HUD_timerAddedData < 2.5f)
	{
		if (isOnepass)
		{
			isOnepass = sfFalse;
			time_to_lerp = 0.0f;
		}
		if (HUD_addedDataAppeared) {
			time_to_lerp += dt;
			if (time_to_lerp / 1.0f <= 1.0f)
				pos = vec2f_lerp(startPos, endPos, time_to_lerp / 1.f);
		}
	}
	else 
	{
		if (!isOnepass)
		{
			isOnepass = sfTrue;
			time_to_lerp = 0.0f;
		}
		time_to_lerp += dt;
		if (time_to_lerp / 1.0f <= 1.0f )
			pos = vec2f_lerp(endPos, startPos, time_to_lerp / 1.f);
		else
		{
			HUD_timerAddedData = 0.f;
			HUD_toggleAddedDataAppered(sfFalse);
		}
	}
	

	HUD_timer += dt;

}

void HUD_DisplayGame(Window* _window) {
	sfRenderTexture_setView(_window->rdrt, sfRenderTexture_getDefaultView(_window->rdrt));

	PlrUnlockOrder plrUnlockState = plr_GetUnlockState();
	PlrRingPowers plrRingPower = plr_GetPower();

	if (plrUnlockState >= PLRUNL_ATTRACT) {
		sfRectangleShape_setPosition(HUD_Rect, HUD_JOY_RING_POS);
		sfRectangleShape_setSize(HUD_Rect, vector2f(HUD_JOY_RING.width, HUD_JOY_RING.height));
		if (plrRingPower == PLR_ATTRACT) sfRectangleShape_setTextureRect(HUD_Rect, HUD_JOY_RING);
		else sfRectangleShape_setTextureRect(HUD_Rect, HUD_JOY_RING_STOPPED);
	}
	else {
		sfRectangleShape_setPosition(HUD_Rect, HUD_NO_POWER_HAPPINESS_POS);
		sfRectangleShape_setSize(HUD_Rect, vector2f(HUD_NO_POWER.width, HUD_NO_POWER.height));
		sfRectangleShape_setTextureRect(HUD_Rect, HUD_NO_POWER);
	}
	sfRenderTexture_drawRectangleShape(_window->rdrt, HUD_Rect, NULL);

	if (plrUnlockState >= PLRUNL_REPULSE) {
		sfRectangleShape_setPosition(HUD_Rect, HUD_SADNESS_RING_POS);
		if (plrRingPower == PLR_REPULSE) sfRectangleShape_setTextureRect(HUD_Rect, HUD_SADNESS_RING);
		else sfRectangleShape_setTextureRect(HUD_Rect, HUD_SADNESS_RING_STOPPED);
	}
	else {
		sfRectangleShape_setPosition(HUD_Rect, HUD_NO_POWER_SADNESS_POS);
		sfRectangleShape_setSize(HUD_Rect, vector2f(HUD_NO_POWER.width, HUD_NO_POWER.height));
		sfRectangleShape_setTextureRect(HUD_Rect, HUD_NO_POWER);
	}
	sfRenderTexture_drawRectangleShape(_window->rdrt, HUD_Rect, NULL);

	if (plrUnlockState >= PLRUNL_STASIS) {
		sfRectangleShape_setPosition(HUD_Rect, HUD_SCARE_RING_POS);
		if (plrRingPower == PLR_STATIS) sfRectangleShape_setTextureRect(HUD_Rect, HUD_SCARE_RING);
		else sfRectangleShape_setTextureRect(HUD_Rect, HUD_SCARE_RING_STOPPED);
	}
	else {
		sfRectangleShape_setPosition(HUD_Rect, HUD_NO_POWER_SCARE_POS);
		sfRectangleShape_setSize(HUD_Rect, vector2f(HUD_NO_POWER.width, HUD_NO_POWER.height));
		sfRectangleShape_setTextureRect(HUD_Rect, HUD_NO_POWER);
	}
	sfRenderTexture_drawRectangleShape(_window->rdrt, HUD_Rect, NULL);

	if (plrUnlockState >= PLRUNL_DESTRUCT) {
		sfRectangleShape_setPosition(HUD_Rect, HUD_ANGER_RING_POS);
		if (plrRingPower == PLR_DESTRUCT) sfRectangleShape_setTextureRect(HUD_Rect, HUD_ANGER_RING);
		else sfRectangleShape_setTextureRect(HUD_Rect, HUD_ANGER_RING_STOPPED);
	}
	else {
		sfRectangleShape_setPosition(HUD_Rect, HUD_NO_POWER_ANGER_POS);
		sfRectangleShape_setSize(HUD_Rect, vector2f(HUD_NO_POWER.width, HUD_NO_POWER.height));
		sfRectangleShape_setTextureRect(HUD_Rect, HUD_NO_POWER);
	}
	sfRenderTexture_drawRectangleShape(_window->rdrt, HUD_Rect, NULL);

	if (HUD_addedDataAppeared) {
		HUD_AddedDataAppered(_window);
	}

	sfRectangleShape_setSize(HUD_Rect, vector2f(HUD_DOWN_ARROW_SELECT.width, HUD_DOWN_ARROW_SELECT.height));
	sfRectangleShape_setPosition(HUD_Rect, HUD_DOWN_SELECT_POS);
	if (Gamepad_isButtonPressedNF(0, DPAD_DOWN)) sfRectangleShape_setTextureRect(HUD_Rect, HUD_DOWN_ARROW_SELECT); else sfRectangleShape_setTextureRect(HUD_Rect, HUD_DOWN_ARROW);
	sfRenderTexture_drawRectangleShape(_window->rdrt, HUD_Rect, NULL);

	sfRectangleShape_setSize(HUD_Rect, vector2f(HUD_RIGHT_ARROW_SELECT.width, HUD_RIGHT_ARROW_SELECT.height));
	sfRectangleShape_setPosition(HUD_Rect, HUD_RIGHT_SELECT_POS);
	if (Gamepad_isButtonPressedNF(0, DPAD_RIGHT)) sfRectangleShape_setTextureRect(HUD_Rect, HUD_RIGHT_ARROW_SELECT); else sfRectangleShape_setTextureRect(HUD_Rect, HUD_RIGHT_ARROW);
	sfRenderTexture_drawRectangleShape(_window->rdrt, HUD_Rect, NULL);

	sfRectangleShape_setSize(HUD_Rect, vector2f(HUD_LEFT_ARROW_SELECT.width, HUD_LEFT_ARROW_SELECT.height));
	sfRectangleShape_setPosition(HUD_Rect, HUD_LEFT_SELECT_POS);
	if (Gamepad_isButtonPressedNF(0, DPAD_LEFT)) sfRectangleShape_setTextureRect(HUD_Rect, HUD_LEFT_ARROW_SELECT); else sfRectangleShape_setTextureRect(HUD_Rect, HUD_LEFT_ARROW);
	sfRenderTexture_drawRectangleShape(_window->rdrt, HUD_Rect, NULL);

	sfRectangleShape_setSize(HUD_Rect, vector2f(HUD_TOP_ARROW_SELECT.width, HUD_TOP_ARROW_SELECT.height));
	sfRectangleShape_setPosition(HUD_Rect, HUD_TOP_SELECT_POS);
	if (Gamepad_isButtonPressedNF(0, DPAD_UP)) sfRectangleShape_setTextureRect(HUD_Rect, HUD_TOP_ARROW_SELECT); else sfRectangleShape_setTextureRect(HUD_Rect, HUD_TOP_ARROW);
	sfRenderTexture_drawRectangleShape(_window->rdrt, HUD_Rect, NULL);

	if (getCurrentBiome() == BIOME_JOY_DUNGEON) {
		sfRectangleShape_setSize(HUD_Rect, vector2f(HUD_KEY.width, HUD_KEY.height));
		sfRectangleShape_setPosition(HUD_Rect, HUD_KEY_POS);
		sfRectangleShape_setTextureRect(HUD_Rect, HUD_KEY);
		sfRenderTexture_drawRectangleShape(_window->rdrt, HUD_Rect, NULL);

		sfRectangleShape_setSize(HUD_Rect, vector2f(HUD_KEY_X.width, HUD_KEY_X.height));
		sfRectangleShape_setPosition(HUD_Rect, HUD_KEY_X_POS);
		sfRectangleShape_setTextureRect(HUD_Rect, HUD_KEY_X);
		sfRenderTexture_drawRectangleShape(_window->rdrt, HUD_Rect, NULL);

		if (HUD_NbJoyKey == 0) { sfRectangleShape_setSize(HUD_Rect, vector2f(HUD_0.width, HUD_0.height)); sfRectangleShape_setTextureRect(HUD_Rect, HUD_0); }
		else if (HUD_NbJoyKey == 1) { sfRectangleShape_setSize(HUD_Rect, vector2f(HUD_1.width, HUD_1.height)); sfRectangleShape_setTextureRect(HUD_Rect, HUD_1); }
		else if (HUD_NbJoyKey == 2) { sfRectangleShape_setSize(HUD_Rect, vector2f(HUD_2.width, HUD_2.height)); sfRectangleShape_setTextureRect(HUD_Rect, HUD_2); }
		else if (HUD_NbJoyKey == 3) { sfRectangleShape_setSize(HUD_Rect, vector2f(HUD_3.width, HUD_3.height)); sfRectangleShape_setTextureRect(HUD_Rect, HUD_3); }
		else if (HUD_NbJoyKey == 4) { sfRectangleShape_setSize(HUD_Rect, vector2f(HUD_4.width, HUD_4.height)); sfRectangleShape_setTextureRect(HUD_Rect, HUD_4); }
		else if (HUD_NbJoyKey == 5) { sfRectangleShape_setSize(HUD_Rect, vector2f(HUD_5.width, HUD_5.height)); sfRectangleShape_setTextureRect(HUD_Rect, HUD_5); }
		else if (HUD_NbJoyKey == 6) { sfRectangleShape_setSize(HUD_Rect, vector2f(HUD_6.width, HUD_6.height)); sfRectangleShape_setTextureRect(HUD_Rect, HUD_6); }
		else if (HUD_NbJoyKey == 7) { sfRectangleShape_setSize(HUD_Rect, vector2f(HUD_7.width, HUD_7.height)); sfRectangleShape_setTextureRect(HUD_Rect, HUD_7); }
		else if (HUD_NbJoyKey == 8) { sfRectangleShape_setSize(HUD_Rect, vector2f(HUD_8.width, HUD_8.height)); sfRectangleShape_setTextureRect(HUD_Rect, HUD_8); }
		else if (HUD_NbJoyKey == 9) { sfRectangleShape_setSize(HUD_Rect, vector2f(HUD_9.width, HUD_9.height)); sfRectangleShape_setTextureRect(HUD_Rect, HUD_9); }
		sfRectangleShape_setPosition(HUD_Rect, HUD_NUMBER_KEY_POS);
		sfRenderTexture_drawRectangleShape(_window->rdrt, HUD_Rect, NULL);
	}

	sfRectangleShape_setSize(HUD_Rect, vector2f(HUD_FULL_HEART.width, HUD_FULL_HEART.height));
	for (int i = 1; i < HUD_NbHeart + 1; i++) {
		sfRectangleShape_setPosition(HUD_Rect, vector2f(HUD_HEART_POS.x + 82.f * (i - 1), HUD_HEART_POS.y));
		if (plr_GetHeart() >= 4 * i) sfRectangleShape_setTextureRect(HUD_Rect, HUD_FULL_HEART);
		else if (plr_GetHeart() == 4 * i - 1) sfRectangleShape_setTextureRect(HUD_Rect, HUD_THREE_QUARTER_HEART);
		else if (plr_GetHeart() == 4 * i - 2) sfRectangleShape_setTextureRect(HUD_Rect, HUD_HALF_HEART);
		else if (plr_GetHeart() == 4 * i - 3) sfRectangleShape_setTextureRect(HUD_Rect, HUD_ONE_QUARTER_HEART);
		else if (plr_GetHeart() <= 4 * i - 4) sfRectangleShape_setTextureRect(HUD_Rect, HUD_EMPTY_HEART);
		sfRenderTexture_drawRectangleShape(_window->rdrt, HUD_Rect, NULL);
	}

	sfRenderTexture_setView(_window->rdrt, mainView->view);
}

void HUD_DeinitGame() {
	sfRectangleShape_destroy(HUD_Rect);
}

void HUD_DisplayPressA(sfVector2f _position, Window* _window) {
	sfRectangleShape_setSize(HUD_Rect, vector2f(HUD_A_INTERACT.width, HUD_A_INTERACT.height));
	sfRectangleShape_setPosition(HUD_Rect, _position);
	sfRectangleShape_setTextureRect(HUD_Rect, HUD_A_INTERACT);
	sfRenderTexture_drawRectangleShape(_window->rdrt, HUD_Rect, NULL);
}

void HUD_AddedDataAppered(Window* _window)
{
	HUD_timerAddedData += getDeltaTime();
	sfSprite_setScale(HUD_spr, vector2f(1.f, 1.f));
	sfSprite_setTexture(HUD_spr, GetSfTexture("HUD_in_game_indication"), sfTrue);
	sfText_setFont(HUDTxt, GetFont("Lilian"));
	switch (HUD_typeOfAddedData)
	{
	case HUD_INVENTORY:
		sfSprite_setTextureRect(HUD_spr, (sfIntRect) { 88, 518, 22, 74 });
		sfSprite_setPosition(HUD_spr, pos);
		sfRenderTexture_drawSprite(_window->rdrt, HUD_spr, NULL);
		sfSprite_setTextureRect(HUD_spr, (sfIntRect) { 110, 518, 218, 74 });
		sfSprite_setPosition(HUD_spr, vector2f(pos.x + 22.f, pos.y));
		sfSprite_setScale(HUD_spr, vector2f(3.5f, 1.f));
		sfRenderTexture_drawSprite(_window->rdrt, HUD_spr, NULL);
		sfText_setString(HUDTxt, lang_GetFromKey("HUD.data.item"));
		sfText_setPosition(HUDTxt, vector2f(pos.x + 22.f, pos.y + 10.f));
		sfRenderTexture_drawText(_window->rdrt, HUDTxt, NULL);
		break;
	case HUD_BESTIARY_BESTIARY:
		sfSprite_setTextureRect(HUD_spr, (sfIntRect) { 88, 518, 22, 74 });
 		sfSprite_setPosition(HUD_spr, pos);
		sfRenderTexture_drawSprite(_window->rdrt, HUD_spr, NULL);
		sfSprite_setTextureRect(HUD_spr, (sfIntRect) { 110, 518, 218, 74 });
		sfSprite_setPosition(HUD_spr, vector2f(pos.x + 22.f, pos.y));
		sfSprite_setScale(HUD_spr, vector2f(3.5f, 1.f));
		sfRenderTexture_drawSprite(_window->rdrt, HUD_spr, NULL);
		sfText_setString(HUDTxt, lang_GetFromKey("HUD.data.bestiary"));
		sfText_setPosition(HUDTxt, vector2f(pos.x + 22.f, pos.y + 10.f));
		sfRenderTexture_drawText(_window->rdrt, HUDTxt, NULL);
		break;
	case HUD_QUEST:
		sfSprite_setTextureRect(HUD_spr, (sfIntRect) { 88, 518, 22, 74 });
		sfSprite_setPosition(HUD_spr, pos);
		sfRenderTexture_drawSprite(_window->rdrt, HUD_spr, NULL);
		sfSprite_setTextureRect(HUD_spr, (sfIntRect) { 110, 518, 218, 74 });
		sfSprite_setPosition(HUD_spr, vector2f(pos.x + 22.f, pos.y));
		sfSprite_setScale(HUD_spr, vector2f(3.5f, 1.f));
		sfRenderTexture_drawSprite(_window->rdrt, HUD_spr, NULL);
		sfText_setString(HUDTxt, lang_GetFromKey("HUD.data.quest"));
		sfText_setPosition(HUDTxt, vector2f(pos.x + 22.f, pos.y + 10.f));
		sfRenderTexture_drawText(_window->rdrt, HUDTxt, NULL);
		break;
	case HUD_BESTIARY_DEMON:
		sfSprite_setTextureRect(HUD_spr, (sfIntRect) { 88, 518, 22, 74 });
		sfSprite_setPosition(HUD_spr, pos);
		sfRenderTexture_drawSprite(_window->rdrt, HUD_spr, NULL);
		sfSprite_setTextureRect(HUD_spr, (sfIntRect) { 110, 518, 218, 74 });
		sfSprite_setPosition(HUD_spr, vector2f(pos.x + 22.f, pos.y));
		sfSprite_setScale(HUD_spr, vector2f(3.5f, 1.f));
		sfRenderTexture_drawSprite(_window->rdrt, HUD_spr, NULL);
		sfText_setString(HUDTxt, lang_GetFromKey("HUD.data.bestiary.demon"));
		sfText_setPosition(HUDTxt, vector2f(pos.x + 22.f, pos.y + 10.f));
		sfRenderTexture_drawText(_window->rdrt, HUDTxt, NULL);
		break;
	default:
		break;
	}
	
}

void HUD_toggleAddedDataAppered(sfBool _playSfx)
{
	if(_playSfx) ("SFX_new_entry");
	HUD_addedDataAppeared = !HUD_addedDataAppeared;
}

void HUD_addKey()
{
	prt_CreateGainParticles(vector2f(80.f, 120.f));
	HUD_NbJoyKey++;
}

int getNbJoyKey()
{
	return HUD_NbJoyKey;
}
