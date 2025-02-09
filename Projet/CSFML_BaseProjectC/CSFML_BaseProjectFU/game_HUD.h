#pragma once
#include "tools.h"
#include "windowManager.h"

typedef enum HUD_Added_Data {
	HUD_INVENTORY,
	HUD_BESTIARY_BESTIARY,
	HUD_BESTIARY_DEMON,
	HUD_QUEST
} HUD_Added_Data;

HUD_Added_Data HUD_typeOfAddedData;

/// <summary>
/// Load hud texture
/// </summary>
void HUD_loadTexture();

/// <summary>
/// inits the game's HUD
/// </summary>
void HUD_InitGame();

/// <summary>
/// updates the game's HUD
/// </summary>
/// <param name="_window">the current window</param>
void HUD_UpdateGame(Window* _window);

/// <summary>
/// displays the game's HUD
/// </summary>
/// <param name="_window">the current window</param>
void HUD_DisplayGame(Window* _window);

/// <summary>
/// destroys every part of the game's HUD if the game state is left
/// </summary>
void HUD_DeinitGame();

void HUD_AddedDataAppered(Window* _window);

void HUD_toggleAddedDataAppered(sfBool _playSfx);

/// <summary>
/// displays the game's HUD a interact button at a determined position
/// </summary>
/// <param name="_window">the current window</param>
/// <param name="_position">the position you want it in</param>
void HUD_DisplayPressA(sfVector2f _position, Window* _window);

void HUD_addKey();

int getNbJoyKey();


