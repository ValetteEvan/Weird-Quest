#pragma once 
#include "tools.h"
#include"windowManager.h"
#include "List.h"

typedef enum {
	SMALL_TEAR,
	BIG_TEAR
}Tears_Type;

typedef struct {
	sfVector2f position;
	sfVector2f startPosition;
	sfVector2f targetPosition;
	sfFloatRect colRect;
	sfIntRect rect;
	float radius;
	float timeOfLerp;
	float timer_radius;
	float scale;
	int frameX;
	float timerAnim;
	float angle;
	int opacity;
	Tears_Type type;
	sfBool isDead;
}Tears_struct;

stdList* tearsList;

void trs_initTears();
void trs_addTears(sfVector2f _startPosition, sfVector2f _endPosition, float _timeOfLerp, Tears_Type _type);
void trs_Update(Window* _window);
void trs_displayTears(Window* _window);
void trs_displayDeadTears(Window* _window);
void trs_Deinit();