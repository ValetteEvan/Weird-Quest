#include "fish.h"
#include "hook.h"
#include "math.h"
#include "game.h"
#include "fontManager.h"
#include "langManager.h"
#include "controller.h"
#include "stateManager.h"
#include "fadeManager.h"
#include "soundManager.h"
#include "textureManager.h"
#include "game.h"
#include "hook.h"
#include "fishing_spot.h"
#include "pause.h"
#include "particlesManager.h"

#define FISH_X_SIZE 86
#define FISH_Y_SIZE 57

#define FISH_X_ORIGIN FISH_X_SIZE / 2
#define FISH_Y_ORIGIN FISH_Y_SIZE / 2

typedef enum {
	FISH_IDLE,
	FISH_FOLLOWING,
	FISH_FLEEING,
	FISH_UPSET
}FishAnim;

typedef enum {
	FISH_NO_EFFECT,
	FISH_ATTRACTED,
	FISH_REPULSED,
	FISH_STASED
}FishEffect;

typedef struct {
	FishType type;
	sfVector2f pos;
	sfVector2f forward;
	sfVector2f targetForward;
	sfVector2f startPos;
	sfVector2f endPos;
	sfVector2f origin;
	sfVector2f size;
	float timer;
	float speed;
	float otherSpeed;
	float maxSpeed;
	float oldSpeed;
	float maxDistance;
	float theta;
	float rad;
	float otherRad;
	float otherTheta;
	float rotationAngle;
	sfBool wasFollowing;
	float steeringIntensity;
	sfVector2f velocity;
	FishEffect effect;
	FishAnim anim;
	float effectTimer;
	sfVector2f startEffectPos;
	sfVector2f endEffectPos;
	float fishTimerSpeed;
	sfBool isAttracted;
	float fishTimerJump;
	float fishOtherTimerJump;
	sfVector2f oldForward;
	sfBool IsFishGameFinish;
	sfBool firstAnim;
	sfBool secondAnim;
	sfBool isFound;
	sfBool isAnimFinished;
	int rockChoiced;
	float waitTimer;

	sfBool hasBeenAttracted;
}FishData;
FishData fish_fish;

typedef struct {
	sfVector2f position;
	sfBool isFishHere;
}Rocks_struct;
Rocks_struct rock[NBR_OF_ROCKS];

sfRectangleShape* fish_Rectangle;
sfRectangleShape* fish_PowerUpAttract;

sfVertexArray* fish_vArray;

sfFloatRect fish_WaterRect = { 0.f, 150.f, 1920.f, 930.f };

float fish_PowerTimer;
float fish_GameTimer;
sfFloatRect fish_fishArea;
sfFloatRect fish_AirArea;
sfBool fish_GameReadyToStart;
sfBool fish_CanUsePower;

sfRectangleShape* fish_Feedback;
sfBool fish_HaveToStart;
sfBool fish_CanCatch;
int fish_frameFeedback;
float fish_timerFeedback;

sfText* fish_GameTutoText;

sfBool fish_OnePassWin;

sfRectangleShape* fish_TutoRectangle;
sfRectangleShape* fish_OtherTutoRectangle;
sfBool fish_IsFirstView;
sfBool fish_ReadyToUsed;
sfBool fish_IsReelUsed;
sfBool fish_ReadyNearHook;
sfBool fish_IsNearHook;
sfBool fish_ReadyView;
sfBool fish_IsIView;
sfBool fish_ReadyTimer;
float fish_ReceptionTuToTimer;
sfBool fish_ReadyReception;
sfBool fish_ReadyEat;
sfBool fish_IsInReception;

sfBool fish_FirstTimeAntiBug;

float Fish_TimerFish;
float fish_ParticlesTimer;
float fish_ParticlesTimer2;
float fish_ParticlesTimer3;
float XforBubleParticles;
int Fish_FramesFish;

sfBool Fish_EmotionReception;
float Fish_EmotionReceptionTimer;

void fish_InitFish(FishType _type, Window* _window)
{
	playMusic("Music_Peche");
	stopMusicIfPlaying("Music_Main");
	fish_Rectangle = sfRectangleShape_create();
	sfRectangleShape_setPosition(fish_Rectangle, vector2f(960.f, 540.f));
	sfRectangleShape_setOrigin(fish_Rectangle, vector2f(FISH_X_ORIGIN, FISH_Y_ORIGIN));
	sfRectangleShape_setSize(fish_Rectangle, vector2f(FISH_X_SIZE, FISH_Y_SIZE));

	fish_PowerUpAttract = sfRectangleShape_create();
	sfRectangleShape_setSize(fish_PowerUpAttract, vector2f(72.f, 72.f));
	sfRectangleShape_setPosition(fish_PowerUpAttract, vector2f(30.f, 960.f));
	sfRectangleShape_setTexture(fish_PowerUpAttract, GetSfTexture("fishing_SpriteSheet"), sfTrue);

	float Fish_TimerFish = 0.f;
	int Fish_FramesFish = 0;

	fish_fishArea = FlRect(0.f, 600.f, 1920.f, 580.f);
	fish_AirArea = FlRect(-400.f, -400.f, 2620.f, 550.f);

	for (int i = 0; i < NBR_OF_ROCKS; i++)
	{
		rock[i].position = vector2f(150.0f + 200.0f * i, 1033.0f);
		rock[i].isFishHere = sfFalse;
	}

	fish_PowerTimer = 3.f;
	fish_fish.IsFishGameFinish = sfFalse;
	fish_GameTimer = 0.f;
	fish_ParticlesTimer = 0.f;
	fish_ParticlesTimer2 = 0.f;
	fish_GameReadyToStart = sfFalse;

	fish_TutoRectangle = sfRectangleShape_create();

	fish_OtherTutoRectangle = sfRectangleShape_create();
	sfRectangleShape_setTexture(fish_OtherTutoRectangle, GetSfTexture("Tuto"), sfTrue);
	sfRectangleShape_setSize(fish_OtherTutoRectangle, vector2f(500.f, 220.f));
	sfRectangleShape_setPosition(fish_OtherTutoRectangle, vector2f(0.f, 0.f));

	fish_GameTutoText = sfText_create();
	sfText_setFont(fish_GameTutoText, GetFont("Dico"));
	sfText_setCharacterSize(fish_GameTutoText, 30);
	sfText_setFillColor(fish_GameTutoText, sfColor_fromRGBA(204, 251, 241, 220));
	sfText_setString(fish_GameTutoText, lang_GetFromKey("fish.first.view"));

	fish_Feedback = sfRectangleShape_create();
	sfRectangleShape_setTexture(fish_Feedback, GetSfTexture("fishing_SpriteSheet"), sfTrue);

	fish_fish.fishTimerJump = 0.f;
	fish_IsFirstView = sfTrue;
	fish_IsReelUsed = sfTrue;
	fish_IsNearHook = sfTrue;
	fish_IsInReception = sfTrue;
	fish_ReadyTimer = sfFalse;
	fish_OnePassWin = sfTrue;
	fish_HaveToStart = sfFalse;
	fish_CanCatch = sfFalse;
	fish_FirstTimeAntiBug = sfFalse;

	fish_ReadyReception = sfFalse;
	fish_ReadyView = sfFalse;
	fish_ReadyNearHook = sfFalse;

	Hook_ChangeReceptionEmotion(HOOK_RECEPTION_IDLE);

	switch (_type)
	{
	case FISH_JOY:
		fish_SetupFish(FISH_JOY);
		sfRectangleShape_setTexture(fish_Rectangle, GetSfTexture("fishing_SpriteSheet"), sfTrue);
		sfRectangleShape_setScale(fish_Rectangle, vector2f(-1, 1));
		AttractToggle(sfFalse);
		hook_InitHook(_window);
		break;
	case FISH_JOY_TUTO:
		fish_SetupFish(FISH_JOY_TUTO);
		sfRectangleShape_setTexture(fish_Rectangle, GetSfTexture("fishing_SpriteSheet"), sfTrue);
		sfRectangleShape_setScale(fish_Rectangle, vector2f(-1, 1));
		AttractToggle(sfFalse);
		fish_IsFirstView = sfFalse;
		fish_IsReelUsed = sfFalse;
		fish_IsNearHook = sfFalse;
		fish_IsInReception = sfFalse;
		hook_InitHookTuto(_window);
		break;
	case FISH_SADNESS:
		fish_SetupFish(FISH_SADNESS);
		sfRectangleShape_setTexture(fish_Rectangle, GetSfTexture("fishing_SpriteSheet"), sfTrue);
		sfRectangleShape_setScale(fish_Rectangle, vector2f(-1, 1));
		AttractToggle(sfTrue);
		hook_InitHook(_window);
		break;
	case FISH_SADNESS_TUTO:
		fish_SetupFish(FISH_SADNESS_TUTO);
		sfRectangleShape_setTexture(fish_Rectangle, GetSfTexture("fishing_SpriteSheet"), sfTrue);
		sfRectangleShape_setScale(fish_Rectangle, vector2f(-1, 1));
		AttractToggle(sfTrue);
		fish_IsFirstView = sfFalse;
		fish_IsNearHook = sfFalse;
		hook_InitHookTuto(_window);
		break;
	default:
		fish_SetupFish(FISH_JOY);
		sfRectangleShape_setTexture(fish_Rectangle, GetSfTexture("fishing_SpriteSheet"), sfTrue);
		sfRectangleShape_setScale(fish_Rectangle, vector2f(-1, 1));
		AttractToggle(sfFalse);
		hook_InitHook(_window);
		break;
	}
	fish_fish.anim = FISH_IDLE;
	fish_fish.timer = 3.f;
	XforBubleParticles = rand_float(50.f, 1850.f);
}

void fish_UpdateFish(Window* _window)
{
	fish_FirstTimeAntiBug = fish_GiveFirstTimeDrop();
	fish_ParticlesTimer += getDeltaTime();
	fish_ParticlesTimer2 += getDeltaTime();
	fish_ParticlesTimer3 += getDeltaTime();

	if (fish_ParticlesTimer3 > 10.8f)
	{
		XforBubleParticles = rand_float(50.f, 1850.f);
		fish_ParticlesTimer3 = 0.f;
	}
	if (fish_ParticlesTimer > 0.8f)
	{
		prt_CreateBubbleParticlesInFront(vector2f(XforBubleParticles, 1050.f));
		fish_ParticlesTimer = 0.f;
	}
	if (fish_ParticlesTimer2 > 1.3f)
	{
		prt_CreateBubbleFishParticles(fish_fish.pos);
		fish_ParticlesTimer2 = 0.f;
	}
	if (fade_GetOpacity() == 255)
	{
		fish_GameReadyToStart = sfTrue;
	}
	if (fish_GameReadyToStart)
	{
		fadeIn(1.5f);
	}
	if (fade_GetOpacity() == 0)
	{
		fish_GameReadyToStart = sfFalse;
	}
	if (!fish_GameReadyToStart)
	{

		float dt = getDeltaTime();
		sfVector2f hookPos = hook_GetHookPos();

		fish_PowerTimer += dt;
		float previousPowerTimer = fish_PowerTimer;
		if (fish_fish.IsFishGameFinish)
		{
			game_ChangeFromFishToGame();
		}
		else
		{
			if (fish_IsFirstView)
			{
				if (fish_CanUsePower)
				{
					if (fish_PowerTimer > FISH_POWER_EFFECT_COOLDOWN && fish_fish.effect == FISH_NO_EFFECT)
					{
						if (Gamepad_isButtonPressed(0, A, _window->renderWindow)) {
							fish_PowerTimer = 0.f;
							hook_ResetPowerRing();
							if (fish_IsInRadius(hookPos)) {
								fish_fish.effect = FISH_ATTRACTED;
								fish_fish.startEffectPos = fish_fish.pos;
							}
						}
					}
				}
				else 
					fish_CanUsePower = sfTrue;
				if (fish_PowerTimer < 0.6f)
				{
					hook_SetPowerRing();
				}

				if (fish_fish.effect != FISH_NO_EFFECT) {
					fish_fish.effectTimer += dt;
					if (fish_fish.effectTimer > FISH_EFFECT_DURATION) {
						fish_fish.effectTimer = 0.f;
						fish_fish.effect = FISH_NO_EFFECT;
					}
				}
			}
			else
			{
				if (Gamepad_isButtonPressed(0, A, _window->renderWindow)) {
					fish_IsFirstView = sfTrue;
				}
			}
			if (fish_fish.type == FISH_JOY)
			{
				fish_CanCatch = sfFalse;
				if (hook_CanBeCaptured(fish_fish.pos)) {
					fish_CanCatch = sfTrue;
					if (Gamepad_isButtonPressed(0, RB, _window->renderWindow) && Gamepad_isButtonPressed(0, LB, _window->renderWindow) && fish_OnePassWin) {
						fish_fish.IsFishGameFinish = sfTrue;
						fish_OnePassWin = sfFalse;
						playSoundFX("SFX_Fishing_Win");
						Hook_ChangeReceptionEmotion(HOOK_RECEPTION_HAPPY);
						fishspot_Delete(fish_IdFishingSpot);
						pause_itemRecup(PAUSE_JOY_FISH);
					}
				}
				if (!hook_CanBeCaptured(fish_fish.pos) && Gamepad_isButtonPressed(0, RB, _window->renderWindow) && Gamepad_isButtonPressed(0, LB, _window->renderWindow)) 
					Hook_ChangeReceptionEmotion(HOOK_RECEPTION_SAD);

				if (fish_isOutOfTheWater(sfRectangleShape_getGlobalBounds(fish_Rectangle)))
				{
					if (fish_fish.fishTimerJump == 0.f)
					{
						fish_fish.fishTimerJump += dt;
						fish_fish.oldSpeed = fish_fish.speed;
						fish_fish.oldForward = fish_fish.forward;
						hook_addSplash(fish_fish.pos);
					}
					fish_fish.forward = fish_fish.oldForward;
				}
				else if (fish_fish.fishTimerJump != 0.f){
					fish_fish.fishTimerJump = 0.f;
					hook_addSplash(fish_fish.pos);
				}
				if (fish_fish.fishTimerJump > 0.f) {
					fish_fish.fishTimerJump += (dt / 4);
					if (fish_fish.fishTimerJump < 0.5f)
						fish_fish.fishOtherTimerJump += (dt / 4);
					fish_fish.forward.y = lerp(fish_fish.forward.y, 1.f, fish_fish.fishTimerJump);
					if (fish_fish.fishTimerJump < 0.5f)
						fish_fish.speed = lerp(fish_fish.oldSpeed, (fish_fish.oldSpeed - 100.f), (fish_fish.fishTimerJump * 2.f - 0.5f));
					if (fish_fish.fishTimerJump > 0.5f)
						fish_fish.speed = lerp(fish_fish.oldSpeed, (fish_fish.oldSpeed + 50.f), (fish_fish.fishOtherTimerJump * 2.f));
					addVectorsf(&fish_fish.pos, multiplyVectorsrf(fish_fish.forward, dt * fish_fish.speed));
					fish_fish.rotationAngle = getAngleBetweenVectors(vector2f(0.f, 0.f), fish_fish.forward) * RAD2DEG;
					fish_fish.endPos = fish_GetDefaultPos();
				}
				if (fish_fish.fishTimerJump > 1.f) {
					fish_fish.fishTimerJump = 1.f;
				}
				if (fish_fish.fishTimerJump == 0.f)
				{
					fish_fish.anim = FISH_IDLE;

					if (!hook_IsReseting() && (isInFOV(hookPos, fish_fish.pos, normalizef(fish_fish.forward), fish_fish.theta, fish_fish.rad) || isInFOV(hookPos, fish_fish.pos, normalizef(fish_fish.forward), fish_fish.otherTheta, fish_fish.otherRad))) {
						fish_fish.anim = FISH_FOLLOWING;
						fish_fish.speed = fish_fish.maxSpeed;
						fish_fish.forward = vec2f_lerp(fish_fish.forward, fish_fish.targetForward, 10.0f * dt);
						fish_fish.wasFollowing = sfTrue;
						fish_fish.targetForward = normalizef(createVectorWithVectorf(fish_fish.pos, hookPos));
						fish_fish.forward = vec2f_lerp(fish_fish.forward, fish_fish.targetForward, 0.8f * dt);

						addVectorsf(&fish_fish.pos, multiplyVectorsrf(fish_fish.forward, dt * fish_fish.speed));
						fish_fish.rotationAngle = getAngleBetweenVectors(vector2f(0.f, 0.f), fish_fish.forward) * RAD2DEG;

					}
					else
					{
						fish_fish.speed = fish_fish.otherSpeed;
						fish_fish.timer += dt;
						if (fish_fish.wasFollowing && fish_fish.timer > 0.5f) {
							fish_fish.speed = fish_fish.maxSpeed - 50.f;
							fish_fish.wasFollowing = sfFalse;
							fish_fish.timer = 0.f;
							fish_fish.startPos = fish_fish.pos;
							fish_fish.endPos = fish_GetDefaultPos();
							for (int i = 0; i < 100; i++)
							{
								sfVector2f tmpEndPos = polarCoord(fish_fish.startPos, fish_fish.maxDistance, rand_float(0.f, PI * 2.f));
								if (sfFloatRect_contains(&fish_fishArea, tmpEndPos.x, tmpEndPos.y)) {
									fish_fish.endPos = tmpEndPos;
									break;
								}
							}
							fish_fish.rotationAngle = getAngleBetweenVectors(vector2f(0.f, 0.f), createVectorWithVectorf(fish_fish.startPos, fish_fish.endPos)) * RAD2DEG;
						}
						if (fish_fish.timer < 2.f)
							fish_fish.anim = FISH_UPSET;
						if (equalsWithOffset(fish_fish.pos, fish_fish.endPos, 50.f)) {
							fish_fish.speed = fish_fish.otherSpeed;
							fish_fish.startPos = fish_fish.pos;
							fish_fish.endPos = fish_GetDefaultPos();
							for (int i = 0; i < 100; i++)
							{
								sfVector2f tmpEndPos = polarCoord(fish_fish.startPos, fish_fish.maxDistance, rand_float(0.f, PI * 2.f));
								if (sfFloatRect_contains(&fish_fishArea, tmpEndPos.x, tmpEndPos.y)) {
									fish_fish.endPos = tmpEndPos;
									break;
								}
							}
							fish_fish.rotationAngle = getAngleBetweenVectors(vector2f(0.f, 0.f), createVectorWithVectorf(fish_fish.startPos, fish_fish.endPos)) * RAD2DEG;
						}

						fish_fish.targetForward = normalizef(createVectorWithVectorf(fish_fish.pos, fish_fish.endPos));
						fish_fish.forward = vec2f_lerp(fish_fish.forward, fish_fish.targetForward, 0.8f * dt);

						addVectorsf(&fish_fish.pos, multiplyVectorsrf(fish_fish.forward, dt * fish_fish.speed));
						fish_fish.rotationAngle = getAngleBetweenVectors(vector2f(0.f, 0.f), fish_fish.forward) * RAD2DEG;
					}
					fish_fish.oldForward = fish_fish.forward;
				}
			}
			else if (fish_fish.type == FISH_JOY_TUTO)
			{
				if (fish_isOutOfTheWater(sfRectangleShape_getGlobalBounds(fish_Rectangle)))
				{
					if (fish_fish.fishTimerJump == 0.f)
					{
						fish_fish.fishTimerJump += dt;
						fish_fish.oldSpeed = fish_fish.speed;
						fish_fish.oldForward = fish_fish.forward;
						hook_addSplash(fish_fish.pos);
					}
					fish_fish.forward = fish_fish.oldForward;
				}
				else if (fish_fish.fishTimerJump != 0.f) {
					fish_fish.fishTimerJump = 0.f;
					hook_addSplash(fish_fish.pos);
				}

				if ((isInFOV(hookPos, fish_fish.pos, normalizef(fish_fish.forward), fish_fish.theta, fish_fish.rad) || isInFOV(hookPos, fish_fish.pos, normalizef(fish_fish.forward), fish_fish.otherTheta, fish_fish.otherRad)) && !fish_ReadyNearHook && fish_FirstTimeAntiBug)
				{
					fish_ReadyNearHook = sfTrue;
					hook_ToggleMovement();
					fish_IsNearHook = sfTrue;
				}
				if (fish_ReadyTimer && !fish_IsInReception && !fish_ReadyReception)
				{
					fish_ReceptionTuToTimer += dt;
					if (fish_ReceptionTuToTimer > 1.f)
					{
						fish_ReadyReception = sfTrue;
						hook_ToggleMovement();
					}
				}
				if (fish_IsNearHook)
				{
					if (Gamepad_isButtonPressed(0, A, _window->renderWindow))
					{
						fish_IsNearHook = sfFalse;
						fish_ReadyEat = sfTrue;
					}
				}
				else if (fish_ReadyReception)
				{
					if (Gamepad_isButtonPressed(0, A, _window->renderWindow))
					{
						fish_IsInReception = sfTrue;
						fish_ReadyReception = sfFalse;
						hook_ToggleMovement();
						game_ChangeFromFishToGame();
					}
				}
				else if (fish_ReadyEat)
				{
					fish_ReceptionTuToTimer += dt;
					if (Gamepad_isButtonPressed(0, A, _window->renderWindow) && fish_ReceptionTuToTimer > 0.5f)
					{
						fish_ReadyTimer = sfTrue;
						fish_ReadyEat = sfFalse;
						fish_ReceptionTuToTimer = 0.f;
						hook_ToggleMovement();
					}
				}
				else
				{
					if (!fish_ReadyToUsed)
					{
						if (fish_fish.fishTimerJump > 0.f) {
							fish_fish.fishTimerJump += (dt / 4);
							if (fish_fish.fishTimerJump < 0.5f)
								fish_fish.fishOtherTimerJump += (dt / 4);
							fish_fish.forward.y = lerp(fish_fish.forward.y, 1.f, fish_fish.fishTimerJump);
							if (fish_fish.fishTimerJump < 0.5f)
								fish_fish.speed = lerp(fish_fish.oldSpeed, (fish_fish.oldSpeed - 100.f), (fish_fish.fishTimerJump * 2.f - 0.5f));
							if (fish_fish.fishTimerJump > 0.5f)
								fish_fish.speed = lerp(fish_fish.oldSpeed, (fish_fish.oldSpeed + 50.f), (fish_fish.fishOtherTimerJump * 2.f));
							addVectorsf(&fish_fish.pos, multiplyVectorsrf(fish_fish.forward, dt * fish_fish.speed));
							fish_fish.rotationAngle = getAngleBetweenVectors(vector2f(0.f, 0.f), fish_fish.forward) * RAD2DEG;
							fish_fish.endPos = fish_GetDefaultPos();
						}
						if (fish_fish.fishTimerJump > 1.f) {
							fish_fish.fishTimerJump = 1.f;
						}
						if (fish_fish.fishTimerJump == 0.f)
						{
							fish_fish.anim = FISH_IDLE;

							if (!hook_IsReseting() && (isInFOV(hookPos, fish_fish.pos, normalizef(fish_fish.forward), fish_fish.theta, fish_fish.rad) || isInFOV(hookPos, fish_fish.pos, normalizef(fish_fish.forward), fish_fish.otherTheta, fish_fish.otherRad))) {
								fish_fish.speed = 150.f;
								fish_fish.anim = FISH_FOLLOWING;
								fish_fish.forward = vec2f_lerp(fish_fish.forward, fish_fish.targetForward, 10.0f * dt);
								fish_fish.wasFollowing = sfTrue;
								fish_fish.targetForward = normalizef(createVectorWithVectorf(fish_fish.pos, hookPos));
								fish_fish.forward = vec2f_lerp(fish_fish.forward, fish_fish.targetForward, 0.8f * dt);

								addVectorsf(&fish_fish.pos, multiplyVectorsrf(fish_fish.forward, dt * fish_fish.speed));
								fish_fish.rotationAngle = getAngleBetweenVectors(vector2f(0.f, 0.f), fish_fish.forward) * RAD2DEG;

							}
							else
							{
								fish_fish.speed = fish_fish.otherSpeed;
								fish_fish.timer += dt;
								if (fish_fish.wasFollowing && fish_fish.timer > 0.5f) {
									fish_fish.speed = 100.f;
									fish_fish.wasFollowing = sfFalse;
									fish_fish.startPos = fish_fish.pos;
									fish_fish.endPos = fish_GetDefaultPos();
									for (int i = 0; i < 100; i++)
									{
										sfVector2f tmpEndPos = polarCoord(fish_fish.startPos, fish_fish.maxDistance, rand_float(0.f, PI * 2.f));
										if (sfFloatRect_contains(&fish_fishArea, tmpEndPos.x, tmpEndPos.y)) {
											fish_fish.endPos = tmpEndPos;
											break;
										}
									}
									fish_fish.rotationAngle = getAngleBetweenVectors(vector2f(0.f, 0.f), createVectorWithVectorf(fish_fish.startPos, fish_fish.endPos)) * RAD2DEG;
								}
								if (fish_fish.timer < 2.f)
									fish_fish.anim = FISH_UPSET;
								if (equalsWithOffset(fish_fish.pos, fish_fish.endPos, 50.f)) {
									fish_fish.speed = 100.f;
									fish_fish.startPos = fish_fish.pos;
									fish_fish.endPos = fish_GetDefaultPos();
									for (int i = 0; i < 100; i++)
									{
										sfVector2f tmpEndPos = polarCoord(fish_fish.startPos, fish_fish.maxDistance, rand_float(0.f, PI * 2.f));
										if (sfFloatRect_contains(&fish_fishArea, tmpEndPos.x, tmpEndPos.y)) {
											fish_fish.endPos = tmpEndPos;
											break;
										}
									}
									fish_fish.rotationAngle = getAngleBetweenVectors(vector2f(0.f, 0.f), createVectorWithVectorf(fish_fish.startPos, fish_fish.endPos)) * RAD2DEG;
								}

								fish_fish.targetForward = normalizef(createVectorWithVectorf(fish_fish.pos, fish_fish.endPos));
								fish_fish.forward = vec2f_lerp(fish_fish.forward, fish_fish.targetForward, 0.8f * dt);

								addVectorsf(&fish_fish.pos, multiplyVectorsrf(fish_fish.forward, dt * fish_fish.speed));
								fish_fish.rotationAngle = getAngleBetweenVectors(vector2f(0.f, 0.f), fish_fish.forward) * RAD2DEG;
							}
							fish_fish.oldForward = fish_fish.forward;
						}
					}
				}
			}
			else if (fish_fish.type == FISH_SADNESS)
			{
				if (hook_CanBeCaptured(fish_fish.pos) && Gamepad_isButtonPressed(0, RB, _window->renderWindow) && Gamepad_isButtonPressed(0, LB, _window->renderWindow) && !hook_IsReseting() && fish_fish.isAttracted) {
					fish_fish.IsFishGameFinish = sfTrue;
				}
				if (sfFloatRect_contains(&fish_AirArea, fish_fish.pos.x, fish_fish.pos.y)) {
					if (fish_fish.fishTimerJump == 0.f)
					{
						fish_fish.fishTimerJump += dt;
						fish_fish.oldForward = fish_fish.forward;
						fish_fish.oldSpeed = fish_fish.speed;
					}
					fish_fish.forward = fish_fish.oldForward;
				}
				else {
					fish_fish.fishTimerJump = 0.f;
				}
				if (fish_fish.fishTimerJump > 0.f) {
					fish_fish.fishTimerJump += (dt / 4);
					if (fish_fish.fishTimerJump < 0.5f)
						fish_fish.fishOtherTimerJump += (dt / 4);
					fish_fish.forward.y = lerp(fish_fish.forward.y, 1.f, fish_fish.fishTimerJump);
					if (fish_fish.fishTimerJump < 0.5f)
						fish_fish.speed = lerp(fish_fish.oldSpeed, (fish_fish.oldSpeed - 100.f), (fish_fish.fishTimerJump * 2.f));
					if (fish_fish.fishTimerJump > 0.5f)
						fish_fish.speed = lerp(fish_fish.oldSpeed, (fish_fish.oldSpeed + 50.f), (fish_fish.fishOtherTimerJump * 2.f));
					addVectorsf(&fish_fish.pos, multiplyVectorsrf(fish_fish.forward, dt * fish_fish.speed));
					fish_fish.rotationAngle = getAngleBetweenVectors(vector2f(0.f, 0.f), fish_fish.forward) * RAD2DEG;
					fish_fish.endPos = fish_GetDefaultPos();
				}
				if (fish_fish.fishTimerJump > 1.f) {
					fish_fish.fishTimerJump = 1.f;
				}
				if (fish_fish.fishTimerJump == 0.f)
				{
					if (!hook_IsReseting() && (isInFOV(hookPos, fish_fish.pos, normalizef(fish_fish.forward), fish_fish.theta, fish_fish.rad) || isInFOV(hookPos, fish_fish.pos, normalizef(fish_fish.forward), fish_fish.otherTheta, fish_fish.otherRad))) {
						if (fish_fish.hasBeenAttracted)
						{
							fish_fish.fishTimerSpeed = 0.f;
							fish_fish.speed = 450.f;
							fish_fish.wasFollowing = sfTrue;
							fish_fish.timer = 0.f;

							fish_fish.targetForward = normalizef(createVectorWithVectorf(fish_fish.pos, hookPos));
							fish_fish.forward = vec2f_lerp(fish_fish.forward, fish_fish.targetForward, 5.f * dt);
							addVectorsf(&fish_fish.pos, multiplyVectorsrf(fish_fish.forward, dt * fish_fish.speed));

						}
						else {
							fish_fish.fishTimerSpeed = 0.f;
							fish_fish.speed = 1000.f;
							fish_fish.wasFollowing = sfTrue;
							fish_fish.timer = 0.f;

							fish_fish.targetForward = normalizef(createVectorWithVectorf(hookPos, fish_fish.pos));
							fish_fish.forward = vec2f_lerp(fish_fish.forward, fish_fish.targetForward, 5.f * dt);
							addVectorsf(&fish_fish.pos, multiplyVectorsrf(fish_fish.forward, dt * fish_fish.speed));

						}

						fish_fish.rotationAngle = getAngleBetweenVectors(vector2f(0.f, 0.f), fish_fish.forward) * RAD2DEG;
					}
					else
					{
						if (fish_fish.hasBeenAttracted)
							fish_fish.speed = 450.f;
						else {
							fish_fish.fishTimerSpeed += dt / 4;
							if (fish_fish.fishTimerSpeed > 1.f)
								fish_fish.fishTimerSpeed = 1.f;
							fish_fish.speed = lerp(1000.f, 150.f, fish_fish.fishTimerSpeed);
						}
						fish_fish.hasBeenAttracted = sfFalse;
						fish_fish.timer += dt;
						if (fish_fish.wasFollowing) {
							fish_fish.wasFollowing = sfFalse;
							fish_fish.startPos = fish_fish.pos;
							fish_fish.endPos = fish_GetDefaultPos();
							for (int i = 0; i < 100; i++)
							{
								sfVector2f tmpEndPos = polarCoord(fish_fish.startPos, fish_fish.maxDistance, rand_float(0.f, PI * 2.f));
								if (sfFloatRect_contains(&fish_fishArea, tmpEndPos.x, tmpEndPos.y)) {
									fish_fish.endPos = tmpEndPos;
									break;
								}
							}
							fish_fish.rotationAngle = getAngleBetweenVectors(vector2f(0.f, 0.f), createVectorWithVectorf(fish_fish.startPos, fish_fish.endPos)) * RAD2DEG;
						}
						if (equalsWithOffset(fish_fish.pos, fish_fish.endPos, 100.f) || !sfFloatRect_contains(&fish_WaterRect, fish_fish.endPos.x, fish_fish.endPos.y)) {
							fish_fish.timer = 0.f;
							fish_fish.startPos = fish_fish.pos;
							fish_fish.endPos = fish_GetDefaultPos();
							for (int i = 0; i < 100; i++)
							{
								sfVector2f tmpEndPos = polarCoord(fish_fish.startPos, fish_fish.maxDistance, rand_float(0.f, PI * 2.f));
								if (sfFloatRect_contains(&fish_fishArea, tmpEndPos.x, tmpEndPos.y)) {
									fish_fish.endPos = tmpEndPos;
									break;
								}
							}
							fish_fish.rotationAngle = getAngleBetweenVectors(vector2f(0.f, 0.f), createVectorWithVectorf(fish_fish.startPos, fish_fish.endPos)) * RAD2DEG;
						}

						if (fish_fish.effect == FISH_ATTRACTED) {
							fish_fish.hasBeenAttracted = sfTrue;
							fish_fish.isAttracted = sfTrue;
							fish_fish.endPos = hook_GetHookPos();
						}
						else if (fish_fish.isAttracted)
						{
							fish_fish.startPos = fish_fish.pos;
							fish_fish.isAttracted = sfFalse;
							fish_fish.endPos = fish_GetDefaultPos();
							for (int i = 0; i < 100; i++)
							{
								sfVector2f tmpEndPos = polarCoord(fish_fish.startPos, fish_fish.maxDistance, rand_float(0.f, PI * 2.f));
								if (sfFloatRect_contains(&fish_fishArea, tmpEndPos.x, tmpEndPos.y)) {
									fish_fish.endPos = tmpEndPos;
									break;
								}
							}
							fish_fish.rotationAngle = getAngleBetweenVectors(vector2f(0.f, 0.f), createVectorWithVectorf(fish_fish.startPos, fish_fish.endPos)) * RAD2DEG;
						}

						fish_fish.targetForward = normalizef(createVectorWithVectorf(fish_fish.pos, fish_fish.endPos));
						fish_fish.forward = vec2f_lerp(fish_fish.forward, fish_fish.targetForward, 0.8f * dt);


						addVectorsf(&fish_fish.pos, multiplyVectorsrf(fish_fish.forward, dt * fish_fish.speed));
						fish_fish.rotationAngle = getAngleBetweenVectors(vector2f(0.f, 0.f), fish_fish.forward) * RAD2DEG;

					}
				}
			}
			else if (fish_fish.type == FISH_SADNESS_TUTO)
			{
				if (fish_IsFirstView)
				{
					if ((isInFOV(hookPos, fish_fish.pos, normalizef(fish_fish.forward), fish_fish.theta, fish_fish.rad) || isInFOV(hookPos, fish_fish.pos, normalizef(fish_fish.forward), fish_fish.otherTheta, fish_fish.otherRad)) && !fish_ReadyNearHook)
					{
						fish_ReadyNearHook = sfTrue;
						fish_IsNearHook = sfTrue;
						fish_changeReadyToUsed();
					}
					if (fish_IsNearHook)
					{
						if (Gamepad_isButtonPressed(0, A, _window->renderWindow))
						{
							fish_IsNearHook = sfFalse;
							fish_changeReadyToUsed();
						}
					}
					if (!fish_ReadyToUsed)
					{
						if (hook_CanBeCaptured(fish_fish.pos) && Gamepad_isButtonPressed(0, RB, _window->renderWindow) && Gamepad_isButtonPressed(0, LB, _window->renderWindow) && !hook_IsReseting() && fish_fish.isAttracted) {
							fish_fish.IsFishGameFinish = sfTrue;
						}
						if (sfFloatRect_contains(&fish_AirArea, fish_fish.pos.x, fish_fish.pos.y)) {
							if (fish_fish.fishTimerJump == 0.f)
							{
								fish_fish.fishTimerJump += dt;
								fish_fish.oldForward = fish_fish.forward;
								fish_fish.oldSpeed = fish_fish.speed;
							}
							fish_fish.forward = fish_fish.oldForward;
						}
						else {
							fish_fish.fishTimerJump = 0.f;
						}
						if (fish_fish.fishTimerJump > 0.f) {
							fish_fish.fishTimerJump += (dt / 4);
							if (fish_fish.fishTimerJump < 0.5f)
								fish_fish.fishOtherTimerJump += (dt / 4);
							fish_fish.forward.y = lerp(fish_fish.forward.y, 1.f, fish_fish.fishTimerJump);
							if (fish_fish.fishTimerJump < 0.5f)
								fish_fish.speed = lerp(fish_fish.oldSpeed, (fish_fish.oldSpeed - 100.f), (fish_fish.fishTimerJump * 2.f));
							if (fish_fish.fishTimerJump > 0.5f)
								fish_fish.speed = lerp(fish_fish.oldSpeed, (fish_fish.oldSpeed + 50.f), (fish_fish.fishOtherTimerJump * 2.f));
							addVectorsf(&fish_fish.pos, multiplyVectorsrf(fish_fish.forward, dt * fish_fish.speed));
							fish_fish.rotationAngle = getAngleBetweenVectors(vector2f(0.f, 0.f), fish_fish.forward) * RAD2DEG;
							fish_fish.endPos = fish_GetDefaultPos();
						}
						if (fish_fish.fishTimerJump > 1.f) {
							fish_fish.fishTimerJump = 1.f;
						}
						if (fish_fish.fishTimerJump == 0.f)
						{
							if (!hook_IsReseting() && (isInFOV(hookPos, fish_fish.pos, normalizef(fish_fish.forward), fish_fish.theta, fish_fish.rad) || isInFOV(hookPos, fish_fish.pos, normalizef(fish_fish.forward), fish_fish.otherTheta, fish_fish.otherRad))) {
								if (fish_fish.hasBeenAttracted)
								{
									fish_fish.fishTimerSpeed = 0.f;
									fish_fish.speed = 450.f;
									fish_fish.wasFollowing = sfTrue;
									fish_fish.timer = 0.f;

									fish_fish.targetForward = normalizef(createVectorWithVectorf(fish_fish.pos, hookPos));
									fish_fish.forward = vec2f_lerp(fish_fish.forward, fish_fish.targetForward, 5.f * dt);
									addVectorsf(&fish_fish.pos, multiplyVectorsrf(fish_fish.forward, dt * fish_fish.speed));

								}
								else {
									fish_fish.fishTimerSpeed = 0.f;
									fish_fish.speed = 1000.f;
									fish_fish.wasFollowing = sfTrue;
									fish_fish.timer = 0.f;

									fish_fish.targetForward = normalizef(createVectorWithVectorf(hookPos, fish_fish.pos));
									fish_fish.forward = vec2f_lerp(fish_fish.forward, fish_fish.targetForward, 5.f * dt);
									addVectorsf(&fish_fish.pos, multiplyVectorsrf(fish_fish.forward, dt * fish_fish.speed));

								}

								fish_fish.rotationAngle = getAngleBetweenVectors(vector2f(0.f, 0.f), fish_fish.forward) * RAD2DEG;
							}
							else
							{
								if (fish_fish.hasBeenAttracted)
									fish_fish.speed = 450.f;
								else {
									fish_fish.fishTimerSpeed += dt / 4;
									if (fish_fish.fishTimerSpeed > 1.f)
										fish_fish.fishTimerSpeed = 1.f;
									fish_fish.speed = lerp(1000.f, 150.f, fish_fish.fishTimerSpeed);
								}
								fish_fish.hasBeenAttracted = sfFalse;
								fish_fish.timer += dt;
								if (fish_fish.wasFollowing) {
									fish_fish.wasFollowing = sfFalse;
									fish_fish.startPos = fish_fish.pos;
									fish_fish.endPos = fish_GetDefaultPos();
									for (int i = 0; i < 100; i++)
									{
										sfVector2f tmpEndPos = polarCoord(fish_fish.startPos, fish_fish.maxDistance, rand_float(0.f, PI * 2.f));
										if (sfFloatRect_contains(&fish_fishArea, tmpEndPos.x, tmpEndPos.y)) {
											fish_fish.endPos = tmpEndPos;
											break;
										}
									}
									fish_fish.rotationAngle = getAngleBetweenVectors(vector2f(0.f, 0.f), createVectorWithVectorf(fish_fish.startPos, fish_fish.endPos)) * RAD2DEG;
								}
								if (equalsWithOffset(fish_fish.pos, fish_fish.endPos, 100.f) || !sfFloatRect_contains(&fish_WaterRect, fish_fish.endPos.x, fish_fish.endPos.y)) {
									fish_fish.timer = 0.f;
									fish_fish.startPos = fish_fish.pos;
									fish_fish.endPos = fish_GetDefaultPos();
									for (int i = 0; i < 100; i++)
									{
										sfVector2f tmpEndPos = polarCoord(fish_fish.startPos, fish_fish.maxDistance, rand_float(0.f, PI * 2.f));
										if (sfFloatRect_contains(&fish_fishArea, tmpEndPos.x, tmpEndPos.y)) {
											fish_fish.endPos = tmpEndPos;
											break;
										}
									}
									fish_fish.rotationAngle = getAngleBetweenVectors(vector2f(0.f, 0.f), createVectorWithVectorf(fish_fish.startPos, fish_fish.endPos)) * RAD2DEG;
								}

								if (fish_fish.effect == FISH_ATTRACTED) {
									fish_fish.hasBeenAttracted = sfTrue;
									fish_fish.isAttracted = sfTrue;
									fish_fish.endPos = hook_GetHookPos();
								}
								else if (fish_fish.isAttracted)
								{
									fish_fish.startPos = fish_fish.pos;
									fish_fish.isAttracted = sfFalse;
									fish_fish.endPos = fish_GetDefaultPos();
									for (int i = 0; i < 100; i++)
									{
										sfVector2f tmpEndPos = polarCoord(fish_fish.startPos, fish_fish.maxDistance, rand_float(0.f, PI * 2.f));
										if (sfFloatRect_contains(&fish_fishArea, tmpEndPos.x, tmpEndPos.y)) {
											fish_fish.endPos = tmpEndPos;
											break;
										}
									}
									fish_fish.rotationAngle = getAngleBetweenVectors(vector2f(0.f, 0.f), createVectorWithVectorf(fish_fish.startPos, fish_fish.endPos)) * RAD2DEG;
								}

								fish_fish.targetForward = normalizef(createVectorWithVectorf(fish_fish.pos, fish_fish.endPos));
								fish_fish.forward = vec2f_lerp(fish_fish.forward, fish_fish.targetForward, 0.8f * dt);


								addVectorsf(&fish_fish.pos, multiplyVectorsrf(fish_fish.forward, dt * fish_fish.speed));
								fish_fish.rotationAngle = getAngleBetweenVectors(vector2f(0.f, 0.f), fish_fish.forward) * RAD2DEG;

							}
						}
						else
						{
							if (Gamepad_isButtonPressed(0, A, _window->renderWindow)) {
								fish_IsFirstView = sfTrue;
							}
						}
					}
				}
			}
			/*
			else if (fish_fish.type == FISH_SCARED)
			{
				if (hook_CanBeCaptured(fish_fish.pos)) {
					fish_SetupFish(FISH_SCARED);
				}

				fish_fish.timer += dt;
				if (!fish_fish.isAnimFinished) {
					fish_LookingForHome();
					fish_fish.forward = vector2f(0.f, 1.f);
				}
				else
				{
					if (!fish_fish.isFound)
					{
						fish_fish.forward = vector2f(0.f, 1.f);
						if (previousPowerTimer > FISH_POWER_EFFECT_COOLDOWN && sfKeyboard_isKeyPressed(sfKeyB) && fish_fish.timer > 0.2f)
						{
							fish_fish.wasFollowing = sfTrue;
							fish_fish.timer = 0.0f;
							fish_pushRock();
						}
					}
					else
					{
						if (fish_fish.waitTimer >= FISH_SCARED_BORED_DURATION)
						{
							fish_fish.forward = vector2f(0.f, 1.f);
							if (fish_fish.pos.y < 1000.0f)
								fish_fish.pos.y += 200.0f * dt;
							else
							{
								fish_fish.hasBeenAttracted = sfFalse;
								fish_fish.waitTimer = 0.0f;
								rock[fish_fish.rockChoiced].position.y = 1033.f;
								fish_fish.rockChoiced = rand() % NBR_OF_ROCKS;
								fish_fish.isFound = sfFalse;
								fish_fish.isAnimFinished = sfFalse;
								fish_fish.timer = 0.f;
							}
						}
						else if (!fish_fish.hasBeenAttracted) {
							fish_fish.waitTimer += dt;
							if (fish_fish.wasFollowing) {
								fish_fish.speed = 1500.f;
								fish_fish.wasFollowing = sfFalse;
								fish_fish.startPos = fish_fish.pos;
								fish_fish.endPos = fish_GetDefaultPos();
								for (int i = 0; i < 100; i++)
								{
									sfVector2f tmpEndPos = polarCoord(fish_fish.startPos, fish_fish.maxDistance, rand_float(0.f, PI * 2.f));
									if (sfFloatRect_contains(&fish_fishArea, tmpEndPos.x, tmpEndPos.y)) {
										fish_fish.endPos = tmpEndPos;
										break;
									}
								}
								fish_fish.rotationAngle = getAngleBetweenVectors(vector2f(0.f, 0.f), createVectorWithVectorf(fish_fish.startPos, fish_fish.endPos)) * RAD2DEG;
							}
							if (equalsWithOffset(fish_fish.pos, fish_fish.endPos, 100.f)) {
								fish_fish.timer = 0.f;
								fish_fish.speed = 1300.f;
								fish_fish.startPos = fish_fish.pos;
								fish_fish.endPos = fish_GetDefaultPos();
								for (int i = 0; i < 100; i++)
								{
									sfVector2f tmpEndPos = polarCoord(fish_fish.startPos, fish_fish.maxDistance, rand_float(0.f, PI * 2.f));
									if (sfFloatRect_contains(&fish_fishArea, tmpEndPos.x, tmpEndPos.y)) {
										fish_fish.endPos = tmpEndPos;
										break;
									}
								}
								fish_fish.rotationAngle = getAngleBetweenVectors(vector2f(0.f, 0.f), createVectorWithVectorf(fish_fish.startPos, fish_fish.endPos)) * RAD2DEG;
							}
							if (fish_fish.effect == FISH_ATTRACTED) {
								fish_fish.hasBeenAttracted = sfTrue;
								fish_fish.endPos = hook_GetHookPos();
							}
							fish_fish.targetForward = normalizef(createVectorWithVectorf(fish_fish.pos, fish_fish.endPos));
							fish_fish.forward = vec2f_lerp(fish_fish.forward, fish_fish.targetForward, 0.8f * dt);

							addVectorsf(&fish_fish.pos, multiplyVectorsrf(fish_fish.forward, dt * fish_fish.speed));
							fish_fish.rotationAngle = getAngleBetweenVectors(vector2f(0.f, 0.f), fish_fish.forward) * RAD2DEG;
						}
						else if (!hook_IsReseting() && isInFOV(hookPos, fish_fish.pos, normalizef(fish_fish.forward), fish_fish.theta, fish_fish.rad) && fish_fish.hasBeenAttracted) {
							fish_fish.speed = 100.f;
							fish_fish.wasFollowing = sfTrue;
							fish_fish.timer = 0.f;
							fish_fish.targetForward = normalizef(createVectorWithVectorf(fish_fish.pos, hookPos));
							fish_fish.forward = vec2f_lerp(fish_fish.forward, fish_fish.targetForward, 5.f * dt);
							sfVector2f futurefishPos = vector2f(fish_fish.pos.x + fish_fish.forward.x * dt * fish_fish.speed, fish_fish.pos.y + fish_fish.forward.y * dt * fish_fish.speed);
							sfVector2f otherForward = fish_fish.forward;
							otherForward.x = otherForward.x;
							otherForward.y = otherForward.y;

							if (futurefishPos.x < fish_WaterRect.left + 1.f || futurefishPos.x > fish_WaterRect.left + fish_WaterRect.width - 1.f) {
								otherForward.x = -otherForward.x;
							}
							if (futurefishPos.y < fish_WaterRect.top + 1.f || futurefishPos.y > fish_WaterRect.top + fish_WaterRect.height - 1.f) {
								otherForward.y = -otherForward.y;
							}

							addVectorsf(&fish_fish.pos, multiplyVectorsrf(otherForward, dt * fish_fish.speed));
							fish_fish.rotationAngle = getAngleBetweenVectors(vector2f(0.f, 0.f), fish_fish.forward) * RAD2DEG;
						}
						else {
							fish_fish.hasBeenAttracted = sfFalse;
						}
					}
				}
			}
			else if (fish_fish.type == FISH_ANGER)
			{
				if (fish_fish.effect == FISH_STASED && !hook_IsReseting() && isInFOV(hookPos, fish_fish.pos, normalizef(fish_fish.forward), fish_fish.theta, fish_fish.rad)) {
					fish_fish.effectTimer = FISH_EFFECT_DURATION - 0.1f;
					fish_fish.speed = 200.f;
					fish_fish.wasFollowing = sfTrue;
					fish_fish.targetForward = normalizef(createVectorWithVectorf(fish_fish.pos, hookPos));
					fish_fish.forward = vec2f_lerp(fish_fish.forward, fish_fish.targetForward, 10.f * dt);
					sfVector2f futurefishPos = vector2f(fish_fish.pos.x + fish_fish.forward.x * dt * fish_fish.speed, fish_fish.pos.y + fish_fish.forward.y * dt * fish_fish.speed);
					sfVector2f otherForward = fish_fish.forward;

					if (futurefishPos.x < fish_WaterRect.left + 1.f || futurefishPos.x > fish_WaterRect.left + fish_WaterRect.width - 1.f) {
						otherForward.x = -otherForward.x;
					}
					if (futurefishPos.y < fish_WaterRect.top + 1.f || futurefishPos.y > fish_WaterRect.top + fish_WaterRect.height - 1.f) {
						otherForward.y = -otherForward.y;
					}
					addVectorsf(&fish_fish.pos, multiplyVectorsrf(otherForward, dt * fish_fish.speed));
					fish_fish.rotationAngle = getAngleBetweenVectors(vector2f(0.f, 0.f), fish_fish.forward) * RAD2DEG;

					if (hook_CanBeCaptured(fish_fish.pos)) {
						fish_SetupFish(FISH_ANGER);
					}
				}
				else
				{
					float offset;
					if (fish_fish.effect == FISH_STASED) {
						fish_fish.speed = 200.f;
						offset = 50.f;
					}
					else {
						fish_fish.speed = 1000.f;
						offset = 150.f;
					}

					if (fish_fish.wasFollowing) {
						fish_fish.wasFollowing = sfFalse;
						fish_fish.startPos = fish_fish.pos;
						if (hookPos.y > 300.f && !hook_IsReseting())
							fish_fish.endPos = hookPos;
						else
							fish_fish.endPos = vector2f(rand_float(0.f, 1920.f), rand_float(700.f, 1000.f));

					}
					if (equalsWithOffset(fish_fish.pos, fish_fish.endPos, offset)) {
						fish_fish.startPos = fish_fish.pos;
						if (hookPos.y > 300.f && !hook_IsReseting())
							fish_fish.endPos = hookPos;
						else {
							fish_fish.endPos = fish_GetDefaultPos();
							for (int i = 0; i < 100; i++)
							{
								sfVector2f tmpEndPos = polarCoord(fish_fish.startPos, fish_fish.maxDistance, rand_float(0.f, PI * 2.f));
								if (sfFloatRect_contains(&fish_fishArea, tmpEndPos.x, tmpEndPos.y)) {
									fish_fish.endPos = tmpEndPos;
									break;
								}
							}
						}
					}

					fish_fish.targetForward = normalizef(createVectorWithVectorf(fish_fish.pos, fish_fish.endPos));
					fish_fish.forward = vec2f_lerp(fish_fish.forward, fish_fish.targetForward, 2.f * dt);

					addVectorsf(&fish_fish.pos, multiplyVectorsrf(fish_fish.forward, dt * fish_fish.speed));
					fish_fish.rotationAngle = getAngleBetweenVectors(vector2f(0.f, 0.f), fish_fish.forward) * RAD2DEG;
				}
			}
			*/

			// eat hook_hook
			if (!hook_IsReseting() && hook_IsFishEatingHook(FlRect(fish_fish.pos.x - fish_fish.origin.x, fish_fish.pos.y - fish_fish.origin.y, fish_fish.size.x, fish_fish.size.y))) {
				hook_ResetHook();
				fish_fish.effect = FISH_NO_EFFECT;
			}
		}
	}
}

void fish_DisplayFish(Window* _window)
{
	if (!fish_fish.IsFishGameFinish)
	{
		if (fish_fish.forward.x < 0.f)
			sfRectangleShape_setScale(fish_Rectangle, vector2f(-1.f, -1.f));
		else
			sfRectangleShape_setScale(fish_Rectangle, vector2f(-1.f, 1.f));
		sfRectangleShape_setPosition(fish_Rectangle, fish_fish.pos);
		sfRectangleShape_setOrigin(fish_Rectangle, fish_fish.origin);
		sfRectangleShape_setSize(fish_Rectangle, fish_fish.size);
		sfRectangleShape_setRotation(fish_Rectangle, fish_fish.rotationAngle);
		switch (fish_fish.type)
		{
		case FISH_JOY:
		case FISH_JOY_TUTO:
			switch (fish_fish.anim)
			{
			case FISH_IDLE:
				sfRectangleShape_setTextureRect(fish_Rectangle, animatorAnim("Fishing_JoyIdle", &Fish_FramesFish, &Fish_TimerFish, 0));
				break;
			case FISH_FLEEING:
				sfRectangleShape_setTextureRect(fish_Rectangle, animatorAnim("Fishing_JoyIdle", &Fish_FramesFish, &Fish_TimerFish, 0));
				break;
			case FISH_FOLLOWING:
				sfRectangleShape_setTextureRect(fish_Rectangle, animatorAnim("Fishing_JoyFollow", &Fish_FramesFish, &Fish_TimerFish, 0));
				break;
			case FISH_UPSET:
				sfRectangleShape_setTextureRect(fish_Rectangle, animatorAnim("Fishing_JoyUpset", &Fish_FramesFish, &Fish_TimerFish, 0));
				break;
			}
			break;
		case FISH_SADNESS:
		case FISH_SADNESS_TUTO:
			switch (fish_fish.anim)
			{
			case FISH_IDLE:
				sfRectangleShape_setTextureRect(fish_Rectangle, animatorAnim("Fishing_SadnessIdle", &Fish_FramesFish, &Fish_TimerFish, 0));
				break;
			case FISH_FLEEING:
				sfRectangleShape_setTextureRect(fish_Rectangle, animatorAnim("Fishing_SadnessFlee", &Fish_FramesFish, &Fish_TimerFish, 0));
				break;
			case FISH_FOLLOWING:
				sfRectangleShape_setTextureRect(fish_Rectangle, animatorAnim("Fishing_SadnessFollow", &Fish_FramesFish, &Fish_TimerFish, 0));
				break;
			case FISH_UPSET:
				sfRectangleShape_setTextureRect(fish_Rectangle, animatorAnim("Fishing_SadnessUpset", &Fish_FramesFish, &Fish_TimerFish, 0));
				break;
			}
			break;
		}
		sfRenderTexture_drawRectangleShape(_window->rdrtShader, fish_Rectangle, NULL);
	}

	if (fish_fish.type == FISH_SCARED) {
		for (int i = 0; i < NBR_OF_ROCKS; i++)
		{
			sfRectangleShape_setPosition(fish_Rectangle, rock[i].position);
			sfRectangleShape_setOrigin(fish_Rectangle, vector2f(50.0f, 50.0f));
			sfRectangleShape_setRotation(fish_Rectangle, 0.f);
			sfRectangleShape_setSize(fish_Rectangle, vector2f(100.0f, 100.0f));
			sfRenderTexture_drawRectangleShape(_window->rdrtShader, fish_Rectangle, NULL);
		}

		sfRectangleShape_setPosition(fish_Rectangle, rock[fish_fish.rockChoiced].position);
		sfRectangleShape_setOrigin(fish_Rectangle, vector2f(5.f, 5.f));
		sfRectangleShape_setSize(fish_Rectangle, vector2f(10.f, 10.f));
		sfRectangleShape_setRotation(fish_Rectangle, 0.f);
		sfRenderTexture_drawRectangleShape(_window->rdrtShader, fish_Rectangle, NULL);
	}
	/*
	POUR FAIRE D'AUTRES POISSONS CA MONTRE L'ENDROIT QU'IL VISE 
	
	sfRectangleShape_setPosition(fish_Rectangle, fish_fish.endPos);
	sfRectangleShape_setOrigin(fish_Rectangle, vector2f(5.f, 5.f));
	sfRectangleShape_setSize(fish_Rectangle, vector2f(10.f, 10.f));
	sfRectangleShape_setRotation(fish_Rectangle, 0.f);
	sfRenderTexture_drawRectangleShape(_window->rdrt, fish_Rectangle, NULL);
	*/
//	sfVector2f hookPos = hook_GetHookPos();

	hook_DisplaySeaweed(_window);

	sfRectangleShape_setTextureRect(fish_PowerUpAttract, IntRect(72, 3665, 72, 72));
	if (fish_PowerTimer > FISH_POWER_EFFECT_COOLDOWN) {
		sfRectangleShape_setTextureRect(fish_PowerUpAttract, IntRect(0, 3665, 72, 72));
	}
	
	if (fish_fish.type == FISH_JOY_TUTO)
	{
		if (!fish_IsFirstView)
		{
			fish_HaveToStart = sfTrue;
			sfRectangleShape_setSize(fish_TutoRectangle, vector2f(1920.f, 580.f));
			sfRectangleShape_setPosition(fish_TutoRectangle, vector2f(0.f, 600.f));
			sfRectangleShape_setFillColor(fish_TutoRectangle, sfColor_fromRGBA(255, 0, 0, 150));
			sfRenderTexture_drawRectangleShape(_window->rdrt, fish_TutoRectangle, NULL);

			hook_TutoCaptureZone();
			sfRenderTexture_drawRectangleShape(_window->rdrt, fish_OtherTutoRectangle, NULL);
			sfText_setString(fish_GameTutoText, lang_GetFromKey("fish.view.joy"));
			sfRenderTexture_drawText(_window->rdrt, fish_GameTutoText, NULL);
		}
		if (fish_ReadyToUsed)
		{
			sfRectangleShape_setSize(fish_Feedback, vector2f(132.f, 70.f));
			if (hook_ReturnCaptureZoneX() > 1920.f / 2.f)
				sfRectangleShape_setPosition(fish_Feedback, vector2f(hook_ReturnCaptureZoneX() - 150.f, 30.f));
			else
				sfRectangleShape_setPosition(fish_Feedback, vector2f(hook_ReturnCaptureZoneX() + 150.f, 30.f));
			if (fish_frameFeedback <= 1)
				sfRectangleShape_setTextureRect(fish_Feedback, animatorAnim("Fishing_PressRT", &fish_frameFeedback, &fish_timerFeedback, 0));
			else
			{
				fish_frameFeedback -= 2;
				sfRectangleShape_setTextureRect(fish_Feedback, animatorAnim("Fishing_PressLT", &fish_frameFeedback, &fish_timerFeedback, 0));
				fish_frameFeedback += 2;
				if (fish_frameFeedback == 4)
					fish_frameFeedback = 0;
			}

			sfRenderTexture_drawRectangleShape(_window->rdrt, fish_Feedback, NULL);

			sfRenderTexture_drawRectangleShape(_window->rdrt, fish_OtherTutoRectangle, NULL);
			sfText_setString(fish_GameTutoText, lang_GetFromKey("fish.reel"));
			sfRenderTexture_drawText(_window->rdrt, fish_GameTutoText, NULL);
		}
		if (fish_IsNearHook)
		{
			sfRenderTexture_drawRectangleShape(_window->rdrt, fish_OtherTutoRectangle, NULL);
			sfText_setString(fish_GameTutoText, lang_GetFromKey("fish.range"));
			sfRenderTexture_drawText(_window->rdrt, fish_GameTutoText, NULL);
		}
		if (fish_ReadyEat)
		{
			sfRenderTexture_drawRectangleShape(_window->rdrt, fish_OtherTutoRectangle, NULL);
			sfText_setString(fish_GameTutoText, lang_GetFromKey("fish.eat"));
			sfRenderTexture_drawText(_window->rdrt, fish_GameTutoText, NULL);
		}
		if (fish_ReadyReception)
		{
			sfRenderTexture_drawRectangleShape(_window->rdrt, fish_OtherTutoRectangle, NULL);
			sfText_setString(fish_GameTutoText, lang_GetFromKey("fish.catching.zone"));
			sfRenderTexture_drawText(_window->rdrt, fish_GameTutoText, NULL);
		}
	}
	if (fish_fish.type == FISH_SADNESS_TUTO)
	{
		if (!fish_IsFirstView)
		{
			hook_TutoCaptureZone();
			sfRenderTexture_drawRectangleShape(_window->rdrt, fish_OtherTutoRectangle, NULL);
			sfText_setString(fish_GameTutoText, lang_GetFromKey("fish.view.sadness"));
			sfRenderTexture_drawText(_window->rdrt, fish_GameTutoText, NULL);
		}
		if (fish_IsNearHook)
		{
			sfRenderTexture_drawRectangleShape(_window->rdrt, fish_OtherTutoRectangle, NULL);
			sfText_setString(fish_GameTutoText, lang_GetFromKey("fish.power.attract"));
			sfRenderTexture_drawText(_window->rdrt, fish_GameTutoText, NULL);
		}
	}
	
	if (fish_fish.type == FISH_SADNESS_TUTO || fish_fish.type == FISH_SADNESS)
		sfRenderTexture_drawRectangleShape(_window->rdrt, fish_PowerUpAttract, NULL);

	if (fish_HaveToStart)
	{
		sfRectangleShape_setSize(fish_Feedback, vector2f(132.f, 70.f));
		if (hook_ReturnCaptureZoneX() > 1920.f / 2.f)
			sfRectangleShape_setPosition(fish_Feedback, vector2f(hook_ReturnCaptureZoneX() - 150.f, 30.f));
		else
			sfRectangleShape_setPosition(fish_Feedback, vector2f(hook_ReturnCaptureZoneX() + 150.f, 30.f));
		sfRectangleShape_setTextureRect(fish_Feedback, animatorAnim("Fishing_PressA", &fish_frameFeedback, &fish_timerFeedback, 0));
		sfRenderTexture_drawRectangleShape(_window->rdrt, fish_Feedback, NULL);
	}
	else if (fish_CanCatch)
	{
		sfRectangleShape_setSize(fish_Feedback, vector2f(183.f, 70.f));
		if (hook_ReturnCaptureZoneX() > 1920.f / 2.f)
			sfRectangleShape_setPosition(fish_Feedback, vector2f(hook_ReturnCaptureZoneX() - 200.f, 30.f));
		else
			sfRectangleShape_setPosition(fish_Feedback, vector2f(hook_ReturnCaptureZoneX() + 200.f, 30.f));
		sfRectangleShape_setTextureRect(fish_Feedback, animatorAnim("Fishing_PressBumps", &fish_frameFeedback, &fish_timerFeedback, 0));
		sfRenderTexture_drawRectangleShape(_window->rdrt, fish_Feedback, NULL);
	}
	fish_HaveToStart = sfFalse;
}

void fish_SetupFish(FishType _type)
{
	fish_PowerTimer = 3.f;

	fish_fish.type = _type;
	fish_fish.effect = FISH_NO_EFFECT;
	fish_fish.effectTimer = 0.f;
	fish_fish.startEffectPos = vector2f(0.f, 0.f);
	fish_fish.endEffectPos = vector2f(0.f, 0.f);

	fish_fish.isFound = sfFalse;
	fish_fish.isAnimFinished = sfFalse;
	fish_fish.waitTimer = 0.0f;
	fish_fish.rockChoiced = 0;
	fish_fish.hasBeenAttracted = sfFalse;

	switch (fish_fish.type)
	{
	case FISH_JOY_TUTO:
		fish_fish.pos = vector2f(rand_float(100.f, FISH_GROUND_X_POS - 100.f), rand_float(600.f, 800.f));
		fish_fish.speed = 0.f;
		fish_fish.otherSpeed = rand_float(50.f, 100.f);
		fish_fish.maxSpeed = rand_float(200.f, 350.f);
		fish_fish.maxDistance = 400.f;
		fish_fish.startPos = fish_fish.pos;
		fish_fish.endPos = polarCoord(fish_fish.startPos, fish_fish.maxDistance, rand_float(0.f, PI * 2.f));
		fish_fish.rotationAngle = getAngleBetweenVectors(vector2f(0.f, 0.f), createVectorWithVectorf(fish_fish.startPos, fish_fish.endPos)) * RAD2DEG;
		fish_fish.forward = normalizef(createVectorWithVectorf(fish_fish.pos, fish_fish.endPos));
		fish_fish.targetForward = fish_fish.forward;
		fish_fish.size = vector2f(86.f, 62.f);
		fish_fish.origin = vector2f(86.f / 2.f, 62.f / 2.f);
		fish_fish.theta = 120.f;
		fish_fish.rad = 175.f;
		fish_fish.otherTheta = 360.f;
		fish_fish.otherRad = 100.f;
		fish_fish.timer = 0.f;
		fish_fish.wasFollowing = sfFalse;
		fish_fish.steeringIntensity = 100.f;
		fish_fish.firstAnim = sfFalse;
		fish_fish.secondAnim = sfFalse;
		break;
	case FISH_JOY:
		fish_fish.pos = vector2f(rand_float(100.f, FISH_GROUND_X_POS - 100.f), rand_float(600.f, 800.f));
		fish_fish.speed = 0.f;
		fish_fish.otherSpeed = rand_float(50.f, 100.f);
		fish_fish.maxSpeed = rand_float(200.f, 350.f);
		fish_fish.maxDistance = 400.f;
		fish_fish.startPos = fish_fish.pos;
		fish_fish.endPos = polarCoord(fish_fish.startPos, fish_fish.maxDistance, rand_float(0.f, PI * 2.f));
		fish_fish.rotationAngle = getAngleBetweenVectors(vector2f(0.f, 0.f), createVectorWithVectorf(fish_fish.startPos, fish_fish.endPos)) * RAD2DEG;
		fish_fish.forward = normalizef(createVectorWithVectorf(fish_fish.pos, fish_fish.endPos));
		fish_fish.targetForward = fish_fish.forward;
		fish_fish.size = vector2f(86.f, 62.f);
		fish_fish.origin = vector2f(86.f / 2.f, 62.f / 2.f);
		fish_fish.theta = 120.f;
		fish_fish.rad = 175.f;
		fish_fish.otherTheta = 360.f;
		fish_fish.otherRad = 100.f;
		fish_fish.timer = 0.f;
		fish_fish.wasFollowing = sfFalse;
		fish_fish.steeringIntensity = 100.f;
		fish_fish.firstAnim = sfFalse;
		fish_fish.secondAnim = sfFalse;
		break;
	case FISH_SADNESS:
	case FISH_SADNESS_TUTO:
		fish_fish.pos = vector2f(rand_float(100.f, FISH_GROUND_X_POS - 100.f), rand_float(600.f, 800.f));
		fish_fish.speed = 100.f;
		fish_fish.maxDistance = 400.f;
		fish_fish.startPos = fish_fish.pos;
		fish_fish.endPos = polarCoord(fish_fish.startPos, fish_fish.maxDistance, rand_float(0.f, PI * 2.f));
		fish_fish.rotationAngle = getAngleBetweenVectors(vector2f(0.f, 0.f), createVectorWithVectorf(fish_fish.startPos, fish_fish.endPos)) * RAD2DEG;
		fish_fish.forward = normalizef(createVectorWithVectorf(fish_fish.pos, fish_fish.endPos));
		fish_fish.targetForward = fish_fish.forward;
		fish_fish.origin = vector2f(FISH_X_ORIGIN, FISH_Y_ORIGIN);
		fish_fish.size = vector2f(FISH_X_SIZE, FISH_Y_SIZE);
		fish_fish.theta = 360.f;
		fish_fish.rad = 200.f;
		fish_fish.otherTheta = 360.f;
		fish_fish.otherRad = 75.f;
		fish_fish.timer = 0.f;
		fish_fish.wasFollowing = sfFalse;
		fish_fish.steeringIntensity = 100.f;
		fish_fish.firstAnim = sfFalse;
		fish_fish.secondAnim = sfFalse;
		break;
	case FISH_ANGER:
		fish_fish.pos = vector2f(rand_float(100.f, FISH_GROUND_X_POS - 100.f), rand_float(600.f, 800.f));
		fish_fish.speed = 1500.f;
		fish_fish.maxDistance = 400.f;
		fish_fish.startPos = fish_fish.pos;
		fish_fish.endPos = polarCoord(fish_fish.startPos, fish_fish.maxDistance, rand_float(0.f, PI * 2.f));
		fish_fish.rotationAngle = getAngleBetweenVectors(vector2f(0.f, 0.f), createVectorWithVectorf(fish_fish.startPos, fish_fish.endPos)) * RAD2DEG;
		fish_fish.forward = normalizef(createVectorWithVectorf(fish_fish.pos, fish_fish.endPos));
		fish_fish.targetForward = fish_fish.forward;
		fish_fish.origin = vector2f(FISH_X_ORIGIN, FISH_Y_ORIGIN);
		fish_fish.size = vector2f(FISH_X_SIZE, FISH_Y_SIZE);
		fish_fish.theta = 60.f;
		fish_fish.rad = 250.f;
		fish_fish.otherTheta = 360.f;
		fish_fish.otherRad = 50.f;
		fish_fish.timer = 0.f;
		fish_fish.wasFollowing = sfFalse;
		fish_fish.steeringIntensity = 100.f;
		fish_fish.firstAnim = sfFalse;
		fish_fish.secondAnim = sfFalse;
		break;
	case FISH_SCARED:
		for (int i = 0; i < NBR_OF_ROCKS; i++)
		{
			rock[i].position = vector2f(150.0f + 200.0f * i, 1033.0f);
			rock[i].isFishHere = sfFalse;
		}
		fish_fish.rockChoiced = rand() % NBR_OF_ROCKS;
		rock[fish_fish.rockChoiced].isFishHere = sfTrue;
		fish_fish.pos = vector2f(500.0f, 1000.0f);
		fish_fish.speed = 1300.f;
		fish_fish.maxDistance = 400.f;
		fish_fish.startPos = fish_fish.pos;
		fish_fish.endPos = polarCoord(fish_fish.startPos, fish_fish.maxDistance, rand_float(0.f, PI * 2.f));
		fish_fish.rotationAngle = getAngleBetweenVectors(vector2f(0.f, 0.f), createVectorWithVectorf(fish_fish.startPos, fish_fish.endPos)) * RAD2DEG;
		fish_fish.forward = normalizef(createVectorWithVectorf(fish_fish.pos, fish_fish.endPos));
		fish_fish.targetForward = fish_fish.forward;
		fish_fish.origin = vector2f(FISH_X_ORIGIN, FISH_Y_ORIGIN);
		fish_fish.size = vector2f(FISH_X_SIZE, FISH_Y_SIZE);
		fish_fish.theta = 70.f;
		fish_fish.rad = 200.f;
		fish_fish.timer = 0.f;
		fish_fish.wasFollowing = sfFalse;
		fish_fish.steeringIntensity = 100.f;
		fish_fish.firstAnim = sfTrue;
		fish_fish.secondAnim = sfTrue;
		break;
	default:
		fish_fish.pos = vector2f(rand_float(100.f, FISH_GROUND_X_POS - 100.f), rand_float(600.f, 800.f));
		fish_fish.speed = 100.f;
		fish_fish.maxDistance = 400.f;
		fish_fish.startPos = fish_fish.pos;
		fish_fish.endPos = polarCoord(fish_fish.startPos, fish_fish.maxDistance, rand_float(0.f, PI * 2.f));
		fish_fish.rotationAngle = getAngleBetweenVectors(vector2f(0.f, 0.f), createVectorWithVectorf(fish_fish.startPos, fish_fish.endPos)) * RAD2DEG;
		fish_fish.forward = normalizef(createVectorWithVectorf(fish_fish.pos, fish_fish.endPos));
		fish_fish.targetForward = fish_fish.forward;
		fish_fish.origin = vector2f(FISH_X_ORIGIN, FISH_Y_ORIGIN);
		fish_fish.size = vector2f(FISH_X_SIZE, FISH_Y_SIZE);
		fish_fish.theta = 120.f;
		fish_fish.rad = 250.f;
		fish_fish.timer = 0.f;
		fish_fish.wasFollowing = sfFalse;
		fish_fish.steeringIntensity = 100.f;
		fish_fish.firstAnim = sfFalse;
		fish_fish.secondAnim = sfFalse;
		break;
	}
}

void fish_pushRock()
{
	for (int i = 0; i < NBR_OF_ROCKS; i++)
	{
		
	}
	if (getSqrMagnitudef(createVectorWithVectorf(hook_GetHookPos(), rock[fish_fish.rockChoiced].position)) < FISH_POWER_EFFECT_RADIUS * FISH_POWER_EFFECT_RADIUS)
	{
		rock[fish_fish.rockChoiced].position.y -= 20.0f;
		fish_fish.isFound = sfTrue;
	}
}

void fish_LookingForHome()
{
	float delta = getDeltaTime();

	if (fish_fish.pos.x > 100.0f && fish_fish.firstAnim)
		fish_fish.pos.x -= 1000.0f * delta;
	else
	{
		fish_fish.firstAnim = sfFalse;
		if (fish_fish.pos.x < 1800.0f && fish_fish.secondAnim)
			fish_fish.pos.x += 1000.0f * delta;
		else
		{
			fish_fish.secondAnim = sfFalse;
			if (fish_fish.pos.x > rock[fish_fish.rockChoiced].position.x)
				fish_fish.pos.x -= 1000.0f * delta;
			else
			{
				if (fish_fish.pos.y < 1200.0f)
					fish_fish.pos.y += 300.0f * delta;
				else
				{
					fish_fish.firstAnim = sfTrue;
					fish_fish.secondAnim = sfTrue;
					fish_fish.isAnimFinished = sfTrue;
				}
			}
		}

	}
}

sfBool fish_IsInRadius(sfVector2f _hookPos)
{
	if (getSqrMagnitudeBetween2Vectorsf(_hookPos, fish_fish.pos) <= FISH_POWER_EFFECT_RADIUS * FISH_POWER_EFFECT_RADIUS)
		return sfTrue;
	
	return sfFalse;
}

/*
prend en compte ou pas le cone
void toRemoveCustomFOV(sfVertexArray* FOV, float teta, float rad, sfVector2f pos, sfVector2f forward, sfColor _color, float _posOffset, sfBool _first, sfColor _firstColor)
{
	int more = 0;
	int count = 0;
	if (_first) {
		more = 1;
		count = 1;
	}
	sfVertex* view = malloc(sizeof(sfVertex) * ((int)teta + more));
	if (_first) {
		view[0].position = polarCoord(pos, 0.f, atan2(forward.y, forward.x) + _posOffset);
		view[0].color = _firstColor;
		sfVertexArray_append(fish_vArray, view[0]);
	}

	for (int i = (int)(-teta / 2); i < (int)(teta / 2); i++) {
		view[count].position = polarCoord(pos, rad, atan2f(forward.y, forward.x) + i * DEG2RAD + _posOffset);
		view[count].color = _color;
		sfVertexArray_append(fish_vArray, view[count]);
		count++;
	}
	if (_first) {
		sfVertexArray_append(fish_vArray, view[0]);
	}
	free(view);
}
*/

void fish_SetPowerTimer(float _timer)
{
	fish_PowerTimer = _timer;
}

FishType fish_GetFishType()
{
	return fish_fish.type;
}

sfVector2f fish_GetDefaultPos()
{
	return vector2f(rand_float(fish_fishArea.left, fish_fishArea.width - fish_fishArea.left), rand_float(fish_fishArea.top, fish_fishArea.height - fish_fishArea.top));
}

void fish_DeinitFish()
{
	sfRectangleShape_destroy(fish_Rectangle);
	sfRectangleShape_destroy(fish_PowerUpAttract);
}

void fish_changeReadyToUsed()
{
	if (fish_fish.type == FISH_JOY_TUTO || fish_fish.type == FISH_SADNESS_TUTO)
	{
		fish_ReadyToUsed = !fish_ReadyToUsed;
		hook_ToggleMovement();
	}
}

void fish_StopPower()
{
	fish_CanUsePower = sfFalse;
}

sfBool fish_isOutOfTheWater(sfFloatRect _fishBounds)
{
	if (_fishBounds.top < fish_WaterRect.top)
		return sfTrue;
	return sfFalse;
}

void toogle_HaveToStart()
{
	fish_HaveToStart = sfTrue;
}