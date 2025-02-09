#pragma once
#include "tools.h"
#include "windowManager.h"

#define FISH_ANGER_STUNT_DURATION 6.f
#define NBR_OF_ROCKS 9

#define FISH_POWER_EFFECT_RADIUS 300.f
#define FISH_POWER_EFFECT_COOLDOWN 3.f
#define FISH_EFFECT_DURATION 1.f

#define FISH_ALPHA_MIN_Y_POS_FOV 500.f

#define FISH_SCARED_BORED_DURATION 100.f

int fish_IdFishingSpot;

typedef enum {
	FISH_JOY,
	FISH_JOY_TUTO,
	FISH_SADNESS,
	FISH_SADNESS_TUTO,
	FISH_SCARED,
	FISH_SCARED_TUTO,
	FISH_ANGER,
	FISH_ANGER_TUTO
}FishType;

void fish_InitFish(FishType _type, Window* _window);
void fish_UpdateFish(Window* _window);
void fish_DisplayFish(Window* _window);
void fish_SetupFish(FishType _type);
void fish_pushRock();
void fish_LookingForHome();
sfBool fish_IsInRadius(sfVector2f _hookPos);
void fish_SetPowerTimer(float _timer);
FishType fish_GetFishType();
sfVector2f fish_GetDefaultPos();
void fish_DeinitFish();
void fish_changeReadyToUsed();
void fish_StopPower();
sfBool fish_isOutOfTheWater(sfFloatRect _fishBounds);
void toogle_HaveToStart();
void fish_HaveToReload(Window* _window);