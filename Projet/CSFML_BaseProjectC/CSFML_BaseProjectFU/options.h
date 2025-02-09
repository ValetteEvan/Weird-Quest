#pragma once
#include "tools.h"
#include "windowManager.h"

typedef struct {
	sfBool fullScreen;
	float musicVol;
	float sfxVol;
}settings;

settings seting;

/// <summary>
/// init all the variables needed for the options
/// </summary>
/// <param name="_window">the current window</param>
void options_Init(Window* _window);

/// <summary>
/// update the variables needed in the options for visual feedbacks and change the values in the options
/// </summary>
/// <param name="_window">the current window</param>
void options_Update(Window* _window);

/// <summary>
/// 
/// </summary>
/// <param name="_window">the current window</param>
void options_Display(Window* _window);

/// <summary>
/// deinit all the variables from the options that are not needed for other states
/// </summary>
void options_Deinit();

/// <summary>
/// sets everything back to normal when options are toggled DO NOT USE OUTSIDE OF TOGGLEOPTIONS() !!!
/// </summary>
void options_SetBackToNormal();