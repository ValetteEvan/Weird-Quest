#pragma once
#include "tools.h"
#include "windowManager.h"

typedef enum 
{
	RTNULL = -1, 
	ALL,
	INTRO,
	MENU,
	CINEMATIC_INTRO,
	GAME,
	EDITOR,
	CINEMATIC_OUTRO,
	CREDITS,
	END
}State;

typedef enum
{
	GAME_OUTSIDE = 0,
	GAME_DUNGEON,
	GAME_FISH
}GameState;

typedef struct 
{
	sfBool state;
	sfMutex* mutex;
}watcher;

watcher w;

static State state = INTRO;
static GameState gameState = GAME_OUTSIDE;

static sfBool onePass = sfFalse;
static sfBool isPaused = sfFalse;
static sfBool isOption = sfFalse;

/// <summary>
/// init the next state and put a loading screen
/// </summary>
/// <param name="_window">the current window</param>
void stateInit(Window* _window);

/// <summary>
/// react to the events depending on the state
/// </summary>
/// <param name="_window">the current window</param>
/// <param name="_event">the event of the window</param>
void stateEventUpdate(Window* _window, sfEvent* _event);

/// <summary>
/// update the current state
/// </summary>
/// <param name="_window">the current window</param>
void stateUpdate(Window* _window);

/// <summary>
/// display the current state
/// </summary>
/// <param name="_window">the current window</param>
void stateDisplay(Window* _window);

/// <summary>
/// deinit the current state
/// </summary>
/// <param name="_window">the current window</param>
void stateDeinit(Window* _window);

/// <summary>
/// switch the current state to a given state
/// </summary>
/// <param name="_window">the current window</param>
/// <param name="_state">the next state</param>
void changeState(Window* _window, State _state);

/// <summary>
/// change the pause state
/// </summary>
void togglePause();

/// <summary>
/// change the option state
/// </summary>
void toggleOptions();

/// <summary>
/// check if the game is paused
/// </summary>
/// <returns>sftrue if the game is paused sffalse if it's not</returns>
sfBool getPause();

/// <summary>
/// get the current state
/// </summary>
/// <returns>the current state</returns>
State getState();

/// <summary>
/// get the current game state
/// </summary>
/// <returns>the current game state</returns>
GameState getGameState();

/// <summary>
/// switch the current state to a given state
/// </summary>
/// <param name="_window">the current window</param>
/// <param name="_state">the next state</param>
void changeGameState(GameState _gameState);

/// <summary>
/// get the current option boolean state
/// </summary>
sfBool getOptions();
