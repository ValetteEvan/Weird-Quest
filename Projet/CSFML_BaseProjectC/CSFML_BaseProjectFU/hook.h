#pragma once
#include "tools.h"
#include "windowManager.h"

#define FISH_WATER_Y_POS 380.f
#define FISH_GROUND_X_POS 1620.f

#define HOOK_X_SIZE 50.f
#define HOOK_Y_SIZE 50.f

#define HOOK_X_ORIGIN HOOK_X_SIZE / 2
#define HOOK_Y_ORIGIN HOOK_Y_SIZE / 2

#define FISH_CAPTURE_ZONE_X_POS 0.f		// now random
#define FISH_CAPTURE_ZONE_Y_POS 0.f		
#define FISH_CAPTURE_ZONE_X_SIZE 130.f	
#define FISH_CAPTURE_ZONE_Y_SIZE 130.f	

#define HOOK_RESET_POS vector2f(960.f, 50.f)
#define HOOK_RESET_DURATION 1.5f

typedef struct {
	sfVector2f velocity;
	float speed;
	sfVector2f pos;
	sfIntRect animRect;
	sfVector2f forward;
	sfVector2f oldForward;
	float animTimer;
	sfBool isReseting;
	sfBool starting;
	sfVector2f resetPos;
	sfVector2f startPos;
	float resetTimer;
}Hook;

typedef struct {
	sfVector2f pos;
	int id;
}Hook_Shell;

typedef struct {
	sfVector2f pos;
	int id;
}Hook_Cloud;

typedef struct {
	sfVector2f pos;
	int id;
}Hook_Coral;

typedef struct {
	sfVector2f pos;
	int id;
	sfBool isGrouped;
	float timerSeaweed;
	int frameSeaweed;
}Hook_Seaweed;

typedef struct {
	sfVector2f pos;
	int id;
}Hook_Rock;

typedef struct {
	sfVector2f pos;
	float timerSplash;
	int frameSplash;
}Hook_Splash;

typedef enum {
	HOOK_SHELL,
	HOOK_CLOUD,
	HOOK_CORAL,
	HOOK_SEAWEED, 
	HOOK_ROCK
}Hook_enumType;

typedef enum {
	HOOK_RECEPTION_IDLE,
	HOOK_RECEPTION_HAPPY,
	HOOK_RECEPTION_SAD
}Hook_Emotion;

void hook_InitHook(Window* _window);

void hook_InitHookTuto(Window* _window);

void hook_UpdateHook(Window* _window);

void hook_DisplayHook(Window* _window);

sfVector2f hook_GetHookPos();

sfBool hook_CanBeCaptured(sfVector2f _fishPos);

void hook_ResetHook();

sfBool hook_IsReseting();

sfBool hook_IsFishEatingHook(sfFloatRect _fishBounds);

void hook_RandomCaptureZone();

void hook_TutoCaptureZone();

float hook_ReturnCaptureZoneX();

float hook_GetMaxYPos();

void hook_DeinitHook();

void hook_ToggleMovement();

void hook_setMovement(sfBool _movement);

float hook_RandomXPos();

void hook_RandomSpawn(int _nbelemets, Hook_enumType _type);

/**
 * @brief Adds a shell to the hook.
 * @param _id The id of the shell to be added.
 */
void hook_AddSheel(int _id); // equivalent of the prepare elements

/**
 * @brief Adds a cloud to the hook.
 * @param _id The id of the cloud to be added.
 */
void hook_AddCloud(int _id); // equivalent of the prepare elements

/**
 * @brief Adds a coral to the hook.
 * @param _id The id of the coral to be added.
 */
void hook_AddCoral(int _id); // equivalent of the prepare elements

/**
 * @brief Adds a seaweed to the hook.
 * @param _id The id of the seaweed to be added.
 */
void hook_AddSeaweed(int _id); // equivalent of the prepare elements


void hook_AddRock(int _id);

/**
 * @brief Displays all the elements attached to the hook.
 * @param _window The window where the hook elements will be displayed.
 */
void hook_DisplayHookElements(Window* _window);

/**
 * @brief Displays the seaweed attached to the hook.
 * @param _window The window where the seaweed will be displayed.
 */
void hook_DisplaySeaweed(Window* _window);

void hook_SetPowerRing();

void hook_ResetPowerRing();

void hook_SetBubblePos(sfVector2f _pos);

void hook_ResetBubble();

void hook_addSplash(sfVector2f _pos);

void Hook_ChangeReceptionEmotion(Hook_Emotion _emotion);

void AttractToggle(sfBool _attract);

sfBool fish_GiveFirstTimeDrop();