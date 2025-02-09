#pragma once
#include "tools.h"
#include "windowManager.h"
#include "List.h"

void fishspot_Init();
void fishspot_Update(Window* _window);
void fishspot_Display(Window* _window);
void fishspot_AddSplash(sfVector2f _pos);
void fishspot_Delete(int _index);
void fishspot_Clear();
void fishspot_Deinit();