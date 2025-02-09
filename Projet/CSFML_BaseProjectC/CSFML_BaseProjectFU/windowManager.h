#pragma once
#include "tools.h"


// Variables
typedef struct 
{
	sfRenderWindow* renderWindow;
	sfBool isDone;
	sfBool isFullscreen;
	char* windowTitle;
	sfVideoMode videoMode;
	sfVector2i defaultVideoMode;
	sfEvent event;
	sfRenderTexture* rdrt;
	sfRenderTexture* rdrtShader;
	sfSprite* spr;
	sfRenderStates* rs;
	sfRenderStates* rsShader;
}Window;
sfBool fullscreen_IsDone;

// Prototypes

/// <summary>
/// tell if the window is closed
/// </summary>
/// <param name="This">the window used</param>
/// <returns>true if the window is closed, false if it's still open</returns>
sfBool IsDone(Window* This);

/// <summary>
/// check if the game is in full screen
/// </summary>
/// <param name="This">the window used </param>
/// <returns>true if the game is in full screen, false if it's not</returns>
sfBool IsFullscreen(Window* This);

/// <summary>
/// fill the window with black
/// </summary>
/// <param name="This">the window used</param>
void BeginDraw(Window* This);

/// <summary>
/// display the changes on the window
/// </summary>
/// <param name="This">the window used</param>
void EndDraw(Window* This);

/// <summary>
/// create a window
/// </summary>
/// <param name="_Title">title of the window</param>
/// <param name="_defaultVideoMode">the size of the window</param>
/// <returns>the window initialized</returns>
Window* windowSetup(const char* _Title, sfVector2i _defaultVideoMode);

/// <summary>
/// destroy the window
/// </summary>
/// <param name="This">the window used</param>
void windowDestroy(Window* This);

/// <summary>
/// create a render window with a setup window
/// </summary>
/// <param name="This">the window used</param>
void windowCreate(Window* This);

/// <summary>
/// change the full screen mode
/// </summary>
/// <param name="This">the window used</param>
void ToggleFullscreen(Window* This);

/// <summary>
/// init the window depending on the current state
/// </summary>
/// <param name="This">the window used</param>
void windowInit(Window* This);

/// <summary>
/// update the window depending on the current state
/// </summary>
/// <param name="This">the window used</param>
void windowUpdate(Window* This);

/// <summary>
/// display the game depending on the current state
/// </summary>
/// <param name="This">the window used</param>
void windowDraw(Window* This);

/// \return The sfShader object associated with this render window
sfShader* windowGetShader(Window* This);

void windowReadSettings();