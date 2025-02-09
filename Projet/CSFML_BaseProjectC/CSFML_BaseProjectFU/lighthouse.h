/// Function prefix: loclh_

#pragma once
#include "tools.h"
#include "math.h"
#include "windowManager.h"
#include "textureManager.h"
#include "map.h"

void loclh_Init(Window* _window);
void loclh_Update(Window* _window);
void loclh_DisplayBG(Window* _window);
void loclh_DisplayFG(Window* _window);
void loclh_Deinit();

void lhloc_Begin(Window* _window);

void loclh_UpdateBedroom(Window* _window);
void loclh_DisplayBedroomBG(Window* _window);
void loclh_DisplayBedroomFG(Window* _window);
void loclh_UpdateHall(Window* _window);

void loclh_UpdateAntechamber(Window* _window);
void loclh_DisplayAntechamber(Window* _window);
void loclh_DisplayHallBG(Window* _window);
void loclh_DisplayHallFG(Window* _window);
void loclh_UpdateOut(Window* _window);
void loclh_DisplayOutBG(Window* _window);
void loclh_DisplayOutFG(Window* _window);
void loclh_UpdateMentor(Window* _window);
void loclh_DisplayMentorBG(Window* _window);
void loclh_DisplayMentorFG(Window* _window);

void loclh_HallReturnTable();
void loclh_DisplayFlowerOut(Window* _window, sfVector2f _pos, int _frameOff);

sfBool loclh_Jumped;