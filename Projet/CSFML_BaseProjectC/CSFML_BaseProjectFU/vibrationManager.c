#include "vibrationManager.h"

static float timerVibration = 0.0f;
static float leftIntensity = 0.0f;
static float rightIntensity = 0.0f;
static sfBool SinusAction = sfTrue;

#define VIBRATION_INTERVAL 0.5f

void ApplySinusoidVibration(int _joystickId, float increment, float maxIntensity) {
	timerVibration += getDeltaTime();
	if (timerVibration > VIBRATION_INTERVAL && SinusAction) {
		leftIntensity += increment;
		rightIntensity += increment;
		timerVibration = 0.0f;
	}
	else if (timerVibration > VIBRATION_INTERVAL && !SinusAction) {
		leftIntensity -= increment;
		rightIntensity -= increment;
		timerVibration = 0.0f;
	}

	if (leftIntensity >= maxIntensity && rightIntensity >= maxIntensity) {
		SinusAction = sfFalse;
	}
	else if (leftIntensity <= 0.0f && rightIntensity <= 0.0f) {
		SinusAction = sfTrue;
	}

	setControlerVibration(_joystickId, leftIntensity, rightIntensity);
}

void ApplyHeartbeatVibration(int _joystickId) {
	timerVibration += getDeltaTime();
	if (timerVibration > 1.0f) {
		setControlerVibration(_joystickId, 0.8f, 0.8f);
		timerVibration = 0.0f;
	}
	else if (timerVibration > 0.2f) {
		setControlerVibration(_joystickId, 0.0f, 0.0f);
	}
}

void ApplyRumbleVibration(int _joystickId) {
	timerVibration += getDeltaTime();
	float randomIntensity = (rand() % 100) / 100.0f * 0.5f + 0.5f; // random intensity between 0.5 and 1.0
	setControlerVibration(_joystickId, randomIntensity, randomIntensity);
}

void ApplyPulseVibration(int _joystickId) {
	timerVibration += getDeltaTime();
	if ((int)(timerVibration * 10) % 2 == 0) {
		setControlerVibration(_joystickId, 0.8f, 0.8f);
	}
	else {
		setControlerVibration(_joystickId, 0.0f, 0.0f);
	}
}

void ApplySweepVibration(int _joystickId) {
	timerVibration += getDeltaTime();
	float intensity = (sin(timerVibration * 2.0f * 3.14159f) + 1.0f) / 2.0f; // sweep from 0 to 1
	setControlerVibration(_joystickId, intensity, intensity);
}

void ApplyBurstVibration(int _joystickId) {
	timerVibration += getDeltaTime();
	if (timerVibration < 0.1f) {
		setControlerVibration(_joystickId, 1.0f, 1.0f);
	}
	else if (timerVibration < 0.3f) {
		setControlerVibration(_joystickId, 0.5f, 0.5f);
	}
	else if (timerVibration < 0.5f) {
		setControlerVibration(_joystickId, 0.2f, 0.2f);
	}
	else {
		timerVibration = 0.0f;
		setControlerVibration(_joystickId, 0.0f, 0.0f);
	}
}

void SetVibration(int _joystickId, VibrationType _vibrationType) {
	switch (_vibrationType) {
	case VIBRATION_NONE:
		setControlerVibration(_joystickId, 0.0f, 0.0f);
		break;
	case VIBRATION_LOW:
		setControlerVibration(_joystickId, 0.7f, 0.7f);
		break;
	case VIBRATION_MEDIUM:
		setControlerVibration(_joystickId, 0.35f, 0.35f);
		break;
	case VIBRATION_HIGH:
		setControlerVibration(_joystickId, 0.8f, 0.8f);
		break;
	case VIBRATION_SINUSOID_LOW:
		ApplySinusoidVibration(_joystickId, 0.01f, 0.3f);
		break;
	case VIBRATION_SINUSOID_MEDIUM:
		ApplySinusoidVibration(_joystickId, 0.05f, 0.5f);
		break;
	case VIBRATION_SINUSOID_HIGH:
		ApplySinusoidVibration(_joystickId, 0.1f, 1.0f);
		break;
	case VIBRATION_IMPACT:
		setControlerVibration(_joystickId, 0.0118f, 0.0118f);
		break;
	case VIBRATION_WALK:
		timerVibration += getDeltaTime();
		if (timerVibration < 0.2f) {
			setControlerVibration(_joystickId, 0.0118f, 0.0118f);
		}
		else if (timerVibration < 0.4f) {
			setControlerVibration(_joystickId, 0.0f, 0.0f);
		}
		else {
			timerVibration = 0.0f;
		}
		break;
	case VIBRATION_HEARTBEAT:
		ApplyHeartbeatVibration(_joystickId);
		break;
	case VIBRATION_RUMBLE:
		ApplyRumbleVibration(_joystickId);
		break;
	case VIBRATION_PULSE:
		ApplyPulseVibration(_joystickId);
		break;
	case VIBRATION_SWEEP:
		ApplySweepVibration(_joystickId);
		break;
	case VIBRATION_BURST:
		ApplyBurstVibration(_joystickId);
		break;
	default:
		setControlerVibration(_joystickId, 0.0f, 0.0f);
		break;
	}
}