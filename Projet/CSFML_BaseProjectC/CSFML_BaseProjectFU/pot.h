/// Function prefix: pot_

#pragma once
#include "tools.h"
#include "windowManager.h"
#include "List.h"

typedef enum { POT_REG, POT_HARD } PotType;

typedef struct {
	PotType type;
	sfVector2f pos;
	sfVector2f spd;
	float mass;
	int hp;
	sfBool isBroken;
	int animFrame;
	float timerBroken;
	sfBool canHaveKey;
	sfBool haveKey;
	sfBool isOnScreen;
} PotData;

typedef struct {
	PotType type;
	sfVector2f pos;
} PotToSave;

stdList* pot_List;

#define POT_GETDATA STD_LIST_GETDATA(pot_List, PotData, i)

#define POT_X_SIZE 64
#define POT_Y_SIZE 64

/// Initializes the breakable pot system.
void pot_Init();

/// Updates the breakable pot system.
void pot_Update();

/// Displays pots.
void pot_Display(Window* _window);

/// Constructs and adds a pot to the pot list.
/// \param _pos - Position where the pot will be created
/// \param _pos_snap - Whether the previously given position should be snapped onto the tile grid
void pot_Create(sfVector2f _pos, sfBool _pos_snap, PotType _type);

/// Predictively tests collisions between the player and the given pot.
/// \param _pot - The pot to test collision with
/// \param _plr - The player's current movement vector
sfBool pot_PlayerCollision(PotData* _pot, sfVector2f _vitesse_plr);

/// Breaks a given pot.
void pot_Break(PotData* _pot);

/// \return An array of pot data holders, for map saving purposes
PotToSave* getPotListToSave();

/// Clears the pot buffer.
void pot_Clear();

/// \return Collisions bounds of the given pot
sfFloatRect pot_GetBounds(PotData* _pot);