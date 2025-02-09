#pragma once
#include "tools.h"
#include "windowManager.h"
#include "List.h"

typedef struct {
	sfVector2f pos;
	sfVector2f destinationPos;
	float timer;
	sfVector2f startPos;
	float timeScale;
	float timeScale2;
	float mass;
	float timerUnscaled;
	float timerAnim;
	int frameX;
	sfIntRect rect;
	sfBool hasFallen;
}BoxData;

typedef struct {
	sfVector2f pos;
}BoxDataToSave;

stdList* boxList;

#define BOX_X_SIZE 64
#define BOX_Y_SIZE 128

#define BOX_GETDATA STD_LIST_GETDATA(boxList, BoxData, i)

/// Initializes the pushable box system.
void box_Init();

/// Updates the pushable box system.
void box_Update();

/// Displays boxes.
void box_Display(Window* _window);
void box_DisplayAbove(Window* _window);


/// Constructs and adds a box to the box list.
void box_Create(sfVector2f _pos);

/// Predictively tests collisions between the player and the given box.
/// \param _box - The box to test collision with
/// \param _plr - The player's current movement vector
sfBool box_PlayerCollision(BoxData* _box, sfVector2f _vitesse_plr);

/// Predictively tests collisions between a box and all others.
/// \param _box - The box to test collision with
/// \param _vitesse - The box's current movement vector
sfBool box_BoxCollision(BoxData* _box, sfVector2f _vitesse);

/// Gets the bounding box of the given box.
sfFloatRect box_GetBounds(BoxData* _box);

sfVector2f getStuntBoxOffset(sfVector2f _ringPos, BoxData* _box);
sfVector2f getStuntBoxPos(sfVector2f _offset, BoxData* _box);

/// Applies the attract ring power's effect onto a box.
void box_Attract(BoxData* _box);

/// Applies the repulse ring power's effect onto a box.
void box_Repulse(BoxData* _box);

/// Applies the stasis ring power's effect onto a box.
void box_Stasis(BoxData* _box);

/// Applies the destruction ring power's effect onto a box. Or removes a box. This function can be used for both purposes, actually.
void box_Destroy(BoxData* _box);

/// Clears the box buffer.
void box_Clear();
void box_AttachRing(sfBool _bool, BoxData* _box);

/// Attempts to push a box.
/// \param _pushDir - Direction in which the player is currently pushing the box
void PushBox(sfVector2f _pushDir, BoxData* _box);

/// Applies the movement applied by the lunge onto the box.
/// \param _pushDir - Player's movement direction
void ShoulderHitBox(sfVector2f _pushDir, BoxData* _box);

/// \return Array of a box's 4 corner positions, for collision detection purposes
sfVector2f* getBoxCollisionPoints(BoxData* _box);

/// \return One of a box's 4 corner positions, specified by _point (0 = top left, 1 = top right, 2 = bottom left, 3 = bottom right)
sfVector2f getBoxCollisionSinglePoint(int _point, BoxData* _box);

/// \return The box that's closest to the specified position
BoxData* getClosestBox(sfVector2f _pos);

/// Fetches the entirety of the box list's data for saving in the map file.
BoxDataToSave* getBoxListToSave();
