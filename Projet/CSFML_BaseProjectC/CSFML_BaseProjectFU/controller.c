#include "controller.h"
#include "stateManager.h"
#include "math.h"
#include "windowManager.h"
#define MAX_PLAYER 1

int buttoncount = 14;

// This define makes your program compile faster by excluding things we are not using
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#if (_WIN32_WINNT >= 0x0602 )
#include <XInput.h>
#pragma comment(lib,"xinput.lib")
#else
#include <XInput.h>
#pragma comment(lib,"xinput9_1_0.lib")
#endif


void GamepadDetection()
{
	sfJoystick_update();
	playernber = 0;
	for (unsigned int i = 0; i < 8; i++)
	{
		if (playernber >= MAX_PLAYER) break;
		if (sfJoystick_isConnected(i) == sfTrue)
			playernber++;
	}
}

void GamepadManager(int _joystickId, sfEvent event, sfRenderWindow* _window)
{
	if (event.type == sfEvtJoystickConnected) {
		// printf("Gamepad is Connected id %d\n", event.joystickConnect.joystickId);
		GamepadDetection();
	}
	if (event.type == sfEvtJoystickDisconnected) {
		// printf("Gamepad is Disconnected id %d\n", event.joystickConnect.joystickId);
		if (!getPause() && getState() == GAME) togglePause();
	}
	if (fullscreen_IsDone) {
		if (event.joystickButton.joystickId == _joystickId && sfRenderWindow_hasFocus(_window)) {
			XINPUT_STATE state;
			ZeroMemory(&state, sizeof(XINPUT_STATE));

			XInputGetState(_joystickId, &state);

			if ((state.Gamepad.sThumbLX < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
				state.Gamepad.sThumbLX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) &&
				(state.Gamepad.sThumbLY < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
					state.Gamepad.sThumbLY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)) {

				state.Gamepad.sThumbLX = 0;
				state.Gamepad.sThumbLY = 0;

			}

			if ((state.Gamepad.sThumbRX < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
				state.Gamepad.sThumbRX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) &&
				(state.Gamepad.sThumbRY < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
					state.Gamepad.sThumbRY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)) {

				state.Gamepad.sThumbRX = 0;
				state.Gamepad.sThumbRY = 0;

			}

			Gamepad[_joystickId].stickL.x = (float)(state.Gamepad.sThumbLX / 327);
			Gamepad[_joystickId].stickL.y = (float)(state.Gamepad.sThumbLY / 327);
			Gamepad[_joystickId].stickR.x = (float)(state.Gamepad.sThumbRX / 327);
			Gamepad[_joystickId].stickR.y = (float)(state.Gamepad.sThumbRY / 327);

			if ((state.Gamepad.wButtons & DPAD_RIGHT)) {
				Gamepad[_joystickId].cross.x = 100.f;
			}
			else if ((state.Gamepad.wButtons & DPAD_LEFT)) {
				Gamepad[_joystickId].cross.x = -100.f;
			}
			else {
				Gamepad[_joystickId].cross.x = 0.f;
			}

			if ((state.Gamepad.wButtons & DPAD_UP)) {
				Gamepad[_joystickId].cross.y = 100.f;
			}
			else if ((state.Gamepad.wButtons & DPAD_DOWN)) {
				Gamepad[_joystickId].cross.y = -100.f;
			}
			else {
				Gamepad[_joystickId].cross.y = 0.f;
			}

			Gamepad[_joystickId].TriggerL = (float)(state.Gamepad.bLeftTrigger) / 255;
			Gamepad[_joystickId].TriggerR = (float)(state.Gamepad.bRightTrigger) / 255;

			Gamepad[_joystickId].Bouton[0] = (state.Gamepad.wButtons & A);
			Gamepad[_joystickId].Bouton[1] = (state.Gamepad.wButtons & B);
			Gamepad[_joystickId].Bouton[2] = (state.Gamepad.wButtons & X);
			Gamepad[_joystickId].Bouton[3] = (state.Gamepad.wButtons & Y);
			Gamepad[_joystickId].Bouton[4] = (state.Gamepad.wButtons & LB);
			Gamepad[_joystickId].Bouton[5] = (state.Gamepad.wButtons & RB);
			Gamepad[_joystickId].Bouton[6] = (state.Gamepad.wButtons & START);
			Gamepad[_joystickId].Bouton[7] = (state.Gamepad.wButtons & BACK);
			Gamepad[_joystickId].Bouton[8] = (state.Gamepad.wButtons & LEFT_THUMB);
			Gamepad[_joystickId].Bouton[9] = (state.Gamepad.wButtons & RIGHT_THUMB);
			Gamepad[_joystickId].Bouton[10] = 0;
			Gamepad[_joystickId].Bouton[11] = 0;
			Gamepad[_joystickId].Bouton[12] = 0;
			Gamepad[_joystickId].Bouton[13] = 0;


		}
		else {
			Gamepad[_joystickId].stickL.x = 0.0f;
			Gamepad[_joystickId].stickL.y = 0.0f;
			Gamepad[_joystickId].stickR.y = 0.0f;
			Gamepad[_joystickId].stickR.x = 0.0f;
			Gamepad[_joystickId].cross.x = 0.0f;
			Gamepad[_joystickId].cross.y = 0.0f;
		}
	}
}

int Gamepad_isButtonPressed(unsigned int _joystickId, gamepadXBOX _button, sfRenderWindow* _window)
{
	if (_window != NULL)
	{
		if (fullscreen_IsDone) {
			if (sfRenderWindow_hasFocus(_window))
			{
				XINPUT_STATE state;
				ZeroMemory(&state, sizeof(XINPUT_STATE));

				XInputGetState(_joystickId, &state);
				return (state.Gamepad.wButtons & _button);
			}
		}
	}
	return sfFalse;
}

int Gamepad_isButtonPressedNF(unsigned int _joystickId, gamepadXBOX _button)
{
	XINPUT_STATE state;
	ZeroMemory(&state, sizeof(XINPUT_STATE));

	XInputGetState(_joystickId, &state);
	return (state.Gamepad.wButtons & _button);
}

sfVector2f Gamepad_getStickPos(unsigned int _joystickId, stick _stick, sfRenderWindow* _window)
{
	if (_window != NULL && fullscreen_IsDone)
	{
		if (sfRenderWindow_hasFocus(_window))
		{
			XINPUT_STATE state;
			ZeroMemory(&state, sizeof(XINPUT_STATE));
			float x, y;
			switch (_stick)
			{
			case STICKL:
				XInputGetState(_joystickId, &state);
				if ((state.Gamepad.sThumbLX < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
					state.Gamepad.sThumbLX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)) {

					state.Gamepad.sThumbLX = 0;

				}
				x = (state.Gamepad.sThumbLX / 327.f);
				if ((state.Gamepad.sThumbLY < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
					state.Gamepad.sThumbLY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)) {

					state.Gamepad.sThumbLY = 0;

				}
				y = (state.Gamepad.sThumbLY / -327.f);
				return vector2f(x, y);
				break;
			case STICKR:
				XInputGetState(_joystickId, &state);
				if ((state.Gamepad.sThumbRX < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
					state.Gamepad.sThumbRX > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)) {

					state.Gamepad.sThumbRX = 0;

				}
				x = (state.Gamepad.sThumbRX / 327.f);
				if ((state.Gamepad.sThumbRY < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
					state.Gamepad.sThumbRY > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)) {

					state.Gamepad.sThumbRY = 0;

				}
				y = (state.Gamepad.sThumbRY / -327.f);
				return vector2f(x, y);
				break;
			case DPAD:
				return Gamepad[_joystickId].cross;
				break;

			default:
				return vector2f(0.f, 0.f);
				break;
			}
		}
	}

	return NULLVECTF;
}

sfVector2f Gamepad_getStickPosNF(unsigned int _joystickId, stick _stick)
{
	XINPUT_STATE state;
	ZeroMemory(&state, sizeof(XINPUT_STATE));
	float x, y;
	switch (_stick)
	{
	case STICKL:
		XInputGetState(_joystickId, &state);
		if ((state.Gamepad.sThumbLX < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
			state.Gamepad.sThumbLX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)) {

			state.Gamepad.sThumbLX = 0;

		}
		x = (state.Gamepad.sThumbLX / 327.f);
		if ((state.Gamepad.sThumbLY < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
			state.Gamepad.sThumbLY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)) {

			state.Gamepad.sThumbLY = 0;

		}
		y = (state.Gamepad.sThumbLY / -327.f);
		return vector2f(x, y);
		break;
	case STICKR:
		XInputGetState(_joystickId, &state);
		if ((state.Gamepad.sThumbRX < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
			state.Gamepad.sThumbRX > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)) {

			state.Gamepad.sThumbRX = 0;

		}
		x = (state.Gamepad.sThumbRX / 327.f);
		if ((state.Gamepad.sThumbRY < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
			state.Gamepad.sThumbRY > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)) {

			state.Gamepad.sThumbRY = 0;

		}
		y = (state.Gamepad.sThumbRY / -327.f);
		return vector2f(x, y);
		break;
	case DPAD:
		return Gamepad[_joystickId].cross;
		break;

	default:
		return vector2f(0.f, 0.f);
		break;
	}
}

sfVector2f Gamepad_getNormalizedStickPos(unsigned int _joystickId, stick _stick, sfRenderWindow* _window)
{
	return normalizef(Gamepad_getStickPos(_joystickId, _stick, _window));
}

sfVector2f Gamepad_getNormalizedStickPosNF(unsigned int _joystickId, stick _stick)
{
	return normalizef(Gamepad_getStickPosNF(_joystickId, _stick));
}

void setControlerVibration(int controlerID, float leftMotor, float rightMotor)
{
	XINPUT_VIBRATION vib;
	ZeroMemory(&vib, sizeof(XINPUT_VIBRATION));

	vib.wLeftMotorSpeed = (unsigned short)(leftMotor * 65535.0f);
	vib.wRightMotorSpeed = (unsigned short)(rightMotor * 65535.0f);

	XInputSetState(controlerID, &vib);
}

float Gamepad_getTriggerPos(unsigned int _joystickId, sfBool _leftTrigger, sfRenderWindow* _window)
{
	if (fullscreen_IsDone) {
		if (_window && sfRenderWindow_hasFocus(_window))
		{
			XINPUT_STATE state;
			ZeroMemory(&state, sizeof(XINPUT_STATE));

			XInputGetState(_joystickId, &state);

			if (_leftTrigger)
				return (float)(state.Gamepad.bLeftTrigger) / 255.f;
			else
				return (float)(state.Gamepad.bRightTrigger) / 225.f;
		}
	}
	return 0.f;
}
