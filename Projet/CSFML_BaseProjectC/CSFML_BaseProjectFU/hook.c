#include "hook.h"
#include "tools.h"
#include "windowManager.h"
#include "controller.h"
#include "math.h"
#include "fish.h"
#include "fadeManager.h"
#include "List.h"
#include "textureManager.h"
#include "soundManager.h"
#include "shaderManager.h"
#include "particlesManager.h"

#define CONTROLLER_OFFSET 0.2f
#define TRIGGER_OFFSET 0.1f

sfRectangleShape* hook_waterRectangle;

sfRectangleShape* hook_hookRing;
Hook hook_hook;

sfBool isAttractHere;

sfRectangleShape* hook_FishReception;
float hook_ReceptionTimerSad;
float hook_FishReceptionTimer;
int hook_FishReceptionFrameX;
Hook_Emotion hook_ReceptionEmotion;


sfRectangleShape* hook_Reel;
sfRectangleShape* hook_ReelHand;
float hook_Angle;
float hook_BaseSpeed = 50;

sfFloatRect fish_captureZone;
sfBool hook_IsFirstView;
sfBool hook_CanMove = sfTrue;
sfBool hook_FirstTimeDrop;
sfBool hook_FirstTimeTrigger;

#define GD_CORAL STD_LIST_GETDATA(CoralList, Hook_Coral, i)
#define GD_CLOUD STD_LIST_GETDATA(CloudList, Hook_Cloud, i)
#define GD_SHELL STD_LIST_GETDATA(ShellList, Hook_Shell, i)
#define GD_SEAWEED STD_LIST_GETDATA(SeaweedList, Hook_Seaweed, i)
#define GD_ROCK STD_LIST_GETDATA(RockList, Hook_Rock, i)
#define GD_SPLASH STD_LIST_GETDATA(SplashList, Hook_Splash, i)

stdList* CoralList;
stdList* CloudList;
stdList* ShellList;
stdList* SeaweedList;
stdList* RockList;
stdList* SplashList;

sfRectangleShape* Hook_SplashRect;
sfBool Hook_onePassWave;

sfRectangleShape* Hook_Star;
float Hook_StarTimer;
int Hook_StarFrameX;

sfRectangleShape* hook_ElementsOnScreen;
sfSprite* hook_sprite;
sfTexture* hook_spritesheet;
sfTexture* hook_bg;
float Hook_timerPhaos;
int Hook_FrameXPhaos;
float Hook_timerRing;
int Hook_FrameXRing;

sfRectangleShape* hook_PowerRing;
sfBool hook_PowerRingIsDisplayed;
float hook_PowerRingTimer;
int hook_PowerRingFrameX;

sfRectangleShape* hook_Bubble;
sfBool hook_BubbleIsDisplayed;
sfVector2f hook_BubblePos;
float hook_BoolTimer;
float hook_BubbleTimer;
int hook_BubbleFrameX;
float hook_BubbleTimer2;
int hook_BubbleFrameX2;

float hook_ShaderTimer;
float hook_ParticlesTimer;

sfBool hook_canPlaySound;

sfBool hook_SplashSound0;
float hook_SplashSound0Timer;
sfBool hook_SplashSound1;
float hook_SplashSound1Timer;
sfBool hook_SplashSound2;
float hook_SplashSound2Timer;
sfBool hook_SplashSound3;
float hook_SplashSound3Timer;
sfBool hook_SplashSound4;
float hook_SplashSound4Timer;
sfBool hook_SplashSound5;
float hook_SplashSound5Timer;
sfBool hook_SplashSound6;
float hook_SplashSound6Timer;
sfBool hook_SplashSound7;
float hook_SplashSound7Timer;
sfBool hook_SplashSound8;
float hook_SplashSound8Timer;
sfBool hook_SplashSound9;
float hook_SplashSound9Timer;

void hook_InitHook(Window* _window) {
	hook_ElementsOnScreen = sfRectangleShape_create();

	hook_setMovement(sfTrue);

	hook_spritesheet = GetSfTexture("fishing_SpriteSheet");
	sfRectangleShape_setTexture(hook_ElementsOnScreen, hook_spritesheet, sfTrue);

	hook_sprite = sfSprite_create();
	sfSprite_setPosition(hook_sprite, vector2f(0, 0));
	hook_Bubble = sfRectangleShape_create();
	sfRectangleShape_setSize(hook_Bubble, vector2f(102.f, 102.f));
	sfRectangleShape_setOrigin(hook_Bubble, vector2f(51.f, 51.f));
	sfRectangleShape_setTexture(hook_Bubble, hook_spritesheet, sfTrue);

	hook_waterRectangle = sfRectangleShape_create();
	sfRectangleShape_setOrigin(hook_waterRectangle, vector2f(0, 930));
	sfRectangleShape_setSize(hook_waterRectangle, vector2f(1920, 930));
	sfRectangleShape_setPosition(hook_waterRectangle, vector2f(0, 1080));
	hook_hook.isReseting = sfTrue;
	hook_hook.velocity = vector2f(0, 0);
	hook_hook.speed = hook_BaseSpeed;
	hook_hook.forward = vector2f(0, 0);
	hook_hook.oldForward = vector2f(0, 0);
	hook_hook.animTimer = 0;
	hook_hook.resetPos = HOOK_RESET_POS;
	hook_hook.startPos = hook_hook.pos;
	hook_hook.resetTimer = 0.f;
	hook_Angle = 180;
	fish_captureZone = FlRect(FISH_CAPTURE_ZONE_X_POS, FISH_CAPTURE_ZONE_Y_POS, FISH_CAPTURE_ZONE_X_SIZE, FISH_CAPTURE_ZONE_Y_SIZE);
	Hook_timerPhaos = 0.f;
	Hook_FrameXPhaos = 0;
	Hook_timerRing = 0.f;
	Hook_FrameXRing = 0;
	hook_BubbleIsDisplayed = sfFalse;
	hook_FirstTimeDrop = sfFalse;
	hook_BubbleTimer = 0.f;
	hook_BubbleFrameX = 0;
	hook_BubbleTimer2 = 0.f;
	hook_BubbleFrameX2 = 0;
	hook_ParticlesTimer = 0.f;

	hook_hookRing = sfRectangleShape_create();
	sfRectangleShape_setOrigin(hook_hookRing, vector2f(HOOK_X_ORIGIN, HOOK_Y_ORIGIN));
	sfRectangleShape_setSize(hook_hookRing, vector2f(HOOK_X_SIZE, HOOK_Y_SIZE));
	sfRectangleShape_setPosition(hook_hookRing, hook_hook.pos);
	sfRectangleShape_setTexture(hook_hookRing, hook_spritesheet, sfTrue);

	hook_Reel = sfRectangleShape_create();
	sfRectangleShape_setSize(hook_Reel, vector2f(84.f, 84.f));
	sfRectangleShape_setOrigin(hook_Reel, vector2f(42.f, 42.f));
	sfRectangleShape_setPosition(hook_Reel, vector2f(1800.f, 1000.f));
	sfRectangleShape_setTexture(hook_Reel, hook_spritesheet, sfTrue);
	sfRectangleShape_setTextureRect(hook_Reel, (sfIntRect) { 0, 3501, 84, 84 });

	hook_ReelHand = sfRectangleShape_create();
	sfRectangleShape_setSize(hook_ReelHand, vector2f(26, 80));
	sfRectangleShape_setOrigin(hook_ReelHand, vector2f(13, 70));
	sfRectangleShape_setPosition(hook_ReelHand, vector2f(1800.f, 1000.f));
	sfRectangleShape_setRotation(hook_ReelHand, hook_Angle);
	sfRectangleShape_setTexture(hook_ReelHand, hook_spritesheet, sfTrue);
	sfRectangleShape_setTextureRect(hook_ReelHand, (sfIntRect) { 0, 3585, 26, 80 });

	hook_FishReception = sfRectangleShape_create();
	sfRectangleShape_setOrigin(hook_FishReception, vector2f(0.f, 0.f));
	sfRectangleShape_setTexture(hook_FishReception, hook_spritesheet, sfTrue);
	hook_ReceptionEmotion = HOOK_RECEPTION_IDLE;

	hook_PowerRing = sfRectangleShape_create();
	sfRectangleShape_setSize(hook_PowerRing, vector2f(600.f, 600.f));
	sfRectangleShape_setOrigin(hook_PowerRing, vector2f(300.f, 300.f));
	sfRectangleShape_setTexture(hook_PowerRing, hook_spritesheet, sfTrue);

	Hook_SplashRect = sfRectangleShape_create();
	sfRectangleShape_setSize(Hook_SplashRect, vector2f(90.f, 40.f));
	sfRectangleShape_setOrigin(Hook_SplashRect, vector2f(45.f, 40.f));
	sfRectangleShape_setTexture(Hook_SplashRect, hook_spritesheet, sfTrue);

	Hook_Star = sfRectangleShape_create();
	sfRectangleShape_setSize(Hook_Star, vector2f(56.f, 58.f));
	sfRectangleShape_setOrigin(Hook_Star, vector2f(26.f, 24.f));
	sfRectangleShape_setTexture(Hook_Star, hook_spritesheet, sfTrue);

	hook_RandomCaptureZone();

	hook_PowerRingIsDisplayed = sfFalse;
	hook_IsFirstView = sfTrue;
	hook_FirstTimeDrop = sfTrue;
	hook_FirstTimeTrigger = sfFalse;

	CloudList = STD_LIST_CREATE(Hook_Cloud, 0);
	ShellList = STD_LIST_CREATE(Hook_Shell, 0);
	CoralList = STD_LIST_CREATE(Hook_Coral, 0);
	RockList = STD_LIST_CREATE(Hook_Rock, 0);
	SeaweedList = STD_LIST_CREATE(Hook_Seaweed, 0);
	SplashList = STD_LIST_CREATE(Hook_Splash, 0);

	hook_RandomSpawn(6, HOOK_CORAL);
	hook_RandomSpawn(8, HOOK_CLOUD);
	hook_RandomSpawn(5, HOOK_SEAWEED);

	hook_ShaderTimer = 0.f;
}

void hook_InitHookTuto(Window* _window) {
	hook_ElementsOnScreen = sfRectangleShape_create();
	hook_setMovement(sfTrue);

	hook_spritesheet = GetSfTexture("fishing_SpriteSheet");
	sfRectangleShape_setTexture(hook_ElementsOnScreen, hook_spritesheet, sfTrue);

	hook_sprite = sfSprite_create();
	sfSprite_setPosition(hook_sprite, vector2f(0, 0));
	hook_Bubble = sfRectangleShape_create();
	sfRectangleShape_setSize(hook_Bubble, vector2f(102.f, 102.f));
	sfRectangleShape_setOrigin(hook_Bubble, vector2f(51.f, 51.f));
	sfRectangleShape_setTexture(hook_Bubble, hook_spritesheet, sfTrue);

	hook_waterRectangle = sfRectangleShape_create();
	sfRectangleShape_setOrigin(hook_waterRectangle, vector2f(0, 930));
	sfRectangleShape_setSize(hook_waterRectangle, vector2f(1920, 930));
	sfRectangleShape_setPosition(hook_waterRectangle, vector2f(0, 1080));
	hook_hook.isReseting = sfTrue;
	hook_hook.velocity = vector2f(0, 0);
	hook_hook.speed = hook_BaseSpeed;
	hook_hook.forward = vector2f(0, 0);
	hook_hook.oldForward = vector2f(0, 0);
	hook_hook.animTimer = 0;
	hook_hook.resetPos = HOOK_RESET_POS;
	hook_hook.startPos = hook_hook.pos;
	hook_hook.resetTimer = 0.f;
	hook_Angle = 180;
	fish_captureZone = FlRect(FISH_CAPTURE_ZONE_X_POS, FISH_CAPTURE_ZONE_Y_POS, FISH_CAPTURE_ZONE_X_SIZE, FISH_CAPTURE_ZONE_Y_SIZE);
	Hook_timerPhaos = 0.f;
	Hook_FrameXPhaos = 0;
	Hook_timerRing = 0.f;
	Hook_FrameXRing = 0;
	hook_BubbleIsDisplayed = sfFalse;
	hook_FirstTimeDrop = sfFalse;
	hook_BubbleTimer = 0.f;
	hook_BubbleFrameX = 0;
	hook_BubbleTimer2 = 0.f;
	hook_ParticlesTimer = 0.f;
	hook_BubbleFrameX2 = 0;

	hook_hookRing = sfRectangleShape_create();
	sfRectangleShape_setOrigin(hook_hookRing, vector2f(HOOK_X_ORIGIN, HOOK_Y_ORIGIN));
	sfRectangleShape_setSize(hook_hookRing, vector2f(HOOK_X_SIZE, HOOK_Y_SIZE));
	sfRectangleShape_setPosition(hook_hookRing, hook_hook.pos);
	sfRectangleShape_setTexture(hook_hookRing, hook_spritesheet, sfTrue);

	hook_Reel = sfRectangleShape_create();
	sfRectangleShape_setSize(hook_Reel, vector2f(84.f, 84.f));
	sfRectangleShape_setOrigin(hook_Reel, vector2f(42.f, 42.f));
	sfRectangleShape_setPosition(hook_Reel, vector2f(1800.f, 1000.f));
	sfRectangleShape_setTexture(hook_Reel, hook_spritesheet, sfTrue);
	sfRectangleShape_setTextureRect(hook_Reel, (sfIntRect) { 0, 3501, 84, 84 });

	hook_ReelHand = sfRectangleShape_create();
	sfRectangleShape_setSize(hook_ReelHand, vector2f(26, 80));
	sfRectangleShape_setOrigin(hook_ReelHand, vector2f(13, 70));
	sfRectangleShape_setPosition(hook_ReelHand, vector2f(1800.f, 1000.f));
	sfRectangleShape_setRotation(hook_ReelHand, hook_Angle);
	sfRectangleShape_setTexture(hook_ReelHand, hook_spritesheet, sfTrue);
	sfRectangleShape_setTextureRect(hook_ReelHand, (sfIntRect) { 0, 3585, 26, 80 });

	hook_FishReception = sfRectangleShape_create();
	sfRectangleShape_setOrigin(hook_FishReception, vector2f(0.f, 0.f));
	sfRectangleShape_setTexture(hook_FishReception, hook_spritesheet, sfTrue);
	hook_ReceptionEmotion = HOOK_RECEPTION_IDLE;

	hook_PowerRing = sfRectangleShape_create();
	sfRectangleShape_setSize(hook_PowerRing, vector2f(600.f, 600.f));
	sfRectangleShape_setOrigin(hook_PowerRing, vector2f(300.f, 300.f));
	sfRectangleShape_setTexture(hook_PowerRing, hook_spritesheet, sfTrue);

	Hook_SplashRect = sfRectangleShape_create();
	sfRectangleShape_setSize(Hook_SplashRect, vector2f(90.f, 40.f));
	sfRectangleShape_setOrigin(Hook_SplashRect, vector2f(45.f, 40.f));
	sfRectangleShape_setTexture(Hook_SplashRect, hook_spritesheet, sfTrue);

	Hook_Star = sfRectangleShape_create();
	sfRectangleShape_setSize(Hook_Star, vector2f(56.f, 58.f));
	sfRectangleShape_setOrigin(Hook_Star, vector2f(26.f, 24.f));
	sfRectangleShape_setTexture(Hook_Star, hook_spritesheet, sfTrue);

	hook_RandomCaptureZone();

	hook_PowerRingIsDisplayed = sfFalse;
	hook_IsFirstView = sfFalse;
	hook_FirstTimeDrop = sfFalse;
	hook_FirstTimeTrigger = sfTrue;

	CloudList = STD_LIST_CREATE(Hook_Cloud, 0);
	ShellList = STD_LIST_CREATE(Hook_Shell, 0);
	CoralList = STD_LIST_CREATE(Hook_Coral, 0);
	RockList = STD_LIST_CREATE(Hook_Rock, 0);
	SeaweedList = STD_LIST_CREATE(Hook_Seaweed, 0);
	SplashList = STD_LIST_CREATE(Hook_Splash, 0);

	hook_RandomSpawn(6, HOOK_CORAL);
	hook_RandomSpawn(8, HOOK_CLOUD);
	hook_RandomSpawn(5, HOOK_SEAWEED);

	hook_ShaderTimer = 0.f;
}

void hook_UpdateHook(Window* _window) {
	if (hook_SplashSound0)
		hook_SplashSound0Timer += getDeltaTime();
	if (hook_SplashSound1)
		hook_SplashSound1Timer += getDeltaTime();
	if (hook_SplashSound2)
		hook_SplashSound2Timer += getDeltaTime();
	if (hook_SplashSound3)
		hook_SplashSound3Timer += getDeltaTime();
	if (hook_SplashSound4)
		hook_SplashSound4Timer += getDeltaTime();
	if (hook_SplashSound5)
		hook_SplashSound5Timer += getDeltaTime();
	if (hook_SplashSound6)
		hook_SplashSound6Timer += getDeltaTime();
	if (hook_SplashSound7)
		hook_SplashSound7Timer += getDeltaTime();
	if (hook_SplashSound8)
		hook_SplashSound8Timer += getDeltaTime();
	if (hook_SplashSound9)
		hook_SplashSound9Timer += getDeltaTime();
	hook_ParticlesTimer += getDeltaTime();

	if (hook_SplashSound0Timer >= 2.f)
	{
		hook_SplashSound0 = sfFalse;
		hook_SplashSound0Timer = 0.f;
	}
	if (hook_SplashSound1Timer >= 2.f)
	{
		hook_SplashSound1 = sfFalse;
		hook_SplashSound1Timer = 0.f;
	}
	if (hook_SplashSound2Timer >= 2.f)
	{
		hook_SplashSound2 = sfFalse;
		hook_SplashSound2Timer = 0.f;
	}
	if (hook_SplashSound3Timer >= 2.f)
	{
		hook_SplashSound3 = sfFalse;
		hook_SplashSound3Timer = 0.f;
	}
	if (hook_SplashSound4Timer >= 2.f)
	{
		hook_SplashSound4 = sfFalse;
		hook_SplashSound4Timer = 0.f;
	}
	if (hook_SplashSound5Timer >= 2.f)
	{
		hook_SplashSound5 = sfFalse;
		hook_SplashSound5Timer = 0.f;
	}
	if (hook_SplashSound6Timer >= 2.f)
	{
		hook_SplashSound6 = sfFalse;
		hook_SplashSound6Timer = 0.f;
	}
	if (hook_SplashSound7Timer >= 2.f)
	{
		hook_SplashSound7 = sfFalse;
		hook_SplashSound7Timer = 0.f;
	}
	if (hook_SplashSound8Timer >= 2.f)
	{
		hook_SplashSound8 = sfFalse;
		hook_SplashSound8Timer = 0.f;
	}
	if (hook_SplashSound9Timer >= 2.f)
	{
		hook_SplashSound9 = sfFalse;
		hook_SplashSound9Timer = 0.f;
	}
	hook_canPlaySound = sfFalse;

	hook_ShaderTimer += getDeltaTime();

	if (fade_GetOpacity() > 250)
	{
		hook_hook.pos = vector2f(fish_captureZone.left + FISH_CAPTURE_ZONE_X_SIZE / 2, fish_captureZone.top + FISH_CAPTURE_ZONE_Y_SIZE / 2);
		sfRectangleShape_setPosition(hook_hookRing, hook_hook.pos);
	}
	if (fade_GetOpacity() == 0)
	{
		if (fish_isOutOfTheWater(sfRectangleShape_getGlobalBounds(hook_hookRing)) && Hook_onePassWave && !hook_hook.isReseting && !hook_hook.starting)
		{
			Hook_onePassWave = sfFalse;
			stopMusicIfPlaying("Music_Fishing_BulleSousLeau");
			hook_addSplash(hook_hook.pos);
		}
		else if (fish_isOutOfTheWater(sfRectangleShape_getGlobalBounds(hook_hookRing)) && Hook_onePassWave)
		{
			stopMusicIfPlaying("Music_Fishing_BulleSousLeau");
			Hook_onePassWave = sfFalse;
		}
		else if (!fish_isOutOfTheWater(sfRectangleShape_getGlobalBounds(hook_hookRing)) && !Hook_onePassWave)
		{
			Hook_onePassWave = sfTrue;
			hook_addSplash(hook_hook.pos);
			playMusicIfNotPlaying("Music_Fishing_BulleSousLeau");
		}

		if (!fish_isOutOfTheWater(sfRectangleShape_getGlobalBounds(hook_hookRing)))
		{
			if (hook_ParticlesTimer > 1.0f)
			{
				prt_CreateBubbleFishParticles(hook_hook.pos);
				hook_ParticlesTimer = 0.f;
			}
		}
		if (hook_IsFirstView) {
			float dt = getDeltaTime();

			if (hook_hook.isReseting) {

				hook_hook.pos = vector2f(fish_captureZone.left + FISH_CAPTURE_ZONE_X_SIZE / 2, fish_captureZone.top + FISH_CAPTURE_ZONE_Y_SIZE / 2);
				hook_hook.startPos = hook_hook.pos;
				hook_hook.resetPos = vector2f(hook_hook.pos.x, hook_hook.pos.y + 200.f);
				fish_StopPower();
				toogle_HaveToStart();
				if (Gamepad_isButtonPressed(0, A, _window->renderWindow)) {
					hook_hook.isReseting = sfFalse;
					hook_hook.starting = sfTrue;
				}
			}
			else if (hook_hook.starting) {
				hook_hook.resetTimer += dt;
				hook_hook.pos = vec2f_lerp_smooth(hook_hook.startPos, hook_hook.resetPos, hook_hook.resetTimer / HOOK_RESET_DURATION);
				fish_StopPower();
				if (hook_hook.resetTimer >= HOOK_RESET_DURATION && !hook_FirstTimeDrop) {
					hook_hook.resetTimer = 0.f;
					hook_hook.starting = sfFalse;
					fish_changeReadyToUsed();
				}

				if (hook_hook.resetTimer >= HOOK_RESET_DURATION) {
					hook_hook.resetTimer = 0.f;
					hook_hook.starting = sfFalse;
				}
			}
			else {
				if (hook_CanMove) {
					if (hook_FirstTimeDrop)
					{
						hook_FirstTimeDrop = sfTrue;
					}

					hook_hook.speed = hook_BaseSpeed * (360.f - hook_Angle) / 36.f;
					hook_hook.forward = Gamepad_getNormalizedStickPos(0, STICKL, _window->renderWindow);

					sfBool isHookXMoved = sfFalse;
					sfBool isHookYMoved = sfFalse;
					sfVector2f futurePos;
					if (hook_hook.forward.x > CONTROLLER_OFFSET) {
						futurePos.x = hook_hook.pos.x + hook_hook.speed * dt;
						if (futurePos.x < 1920.f - HOOK_X_ORIGIN) {
							hook_hook.velocity.x = hook_hook.speed;
							hook_hook.oldForward = hook_hook.forward;
							isHookXMoved = sfTrue;
						}
						else {
							hook_hook.velocity.x = 0.f;
						}
					}
					else if (hook_hook.forward.x < -CONTROLLER_OFFSET) {
						futurePos.x = hook_hook.pos.x - hook_hook.speed * dt;
						if (futurePos.x > HOOK_X_ORIGIN) {
							hook_hook.velocity.x = -hook_hook.speed;
							hook_hook.oldForward = hook_hook.forward;
							isHookXMoved = sfTrue;
						}
						else {
							hook_hook.velocity.x = 0.f;
						}
					}

					if (isHookXMoved) {
						if (hook_hook.forward.y > CONTROLLER_OFFSET) {
							futurePos.y = hook_hook.pos.y + hook_hook.speed * dt;
							if (futurePos.y < hook_GetMaxYPos() - HOOK_Y_ORIGIN) {
								hook_hook.velocity.y = hook_hook.speed;
							}
							else
							{
								hook_hook.velocity.y = 0.f;
							}
						}
						else if (hook_hook.forward.y < -CONTROLLER_OFFSET) {
							futurePos.y = hook_hook.pos.y - hook_hook.speed * dt;
							if (futurePos.y > HOOK_Y_ORIGIN) {
								hook_hook.velocity.y = -hook_hook.speed;
							}
							else
							{
								hook_hook.velocity.y = 0.f;
							}
						}
						else
						{
							hook_hook.velocity.y = 0.f;
						}
					}
					else if (hook_hook.forward.y > CONTROLLER_OFFSET) {
						futurePos.y = hook_hook.pos.y + hook_hook.speed * dt;
						if (futurePos.y < hook_GetMaxYPos() - HOOK_Y_ORIGIN) {
							hook_hook.velocity.y = hook_hook.speed;
							hook_hook.oldForward = hook_hook.forward;
							isHookYMoved = sfTrue;
						}
						else {
							hook_hook.velocity.y = 0.f;
						}
					}
					else if (hook_hook.forward.y < -CONTROLLER_OFFSET) {
						futurePos.y = hook_hook.pos.y - hook_hook.speed * dt;
						if (futurePos.y > HOOK_Y_ORIGIN) {
							hook_hook.velocity.y = -hook_hook.speed;
							hook_hook.oldForward = hook_hook.forward;
							isHookYMoved = sfTrue;
						}
						else {
							hook_hook.velocity.y = 0.f;
						}
					}
					else {
						hook_hook.velocity.y = 0.f;
					}

					if (!isHookXMoved && !isHookYMoved) {
						hook_hook.velocity = vector2f(0.f, 0.f);
					}

					hook_hook.velocity = multiplyVectorsrf(normalizef(hook_hook.velocity), hook_hook.speed);

					addVectorsf(&hook_hook.pos, multiplyVectorsrf(hook_hook.velocity, dt));

					if (Gamepad_getTriggerPos(0, sfTrue, _window->renderWindow) > TRIGGER_OFFSET && hook_Angle < 330.f) {
						sfRectangleShape_setRotation(hook_ReelHand, hook_Angle + (Gamepad_getTriggerPos(0, sfTrue, _window->renderWindow) / 10.f));
						hook_Angle = sfRectangleShape_getRotation(hook_ReelHand);
						hook_canPlaySound = sfTrue;
					}
					else if (Gamepad_getTriggerPos(0, sfFalse, _window->renderWindow) > TRIGGER_OFFSET && hook_Angle > 30.f) {
						sfRectangleShape_setRotation(hook_ReelHand, hook_Angle - (Gamepad_getTriggerPos(0, sfFalse, _window->renderWindow) / 10.f));
						hook_Angle = sfRectangleShape_getRotation(hook_ReelHand);
						hook_canPlaySound = sfTrue;
					}
				}
				else
				{
					if (Gamepad_getTriggerPos(0, sfTrue, _window->renderWindow) > TRIGGER_OFFSET && hook_Angle < 330.f && hook_FirstTimeTrigger) {
						sfRectangleShape_setRotation(hook_ReelHand, hook_Angle + (Gamepad_getTriggerPos(0, sfTrue, _window->renderWindow) / 10.f));
						hook_Angle = sfRectangleShape_getRotation(hook_ReelHand);
						fish_changeReadyToUsed();
						hook_FirstTimeTrigger = sfFalse;
						hook_canPlaySound = sfTrue;
						hook_FirstTimeDrop = sfTrue;
					}
					else if (Gamepad_getTriggerPos(0, sfFalse, _window->renderWindow) > TRIGGER_OFFSET && hook_Angle > 30.f && hook_FirstTimeTrigger) {
						sfRectangleShape_setRotation(hook_ReelHand, hook_Angle - (Gamepad_getTriggerPos(0, sfFalse, _window->renderWindow) / 10.f));
						hook_Angle = sfRectangleShape_getRotation(hook_ReelHand);
						fish_changeReadyToUsed();
						hook_FirstTimeTrigger = sfFalse;
						hook_canPlaySound = sfTrue;
						hook_FirstTimeDrop = sfTrue;
					}
				}
			}
		}
		else
		{
			hook_hook.pos = vector2f(fish_captureZone.left + FISH_CAPTURE_ZONE_X_SIZE / 2, fish_captureZone.top + FISH_CAPTURE_ZONE_Y_SIZE / 2);
			sfRectangleShape_setPosition(hook_hookRing, hook_hook.pos);

			if (Gamepad_isButtonPressed(0, A, _window->renderWindow)) {
				hook_IsFirstView = sfTrue;
			}
		}
	}

	if (hook_canPlaySound)
	{
		hook_canPlaySound = sfFalse;
		playMusicIfNotPlaying("Music_Fishing_CrickCaneAPeche");
	}
	else
	{
		stopMusicIfPlaying("Music_Fishing_CrickCaneAPeche");
	}
}

void hook_DisplayHook(Window* _window) {
	sfShader_setFloatUniform(GetShader("shd_fishing"), "u_Time", hook_ShaderTimer);
	sfShader_setFloatUniform(GetShader("shd_fishing"), "u_Level", 0.143);
	sfShader_setIntUniform(GetShader("shd_fishing"), "u_DistLevel", 75.);

	sfSprite_setTexture(hook_sprite, GetSfTexture("fishing_bg"), sfTrue);
	sfRenderTexture_drawSprite(_window->rdrtShader, hook_sprite, NULL);

	hook_DisplayHookElements(_window);

	sfRectangleShape_setPosition(hook_hookRing, hook_hook.pos);
	sfRectangleShape_setTextureRect(hook_hookRing, animatorAnim("Fishing_RingMovement", &Hook_FrameXRing, &Hook_timerRing, 0));

	sfRectangleShape_setPosition(hook_FishReception, vector2f(fish_captureZone.left, fish_captureZone.top));
	switch (hook_ReceptionEmotion)
	{
	case HOOK_RECEPTION_HAPPY:
		sfRectangleShape_setTextureRect(hook_FishReception, animatorWithBeginFrame("Fishing_PhaosWin", &hook_FishReceptionFrameX, &hook_FishReceptionTimer, 0, 8));
		sfRectangleShape_setTextureRect(hook_hookRing, animatorAnim("Fishing_RingCapture", &Hook_FrameXRing, &Hook_timerRing, 0));
		sfRectangleShape_setPosition(Hook_Star, hook_hook.pos);
		sfRectangleShape_setTextureRect(Hook_Star, animatorWithBeginFrame("Fishing_Stars", &Hook_StarFrameX, &Hook_StarTimer, 0, 18));
		sfRectangleShape_setSize(hook_FishReception, vector2f(fish_captureZone.width, fish_captureZone.height));
		sfRenderTexture_drawRectangleShape(_window->rdrtShader, hook_FishReception, NULL);
		sfRenderTexture_drawRectangleShape(_window->rdrtShader, Hook_Star, NULL);
		break;
	case HOOK_RECEPTION_SAD:
		sfRectangleShape_setTextureRect(hook_FishReception, animatorAnim("Fishing_PhaosLose", &hook_FishReceptionFrameX, &hook_FishReceptionTimer, 0));
		hook_ReceptionTimerSad += getDeltaTime();
		if (hook_ReceptionTimerSad >= 1.f) {
			hook_ReceptionEmotion = HOOK_RECEPTION_IDLE;
			hook_ReceptionTimerSad = 0.f;
			hook_FishReceptionTimer = 0.f;
			hook_FishReceptionFrameX = 0;
		}
		sfRectangleShape_setSize(hook_FishReception, vector2f(fish_captureZone.width, fish_captureZone.height));
		sfRenderTexture_drawRectangleShape(_window->rdrtShader, hook_FishReception, NULL);
		break;
	case HOOK_RECEPTION_IDLE:
		sfRectangleShape_setTextureRect(hook_FishReception, IntRect(0, 1690, 130, 114));
		sfRectangleShape_setSize(hook_FishReception, vector2f(fish_captureZone.width, fish_captureZone.height));
		sfRenderTexture_drawRectangleShape(_window->rdrtShader, hook_FishReception, NULL);
		break;
	}

	if (hook_PowerRingIsDisplayed && isAttractHere) {
		sfRectangleShape_setPosition(hook_PowerRing, hook_hook.pos);
		sfRectangleShape_setTextureRect(hook_PowerRing, (sfIntRect) { 0, 2550, 600, 600 });
		sfRenderTexture_drawRectangleShape(_window->rdrtShader, hook_PowerRing, NULL);

		sfRectangleShape_setTextureRect(hook_PowerRing, animatorWithBeginFrame("Fishing_PowerZone", &hook_PowerRingFrameX, &hook_PowerRingTimer, 0, 7));
		sfRenderTexture_drawRectangleShape(_window->rdrtShader, hook_PowerRing, NULL);

		hook_PowerRingIsDisplayed = sfFalse;
	}
	if (hook_BubbleIsDisplayed) {
		hook_CanMove = sfFalse;
		sfRectangleShape_setPosition(hook_Bubble, hook_BubblePos);
		sfRectangleShape_setTextureRect(hook_Bubble, animatorWithBeginFrame("Fishing_Death", &hook_BubbleFrameX, &hook_BubbleTimer, 0, 7));
		sfRenderTexture_drawRectangleShape(_window->rdrtShader, hook_Bubble, NULL);

		sfRectangleShape_setPosition(hook_Bubble, vector2f(fish_captureZone.left + FISH_CAPTURE_ZONE_X_SIZE / 2, fish_captureZone.top + FISH_CAPTURE_ZONE_Y_SIZE / 2));
		sfRectangleShape_setTextureRect(hook_Bubble, animatorReverse("Fishing_Death", &hook_BubbleFrameX2, &hook_BubbleTimer2, 0, 0));
		sfRenderTexture_drawRectangleShape(_window->rdrtShader, hook_Bubble, NULL);
		hook_BoolTimer += getDeltaTime();
		if (hook_BoolTimer >= 1.f) {
			hook_BubbleIsDisplayed = sfFalse;
			hook_CanMove = sfTrue;
		}
	}
}

sfVector2f hook_GetHookPos() {
	return hook_hook.pos;
}

sfBool hook_CanBeCaptured(sfVector2f _fishPos)
{
	return sfFloatRect_contains(&fish_captureZone, _fishPos.x, _fishPos.y);
}

void hook_ResetHook()
{
	hook_hook.isReseting = sfTrue;
	hook_hook.startPos = hook_hook.pos;
	hook_SetBubblePos(hook_hook.pos);
	hook_hook.resetTimer = 0.f;
}

sfBool hook_IsReseting()
{
	return hook_hook.isReseting;
}

sfBool hook_IsFishEatingHook(sfFloatRect _fishBounds)
{
	sfFloatRect hookBounds = FlRect(hook_hook.pos.x - HOOK_X_ORIGIN, hook_hook.pos.y - HOOK_Y_ORIGIN, HOOK_X_SIZE, HOOK_Y_SIZE);
	return sfFloatRect_intersects(&hookBounds, &_fishBounds, NULL);
}

void hook_RandomCaptureZone()
{
	fish_captureZone.left = rand_float(0.f, 1920.f - fish_captureZone.width);
}

void hook_TutoCaptureZone()
{
	fish_captureZone.left = 1100.f;
}

float hook_ReturnCaptureZoneX()
{
	return fish_captureZone.left;
}

float hook_GetMaxYPos()
{
	if (fish_GetFishType() == FISH_SCARED)
		return 940.f;

	return 1080.f;
}

void hook_DeinitHook()
{
	sfRectangleShape_destroy(hook_waterRectangle);
	sfRectangleShape_destroy(hook_hookRing);
	sfRectangleShape_destroy(hook_FishReception);
	sfRectangleShape_destroy(hook_Reel);
	sfRectangleShape_destroy(hook_ReelHand);
}

void hook_ToggleMovement()
{
	hook_CanMove = !hook_CanMove;
}

void hook_setMovement(sfBool _canMove)
{
	hook_CanMove = _canMove;
}

float hook_RandomXPos()
{
	return rand_float(20.f, 1920.f);
}

void hook_AddSeaweed(int _id)
{
	Hook_Seaweed tmp;

	tmp.id = _id;
	int rand = iRand(0, 2);
	tmp.isGrouped = rand == 1 ? sfTrue : sfFalse;
	tmp.pos.x = hook_RandomXPos();
	tmp.pos.y = 376.f;
	tmp.frameSeaweed = 0;
	tmp.timerSeaweed = 0.f;

	SeaweedList->push_back(&SeaweedList, &tmp);
}

void hook_AddSheel(int _id)
{
	Hook_Coral tmp;

	tmp.id = _id;
	tmp.pos.x = hook_RandomXPos();
	tmp.pos.y = 1020.f;

	CoralList->push_back(&ShellList, &tmp);
}

void hook_AddCloud(int _id)
{
	Hook_Cloud tmp;

	tmp.id = _id;
	tmp.pos.x = hook_RandomXPos();
	tmp.pos.y = 0.f;

	CloudList->push_back(&CloudList, &tmp);
}

void hook_AddCoral(int _id)
{
	Hook_Coral tmp;

	tmp.id = _id;
	tmp.pos.x = hook_RandomXPos();
	tmp.pos.y = 780.f;

	CoralList->push_back(&CoralList, &tmp);
}

void hook_AddRock(int _id)
{
	Hook_Rock tmp;

	tmp.id = _id;
	tmp.pos.x = hook_RandomXPos();
	tmp.pos.y = 1020.f;

	RockList->push_back(&RockList, &tmp);
}

void hook_DisplayHookElements(Window* _window)
{
	sfRectangleShape_setOrigin(hook_ElementsOnScreen, vector2f(0.f, 0.f));


	for (int i = 0; i < CloudList->size(CloudList); i++)
	{
		sfRectangleShape_setPosition(hook_ElementsOnScreen, GD_CLOUD->pos);
		sfRectangleShape_setOrigin(hook_ElementsOnScreen, vector2f(0.f, 0.f));
		if (GD_CLOUD->id == 0)
		{
			sfRectangleShape_setSize(hook_ElementsOnScreen, vector2f(572, 110));
			sfRectangleShape_setTextureRect(hook_ElementsOnScreen, IntRect(0, 1480, 572, 110));
		}
		else if (GD_CLOUD->id == 1)
		{
			sfRectangleShape_setSize(hook_ElementsOnScreen, vector2f(572, 110));
			sfRectangleShape_setTextureRect(hook_ElementsOnScreen, IntRect(572, 1480, 572, 110));
		}
		else if (GD_CLOUD->id == 2)
		{
			sfRectangleShape_setSize(hook_ElementsOnScreen, vector2f(572, 110));
			sfRectangleShape_setTextureRect(hook_ElementsOnScreen, IntRect(572 * 2, 1480, 572, 110));
		}
		else if (GD_CLOUD->id == 3)
		{
			sfRectangleShape_setSize(hook_ElementsOnScreen, vector2f(572, 110));
			sfRectangleShape_setTextureRect(hook_ElementsOnScreen, IntRect(572 * 3, 1480, 572, 110));
		}
		sfRenderTexture_drawRectangleShape(_window->rdrtShader, hook_ElementsOnScreen, NULL);
	}

	for (int i = 0; i < ShellList->size(ShellList); i++)
	{
		sfRectangleShape_setPosition(hook_ElementsOnScreen, GD_SHELL->pos);
		if (GD_SHELL->id == 0)
		{
			sfRectangleShape_setSize(hook_ElementsOnScreen, vector2f(70.f, 36.f));
			sfRectangleShape_setTextureRect(hook_ElementsOnScreen, IntRect(0, 0, 70, 36));
		}
		else if (GD_SHELL->id == 1)
		{
			sfRectangleShape_setSize(hook_ElementsOnScreen, vector2f(52.f, 42.f));
			sfRectangleShape_setTextureRect(hook_ElementsOnScreen, IntRect(0, 36, 52, 42));
		}
		else if (GD_SHELL->id == 2)
		{
			sfRectangleShape_setSize(hook_ElementsOnScreen, vector2f(48.f, 44.f));
			sfRectangleShape_setTextureRect(hook_ElementsOnScreen, IntRect(0, 78, 48, 44));
		}
		else if (GD_SHELL->id == 3)
		{
			sfRectangleShape_setSize(hook_ElementsOnScreen, vector2f(56.f, 50.f));
			sfRectangleShape_setTextureRect(hook_ElementsOnScreen, IntRect(0, 122, 56, 50));
		}
		sfRenderTexture_drawRectangleShape(_window->rdrtShader, hook_ElementsOnScreen, NULL);
	}

	for (int i = 0; i < RockList->size(RockList); i++)
	{
		sfRectangleShape_setPosition(hook_ElementsOnScreen, GD_ROCK->pos);
		if (GD_ROCK->id == 0)
		{
			sfRectangleShape_setSize(hook_ElementsOnScreen, vector2f(62.f, 30.f));
			sfRectangleShape_setTextureRect(hook_ElementsOnScreen, IntRect(0, 172, 62, 30));
		}
		else if (GD_ROCK->id == 1)
		{
			sfRectangleShape_setSize(hook_ElementsOnScreen, vector2f(120.f, 40.f));
			sfRectangleShape_setTextureRect(hook_ElementsOnScreen, IntRect(0, 202, 120, 40));
		}
		else if (GD_ROCK->id == 2)
		{
			sfRectangleShape_setSize(hook_ElementsOnScreen, vector2f(38.f, 22.f));
			sfRectangleShape_setTextureRect(hook_ElementsOnScreen, IntRect(0, 242, 38, 22));
		}
		else if (GD_ROCK->id == 3)
		{
			sfRectangleShape_setSize(hook_ElementsOnScreen, vector2f(32.f, 18.f));
			sfRectangleShape_setTextureRect(hook_ElementsOnScreen, IntRect(0, 264, 33, 18));
		}
		sfRenderTexture_drawRectangleShape(_window->rdrtShader, hook_ElementsOnScreen, NULL);
	}

	for (int i = 0; i < CoralList->size(CoralList); i++)
	{
		sfRectangleShape_setPosition(hook_ElementsOnScreen, GD_CORAL->pos);
		if (GD_CORAL->id == 0)
		{
			sfRectangleShape_setSize(hook_ElementsOnScreen, vector2f(152.f, 300.f));
			sfRectangleShape_setTextureRect(hook_ElementsOnScreen, IntRect(0, 476, 152, 300));
		}
		else if (GD_CORAL->id == 1)
		{
			sfRectangleShape_setSize(hook_ElementsOnScreen, vector2f(160.f, 300.f));
			sfRectangleShape_setTextureRect(hook_ElementsOnScreen, IntRect(152, 476, 160, 300));
		}
		else if (GD_CORAL->id == 2)
		{
			sfRectangleShape_setSize(hook_ElementsOnScreen, vector2f(122.f, 300.f));
			sfRectangleShape_setTextureRect(hook_ElementsOnScreen, IntRect(312, 476, 122, 300));
		}
		else if (GD_CORAL->id == 3)
		{
			sfRectangleShape_setSize(hook_ElementsOnScreen, vector2f(126.f, 300.f));
			sfRectangleShape_setTextureRect(hook_ElementsOnScreen, IntRect(590, 476, 212, 300));
		}
		else if (GD_CORAL->id == 4)
		{
			sfRectangleShape_setSize(hook_ElementsOnScreen, vector2f(162.f, 300.f));
			sfRectangleShape_setTextureRect(hook_ElementsOnScreen, IntRect(802, 476, 162, 300));
		}
		sfRenderTexture_drawRectangleShape(_window->rdrtShader, hook_ElementsOnScreen, NULL);
	}
}


void hook_RandomSpawn(int _nbelemets, Hook_enumType _type)
{
	if (_type == HOOK_SHELL)
	{
		int elementone = 1, elementtwo = 1, elementthree = 1, elementfour = 1;

		for (int i = 0; i < _nbelemets; i++)
		{
			while (1)
			{
				int random = iRand(0, 4);

				if (random == 0 && elementone > 0)
				{
					if (_type == HOOK_SHELL)
					{
						hook_AddSheel(0);
					}
					elementone--;
					break;
				}
				else if (random == 1 && elementtwo > 0)
				{
					if (_type == HOOK_SHELL)
					{
						hook_AddSheel(1);
					}
					elementtwo--;
					break;
				}
				else if (random == 2 && elementthree > 0)
				{
					if (_type == HOOK_SHELL)
					{
						hook_AddSheel(2);
					}
					elementthree--;
					break;
				}
				else if (random == 3 && elementfour > 0)
				{
					if (_type == HOOK_SHELL)
					{
						hook_AddSheel(3);
					}
					elementfour--;
					break;
				}

				if (elementone == 0 && elementtwo == 0 && elementthree == 0 && elementfour == 0)
				{
					elementone = 1;
					elementtwo = elementone;
					elementthree = elementone;
					elementfour = elementone;
					i--;
					break;
				}
			}
		}
	}
	else if (_type == HOOK_CORAL)
	{
		int elementone = 1, elementtwo = 1, elementthree = 1, elementfour = 1, elementfive = 1;

		for (int i = 0; i < _nbelemets; i++)
		{
			while (1)
			{
				int random = iRand(0, 5);

				if (random == 0 && elementone > 0)
				{
					if (_type == HOOK_CORAL)
					{
						hook_AddCoral(0);
					}
					elementone--;
					break;
				}
				else if (random == 1 && elementtwo > 0)
				{
					if (_type == HOOK_CORAL)
					{
						hook_AddCoral(1);
					}
					elementtwo--;
					break;
				}
				else if (random == 2 && elementthree > 0)
				{
					if (_type == HOOK_CORAL)
					{
						hook_AddCoral(2);
					}
					elementthree--;
					break;
				}
				else if (random == 3 && elementfour > 0)
				{
					if (_type == HOOK_CORAL)
					{
						hook_AddCoral(3);
					}
					elementfour--;
					break;
				}
				else if (random == 4 && elementfive > 0)
				{
					if (_type == HOOK_CORAL)
					{
						hook_AddCoral(4);
					}
					elementfive--;
					break;
				}

				if (elementone == 0 && elementtwo == 0 && elementthree == 0 && elementfour == 0 && elementfive == 0)
				{
					elementone = 1;
					elementtwo = elementone;
					elementthree = elementone;
					elementfour = elementone;
					elementfive = elementone;
					i--;
					break;
				}
			}
		}
	}
	else if (_type == HOOK_CLOUD || _type == HOOK_ROCK)
	{
		int elementone = 1, elementtwo = 1, elementthree = 1, elementfour = 1;

		for (int i = 0; i < _nbelemets; i++)
		{
			while (1)
			{
				int random = iRand(0, 4);

				if (random == 0 && elementone > 0)
				{
					if (_type == HOOK_CLOUD)
						hook_AddCloud(0);
					else
						hook_AddRock(0);
					elementone--;
					break;
				}
				else if (random == 1 && elementtwo > 0)
				{
					if (_type == HOOK_CLOUD)
						hook_AddCloud(1);
					else
						hook_AddRock(1);
					elementtwo--;
					break;
				}
				else if (random == 2 && elementthree > 0)
				{
					if (_type == HOOK_CLOUD)
						hook_AddCloud(2);
					else
						hook_AddRock(2);
					elementthree--;
					break;
				}
				else if (random == 3 && elementfour > 0)
				{
					if (_type == HOOK_CLOUD)
						hook_AddCloud(3);
					else
						hook_AddRock(3);
					elementfour--;
					break;
				}
			}

			if (elementone == 0 && elementtwo == 0 && elementthree == 0 && elementfour == 0)
			{
				elementone = 1;
				elementtwo = elementone;
				elementthree = elementone;
				elementfour = elementone;
				i--;
				break;
			}
		}
	}
	else if (_type == HOOK_SEAWEED)
	{
		int elementone = 1, elementtwo = 1, elementthree = 1, elementfour = 1, elementfive = 1;

		for (int i = 0; i < _nbelemets; i++)
		{
			while (1)
			{
				int random = iRand(0, 5);

				if (random == 0 && elementone > 0)
				{
					hook_AddSeaweed(0);
					elementone--;
					break;
				}
				else if (random == 1 && elementtwo > 0)
				{
					hook_AddSeaweed(1);
					elementtwo--;
					break;
				}
				else if (random == 2 && elementthree > 0)
				{
					hook_AddSeaweed(2);
					elementthree--;
					break;
				}
				else if (random == 3 && elementfour > 0)
				{
					hook_AddSeaweed(3);
					elementfour--;
					break;
				}
				else if (random == 4 && elementfive > 0)
				{
					hook_AddSeaweed(4);
					elementfive--;
					break;
				}
			}

			if (elementone == 0 && elementtwo == 0 && elementthree == 0 && elementfour == 0 && elementfive == 0)
			{
				elementone = 1;
				elementtwo = elementone;
				elementthree = elementone;
				elementfour = elementone;
				elementfive = elementone;
				i--;
				break;
			}
		}
	}

}

void hook_DisplaySeaweed(Window* _window)
{
	if (!hook_BubbleIsDisplayed)
		sfRenderTexture_drawRectangleShape(_window->rdrtShader, hook_hookRing, NULL);

	for (int i = 0; i < SeaweedList->size(SeaweedList); i++)
	{
		if (!GD_SEAWEED->isGrouped)
		{
			sfRectangleShape_setPosition(hook_ElementsOnScreen, GD_SEAWEED->pos);

			if (GD_SEAWEED->id == 0)
			{
				sfRectangleShape_setSize(hook_ElementsOnScreen, vector2f(202.f, 704.f));
				sfRectangleShape_setTextureRect(hook_ElementsOnScreen, IntRect(0, 776, 202, 704));
			}
			else if (GD_SEAWEED->id == 1)
			{
				sfRectangleShape_setSize(hook_ElementsOnScreen, vector2f(202.f, 704.f));
				sfRectangleShape_setTextureRect(hook_ElementsOnScreen, IntRect(606, 776, 202, 704));
			}
			else if (GD_SEAWEED->id == 2)
			{
				sfRectangleShape_setSize(hook_ElementsOnScreen, vector2f(202.f, 704.f));
				sfRectangleShape_setTextureRect(hook_ElementsOnScreen, IntRect(808, 776, 202, 704));
			}
			else if (GD_SEAWEED->id == 3)
			{
				sfRectangleShape_setSize(hook_ElementsOnScreen, vector2f(202.f, 704.f));
				sfRectangleShape_setTextureRect(hook_ElementsOnScreen, IntRect(1010, 776, 202, 704));
			}
			else if (GD_SEAWEED->id == 4)
			{
				sfRectangleShape_setSize(hook_ElementsOnScreen, vector2f(202.f, 704.f));
				sfRectangleShape_setTextureRect(hook_ElementsOnScreen, IntRect(1212, 776, 202, 704));
			}
			sfRenderTexture_drawRectangleShape(_window->rdrtShader, hook_ElementsOnScreen, NULL);
		}
		else
		{
			if (GD_SEAWEED->id == 0)
			{
				sfRectangleShape_setPosition(hook_ElementsOnScreen, GD_SEAWEED->pos);
				sfRectangleShape_setSize(hook_ElementsOnScreen, vector2f(202.f, 704.f));
				sfRectangleShape_setPosition(hook_ElementsOnScreen, vector2f(GD_SEAWEED->pos.x, GD_SEAWEED->pos.y));
				sfRectangleShape_setTextureRect(hook_ElementsOnScreen, IntRect(202, 776, 202, 704));
				sfRenderTexture_drawRectangleShape(_window->rdrtShader, hook_ElementsOnScreen, NULL);

				sfRectangleShape_setPosition(hook_ElementsOnScreen, vector2f(GD_SEAWEED->pos.x + 205.f, GD_SEAWEED->pos.y));
				sfRectangleShape_setSize(hook_ElementsOnScreen, vector2f(202.f, 704.f));
				sfRectangleShape_setTextureRect(hook_ElementsOnScreen, IntRect(606, 776, 202, 704));
				sfRenderTexture_drawRectangleShape(_window->rdrtShader, hook_ElementsOnScreen, NULL);
			}
			else if (GD_SEAWEED->id == 1)
			{
				sfRectangleShape_setPosition(hook_ElementsOnScreen, GD_SEAWEED->pos);
				sfRectangleShape_setSize(hook_ElementsOnScreen, vector2f(202.f, 704.f));
				sfRectangleShape_setTextureRect(hook_ElementsOnScreen, IntRect(808, 776, 202, 704));
				sfRenderTexture_drawRectangleShape(_window->rdrtShader, hook_ElementsOnScreen, NULL);

				sfRectangleShape_setPosition(hook_ElementsOnScreen, vector2f(GD_SEAWEED->pos.x - 205.f, GD_SEAWEED->pos.y));
				sfRectangleShape_setSize(hook_ElementsOnScreen, vector2f(202.f, 704.f));
				sfRectangleShape_setTextureRect(hook_ElementsOnScreen, IntRect(606, 776, 202, 704));
				sfRenderTexture_drawRectangleShape(_window->rdrtShader, hook_ElementsOnScreen, NULL);

				sfRectangleShape_setPosition(hook_ElementsOnScreen, vector2f(GD_SEAWEED->pos.x + 105.f, GD_SEAWEED->pos.y));

				sfRectangleShape_setSize(hook_ElementsOnScreen, vector2f(202.f, 704.f));
				sfRectangleShape_setTextureRect(hook_ElementsOnScreen, IntRect(1010, 776, 202, 704));
				sfRenderTexture_drawRectangleShape(_window->rdrtShader, hook_ElementsOnScreen, NULL);
			}
			else if (GD_SEAWEED->id == 2)
			{
				sfRectangleShape_setPosition(hook_ElementsOnScreen, GD_SEAWEED->pos);
				sfRectangleShape_setSize(hook_ElementsOnScreen, vector2f(202.f, 704.f));
				sfRectangleShape_setTextureRect(hook_ElementsOnScreen, IntRect(808, 776, 202, 704));
				sfRenderTexture_drawRectangleShape(_window->rdrtShader, hook_ElementsOnScreen, NULL);

				sfRectangleShape_setPosition(hook_ElementsOnScreen, vector2f(GD_SEAWEED->pos.x + 205.f, GD_SEAWEED->pos.y));
				sfRectangleShape_setSize(hook_ElementsOnScreen, vector2f(202.f, 704.f));
				sfRectangleShape_setTextureRect(hook_ElementsOnScreen, IntRect(404, 776, 202, 704));
				sfRenderTexture_drawRectangleShape(_window->rdrtShader, hook_ElementsOnScreen, NULL);
			}
			else if (GD_SEAWEED->id == 3)
			{
				sfRectangleShape_setPosition(hook_ElementsOnScreen, GD_SEAWEED->pos);
				sfRectangleShape_setSize(hook_ElementsOnScreen, vector2f(202.f, 704.f));
				sfRectangleShape_setTextureRect(hook_ElementsOnScreen, IntRect(808, 776, 202, 704));
				sfRenderTexture_drawRectangleShape(_window->rdrtShader, hook_ElementsOnScreen, NULL);

				sfRectangleShape_setPosition(hook_ElementsOnScreen, vector2f(GD_SEAWEED->pos.x + 205.f, GD_SEAWEED->pos.y));
				sfRectangleShape_setSize(hook_ElementsOnScreen, vector2f(202.f, 704.f));
				sfRectangleShape_setTextureRect(hook_ElementsOnScreen, IntRect(1212, 776, 202, 704));
				sfRenderTexture_drawRectangleShape(_window->rdrtShader, hook_ElementsOnScreen, NULL);
			}
			else if (GD_SEAWEED->id == 4)
			{
				sfRectangleShape_setPosition(hook_ElementsOnScreen, GD_SEAWEED->pos);
				sfRectangleShape_setSize(hook_ElementsOnScreen, vector2f(202.f, 704.f));
				sfRectangleShape_setTextureRect(hook_ElementsOnScreen, IntRect(1414, 776, 202, 704));
				sfRenderTexture_drawRectangleShape(_window->rdrtShader, hook_ElementsOnScreen, NULL);

				sfRectangleShape_setPosition(hook_ElementsOnScreen, vector2f(GD_SEAWEED->pos.x + 205.f, GD_SEAWEED->pos.y));
				sfRectangleShape_setSize(hook_ElementsOnScreen, vector2f(202.f, 704.f));
				sfRectangleShape_setTextureRect(hook_ElementsOnScreen, IntRect(0, 776, 202, 704));
				sfRenderTexture_drawRectangleShape(_window->rdrtShader, hook_ElementsOnScreen, NULL);
			}
		}
	}

	for (int i = 0; i < SplashList->size(SplashList); i++)
	{
		if (GD_SPLASH->frameSplash == 12)
		{
			SplashList->erase(&SplashList, i);
			continue;
		}

		sfRectangleShape_setPosition(Hook_SplashRect, GD_SPLASH->pos);
		sfRectangleShape_setTextureRect(Hook_SplashRect, animatorAnim("Fishing_Splash", &GD_SPLASH->frameSplash, &GD_SPLASH->timerSplash, 0));
		sfRenderTexture_drawRectangleShape(_window->rdrt, Hook_SplashRect, NULL);

	}

	/*
	//shader pour l'eau

	sfShader_setTextureUniform(hook_waterShader, "texture", GetSfTexture("fishing_bg"));
	sfShader_setTextureUniform(hook_waterShader, "normalMap", GetSfTexture("fishing_bg_normal"));
	sfShader_setTextureUniform(hook_waterShader, "reflectionMap", GetSfTexture("fishing_bg_reflection"));
	sfShader_setTextureUniform(hook_waterShader, "refractionMap", GetSfTexture("fishing_bg_refraction"));
	sfShader_setTextureUniform(hook_waterShader, "dudvMap", GetSfTexture("fishing_bg_dudv"));
	*/

	sfRenderTexture_drawRectangleShape(_window->rdrt, hook_ReelHand, NULL);

	sfRenderTexture_drawRectangleShape(_window->rdrt, hook_Reel, NULL);

}

void hook_SetPowerRing()
{
	hook_PowerRingIsDisplayed = sfTrue;
}

void hook_ResetPowerRing()
{
	hook_PowerRingFrameX = 0;
	hook_PowerRingTimer = 0.f;
}

void hook_SetBubblePos(sfVector2f _pos)
{
	playSoundFX("SFX_Fishing_MagicExplosion");
	hook_BubbleIsDisplayed = sfTrue;
	hook_BubblePos = _pos;
	hook_BoolTimer = 0.f;
	hook_BubbleFrameX = 0;
	hook_BubbleTimer = 0.f;
	hook_BubbleFrameX2 = 0;
	hook_BubbleTimer2 = 0.f;
}

void hook_ResetBubble()
{
	hook_BubbleFrameX = 0;
	hook_BubbleTimer = 0.f;
	hook_BubbleFrameX2 = 0;
	hook_BubbleTimer2 = 0.f;
}

void hook_addSplash(sfVector2f _pos)
{
	Hook_Splash tmp;
	tmp.pos = _pos;
	tmp.pos.y = 175.f;
	tmp.frameSplash = 0;
	tmp.timerSplash = 0.f;

	int tmpnumb = rand_int(0, 10);
	sfBool tmpBool = sfTrue;
	while (tmpBool)
	{
		switch (tmpnumb)
		{
		case 0:
			if (!hook_SplashSound0)
			{
				playSoundFX("SFX_Fishing_WaterSplash0");
				hook_SplashSound0 = sfTrue;
				tmpBool = sfFalse;
			}
			break;
		case 1:
			if (!hook_SplashSound1)
			{
				playSoundFX("SFX_Fishing_WaterSplash1");
				hook_SplashSound1 = sfTrue;
				tmpBool = sfFalse;
			}
			break;
		case 2:
			if (!hook_SplashSound2)
			{
				playSoundFX("SFX_Fishing_WaterSplash2");
				hook_SplashSound2 = sfTrue;
				tmpBool = sfFalse;
			}
			break;
		case 3:
			if (!hook_SplashSound3)
			{
				playSoundFX("SFX_Fishing_WaterSplash3");
				hook_SplashSound3 = sfTrue;
				tmpBool = sfFalse;
			}
			break;
		case 4:
			if (!hook_SplashSound4)
			{
				playSoundFX("SFX_Fishing_WaterSplash4");
				hook_SplashSound4 = sfTrue;
				tmpBool = sfFalse;
			}
			break;
		case 5:
			if (!hook_SplashSound5)
			{
				playSoundFX("SFX_Fishing_WaterSplash5");
				hook_SplashSound5 = sfTrue;
				tmpBool = sfFalse;
			}
			break;
		case 6:
			if (!hook_SplashSound6)
			{
				playSoundFX("SFX_Fishing_WaterSplash6");
				hook_SplashSound6 = sfTrue;
				tmpBool = sfFalse;
			}
			break;
		case 7:
			if (!hook_SplashSound7)
			{
				playSoundFX("SFX_Fishing_WaterSplash7");
				hook_SplashSound7 = sfTrue;
				tmpBool = sfFalse;
			}
			break;
		case 8:
			if (!hook_SplashSound8)
			{
				playSoundFX("SFX_Fishing_WaterSplash8");
				hook_SplashSound8 = sfTrue;
				tmpBool = sfFalse;
			}
			break;
		case 9:
			if (!hook_SplashSound9)
			{
				playSoundFX("SFX_Fishing_WaterSplash9");
				hook_SplashSound9 = sfTrue;
				tmpBool = sfFalse;
			}
			break;
		}
		tmpnumb++;
		if (tmpnumb > 9)
			tmpnumb = 0;
		if (hook_SplashSound0 && hook_SplashSound1 && hook_SplashSound2 && hook_SplashSound3 && hook_SplashSound4 && hook_SplashSound5 && hook_SplashSound6 && hook_SplashSound7 && hook_SplashSound8 && hook_SplashSound9)
			tmpBool = sfFalse;
	}




	SplashList->push_back(&SplashList, &tmp);
}

void Hook_ChangeReceptionEmotion(Hook_Emotion _emotion)
{
	hook_ReceptionEmotion = _emotion;
}

void AttractToggle(sfBool _attract)
{
	isAttractHere = _attract;
}

sfBool fish_GiveFirstTimeDrop()
{
	return hook_FirstTimeDrop;
}