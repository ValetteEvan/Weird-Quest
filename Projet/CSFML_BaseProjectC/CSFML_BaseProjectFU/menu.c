#include "menu.h"
#include "textureManager.h"
#include "soundManager.h"
#include "controller.h"
#include "viewManager.h"
#include "fontManager.h"
#include "fadeManager.h"
#include "langManager.h"
#include "vibrationManager.h"
#include "fish.h"

#define MENU_FULL_RECT_SIZE vector2f(1920.f, 1080.f)
#define MENU_NORMAL_SIZE vector2f(426.f, 118.f)
#define MENU_SURVOL_SIZE vector2f(458.f, 150.f)
#define MENU_ROUND_SIZE vector2f(55.f, 55.f)
#define MENU_ICON_CONTINUE_SIZE vector2f(646.f, 646.f)
#define MENU_ICON_NEWGAME_SIZE vector2f(646.f, 646.f)
#define MENU_ICON_SETTINGS_SIZE vector2f(646.f, 646.f)
#define MENU_ICON_QUIT_SIZE vector2f(646.f, 646.f)
#define MENU_CONTINUE_TXT_POS vector2f(230.f, 450.f)
#define MENU_NEWGAME_TXT_POS vector2f(230.f, 602.f)
#define MENU_SETTINGS_TXT_POS vector2f(230.f, 758.f)
#define MENU_QUIT_TXT_POS vector2f(230.f, 930.f)
#define MENU_SURVOL_CONTINUE_TXT_POS vector2f(358.f, 450.f)
#define MENU_SURVOL_NEWGAME_TXT_POS vector2f(358.f, 602.f)
#define MENU_SURVOL_SETTINGS_TXT_POS vector2f(358.f, 758.f)
#define MENU_SURVOL_QUIT_TXT_POS vector2f(358.f, 930.f)

sfRectangleShape* menuRect;
sfText* menuTxt;
sfBool onePassMenu;
sfBool isNewGame;

enum { MENU_SEL_PLAY = 0, MENU_SEL_GAME, MENU_SEL_SETTINGS, MENU_SEL_QUIT } menu_Sel;
float menuTimer;

void menu_Init() {
	if (!GetSfTexture("menuSprite")) Texture_Onload(MENU);
	soundsOnload(MENU);

	menuRect = sfRectangleShape_create();
	sfRectangleShape_setSize(menuRect, MENU_FULL_RECT_SIZE);
	sfRectangleShape_setTexture(menuRect, GetSfTexture("menuSprite"), sfTrue);

	menuTxt = sfText_create();
	sfText_setFont(menuTxt, GetFont("Pixel"));
	sfText_setOutlineThickness(menuTxt, 6.f);
	sfText_setCharacterSize(menuTxt, 70.f);
	
	menu_Sel = MENU_SEL_PLAY;
	menuTimer = 0.f;
	onePassMenu = sfTrue;
	isNewGame = sfFalse;
}

void menu_Update(Window* _window) {

	if (onePassMenu)
	{
		setVolume();
		playMusicIfNotPlaying("Music_Menu");
		onePassMenu = sfFalse;
	}

	if (fade_GetOpacity() > 0) fadeIn(1.f);
	else menuTimer += getDeltaTime();

	/// Menu navigation
	if ((Gamepad_isButtonPressed(0, A, _window->renderWindow) || customKeyboardIsKeyPressed(sfKeyEnter, _window->renderWindow)) && menuTimer > 0.2f) {
		menuTimer = 0.0f;
		switch (menu_Sel) {
			case MENU_SEL_PLAY:
				isNewGame = sfFalse;
				stopMusicIfPlaying("Music_Menu");
				changeState(_window, GAME);
				if (getGameState() == GAME_FISH)
					changeGameState(GAME_OUTSIDE);
				break;
			case MENU_SEL_GAME:
				isNewGame = sfTrue;
				stopMusicIfPlaying("Music_Menu");
				changeState(_window, CINEMATIC_INTRO);
				break;
			case MENU_SEL_SETTINGS:
				toggleOptions();
				break;
			case MENU_SEL_QUIT:
				_window->isDone = sfTrue;
				break;
			default: break;
		}
	}
	/// Selecting options
	else if ((customKeyboardIsKeyPressed(sfKeyUp, _window->renderWindow) || Gamepad_isButtonPressed(0, DPAD_UP, _window->renderWindow) || Gamepad_getStickPos(0, STICKL, _window->renderWindow).y < 0) && menuTimer > 0.2f) {
		menu_Sel--;
		if (menu_Sel < MENU_SEL_PLAY) menu_Sel = MENU_SEL_QUIT;
		menuTimer = 0.0f;
	}
	else if ((customKeyboardIsKeyPressed(sfKeyDown, _window->renderWindow) || Gamepad_isButtonPressed(0, DPAD_DOWN, _window->renderWindow) || Gamepad_getStickPos(0, STICKL, _window->renderWindow).y > 0) && menuTimer > 0.2f) {
		menu_Sel++;
		if (menu_Sel > MENU_SEL_QUIT) menu_Sel = MENU_SEL_PLAY;
		menuTimer = 0.0f;
	}
	/// Opening the editor via a debug keybind
	else if (DEBUG_STATE && customKeyboardIsKeyPressed(sfKeyTab, _window->renderWindow)) changeState(_window, EDITOR);
}

void menu_Display(Window* _window) {
	/// Light memory leak in that function, might need further analysis

	sfRectangleShape_setSize(menuRect, MENU_FULL_RECT_SIZE);
	sfRectangleShape_setPosition(menuRect, vector2f(0.f, 0.f));
	sfRectangleShape_setTextureRect(menuRect, (sfIntRect) { 0, 0, 1920, 1080 });
	sfRenderTexture_drawRectangleShape(_window->rdrt, menuRect, NULL);

	sfRectangleShape_setSize(menuRect, MENU_NORMAL_SIZE);
	sfRectangleShape_setPosition(menuRect, vector2f(156.f, 422.f)); 
	sfRectangleShape_setTextureRect(menuRect, (sfIntRect) { 0, 1080, MENU_NORMAL_SIZE.x, MENU_NORMAL_SIZE.y });
	
	/// 154 px in Y between each menu button

	/// Selection circle
	sfRectangleShape_setSize(menuRect, MENU_ROUND_SIZE);
	sfRectangleShape_setPosition(menuRect, vector2f(150.f, 570.f + 100.f * menu_Sel));
	sfRectangleShape_setTextureRect(menuRect, (sfIntRect) { 0, 1339, MENU_ROUND_SIZE.x, MENU_ROUND_SIZE.y });
	sfRenderTexture_drawRectangleShape(_window->rdrt, menuRect, NULL);

	/// Rightside icon
	switch (menu_Sel) { /// Selecting which icon to render and placing the sprite accordingly
		case MENU_SEL_PLAY:
			sfRectangleShape_setSize(menuRect, MENU_ICON_CONTINUE_SIZE);
			sfRectangleShape_setPosition(menuRect, vector2f(1170.f, 260.f));
			sfRectangleShape_setTextureRect(menuRect, (sfIntRect) { 0, 1396, MENU_ICON_CONTINUE_SIZE.x, MENU_ICON_CONTINUE_SIZE.y });
			break;
		case MENU_SEL_GAME:
			sfRectangleShape_setSize(menuRect, MENU_ICON_NEWGAME_SIZE);
			sfRectangleShape_setPosition(menuRect, vector2f(1170.f, 253.f));
			sfRectangleShape_setTextureRect(menuRect, (sfIntRect) { 0, 2042, MENU_ICON_NEWGAME_SIZE.x, MENU_ICON_NEWGAME_SIZE.y });
			break;
		case MENU_SEL_SETTINGS:
			sfRectangleShape_setSize(menuRect, MENU_ICON_SETTINGS_SIZE);
			sfRectangleShape_setPosition(menuRect, vector2f(1170.f, 260.f));
			sfRectangleShape_setTextureRect(menuRect, (sfIntRect) { 0, 2688, MENU_ICON_SETTINGS_SIZE.x, MENU_ICON_SETTINGS_SIZE.y });
			break;
		case MENU_SEL_QUIT:
			sfRectangleShape_setSize(menuRect, MENU_ICON_QUIT_SIZE);
			sfRectangleShape_setPosition(menuRect, vector2f(1170.f, 260.f));
			sfRectangleShape_setTextureRect(menuRect, (sfIntRect) { 0, 3334, MENU_ICON_QUIT_SIZE.x, MENU_ICON_QUIT_SIZE.y });
			break;
	}
	sfRenderTexture_drawRectangleShape(_window->rdrt, menuRect, NULL);

	/// Play button
	if(menu_Sel == MENU_SEL_PLAY) sfText_setOutlineColor(menuTxt, color(107, 190, 161, 255));
	else sfText_setOutlineColor(menuTxt, color(108, 91, 63, 255));
	sfText_setString(menuTxt, lang_GetFromKey("menu.play"));
	sfText_setPosition(menuTxt, vector2f(230.f, 550.f));
	sfRenderTexture_drawText(_window->rdrt, menuTxt, NULL);
	
	/// New game button
	if (menu_Sel == MENU_SEL_GAME) sfText_setOutlineColor(menuTxt, color(107, 190, 161, 255));
	else sfText_setOutlineColor(menuTxt, color(108, 91, 63, 255));
	sfText_setString(menuTxt, lang_GetFromKey("menu.new.game"));
	sfText_setPosition(menuTxt,vector2f(230.f, 650.f));
	sfRenderTexture_drawText(_window->rdrt, menuTxt, NULL);
	
	/// Settings button
	if (menu_Sel == MENU_SEL_SETTINGS) sfText_setOutlineColor(menuTxt, color(107, 190, 161, 255));
	else sfText_setOutlineColor(menuTxt, color(108, 91, 63, 255));
	sfText_setString(menuTxt, lang_GetFromKey("menu.settings"));
	sfText_setPosition(menuTxt,vector2f(230.f, 750.f));
	sfRenderTexture_drawText(_window->rdrt, menuTxt, NULL);
	
	/// Quit button
	if (menu_Sel == MENU_SEL_QUIT) sfText_setOutlineColor(menuTxt, color(107, 190, 161, 255));
	else sfText_setOutlineColor(menuTxt, color(108, 91, 63, 255));
	sfText_setString(menuTxt, lang_GetFromKey("menu.quit"));
	sfText_setPosition(menuTxt, vector2f(230.f, 850.f));
	sfRenderTexture_drawText(_window->rdrt, menuTxt, NULL);

	fade_Display(_window);
}

void menu_Deinit() {
	if (!menuRect || !menuTxt) return;
	sfRectangleShape_destroy(menuRect);
	sfText_destroy(menuTxt);
	RemoveAllTextureButALL();
	RemoveAllSoundsNotNeededInEveryStates();
	RemoveAllFontsNotNeededInEveryStates();
}

void setOnePassMenu(sfBool _value)
{
	onePassMenu = _value;
}

sfBool getIsNewGame()
{
	return isNewGame;
}