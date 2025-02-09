#pragma once
#include "tools.h"
#include "windowManager.h"
#include "List.h"

#define EXPT_GETDATA STD_LIST_GETDATA(expt_List, exptData, i) /// Shortcut for fetching LL data

typedef struct exptData exptData; /// Data storage for expandable tiles

/// Data storage for expandable tiles
/// An expandable tile is virtually made up of two entities - a base and an endpoint.
/// The base is immovable, while the endpoint can be ring'd around into expanding or retracting the tile bridge.
/// The function expt_GetEndpointBounds() allows to fetch the endpoint's hitbox, and the function expt_GetSurfaceBounds() allows to fetch the hitbox of the entire set.
struct exptData {
	sfVector2f pos; /// Position of the tile's base (analog)
	sfVector2i posSnap; /// Position of the tile's base (snapped onto the tile grid)
	sfVector2f posEndpoint; /// Position of the tile's endpoint
	Direction dir; /// Direction in which the object can be expanded
	sfVector2f spd; /// Movement speed of the tile's base
};

typedef struct{
	sfVector2f pos;
	Direction dir; 
}ExptToSave;

stdList* expt_List;

/// Initializes expandable tiles.
void expt_Init();

/// Creates a new expandable tile at the given position and with the given direction.
exptData* expt_Create(sfVector2f _pos, Direction _dir);

/// Updates expandable tiles.
void expt_Update();

/// Renders expandable tiles.
void expt_Display(Window* _window);

/// Deinitializes expandable tiles and frees up allocated memory.
void expt_Deinit();

/// \return A grid position in tiles corresponding to the passed pixel coordinates
sfVector2i expt_SnapPos(sfVector2f _pos);

/// \return The expandable tile closest to the passed pixel coordinates
exptData* expt_GetClosest(sfVector2f _pos);

/// \return The hitbox of the endpoint of the passed expandable tile
sfFloatRect expt_GetEndpointBounds(exptData* _expt);

/// \return The hitbox of the entire surface of the passed expandable tile.
/// Suitable for collision computing.
sfFloatRect expt_GetSurfaceBounds(exptData* _expt);

/// \return Whether the passed position is close enough to the passed expandable tile to consider they're superposed.
/// Used to detect whether a retracting one has retracted all the way back.
sfBool expt_CheckWedgeStart(sfVector2f _pos, exptData* _expt);

/// \return Whether the passed position is too far from the passed expandable tile to keep expanding.
/// Used to detect whether an expanding one has reached its limit.
sfBool expt_CheckWedgeEnd(sfVector2f _pos, exptData* _expt);

void expt_Attract(exptData* _expt);
void expt_Repulse(exptData* _expt);
void expt_Stasis(exptData* _expt);
void expt_Destroy(exptData* _expt);

ExptToSave* getExptListToSave();
void expt_clearList();