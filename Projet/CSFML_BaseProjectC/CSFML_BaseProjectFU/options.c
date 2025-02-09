#include "options.h"
#include "soundManager.h"
#include "stateManager.h"
#include "viewManager.h"
#include "controller.h"
#include "fadeManager.h"
#include "textureManager.h"
#include "shaderManager.h"

#define SETTINGS_BOX_1_SIZE vector2f(944.f, 748.f)
#define SETTINGS_ICON_WINDOW_SIZE vector2f(220.f, 134.f)
#define SETTINGS_CURSOR_BAR_SIZE vector2f(30.f, 34.f)
#define SETTINGS_FX_SELECT_SIZE vector2f(304.f, 162.f)
#define SETTINGS_BOX_2_SIZE vector2f(1760.f, 748.f)

#define SETTINGS_BOX_1_Y 0
#define SETTINGS_ICON_WINDOW_Y 748
#define SETTINGS_CURSOR_BAR_Y 882
#define SETTINGS_FX_SELECT_Y 916
#define SETTINGS_BOX_2_Y 1078

#define SETTINGS_BOX_1_POS vector2f(482.f, 180.f)
#define SETTINGS_BOX_MUSIC_POS vector2f(978.f, 564.f) // 168
#define SETTINGS_BOX_SOUND_POS vector2f(978.f, 564.f - 168.f)
#define SETTTINGS_FEEDBACK_HTP vector2f(1029.f, 230.f)
#define SETTTINGS_FEEDBACK_SOUND vector2f(711.f, 296.f)
#define SETTTINGS_FEEDBACK_MUSIC vector2f(711.f, 464.f)
#define SETTTINGS_FEEDBACK_FULLSCREEN vector2f(703.f, 666.f)
#define SETTTINGS_FEEDBACK_EXIT vector2f(1103.f, 792.f)
#define SETTINGS_IF_FULLSCREEN vector2f(702.f, 669.f)
#define SETTINGS_IF_NO_FULLSCREEN vector2f(714.f, 675.f)

#define SETTINGS_BOX_OPENED_BOX vector2f(70.f, 162.f)
#define SETTINGS_BOX_OPENED_CURSOR_SOUND vector2f(566.f, 378.f)
#define SETTINGS_BOX_OPENED_CURSOR_MUSIC vector2f(566.f, 546.f)
#define SETTINGS_BOX_OPENED_IF_FULLSCREEN vector2f(290.f, 669.f)
#define SETTINGS_BOX_OPENED_IF_NO_FULLSCREEN vector2f(302.f, 675.f)
#define SETTINGS_BOX_OPENED_FEEDBACK_HTP vector2f(621.f, 212.f)
#define SETTINGS_BOX_OPENED_FEEDBACK_SOUND vector2f(303.f, 278.f)
#define SETTINGS_BOX_OPENED_FEEDBACK_MUSIC vector2f(303.f, 445.f)
#define SETTINGS_BOX_OPENED_FULLSCREEN vector2f(295.f, 666.f)
#define SETTINGS_BOX_OPENED_EXIT vector2f(695.f, 774.f)

sfSprite* options_Sprite;
sfRectangleShape* options_Rect;
int optionsSelect;
float options_Timer;
sfBool options_IsSound;
sfBool options_IsMusic;
sfBool options_IsHowToPlay;

void options_SetOptionSelect(int _new_value) {
	optionsSelect = _new_value;
}

void options_Init(Window* _window) {
	options_Sprite = sfSprite_create();
	optionsSelect = 0;
	options_Timer = 0.f;
	options_Rect = sfRectangleShape_create();
	sfRectangleShape_setTexture(options_Rect, GetSfTexture("options"), sfTrue);
	options_IsSound, options_IsMusic, options_IsHowToPlay = sfFalse;
}

void options_Update(Window* _window) {
	setVolume();
	options_Timer += getUnscaledDeltaTime();
	if (options_IsSound) {
		if ((customKeyboardIsKeyPressed(sfKeyRight, _window->renderWindow) || Gamepad_isButtonPressed(0, DPAD_RIGHT, _window->renderWindow) || Gamepad_getStickPos(0, STICKL, _window->renderWindow).x > 0) && options_Timer > 0.025f && SFXVolume < 100.f) {
			SFXVolume++;
			options_Timer = 0.f;
		}
		else if ((customKeyboardIsKeyPressed(sfKeyLeft, _window->renderWindow) || Gamepad_isButtonPressed(0, DPAD_LEFT, _window->renderWindow) || Gamepad_getStickPos(0, STICKL, _window->renderWindow).x < 0) && options_Timer > 0.025f && SFXVolume > 0.f) {
			SFXVolume--;
			options_Timer = 0.f;
		}
		else if ((customKeyboardIsKeyPressed(sfKeyEscape, _window->renderWindow) || Gamepad_isButtonPressed(0, A, _window->renderWindow)) && options_Timer > 0.2f) {
			options_IsSound = sfFalse;
			options_Timer = 0.f;
		}
	}
	else if (options_IsMusic) {
		if ((customKeyboardIsKeyPressed(sfKeyRight, _window->renderWindow) || Gamepad_isButtonPressed(0, DPAD_RIGHT, _window->renderWindow) || Gamepad_getStickPos(0, STICKL, _window->renderWindow).x > 0) && options_Timer > 0.025f && musicVolume < 100.f) {
			musicVolume++;
			options_Timer = 0.f;
		}
		else if ((customKeyboardIsKeyPressed(sfKeyLeft, _window->renderWindow) || Gamepad_isButtonPressed(0, DPAD_LEFT, _window->renderWindow) || Gamepad_getStickPos(0, STICKL, _window->renderWindow).x < 0) && options_Timer > 0.025f && musicVolume > 0.f) {
			musicVolume--;
			options_Timer = 0.f;
		}
		else if ((customKeyboardIsKeyPressed(sfKeyEscape, _window->renderWindow) || Gamepad_isButtonPressed(0, A, _window->renderWindow)) && options_Timer > 0.2f) {
			options_IsMusic = sfFalse;
			options_Timer = 0.f;
		}
	}
	
	if (optionsSelect == 1) options_IsSound = sfTrue;
	if (optionsSelect == 2) options_IsMusic = sfTrue;

	if ((customKeyboardIsKeyPressed(sfKeyEnter, _window->renderWindow) || Gamepad_isButtonPressed(0, A, _window->renderWindow)) && options_Timer > 0.2f) {
		if (optionsSelect == 0) options_IsHowToPlay = !options_IsHowToPlay;
		else if (optionsSelect == 3) ToggleFullscreen(_window);
		else if (optionsSelect == 4) {
			if (getState() == MENU) toggleOptions();
			else {
				if (getGameState() == GAME_FISH)
					toggleOptions();
				else
					togglePause();
				changeState(_window, MENU);
				StopALLMusic();
			}
		}
		options_Timer = 0.f;
	}
	else if ((customKeyboardIsKeyPressed(sfKeyDown, _window->renderWindow) || Gamepad_isButtonPressed(0, DPAD_DOWN, _window->renderWindow) || Gamepad_isButtonPressed(0, DPAD_RIGHT, _window->renderWindow) || Gamepad_getStickPos(0, STICKL, _window->renderWindow).y > 0 || Gamepad_getStickPos(0, STICKL, _window->renderWindow).x > 0) && options_Timer > 0.2f) {
		optionsSelect++;
		options_IsSound = sfFalse;
		options_IsMusic = sfFalse;
		if (optionsSelect > 4) optionsSelect = 0;
		options_Timer = 0.f;
	}
	else if ((customKeyboardIsKeyPressed(sfKeyUp, _window->renderWindow) || Gamepad_isButtonPressed(0, DPAD_UP, _window->renderWindow) || Gamepad_isButtonPressed(0, DPAD_LEFT, _window->renderWindow) || Gamepad_getStickPos(0, STICKL, _window->renderWindow).y < 0 || Gamepad_getStickPos(0, STICKL, _window->renderWindow).x < 0) && options_Timer > 0.2f) {
		optionsSelect--;
		options_IsSound = sfFalse;
		options_IsMusic = sfFalse;
		if (optionsSelect < 0) optionsSelect = 4;
		options_Timer = 0.f;
	}

	if (getGameState() == GAME_FISH)
	{
		if ((customKeyboardIsKeyPressed(sfKeyEscape, _window->renderWindow) || Gamepad_isButtonPressed(0, START, _window->renderWindow) || Gamepad_isButtonPressed(0, B, _window->renderWindow)) && options_Timer > 0.2f) {
			options_Timer = 0.f;
			toggleOptions();
		}
	}
}

void options_Display(Window* _window) {

	sfRectangleShape_setOrigin(options_Rect, vector2f(0.f, 0.f));
	if (options_IsHowToPlay) {
		sfRectangleShape_setSize(options_Rect, SETTINGS_BOX_2_SIZE);
		sfRectangleShape_setPosition(options_Rect, SETTINGS_BOX_OPENED_BOX);
		sfRectangleShape_setTextureRect(options_Rect, (sfIntRect) { 0, SETTINGS_BOX_2_Y, SETTINGS_BOX_2_SIZE.x, SETTINGS_BOX_2_SIZE.y });
	}
	else {
		sfRectangleShape_setSize(options_Rect, SETTINGS_BOX_1_SIZE);
		sfRectangleShape_setPosition(options_Rect, SETTINGS_BOX_1_POS);
		sfRectangleShape_setTextureRect(options_Rect, (sfIntRect) { 0, SETTINGS_BOX_1_Y, SETTINGS_BOX_1_SIZE.x, SETTINGS_BOX_1_SIZE.y });
	}
	sfRenderTexture_drawRectangleShape(_window->rdrt, options_Rect, NULL);

	if (!options_IsHowToPlay) {
		if (IsFullscreen(_window)) sfRectangleShape_setPosition(options_Rect, SETTINGS_IF_FULLSCREEN); else sfRectangleShape_setPosition(options_Rect, SETTINGS_IF_NO_FULLSCREEN);
	}
	else {
		if (IsFullscreen(_window)) sfRectangleShape_setPosition(options_Rect, SETTINGS_BOX_OPENED_IF_FULLSCREEN); else sfRectangleShape_setPosition(options_Rect, SETTINGS_BOX_OPENED_IF_NO_FULLSCREEN);
	}
	sfRectangleShape_setSize(options_Rect, SETTINGS_ICON_WINDOW_SIZE);
	if (IsFullscreen(_window)) sfRectangleShape_setTextureRect(options_Rect, (sfIntRect) { 0, SETTINGS_ICON_WINDOW_Y, SETTINGS_ICON_WINDOW_SIZE.x, SETTINGS_ICON_WINDOW_SIZE.y }); else sfRectangleShape_setTextureRect(options_Rect, (sfIntRect) { 220, SETTINGS_ICON_WINDOW_Y, SETTINGS_ICON_WINDOW_SIZE.x, SETTINGS_ICON_WINDOW_SIZE.y });
	sfRenderTexture_drawRectangleShape(_window->rdrt, options_Rect, NULL);

	sfRectangleShape_setSize(options_Rect, SETTINGS_CURSOR_BAR_SIZE);
	if (options_IsMusic) {
		sfRectangleShape_setTextureRect(options_Rect, (sfIntRect) { 0, SETTINGS_CURSOR_BAR_Y, SETTINGS_CURSOR_BAR_SIZE.x, SETTINGS_CURSOR_BAR_SIZE.y });
		if (options_IsHowToPlay) {
			sfRectangleShape_setPosition(options_Rect, vector2f(SETTINGS_BOX_OPENED_CURSOR_MUSIC.x + lerp(0.f, 192.f, musicVolume / 100.f), SETTINGS_BOX_OPENED_CURSOR_MUSIC.y));
		}
		else {
			sfRectangleShape_setPosition(options_Rect, vector2f(SETTINGS_BOX_MUSIC_POS.x + lerp(0.f, 192.f, musicVolume / 100.f), SETTINGS_BOX_MUSIC_POS.y));
		}
	}
	else {
		sfRectangleShape_setTextureRect(options_Rect, (sfIntRect) { SETTINGS_CURSOR_BAR_SIZE.x, SETTINGS_CURSOR_BAR_Y, SETTINGS_CURSOR_BAR_SIZE.x, SETTINGS_CURSOR_BAR_SIZE.y });
		if (options_IsHowToPlay) {
			sfRectangleShape_setPosition(options_Rect, vector2f(SETTINGS_BOX_OPENED_CURSOR_MUSIC.x + lerp(0.f, 192.f, musicVolume / 100.f), SETTINGS_BOX_OPENED_CURSOR_MUSIC.y));
		}
		else {
			sfRectangleShape_setPosition(options_Rect, vector2f(SETTINGS_BOX_MUSIC_POS.x + lerp(0.f, 192.f, musicVolume / 100.f), SETTINGS_BOX_MUSIC_POS.y));
		}
	}
	sfRectangleShape_setOrigin(options_Rect, vector2f(sfRectangleShape_getGlobalBounds(options_Rect).width / 2.0f, 1.f));
	sfRenderTexture_drawRectangleShape(_window->rdrt, options_Rect, NULL);

	if (options_IsSound) {
		sfRectangleShape_setTextureRect(options_Rect, (sfIntRect) { 0, SETTINGS_CURSOR_BAR_Y, SETTINGS_CURSOR_BAR_SIZE.x, SETTINGS_CURSOR_BAR_SIZE.y });
		if (options_IsHowToPlay) {
			sfRectangleShape_setPosition(options_Rect, vector2f(SETTINGS_BOX_OPENED_CURSOR_SOUND.x + lerp(0.f, 192.f, SFXVolume / 100.f), SETTINGS_BOX_OPENED_CURSOR_SOUND.y));
		}
		else {
			sfRectangleShape_setPosition(options_Rect, vector2f(SETTINGS_BOX_SOUND_POS.x + lerp(0.f, 192.f, SFXVolume / 100.f), SETTINGS_BOX_SOUND_POS.y));
		}
	}
	else {
		sfRectangleShape_setTextureRect(options_Rect, (sfIntRect) { SETTINGS_CURSOR_BAR_SIZE.x, SETTINGS_CURSOR_BAR_Y, SETTINGS_CURSOR_BAR_SIZE.x, SETTINGS_CURSOR_BAR_SIZE.y });
		if (options_IsHowToPlay) {
			sfRectangleShape_setPosition(options_Rect, vector2f(SETTINGS_BOX_OPENED_CURSOR_SOUND.x + lerp(0.f, 192.f, SFXVolume / 100.f), SETTINGS_BOX_OPENED_CURSOR_SOUND.y));
		}
		else {
			sfRectangleShape_setPosition(options_Rect, vector2f(SETTINGS_BOX_SOUND_POS.x + lerp(0.f, 192.f, SFXVolume / 100.f), SETTINGS_BOX_SOUND_POS.y));
		}
	}
	sfRenderTexture_drawRectangleShape(_window->rdrt, options_Rect, NULL);

	sfRectangleShape_setSize(options_Rect, SETTINGS_FX_SELECT_SIZE);
	sfIntRect options_TempRect = (sfIntRect){ 0, SETTINGS_FX_SELECT_Y , SETTINGS_FX_SELECT_SIZE.x, SETTINGS_FX_SELECT_SIZE.y };
	if (!options_IsHowToPlay) {
		if (optionsSelect == 0) {
			sfRectangleShape_setPosition(options_Rect, SETTTINGS_FEEDBACK_HTP);
			options_TempRect.left = SETTINGS_FX_SELECT_SIZE.x * 2;
		}
		else if (optionsSelect == 1) {
			sfRectangleShape_setPosition(options_Rect, SETTTINGS_FEEDBACK_SOUND);
			options_TempRect.left = SETTINGS_FX_SELECT_SIZE.x;
		}
		else if (optionsSelect == 2) {
			sfRectangleShape_setPosition(options_Rect, SETTTINGS_FEEDBACK_MUSIC);
			options_TempRect.left = SETTINGS_FX_SELECT_SIZE.x;
		}
		else if (optionsSelect == 3) {
			if (!IsFullscreen(_window)) sfRectangleShape_setPosition(options_Rect, SETTTINGS_FEEDBACK_FULLSCREEN); else sfRectangleShape_setPosition(options_Rect, vector2f(SETTTINGS_FEEDBACK_FULLSCREEN.x, SETTTINGS_FEEDBACK_FULLSCREEN.y - 9.f));
			options_TempRect.left = 0;
		}
		else if (optionsSelect == 4) {
			sfRectangleShape_setPosition(options_Rect, SETTTINGS_FEEDBACK_EXIT);
			options_TempRect.left = SETTINGS_FX_SELECT_SIZE.x * 3;
		}
	}
	else {
		if (optionsSelect == 0) {
			sfRectangleShape_setPosition(options_Rect, SETTINGS_BOX_OPENED_FEEDBACK_HTP);
			options_TempRect.left = SETTINGS_FX_SELECT_SIZE.x * 2;
		}
		else if (optionsSelect == 1) {
			sfRectangleShape_setPosition(options_Rect, SETTINGS_BOX_OPENED_FEEDBACK_SOUND);
			options_TempRect.left = SETTINGS_FX_SELECT_SIZE.x;
		}
		else if (optionsSelect == 2) {
			sfRectangleShape_setPosition(options_Rect, SETTINGS_BOX_OPENED_FEEDBACK_MUSIC);
			options_TempRect.left = SETTINGS_FX_SELECT_SIZE.x;
		}
		else if (optionsSelect == 3) {
			if (!IsFullscreen(_window)) sfRectangleShape_setPosition(options_Rect, SETTINGS_BOX_OPENED_FULLSCREEN); else sfRectangleShape_setPosition(options_Rect, vector2f(SETTINGS_BOX_OPENED_FULLSCREEN.x, SETTINGS_BOX_OPENED_FULLSCREEN.y - 9.f));
			options_TempRect.left = 0;
		}
		else if (optionsSelect == 4) {
			sfRectangleShape_setPosition(options_Rect, SETTINGS_BOX_OPENED_EXIT);
			options_TempRect.left = SETTINGS_FX_SELECT_SIZE.x * 3;
		}
	}
	sfRectangleShape_setTextureRect(options_Rect, options_TempRect);
	sfRenderTexture_drawRectangleShape(_window->rdrt, options_Rect, NULL);

	if (getGameState() != GAME_FISH)
	{
		sfSprite_setTexture(options_Sprite, GetSfTexture("pause_BO"), sfTrue);
		sfSprite_setTextureRect(options_Sprite, (sfIntRect) { 0, 5984, 82, 93 });
		sfSprite_setPosition(options_Sprite, (sfVector2f) { 778.f, 90.f });
		sfRenderTexture_drawSprite(_window->rdrt, options_Sprite, NULL);

		sfSprite_setTextureRect(options_Sprite, (sfIntRect) { 82, 5984, 82, 96 });
		sfSprite_setPosition(options_Sprite, (sfVector2f) { 778.f + 100, 90.f });
		sfRenderTexture_drawSprite(_window->rdrt, options_Sprite, NULL);

		sfSprite_setTextureRect(options_Sprite, (sfIntRect) { 82 * 2, 5984, 82, 90 });
		sfSprite_setPosition(options_Sprite, (sfVector2f) { 778.f + 200, 90.f });
		sfRenderTexture_drawSprite(_window->rdrt, options_Sprite, NULL);

		sfSprite_setTextureRect(options_Sprite, (sfIntRect) { 82 * 3, 6076, 82, 96 });
		sfSprite_setPosition(options_Sprite, (sfVector2f) { 778.f + 300, 64.f });
		sfRenderTexture_drawSprite(_window->rdrt, options_Sprite, NULL);
	}
}

void options_Deinit() {
	sfRectangleShape_destroy(options_Rect);
}

void options_SetBackToNormal() {
	options_IsHowToPlay = sfFalse;
	options_IsMusic = sfFalse;
	options_IsSound = sfFalse;
	optionsSelect = 0;
}