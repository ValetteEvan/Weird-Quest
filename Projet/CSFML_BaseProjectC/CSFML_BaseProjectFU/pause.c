#include "pause.h"
#include "stateManager.h"
#include "viewManager.h"
#include "controller.h"
#include "dialogBox.h"
#include "fontManager.h"
#include "fadeManager.h"
#include "langManager.h"
#include "textureManager.h"
#include "quest.h"
#include "player.h"
#include "options.h"
#include "soundManager.h"


static sfRectangleShape* pause_RenderRect;
static sfText* pause_RenderText;
static sfSprite* spMenuPause;

static int pauseSelect = 0;
static int pauseQuete = 0;
static int yPauseSelect = 0;

sfBool isSelectedBestiare = sfFalse;

static float pause_TimerInput = 0.0f;

static char pause_StrTemp[100];

sfFont* pause_fontTitlle;

char* itemNbChar;

void initPause(Window* _window)
{
	static sfBool onePass = sfTrue;
	if (onePass) {
		itemList = stdList_Create(sizeof(Itemlist), 0);
		pause_AddItem(PAUSE_TOTEM_PIG);
		pause_AddItem(PAUSE_TOTEM_OCTO);
		pause_AddItem(PAUSE_TOTEM_EAGLE);
		pause_AddItem(PAUSE_TOTEM_CHAMELEON);
		pause_AddItem(PAUSE_GUARDIAN_STICK);
		pause_AddItem(PAUSE_GUARDIAN_RING);
		pause_AddItem(PAUSE_JOY_RING);
		pause_AddItem(PAUSE_SADNESS_RING);
		pause_AddItem(PAUSE_SCARE_RING);
		pause_AddItem(PAUSE_ANGER_RING);
		pause_AddItem(PAUSE_JOY_FISH);
		pause_AddItem(PAUSE_SADNESS_FISH);
		pause_AddItem(PAUSE_SCARE_FISH);
		pause_AddItem(PAUSE_ANGER_FISH);
		pause_AddItem(PAUSE_YOYO);
		pause_AddItem(PAUSE_CABORALS);
		pause_AddItem(PAUSE_SANDAMANDER);
		pause_AddItem(PAUSE_VOLTIGEONS);

		itemNbChar = calloc(1, sizeof(char) * 3);

		pause_RenderRect = sfRectangleShape_create();
		pause_RenderText = sfText_create();
		spMenuPause = sfSprite_create();
		sfRectangleShape_setSize(pause_RenderRect, (sfVector2f) { (float)mainView->defaultVideoMode.x, (float)mainView->defaultVideoMode.y });
		sfRectangleShape_setFillColor(pause_RenderRect, (sfColor) { 0, 0, 0, 150 });
		sfText_setFont(pause_RenderText, GetFont("Lilian"));
		sfText_setColor(pause_RenderText, sfRed);

		onePass = sfFalse;
	}
	pauseSelect = 0;

}

void pause_AddItem(Items _item)
{
	STD_LIST_PUSHBACK(itemList, Itemlist, { _item, 0});
}

void updatePause(Window* _window)
{
	if (getState() != GAME) return;
	pause_TimerInput += getUnscaledDeltaTime();
	if (Gamepad_isButtonPressed(0, LB, _window->renderWindow) && pause_TimerInput > 0.2f)
	{
		pause_TimerInput = 0.0f;
		pauseSelect = 0;
		pauseQuete = 0;
		isSelectedBestiare = sfFalse;
		pauseMenu--;
		playSoundFX("UI_NextPage");
		if (pauseMenu < 0) pauseMenu = 3;
	}
	if (Gamepad_isButtonPressed(0, RB, _window->renderWindow) && pause_TimerInput > 0.2f)
	{
		pause_TimerInput = 0.0f;
		pauseSelect = 0;
		pauseQuete = 0;
		isSelectedBestiare = sfFalse;
		pauseMenu++;
		playSoundFX("UI_NextPage");
		if (pauseMenu > 3) pauseMenu = 0;
	}

	if (customKeyboardIsKeyPressed(sfKeyLeft, _window->renderWindow) && pause_TimerInput > 0.2f)
	{
		pause_TimerInput = 0.0f;
		pauseSelect = 0;
		pauseQuete = 0;
		isSelectedBestiare = sfFalse;
		pauseMenu--;
		if (pauseMenu < 0) pauseMenu = 3;
	}

	if (customKeyboardIsKeyPressed(sfKeyRight, _window->renderWindow) && pause_TimerInput > 0.2f)
	{
		pause_TimerInput = 0.0f;
		pauseSelect = 0;
		pauseQuete = 0;
		isSelectedBestiare = sfFalse;
		pauseMenu++;
		if (pauseMenu > 3) pauseMenu = 0;
	}
	if (Gamepad_isButtonPressed(0, START, _window->renderWindow) && pause_TimerInput > 0.2f)
	{
		pauseSelect = 0;
		isSelectedBestiare = sfFalse;
		togglePause();
		pause_TimerInput = 0.0f;
	}
	
	switch (pauseMenu)
	{
	case PAUSE_PERSO:
		if (Gamepad_isButtonPressed(0, B, _window->renderWindow) && pause_TimerInput > 0.2f)
		{
			pauseSelect = 0;
			isSelectedBestiare = sfFalse;
			togglePause();
			pause_TimerInput = 0.0f;
			playSoundFX("UI_ChangeSelection");
		}
		if (Gamepad_isButtonPressed(0, START, _window->renderWindow) && pause_TimerInput > 0.2f)
		{
			pauseSelect = 0;
			isSelectedBestiare = sfFalse;
			togglePause();
			pause_TimerInput = 0.0f;
		}
		if (Gamepad_getStickPos(0, STICKL, _window->renderWindow).y < 0 && pause_TimerInput > 0.2f)
		{
			if (pauseSelect >= 13)
				pauseSelect -= 3;
			pauseSelect--;
			if (pauseSelect < 0)
				pauseSelect = 17;
			pause_TimerInput = 0.0f;
			playSoundFX("UI_ChangeSelection");
		}
		if (Gamepad_getStickPos(0, STICKL, _window->renderWindow).y > 0 && pause_TimerInput > 0.2f)
		{
			if (pauseSelect >= 9 && pauseSelect < 13)
				pauseSelect += 3;
			pauseSelect++;
			if (pauseSelect > 17)
				pauseSelect = 0;
			pause_TimerInput = 0.0f;
			playSoundFX("UI_ChangeSelection");
		}
		if (Gamepad_getStickPos(0, STICKL, _window->renderWindow).x < 0 && pause_TimerInput > 0.2f)
		{
			
			pauseSelect--;
			if (pauseSelect < 0)
				pauseSelect = 17;
			pause_TimerInput = 0.0f;
			playSoundFX("UI_ChangeSelection");
		}
		if (Gamepad_getStickPos(0, STICKL, _window->renderWindow).x > 0 && pause_TimerInput > 0.2f)
		{
			pauseSelect++;
			if (pauseSelect > 17)
				pauseSelect = 0;
			pause_TimerInput = 0.0f;
			playSoundFX("UI_ChangeSelection");
		}
		break;
	case PAUSE_BESTIARE:
		if (!isSelectedBestiare)
		{
			if (Gamepad_isButtonPressed(0, B, _window->renderWindow) && pause_TimerInput > 0.2f)
			{
				pauseSelect = 0;
				isSelectedBestiare = sfFalse;
				togglePause();
				pause_TimerInput = 0.0f;
				playSoundFX("UI_ChangeSelection");
			}
			if (Gamepad_getStickPos(0, STICKL, _window->renderWindow).x < 0 && pause_TimerInput > 0.2f)
			{
				pauseSelect--;
				if (pauseSelect < 0)
					pauseSelect = 1;
				pause_TimerInput = 0.0f;
				playSoundFX("UI_ChangeSelection");
			}
			if (Gamepad_getStickPos(0, STICKL, _window->renderWindow).x > 0 && pause_TimerInput > 0.2f)
			{
				pauseSelect++;
				if (pauseSelect > 1)
					pauseSelect = 0;
				pause_TimerInput = 0.0f;
				playSoundFX("UI_ChangeSelection");
			}
		}
		
		if (Gamepad_isButtonPressed(0, A, _window->renderWindow) && pause_TimerInput > 0.2f)
		{
			if (pauseSelect == 0)
			{
				yPauseSelect = 0;
				bestiare = PAUSE_PET;
				pauseSelect = 0;
				isSelectedBestiare = sfTrue;
				playSoundFX("UI_ChangeSelection");
			}
			else if (pauseSelect == 1)
			{
				yPauseSelect = 0;
				bestiare = PAUSE_MONSTER;
				pauseSelect = 0;
				isSelectedBestiare = sfTrue;
				playSoundFX("UI_ChangeSelection");
			}
			pause_TimerInput = 0.0f;
		}
		if (isSelectedBestiare && bestiare == PAUSE_PET) {
			if (Gamepad_isButtonPressed(0, B, _window->renderWindow) && pause_TimerInput > 0.2f)
			{
				pauseSelect = 0;
				isSelectedBestiare = sfFalse;
				pause_TimerInput = 0.0f;
				playSoundFX("UI_ChangeSelection");
			}
			if (Gamepad_getStickPos(0, STICKL, _window->renderWindow).y < 0 && pause_TimerInput > 0.2f)
			{
				yPauseSelect--;
				if (yPauseSelect < 0)
				{
					yPauseSelect = 0;
				}
				pause_TimerInput = 0.0f;
				playSoundFX("UI_ChangeSelection");
			}
			if (Gamepad_getStickPos(0, STICKL, _window->renderWindow).y > 0 && pause_TimerInput > 0.2f)
			{
				yPauseSelect++;
				if (yPauseSelect > 4)
				{
					yPauseSelect = 4;
				}
				pause_TimerInput = 0.0f;
				playSoundFX("UI_ChangeSelection");
			}
			if (Gamepad_getStickPos(0, STICKL, _window->renderWindow).x < 0 && pause_TimerInput > 0.2f)
			{
				pauseSelect--;
				if (pauseSelect < 0)
				{
					pauseSelect = 0;
				}
				pause_TimerInput = 0.0f;
				playSoundFX("UI_ChangeSelection");
			}
			if (Gamepad_getStickPos(0, STICKL, _window->renderWindow).x > 0 && pause_TimerInput > 0.2f)
			{
				pauseSelect++;
				if (pauseSelect > 2)
				{
					pauseSelect = 2;
				}
				pause_TimerInput = 0.0f;
				playSoundFX("UI_ChangeSelection");
			}
		}
		if (isSelectedBestiare && bestiare == PAUSE_MONSTER) {
			if (Gamepad_isButtonPressed(0, B, _window->renderWindow) && pause_TimerInput > 0.2f)
			{
				pauseSelect = 0;
				isSelectedBestiare = sfFalse;
				pause_TimerInput = 0.0f;
				playSoundFX("UI_ChangeSelection");
			}
			if (Gamepad_getStickPos(0, STICKL, _window->renderWindow).x < 0 && pause_TimerInput > 0.2f)
			{
				pauseSelect--;
				if (pauseSelect < 0)
				{
					pauseSelect = 0;
				}
				pause_TimerInput = 0.0f;
				playSoundFX("UI_ChangeSelection");
			}
			if (Gamepad_getStickPos(0, STICKL, _window->renderWindow).x > 0 && pause_TimerInput > 0.2f)
			{
				pauseSelect++;
				if (pauseSelect > 2)
				{
					pauseSelect = 2;
				}
				pause_TimerInput = 0.0f;
				playSoundFX("UI_ChangeSelection");
			}
		}
		break;
	case PAUSE_QUETE:
		qst_UpdateScreen(_window);
		if (Gamepad_isButtonPressed(0, B, _window->renderWindow) && pause_TimerInput > 0.2f)
		{
			pauseSelect = 0;
			isSelectedBestiare = sfFalse;
			togglePause();
			pause_TimerInput = 0.0f;
			playSoundFX("UI_ChangeSelection");
		}
		if (Gamepad_getStickPos(0, STICKL, _window->renderWindow).x < 0 && pause_TimerInput > 0.2f)
		{
			qst_PrevCategory();
			pause_TimerInput = 0.0f;
			playSoundFX("UI_ChangeSelection");
		}
		if (Gamepad_getStickPos(0, STICKL, _window->renderWindow).x > 0 && pause_TimerInput > 0.2f)
		{
			qst_NextCategory();
			pause_TimerInput = 0.0f;
			playSoundFX("UI_ChangeSelection");
		}
		if (pause_queteIdPNJ == PAUSE_MENTOR)
		{
			if (Gamepad_getStickPos(0, STICKL, _window->renderWindow).y > 0 && pause_TimerInput > 0.2f)
			{
				qst_SelectNext();
				pause_TimerInput = 0.0f;
				playSoundFX("UI_ChangeSelection");
			}
			if (Gamepad_getStickPos(0, STICKL, _window->renderWindow).y < 0 && pause_TimerInput > 0.2f)
			{
				qst_SelectPrev();
				pause_TimerInput = 0.0f;
				playSoundFX("UI_ChangeSelection");
			}
		}	
		if (pause_queteIdPNJ == PAUSE_ARNOLD)
		{
			if (Gamepad_getStickPos(0, STICKL, _window->renderWindow).y > 0 && pause_TimerInput > 0.2f)
			{
				qst_SelectNext();
				pause_TimerInput = 0.0f;
				playSoundFX("UI_ChangeSelection");
			}
			if (Gamepad_getStickPos(0, STICKL, _window->renderWindow).y < 0 && pause_TimerInput > 0.2f)
			{
				qst_SelectPrev();
				pause_TimerInput = 0.0f;
				playSoundFX("UI_ChangeSelection");
			}
		}
		if (pause_queteIdPNJ == PAUSE_SHAMAN)
		{
			if (Gamepad_getStickPos(0, STICKL, _window->renderWindow).y > 0 && pause_TimerInput > 0.2f)
			{
				qst_SelectNext();
				pause_TimerInput = 0.0f;
				playSoundFX("UI_ChangeSelection");
			}
			if (Gamepad_getStickPos(0, STICKL, _window->renderWindow).y < 0 && pause_TimerInput > 0.2f)
			{
				qst_SelectPrev();
				pause_TimerInput = 0.0f;
				playSoundFX("UI_ChangeSelection");
			}
		}
		break;
	case PAUSE_OPTIONS:
		options_Update(_window);
		if (Gamepad_isButtonPressed(0, B, _window->renderWindow) && pause_TimerInput > 0.2f)
		{
			pause_TimerInput = 0.0f;
			pauseSelect = 0;
			isSelectedBestiare = sfFalse;
			playSoundFX("UI_ChangeSelection");
			togglePause();
		}
		break;
	default:
		break;
	}

	qst_Update(_window);
}

void displayPause(Window* _window)
{
	sfRenderTexture_setView(_window->rdrt, sfRenderTexture_getDefaultView(_window->rdrt));
	//fade_Display(_window);

	sfSprite_setTexture(spMenuPause, GetSfTexture("pause_BO"), sfTrue);
	switch (pauseMenu)
	{
	case PAUSE_PERSO:
		sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 0, 0, 1644, 850 });
		sfSprite_setPosition(spMenuPause, (sfVector2f) { 136.f, 170.f });
		sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);

		sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 0, 6076, 82, 96 });
		sfSprite_setPosition(spMenuPause, (sfVector2f) { 778.f, 64.f });
		sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);

		sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 82, 5984, 82, 96 });
		sfSprite_setPosition(spMenuPause, (sfVector2f) { 778.f + 100, 90.f });
		sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);

		sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 82 * 2, 5984, 82, 90 });
		sfSprite_setPosition(spMenuPause, (sfVector2f) { 778.f + 200, 90.f });
		sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);

		sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 82 * 3, 5984, 82, 96 });
		sfSprite_setPosition(spMenuPause, (sfVector2f) { 778.f + 300, 90.f });
		sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
		if (pauseSelect == 9) {
			if (plr_GetUnlockState() >= PLRUNL_PIG_HAT)
			{
				sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 0, 1850, 112, 100 });
				sfSprite_setPosition(spMenuPause, (sfVector2f) { 916.f, 411.f });
				sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
				sfText_setColor(pause_RenderText, sfWhite);
				sfText_setString(pause_RenderText, lang_GetFromKey("explain.pig.totem"));
				sfText_setCharacterSize(pause_RenderText, 19);
				sfText_setPosition(pause_RenderText, vector2f(744.f, 790.f));
				sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
				sfText_setString(pause_RenderText, lang_GetFromKey("name.pig.totem"));
				sfText_setCharacterSize(pause_RenderText, 29);
				sfText_setPosition(pause_RenderText, vector2f(888.f, 726.f));
				sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
			}
			else
			{
				sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 0, 1850, 112, 100 });
				sfSprite_setPosition(spMenuPause, (sfVector2f) { 916.f, 411.f });
				sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
				sfText_setColor(pause_RenderText, sfWhite);
				sfText_setString(pause_RenderText, "???");
				sfText_setCharacterSize(pause_RenderText, 19);
				sfText_setPosition(pause_RenderText, vector2f(744.f, 790.f));
				sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
				sfText_setString(pause_RenderText, "???");
				sfText_setCharacterSize(pause_RenderText, 29);
				sfText_setPosition(pause_RenderText, vector2f(888.f, 726.f));
				sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
			}
		}
		if (plr_GetUnlockState() >= PLRUNL_PIG_HAT)
			sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 0, 1316, 118, 118 });
		else
			sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 0, 1316 + 118, 118, 118 });
		sfSprite_setPosition(spMenuPause, (sfVector2f) { 914.f, 418.f });
		sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);

		if (pauseSelect == 8) {
			if (plr_GetUnlockState() >= PLRUNL_POLPO_HAT)
			{
				sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 0, 1552, 146, 138 });
				sfSprite_setPosition(spMenuPause, (sfVector2f) { 772.f, 292.f });
				sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
				sfText_setColor(pause_RenderText, sfWhite);
				sfText_setString(pause_RenderText, lang_GetFromKey("explain.octopus.totem"));
				sfText_setCharacterSize(pause_RenderText, 19);
				sfText_setPosition(pause_RenderText, vector2f(744.f, 790.f));
				sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
				sfText_setString(pause_RenderText, lang_GetFromKey("name.octopus.totem"));
				sfText_setCharacterSize(pause_RenderText, 29);
				sfText_setPosition(pause_RenderText, vector2f(888.f, 726.f));
				sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
			}
			else
			{
				sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 0, 1552, 146, 138 });
				sfSprite_setPosition(spMenuPause, (sfVector2f) { 772.f, 292.f });
				sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
				sfText_setColor(pause_RenderText, sfWhite);
				sfText_setString(pause_RenderText, "???");
				sfText_setCharacterSize(pause_RenderText, 19);
				sfText_setPosition(pause_RenderText, vector2f(744.f, 790.f));
				sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
				sfText_setString(pause_RenderText, "???");
				sfText_setCharacterSize(pause_RenderText, 29);
				sfText_setPosition(pause_RenderText, vector2f(888.f, 726.f));
				sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
			}
		}
		if (plr_GetUnlockState() >= PLRUNL_POLPO_HAT)
			sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 118, 1316, 118, 118 });
		else
			sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 118, 1316 + 118, 118, 118 });
		sfSprite_setPosition(spMenuPause, (sfVector2f) { 786.f, 302.f });
		sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);

		if (pauseSelect == 7) {
			if (plr_GetUnlockState() >= PLRUNL_EAGLE_HAT)
			{
				sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 0, 1552, 146, 138 });
				sfSprite_setPosition(spMenuPause, (sfVector2f) { 772.f, 518.f });
				sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
				sfText_setColor(pause_RenderText, sfWhite);
				sfText_setString(pause_RenderText, lang_GetFromKey("explain.eagle.totem"));
				sfText_setCharacterSize(pause_RenderText, 19);
				sfText_setPosition(pause_RenderText, vector2f(744.f, 790.f));
				sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
				sfText_setString(pause_RenderText, lang_GetFromKey("name.eagle.totem"));
				sfText_setCharacterSize(pause_RenderText, 29);
				sfText_setPosition(pause_RenderText, vector2f(888.f, 726.f));
				sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
			}
			else
			{
				sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 0, 1552, 146, 138 });
				sfSprite_setPosition(spMenuPause, (sfVector2f) { 772.f, 518.f });
				sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
				sfText_setColor(pause_RenderText, sfWhite);
				sfText_setString(pause_RenderText, "???");
				sfText_setCharacterSize(pause_RenderText, 19);
				sfText_setPosition(pause_RenderText, vector2f(744.f, 790.f));
				sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
				sfText_setString(pause_RenderText, "???");
				sfText_setCharacterSize(pause_RenderText, 29);
				sfText_setPosition(pause_RenderText, vector2f(888.f, 726.f));
				sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
			}
		}
		if (plr_GetUnlockState() >= PLRUNL_EAGLE_HAT)
			sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 118*2, 1316, 118, 118 });
		else
			sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 118*2, 1316 + 118, 118, 118 });
		sfSprite_setPosition(spMenuPause, (sfVector2f) { 786.f, 530.f });
		sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);

		if (pauseSelect == 6) {
			if (plr_GetUnlockState() >= PLRUNL_INVISIBILITY_HAT)
			{
				sfText_setColor(pause_RenderText, sfWhite);
				sfText_setString(pause_RenderText, lang_GetFromKey("explain.chameleon.totem"));
				sfText_setCharacterSize(pause_RenderText, 19);
				sfText_setPosition(pause_RenderText, vector2f(744.f, 790.f));
				sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
				sfText_setString(pause_RenderText, lang_GetFromKey("name.chameleon.totem"));
				sfText_setCharacterSize(pause_RenderText, 29);
				sfText_setPosition(pause_RenderText, vector2f(888.f, 726.f));
				sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
			}
			else
			{
				sfText_setColor(pause_RenderText, sfWhite);
				sfText_setString(pause_RenderText, "???");
				sfText_setCharacterSize(pause_RenderText, 19);
				sfText_setPosition(pause_RenderText, vector2f(744.f, 790.f));
				sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
				sfText_setString(pause_RenderText, "???");
				sfText_setCharacterSize(pause_RenderText, 29);
				sfText_setPosition(pause_RenderText, vector2f(888.f, 726.f));
				sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
			}

			sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 0, 1552, 146, 138 });
			sfSprite_setPosition(spMenuPause, (sfVector2f) { 628.f, 408.f });
			sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
		}
		if (plr_GetUnlockState() >= PLRUNL_INVISIBILITY_HAT)
			sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 118 * 3, 1316, 118, 118 });
		else
			sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 118 * 3, 1316 + 118, 118, 118 });
		sfSprite_setPosition(spMenuPause, (sfVector2f) { 642.f, 418.f });
		sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);

		if (pauseSelect == 0) {
			if (plr_GetUnlockState() >= PLRUNL_STAFF)
			{
				sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 0, 2690, 112, 394 });
				sfSprite_setPosition(spMenuPause, (sfVector2f) { 250.f, 268.f });
				sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
				sfText_setColor(pause_RenderText, sfWhite);
				sfText_setString(pause_RenderText, lang_GetFromKey("explain.guardian.stick"));
				sfText_setCharacterSize(pause_RenderText, 19);
				sfText_setPosition(pause_RenderText, vector2f(744.f, 790.f));
				sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
				sfText_setString(pause_RenderText, lang_GetFromKey("name.guardian.stick"));
				sfText_setCharacterSize(pause_RenderText, 29);
				sfText_setPosition(pause_RenderText, vector2f(888.f, 726.f));
				sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
			}
			else
			{
				sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 0, 2690, 112, 394 });
				sfSprite_setPosition(spMenuPause, (sfVector2f) { 250.f, 268.f });
				sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
				sfText_setColor(pause_RenderText, sfWhite);
				sfText_setString(pause_RenderText, "???");
				sfText_setCharacterSize(pause_RenderText, 19);
				sfText_setPosition(pause_RenderText, vector2f(744.f, 790.f));
				sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
				sfText_setString(pause_RenderText, "???");
				sfText_setCharacterSize(pause_RenderText, 29);
				sfText_setPosition(pause_RenderText, vector2f(888.f, 726.f));
				sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
			}
		}
		if (plr_GetUnlockState() >= PLRUNL_STAFF)
			sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 0, 1950, 80, 370 });
		else
			sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 0, 1950 + 370, 80, 370 });
		sfSprite_setPosition(spMenuPause, (sfVector2f) { 266.f, 282.f });
		sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);

		if (pauseSelect == 5) {
			if (plr_GetUnlockState() >= PLRUNL_RING_CAPTURE)
			{
				sfText_setColor(pause_RenderText, sfWhite);
				sfText_setString(pause_RenderText, lang_GetFromKey("explain.guardian.ring"));
				sfText_setCharacterSize(pause_RenderText, 19);
				sfText_setPosition(pause_RenderText, vector2f(744.f, 790.f));
				sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
				sfText_setString(pause_RenderText, lang_GetFromKey("name.guardian.ring"));
				sfText_setCharacterSize(pause_RenderText, 29);
				sfText_setPosition(pause_RenderText, vector2f(888.f, 726.f));
				sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);

			}
			else 
			{
				sfText_setColor(pause_RenderText, sfWhite);
				sfText_setString(pause_RenderText, "???");
				sfText_setCharacterSize(pause_RenderText, 19);
				sfText_setPosition(pause_RenderText, vector2f(744.f, 790.f));
				sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
				sfText_setString(pause_RenderText, "???");
				sfText_setCharacterSize(pause_RenderText, 29);
				sfText_setPosition(pause_RenderText, vector2f(888.f, 726.f));
				sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
			}
			sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 0, 1850, 112, 100 });
			sfSprite_setPosition(spMenuPause, (sfVector2f) { 358.f, 582.f });
			sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
		}
		
		if (plr_GetUnlockState() >= PLRUNL_RING_CAPTURE)
			sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 0, 1690, 84, 80 });
		else
			sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 0, 1690 + 80, 84, 80 });
		sfSprite_setPosition(spMenuPause, (sfVector2f) { 370.f, 592.f});
		sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);

		if (pauseSelect == 4) {
			if (plr_GetUnlockState() >= PLRUNL_ATTRACT)
			{
				sfText_setColor(pause_RenderText, sfWhite);
				sfText_setString(pause_RenderText, lang_GetFromKey("explain.joy.ring"));
				sfText_setCharacterSize(pause_RenderText, 19);
				sfText_setPosition(pause_RenderText, vector2f(744.f, 790.f));
				sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
				sfText_setString(pause_RenderText, lang_GetFromKey("name.joy.ring"));
				sfText_setCharacterSize(pause_RenderText, 29);
				sfText_setPosition(pause_RenderText, vector2f(888.f, 726.f));
				sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
			}
			else
			{
				sfText_setColor(pause_RenderText, sfWhite);
				sfText_setString(pause_RenderText, "???");
				sfText_setCharacterSize(pause_RenderText, 19);
				sfText_setPosition(pause_RenderText, vector2f(744.f, 790.f));
				sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
				sfText_setString(pause_RenderText, "???");
				sfText_setCharacterSize(pause_RenderText, 29);
				sfText_setPosition(pause_RenderText, vector2f(888.f, 726.f));
				sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
			}
			sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 0, 1850, 112, 100 });
			sfSprite_setPosition(spMenuPause, (sfVector2f) { 440.f, 485.f });
			sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
		}
		if (plr_GetUnlockState() >= PLRUNL_ATTRACT)
			sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 84, 1690, 84, 80 });
		else
			sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 84, 1690 + 80, 84, 80 });
		sfSprite_setPosition(spMenuPause, (sfVector2f) { 456.f, 496.f});
		sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);

		if (pauseSelect == 3) {
			if (plr_GetUnlockState() >= PLRUNL_REPULSE)
			{
				sfText_setColor(pause_RenderText, sfWhite);
				sfText_setString(pause_RenderText, lang_GetFromKey("explain.sadness.ring"));
				sfText_setCharacterSize(pause_RenderText, 19);
				sfText_setPosition(pause_RenderText, vector2f(744.f, 790.f));
				sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
				sfText_setString(pause_RenderText, lang_GetFromKey("name.sadness.ring"));
				sfText_setCharacterSize(pause_RenderText, 29);
				sfText_setPosition(pause_RenderText, vector2f(888.f, 726.f));
				sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
			}
			else
			{
				sfText_setColor(pause_RenderText, sfWhite);
				sfText_setString(pause_RenderText, "???");
				sfText_setCharacterSize(pause_RenderText, 19);
				sfText_setPosition(pause_RenderText, vector2f(744.f, 790.f));
				sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
				sfText_setString(pause_RenderText, "???");
				sfText_setCharacterSize(pause_RenderText, 29);
				sfText_setPosition(pause_RenderText, vector2f(888.f, 726.f));
				sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
			}
			sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 0, 1850, 112, 100 });
			sfSprite_setPosition(spMenuPause, (sfVector2f) { 340.f, 570.f - 172.f});
			sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
		}
		if (plr_GetUnlockState() >= PLRUNL_REPULSE)
			sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 84 * 2, 1690, 84, 80 });
		else
			sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 84 * 2, 1690 + 80, 84, 80 });
		sfSprite_setPosition(spMenuPause, (sfVector2f) { 358.f, 582.f - 172.f});
		sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);

		if (pauseSelect == 2) {
			if (plr_GetUnlockState() >= PLRUNL_STASIS)
			{
				sfText_setColor(pause_RenderText, sfWhite);
				sfText_setString(pause_RenderText, lang_GetFromKey("explain.scare.ring"));
				sfText_setCharacterSize(pause_RenderText, 19);
				sfText_setPosition(pause_RenderText, vector2f(744.f, 790.f));
				sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
				sfText_setString(pause_RenderText, lang_GetFromKey("name.scare.ring"));
				sfText_setCharacterSize(pause_RenderText, 29);
				sfText_setPosition(pause_RenderText, vector2f(888.f, 726.f));
				sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
			}
			else
			{
				sfText_setColor(pause_RenderText, sfWhite);
				sfText_setString(pause_RenderText, "???");
				sfText_setCharacterSize(pause_RenderText, 19);
				sfText_setPosition(pause_RenderText, vector2f(744.f, 790.f));
				sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
				sfText_setString(pause_RenderText, "???");
				sfText_setCharacterSize(pause_RenderText, 29);
				sfText_setPosition(pause_RenderText, vector2f(888.f, 726.f));
				sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
			}
			sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 0, 1850, 112, 100 });
			sfSprite_setPosition(spMenuPause, (sfVector2f) { 440.f, 485.f - 172 });
			sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
		}
		if (plr_GetUnlockState() >= PLRUNL_STASIS)
			sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 84 * 3, 1690, 84, 80 });
		else
			sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 84 * 3, 1690 + 80, 84, 80 });
		sfSprite_setPosition(spMenuPause, (sfVector2f) { 456.f, 496.f - 172.f });
		sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);

		if (pauseSelect == 1) {
			if (plr_GetUnlockState() >= PLRUNL_DESTRUCT)
			{
				sfText_setColor(pause_RenderText, sfWhite);
				sfText_setString(pause_RenderText, lang_GetFromKey("explain.anger.ring"));
				sfText_setCharacterSize(pause_RenderText, 19);
				sfText_setPosition(pause_RenderText, vector2f(744.f, 790.f));
				sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
				sfText_setString(pause_RenderText, lang_GetFromKey("name.anger.ring"));
				sfText_setCharacterSize(pause_RenderText, 29);
				sfText_setPosition(pause_RenderText, vector2f(888.f, 726.f));
				sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
			}
			else
			{
				sfText_setColor(pause_RenderText, sfWhite);
				sfText_setString(pause_RenderText, "???");
				sfText_setCharacterSize(pause_RenderText, 19);
				sfText_setPosition(pause_RenderText, vector2f(744.f, 790.f));
				sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
				sfText_setString(pause_RenderText, "???");
				sfText_setCharacterSize(pause_RenderText, 29);
				sfText_setPosition(pause_RenderText, vector2f(888.f, 726.f));
				sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
			}

			sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 0, 1850, 112, 100 });
			sfSprite_setPosition(spMenuPause, (sfVector2f) { 355.f, 575.f - (172.f * 2) });
			sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
		}
		if (plr_GetUnlockState() >= PLRUNL_DESTRUCT)
			sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 84 * 4, 1690, 84, 80 });
		else
			sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 84 * 4, 1690 + 80, 84, 80 });
		sfSprite_setPosition(spMenuPause, (sfVector2f) { 370.f, 582.f - (172.f * 2) });
		sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);

		if (pauseSelect == 14) {
			sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 0 + 138, 850, 138, 138 });
			sfSprite_setPosition(spMenuPause, (sfVector2f) { 1086.f, 496.f });
			sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
			
			if (pause_ItemsDiscovered & (PAUSE_JOY_FISH))
			{
				sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 0, 1168, 152, 148 });
				sfSprite_setPosition(spMenuPause, (sfVector2f) { 1076.f, 302.f + 192});
				sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
				sfText_setColor(pause_RenderText, sfWhite);
				sfText_setString(pause_RenderText, lang_GetFromKey("info.joy.fish"));
				sfText_setCharacterSize(pause_RenderText, 19);
				sfText_setPosition(pause_RenderText, vector2f(744.f, 790.f));
				sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
				sfText_setString(pause_RenderText, lang_GetFromKey("name.joy.fish"));
				sfText_setCharacterSize(pause_RenderText, 29);
				sfText_setPosition(pause_RenderText, vector2f(888.f, 726.f));
				sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
				
			}
			else
			{
				sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 0, 1168, 152, 148 });
				sfSprite_setPosition(spMenuPause, (sfVector2f) { 1076.f, 302.f + 192 });
				sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
				sfText_setColor(pause_RenderText, sfWhite);
				sfText_setString(pause_RenderText, "???");
				sfText_setCharacterSize(pause_RenderText, 19);
				sfText_setPosition(pause_RenderText, vector2f(744.f, 790.f));
				sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
				sfText_setString(pause_RenderText, "???");
				sfText_setCharacterSize(pause_RenderText, 29);
				sfText_setPosition(pause_RenderText, vector2f(888.f, 726.f));
				sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
			}
		}
		else 
		{
			sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 0, 850, 138, 138 });
			sfSprite_setPosition(spMenuPause, (sfVector2f) { 1086.f, 496.f });
			sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
		}
		sfText_setColor(pause_RenderText, sfWhite);
		sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 0, 988, 28, 28 });
		sfSprite_setPosition(spMenuPause, (sfVector2f) { 1190.f, 404.f + 198.f });
		sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
		sprintf(itemNbChar, "%d", STD_LIST_GETDATA(itemList, Itemlist, 10)->nbObject);
		sfText_setString(pause_RenderText, itemNbChar);
		sfText_setCharacterSize(pause_RenderText, 14);
		sfText_setPosition(pause_RenderText, vector2f(1196.f, 408.f + 198.f));
		sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
		if (pause_ItemsDiscovered & (PAUSE_JOY_FISH))
			sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 106 * 2, 1016, 106, 76 });
		else
			sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 106*2, 1016 + 76, 106, 76 });
		sfSprite_setPosition(spMenuPause, (sfVector2f) { 1102.f, 338.f + 194.f });
		sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
		

		if (pauseSelect == 15) {
			sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 0 + 138, 850, 138, 138 });
			sfSprite_setPosition(spMenuPause, (sfVector2f) { 1086.f + 158, 496.f });
			sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
			if (pause_ItemsDiscovered & (PAUSE_SADNESS_FISH))
			{
				sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 0, 1168, 152, 148 });
				sfSprite_setPosition(spMenuPause, (sfVector2f) { 1076.f + 160, 302.f + 192 });
				sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
				sfText_setColor(pause_RenderText, sfWhite);
				sfText_setString(pause_RenderText, lang_GetFromKey("info.sadness.fish"));
				sfText_setCharacterSize(pause_RenderText, 19);
				sfText_setPosition(pause_RenderText, vector2f(744.f, 790.f));
				sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
				sfText_setString(pause_RenderText, lang_GetFromKey("name.sadness.fish"));
				sfText_setCharacterSize(pause_RenderText, 29);
				sfText_setPosition(pause_RenderText, vector2f(888.f, 726.f));
				sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
			}
			else
			{
				sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 0, 1168, 152, 148 });
				sfSprite_setPosition(spMenuPause, (sfVector2f) { 1076.f + 160, 302.f + 192 });
				sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
				sfText_setColor(pause_RenderText, sfWhite);
				sfText_setString(pause_RenderText, "???");
				sfText_setCharacterSize(pause_RenderText, 19);
				sfText_setPosition(pause_RenderText, vector2f(744.f, 790.f));
				sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
				sfText_setString(pause_RenderText, "???");
				sfText_setCharacterSize(pause_RenderText, 29);
				sfText_setPosition(pause_RenderText, vector2f(888.f, 726.f));
				sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
			}
		}
		else
		{
			sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 0, 850, 138, 138 });
			sfSprite_setPosition(spMenuPause, (sfVector2f) { 1086.f + 158, 496.f });
			sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
		}
		sfText_setColor(pause_RenderText, sfWhite);
		sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 0, 988, 28, 28 });
		sfSprite_setPosition(spMenuPause, (sfVector2f) { 1190.f + 160.f, 404.f + 198.f });
		sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
		sprintf(itemNbChar, "%d", STD_LIST_GETDATA(itemList, Itemlist, 11)->nbObject);
		sfText_setString(pause_RenderText, itemNbChar);
		sfText_setCharacterSize(pause_RenderText, 20);
		sfText_setPosition(pause_RenderText, vector2f(1196.f + 160.f, 408.f + 198));
		sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
		if (pause_ItemsDiscovered & (PAUSE_SADNESS_FISH))
			sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 106 * 3, 1016, 106, 76 });
		else
			sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 106 * 3, 1016 + 76, 106, 76 });
		sfSprite_setPosition(spMenuPause, (sfVector2f) { 1102.f + 160.f, 338+194 });
		sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);

	
		if (pauseSelect == 16) {

			sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 0 + 138, 850, 138, 138 });
			sfSprite_setPosition(spMenuPause, (sfVector2f) { 1086.f + (158 * 2), 496.f });
			sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
			if (pause_ItemsDiscovered & (PAUSE_SCARE_FISH))
			{
				sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 0, 1168, 152, 148 });
				sfSprite_setPosition(spMenuPause, (sfVector2f) { 1076.f + (160 * 2), 302.f + 192 });
				sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
				sfText_setColor(pause_RenderText, sfWhite);
				sfText_setString(pause_RenderText, lang_GetFromKey("info.scare.fish"));
				sfText_setCharacterSize(pause_RenderText, 19);
				sfText_setPosition(pause_RenderText, vector2f(744.f, 790.f));
				sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
				sfText_setString(pause_RenderText, lang_GetFromKey("name.scare.fish"));
				sfText_setCharacterSize(pause_RenderText, 29);
				sfText_setPosition(pause_RenderText, vector2f(888.f, 726.f));
				sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
				
			}
			else
			{
				sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 0, 1168, 152, 148 });
				sfSprite_setPosition(spMenuPause, (sfVector2f) { 1076.f + (160 * 2), 302.f + 192 });
				sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
				sfText_setColor(pause_RenderText, sfWhite);
				sfText_setString(pause_RenderText, "???");
				sfText_setCharacterSize(pause_RenderText, 19);
				sfText_setPosition(pause_RenderText, vector2f(744.f, 790.f));
				sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
				sfText_setString(pause_RenderText, "???");
				sfText_setCharacterSize(pause_RenderText, 29);
				sfText_setPosition(pause_RenderText, vector2f(888.f, 726.f));
				sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
			}
		}
		else
		{
			sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 0, 850, 138, 138 });
			sfSprite_setPosition(spMenuPause, (sfVector2f) { 1086.f + (158 * 2), 496.f });
			sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
		}
		sfText_setColor(pause_RenderText, sfWhite);
		sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 0, 988, 28, 28 });
		sfSprite_setPosition(spMenuPause, (sfVector2f) { 1190.f + (160.f * 2), 404.f + 198.f });
		sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
		sprintf(itemNbChar, "%d", STD_LIST_GETDATA(itemList, Itemlist, 12)->nbObject);
		sfText_setString(pause_RenderText, itemNbChar);
		sfText_setCharacterSize(pause_RenderText, 20);
		sfText_setPosition(pause_RenderText, vector2f(1196.f + (160.f * 2), 408.f + 198));
		sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
		if (pause_ItemsDiscovered & (PAUSE_SCARE_FISH))
			sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 106 * 4, 1016, 106, 76 });
		else
			sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 106 * 4, 1016 + 76, 106, 76 });
		sfSprite_setPosition(spMenuPause, (sfVector2f) { 1102.f + 160.f * 2, 338 + 194 });
		sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
		
		if (pauseSelect == 17) {
			sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 0 + 138, 850, 138, 138 });
			sfSprite_setPosition(spMenuPause, (sfVector2f) { 1086.f + (158 * 3), 496.f });
			sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
			if (pause_ItemsDiscovered & (PAUSE_ANGER_FISH))
			{
				sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 0, 1168, 152, 148 });
				sfSprite_setPosition(spMenuPause, (sfVector2f) { 1076.f + (160 * 3), 302.f + 192 });
				sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
				sfText_setColor(pause_RenderText, sfWhite);
				sfText_setString(pause_RenderText, lang_GetFromKey("info.anger.fish"));
				sfText_setCharacterSize(pause_RenderText, 19);
				sfText_setPosition(pause_RenderText, vector2f(744.f, 790.f));
				sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
				sfText_setString(pause_RenderText, lang_GetFromKey("name.anger.fish"));
				sfText_setCharacterSize(pause_RenderText, 29);
				sfText_setPosition(pause_RenderText, vector2f(888.f, 726.f));
				sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
				
			}
			else
			{
				sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 0, 1168, 152, 148 });
				sfSprite_setPosition(spMenuPause, (sfVector2f) { 1076.f + (160 * 3), 302.f + 192 });
				sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
				sfText_setColor(pause_RenderText, sfWhite);
				sfText_setString(pause_RenderText, "???");
				sfText_setCharacterSize(pause_RenderText, 19);
				sfText_setPosition(pause_RenderText, vector2f(744.f, 790.f));
				sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
				sfText_setString(pause_RenderText, "???");
				sfText_setCharacterSize(pause_RenderText, 29);
				sfText_setPosition(pause_RenderText, vector2f(888.f, 726.f));
				sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
			}
		}
		else {
			sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 0, 850, 138, 138 });
			sfSprite_setPosition(spMenuPause, (sfVector2f) { 1086.f + (158 * 3), 496.f });
			sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
		}
		sfText_setColor(pause_RenderText, sfWhite);
		sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 0, 988, 28, 28 });
		sfSprite_setPosition(spMenuPause, (sfVector2f) { 1190.f + (160.f * 3), 404.f + 198.f });
		sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
		sprintf(itemNbChar, "%d", STD_LIST_GETDATA(itemList, Itemlist, 13)->nbObject);
		sfText_setString(pause_RenderText, itemNbChar);
		sfText_setCharacterSize(pause_RenderText, 20);
		sfText_setPosition(pause_RenderText, vector2f(1196.f + (160.f * 3), 408.f + 198));
		sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
		if (pause_ItemsDiscovered & (PAUSE_ANGER_FISH))
			sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 106 * 5, 1016, 106, 76 });
		else
			sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 106 * 5, 1016 + 76, 106, 76 });
		sfSprite_setPosition(spMenuPause, (sfVector2f) { 1102.f + 160.f * 3, 338 + 194 });
		sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);

		if (pauseSelect == 10) {
			sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 0 + 138, 850, 138, 138 });
			sfSprite_setPosition(spMenuPause, (sfVector2f) { 1086.f + (158 * 0), 306.f });
			sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
			if (pause_ItemsDiscovered & (PAUSE_YOYO))
			{
				sfText_setColor(pause_RenderText, sfWhite);
				sfText_setString(pause_RenderText, lang_GetFromKey("info.yoyo"));
				sfText_setCharacterSize(pause_RenderText, 19);
				sfText_setPosition(pause_RenderText, vector2f(744.f, 790.f));
				sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
				sfText_setString(pause_RenderText, lang_GetFromKey("name.yoyo"));
				sfText_setCharacterSize(pause_RenderText, 29);
				sfText_setPosition(pause_RenderText, vector2f(888.f, 726.f));
				sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);;
			}
			else
			{
				sfText_setColor(pause_RenderText, sfWhite);
				sfText_setString(pause_RenderText, "???");
				sfText_setCharacterSize(pause_RenderText, 19);
				sfText_setPosition(pause_RenderText, vector2f(744.f, 790.f));
				sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
				sfText_setString(pause_RenderText, "???");
				sfText_setCharacterSize(pause_RenderText, 29);
				sfText_setPosition(pause_RenderText, vector2f(888.f, 726.f));
				sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
			}
			sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 0, 1168, 152, 148 });
			sfSprite_setPosition(spMenuPause, (sfVector2f) { 1076.f + (160 * 0), 302.f });
			sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
		}
		else {
			sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 0, 850, 138, 138 });
			sfSprite_setPosition(spMenuPause, (sfVector2f) { 1086.f + (158 * 0), 306.f });
			sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
		}
		sfText_setColor(pause_RenderText, sfWhite);
		sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 0, 988, 28, 28 });
		sfSprite_setPosition(spMenuPause, (sfVector2f) { 1190.f + (160.f * 0), 404.f });
		sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
		sprintf(itemNbChar, "%d", STD_LIST_GETDATA(itemList, Itemlist, 14)->nbObject);
		sfText_setString(pause_RenderText, itemNbChar);
		sfText_setCharacterSize(pause_RenderText, 20);
		sfText_setPosition(pause_RenderText, vector2f(1196.f + (160.f * 0), 408.f));
		sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
		if (pause_ItemsDiscovered & (PAUSE_YOYO))
			sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 0, 1016, 106, 76 });
		else
			sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 0, 1016 + 76, 106, 76 });
		sfSprite_setPosition(spMenuPause, (sfVector2f) { 1102.f, 338 });
		sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);

		if (pauseSelect == 11) {
			sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 0 + 138, 850, 138, 138 });
			sfSprite_setPosition(spMenuPause, (sfVector2f) { 1086.f + (158 * 1), 306.f });
			sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
			if (pause_ItemsDiscovered & (PAUSE_CABORALS))
			{
				sfText_setColor(pause_RenderText, sfWhite);
				sfText_setString(pause_RenderText, lang_GetFromKey("info.craborail"));
				sfText_setCharacterSize(pause_RenderText, 19);
				sfText_setPosition(pause_RenderText, vector2f(744.f, 790.f));
				sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
				sfText_setString(pause_RenderText, lang_GetFromKey("name.craborail"));
				sfText_setCharacterSize(pause_RenderText, 29);
				sfText_setPosition(pause_RenderText, vector2f(888.f, 726.f));
				sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
			}
			else
			{
				sfText_setColor(pause_RenderText, sfWhite);
				sfText_setString(pause_RenderText, "???");
				sfText_setCharacterSize(pause_RenderText, 19);
				sfText_setPosition(pause_RenderText, vector2f(744.f, 790.f));
				sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
				sfText_setString(pause_RenderText, "???");
				sfText_setCharacterSize(pause_RenderText, 29);
				sfText_setPosition(pause_RenderText, vector2f(888.f, 726.f));
				sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
			}
			sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 0, 1168, 152, 148 });
			sfSprite_setPosition(spMenuPause, (sfVector2f) { 1076.f + (160 * 1), 302.f });
			sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
		}
		else {
			sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 0, 850, 138, 138 });
			sfSprite_setPosition(spMenuPause, (sfVector2f) { 1086.f + (158 * 1), 306.f });
			sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
		}
		sfText_setColor(pause_RenderText, sfWhite);
		sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 0, 988, 28, 28 });
		sfSprite_setPosition(spMenuPause, (sfVector2f) { 1190.f + (160.f * 1), 404.f });
		sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
		sprintf(itemNbChar, "%d", STD_LIST_GETDATA(itemList, Itemlist, 15)->nbObject);
		sfText_setString(pause_RenderText, itemNbChar);
		sfText_setCharacterSize(pause_RenderText, 20);
		sfText_setPosition(pause_RenderText, vector2f(1196.f + (160.f * 1), 408.f));
		sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
		if (pause_ItemsDiscovered & (PAUSE_CABORALS))
			sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 106, 1016, 106, 76 });
		else
			sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 106, 1016 + 76, 106, 76 });
		sfSprite_setPosition(spMenuPause, (sfVector2f) { 1102.f + 160.f, 338 });
		sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);

		if (pauseSelect == 12) {
			sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 0 + 138, 850, 138, 138 });
			sfSprite_setPosition(spMenuPause, (sfVector2f) { 1086.f + (158 * 2), 306.f });
			sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
			if (pause_ItemsDiscovered & (PAUSE_SANDAMANDER))
			{
				sfText_setColor(pause_RenderText, sfWhite);
				sfText_setString(pause_RenderText, lang_GetFromKey("info.sablamandre"));
				sfText_setCharacterSize(pause_RenderText, 19);
				sfText_setPosition(pause_RenderText, vector2f(744.f, 790.f));
				sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
				sfText_setString(pause_RenderText, lang_GetFromKey("name.sablamandre"));
				sfText_setCharacterSize(pause_RenderText, 29);
				sfText_setPosition(pause_RenderText, vector2f(888.f, 726.f));
				sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
			}
			else
			{
				sfText_setColor(pause_RenderText, sfWhite);
				sfText_setString(pause_RenderText, "???");
				sfText_setCharacterSize(pause_RenderText, 19);
				sfText_setPosition(pause_RenderText, vector2f(744.f, 790.f));
				sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
				sfText_setString(pause_RenderText, "???");
				sfText_setCharacterSize(pause_RenderText, 29);
				sfText_setPosition(pause_RenderText, vector2f(888.f, 726.f));
				sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
			}
			sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 0, 1168, 152, 148 });
			sfSprite_setPosition(spMenuPause, (sfVector2f) { 1076.f + (160 * 2), 302.f });
			sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
		}
		else {
			sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 0, 850, 138, 138 });
			sfSprite_setPosition(spMenuPause, (sfVector2f) { 1086.f + (158 * 2), 306.f });
			sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
		}
		sfText_setColor(pause_RenderText, sfWhite);
		sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 0, 988, 28, 28 });
		sfSprite_setPosition(spMenuPause, (sfVector2f) { 1190.f + (160.f * 2), 404.f });
		sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
		sprintf(itemNbChar, "%d", STD_LIST_GETDATA(itemList, Itemlist, 16)->nbObject);
		sfText_setString(pause_RenderText, itemNbChar);
		sfText_setCharacterSize(pause_RenderText, 20);
		sfText_setPosition(pause_RenderText, vector2f(1196.f + (160.f * 2), 408.f));
		sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
		if (pause_ItemsDiscovered & (PAUSE_SANDAMANDER))
			sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 1400, 5526, 76, 76 });
		else
			sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 1400, 5646, 76, 76 });
		sfSprite_setPosition(spMenuPause, (sfVector2f) { 1102.f  + 160.f * 2, 338 });
		sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);

		if (pauseSelect == 13) {
			sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 0 + 138, 850, 138, 138 });
			sfSprite_setPosition(spMenuPause, (sfVector2f) { 1086.f + (158 * 3), 306.f });
			sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
			if (pause_ItemsDiscovered & (PAUSE_VOLTIGEONS))
			{
				sfText_setColor(pause_RenderText, sfWhite);
				sfText_setString(pause_RenderText, lang_GetFromKey("info.voltigeon"));
				sfText_setCharacterSize(pause_RenderText, 19);
				sfText_setPosition(pause_RenderText, vector2f(744.f, 790.f));
				sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
				sfText_setString(pause_RenderText, lang_GetFromKey("name.voltigeon"));
				sfText_setCharacterSize(pause_RenderText, 29);
				sfText_setPosition(pause_RenderText, vector2f(888.f, 726.f));
				sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
			}
			else
			{
				sfText_setColor(pause_RenderText, sfWhite);
				sfText_setString(pause_RenderText, "???");
				sfText_setCharacterSize(pause_RenderText, 19);
				sfText_setPosition(pause_RenderText, vector2f(744.f, 790.f));
				sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
				sfText_setString(pause_RenderText, "???");
				sfText_setCharacterSize(pause_RenderText, 29);
				sfText_setPosition(pause_RenderText, vector2f(888.f, 726.f));
				sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
			}
			sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 0, 1168, 152, 148 });
			sfSprite_setPosition(spMenuPause, (sfVector2f) { 1076.f + (160 * 3), 302.f });
			sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
		}
		else {
			sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 0, 850, 138, 138 });
			sfSprite_setPosition(spMenuPause, (sfVector2f) { 1086.f + (158 * 3), 306.f });
			sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
		}
		sfText_setColor(pause_RenderText, sfWhite);
		sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 0, 988, 28, 28 });
		sfSprite_setPosition(spMenuPause, (sfVector2f) { 1190.f + (160.f * 3), 404.f });
		sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
		sprintf(itemNbChar, "%d", STD_LIST_GETDATA(itemList, Itemlist, 17)->nbObject);
		sfText_setString(pause_RenderText, itemNbChar);
		sfText_setCharacterSize(pause_RenderText, 20);
		sfText_setPosition(pause_RenderText, vector2f(1196.f + (160.f * 3), 408.f));
		sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
		if (pause_ItemsDiscovered & (PAUSE_VOLTIGEONS))
			sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 712, 5530, 80, 68 });
		else
			sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 712, 5650, 80, 68 });
		sfSprite_setPosition(spMenuPause, (sfVector2f) { 1102.f + (160.f * 3) + 20.f, 338 });
		sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
		break; 
	case PAUSE_BESTIARE:
		sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 0, 3084, 1644, 850 });
		sfSprite_setPosition(spMenuPause, (sfVector2f) { 136.f, 170.f });
		sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);

		sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 0, 5984, 82, 93 });
		sfSprite_setPosition(spMenuPause, (sfVector2f) { 778.f, 90.f });
		sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);

		sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 82, 5984, 82, 96 });
		sfSprite_setPosition(spMenuPause, (sfVector2f) { 778.f + 100, 90.f });
		sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);

		sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 82 * 2, 6076, 82, 96 });
		sfSprite_setPosition(spMenuPause, (sfVector2f) { 778.f + 200, 64.f });
		sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);

		sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 82 * 3, 5984, 82, 96 });
		sfSprite_setPosition(spMenuPause, (sfVector2f) { 778.f + 300, 90.f });
		sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
		
		if (!isSelectedBestiare)
		{
			if (pauseSelect == 0) {

				sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 0, 5238, 104, 100 });
				sfSprite_setPosition(spMenuPause, (sfVector2f) {488.f, 218.f });
				sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);

				sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 0, 5338, 162, 162 });
				sfSprite_setPosition(spMenuPause, (sfVector2f) { 320.f, 426.f });
				sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
				if (pause_ItemsDiscovered & (PAUSE_YOYO))
					sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 100 * 8, 5508, 100, 120 });
				else
					sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 100 * 8, 5508 + 120, 100, 120 });
				sfSprite_setPosition(spMenuPause, (sfVector2f) { 354.f, 448.f });
				sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);

				sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 0, 5338, 162, 162 });
				sfSprite_setPosition(spMenuPause, (sfVector2f) { 320.f + 228.f, 426.f });
				sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
				if (pause_ItemsDiscovered & (PAUSE_CABORALS))
					sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 100 * 11, 5508, 100, 120 });
				else
					sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 100 * 11, 5508 + 120, 100, 120 });
				sfSprite_setPosition(spMenuPause, (sfVector2f) { 354.f + 228.f, 448.f });
				sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);

				sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 0, 5338, 162, 162 });
				sfSprite_setPosition(spMenuPause, (sfVector2f) { 320.f + 228.f * 2, 426.f });
				sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
				if (pause_ItemsDiscovered & (PAUSE_SANDAMANDER))
					sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 100 * 14, 5508, 100, 120 });
				else
					sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 100 * 14, 5508 + 120, 100, 120 });
				sfSprite_setPosition(spMenuPause, (sfVector2f) { 354.f + 228.f * 2, 448.f });
				sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);

				sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 0, 5338, 162, 162 });
				sfSprite_setPosition(spMenuPause, (sfVector2f) { 320.f, 426.f + 228.f });
				sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
				if (pause_ItemsDiscovered & (PAUSE_VOLTIGEONS))
					sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 100 * 7, 5508, 100, 120 });
				else
					sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 100 * 7, 5650, 100, 120 });
				sfSprite_setPosition(spMenuPause, (sfVector2f) { 354.f, 448.f + 228 });
				sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);

				sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 0, 5338, 162, 162 });
				sfSprite_setPosition(spMenuPause, (sfVector2f) { 320.f + 228.f, 426.f + 228 });
				sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
				if (pause_EnnemisDiscovered & (PAUSE_POISON_SPIDER))
					sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 100 * 9, 5508, 100, 120 });
				else
					sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 100 * 9, 5508 + 120, 100, 120 });
				sfSprite_setPosition(spMenuPause, (sfVector2f) { 354.f + 228.f, 448.f + 228 });
				sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);

				sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 0, 5338, 162, 162 });
				sfSprite_setPosition(spMenuPause, (sfVector2f) { 320.f + 228.f * 2, 426.f + 228.f });
				sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
				if (pause_EnnemisDiscovered & (PAUSE_SNAKE))
					sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 100 * 4, 5508, 100, 120 });
				else
					sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 100 * 4, 5508 + 120, 100, 120 });
				sfSprite_setPosition(spMenuPause, (sfVector2f) { 354.f + 228.f * 2, 448.f + 228.f });
				sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
			}
			else
			{
				sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 0, 5140, 104, 100 });
				sfSprite_setPosition(spMenuPause, (sfVector2f) { 488.f, 274.f });
				sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
			}
 			if (pauseSelect == 1) {
				sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 104, 5238, 104, 97 });
				sfSprite_setPosition(spMenuPause, (sfVector2f) { 488.f + 184.f, 218.f });
				sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);

				sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 0, 5338, 162, 162 });
				sfSprite_setPosition(spMenuPause, (sfVector2f) { 320.f, 426.f });
				sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
				if (pause_EnnemisDiscovered & (PAUSE_LITTLE_JOY_DEMON))
					sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 100 * 4, 5740, 100, 120 });
				else
					sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 100 * 4, 5740 + 120, 100, 120 });
				sfSprite_setPosition(spMenuPause, (sfVector2f) { 354.f, 448.f });
				sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);

				sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 0, 5338, 162, 162 });
				sfSprite_setPosition(spMenuPause, (sfVector2f) { 320.f + 228.f, 426.f });
				sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
				if (pause_EnnemisDiscovered & (PAUSE_BIG_JOY_DEMON))
					sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 100 * 1, 5740, 100, 120 });
				else
					sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 100 * 1, 5740 + 120, 100, 120 });
				sfSprite_setPosition(spMenuPause, (sfVector2f) { 354.f + 228.f, 448.f });
				sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);

				sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 0, 5338, 162, 162 });
				sfSprite_setPosition(spMenuPause, (sfVector2f) { 320.f + 228.f * 2, 426.f });
				sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
				if (pause_EnnemisDiscovered & (PAUSE_JOY_GOLEM))
					sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 100 * 6, 5740, 100, 120 });
				else
					sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 100 * 6, 5740 + 120, 100, 120 });
				sfSprite_setPosition(spMenuPause, (sfVector2f) { 354.f + 228.f * 2, 448.f });
				sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
			}
			else {
				sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 104, 5140, 104, 97 });
				sfSprite_setPosition(spMenuPause, (sfVector2f) { 488.f + 184,  274.f });
				sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
			}
		}
		else 
		{
			sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 0, 5140, 104, 100 });
			sfSprite_setPosition(spMenuPause, (sfVector2f) { 488.f, 274.f });
			sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
			sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 104, 5140, 104, 97 });
			sfSprite_setPosition(spMenuPause, (sfVector2f) { 488.f + 184, 274.f });
			sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
			switch (bestiare)
			{
			case PAUSE_PET:
				if (yPauseSelect >= 0 && yPauseSelect <= 1) {
					if (pauseSelect == 0) {
						if (yPauseSelect == 0) {
							sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 162, 5338, 162, 162 });
							sfSprite_setPosition(spMenuPause, (sfVector2f) { 320.f, 426.f });
							sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
							if (pause_ItemsDiscovered & (PAUSE_YOYO))
							{
								sfText_setColor(pause_RenderText, sfWhite);
								sfText_setString(pause_RenderText, lang_GetFromKey("info.yoyo"));
								sfText_setCharacterSize(pause_RenderText, 19);
								sfText_setPosition(pause_RenderText, vector2f(1104.f, 378.f));
								sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
								sfText_setString(pause_RenderText, lang_GetFromKey("name.yoyo"));
								sfText_setCharacterSize(pause_RenderText, 29);
								sfText_setPosition(pause_RenderText, vector2f(1210.f, 278.f));
								sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);;
							}
							else
							{
								sfText_setColor(pause_RenderText, sfWhite);
								sfText_setString(pause_RenderText, "???");
								sfText_setCharacterSize(pause_RenderText, 19);
								sfText_setPosition(pause_RenderText, vector2f(1104.f, 378.f));
								sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
								sfText_setString(pause_RenderText, "???");
								sfText_setCharacterSize(pause_RenderText, 29);
								sfText_setPosition(pause_RenderText, vector2f(1210.f, 278.f));
								sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
							}
						}
						else
						{
							sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 0, 5338, 162, 162 });
							sfSprite_setPosition(spMenuPause, (sfVector2f) { 320.f, 426.f });
							sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
						}
					}
					else
					{
						sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 0, 5338, 162, 162 });
						sfSprite_setPosition(spMenuPause, (sfVector2f) { 320.f, 426.f });
						sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
					}
					if (pause_ItemsDiscovered & (PAUSE_YOYO))
						sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 100 * 8, 5508, 100, 120 });
					else
						sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 100 * 8, 5508 + 120, 100, 120 });
					sfSprite_setPosition(spMenuPause, (sfVector2f) { 354.f, 448.f });
					sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);

					if (pauseSelect == 1) {
						if (yPauseSelect == 0) {
							sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 162, 5338, 162, 162 });
							sfSprite_setPosition(spMenuPause, (sfVector2f) { 320.f + 228.f, 426.f });
							sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
							if (pause_ItemsDiscovered & (PAUSE_CABORALS))
							{
								sfText_setColor(pause_RenderText, sfWhite);
								sfText_setString(pause_RenderText, lang_GetFromKey("info.craborail"));
								sfText_setCharacterSize(pause_RenderText, 19);
								sfText_setPosition(pause_RenderText, vector2f(1104.f, 378.f));
								sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
								sfText_setString(pause_RenderText, lang_GetFromKey("name.craborail"));
								sfText_setCharacterSize(pause_RenderText, 29);
								sfText_setPosition(pause_RenderText, vector2f(1210.f, 278.f));
								sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
							}
							else
							{
								sfText_setColor(pause_RenderText, sfWhite);
								sfText_setString(pause_RenderText, "???");
								sfText_setCharacterSize(pause_RenderText, 19);
								sfText_setPosition(pause_RenderText, vector2f(1104.f, 378.f));
								sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
								sfText_setString(pause_RenderText, "???");
								sfText_setCharacterSize(pause_RenderText, 29);
								sfText_setPosition(pause_RenderText, vector2f(1210.f, 278.f));
								sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
							}
						}
						else
						{
							sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 0, 5338, 162, 162 });
							sfSprite_setPosition(spMenuPause, (sfVector2f) { 320.f + 228.f, 426.f });
							sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
						}
					}
					else
					{
						sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 0, 5338, 162, 162 });
						sfSprite_setPosition(spMenuPause, (sfVector2f) { 320.f + 228.f, 426.f });
						sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
					}
					if (pause_ItemsDiscovered & (PAUSE_CABORALS))
						sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 100 * 11, 5508, 100, 120 });
					else
						sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 100 * 11, 5508 + 120, 100, 120 });
					sfSprite_setPosition(spMenuPause, (sfVector2f) { 354.f + 228.f, 448.f });
					sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);


					if (pauseSelect == 2) {
						if (yPauseSelect == 0) {
							sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 162, 5338, 162, 162 });
							sfSprite_setPosition(spMenuPause, (sfVector2f) { 320.f + 228.f * 2, 426.f });
							sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
							if (pause_ItemsDiscovered & (PAUSE_SANDAMANDER))
							{
								sfText_setColor(pause_RenderText, sfWhite);
								sfText_setString(pause_RenderText, lang_GetFromKey("info.sablamandre"));
								sfText_setCharacterSize(pause_RenderText, 19);
								sfText_setPosition(pause_RenderText, vector2f(1104.f, 378.f));
								sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
								sfText_setString(pause_RenderText, lang_GetFromKey("name.sablamandre"));
								sfText_setCharacterSize(pause_RenderText, 29);
								sfText_setPosition(pause_RenderText, vector2f(1210.f, 278.f));
								sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
							}
							else
							{
								sfText_setColor(pause_RenderText, sfWhite);
								sfText_setString(pause_RenderText, "???");
								sfText_setCharacterSize(pause_RenderText, 19);
								sfText_setPosition(pause_RenderText, vector2f(1104.f, 378.f));
								sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
								sfText_setString(pause_RenderText, "???");
								sfText_setCharacterSize(pause_RenderText, 29);
								sfText_setPosition(pause_RenderText, vector2f(1210.f, 278.f));
								sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
							}
						}
						else
						{
							sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 0, 5338, 162, 162 });
							sfSprite_setPosition(spMenuPause, (sfVector2f) { 320.f + 228.f * 2, 426.f });
							sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
						}
					}
					else
					{
						sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 0, 5338, 162, 162 });
						sfSprite_setPosition(spMenuPause, (sfVector2f) { 320.f + 228.f * 2, 426.f });
						sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
					}
					if (pause_ItemsDiscovered & (PAUSE_SANDAMANDER))
						sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 100 * 14, 5508, 100, 120 });
					else
						sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 100 * 14, 5508 + 120, 100, 120 });
					sfSprite_setPosition(spMenuPause, (sfVector2f) { 354.f + 228.f * 2, 448.f });
					sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);

					if (pauseSelect == 0) {
						if (yPauseSelect == 1) {
							sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 162, 5338, 162, 162 });
							sfSprite_setPosition(spMenuPause, (sfVector2f) { 320.f, 426.f + 228.f });
							sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
							if (pause_ItemsDiscovered & (PAUSE_VOLTIGEONS))
							{
								sfText_setColor(pause_RenderText, sfWhite);
								sfText_setString(pause_RenderText, lang_GetFromKey("info.voltigeon"));
								sfText_setCharacterSize(pause_RenderText, 19);
								sfText_setPosition(pause_RenderText, vector2f(1104.f, 378.f));
								sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
								sfText_setString(pause_RenderText, lang_GetFromKey("name.voltigeon"));
								sfText_setCharacterSize(pause_RenderText, 29);
								sfText_setPosition(pause_RenderText, vector2f(1210.f, 278.f));
								sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
							}
							else
							{
								sfText_setColor(pause_RenderText, sfWhite);
								sfText_setString(pause_RenderText, "???");
								sfText_setCharacterSize(pause_RenderText, 19);
								sfText_setPosition(pause_RenderText, vector2f(1104.f, 378.f));
								sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
								sfText_setString(pause_RenderText, "???");
								sfText_setCharacterSize(pause_RenderText, 29);
								sfText_setPosition(pause_RenderText, vector2f(1210.f, 278.f));
								sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
							}
						}
						else
						{
							sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 0, 5338, 162, 162 });
							sfSprite_setPosition(spMenuPause, (sfVector2f) { 320.f, 426.f + 228.f });
							sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
						}
					}
					else
					{
						sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 0, 5338, 162, 162 });
						sfSprite_setPosition(spMenuPause, (sfVector2f) { 320.f, 426.f + 228.f });
						sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
					}
					if (pause_ItemsDiscovered & (PAUSE_VOLTIGEONS))
						sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 100*7, 5508, 100, 120 });
					else
						sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 100*7, 5650, 100, 120 });
					sfSprite_setPosition(spMenuPause, (sfVector2f) { 354.f, 448.f + 228});
					sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);

					if (pauseSelect == 1) {
						if (yPauseSelect == 1) {
							sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 162, 5338, 162, 162 });
							sfSprite_setPosition(spMenuPause, (sfVector2f) { 320.f + 228.f, 426.f + 228.f});
							sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
							if (pause_EnnemisDiscovered & (PAUSE_POISON_SPIDER))
							{
								sfText_setColor(pause_RenderText, sfWhite);
								sfText_setString(pause_RenderText, lang_GetFromKey("info.poisonous.spider"));
								sfText_setCharacterSize(pause_RenderText, 18);
								sfText_setPosition(pause_RenderText, vector2f(1104.f, 378.f));
								sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
								sfText_setString(pause_RenderText, lang_GetFromKey("name.poisonous.spider"));
								sfText_setCharacterSize(pause_RenderText, 29);
								sfText_setPosition(pause_RenderText, vector2f(1210.f, 278.f));
								sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
							}
							else
							{
								sfText_setColor(pause_RenderText, sfWhite);
								sfText_setString(pause_RenderText, "???");
								sfText_setCharacterSize(pause_RenderText, 18);	
								sfText_setPosition(pause_RenderText, vector2f(1104.f, 378.f));
								sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
								sfText_setString(pause_RenderText, "???");
								sfText_setCharacterSize(pause_RenderText, 29);
								sfText_setPosition(pause_RenderText, vector2f(1210.f, 278.f));
								sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
							}
						}
						else
						{
							sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 0, 5338, 162, 162 });
							sfSprite_setPosition(spMenuPause, (sfVector2f) { 320.f + 228.f, 426.f + 228 });
							sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
						}
					}
					else
					{
						sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 0, 5338, 162, 162 });
						sfSprite_setPosition(spMenuPause, (sfVector2f) { 320.f + 228.f, 426.f + 228 });
						sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
					}
					if (pause_EnnemisDiscovered & (PAUSE_POISON_SPIDER))
						sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 100 * 9, 5508, 100, 120 });
					else
						sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 100 * 9, 5508 + 120, 100, 120 });
					sfSprite_setPosition(spMenuPause, (sfVector2f) { 354.f + 228.f, 448.f + 228 });
					sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);

					if (pauseSelect == 2) {
						if (yPauseSelect == 1) {
							sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 162, 5338, 162, 162 });
							sfSprite_setPosition(spMenuPause, (sfVector2f) { 320.f + 228.f * 2, 426.f + 228.f });
							sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
							if (pause_EnnemisDiscovered & (PAUSE_SNAKE))
							{
								sfText_setColor(pause_RenderText, sfWhite);
								sfText_setString(pause_RenderText, lang_GetFromKey("info.snake"));
								sfText_setCharacterSize(pause_RenderText, 18);
								sfText_setPosition(pause_RenderText, vector2f(1104.f, 378.f));
								sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
								sfText_setString(pause_RenderText, lang_GetFromKey("name.snake"));
								sfText_setCharacterSize(pause_RenderText, 29);
								sfText_setPosition(pause_RenderText, vector2f(1210.f, 278.f));
								sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
							}
							else
							{
								sfText_setColor(pause_RenderText, sfWhite);
								sfText_setString(pause_RenderText, "???");
								sfText_setCharacterSize(pause_RenderText, 18);
								sfText_setPosition(pause_RenderText, vector2f(1104.f, 378.f));
								sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
								sfText_setString(pause_RenderText, "???");
								sfText_setCharacterSize(pause_RenderText, 29);
								sfText_setPosition(pause_RenderText, vector2f(1210.f, 278.f));
								sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
							}
						}
						else
						{
							sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 0, 5338, 162, 162 });
							sfSprite_setPosition(spMenuPause, (sfVector2f) { 320.f + 228.f * 2, 426.f + 228.f });
							sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
						}
					}
					else
					{
						sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 0, 5338, 162, 162 });
						sfSprite_setPosition(spMenuPause, (sfVector2f) { 320.f + 228.f * 2, 426.f + 228.f });
						sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
					}
					if (pause_EnnemisDiscovered & (PAUSE_SNAKE))
						sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 100 * 4, 5508, 100, 120 });
					else
						sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 100 * 4, 5508 + 120, 100, 120 });
					sfSprite_setPosition(spMenuPause, (sfVector2f) { 354.f + 228.f * 2, 448.f + 228.f });
					sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
				}

				if (yPauseSelect >= 2 && yPauseSelect <= 3) {
					if (pauseSelect == 0) {
						if (yPauseSelect == 2) {
							sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 162, 5338, 162, 162 });
							sfSprite_setPosition(spMenuPause, (sfVector2f) { 320.f, 426.f});
							sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
							if (pause_EnnemisDiscovered & (PAUSE_MOSQUITO))
							{
								sfText_setColor(pause_RenderText, sfWhite);
								sfText_setString(pause_RenderText, lang_GetFromKey("info.mosquito"));
								sfText_setCharacterSize(pause_RenderText, 18);
								sfText_setPosition(pause_RenderText, vector2f(1104.f, 378.f));
								sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
								sfText_setString(pause_RenderText, lang_GetFromKey("name.mosquito"));
								sfText_setCharacterSize(pause_RenderText, 29);
								sfText_setPosition(pause_RenderText, vector2f(1210.f, 278.f));
								sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
							}
							else
							{
								sfText_setColor(pause_RenderText, sfWhite);
								sfText_setString(pause_RenderText, "???");
								sfText_setCharacterSize(pause_RenderText, 18);
								sfText_setPosition(pause_RenderText, vector2f(1104.f, 378.f));
								sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
								sfText_setString(pause_RenderText, "???");
								sfText_setCharacterSize(pause_RenderText, 29);
								sfText_setPosition(pause_RenderText, vector2f(1210.f, 278.f));
								sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
							}

						}
						else
						{
							sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 0, 5338, 162, 162 });
							sfSprite_setPosition(spMenuPause, (sfVector2f) { 320.f, 426.f});
							sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
						}
					}
					else
					{
						sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 0, 5338, 162, 162 });
						sfSprite_setPosition(spMenuPause, (sfVector2f) { 320.f, 426.f });
						sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
					}
					if (pause_EnnemisDiscovered & (PAUSE_MOSQUITO))
						sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 100 * 1, 5508, 100, 113 });
					else
						sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 100 * 1, 5508 + 120, 100, 113 });
					sfSprite_setPosition(spMenuPause, (sfVector2f) { 354.f, 448.f});
					sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);

					if (pauseSelect == 1) {
						if (yPauseSelect == 2) {
							sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 162, 5338, 162, 162 });
							sfSprite_setPosition(spMenuPause, (sfVector2f) { 320.f + 228.f, 426.f });
							sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
							if (pause_EnnemisDiscovered & (PAUSE_SEAGULL))
							{
								sfText_setColor(pause_RenderText, sfWhite);
								sfText_setString(pause_RenderText, lang_GetFromKey("info.seagull"));
								sfText_setCharacterSize(pause_RenderText, 18);
								sfText_setPosition(pause_RenderText, vector2f(1104.f, 378.f));
								sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
								sfText_setString(pause_RenderText, lang_GetFromKey("name.seagull"));
								sfText_setCharacterSize(pause_RenderText, 29);
								sfText_setPosition(pause_RenderText, vector2f(1210.f, 278.f));
								sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
							}
							else
							{
								sfText_setColor(pause_RenderText, sfWhite);
								sfText_setString(pause_RenderText, "???");
								sfText_setCharacterSize(pause_RenderText, 18);
								sfText_setPosition(pause_RenderText, vector2f(1104.f, 378.f));
								sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
								sfText_setString(pause_RenderText, "???");
								sfText_setCharacterSize(pause_RenderText, 29);
								sfText_setPosition(pause_RenderText, vector2f(1210.f, 278.f));
								sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
							}
						}
						else
						{
							sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 0, 5338, 162, 162 });
							sfSprite_setPosition(spMenuPause, (sfVector2f) { 320.f + 228.f, 426.f });
							sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
						}
					}
					else
					{
						sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 0, 5338, 162, 162 });
						sfSprite_setPosition(spMenuPause, (sfVector2f) { 320.f + 228.f, 426.f });
						sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
					}
					if (pause_EnnemisDiscovered & (PAUSE_SEAGULL))
						sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 100 * 0, 5508, 100, 118 });
					else
						sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 100 * 0, 5508 + 120, 100, 118 });
					sfSprite_setPosition(spMenuPause, (sfVector2f) { 354.f + 228.f, 448.f });
					sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);

					if (pauseSelect == 2) {
						if (yPauseSelect == 2) {
							sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 162, 5338, 162, 162 });
							sfSprite_setPosition(spMenuPause, (sfVector2f) { 320.f + 228.f * 2, 426.f });
							sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
							if (pause_EnnemisDiscovered & (PAUSE_CENTIPEDE))
							{
								sfText_setColor(pause_RenderText, sfWhite);
								sfText_setString(pause_RenderText, lang_GetFromKey("info.centipede"));
								sfText_setCharacterSize(pause_RenderText, 18);
								sfText_setPosition(pause_RenderText, vector2f(1104.f, 378.f));
								sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
								sfText_setString(pause_RenderText, lang_GetFromKey("name.centipede"));
								sfText_setCharacterSize(pause_RenderText, 29);
								sfText_setPosition(pause_RenderText, vector2f(1210.f, 278.f));
								sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
							}
							else
							{
								sfText_setColor(pause_RenderText, sfWhite);
								sfText_setString(pause_RenderText, "???");
								sfText_setCharacterSize(pause_RenderText, 18);
								sfText_setPosition(pause_RenderText, vector2f(1104.f, 378.f));
								sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
								sfText_setString(pause_RenderText, "???");
								sfText_setCharacterSize(pause_RenderText, 29);
								sfText_setPosition(pause_RenderText, vector2f(1210.f, 278.f));
								sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
							}
						}
						else
						{
							sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 0, 5338, 162, 162 });
							sfSprite_setPosition(spMenuPause, (sfVector2f) { 320.f + 228.f * 2, 426.f });
							sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
						}
					}
					else
					{
						sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 0, 5338, 162, 162 });
						sfSprite_setPosition(spMenuPause, (sfVector2f) { 320.f + 228.f * 2, 426.f });
						sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
					}
					if (pause_EnnemisDiscovered & (PAUSE_CENTIPEDE))
						sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 100 * 3, 5508, 100, 120 });
					else
						sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 100 * 3, 5508 + 120, 100, 120 });
					sfSprite_setPosition(spMenuPause, (sfVector2f) { 354.f + 228.f * 2, 448.f });
					sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);

					if (pauseSelect == 0) {
						if (yPauseSelect == 3) {
							sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 162, 5338, 162, 162 });
							sfSprite_setPosition(spMenuPause, (sfVector2f) { 320.f, 426.f + 228.f });
							sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
							if (pause_EnnemisDiscovered & (PAUSE_VARAN))
							{
								sfText_setColor(pause_RenderText, sfWhite);
								sfText_setString(pause_RenderText, lang_GetFromKey("info.varan"));
								sfText_setCharacterSize(pause_RenderText, 18);
								sfText_setPosition(pause_RenderText, vector2f(1104.f, 378.f));
								sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
								sfText_setString(pause_RenderText, lang_GetFromKey("name.varan"));
								sfText_setCharacterSize(pause_RenderText, 29);
								sfText_setPosition(pause_RenderText, vector2f(1210.f, 278.f));
								sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
							}
							else
							{
								sfText_setColor(pause_RenderText, sfWhite);
								sfText_setString(pause_RenderText, "???");
								sfText_setCharacterSize(pause_RenderText, 18);
								sfText_setPosition(pause_RenderText, vector2f(1104.f, 378.f));
								sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
								sfText_setString(pause_RenderText, "???");
								sfText_setCharacterSize(pause_RenderText, 29);
								sfText_setPosition(pause_RenderText, vector2f(1210.f, 278.f));
								sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
							}
						}
						else
						{
							sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 0, 5338, 162, 162 });
							sfSprite_setPosition(spMenuPause, (sfVector2f) { 320.f, 426.f + 228.f });
							sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
						}
					}
					else
					{
						sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 0, 5338, 162, 162 });
						sfSprite_setPosition(spMenuPause, (sfVector2f) { 320.f, 426.f + 228.f });
						sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
					}
					if (pause_EnnemisDiscovered & (PAUSE_VARAN))
						sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 100 * 6, 5508, 100, 120 });
					else
						sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 100 * 6, 5508 + 120, 100, 120 });
					sfSprite_setPosition(spMenuPause, (sfVector2f) { 354.f, 448.f + 228.f });
					sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);

					if (pauseSelect == 1) {
						if (yPauseSelect == 3) {
							sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 162, 5338, 162, 162 });
							sfSprite_setPosition(spMenuPause, (sfVector2f) { 320.f + 228.f, 426.f + 228.f });
							sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
							if (pause_EnnemisDiscovered & (PAUSE_MOLE))
							{
								sfText_setColor(pause_RenderText, sfWhite);
								sfText_setString(pause_RenderText, lang_GetFromKey("info.mole"));
								sfText_setCharacterSize(pause_RenderText, 18);
								sfText_setPosition(pause_RenderText, vector2f(1104.f, 378.f));
								sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
								sfText_setString(pause_RenderText, lang_GetFromKey("name.mole"));
								sfText_setCharacterSize(pause_RenderText, 29);
								sfText_setPosition(pause_RenderText, vector2f(1210.f, 278.f));
								sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
							}
							else
							{
								sfText_setColor(pause_RenderText, sfWhite);
								sfText_setString(pause_RenderText, "???");
								sfText_setCharacterSize(pause_RenderText, 18);
								sfText_setPosition(pause_RenderText, vector2f(1104.f, 378.f));
								sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
								sfText_setString(pause_RenderText, "???");
								sfText_setCharacterSize(pause_RenderText, 29);
								sfText_setPosition(pause_RenderText, vector2f(1210.f, 278.f));
								sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
							}
						}
						else
						{
							sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 0, 5338, 162, 162 });
							sfSprite_setPosition(spMenuPause, (sfVector2f) { 320.f + 228.f, 426.f + 228.f });
							sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
						}
					}
					else
					{
						sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 0, 5338, 162, 162 });
						sfSprite_setPosition(spMenuPause, (sfVector2f) { 320.f + 228.f, 426.f + 228.f });
						sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
					}
					if (pause_EnnemisDiscovered & (PAUSE_MOLE))
						sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 100 * 5, 5508, 100, 120 });
					else
						sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 100 * 5, 5508 + 120, 100, 120 });
					sfSprite_setPosition(spMenuPause, (sfVector2f) { 354.f + 228.f, 448.f + 228.f });
					sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);

					if (pauseSelect == 2) {
						if (yPauseSelect == 3) {
							sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 162, 5338, 162, 162 });
							sfSprite_setPosition(spMenuPause, (sfVector2f) { 320.f + 228.f * 2, 426.f + 228.f });
							sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
							if (pause_EnnemisDiscovered & (PAUSE_BAT))
							{
								sfText_setColor(pause_RenderText, sfWhite);
								sfText_setString(pause_RenderText, lang_GetFromKey("info.bat"));
								sfText_setCharacterSize(pause_RenderText, 18);
								sfText_setPosition(pause_RenderText, vector2f(1104.f, 378.f));
								sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
								sfText_setString(pause_RenderText, lang_GetFromKey("name.bat"));
								sfText_setCharacterSize(pause_RenderText, 29);
								sfText_setPosition(pause_RenderText, vector2f(1210.f, 278.f));
								sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
							}
							else
							{
								sfText_setColor(pause_RenderText, sfWhite);
								sfText_setString(pause_RenderText, "???");
								sfText_setCharacterSize(pause_RenderText, 18);
								sfText_setPosition(pause_RenderText, vector2f(1104.f, 378.f));
								sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
								sfText_setString(pause_RenderText, "???");
								sfText_setCharacterSize(pause_RenderText, 29);
								sfText_setPosition(pause_RenderText, vector2f(1210.f, 278.f));
								sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
							}
						}
						else
						{
							sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 0, 5338, 162, 162 });
							sfSprite_setPosition(spMenuPause, (sfVector2f) { 320.f + 228.f * 2, 426.f + 228.f });
							sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
						}
					}
					else
					{
						sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 0, 5338, 162, 162 });
						sfSprite_setPosition(spMenuPause, (sfVector2f) { 320.f + 228.f * 2, 426.f + 228.f });
						sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
					}
					if (pause_EnnemisDiscovered & (PAUSE_BAT))
						sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 100 * 10, 5508, 100, 120 });
					else
						sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 100 * 10, 5508 + 120, 100, 120 });
					sfSprite_setPosition(spMenuPause, (sfVector2f) { 354.f + 228.f * 2, 448.f + 228.f });
					sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
				}
				if (yPauseSelect == 4) {
					if (pauseSelect == 0) {
						if (yPauseSelect == 4) {
							sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 162, 5338, 162, 162 });
							sfSprite_setPosition(spMenuPause, (sfVector2f) { 320.f, 426.f });
							sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
							if (pause_EnnemisDiscovered & (PAUSE_GOOSE))
							{
								sfText_setColor(pause_RenderText, sfWhite);
								sfText_setString(pause_RenderText, lang_GetFromKey("info.goose"));
								sfText_setCharacterSize(pause_RenderText, 18);
								sfText_setPosition(pause_RenderText, vector2f(1104.f, 378.f));
								sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
								sfText_setString(pause_RenderText, lang_GetFromKey("name.goose"));
								sfText_setCharacterSize(pause_RenderText, 29);
								sfText_setPosition(pause_RenderText, vector2f(1210.f, 278.f));
								sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
							}
							else
							{
								sfText_setColor(pause_RenderText, sfWhite);
								sfText_setString(pause_RenderText, "???");
								sfText_setCharacterSize(pause_RenderText, 18);
								sfText_setPosition(pause_RenderText, vector2f(1104.f, 378.f));
								sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
								sfText_setString(pause_RenderText, "???");
								sfText_setCharacterSize(pause_RenderText, 29);
								sfText_setPosition(pause_RenderText, vector2f(1210.f, 278.f));
								sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
							}
						}
						else
						{
							sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 0, 5338, 162, 162 });
							sfSprite_setPosition(spMenuPause, (sfVector2f) { 320.f, 426.f + 228.f });
							sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
						}
					}
					else
					{
						sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 0, 5338, 162, 162 });
						sfSprite_setPosition(spMenuPause, (sfVector2f) { 320.f, 426.f});
						sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
					}
					if (pause_EnnemisDiscovered & (PAUSE_GOOSE))
						sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 100 * 2, 5508, 100, 120 });
					else
						sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 100 * 2, 5508 + 120, 100, 120 });
					sfSprite_setPosition(spMenuPause, (sfVector2f) { 354.f, 448.f});
					sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);

					if (pauseSelect == 1) {
						if (yPauseSelect == 4) {
							sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 162, 5338, 162, 162 });
							sfSprite_setPosition(spMenuPause, (sfVector2f) { 320.f + 228.f, 426.f});
							sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
							if (pause_EnnemisDiscovered & (PAUSE_GOAT))
							{
								sfText_setColor(pause_RenderText, sfWhite);
								sfText_setString(pause_RenderText, lang_GetFromKey("info.goat"));
								sfText_setCharacterSize(pause_RenderText, 18);
								sfText_setPosition(pause_RenderText, vector2f(1104.f, 378.f));
								sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
								sfText_setString(pause_RenderText, lang_GetFromKey("name.goat"));
								sfText_setCharacterSize(pause_RenderText, 29);
								sfText_setPosition(pause_RenderText, vector2f(1210.f, 278.f));
								sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
							}
							else
							{
								sfText_setColor(pause_RenderText, sfWhite);
								sfText_setString(pause_RenderText, "???");
								sfText_setCharacterSize(pause_RenderText, 18);
								sfText_setPosition(pause_RenderText, vector2f(1104.f, 378.f));
								sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
								sfText_setString(pause_RenderText, "???");
								sfText_setCharacterSize(pause_RenderText, 29);
								sfText_setPosition(pause_RenderText, vector2f(1210.f, 278.f));
								sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
							}
						}
						else
						{
							sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 0, 5338, 162, 162 });
							sfSprite_setPosition(spMenuPause, (sfVector2f) { 320.f + 228.f, 426.f});
							sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
						}
					}
					else
					{
						sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 0, 5338, 162, 162 });
						sfSprite_setPosition(spMenuPause, (sfVector2f) { 320.f + 228.f, 426.f});
						sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
					}
					if (pause_EnnemisDiscovered & (PAUSE_GOAT))
						sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 100 * 13, 5508, 100, 120 });
					else
						sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 100 * 13, 5508 + 120, 100, 120 });
					sfSprite_setPosition(spMenuPause, (sfVector2f) { 354.f + 228.f, 448.f});
					sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
					if (pauseSelect == 2) {
						if (yPauseSelect == 4) {
							sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 162, 5338, 162, 162 });
							sfSprite_setPosition(spMenuPause, (sfVector2f) { 320.f + 228.f * 2, 426.f});
							sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
							if (pause_EnnemisDiscovered & (PAUSE_GOAT_BLACK))
							{
								sfText_setColor(pause_RenderText, sfWhite);
								sfText_setString(pause_RenderText, lang_GetFromKey("info.goat.black"));
								sfText_setCharacterSize(pause_RenderText, 18);
								sfText_setPosition(pause_RenderText, vector2f(1104.f, 378.f));
								sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
								sfText_setString(pause_RenderText, lang_GetFromKey("name.goat.black"));
								sfText_setCharacterSize(pause_RenderText, 29);
								sfText_setPosition(pause_RenderText, vector2f(1210.f, 278.f));
								sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
							}
							else
							{
								sfText_setColor(pause_RenderText, sfWhite);
								sfText_setString(pause_RenderText, "???");
								sfText_setCharacterSize(pause_RenderText, 18);
								sfText_setPosition(pause_RenderText, vector2f(1104.f, 378.f));
								sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
								sfText_setString(pause_RenderText, "???");
								sfText_setCharacterSize(pause_RenderText, 29);
								sfText_setPosition(pause_RenderText, vector2f(1210.f, 278.f));
								sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
							}
						}
						else
						{
							sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 0, 5338, 162, 162 });
							sfSprite_setPosition(spMenuPause, (sfVector2f) { 320.f + 228.f * 2, 426.f});
							sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
						}
					}
					else
					{
						sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 0, 5338, 162, 162 });
						sfSprite_setPosition(spMenuPause, (sfVector2f) { 320.f + 228.f * 2, 426.f});
						sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
					}
					if (pause_EnnemisDiscovered & (PAUSE_GOAT_BLACK))
						sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 100 * 12, 5508, 100, 120 });
					else
					sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 100 * 12, 5508 + 120, 100, 120 });
					sfSprite_setPosition(spMenuPause, (sfVector2f) { 354.f + 228.f * 2, 448.f});
					sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
				}
				break;
			case PAUSE_MONSTER:
				if (yPauseSelect >= 0 && yPauseSelect <= 1) {
					if (pauseSelect == 0) {
						if (yPauseSelect == 0) {
							sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 162, 5338, 162, 162 });
							sfSprite_setPosition(spMenuPause, (sfVector2f) { 320.f, 426.f });
							sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
							if (pause_EnnemisDiscovered & (PAUSE_LITTLE_JOY_DEMON))
							{
								sfText_setColor(pause_RenderText, sfWhite);
								sfText_setString(pause_RenderText, lang_GetFromKey("info.little.joy.demon"));
								sfText_setCharacterSize(pause_RenderText, 19);
								sfText_setPosition(pause_RenderText, vector2f(1104.f, 378.f));
								sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
								sfText_setString(pause_RenderText, lang_GetFromKey("name.little.joy.demon"));
								sfText_setCharacterSize(pause_RenderText, 29);
								sfText_setPosition(pause_RenderText, vector2f(1210.f, 278.f));
								sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);;
							}
							else
							{
								sfText_setColor(pause_RenderText, sfWhite);
								sfText_setString(pause_RenderText, "???");
								sfText_setCharacterSize(pause_RenderText, 19);
								sfText_setPosition(pause_RenderText, vector2f(1104.f, 378.f));
								sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
								sfText_setString(pause_RenderText, "???");
								sfText_setCharacterSize(pause_RenderText, 29);
								sfText_setPosition(pause_RenderText, vector2f(1210.f, 278.f));
								sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
							}
						}
						else
						{
							sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 0, 5338, 162, 162 });
							sfSprite_setPosition(spMenuPause, (sfVector2f) { 320.f, 426.f });
							sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
						}
					}
					else
					{
						sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 0, 5338, 162, 162 });
						sfSprite_setPosition(spMenuPause, (sfVector2f) { 320.f, 426.f });
						sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
					}
					if (pause_EnnemisDiscovered & (PAUSE_LITTLE_JOY_DEMON))
						 sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 100 * 4, 5740, 100, 120 });
					else
						sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 100 * 4, 5740 + 120, 100, 120 });
					sfSprite_setPosition(spMenuPause, (sfVector2f) { 354.f, 448.f });
					sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);

					if (pauseSelect == 1) {
						if (yPauseSelect == 0) {
							sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 162, 5338, 162, 162 });
							sfSprite_setPosition(spMenuPause, (sfVector2f) { 320.f + 228.f, 426.f });
							sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
							if (pause_EnnemisDiscovered & (PAUSE_BIG_JOY_DEMON))
							{
								sfText_setColor(pause_RenderText, sfWhite);
								sfText_setString(pause_RenderText, lang_GetFromKey("info.big.joy.demon"));
								sfText_setCharacterSize(pause_RenderText, 19);
								sfText_setPosition(pause_RenderText, vector2f(1104.f, 378.f));
								sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
								sfText_setString(pause_RenderText, lang_GetFromKey("name.big.joy.demon"));
								sfText_setCharacterSize(pause_RenderText, 29);
								sfText_setPosition(pause_RenderText, vector2f(1210.f, 278.f));
								sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
							}
							else
							{
								sfText_setColor(pause_RenderText, sfWhite);
								sfText_setString(pause_RenderText, "???");
								sfText_setCharacterSize(pause_RenderText, 19);
								sfText_setPosition(pause_RenderText, vector2f(1104.f, 378.f));
								sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
								sfText_setString(pause_RenderText, "???");
								sfText_setCharacterSize(pause_RenderText, 29);
								sfText_setPosition(pause_RenderText, vector2f(1210.f, 278.f));
								sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
							}
						}
						else
						{
							sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 0, 5338, 162, 162 });
							sfSprite_setPosition(spMenuPause, (sfVector2f) { 320.f + 228.f, 426.f });
							sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
						}
					}
					else
					{
						sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 0, 5338, 162, 162 });
						sfSprite_setPosition(spMenuPause, (sfVector2f) { 320.f + 228.f, 426.f });
						sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
					}
					if (pause_EnnemisDiscovered & (PAUSE_BIG_JOY_DEMON))
						sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 100 * 1, 5740, 100, 120 });
					else
						sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 100 * 1, 5740 + 120, 100, 120 });
					sfSprite_setPosition(spMenuPause, (sfVector2f) { 354.f + 228.f, 448.f });
					sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);


					if (pauseSelect == 2) {
						if (yPauseSelect == 0) {
							sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 162, 5338, 162, 162 });
							sfSprite_setPosition(spMenuPause, (sfVector2f) { 320.f + 228.f * 2, 426.f });
							sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
							if (pause_EnnemisDiscovered & (PAUSE_JOY_GOLEM))
							{
								sfText_setColor(pause_RenderText, sfWhite);
								sfText_setString(pause_RenderText, lang_GetFromKey("info.joy.golem"));
								sfText_setCharacterSize(pause_RenderText, 19);
								sfText_setPosition(pause_RenderText, vector2f(1104.f, 378.f));
								sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
								sfText_setString(pause_RenderText, lang_GetFromKey("name.joy.golem"));
								sfText_setCharacterSize(pause_RenderText, 29);
								sfText_setPosition(pause_RenderText, vector2f(1210.f, 278.f));
								sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
							}
							else
							{
								sfText_setColor(pause_RenderText, sfWhite);
								sfText_setString(pause_RenderText, "???");
								sfText_setCharacterSize(pause_RenderText, 19);
								sfText_setPosition(pause_RenderText, vector2f(1104.f, 378.f));
								sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
								sfText_setString(pause_RenderText, "???");
								sfText_setCharacterSize(pause_RenderText, 29);
								sfText_setPosition(pause_RenderText, vector2f(1210.f, 278.f));
								sfRenderTexture_drawText(_window->rdrt, pause_RenderText, NULL);
							}
						}
						else
						{
							sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 0, 5338, 162, 162 });
							sfSprite_setPosition(spMenuPause, (sfVector2f) { 320.f + 228.f * 2, 426.f });
							sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
						}
					}
					else
					{
						sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 0, 5338, 162, 162 });
						sfSprite_setPosition(spMenuPause, (sfVector2f) { 320.f + 228.f * 2, 426.f });
						sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
					}
					if (pause_EnnemisDiscovered & (PAUSE_JOY_GOLEM))
						sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 100 * 6, 5740, 100, 120 });
					else
						sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 100 * 6, 5740 + 120, 100, 120 });
					sfSprite_setPosition(spMenuPause, (sfVector2f) { 354.f + 228.f * 2, 448.f });
					sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
				}
				break;
			}
		}
		break;
	case PAUSE_QUETE:
		sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 0, 4288, 1644, 850 });
		sfSprite_setPosition(spMenuPause, (sfVector2f) { 136.f, 170.f });
		sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL); 

		sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 0, 5984, 82, 93 });
		sfSprite_setPosition(spMenuPause, (sfVector2f) { 778.f, 90.f });
		sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);

		sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 82, 6076, 82, 96 });
		sfSprite_setPosition(spMenuPause, (sfVector2f) { 778.f + 100, 64.f });
		sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);

		sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 82 * 2, 5984, 82, 90 });
		sfSprite_setPosition(spMenuPause, (sfVector2f) { 778.f + 200, 90.f });
		sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);

		sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 82 * 3, 5984, 82, 96 });
		sfSprite_setPosition(spMenuPause, (sfVector2f) { 778.f + 300, 90.f });
		sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);

		if (qst_GetCategory() == PAUSE_MENTOR) {
			if (qst_getNumberofQuestsOfPNJ(PAUSE_MENTOR) > 0)
			{
				sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 0, 3938 + 130, 184, 130 });
			}
			else
			{
				sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 188 * 3, 3938 + 130, 185, 130 });
			}
			sfSprite_setPosition(spMenuPause, (sfVector2f) { 328.f, 218.f });
		}
		else 
		{
			if (qst_getNumberofQuestsOfPNJ(PAUSE_MENTOR) > 0) {
				sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 0, 3938, 185, 130 });
			}
			else {
				sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 188 * 3, 3938, 175, 130 });
			}
			sfSprite_setPosition(spMenuPause, (sfVector2f) { 328.f, 246.f });
		}
		sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);

		if (qst_GetCategory() == PAUSE_ARNOLD) {
			if (qst_getNumberofQuestsOfPNJ(PAUSE_ARNOLD) > 0) {
				sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 188, 3938 + 130, 188, 130 });
			}
			else {
				sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 188 * 4 -2, 3938 + 130, 176, 130 });
			}
			sfSprite_setPosition(spMenuPause, (sfVector2f) { 328.f + 202.f, 218.f });
		}
		else
		{
			if (qst_getNumberofQuestsOfPNJ(PAUSE_ARNOLD) > 0) {
				sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 188 , 3938, 188, 130 });
			}
			else {
				sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 188 * 4 - 14, 3938, 180, 130 });
			}
			sfSprite_setPosition(spMenuPause, (sfVector2f) { 328.f + 202.f, 246.f });
		}
		sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);

		if (qst_GetCategory() == PAUSE_SHAMAN) {
			if (qst_getNumberofQuestsOfPNJ(PAUSE_SHAMAN) > 0)
			{
				sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 188 * 2, 3938 + 130, 188, 130 });
				sfSprite_setPosition(spMenuPause, (sfVector2f) { 328.f + 202.f * 2, 218.f });
			}
			else
			{
				sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 188 * 5 - 14, 3938 + 130, 188, 130 });
				sfSprite_setPosition(spMenuPause, (sfVector2f) { 328.f + 202.f * 2, 218.f });
			}
		}
		else
		{
			if (qst_getNumberofQuestsOfPNJ(PAUSE_SHAMAN) > 0)
			{
				sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 188 * 2, 3938, 188, 130 });
				sfSprite_setPosition(spMenuPause, (sfVector2f) { 328.f + 202.f * 2, 246.f });
			}
			else
			{
				sfSprite_setTextureRect(spMenuPause, (sfIntRect) { 188 * 5 - 14, 3938, 188, 130 });
				sfSprite_setPosition(spMenuPause, (sfVector2f) { 328.f + 202.f * 2, 246.f });
			}
		}
		sfRenderTexture_drawSprite(_window->rdrt, spMenuPause, NULL);
		qst_Display(_window);
		break;
	case PAUSE_OPTIONS:
		options_Display(_window);
		break;
	}
	sfRenderTexture_setView(_window->rdrt, mainView->view);
}

void pause_itemRecup(Items _item)
{
	for (int i = 0; i < itemList->size(itemList); i++)
	{
		if (STD_LIST_GETDATA(itemList, Itemlist, i)->item == _item)
		{
			STD_LIST_GETDATA(itemList, Itemlist, i)->nbObject++;
			pause_ItemsDiscovered |= _item;
			pause_EnnemisDiscovered |= _item;
		}
		
	}
}

void pause_itemRemove(Items _item, int _nbItemToSuppr)
{
	for (int i = 0; i < itemList->size(itemList); i++)
	{
		if (STD_LIST_GETDATA(itemList, Itemlist, i)->nbObject > 0)
		{
			if (STD_LIST_GETDATA(itemList, Itemlist, i)->item == _item)
				STD_LIST_GETDATA(itemList, Itemlist, i)->nbObject -= _nbItemToSuppr;
			if (STD_LIST_GETDATA(itemList, Itemlist, i)->nbObject <= 0)
			{
				STD_LIST_GETDATA(itemList, Itemlist, i)->nbObject = 0;
				//pause_ItemsDiscovered &= ~_item;
			}
		}
	}
}

int pause_getNbItem(Items _item)
{
	for (int i = 0; i < itemList->size(itemList); i++)
	{
		if (STD_LIST_GETDATA(itemList, Itemlist, i)->item == _item)
		{
			return STD_LIST_GETDATA(itemList, Itemlist, i)->nbObject;
		}
	}
	return 0;
}

quetePNJ pause_getQueteIdPNJ()
{
	return pause_queteIdPNJ;
}

void deinitPause()
{
	if (!pause_RenderRect || !pause_RenderText || spMenuPause) return;
	sfRectangleShape_destroy(pause_RenderRect);
	sfText_destroy(pause_RenderText);
	sfSprite_destroy(spMenuPause);
}