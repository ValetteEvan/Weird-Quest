#pragma once
#include "controller.h"
#include "stateManager.h"
#include "math.h"
#include "tools.h"
#define MAX_PLAYER 1

typedef enum {
	VIBRATION_NONE = 0,
	VIBRATION_LOW,
	VIBRATION_MEDIUM,
	VIBRATION_HIGH,
	VIBRATION_SINUSOID_LOW,
	VIBRATION_SINUSOID_MEDIUM,
	VIBRATION_SINUSOID_HIGH,
	VIBRATION_IMPACT,
	VIBRATION_WALK,
	VIBRATION_HEARTBEAT,
	VIBRATION_RUMBLE,
	VIBRATION_PULSE,
	VIBRATION_SWEEP,
	VIBRATION_BURST
} VibrationType;

void SetVibration(int _joystickId, VibrationType _vibrationType);

