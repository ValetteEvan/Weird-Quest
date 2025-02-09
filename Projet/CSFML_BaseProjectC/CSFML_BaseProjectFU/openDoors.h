#pragma once
#include "tools.h"
#include "textureManager.h"
#include "List.h"

typedef enum {
	DOOR,
	DOOR_TOP,
	DOOR_CLOSED,
	DOOR_CLOSED_TOP,
	DOOR_BOSS,
	DOOR_BOSS_TOP
}PropsDoor;
/// Enum potentiellement inutile si ignore du visuel de l'editeur
typedef enum {
	DOOR_AXE_NEUTRAL = -1,
	DOOR_AXE_TOP,
	DOOR_AXE_DOWN,
	DOOR_AXE_LEFT,
	DOOR_AXE_RIGHT
	
}DoorOrientation;

typedef struct {
	PropsDoor type;
	sfVector2f position;
	DoorOrientation doorOrientation;
	float timer;
	sfBool isOpen;
} opDoors_DoorsList;

stdList* opDoors_listDoors;

/// <summary>
/// function to load texture
/// </summary>
void opDoors_loadTexture();
/// <summary>
/// openDoors variable initialisation function
/// </summary>
/// <param name="_window"></param>
void opDoors_InitDoor(Window* _window);
/// <summary>
/// function for interactions with the openDoors
/// </summary>
/// <param name="_window"></param>
void opDoors_UpdateDoor(Window* _window);
/// <summary>
/// function of the openDoors visual in front the player
/// </summary>
/// <param name="_window"></param>
void opDoors_DisplayDoor(Window* _window);
/// <summary>
/// free memory
/// </summary>
void opDoors_DeinitDoor();

/// <summary>
/// function add doors
/// </summary>
/// <param name="_type"></param>
/// <param name="_props"></param>
/// <param name="_pos"></param>
/// <param name="_load"></param>
void opDoors_addDoors(PropsDoor _type, sfVector2f _pos);

/// <summary>
/// function delete doors
/// </summary>
/// <param name="_pos"></param>
void opDoors_delDoors(int _index);
/// <summary>
/// functions for emptying the chained list of doors
/// </summary>
void opDoors_clearDoorsList();

void setOnePassOPDoors();