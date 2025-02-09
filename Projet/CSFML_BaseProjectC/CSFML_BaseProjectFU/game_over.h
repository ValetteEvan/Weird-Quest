#pragma once
#include "tools.h"
#include "windowManager.h"

static sfBool gameOverScreen = sfFalse;

void gameOver_init();
void gameOver_display(Window* _w);
void toggle_gameOver();
sfBool get_gameOverState();