#pragma once
#include "tools.h"
#include "windowManager.h"

sfMutex* mutexThread2;
sfMutex* mutexThread3;

/// <summary>
/// init all the variables necessary for the state game
/// </summary>
/// <param name="_window">the current window</param>
void initGame(Window* _window);

/// <summary>
/// update all the variables necessary to play
/// </summary>
/// <param name="_window">the current window</param>
void updateGame(Window* _window);

/// <summary>
/// display the game
/// </summary>
/// <param name="_window">the current window</param>
void displayGame(Window* _window);

/// <summary>
/// deinit all the variables used only in the state game
/// </summary>
void deinitGame();

void game_ChangeFromFishToGame();
void game_ChangeFromGameToFish();
void game_ChangeFromGameToFishTuto();