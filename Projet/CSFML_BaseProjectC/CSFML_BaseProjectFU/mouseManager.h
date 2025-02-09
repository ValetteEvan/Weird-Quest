#pragma once
#include "tools.h"

sfBool leftClick(sfRenderWindow* _window);
sfBool rightClick(sfRenderWindow* _window);

void MouseScrollWheel(sfEvent event);
int getMouseScrollWheel();

sfVector2f getfMousePos(sfRenderWindow* _window);
sfVector2f getfWorldMousePos(sfRenderWindow* _window);