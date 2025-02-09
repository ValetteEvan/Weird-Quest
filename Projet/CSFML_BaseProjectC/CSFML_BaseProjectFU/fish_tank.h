/// Function prefix: jar_

#pragma once
#include "tools.h"
#include "windowManager.h"
#include "List.h"


typedef enum { BOCAL_JOY, BOCAL_SADNESS, BOCAL_FEAR, BOCAL_ANGER} wichBocal;

typedef struct {
	int wichfish;
	sfVector2f pos;
	sfVector2f spawnPoint;
	sfVector2f maxPos;
	float rotation;
	float timerAnim;
	int frameX;
	sfVector2f forward;
	sfVector2f origin;
	sfBool toSpawn;
} tank;

stdList* tank_List;

#define TANK_GETDATA STD_LIST_GETDATA(tank_List, tank, i)

/// Initializes fish fountains.
void tank_Init();

/// Creates a new fish fountain.
/// \param _fish_type - The type of fish this fountain is intended to host
void tank_CreateFish(int _fish_type);

/// Updates fish fountains.
void tank_Update(Window* _window);

/// Renders fish fountains.
void tank_Display(Window* _window);

void tank_SetReleaseStatus(sfBool _status);


