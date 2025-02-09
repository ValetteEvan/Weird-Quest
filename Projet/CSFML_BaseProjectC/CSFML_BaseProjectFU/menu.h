/// The main menu.
/// Function prefix: menu_

#pragma once
#include "tools.h"
#include "stateManager.h"
#include "windowManager.h"

/// Initializes the main menu.
void menu_Init();

/// Updates the main menu.
void menu_Update(Window* _window);

/// Renders the main menu.
void menu_Display(Window* _window);

/// Deinits the main menu and frees up allocated memory.
void menu_Deinit();

void setOnePassMenu(sfBool _value);

sfBool getIsNewGame();
