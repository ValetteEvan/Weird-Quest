#include "stateManager.h"
#include "ressourcesManager.h"
#include "textureManager.h"
#include "soundManager.h"
#include "fontManager.h"
#include "pause.h"
#include "menu.h"
#include "game.h"
#include "dialogBox.h"
#include "options.h"
#include "viewManager.h"
#include "editor.h"
#include "langManager.h"
#include "shaderManager.h"
#include "fadeManager.h"
#include "options.h"
#include "opening_screen.h"
#include "cinematic_intro.h"
#include "cinematic_outro.h"
#include "game_HUD.h"
#include "current.h"
#include "expandingTile.h"
#include "credits.h"
#include "vibrationManager.h"

sfBool firstLoadThread = sfFalse;

sfThread* loadingThread;
sfRectangleShape* spLoading;
sfRectangleShape* backLoading;
sfRectangleShape* shadowLoading;

sfTexture* textureBack;
sfTexture* textureWalk;
sfTexture* textureShadow;

int framXLoading;
float timerLoading;

int framXBackLoading;
float timerBackLoading;

int framXShadowLoading;
float timerShadowLoading;

void stateInit(Window* _window)
{
	static sfBool firstload = sfFalse;
	if (!firstload)
	{

		OnLoad("../Ressources/ressources.load");  
		Texture_Onload(ALL); 
		fontsOnload(ALL);
		shaderOnload(ALL);

		backLoading = sfRectangleShape_create(); 
		spLoading = sfRectangleShape_create(); 
		shadowLoading = sfRectangleShape_create();

		textureBack = GetSfTexture("loadingWalk");
		textureWalk = GetSfTexture("loadingWalk");
		textureShadow = GetSfTexture("loadingWalk"); 

		sfRectangleShape_setTexture(spLoading, textureBack, sfTrue); 
		sfIntRect AnimRect = { 0, 1080, 211 , 314 };
		sfRectangleShape_setOrigin(spLoading, vector2f(211.f / 2, 314.f / 2));
		sfRectangleShape_setPosition(spLoading, vector2f(mainView->viewPos.x, mainView->viewPos.y));
		sfRectangleShape_setTextureRect(spLoading, AnimRect);
		sfRectangleShape_setScale(spLoading, vector2f(1.1, 1.1)); 
		 
		sfRectangleShape_setTexture(backLoading, textureWalk, sfTrue); 
		sfIntRect backRect = { 0, 0, 1920 , 1080 };
		sfRectangleShape_setOrigin(backLoading, vector2f(1920.f / 2, 1080.f / 2)); 
		sfRectangleShape_setPosition(backLoading, vector2f(mainView->viewPos.x, mainView->viewPos.y));
		sfRectangleShape_setTextureRect(backLoading, backRect); 
		sfRectangleShape_setScale(backLoading, vector2f(1.1, 1.1));

		sfRectangleShape_setTexture(shadowLoading, textureShadow, sfTrue); 
		sfIntRect shadowRect = { 0, 1394,670 , 56 };
		sfRectangleShape_setFillColor(shadowLoading, sfColor_fromRGBA(255,255,255,55)); 
		sfRectangleShape_setOrigin(shadowLoading, vector2f(670.f / 2, 56.f / 2));
		sfRectangleShape_setPosition(shadowLoading, vector2f(mainView->viewPos.x, mainView->viewPos.y));
		sfRectangleShape_setTextureRect(shadowLoading, backRect);
		sfRectangleShape_setScale(shadowLoading, vector2f(1.1, 1.1)); 

		SFXVolume = seting.sfxVol;
		musicVolume = seting.musicVol;
		soundsOnload(ALL);
		lang_Init();
		fade_Init();
		w.state = sfTrue;
		firstload = sfTrue;
	}

	if (!onePass)
	{
		onePass = sfTrue;
		if (state == INTRO)
		{
			openingScreen_Init();
		}
		if (state == MENU)
		{
			menu_Init(_window);
			options_Init(_window);
		}
		if (state == CINEMATIC_INTRO) {
			cinematicIntro_Init();
		}
		if (state == CINEMATIC_OUTRO) {
			cinematicOutro_Init();
		}
		if (state == CREDITS) {
			credits_Init();
		}
		if (state == EDITOR)
		{
			if (!GetSfTexture("HUD")) Texture_Onload(GAME);
			editor_Init(_window); 
			options_Init(_window);
			initPause(_window);
		}
		if (state == GAME)
		{
			w.mutex = sfMutex_create();
			w.state = sfFalse;
		
			// current_Init();
			npc_Init(_window);
			if (!firstLoadThread) {
				loadingThread = sfThread_create(&initGame, _window);
				sfThread_launch(loadingThread);
				initPause(_window);
				options_Init(_window);
			}
			else {
				loadingThread = sfThread_create(&initGame, _window);
				initPause(_window);
				options_Init(_window);
				sfThread_launch(loadingThread);
			}
		}
		if (state == END)
		{
			
		}
	}
	framXLoading = 0; 
	timerLoading = 0.f; 

	if (state == MENU) {
		initializeView(mainView, (sfFloatRect) { 0.f, 0.f, 1920.f, 1080.f });
		resetView(mainView);
	}
}

void stateEventUpdate(Window* _window, sfEvent* _event)
{
	if (w.state)
	{
			if (state == INTRO)
			{

			}
			else if (state == MENU)
			{
				
			}
			else if (state == GAME)
			{
				
			}
			else if (state == END)
			{

			}
	}
}

void stateUpdate(Window* _window)
{

	if (sfMouse_isButtonPressed(sfMouseXButton1))
		_window->isDone = sfTrue;
	fade_Update();
	if (w.state)
	{
		if (state == INTRO)
		{
			openingScreen_Update(_window);
		}
		else if (state == CINEMATIC_INTRO) {
			cinematicIntro_Update(_window);
		}
		else if (state == CINEMATIC_OUTRO) {
			cinematicOutro_Update(_window);
		}
		else if (state == CREDITS) {
			credits_Update(_window);
		}
		else if (state == MENU)
		{ 
			if (isOption)
				options_Update(_window);
			else
				menu_Update(_window);
//		initializeView(mainView, (sfFloatRect) { 0.f, 0.f, 1920.f, 1080.f }); /// buh
	    resetView(mainView); 
		}
		else if (state == GAME)
		{
			HUD_UpdateGame(_window);
			if (getGameState() == GAME_FISH)
			{
				if (isOption)
					options_Update(_window);
				else
					updateGame(_window);
			}
			else {
				if (isPaused)
				{
					updatePause(_window);
				}
				else
					updateGame(_window);
				// current_Update();
			}
		}
		else if (state == EDITOR)
		{
			if (isPaused)
			{
					updatePause(_window);
			}
			else
				editor_Update(_window);
		}
		else if (state == END)
		{

		}
	}
	else if (!w.state)
	{
		static sfIntRect AnimRect = { 0, 0, 128, 128 };
		static int frame = 0;
		static float timer = 0.0f;
		timer += getDeltaTime();

		sfMutex_lock(w.mutex);

		sfRectangleShape_setSize(spLoading, vector2f(211.f,314.f));
		sfRectangleShape_setOrigin(spLoading, vector2f(211.f / 2, 314.f / 2)); 
		sfRectangleShape_setTextureRect(spLoading, animatorAnim("loadingWalk", &framXLoading, &timerLoading, 0)); 
		sfRectangleShape_setPosition(spLoading, vector2f(mainView->viewPos.x, mainView->viewPos.y));

		sfRectangleShape_setSize(backLoading, vector2f(1920.f, 1080.f)); 
	    sfRectangleShape_setTextureRect(backLoading, animatorAnim("loadingBack", &framXBackLoading, &timerBackLoading, 0)); 
		sfRectangleShape_setPosition(backLoading, vector2f(mainView->viewPos.x, mainView->viewPos.y));

		sfRectangleShape_setSize(shadowLoading, vector2f(670.f , 56.f));
		sfRectangleShape_setTextureRect(shadowLoading, animatorAnim("loadingShadow", &framXShadowLoading, &timerShadowLoading, 0));
		sfRectangleShape_setPosition(shadowLoading, vector2f(mainView->viewPos.x +180, mainView->viewPos.y +170));
		sfMutex_unlock(w.mutex);
	}
	if (SFXVolume > 100.f) SFXVolume = 100.f;
	if (musicVolume > 100.f) musicVolume = 100.f;
}

void stateDisplay(Window* _window)
{
	if (_window->isDone) return;
	if (w.state)
	{
		if (state == INTRO)
		{
			openingScreen_Display(_window);
		}
		if (state == MENU)
		{
			menu_Display(_window);
			if (isOption)
			{
				options_Display(_window);
			}
		}
		if (state == CINEMATIC_INTRO)
		{
			cinematicIntro_Display(_window);
		}
		if (state == CINEMATIC_OUTRO)
		{
			cinematicOutro_Display(_window);
		}
		if (state == CREDITS)
		{
			credits_Display(_window);
		}
		if (state == GAME)
		{
			displayGame(_window);
			if (isPaused)
				displayPause(_window);
			if (getGameState() == GAME_FISH)
			{
				if (isOption)
				{
					options_Display(_window);
				}
			}
			
		}
		if (state == EDITOR)
		{
			editor_Display(_window);
			if (isPaused)
			{
				displayPause(_window);
			}
		}
		if (state == END)
		{

		}
	}
	else if (!w.state)
	{
		sfMutex_lock(w.mutex);
		sfRenderTexture_drawRectangleShape(_window->rdrt, backLoading, NULL); 
		sfRenderTexture_drawRectangleShape(_window->rdrt, shadowLoading, NULL);
		sfRenderTexture_drawRectangleShape(_window->rdrt, spLoading, NULL); 
		sfMutex_unlock(w.mutex);
	}
}

void stateDeinit(Window* _window)
{
	if (state == INTRO)
	{
		openingScreen_Deinit();
	}
	if (state == CINEMATIC_INTRO)
	{
		cinematicIntro_Deinit();
	}
	if (state == CREDITS)
	{
		credits_Deinit();
	}
	if (state == CINEMATIC_OUTRO)
	{
		cinematicOutro_Deinit();
	}
	if (state == MENU)
	{
		menu_Deinit();
		options_Deinit();
	}
	if (state == GAME)
	{
		deinitGame();
		HUD_DeinitGame();
		options_Deinit();
		// expandingTile_Deinit();
		// current_Deinit();
		// deinitNPC();
		map_DeinitMap();
	}
	if (state == EDITOR)
	{
		editor_Deinit();
	}
	if (state == END)
	{

	}
}

void changeState(Window* _window, State _state)
{
	if (state != INTRO) setOnePassMenu(sfTrue);

	if (_state == MENU) {
		setFirstPassPotSpawn();
	}

	stateDeinit(_window);
	RemoveAllTextureButALL();
	RemoveAllSoundsNotNeededInEveryStates();
	state = _state;
	onePass = sfFalse;
	stateInit(_window);
	initializeView(mainView, (sfFloatRect) { 0.f, 0.f, 1920.f, 1080.f }); 
	resetView(mainView); 
}

void togglePause()
{
	SetVibration(0, VIBRATION_NONE);
	shakingView(0.f);
	if (!isPaused) {
		setTimeScale(0.f);
		pauseAllSfx();
	}
	else {
		playAllSfx();
		setTimeScale(1.f);
	}
	isPaused = !isPaused;
}

void toggleOptions()
{
	SetVibration(0, VIBRATION_NONE);
	shakingView(0.f);
	isOption = !isOption;
	if (getState() == MENU) {
		if (isOption) {
			fade_SetOpacity(180);
		}
		else {
			fade_SetOpacity(0);
		}
	}
	options_SetBackToNormal();
}

sfBool getPause()
{
	return isPaused;
}

State getState()
{
	return state;
}

GameState getGameState()
{
	return gameState;
}

void changeGameState(GameState _gameState)
{
	gameState = _gameState;
}

