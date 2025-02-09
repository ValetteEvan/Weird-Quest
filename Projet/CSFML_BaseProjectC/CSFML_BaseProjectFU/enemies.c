#include "enemies.h"
#include "math.h"
#include "player.h"
#include "map.h"
#include "game.h"
#include "collisionManager.h"
#include "tears.h"
#include "debuffTear.h"
#include "textureManager.h"
#include "bullets.h"
#include "ring.h"
#include "viewManager.h"
#include "dialogBox.h"
#include "particlesManager.h"
#include "quest.h"
#include "pause.h"
#include "soundManager.h"
#include "FXenemies.h"
#include "pause.h"
#include "game_HUD.h"
//playSoundFX
#define DETECTION_DIAMETER 50000.f
#define SPAWN_DIAMETER 360000.f
#define TIME_CANT_SEE_PLY 2.f

sfTexture* en_TextureShadow;
sfRectangleShape* en_Rectangle;

sfRectangleShape* en_rectangleShape;
sfRectangleShape* en_rectangleTree;
sfRectangleShape* en_rectangleSpider;
sfCircleShape* en_circleShape;
sfBool isTreeHit = sfFalse;
sfFloatRect rectTree;


float en_timer_test, en_timer_tree;
sfBool toRemoveToggleDebugRect = sfFalse;


sfTexture* en_Textures[EN_NB_ENEMIES_TYPES];
void (*en_UpdateEnemyType[EN_NB_ENEMIES_TYPES])(EnemiesStruct*, EnemiesState, int, float, sfVector2f, Window*) = { en_UpdateYoyoSpider, en_UpdateVenimousYoyoSpider, en_UpdateCrab, en_UpdateGoose, en_UpdateWhiteBillyGoat, en_UpdateBlackBillyGoat, en_UpdateSnake, en_UpdateMosquito, en_UpdateMole, en_UpdateScolopendre, en_UpdateChameleon, en_UpdateBat, en_UpdateLittleGhostJoy, en_UpdateBigGhostJoy, en_UpdateLittleGhostSadness, en_UpdateBigGhostSadness, en_UpdateImunity, en_UpdateSeagull };
void (*en_DisplayEnemyType[EN_NB_ENEMIES_TYPES])(sfRenderTexture*, EnemiesStruct*, sfVector2f) = { en_DisplayYoyoSpider, en_DisplayVenimousYoyoSpider, en_DisplayCrab, en_DisplayGoose, en_DisplayWhiteBillyGoat, en_DisplayBlackBillyGoat, en_DisplaySnake, en_DisplayMosquito, en_DisplayMole, en_DisplayScolopendre, en_DisplayChameleon, en_DisplayBat, en_DisplayLittleGhostJoy, en_DisplayBigGhostJoy, en_DisplayLittleGhostSadness, en_DisplayBigGhostSadness, en_DisplayImunity, en_DisplaySeagull };
void (*en_DisplayShadows[EN_NB_ENEMIES_TYPES])(sfRenderTexture*, EnemiesStruct*) = { en_DisplayYoyoSpiderShadow, en_DisplayVenimousYoyoSpiderShadow, en_DisplayCrabShadow, en_DisplayGooseShadow, en_DisplayWhiteBillyGoatShadow, en_DisplayBlackBillyGoatShadow, en_DisplaySnakeShadow, en_DisplayMosquitoShadow, en_DisplayMoleShadow, en_DisplayScolopendreShadow, en_DisplayChameleonShadow, en_DisplayBatShadow, en_DisplayLittleGhostJoyShadow, en_DisplayBigGhostJoyShadow, en_DisplayLittleGhostSadnessShadow, en_DisplayBigGhostSadnessShadow, en_DisplayImunityShadow, en_DisplaySeagullShadow };


void en_loadTexture()
{
	en_Textures[EN_YOYO_SPIDER] = GetSfTexture("spider_IDLE");
	en_Textures[EN_VENIMOUS_YOYO_SPIDER] = GetSfTexture("spider_BAD_IDLE");
	en_Textures[EN_WHITE_BILLY_GOAT] = GetSfTexture("WHITE_GOAT_IDLE");
	en_Textures[EN_BLACK_BILLY_GOAT] = GetSfTexture("BLACK_GOAT_IDLE");
	en_Textures[EN_MOLE] = GetSfTexture("moleFaceIdle");
	en_Textures[EN_SNAKE] = GetSfTexture("snakeIdle");
	en_Textures[EN_BIG_GHOST_JOY] = GetSfTexture("bigGhostJoyIdle");
	en_Textures[EN_BIG_GHOST_SADNESS] = GetSfTexture("bigSadIdle");
	en_Textures[EN_CHAMELEON] = GetSfTexture("chameleonIdle");
	en_Textures[EN_MOSQUITO] = GetSfTexture("mosquito_WALK");
	en_Textures[EN_CRAB] = GetSfTexture("crabAttract");
	en_Textures[EN_BAT] = GetSfTexture("bat_IDLE");
	en_Textures[EN_LITTLE_GHOST_JOY] = GetSfTexture("littleJoyWalk");
	en_Textures[EN_SCOLOPENDRE] = GetSfTexture("scolopendreUp");
	en_Textures[EN_LITTLE_GHOST_SADNESS] = GetSfTexture("SmallSadMove");
	en_Textures[EN_GOOSE] = GetSfTexture("Goose_Idle");
	en_Textures[EN_SEAGULL] = GetSfTexture("seagullIdle");
	en_TextureShadow = GetSfTexture("shadows");
}

void en_InitEnemies()
{
	en_timer_test = 0.f;
	en_circleShape = sfCircleShape_create();
	en_rectangleTree = sfRectangleShape_create();
	en_rectangleSpider = sfRectangleShape_create();
	en_Rectangle = sfRectangleShape_create();
	sfRectangleShape_setSize(en_rectangleTree, vector2f(100.0f, 200.0f));
	//	sfRectangleShape_setOrigin(en_rectangleTree, vector2f(100.0f, 100.0f));
	sfRectangleShape_setPosition(en_rectangleTree, vector2f(500.f, 500.f));
	sfRectangleShape_setFillColor(en_rectangleTree, sfGreen);
	rectTree = sfRectangleShape_getGlobalBounds(en_rectangleTree);

	sfRectangleShape_setSize(en_rectangleSpider, vector2f(20.0f, 20.0f));
	sfRectangleShape_setPosition(en_rectangleSpider, vector2f(650.f, 475.f));
	sfRectangleShape_setFillColor(en_rectangleSpider, sfWhite);
	en_rectangleShape = sfRectangleShape_create();
	sfRectangleShape_setSize(en_rectangleShape, vector2f(64.0f, 64.0f));
	enemiesList = STD_LIST_CREATE(EnemiesStruct, 0);

	en_loadTexture();
}

void en_SpawnEnemy(sfVector2f _pos, EnemiesType _type, float _rotation, sfBool _isFloorSpider)
{
	EnemiesStruct tmp = { 0 };

	tmp.pos = _pos;
	tmp.origin = vector2f(32.f, 32.f);
	tmp.spawnPos = _pos;
	tmp.startPos = _pos;
	tmp.endPos = _pos;
	tmp.forward = normalizef(vector2f(rand_float(-1.0f, 1.f), rand_float(-1.0f, 1.f)));
	tmp.targetForward = tmp.forward;
	tmp.scale = vector2f(1.f, 1.f);
	tmp.size = vector2f(64.0f, 64.0f);
	tmp.state = EN_STATE_WALK;
	tmp.time_wait = 0.0f;
	tmp.speed = 50.f;
	tmp.radiusDetection = 30000.0f;
	tmp.type = _type;
	tmp.color = sfYellow;
	tmp.time_detection = 0.0f;
	tmp.detectionAngle = 45.f;
	tmp.radiusDetectionSQRT = 200.0f;
	tmp.currentVelocity = vector2f(0.0f, 0.0f);
	tmp.maxVelocity = 200.f;
	tmp.stunTimer = 0.f;
	tmp.isFallingAnimDone = sfFalse;
	tmp.isOnString = sfTrue;
	tmp.prtCapture = sfTrue;
	tmp.colRect = FlRect(0.0f, 0.0f, 0.0f, 0.0f);
	tmp.timerAllAttacks = 0.0f;
	tmp.timerTears = 0.0f;
	tmp.frameX = 0;
	tmp.timerAnim = 0.f;
	tmp.mass = 10.f;
	tmp.radius = 32.f;
	tmp.isInHole = sfTrue;
	tmp.isOutHole = sfFalse;
	tmp.time_invincibility = 0.51f;
	tmp.spriteRect.left = 0;
	tmp.spriteRect.top = 0;
	tmp.spriteRect.width = 64;
	tmp.spriteRect.height = 64;
	tmp.rebootFrameX = sfFalse;
	tmp.rebootFrameX2 = sfFalse;
	tmp.isOnScreen = sfTrue;
	tmp.life = 1;
	tmp.hasBeenRushed = sfFalse;
	tmp.haveToHome = sfFalse;
	tmp.collisionsBounds = FlRect(0.f, 0.f, 64.f, 64.f);
	tmp.rotation = _rotation;
	tmp.isOnePassedPulse = sfFalse;
	tmp.countOfSeePlayer = 0;
	tmp.damage = 1;
	tmp.isHooking = sfFalse;
	tmp.rotationFalling = 0.0f;
	tmp.time_sound = 0.0f;


	switch (tmp.type)
	{
	case EN_YOYO_SPIDER:
		tmp.forward = normalizef(vector2f(rand_float(-1.0f, 1.f), 0));
		tmp.targetForward = tmp.forward;
		tmp.isOnString = sfTrue;
		tmp.state = EN_STATE_WAIT;
		tmp.color = sfWhite;
		tmp.spriteRect.left = 0;
		tmp.spriteRect.top = 0;
		tmp.spriteRect.width = 78;
		tmp.spriteRect.height = 56;
		tmp.speed = PLR_SPEED * 3;
		tmp.life = 1;
		tmp.maxVelocity = 50.0f;
		break;
	case EN_VENIMOUS_YOYO_SPIDER:
		tmp.isOnString = sfTrue;
		tmp.state = EN_STATE_WAIT;
		tmp.color = sfBlack;
		tmp.spriteRect.left = 0;
		tmp.spriteRect.top = 0;
		tmp.spriteRect.width = 78;
		tmp.spriteRect.height = 56;
		tmp.speed = PLR_SPEED * 3;
		tmp.life = 1;
		tmp.maxVelocity = 50.0f;
		if (_isFloorSpider)
		{
			tmp.isOnString = sfFalse;
			tmp.isFallingAnimDone = sfTrue;
			tmp.state = EN_STATE_REVEALING;
		}
		break;
	case EN_CRAB:
		tmp.maxVelocity = 50.0f;
		tmp.mass = 10.0f;
		tmp.life = 1;
		tmp.state = EN_STATE_WAIT;
		//	tmp.timeChased = 0.f;
		break;
	case EN_GOOSE:
		tmp.maxVelocity = 150.0f;
		tmp.life = 2;
		tmp.mass = 10.f;
		tmp.Patrolpoint[0] = tmp.spawnPos;
		tmp.Patrolpoint[1] = vector2f(tmp.spawnPos.x + 320.f, tmp.spawnPos.y);
		tmp.Patrolpoint[2] = vector2f(tmp.spawnPos.x + 320.f, tmp.spawnPos.y + 320.f);
		tmp.Patrolpoint[3] = vector2f(tmp.spawnPos.x, tmp.spawnPos.y + 320.f);
		tmp.radiusDetectionSQRT = 320.f;
		tmp.detectionAngle = 60.f;
		tmp.radiusDetection = 16384.f;
		tmp.CurrentPatrolPoint = 0;
		tmp.state = EN_STATE_PATROLING;
		tmp.collisionsBounds = FlRect(0.0f, 52.0f, 64.f, 42.f);
		break;
	case EN_WHITE_BILLY_GOAT:
		tmp.speed = 7.f;
		tmp.maxVelocity = 0.f;
		tmp.detectionAngle = 60.f;
		tmp.radiusDetectionSQRT = 250.0f;
		tmp.state = EN_STATE_WAIT;
		tmp.life = 1;
		tmp.collisionsBounds = FlRect(0.0f, 50.0f, 128.f, 64.f);
		tmp.origin = vector2f(64.f, 32.f);
		break;
	case EN_BLACK_BILLY_GOAT:
		tmp.speed = 10.f;
		tmp.maxVelocity = 0.f;
		tmp.detectionAngle = 75.f;
		tmp.radiusDetectionSQRT = 375.0f;
		tmp.state = EN_STATE_WAIT;
		tmp.life = 2;
		tmp.collisionsBounds = FlRect(0.0f, 50.0f, 128.f, 64.f);
		tmp.origin = vector2f(64.f, 32.f);
		break;
	case EN_SNAKE:
		tmp.state = EN_STATE_WAIT;
		tmp.size = vector2f(90.f, 76.f);
		tmp.origin = vector2f(45.f, 38.f);
		tmp.collisionsBounds = FlRect(0.f, 0.f, 90.f, 76.f);
		tmp.radiusDetectionSQRT = 707.f;
		tmp.radiusDetection = 500000.f;
		tmp.damage = 2;
		break;
	case EN_MOSQUITO:
		tmp.maxVelocity = 125.f;
		tmp.forward = vector2f(rand_float(-1, 1), rand_float(-1, 1));
		tmp.life = 1;
		break;
	case EN_MOLE:
		tmp.maxVelocity = 0.0f;
		tmp.time_wait = 4.f;
		tmp.origin = vector2f(32.f, 32.f);
		break;
	case EN_SCOLOPENDRE:
		tmp.state = EN_STATE_HIDDEN;
		tmp.damage = 2;
		if (en_IsScolopendreHorizontal(tmp.rotation))
			tmp.origin = vector2f(95.f, 23.f);
		else
			tmp.origin = vector2f(34.f, 96.f);
		tmp.collisionsBounds = en_GetAppropriateScolopendreBounds(&tmp);
		tmp.radius = 100.f;
		tmp.mass = 15.f;
		tmp.life = 2727;
		tmp.time_wait = 3.f;
		break;
	case EN_CHAMELEON:
		tmp.radiusDetectionSQRT = 500.f;
		tmp.radiusDetection = 250000.f;
		tmp.life = 2;
		tmp.size = vector2f(88.f, 78.f);
		tmp.origin = vector2f(44.f, 46.f);
		tmp.collisionsBounds = FlRect(0.f, 0.f, 88.f, 78.f);
		tmp.state = EN_STATE_HIDDEN;
		break;
	case EN_BAT:
		tmp.maxVelocity = 50.f;
		tmp.isOnRoof = sfTrue;
		tmp.life = 1;
		tmp.state = EN_STATE_WAIT;
		tmp.time_sound = 8.f;
		break;
	case EN_LITTLE_GHOST_JOY:
		tmp.maxVelocity = 100.0f;
		tmp.mass = 10.0f;
		tmp.state = EN_STATE_WALK;
		tmp.life = 1;
		tmp.origin = vector2f(42.f, 80.0f);
		tmp.collisionsBounds = FlRect(15.f, 85.f, 50.f, 42.f);
		break;
	case EN_BIG_GHOST_JOY:
		tmp.state = EN_STATE_WAIT;
		tmp.maxVelocity = 50.f;
		tmp.radiusDetectionSQRT = 400.f;
		tmp.radiusDetection = 160000.f;
		tmp.life = 2;
		tmp.size = vector2f(150.f, 234.f);
		tmp.origin = vector2f(75.f, 115.f);
		tmp.radius = 100.f;
		tmp.mass = 20.f;
		tmp.collisionsBounds = FlRect(0.f, 138.f, 150.f, 80.f);
		break;
	case EN_LITTLE_GHOST_SADNESS:
		tmp.life = 1;
		tmp.maxVelocity = 40.0f;
		tmp.state = EN_STATE_WALK;
		tmp.mass = 10.f;
		tmp.radius = 50.0f;
		tmp.collisionsBounds = FlRect(0.f, 55.f, 60.f, 35.f);
		break;
	case EN_BIG_GHOST_SADNESS:
		tmp.maxVelocity = 100.f;
		tmp.state = EN_STATE_WAIT;
		tmp.radius = 100.f;
		tmp.life = 2;
		tmp.origin = vector2f(90.f, 58.f);
		tmp.mass = 20.f;
		tmp.collisionsBounds = FlRect(40.f, 110.f, 110.f, 45.0f);
		tmp.damage = 2;
		break;
	case EN_IMUNITY:
		tmp.maxVelocity = 100.f;
		tmp.mass = 40.f;
		tmp.origin = vector2f(73.f, 78.f);
		tmp.collisionsBounds = FlRect(0.f, 0.f, 146.f, 156.f);
		break;
	case EN_SEAGULL:
		tmp.time_to_wait = rand_float(2.f, 7.f);
		tmp.state = EN_STATE_WAIT;
		tmp.origin = vector2f(52.f, 32.f);
		break;
	default:
		break;
	}

	if (getState() == EDITOR)
		enemiesList->push_back(&enemiesList, &tmp);
	else
	{
		sfMutex_lock(mutexThread2);
		enemiesList->push_back(&enemiesList, &tmp);
		sfMutex_unlock(mutexThread2);
	}
}

void en_UpdateEnemies(Window* _window)
{
	float delta = getDeltaTimeThread2();
	sfVector2f tmpPpos = plr_GetFeetPos();

	en_timer_test += delta;
	en_timer_tree += delta;

	if (DEBUG_STATE)
	{
		if (customKeyboardIsKeyPressed(sfKeyNumpad8, _window->renderWindow) && en_timer_test > 0.2f)
		{
			toRemoveToggleDebugRect = !toRemoveToggleDebugRect;
			en_timer_test = 0.0f;
		}
		if (customKeyboardIsKeyPressed(sfKeyQ, _window->renderWindow) && en_timer_test > 0.2f)
		{
			en_SpawnEnemy(vector2f(sfRectangleShape_getPosition(en_rectangleSpider).x - 350, sfRectangleShape_getPosition(en_rectangleSpider).y), EN_VENIMOUS_YOYO_SPIDER, 0.f, sfTrue);
			en_timer_test = 0.0f;
		}
		if (customKeyboardIsKeyPressed(sfKeyS, _window->renderWindow) && en_timer_test > 0.2f)
		{
			en_SpawnEnemy(vector2f(500.0f, 200.0f), EN_GOOSE, 0.f, sfFalse);
			en_timer_test = 0.0f;
		}
		if (customKeyboardIsKeyPressed(sfKeyD, _window->renderWindow) && en_timer_test > 0.2f)
		{
			en_SpawnEnemy(vector2f(500.0f, 200.0f), EN_MOSQUITO, 0.f, sfFalse);
			en_timer_test = 0.0f;
		}
		if (customKeyboardIsKeyPressed(sfKeyF, _window->renderWindow) && en_timer_test > 0.2f)
		{
			en_SpawnEnemy(vector2f(plr_GetPos().x + 400.0f, plr_GetPos().y), EN_MOLE, 0.f, sfFalse);
			en_timer_test = 0.0f;
		}
		if (customKeyboardIsKeyPressed(sfKeyG, _window->renderWindow) && en_timer_test > 0.2f)
		{
			en_SpawnEnemy(vector2f(300.0f, 300.0f), EN_BIG_GHOST_JOY, 0.f, sfFalse);
			en_timer_test = 0.0f;
		}
		if (customKeyboardIsKeyPressed(sfKeyH, _window->renderWindow) && en_timer_test > 0.2f)
		{
			en_SpawnEnemy(vector2f(344.0f, 346.0f), EN_CHAMELEON, 0.f, sfFalse);
			en_timer_test = 0.0f;
		}
		if (customKeyboardIsKeyPressed(sfKeyJ, _window->renderWindow) && en_timer_test > 0.2f)
		{
			en_SpawnEnemy(vector2f(500.0f, 500.0f), EN_BLACK_BILLY_GOAT, 0.f, sfFalse);
			en_timer_test = 0.0f;
		}
		if (customKeyboardIsKeyPressed(sfKeyK, _window->renderWindow) && en_timer_test > 0.2f)
		{
			en_SpawnEnemy(vector2f(500.0f, 500.0f), EN_LITTLE_GHOST_SADNESS, 0.f, sfFalse);
			en_timer_test = 0.0f;
		}
		if (customKeyboardIsKeyPressed(sfKeyP, _window->renderWindow) && en_timer_test > 0.2f)
		{
			en_SpawnEnemy(vector2f(500.0f, 500.0f), EN_BIG_GHOST_SADNESS, 0.f, sfFalse);
			en_timer_test = 0.0f;
		}
		if (customKeyboardIsKeyPressed(sfKeyO, _window->renderWindow) && en_timer_test > 0.2f)
		{
			en_SpawnEnemy(/*vector2f(500.0f, 500.0f)*/vector2f(plr_GetPos().x + 200.0f, plr_GetPos().y), EN_SEAGULL, 0.f, sfFalse);
			en_timer_test = 0.0f;
		}
		if (customKeyboardIsKeyPressed(sfKeyI, _window->renderWindow) && en_timer_test > 0.2f)
		{
			//en_SpawnEnemy(vector2f(plr_GetPos().x + 600.0f, plr_GetPos().y), EN_BAT, 180.f, sfFalse);
			en_timer_test = 0.0f;
		}
		if (en_timer_test > 0.3f)
		{
			if (customKeyboardIsKeyPressed(sfKeyL, _window->renderWindow)) {
				en_SpawnEnemy(vector2f(TILE_SIZE * 5.f + TILE_SIZE / 2.f, TILE_SIZE * 4.f + TILE_SIZE / 2.f), EN_SNAKE, 0.f, sfFalse);
				en_timer_test = 0.0f;
			}
			if (customKeyboardIsKeyPressed(sfKeyM, _window->renderWindow)) {

				en_SpawnEnemy(vector2f(500.f, 800.f), EN_CRAB, 0.f, sfFalse);
				en_timer_test = 0.0f;
			}
		}
	}

	if (!db_IsUp()) {
		for (int i = 0; i < enemiesList->size(enemiesList); i++)
		{
			if (getIsFreeingMap()) return;
			if (GD_ENEMY->pos.x - GD_ENEMY->size.x < getViewPosition(mainView).x + 960 && GD_ENEMY->pos.x + GD_ENEMY->size.x  > getViewPosition(mainView).x - 960 &&
				GD_ENEMY->pos.y - GD_ENEMY->size.y  < getViewPosition(mainView).y + 540 && GD_ENEMY->pos.y + GD_ENEMY->size.y > getViewPosition(mainView).y - 540)
				GD_ENEMY->isOnScreen = sfTrue;
			else {
				GD_ENEMY->isOnScreen = sfFalse;
				if (GD_ENEMY->type == EN_YOYO_SPIDER || GD_ENEMY->type == EN_VENIMOUS_YOYO_SPIDER) {
					sfSound_stop(GetSound("SFX_Yoyo_Running"));
					en_Erase(i, GD_ENEMY->pos, FX_NULL, _window);
					i--;
					continue;
				}
			}

			// uncomment this when it's ready

			if (GD_ENEMY->time_invincibility < 0.5f)
				GD_ENEMY->time_invincibility += delta;
			if (GD_ENEMY->isOnScreen)
				en_UpdateEnemyType[GD_ENEMY->type](GD_ENEMY, GD_ENEMY->state, i, delta, tmpPpos, _window);
		}
	}
}

void en_UpdateYoyoSpider(EnemiesStruct* _enemy, EnemiesState _tmpState, int _i, float _delta, sfVector2f _tmpPpos, Window* _window)
{
	_enemy->time_prt += _delta;
	_enemy->time_wait += _delta;
	if (_enemy->state == EN_STATE_BACK_HOME)
	{
		if (_enemy->time_wait > 0.75f)
		{
			if (_enemy->time_prt > 0.25f)
			{
				prt_CreateStunParticles(vector2f(_enemy->pos.x, _enemy->pos.y + 125.f));
				_enemy->time_prt = 0.f;
			}
		}
		_enemy->spriteRect.left = 0.f;
		_enemy->spriteRect.top = 112.f;
		_enemy->spriteRect.width = 78.f;
		_enemy->spriteRect.height = 62.f;
		if (_enemy->time_wait > 1.25f)
		{
			_enemy->pos = vector2f(_enemy->pos.x, _enemy->pos.y + 150.f);
			_enemy->forward = vector2f(-normalizef(createVectorWithVectorf(_enemy->pos, plr_GetFeetPos())).x, -normalizef(createVectorWithVectorf(_enemy->pos, plr_GetFeetPos())).y);
			_enemy->state = EN_STATE_ESCAPING;
			_enemy->frameX = 0.f;
			_enemy->time_sound = 4.0f;
			_enemy->timerAnim = 0.f;
		}
	}
	else if (_enemy->state == EN_STATE_ESCAPING)
	{
		_enemy->time_sound += _delta;
		if (_enemy->time_sound > 4.f)
		{
			_enemy->time_sound = 0.0f;
			playSoundFX("SFX_Yoyo_Running");
		}
		_enemy->maxVelocity = 250.f;
		checkCollisions(_i, _delta);
	}
	else if (_enemy->state == EN_STATE_WAIT)
	{
		if (_enemy->isOnString && _enemy->time_wait > 5.f)
		{
			_enemy->state = EN_STATE_GOINGUP;
			_enemy->frameX = 0.f;
			_enemy->timerAnim = 0.f;
			_enemy->time_wait = 0.f;
		}
	}
	else if (_enemy->state == EN_STATE_GOINGUP)
	{
		if (_enemy->time_wait > 1.1f)
		{
			_enemy->time_wait = 0.f;
			en_Erase(_i, _enemy->pos, FX_NULL, _window);
			return;
		}
	}
	else if (_enemy->state == EN_STATE_DAMAGING)
	{
		_enemy->time_wait += _delta;
		if (_enemy->time_wait > 0.2f)
		{
			sfSound_stop(GetSound("SFX_Yoyo_Running"));
			en_Erase(_i, _enemy->pos, FX_EXPLOSION, _window);

			return;
		}
	}
	else if (_enemy->state == EN_STATE_FALLING2)
	{
		_enemy->time_wait += _delta;

		_enemy->scale.x = 1.f - (_enemy->time_wait / 2.f);
		_enemy->scale.y = 1.f - (_enemy->time_wait / 2.f);
		_enemy->rotationFalling += (_enemy->time_wait / 30.f);
		if (_enemy->time_wait >= 2.f)
		{
			sfSound_stop(GetSound("SFX_Yoyo_Running"));
			en_Erase(_i, _enemy->pos, FX_STARS, _window);
			return;
		}
	}
	else if (_enemy->state == EN_STATE_STUNNED)
	{
		_enemy->currentVelocity = vector2f(0.0f, 0.0f);
	}
	else if (_enemy->state == EN_STATE_THROWN)
	{
		checkCollisions(_i, _delta);
		_enemy->time_wait += _delta;
		if (_enemy->time_wait > 1.f)
		{
			_enemy->time_wait = 0.f;
			_enemy->frameX = 0;
			_enemy->timerAnim = 0.0f;
			_enemy->state = EN_STATE_ESCAPING;
		}
	}


	if (_enemy->life <= 0)
	{
		if (_enemy->time_wait >= 1.f)
		{
			sfSound_stop(GetSound("SFX_Yoyo_Running"));
			en_Erase(_i, _enemy->pos, FX_EXPLOSION, _window);
			return;
		}
	}
}


void en_UpdateVenimousYoyoSpider(EnemiesStruct* _enemy, EnemiesState _tmpState, int _i, float _delta, sfVector2f _tmpPpos, Window* _window)
{
	_enemy->time_wait += _delta;
	_enemy->time_prt += _delta;
	if (_enemy->state == EN_STATE_BACK_HOME)
	{
		if (_enemy->time_wait > 0.75f)
		{
			if (_enemy->time_prt > 0.25f)
			{
				prt_CreateStunParticles(vector2f(_enemy->pos.x, _enemy->pos.y + 125.f));
				_enemy->time_prt = 0.f;
			}
		}
		_enemy->spriteRect.left = 0.f;
		_enemy->spriteRect.top = 112.f;
		_enemy->spriteRect.width = 78.f;
		_enemy->spriteRect.height = 62.f;
		if (_enemy->time_wait > 1.25f)
		{
			_enemy->pos = vector2f(_enemy->pos.x, _enemy->pos.y + 150.f);
			_enemy->forward = vector2f(-normalizef(createVectorWithVectorf(_enemy->pos, plr_GetFeetPos())).x, -normalizef(createVectorWithVectorf(_enemy->pos, plr_GetFeetPos())).y);
			_enemy->time_sound = 4.0f;
			_enemy->state = EN_STATE_ESCAPING;
			_enemy->frameX = 0.f;
			_enemy->timerAnim = 0.f;
		}
	}
	else if (_enemy->state == EN_STATE_ESCAPING)
	{
		_enemy->time_sound += _delta;
		if (_enemy->time_sound > 4.f)
		{
			_enemy->time_sound = 0.0f;
			playSoundFX("SFX_Yoyo_Running");
		}
		_enemy->maxVelocity = 250.f;
		checkCollisions(_i, _delta);
		if (_enemy->pos.x > mainView->viewPos.x + _enemy->spriteRect.width + (mainView->size.x / 2.f) || _enemy->pos.x < mainView->viewPos.x - _enemy->spriteRect.width - (mainView->size.x / 2.f) ||
			_enemy->pos.y > mainView->viewPos.y + _enemy->spriteRect.height + (mainView->size.y / 2.f) || _enemy->pos.y < mainView->viewPos.y - _enemy->spriteRect.height - (mainView->size.y / 2.f))
		{
			sfSound_stop(GetSound("SFX_Yoyo_Running"));
			en_Erase(_i, _enemy->pos, FX_NULL, _window);
			return;
		}
	}
	else if (_enemy->state == EN_STATE_WAIT)
	{
		if (_enemy->isOnString && _enemy->time_wait > 5.f)
		{
			_enemy->state = EN_STATE_GOINGUP;
			_enemy->frameX = 0.f;
			_enemy->timerAnim = 0.f;
			_enemy->time_wait = 0.f;
		}
	}
	else if (_enemy->state == EN_STATE_GOINGUP)
	{
		if (_enemy->time_wait > 1.1f)
		{
			_enemy->time_wait = 0.f;
			en_Erase(_i, _enemy->pos, FX_NULL, _window);
			return;
		}
	}
	else if (_enemy->state == EN_STATE_DAMAGING)
	{
		_enemy->time_wait += _delta;
		if (_enemy->time_wait > 0.2f)
		{
			sfSound_stop(GetSound("SFX_Yoyo_Running"));
			en_Erase(_i, _enemy->pos, FX_EXPLOSION, _window);
			return;
		}
	}
	else if (_enemy->state == EN_STATE_STUNNED)
	{
		_enemy->currentVelocity = vector2f(0.0f, 0.0f);
	}
	else if (_enemy->state == EN_STATE_FALLING2)
	{
		_enemy->time_wait += _delta;

		_enemy->scale.x = 1.f - (_enemy->time_wait / 2.f);
		_enemy->scale.y = 1.f - (_enemy->time_wait / 2.f);
		_enemy->rotationFalling += (_enemy->time_wait / 30.f);
		if (_enemy->time_wait >= 2.f)
		{
			sfSound_stop(GetSound("SFX_Yoyo_Running"));
			en_Erase(_i, _enemy->pos, FX_STARS, _window);
			return;
		}
	}
	else if (_enemy->state == EN_STATE_THROWN)
	{
		checkCollisions(_i, _delta);
		_enemy->time_wait += _delta;
		if (_enemy->time_wait > 1.f)
		{
			_enemy->time_wait = 0.f;
			_enemy->frameX = 0;
			_enemy->timerAnim = 0.0f;
			_enemy->state = EN_STATE_ESCAPING;
		}
	}
	else if (_enemy->state == EN_STATE_REVEALING)
	{
		_enemy->timerAllAttacks += _delta;

		if (_enemy->timerAllAttacks >= 0.5f)
		{
			_enemy->timerAllAttacks = 0.0f;
			_enemy->time_wait = 0.0f;
			_enemy->forward = vector2f(-normalizef(createVectorWithVectorf(_enemy->pos, plr_GetFeetPos())).x, -normalizef(createVectorWithVectorf(_enemy->pos, plr_GetFeetPos())).y);
			_enemy->state = EN_STATE_ESCAPING;
		}
	}
	if (_enemy->life <= 0)
	{
		if (_enemy->time_wait >= 1.f)
		{
			sfSound_stop(GetSound("SFX_Yoyo_Running"));
			en_Erase(_i, _enemy->pos, FX_EXPLOSION, _window);
			return;
		}
	}
}

void en_UpdateCrab(EnemiesStruct* _enemy, EnemiesState _tmpState, int _i, float _delta, sfVector2f _tmpPpos, Window* _window)
{
	_enemy->time_prt += _delta;
	if (_enemy->state == EN_STATE_WALK)
	{
		_enemy->time_wait += _delta;
		if (_enemy->time_wait > 4.f)
		{
			_enemy->time_wait = 0.0f;
			_enemy->state = EN_STATE_WAIT;
		}
		if (_enemy->time_prt > 0.30f)
		{
			prt_CreateCircleParticles(vector2f(_enemy->pos.x, _enemy->pos.y + 20.f), 3, 6, sfColor_fromRGBA(235.f, 238.f, 176.f, 255.f), sfColor_fromRGBA(235.f, 238.f, 176.f, 200.f),
				0.9f, 8.f, 0.f, 360.f, 0.f, 65.f, 250.f, 0.6f, sfColor_fromRGBA(255.f, 255.f, 255.f, 105.f), sfColor_fromRGBA(255.f, 255.f, 255.f, 50.f), sfFalse, sfTrue, sfFalse, NULL, sfFalse, sfFalse, sfFalse, DUST);
			_enemy->time_prt = 0.f;
		}
	}
	else if (_enemy->state == EN_STATE_WAIT)
	{
		_enemy->time_wait += _delta;
		if (_enemy->time_wait > 5.f)
		{
			_enemy->time_wait = 0.0f;
			_enemy->targetForward = normalizef(vector2f(rand_float(-1.0f, 1.0f), rand_float(-1.0f, 1.0f)));
			_enemy->state = EN_STATE_WALK;
		}
	}
	else if (_enemy->state == EN_STATE_DETECTING)
	{
		if (_enemy->time_prt > 0.30f)
		{
			prt_CreateCircleParticles(vector2f(_enemy->pos.x, _enemy->pos.y + 20.f), 3, 6, sfColor_fromRGBA(235.f, 238.f, 176.f, 255.f), sfColor_fromRGBA(235.f, 238.f, 176.f, 200.f),
				0.9f, 8.f, 0.f, 360.f, 0.f, 65.f, 250.f, 0.6f, sfColor_fromRGBA(255.f, 255.f, 255.f, 105.f), sfColor_fromRGBA(255.f, 255.f, 255.f, 50.f), sfFalse, sfTrue, sfFalse, NULL, sfFalse, sfFalse, sfFalse, DUST);
			_enemy->time_prt = 0.f;
		}
		_enemy->timerAnim *= 1.1f;
		_enemy->time_wait += _delta;
		if (_enemy->time_wait > 1.5f && getSqrMagnitudeBetween2Vectorsf(_enemy->pos, _tmpPpos) > 200.0f * 200.0f)
		{
			_enemy->countOfSeePlayer++;
			_enemy->maxVelocity = 50.0f;
			_enemy->currentVelocity = vector2f(0.0f, 0.0f);
			_enemy->time_wait = 0.0f;
			_enemy->state = EN_STATE_WAIT;
		}
		_enemy->targetForward = normalizef(createVectorWithVectorf(_tmpPpos, _enemy->pos));
	}
	else if (_enemy->state == EN_STATE_CHASE)
	{
		if (_enemy->time_prt > 0.30f)
		{
			prt_CreateCircleParticles(vector2f(_enemy->pos.x, _enemy->pos.y + 20.f), 3, 6, sfColor_fromRGBA(235.f, 238.f, 176.f, 255.f), sfColor_fromRGBA(235.f, 238.f, 176.f, 200.f),
				0.9f, 8.f, 0.f, 360.f, 0.f, 65.f, 250.f, 0.6f, sfColor_fromRGBA(255.f, 255.f, 255.f, 105.f), sfColor_fromRGBA(255.f, 255.f, 255.f, 50.f), sfFalse, sfTrue, sfFalse, NULL, sfFalse, sfFalse, sfFalse, DUST);
			_enemy->time_prt = 0.f;
		}
		_enemy->time_wait += _delta;
		if (_enemy->time_wait > 2.2f)
		{
			_enemy->time_wait = 0.0f;
			_enemy->frameX = 0;
			_enemy->timerAnim = 0.0f;
			_enemy->countOfSeePlayer = 0;
			_enemy->state = EN_STATE_WAIT;
		}
	}
	else if (_enemy->state == EN_STATE_BACK_HOME)
	{
		if (_enemy->time_prt > 0.30f)
		{
			prt_CreateCircleParticles(vector2f(_enemy->pos.x, _enemy->pos.y + 20.f), 3, 6, sfColor_fromRGBA(235.f, 238.f, 176.f, 255.f), sfColor_fromRGBA(235.f, 238.f, 176.f, 200.f),
				0.9f, 8.f, 0.f, 360.f, 0.f, 65.f, 250.f, 0.6f, sfColor_fromRGBA(255.f, 255.f, 255.f, 105.f), sfColor_fromRGBA(255.f, 255.f, 255.f, 50.f), sfFalse, sfTrue, sfFalse, NULL, sfFalse, sfFalse, sfFalse, DUST);
			_enemy->time_prt = 0.f;
		}
		_enemy->time_wait += _delta;
		if (_enemy->countOfSeePlayer >= 3)
		{
			if (_enemy->time_wait > 3.4f)
			{
				en_Erase(_i, _enemy->pos, FX_NULL, _window);
				return;
			}
		}
		else
		{
			if (_enemy->time_wait > 3.4f)
			{
				_enemy->time_wait = 0.0f;
				_enemy->pos = _enemy->startPos;
				_enemy->frameX = 0;
				_enemy->timerAnim = 0.0f;
				_enemy->time_detection = 0.0f;
				_enemy->state = EN_STATE_CHASE;
			}
		}
	}
	else if (_enemy->state == EN_STATE_THROWN)
	{
		if (_enemy->time_prt > 0.5f)
		{
			prt_CreateStunParticles(_enemy->pos);
			_enemy->time_prt = 0.f;
		}
		_enemy->stunTimer -= _delta;
		_enemy->forward = normalizef(vector2f(rand_float(-1.0f, 1.0f), rand_float(-1.0f, 1.0f)));

		if (_enemy->stunTimer > 0.0f)
		{
			if (cll_RectMap(FlRect(_enemy->pos.x - _enemy->origin.x, _enemy->pos.y - _enemy->origin.y, _enemy->size.x, _enemy->size.y), multiplyVectorsrf(_enemy->currentVelocity, _delta))) {
				_enemy->stunTimer = 0.f;
				_enemy->endPos = _enemy->pos;
			}
			else
				checkCollisions(_i, _delta);
		}
		else if (_enemy->stunTimer < -2.0f)
		{
			if (getSqrMagnitudeBetween2Vectorsf(_enemy->pos, _tmpPpos) < 200.0f * 200.0f)
			{
				_enemy->frameX = 0;
				_enemy->timerAnim = 0.0f;
				_enemy->time_detection = 0.0f;
				_enemy->state = EN_STATE_BACK_HOME;
			}
			else
				_enemy->state = EN_STATE_WAIT;
		}

	}
	else if (_enemy->state == EN_STATE_DAMAGING)
	{
		if (!_enemy->isOnePassedPulse)
		{
			_enemy->time_invincibility = 0.0f;
			_enemy->isOnePassedPulse = sfTrue;
		}
		if (_enemy->time_invincibility > 0.5f)
		{
			_enemy->time_invincibility = 0.0f;
			en_Erase(_i, _enemy->pos, FX_EXPLOSION, _window);
			return;
		}
	}
	if (getSqrMagnitudeBetween2Vectorsf(_enemy->pos, _tmpPpos) < 200.0f * 200.0f && _enemy->state != EN_STATE_DAMAGING && _enemy->state != EN_STATE_DETECTING && _enemy->state != EN_STATE_BACK_HOME && _enemy->state != EN_STATE_CHASE && _enemy->state != EN_STATE_THROWN && _enemy->state != EN_STATE_STUNNED)
	{
		_enemy->state = EN_STATE_DETECTING;
		_enemy->time_wait = 0.0f;
		_enemy->maxVelocity = 300.0f;
	}
	if (_enemy->countOfSeePlayer >= 3 && _enemy->state != EN_STATE_BACK_HOME)
	{
		_enemy->time_wait = 0.0f;
		_enemy->frameX = 0;
		_enemy->timerAnim = 0.0f;
		_enemy->time_detection = 0.0f;
		_enemy->state = EN_STATE_BACK_HOME;
	}

	_enemy->forward = vec2f_lerp(_enemy->targetForward, _enemy->forward, 0.2 * _delta);

	if (_enemy->state != EN_STATE_WAIT && _enemy->state != EN_STATE_BACK_HOME && _enemy->state != EN_STATE_CHASE && _enemy->state != EN_STATE_THROWN && _enemy->state != EN_STATE_STUNNED && _enemy->state != EN_STATE_DAMAGING)
		checkCollisions(_i, _delta);
}

void en_UpdateGoose(EnemiesStruct* _enemy, EnemiesState _tmpState, int _i, float _delta, sfVector2f _tmpPpos, Window* _window)
{
	_enemy->time_prt += _delta;
	if (_enemy->state != EN_STATE_THROWN && _enemy->state != EN_STATE_FALLING2)
	{
		if (_enemy->state != EN_STATE_RUSH)
			_enemy->forward = vec2f_lerp(_enemy->targetForward, _enemy->forward, 0.2f * _delta);
		else
			_enemy->forward = vec2f_lerp(_enemy->targetForward, _enemy->forward, 0.01f * _delta);
	}

	if (_enemy->state != EN_STATE_WAIT && _enemy->state != EN_STATE_CHASE && _enemy->state != EN_STATE_ATTACK && _enemy->state != EN_STATE_DAMAGING && _enemy->state != EN_STATE_STUNNED && _enemy->state != EN_STATE_FALLING2)
		checkCollisions(_i, _delta);

	if (getSqrMagnitudeBetween2Vectorsf(_enemy->pos, _tmpPpos) < 300.0f * 300.0f && _enemy->state != EN_STATE_ATTACK && _enemy->state != EN_STATE_CHASE && _enemy->state != EN_STATE_RUSH && _enemy->state != EN_STATE_DAMAGING && _enemy->state != EN_STATE_THROWN && _enemy->state != EN_STATE_STUNNED && _enemy->state != EN_STATE_FALLING2)
	{
		if(_enemy->state == EN_STATE_PATROLING)
			sfSound_stop(GetSound("SFX_Goose_Walk"));
		_enemy->state = EN_STATE_CHASE;
		_enemy->time_wait = 0.0f;
		_enemy->frameX = 0;
		_enemy->timerAnim = 0.0f;
	}

	switch (_tmpState)
	{
	case EN_STATE_PATROLING:
		_enemy->time_sound += _delta;
		if (_enemy->time_sound >= 9.f)
		{
			_enemy->time_sound = 0.0f;
			playSoundFX("SFX_Goose_Walk");
		}

		_enemy->targetForward = normalizef(createVectorWithVectorf(_enemy->pos, _enemy->Patrolpoint[_enemy->CurrentPatrolPoint]));
		if (equalsWithOffset(_enemy->pos, _enemy->Patrolpoint[_enemy->CurrentPatrolPoint], 10.f))
		{
			_enemy->CurrentPatrolPoint++;
			if (_enemy->CurrentPatrolPoint >= 4)
				_enemy->CurrentPatrolPoint = 0;
			sfSound_stop(GetSound("SFX_Goose_Walk"));
			_enemy->state = EN_STATE_WAIT;
			_enemy->frameX = 0;
			_enemy->timerAnim = 0.0f;
			_enemy->time_wait = 0.0f;
		}
		break;
	case EN_STATE_WAIT:
		_enemy->time_wait += _delta;

		if (_enemy->time_wait > 2.f)
		{
			_enemy->time_wait = 0.0f;
			_enemy->timerAnim = 0.0f;
			_enemy->frameX = 0;
			_enemy->time_sound = 9.0f;
			_enemy->state = EN_STATE_PATROLING;
		}
		break;
	case EN_STATE_CHASE:
		_enemy->time_wait += _delta;

		if (_enemy->time_wait > 1.f)
		{
			_enemy->time_wait = 0.0f;
			_enemy->frameX = 0;
			_enemy->timerAnim = 0.0f;
			_enemy->time_sound = 9.0f;
			sfSound_stop(GetSound("SFX_Goose_Walk"));
			_enemy->forward = normalizef(createVectorWithVectorf(_enemy->pos, _tmpPpos));
			_enemy->state = EN_STATE_RUSH;
		}

		if (getSqrMagnitudeBetween2Vectorsf(_enemy->pos, _tmpPpos) > 300.0f * 300.0f)
		{
			_enemy->time_wait = 0.0f;
			_enemy->state = EN_STATE_WAIT;
			_enemy->frameX = 0;
			_enemy->timerAnim = 0.0f;
		}
		break;
	case EN_STATE_RUSH:
		_enemy->time_sound += _delta;
		if (_enemy->time_sound >= 9.f)
		{
			_enemy->time_sound = 0.0f;
			playSoundFX("SFX_Goose_Walk");
		}

		_enemy->targetForward = normalizef(createVectorWithVectorf(_enemy->pos, _tmpPpos));
		_enemy->maxVelocity = 250.0f;
		if (getSqrMagnitudeBetween2Vectorsf(_enemy->pos, _tmpPpos) < 100.0f * 100.0f)
		{
			sfSound_stop(GetSound("SFX_Goose_Walk"));
			_enemy->state = EN_STATE_ATTACK;
			_enemy->frameX = 0;
			_enemy->timerAnim = 0.0f;
		}

		if (getSqrMagnitudeBetween2Vectorsf(_enemy->pos, _tmpPpos) > 300.0f * 300.0f)
		{
			_enemy->state = EN_STATE_PATROLING;
			_enemy->time_sound = -9.0f;
			_enemy->frameX = 0;
			_enemy->timerAnim = 0.0f;
		}
		break;
	case EN_STATE_ATTACK:

		if (_enemy->time_prt > 1.0f)
		{
			prt_CreateCircleParticles(_enemy->pos, 5, 8, sfColor_fromRGBA(36.f, 165.f, 15.f, 200.f), sfColor_fromRGBA(36.f, 165.f, 15.f, 150.f), 1.f, 8.f, 5.f, 0.f, 360.f, 90.f, 200.f, 0.3f,
				sfColor_fromRGBA(255.f, 255.f, 255.f, 200.f), sfColor_fromRGBA(255.f, 255.f, 255.f, 150.f), sfTrue, sfFalse, sfFalse, GetSfTexture("feather_Particles"), sfFalse, sfFalse, sfFalse, FEATHER);
			_enemy->time_prt = 0.f;
		}
		_enemy->time_wait += _delta;

		if (_enemy->time_wait > 0.7f)
		{
			_enemy->time_wait = 0.0f;
			if (getSqrMagnitudeBetween2Vectorsf(_enemy->pos, _tmpPpos) < 100.0f * 100.0f)
			{
				plr_addDamagePlayerLife(1);
			}
			else
			{
				_enemy->state = EN_STATE_PATROLING;
				_enemy->time_sound = -9.0f;
				_enemy->frameX = 0;
				_enemy->timerAnim = 0.0f;
			}
		}
		break;
	case EN_STATE_DAMAGING:
		_enemy->time_invincibility += _delta;

		if (_enemy->time_invincibility > .2f)
		{
			sfSound_stop(GetSound("SFX_Goose_Walk"));
			en_Erase(_i, _enemy->pos, FX_EXPLOSION, _window);
		}
		break;
	case EN_STATE_THROWN:
		_enemy->time_wait += _delta;
		if (_enemy->time_wait > 1.f)
		{
			_enemy->time_wait = 0.f;
			_enemy->frameX = 0;
			_enemy->timerAnim = 0.0f;
			_enemy->time_sound = 9.0f;
			_enemy->state = EN_STATE_PATROLING;
		}
		break;
	case EN_STATE_STUNNED:
		_enemy->time_wait += _delta;
		if (_enemy->time_wait > 5.f)
		{
			_enemy->frameX = 0;
			_enemy->timerAnim = 0.0f;
			_enemy->time_wait = 0.f;
			_enemy->time_sound = 9.0f;
			_enemy->state = EN_STATE_PATROLING;
		}
		break;
	case EN_STATE_FALLING2:
		_enemy->time_wait += _delta;
		sfSound_stop(GetSound("SFX_Goose_Walk"));
		_enemy->scale.x = 1.f - (_enemy->time_wait / 2.f);
		_enemy->scale.y = 1.f - (_enemy->time_wait / 2.f);
		_enemy->rotationFalling += (_enemy->time_wait / 30.f);
		if (_enemy->time_wait >= 2.f)
		{
			en_Erase(_i, _enemy->pos, FX_STARS, _window);
			return;
		}
		break;
	default:
		_enemy->frameX = 0;
		_enemy->timerAnim = 0.0f;
		_enemy->time_wait = 0.f;
		_enemy->time_sound = 9.0f;
		_enemy->state = EN_STATE_PATROLING;
		break;
	}
}

void en_UpdateWhiteBillyGoat(EnemiesStruct* _enemy, EnemiesState _tmpState, int _i, float _delta, sfVector2f _tmpPpos, Window* _window)
{
	_enemy->time_wait += _delta;
	_enemy->time_prt += _delta;

	if (_enemy->state == EN_STATE_WAIT)
	{
		_enemy->forward = vector2f(1.f, 0.f);
		if (getSqrMagnitudeBetween2Vectorsf(_enemy->pos, _tmpPpos) < 300.f * 300.f && !plr_IsInvisible())
		{
			_enemy->time_wait = 0.f;
			_enemy->state = EN_STATE_CHASE;
			_enemy->frameX = 0.f;
			_enemy->timerAnim = 0.f;
			return;
		}
		_enemy->maxVelocity = 0.f;
		if (getSqrMagnitudeBetween2Vectorsf(_enemy->pos, _enemy->spawnPos) > SPAWN_DIAMETER * 2)
		{
			_enemy->time_sound = 1.5f;
			_enemy->state = EN_STATE_BACK_HOME;
			_enemy->frameX = 0.f;
			_enemy->timerAnim = 0.f;
		}
	}
	else if (_enemy->state == EN_STATE_BACK_HOME)
	{
		_enemy->time_sound += _delta;
		if (_enemy->time_sound > 1.5f)
		{
			_enemy->time_sound = 0.f;
			playSoundFX("SFX_Bouc_Running");
		}
		_enemy->forward = normalizef(createVectorWithVectorf(_enemy->pos, _enemy->spawnPos));
		_enemy->targetForward = normalizef(createVectorWithVectorf(_enemy->pos, _enemy->spawnPos));
		_enemy->maxVelocity = 175.f;
		checkCollisions(_i, _delta);
		if (getSqrMagnitudeBetween2Vectorsf(_enemy->pos, _tmpPpos) < 80.0f * 80.0f)
		{
			plr_addDamagePlayerLife(1);
		}
		if (_enemy->time_prt > 0.30f)
		{

			float angleptr = getAngleBetweenVectors(vector2f(1.f, 0.f), vector2f(-_enemy->forward.x, -_enemy->forward.y));
			angleptr *= RAD2DEG;
			if (_enemy->forward.x > 0.f)
			{
				prt_CreateCircleParticles(vector2f(_enemy->pos.x - 70, _enemy->pos.y + 70.f), 3, 8, sfColor_fromRGBA(137.f, 108.f, 82.f, 200.f), sfColor_fromRGBA(137.f, 108.f, 82.f, 150.f), 0.5f, 8.f, 0.f, angleptr - 25.f, angleptr + 05.f, 65.f, 200.f, 0.3f,
					sfColor_fromRGBA(255.f, 255.f, 255.f, 200.f), sfColor_fromRGBA(255.f, 255.f, 255.f, 150.f), sfTrue, sfFalse, sfFalse, NULL, sfFalse, sfFalse, sfFalse, DUST);
				prt_CreateCircleParticles(vector2f(_enemy->pos.x, _enemy->pos.y + 70.f), 3, 8, sfColor_fromRGBA(137.f, 108.f, 82.f, 200.f), sfColor_fromRGBA(137.f, 108.f, 82.f, 150.f), 0.5f, 8.f, 0.f, angleptr - 25.f, angleptr + 05.f, 65.f, 200.f, 0.3f,
					sfColor_fromRGBA(255.f, 255.f, 255.f, 200.f), sfColor_fromRGBA(255.f, 255.f, 255.f, 150.f), sfTrue, sfFalse, sfFalse, NULL, sfFalse, sfFalse, sfFalse, DUST);
			}
			if (_enemy->forward.x < 0.f)
			{
				prt_CreateCircleParticles(vector2f(_enemy->pos.x + 70, _enemy->pos.y + 70.f), 3, 8, sfColor_fromRGBA(137.f, 108.f, 82.f, 200.f), sfColor_fromRGBA(137.f, 108.f, 82.f, 150.f), 0.5f, 8.f, 0.f, angleptr - 25.f, angleptr + 05.f, 65.f, 200.f, 0.3f,
					sfColor_fromRGBA(255.f, 255.f, 255.f, 200.f), sfColor_fromRGBA(255.f, 255.f, 255.f, 155.f), sfTrue, sfFalse, sfFalse, NULL, sfFalse, sfFalse, sfFalse, DUST);
				prt_CreateCircleParticles(vector2f(_enemy->pos.x, _enemy->pos.y + 70.f), 3, 8, sfColor_fromRGBA(137.f, 108.f, 82.f, 200.f), sfColor_fromRGBA(137.f, 108.f, 82.f, 150.f), 0.5f, 8.f, 0.f, angleptr - 25.f, angleptr + 05.f, 65.f, 200.f, 0.3f,
					sfColor_fromRGBA(255.f, 255.f, 255.f, 200.f), sfColor_fromRGBA(255.f, 255.f, 255.f, 150.f), sfTrue, sfFalse, sfFalse, NULL, sfFalse, sfFalse, sfFalse, DUST);
			}
			_enemy->time_prt = 0.f;
		}
		if (getSqrMagnitudeBetween2Vectorsf(_enemy->pos, _enemy->spawnPos) < rand_float(1000.0f, 30000.0f))
		{
			_enemy->time_detection = 0.0f;
			_enemy->state = EN_STATE_WAIT;
			_enemy->frameX = 0.f;
			_enemy->timerAnim = 0.f;
		}
	}
	else if (_enemy->state == EN_STATE_CHASE)
	{
		_enemy->maxVelocity = 0.f;
		if (_enemy->time_wait > 1.5f)
		{

			_enemy->forward = normalizef(createVectorWithVectorf(_enemy->pos, _tmpPpos));

			_enemy->time_wait = 0.f;
			_enemy->time_sound = 1.5f;
			_enemy->state = EN_STATE_RUSH;
			_enemy->frameX = 0.f;
			_enemy->timerAnim = 0.f;
			_enemy->startVelocity = NULLVECTF;
			_enemy->currentVelocity = NULLVECTF;
		}
		if ((getSqrMagnitudeBetween2Vectorsf(_enemy->pos, _tmpPpos) > 300.f * 300.f || plr_IsInvisible()))
		{
			_enemy->state = EN_STATE_WAIT;
			_enemy->frameX = 0.f;
			_enemy->timerAnim = 0.f;
		}

		if (getSqrMagnitudeBetween2Vectorsf(_enemy->spawnPos, plr_GetPos()) > SPAWN_DIAMETER * 2)
		{
			_enemy->time_sound = 1.5f;
			_enemy->state = EN_STATE_BACK_HOME;
			_enemy->frameX = 0.f;
			_enemy->timerAnim = 0.f;
		}
	}
	else if (_enemy->state == EN_STATE_RUSH)
	{
		_enemy->time_sound += _delta;
		if (_enemy->time_sound > 1.5f)
		{
			_enemy->time_sound = 0.f;
			playSoundFX("SFX_Bouc_Running");
		}
		_enemy->maxVelocity = 450.f;
		checkCollisions(_i, _delta);
		if (getSqrMagnitudeBetween2Vectorsf(_enemy->pos, _tmpPpos) < 80.0f * 80.0f)
		{
			plr_addDamagePlayerLife(2);
		}
		if (_enemy->time_prt > 0.30f)
		{

			float angleptr = getAngleBetweenVectors(vector2f(1.f, 0.f), vector2f(-_enemy->forward.x, -_enemy->forward.y));
			angleptr *= RAD2DEG;
			if (_enemy->forward.x > 0.f)
			{
				prt_CreateCircleParticles(vector2f(_enemy->pos.x - 70, _enemy->pos.y + 70.f), 3, 8, sfColor_fromRGBA(137.f, 108.f, 82.f, 200.f), sfColor_fromRGBA(137.f, 108.f, 82.f, 150.f), 0.5f, 8.f, 0.f, angleptr - 25.f, angleptr + 05.f, 65.f, 200.f, 0.3f,
					sfColor_fromRGBA(255.f, 255.f, 255.f, 200.f), sfColor_fromRGBA(255.f, 255.f, 255.f, 150.f), sfTrue, sfFalse, sfFalse, NULL, sfFalse, sfFalse, sfFalse, DUST);
				prt_CreateCircleParticles(vector2f(_enemy->pos.x, _enemy->pos.y + 70.f), 3, 8, sfColor_fromRGBA(137.f, 108.f, 82.f, 200.f), sfColor_fromRGBA(137.f, 108.f, 82.f, 150.f), 0.5f, 8.f, 0.f, angleptr - 25.f, angleptr + 05.f, 65.f, 200.f, 0.3f,
					sfColor_fromRGBA(255.f, 255.f, 255.f, 200.f), sfColor_fromRGBA(255.f, 255.f, 255.f, 150.f), sfTrue, sfFalse, sfFalse, NULL, sfFalse, sfFalse, sfFalse, DUST);
			}
			if (_enemy->forward.x < 0.f)
			{
				prt_CreateCircleParticles(vector2f(_enemy->pos.x + 70, _enemy->pos.y + 70.f), 3, 8, sfColor_fromRGBA(137.f, 108.f, 82.f, 200.f), sfColor_fromRGBA(137.f, 108.f, 82.f, 150.f), 0.5f, 8.f, 0.f, angleptr - 25.f, angleptr + 05.f, 65.f, 200.f, 0.3f,
					sfColor_fromRGBA(255.f, 255.f, 255.f, 200.f), sfColor_fromRGBA(255.f, 255.f, 255.f, 155.f), sfTrue, sfFalse, sfFalse, NULL, sfFalse, sfFalse, sfFalse, DUST);
				prt_CreateCircleParticles(vector2f(_enemy->pos.x, _enemy->pos.y + 70.f), 3, 8, sfColor_fromRGBA(137.f, 108.f, 82.f, 200.f), sfColor_fromRGBA(137.f, 108.f, 82.f, 150.f), 0.5f, 8.f, 0.f, angleptr - 25.f, angleptr + 05.f, 65.f, 200.f, 0.3f,
					sfColor_fromRGBA(255.f, 255.f, 255.f, 200.f), sfColor_fromRGBA(255.f, 255.f, 255.f, 150.f), sfTrue, sfFalse, sfFalse, NULL, sfFalse, sfFalse, sfFalse, DUST);
			}
			_enemy->time_prt = 0.f;
		}
		if (_enemy->time_wait > 2.5f)
		{
			_enemy->time_wait = 0.f;
			_enemy->maxVelocity = 0.f;
			_enemy->state = EN_STATE_CHASE;
			_enemy->frameX = 0.f;
			_enemy->timerAnim = 0.f;
		}
		if (getSqrMagnitudeBetween2Vectorsf(_enemy->pos, _enemy->spawnPos) > SPAWN_DIAMETER * 2)
		{
			_enemy->state = EN_STATE_BACK_HOME;
			_enemy->frameX = 0.f;
			_enemy->timerAnim = 0.f;
			_enemy->time_wait = 0.f;
		}
	}
	else if (_enemy->state == EN_STATE_THROWN)
	{
		checkCollisions(_i, _delta);
		_enemy->time_wait += _delta;
		if (_enemy->time_wait > 1.f)
		{
			_enemy->time_wait = 0.f;
			_enemy->frameX = 0;
			_enemy->timerAnim = 0.0f;
			sfSound_stop(GetSound("Bouc_Running"));
			_enemy->state = EN_STATE_WAIT;
		}
	}
	else if (_enemy->state == EN_STATE_DAMAGING)
	{
		_enemy->time_wait += _delta;
		if (_enemy->time_wait > 0.2f)
		{
			en_Erase(_i, _enemy->pos, FX_EXPLOSION, _window);
		}
	}
	else if (_enemy->state == EN_STATE_FALLING2)
	{
		_enemy->time_wait += _delta;

		_enemy->scale.x = 1.f - (_enemy->time_wait / 2.f);
		_enemy->scale.y = 1.f - (_enemy->time_wait / 2.f);
		_enemy->rotationFalling += (_enemy->time_wait / 30.f);
		if (_enemy->time_wait >= 2.f)
		{
			en_Erase(_i, _enemy->pos, FX_STARS, _window);
			return;
		}
	}
}

void en_UpdateBlackBillyGoat(EnemiesStruct* _enemy, EnemiesState _tmpState, int _i, float _delta, sfVector2f _tmpPpos, Window* _window)
{
	_enemy->time_wait += _delta;
	_enemy->time_prt += _delta;

	if (_enemy->state == EN_STATE_WAIT)
	{
		_enemy->forward = vector2f(1.f, 0.f);
		_enemy->maxVelocity = 0.f;
		if (getSqrMagnitudeBetween2Vectorsf(_enemy->pos, _tmpPpos) < 450.f * 450.f && !plr_IsInvisible())
		{
			_enemy->time_wait = 0.f;
			_enemy->state = EN_STATE_CHASE;
			_enemy->frameX = 0.f;
			_enemy->timerAnim = 0.f;
			return;
		}
		if (getSqrMagnitudeBetween2Vectorsf(_enemy->pos, _enemy->spawnPos) > SPAWN_DIAMETER * 3.f)
		{
			_enemy->time_sound = 1.5f;
			_enemy->state = EN_STATE_BACK_HOME;
			_enemy->frameX = 0.f;
			_enemy->timerAnim = 0.f;
		}
	}
	else if (_enemy->state == EN_STATE_BACK_HOME)
	{
		_enemy->time_sound += _delta;
		if (_enemy->time_sound > 1.5f)
		{
			_enemy->time_sound = 0.f;
			playSoundFX("SFX_Bouc_Running");
		}
		_enemy->forward = normalizef(createVectorWithVectorf(_enemy->pos, _enemy->spawnPos));
		_enemy->targetForward = normalizef(createVectorWithVectorf(_enemy->pos, _enemy->spawnPos));
		_enemy->maxVelocity = 225.f;
		checkCollisions(_i, _delta);
		if (getSqrMagnitudeBetween2Vectorsf(_enemy->pos, _tmpPpos) < 80.0f * 80.0f)
		{
			plr_addDamagePlayerLife(2);
		}
		if (_enemy->time_prt > 0.30f)
		{

			float angleptr = getAngleBetweenVectors(vector2f(1.f, 0.f), vector2f(-_enemy->forward.x, -_enemy->forward.y));
			angleptr *= RAD2DEG;
			if (_enemy->forward.x > 0.f)
			{
				prt_CreateCircleParticles(vector2f(_enemy->pos.x - 70, _enemy->pos.y + 70.f), 3, 8, sfColor_fromRGBA(137.f, 108.f, 82.f, 200.f), sfColor_fromRGBA(137.f, 108.f, 82.f, 150.f), 0.5f, 8.f, 0.f, angleptr - 25.f, angleptr + 05.f, 65.f, 200.f, 0.3f,
					sfColor_fromRGBA(255.f, 255.f, 255.f, 200.f), sfColor_fromRGBA(255.f, 255.f, 255.f, 150.f), sfTrue, sfFalse, sfFalse, NULL, sfFalse, sfFalse, sfFalse, DUST);
				prt_CreateCircleParticles(vector2f(_enemy->pos.x, _enemy->pos.y + 70.f), 3, 8, sfColor_fromRGBA(137.f, 108.f, 82.f, 200.f), sfColor_fromRGBA(137.f, 108.f, 82.f, 150.f), 0.5f, 8.f, 0.f, angleptr - 25.f, angleptr + 05.f, 65.f, 200.f, 0.3f,
					sfColor_fromRGBA(255.f, 255.f, 255.f, 200.f), sfColor_fromRGBA(255.f, 255.f, 255.f, 150.f), sfTrue, sfFalse, sfFalse, NULL, sfFalse, sfFalse, sfFalse, DUST);
			}
			if (_enemy->forward.x < 0.f)
			{
				prt_CreateCircleParticles(vector2f(_enemy->pos.x + 70, _enemy->pos.y + 70.f), 3, 8, sfColor_fromRGBA(137.f, 108.f, 82.f, 200.f), sfColor_fromRGBA(137.f, 108.f, 82.f, 150.f), 0.5f, 8.f, 0.f, angleptr - 25.f, angleptr + 05.f, 65.f, 200.f, 0.3f,
					sfColor_fromRGBA(255.f, 255.f, 255.f, 200.f), sfColor_fromRGBA(255.f, 255.f, 255.f, 155.f), sfTrue, sfFalse, sfFalse, NULL, sfFalse, sfFalse, sfFalse, DUST);
				prt_CreateCircleParticles(vector2f(_enemy->pos.x, _enemy->pos.y + 70.f), 3, 8, sfColor_fromRGBA(137.f, 108.f, 82.f, 200.f), sfColor_fromRGBA(137.f, 108.f, 82.f, 150.f), 0.5f, 8.f, 0.f, angleptr - 25.f, angleptr + 05.f, 65.f, 200.f, 0.3f,
					sfColor_fromRGBA(255.f, 255.f, 255.f, 200.f), sfColor_fromRGBA(255.f, 255.f, 255.f, 150.f), sfTrue, sfFalse, sfFalse, NULL, sfFalse, sfFalse, sfFalse, DUST);
			}
			_enemy->time_prt = 0.f;
		}
		if (getSqrMagnitudeBetween2Vectorsf(_enemy->pos, _enemy->spawnPos) < rand_float(1000.0f, 30000.0f))
		{
			_enemy->time_detection = 0.0f;
			_enemy->state = EN_STATE_WAIT;
			_enemy->frameX = 0.f;
			_enemy->timerAnim = 0.f;
		}
	}
	else if (_enemy->state == EN_STATE_CHASE)
	{
		_enemy->maxVelocity = 0.f;
		if (_enemy->time_wait > 0.75f)
		{	
			_enemy->forward = normalizef(createVectorWithVectorf(_enemy->pos, _tmpPpos));
			_enemy->time_wait = 0.f;
			_enemy->time_sound = 1.5f;
			_enemy->state = EN_STATE_RUSH;
			_enemy->frameX = 0.f;
			_enemy->timerAnim = 0.f;
			_enemy->startVelocity = NULLVECTF;
			_enemy->currentVelocity = NULLVECTF;
		}
		if ((getSqrMagnitudeBetween2Vectorsf(_enemy->pos, _tmpPpos) > 450.f * 450.f || plr_IsInvisible()))
		{
			_enemy->state = EN_STATE_WAIT;
			_enemy->frameX = 0.f;
			_enemy->timerAnim = 0.f;
		}

		if (getSqrMagnitudeBetween2Vectorsf(_enemy->spawnPos, _tmpPpos) > SPAWN_DIAMETER * 3.f)

		{
			_enemy->state = EN_STATE_BACK_HOME;
			_enemy->frameX = 0.f;
			_enemy->timerAnim = 0.f;
		}
	}
	else if (_enemy->state == EN_STATE_RUSH)
	{
		_enemy->time_sound += _delta;
		if (_enemy->time_sound > 1.5f)
		{
			_enemy->time_sound = 0.f;
			playSoundFX("SFX_Bouc_Running");
		}
		_enemy->maxVelocity = 600.f;
		checkCollisions(_i, _delta);
		if (getSqrMagnitudeBetween2Vectorsf(_enemy->pos, _tmpPpos) < 80.0f * 80.0f)
		{
			plr_addDamagePlayerLife(3);
		}
		if (_enemy->time_prt > 0.30f)
		{

			float angleptr = getAngleBetweenVectors(vector2f(1.f, 0.f), vector2f(-_enemy->forward.x, -_enemy->forward.y));
			angleptr *= RAD2DEG;
			if (_enemy->forward.x > 0.f)
			{
				prt_CreateCircleParticles(vector2f(_enemy->pos.x - 70, _enemy->pos.y + 70.f), 3, 8, sfColor_fromRGBA(137.f, 108.f, 82.f, 200.f), sfColor_fromRGBA(137.f, 108.f, 82.f, 150.f), 0.5f, 8.f, 0.f, angleptr - 25.f, angleptr + 05.f, 65.f, 200.f, 0.3f,
					sfColor_fromRGBA(255.f, 255.f, 255.f, 200.f), sfColor_fromRGBA(255.f, 255.f, 255.f, 200.f), sfTrue, sfFalse, sfFalse, NULL, sfFalse, sfFalse, sfFalse, DUST);
				prt_CreateCircleParticles(vector2f(_enemy->pos.x, _enemy->pos.y + 70.f), 3, 8, sfColor_fromRGBA(137.f, 108.f, 82.f, 200.f), sfColor_fromRGBA(137.f, 108.f, 82.f, 150.f), 0.5f, 8.f, 0.f, angleptr - 25.f, angleptr + 05.f, 65.f, 200.f, 0.3f,
					sfColor_fromRGBA(255.f, 255.f, 255.f, 200.f), sfColor_fromRGBA(255.f, 255.f, 255.f, 200.f), sfTrue, sfFalse, sfFalse, NULL, sfFalse, sfFalse, sfFalse, DUST);
			}
			if (_enemy->forward.x < 0.f)
			{
				prt_CreateCircleParticles(vector2f(_enemy->pos.x + 70, _enemy->pos.y + 70.f), 3, 8, sfColor_fromRGBA(137.f, 108.f, 82.f, 200.f), sfColor_fromRGBA(137.f, 108.f, 82.f, 150.f), 0.5f, 8.f, 0.f, angleptr - 25.f, angleptr + 05.f, 65.f, 200.f, 0.3f,
					sfColor_fromRGBA(255.f, 255.f, 255.f, 200.f), sfColor_fromRGBA(255.f, 255.f, 255.f, 150.f), sfTrue, sfFalse, sfFalse, NULL, sfFalse, sfFalse, sfFalse, DUST);
				prt_CreateCircleParticles(vector2f(_enemy->pos.x, _enemy->pos.y + 70.f), 3, 8, sfColor_fromRGBA(137.f, 108.f, 82.f, 200.f), sfColor_fromRGBA(137.f, 108.f, 82.f, 150.f), 0.5f, 8.f, 0.f, angleptr - 25.f, angleptr + 05.f, 65.f, 200.f, 0.3f,
					sfColor_fromRGBA(255.f, 255.f, 255.f, 200.f), sfColor_fromRGBA(255.f, 255.f, 255.f, 150.f), sfTrue, sfFalse, sfFalse, NULL, sfFalse, sfFalse, sfFalse, DUST);
			}
			_enemy->time_prt = 0.f;
		}
		if (_enemy->time_wait > 2.5f)
		{
			_enemy->time_wait = 0.f;
			_enemy->maxVelocity = 0.f;
			_enemy->state = EN_STATE_CHASE;
			_enemy->frameX = 0.f;
			_enemy->timerAnim = 0.f;
		}
		if (getSqrMagnitudeBetween2Vectorsf(_enemy->pos, _enemy->spawnPos) > SPAWN_DIAMETER * 3.f)
		{
			_enemy->time_sound = 1.5f;
			_enemy->state = EN_STATE_BACK_HOME;
			_enemy->frameX = 0.f;
			_enemy->timerAnim = 0.f;
			_enemy->time_wait = 0.f;
		}
	}
	else if (_enemy->state == EN_STATE_DAMAGING)
	{
		_enemy->time_wait += _delta;
		if (_enemy->time_wait > 0.2f)
		{
			if (_enemy->life <= 0)
			{
				en_Erase(_i, _enemy->pos, FX_EXPLOSION, _window);
			}
			else
			{
				_enemy->state = EN_STATE_WAIT;
				_enemy->frameX = 0;
				_enemy->timerAnim = 0.0f;
				_enemy->time_wait = 0.0f;
			}
		}
	}
	else if (_enemy->state == EN_STATE_THROWN)
	{
		checkCollisions(_i, _delta);
		_enemy->time_wait += _delta;
		if (_enemy->time_wait > 1.f)
		{
			_enemy->time_wait = 0.f;
			_enemy->frameX = 0;
			_enemy->timerAnim = 0.0f;
			_enemy->state = EN_STATE_WAIT;
		}
	}
	else if (_enemy->state == EN_STATE_FALLING2)
	{
		_enemy->time_wait += _delta;

		_enemy->scale.x = 1.f - (_enemy->time_wait / 2.f);
		_enemy->scale.y = 1.f - (_enemy->time_wait / 2.f);
		_enemy->rotationFalling += (_enemy->time_wait / 30.f);
		if (_enemy->time_wait >= 2.f)
		{
			en_Erase(_i, _enemy->pos, FX_STARS, _window);
			return;
		}
	}
}

void en_UpdateSnake(EnemiesStruct* _enemy, EnemiesState _tmpState, int _i, float _delta, sfVector2f _tmpPpos, Window* _window)
{
	_enemy->timerAllAttacks += _delta;

	sfVector2i tmpIndexPos = map_getBlockPos(_enemy->pos);
	tmpIndexPos = vector2i(tmpIndexPos.y, tmpIndexPos.x);
	if (map_HasTileCollisions(tmpIndexPos) == CLD_VOID && _enemy->state != EN_STATE_FALLING2)
	{
		_enemy->time_wait = 0.0f;
		_enemy->time_detection = 0.0f;
		_enemy->state = EN_STATE_FALLING2;
	}

	if (_tmpState == EN_STATE_WAIT)
	{
		_enemy->time_wait += _delta;

		sfBool isPlayerClose = sfFalse;
		if (getSqrMagnitudeBetween2Vectorsf(_enemy->pos, _tmpPpos) < _enemy->radiusDetection && !plr_IsInvisible())
			isPlayerClose = sfTrue;

		if ((_enemy->time_wait > 1.55f && isPlayerClose) || _enemy->time_wait > 4.75f) { // one full anim or three full anim

			sfVector2f tmpPos = _enemy->pos;
			sfVector2i tmpBlockPos = map_getBlockPos(tmpPos);
			int nbMaxPaths = (isPlayerClose ? 8 : 4);

			if (!map_isIndexInArray(tmpBlockPos)) {
				//enemiesList->erase(&enemiesList, _i);
				return;
			}

			if (map_SnakeHasTileCollisions(tmpBlockPos) != CLD_NO_COLL) {
				_enemy->endPos = _enemy->startPos;
				_enemy->startPos = _enemy->pos;
				_enemy->time_detection = 0.f;
				_enemy->time_wait = 0.f;
				_enemy->state = EN_STATE_WALK;
			}
			else {

				sfVector2i* iAllPaths = calloc(nbMaxPaths, sizeof(sfVector2i)); // all tiles around the snake
				iAllPaths[0] = vector2i(tmpBlockPos.x, tmpBlockPos.y - 1); // up
				iAllPaths[1] = vector2i(tmpBlockPos.x, tmpBlockPos.y + 1); // down
				iAllPaths[2] = vector2i(tmpBlockPos.x - 1, tmpBlockPos.y); // left
				iAllPaths[3] = vector2i(tmpBlockPos.x + 1, tmpBlockPos.y); // right

				if (isPlayerClose) {
					iAllPaths[4] = vector2i(tmpBlockPos.x - 1, tmpBlockPos.y - 1); // upper left
					iAllPaths[5] = vector2i(tmpBlockPos.x + 1, tmpBlockPos.y - 1); // upper right
					iAllPaths[6] = vector2i(tmpBlockPos.x - 1, tmpBlockPos.y + 1); // bottom left
					iAllPaths[7] = vector2i(tmpBlockPos.x + 1, tmpBlockPos.y + 1); // bottom right
				}

				sfVector2i* iNonSolidPaths = calloc(nbMaxPaths, sizeof(sfVector2i)); // all non solid existing tiles around the snake
				int nbNonSolidPaths = 0;
				for (int path = 0; path < nbMaxPaths; path++)
				{
					if (map_isIndexInArray(iAllPaths[path])) {
						if (map_SnakeHasTileCollisions(iAllPaths[path]) == CLD_NO_COLL) {
							iNonSolidPaths[nbNonSolidPaths] = iAllPaths[path];
							nbNonSolidPaths++;
						}
					}
				}

				sfVector2f* possiblePaths = calloc(nbNonSolidPaths, sizeof(sfVector2f)); // all possible postions the snake can go to
				int nbPossiblePaths = 0;
				for (int path = 0; path < nbNonSolidPaths; path++)
				{
					sfBool takenPath = sfFalse;
					for (int j = 0; j < enemiesList->size(enemiesList); j++)
					{
						if (_i != j && STD_LIST_GETDATA(enemiesList, EnemiesStruct, j)->type == EN_SNAKE)
						{
							if (equalsVectorsri(iNonSolidPaths[path], map_getBlockPos(STD_LIST_GETDATA(enemiesList, EnemiesStruct, j)->pos)) || equalsVectorsri(iNonSolidPaths[path], map_getBlockPos(STD_LIST_GETDATA(enemiesList, EnemiesStruct, j)->endPos))) {
								takenPath = sfTrue;
								break;
							}
						}
					}
					if (!takenPath) { // if no snake is there or is willing to get there, it is a possible path
						possiblePaths[nbPossiblePaths] = addVectorsrf(map_getWorldPos(iNonSolidPaths[path]), vector2f(TILE_SIZE / 2.f, TILE_SIZE / 2.f));
						nbPossiblePaths++;
					}
				}

				if (nbPossiblePaths > 0) {
					_enemy->startPos = _enemy->pos;

					if (isPlayerClose) { // take the closest non-taken path to the player
						_enemy->endPos = possiblePaths[0];
						float closestMagnintude = getSqrMagnitudeBetween2Vectorsf(possiblePaths[0], _tmpPpos);
						for (int path = 1; path < nbPossiblePaths; path++)
						{
							float tmpMagnitude = getSqrMagnitudeBetween2Vectorsf(possiblePaths[path], _tmpPpos);
							if (tmpMagnitude < closestMagnintude) {
								_enemy->endPos = possiblePaths[path];
								closestMagnintude = tmpMagnitude;
							}
						}
					}
					else // take a random non-taken path
						_enemy->endPos = possiblePaths[rand() % nbPossiblePaths];

					_enemy->state = EN_STATE_WALK;
					_enemy->time_detection = 0.f;

					if (_enemy->endPos.x + EPSILON > _enemy->pos.x)
						_enemy->currentVelocity.x = 1.f;
					if (_enemy->endPos.x - EPSILON < _enemy->pos.x)
						_enemy->currentVelocity.x = -1.f;
				}

				free(iAllPaths);
				free(iNonSolidPaths);
				free(possiblePaths);

				_enemy->time_wait = 0.f;
			}
		}
	}
	else if (_tmpState == EN_STATE_WALK)
	{
		_enemy->time_detection += _delta;

		if (_enemy->time_detection >= 1.f) {
			_enemy->state = EN_STATE_WAIT;
			_enemy->pos = _enemy->endPos;
		}
		else
			_enemy->pos = vec2f_lerp(_enemy->startPos, _enemy->endPos, _enemy->time_detection);
	}
	else if (_tmpState == EN_STATE_ATTACK)
	{
		_enemy->time_wait += _delta;
		_enemy->currentVelocity = vector2f(0.f, 0.f);
		// attack
		if (_enemy->time_wait > 1.f || plr_IsInvisible()) { // one attack anim
			_enemy->time_wait = 0.f;
			_enemy->state = EN_STATE_WAIT;
		}
	}
	else if (_tmpState == EN_STATE_THROWN)
	{
		_enemy->stunTimer -= _delta;

		if (cll_RectMap(FlRect(_enemy->pos.x - _enemy->origin.x, _enemy->pos.y - _enemy->origin.y, _enemy->size.x, _enemy->size.y), multiplyVectorsrf(_enemy->currentVelocity, _delta))) {
			_enemy->stunTimer = 0.f;
			_enemy->endPos = _enemy->pos;
		}
		else
			checkCollisions(_i, _delta);

		if (_enemy->stunTimer <= 0.f)
			_enemy->state = EN_STATE_WAIT;
	}
	else if (_tmpState == EN_STATE_STUNNED)
	{
		_enemy->stunTimer -= _delta;

		if (_enemy->stunTimer <= 0.f)
			_enemy->state = EN_STATE_WAIT;
	}
	else if (_tmpState == EN_STATE_DAMAGING)
	{
		_enemy->time_wait += _delta;
		if (_enemy->time_wait <= 0.1f)
			playSoundFX("SFX_Snake_Damage");
		if (_enemy->time_wait > 0.2f) {
			qst_IncrementCount(QST_ID_S_KILL_SNAKES);
			en_Erase(_i, _enemy->pos, FX_EXPLOSION, _window);
			return;
		}
	}
	if (_enemy->state == EN_STATE_FALLING2)
	{
		_enemy->time_wait += _delta;

		_enemy->scale.x = 1.f - (_enemy->time_wait / 2.f);
		_enemy->scale.y = 1.f - (_enemy->time_wait / 2.f);
		_enemy->rotationFalling += (_enemy->time_wait / 30.f);
		if (_enemy->time_wait >= 2.f)
		{
			en_Erase(_i, _enemy->pos, FX_STARS, _window);
			return;
		}
	}

	EnemiesState currentState = _enemy->state;

	sfFloatRect plrBounds = plr_getPlayerBoundsWithEnemies();
	sfFloatRect enBounds = en_getCollisionsBounds(_enemy);

	// player collisions
	if (_enemy->timerAllAttacks > 0.1f && _enemy->life > 0 && currentState != EN_STATE_ATTACK && currentState != EN_STATE_DAMAGING && currentState != EN_STATE_STUNNED && currentState != EN_STATE_THROWN && !plr_IsInvisible()) {
		if (sfFloatRect_intersects(&plrBounds, &enBounds, NULL) && !plr_IsInvisible()) {
			_enemy->state = EN_STATE_ATTACK;
			_enemy->time_wait = 0.f;
			currentState = EN_STATE_ATTACK;
		}
	}

	if (currentState != _tmpState) {
		_enemy->frameX = 0;
		_enemy->timerAnim = 0.f;
	}

	if (_enemy->timerAllAttacks > 0.1f && _enemy->life > 0 && currentState == EN_STATE_ATTACK && _enemy->frameX == 4 && sfFloatRect_intersects(&plrBounds, &enBounds, NULL) && !plr_IsInvisible()) {
		plr_addDamagePlayerLife(_enemy->damage);
		playSoundFX("SFX_Snake_Attack");
	}
}

void en_UpdateMosquito(EnemiesStruct* _enemy, EnemiesState _tmpState, int _i, float _delta, sfVector2f _tmpPpos, Window* _window)
{
	/*if (_enemy->state != EN_STATE_ATTACK && _enemy->state != EN_STATE_THROWN && _enemy->state != EN_STATE_STUNNED && _enemy->state != EN_STATE_STUNNED) 
	{
		_enemy->time_sound += _delta;
		if(_enemy->time_sound > )
	}*/
	if (_enemy->state == EN_STATE_WALK)
		_enemy->maxVelocity = 200.f;
	else
		_enemy->maxVelocity = 400.0f;

	_enemy->time_wait += _delta;
	if (_enemy->state == EN_STATE_WALK)
	{
		_enemy->maxVelocity = 75.f;
		checkCollisions(_i, _delta);
		if (getSqrMagnitudeBetween2Vectorsf(_enemy->pos, _enemy->spawnPos) > SPAWN_DIAMETER / 1.5)
		{
			_enemy->time_wait = 0.f;
			_enemy->state = EN_STATE_BACK_HOME;
			_enemy->frameX = 0.f;
			_enemy->timerAnim = 0.f;
			return;
		}
		if (_enemy->time_wait > rand_float(7.5f, 12.5f))
		{
			_enemy->time_wait = 0.0f;
			_enemy->forward = vector2f(rand_float(-1, 1), rand_float(-1, 1));
		}
		if (getSqrMagnitudeBetween2Vectorsf(_enemy->pos, _tmpPpos) < 300.f * 300.f && !plr_IsInvisible())
		{
			_enemy->time_wait = 0.f;
			_enemy->state = EN_STATE_CHASE;
			_enemy->frameX = 0.f;
			_enemy->timerAnim = 0.f;
			return;
		}
	}
	else if (_enemy->state == EN_STATE_BACK_HOME)
	{
		_enemy->forward = normalizef(createVectorWithVectorf(_enemy->pos, _enemy->spawnPos));
		_enemy->maxVelocity = 75.f;
		checkCollisions(_i, _delta);
		if (getSqrMagnitudeBetween2Vectorsf(_enemy->pos, _enemy->spawnPos) < rand_float(1000.0f, 30000.0f))
		{
			_enemy->forward = vector2f(rand_float(-1, 1), rand_float(-1, 1));
			_enemy->time_wait = 0.0f;
			_enemy->frameX = 0.f;
			_enemy->timerAnim = 0.f;
			_enemy->state = EN_STATE_WALK;
			return;
		}
	}
	else if (_enemy->state == EN_STATE_CHASE)
	{
		_enemy->forward = normalizef(createVectorWithVectorf(_enemy->pos, _tmpPpos));
		_enemy->maxVelocity = 125.f;
		checkCollisions(_i, _delta);

		if (getSqrMagnitudeBetween2Vectorsf(_enemy->pos, _tmpPpos) > 300.f * 300.f || plr_IsInvisible())
		{
			_enemy->forward = vector2f(rand_float(-1, 1), rand_float(-1, 1));
			_enemy->time_wait = 0.0f;
			_enemy->frameX = 0.f;
			_enemy->timerAnim = 0.f;
			_enemy->state = EN_STATE_WALK;
			return;
		}
		if (getSqrMagnitudeBetween2Vectorsf(_enemy->pos, _tmpPpos) < 100.f * 100.f)
		{
			_enemy->time_wait = 0.0f;
			_enemy->frameX = 0.f;
			_enemy->timerAnim = 0.f;
			_enemy->state = EN_STATE_ATTACK;
			return;
		}
		if (getSqrMagnitudeBetween2Vectorsf(_enemy->pos, _enemy->spawnPos) > SPAWN_DIAMETER / 1.5)
		{
			_enemy->state = EN_STATE_BACK_HOME;
			_enemy->frameX = 0.f;
			_enemy->timerAnim = 0.f;
			_enemy->time_wait = 0.f;
			return;
		}
	}
	else if (_enemy->state == EN_STATE_ATTACK)
	{

		_enemy->forward = normalizef(createVectorWithVectorf(_enemy->pos, _tmpPpos));
		_enemy->maxVelocity = 0.f;
		if (_enemy->time_wait >= .5f)
		{
			playSoundFX("SFX_Mosquito_Idle");
			if (getSqrMagnitudeBetween2Vectorsf(_enemy->pos, _tmpPpos) < 100.f * 100.f)
			{
				plr_addDamagePlayerLife(1);
			}
			_enemy->time_wait = 0.f;
			_enemy->state = EN_STATE_CHASE;
			_enemy->frameX = 0.f;
			_enemy->timerAnim = 0.f;
			return;
		}
		else if (plr_IsInvisible())
		{
			_enemy->time_wait = 0.f;
			_enemy->state = EN_STATE_WALK;
			_enemy->frameX = 0.f;
			_enemy->timerAnim = 0.f;
			return;
		}
	}
	else if (_enemy->state == EN_STATE_STUNNED)
	{
		_enemy->time_wait += _delta;
		if (_enemy->time_wait > 5.f)
		{
			_enemy->frameX = 0;
			_enemy->timerAnim = 0.0f;
			_enemy->time_wait = 0.f;
			_enemy->state = EN_STATE_WALK;
		}
	}
	else if (_enemy->state == EN_STATE_THROWN)
	{
		checkCollisions(_i, _delta);
		_enemy->time_wait += _delta;
		if (_enemy->time_wait > 1.f)
		{
			_enemy->time_wait = 0.f;
			_enemy->frameX = 0;
			_enemy->timerAnim = 0.0f;
			_enemy->state = EN_STATE_WALK;
		}
	}

	if (_enemy->life <= 0)
	{
		if (_enemy->time_wait >= .2f)
		{
			playSoundFX("SFX_Mosquito_Attack");
			en_Erase(_i, _enemy->pos, FX_EXPLOSION, _window);
			return;
		}
	}
}

void en_UpdateMole(EnemiesStruct* _enemy, EnemiesState _tmpState, int _i, float _delta, sfVector2f _tmpPpos, Window* _window)
{

	if (plr_IsInvisible()) //it can makes bug
	{
		if (_enemy->state != EN_STATE_WALK)
		{
			_enemy->rebootFrameX = sfTrue;
			_enemy->haveToHome = sfFalse;
			_enemy->time_wait = 0.0f;
			_enemy->isOutHole = sfFalse;
			if ((_enemy->frameX < 12 && !_enemy->isInHole) || (_enemy->frameX < 12 && _enemy->rebootFrameX2)) {
				//printf("Go in\n");
				_enemy->state = EN_STATE_BACK_HOME;
			}
			else
			{
				_enemy->rebootFrameX2 = sfFalse;
				//printf("In Hole\n");
				_enemy->isInHole = sfTrue;
				_enemy->frameX = 0;
				_enemy->state = EN_STATE_WALK;
			}
		}
		return;
	}

	if (_enemy->state != EN_STATE_DAMAGING)
	{
		if (getSqrMagnitudeBetween2Vectorsf(_enemy->pos, _tmpPpos) > 300.0f * 300.0f /*|| getSqrMagnitudeBetween2Vectorsf(GD_ENEMY->pos, ring_getRingPos()) > 500.0f * 500.0f*/)
		{
			if (!_enemy->haveToHome)
			{
				if (_enemy->isInHole || _enemy->rebootFrameX)
				{
					_enemy->rebootFrameX2 = sfTrue;
					if (_enemy->frameX < 11 && !_enemy->isOutHole)
					{
						//printf("Go out\n");
						if (_enemy->frameX == 0)
							playSoundFX("SFX_Mole_GetOut");
						_enemy->state = EN_STATE_CHASE;
					}
					else
					{
						_enemy->rebootFrameX = sfFalse;
						if (!_enemy->isOutHole)
						{
							_enemy->isOutHole = sfTrue;
							_enemy->frameX = 0;
							_enemy->timerAnim = 0.0f;
						}
						_enemy->isInHole = sfFalse;
					}
				}
				else
				{
					_enemy->time_wait += _delta;
					//printf("Wait\n");
					if (_enemy->time_wait > 2.f)
					{
						_enemy->state = EN_STATE_ATTACK;
						if (_enemy->time_wait > 2.6f)
						{
							_enemy->time_wait = 0.0f;
							playSoundFX("SFX_Mole_Shoot");
							blt_addBullets(_enemy->pos, normalizef(createVectorWithVectorf(_enemy->pos, _tmpPpos)), rand() % 3);
							_enemy->frameX = 0;
							_enemy->timerAnim = 0.0f;
							_enemy->haveToHome = sfTrue;
						}
					}
					else
						_enemy->state = EN_STATE_WAIT;
				}
			}
			else
			{
				_enemy->isOutHole = sfFalse;
				if (_enemy->frameX < 12 && !_enemy->isInHole) {
					if (_enemy->frameX == 0)
						//playSoundFX("SFX_Mole_GetIn");
						_enemy->state = EN_STATE_BACK_HOME;
					//printf("have to go in\n");
				}
				else
				{
					if (!_enemy->isInHole)
					{
						_enemy->isInHole = sfTrue;
						_enemy->frameX = 0;
					}
					if (_enemy->frameX < 11 && !_enemy->isOutHole)
					{
						//printf("have to go out\n");
						if (_enemy->frameX == 0)
							playSoundFX("SFX_Mole_GetOut");
						_enemy->state = EN_STATE_CHASE;
					}
					else
					{
						_enemy->isInHole = sfFalse;
						if (!_enemy->isOutHole)
						{
							_enemy->isOutHole = sfTrue;
							_enemy->frameX = 0;
							_enemy->timerAnim = 0.0f;
							_enemy->haveToHome = sfFalse;
						}
					}
				}
			}
		}
		else
		{
			_enemy->rebootFrameX = sfTrue;
			_enemy->haveToHome = sfFalse;
			_enemy->time_wait = 0.0f;
			_enemy->isOutHole = sfFalse;
			if ((_enemy->frameX < 12 && !_enemy->isInHole) || (_enemy->frameX < 12 && _enemy->rebootFrameX2)) {
				//printf("Go in\n");
				_enemy->state = EN_STATE_BACK_HOME;
			}
			else
			{
				_enemy->rebootFrameX2 = sfFalse;
				//printf("In Hole\n");
				_enemy->isInHole = sfTrue;
				_enemy->frameX = 0;
				_enemy->state = EN_STATE_WALK;
			}
		}
	}
	else
	{
		_enemy->stunTimer += _delta;
		if (_enemy->stunTimer >= 0.2f)
			en_Erase(_i,_enemy->pos,FX_EXPLOSION, _window);
	}

	if (isNutHere() && !_enemy->isInHole)
	{
		sfFloatRect tmpEnenmyRect = en_getBounds(_enemy);
		for (int x = 0; x < bulletsList->size(bulletsList); x++)
		{
			if (sfFloatRect_intersects(&tmpEnenmyRect, &STD_LIST_GETDATA(bulletsList, Bullets_struct, x)->colRect, NULL) && STD_LIST_GETDATA(bulletsList, Bullets_struct, x)->isBack)
			{
				bulletsList->erase(&bulletsList, x);
				_enemy->state = EN_STATE_DAMAGING;
				break;
			}
		}
	}
}

void en_UpdateScolopendre(EnemiesStruct* _enemy, EnemiesState _tmpState, int _i, float _delta, sfVector2f _tmpPpos)
{
	_enemy->time_prt += _delta;
	switch (_tmpState)
	{
	case EN_STATE_HIDDEN:
		_enemy->time_wait += _delta;
		if (_enemy->time_wait > 4.f) {
			_enemy->time_wait = 0.f;
			_enemy->state = EN_STATE_GOINGUP;
		}
		break;
	case EN_STATE_GOINGUP:
		if (_enemy->time_prt > 0.60f)
		{
			prt_CreateCircleParticles(vector2f(_enemy->pos.x + 38.f, _enemy->pos.y), 5, 6, sfColor_fromRGBA(96.f, 48.f, 26.f, 155.f), sfColor_fromRGBA(96.f, 48.f, 26.f, 100.f),
				0.3f, 8.f, 0.f, 0.f, 360.f, 155.f, 250.f, 0.6f, sfColor_fromRGBA(0.f, 0.f, 0.f, 105.f), sfColor_fromRGBA(0.f, 0.f, 0.f, 50.f), sfTrue, sfFalse, sfFalse, NULL, sfFalse, sfFalse, sfFalse, DUST);
			_enemy->time_prt = 0.f;
		}
		_enemy->time_wait += _delta;
		sfBool isHorizontal = en_IsScolopendreHorizontal(_enemy->rotation);
		if (_enemy->time_wait > (isHorizontal ? 1.15f : 1.35)) {
			_enemy->time_wait = 0.f;
			_enemy->state = EN_STATE_WALK;
			_enemy->currentVelocity = en_GetAppropriateVectorRotation(vector2f(-100.f, 0.f), _enemy->rotation);
		}
		break;
	case EN_STATE_WALK:
		_enemy->time_wait += _delta;
		if (_enemy->time_wait > 3.f) {
			_enemy->time_wait = 0.f;
			_enemy->state = EN_STATE_GOINGDOWN;
		}
		else {
			addVectorsf(&_enemy->pos, multiplyVectorsrf(_enemy->currentVelocity, _delta));
		}
		break;
	case EN_STATE_GOINGDOWN:
		if (_enemy->time_prt > 0.60f)
		{
			prt_CreateCircleParticles(vector2f(_enemy->pos.x - 32.f, _enemy->pos.y), 5, 6, sfColor_fromRGBA(96.f, 48.f, 26.f, 155.f), sfColor_fromRGBA(96.f, 48.f, 26.f, 100.f),
				0.3f, 8.f, 0.f, 0.f, 360.f, 155.f, 250.f, 0.6f, sfColor_fromRGBA(0.f, 0.f, 0.f, 105.f), sfColor_fromRGBA(0.f, 0.f, 0.f, 50.f), sfTrue, sfFalse, sfFalse, NULL, sfFalse, sfFalse, sfFalse, DUST);
			_enemy->time_prt = 0.f;
		}
		_enemy->time_wait += _delta;
		if (_enemy->time_wait > 1.95f) {
			_enemy->time_wait = 0.f;
			_enemy->state = EN_STATE_HIDDEN;
			_enemy->pos = _enemy->spawnPos;
		}
		break;
	default:
		break;
	}

	_enemy->collisionsBounds = en_GetAppropriateScolopendreBounds(_enemy);

	sfFloatRect enBounds = en_getCollisionsBounds(_enemy);
	sfFloatRect plrBounds = plr_getPlayerBoundsWithEnemies();
	if (sfFloatRect_intersects(&enBounds, &plrBounds, NULL)) {
		plr_addDamagePlayerLife(_enemy->damage);
		pause_EnnemisDiscovered |= PAUSE_CENTIPEDE;
	}

	EnemiesState currentState = _enemy->state;

	if (currentState != _tmpState) {
		_enemy->frameX = 0;
		_enemy->timerAnim = 0.f;
	}
}

void en_UpdateChameleon(EnemiesStruct* _enemy, EnemiesState _tmpState, int _i, float _delta, sfVector2f _tmpPpos, Window* _window)
{
	sfFloatRect tongueArea = en_GetChameleonTongueArea(_enemy);
	sfVector2f feetPos = plr_GetPos();

	switch (_tmpState)
	{
	case EN_STATE_HIDDEN:
		if (getSqrMagnitudeBetween2Vectorsf(_enemy->pos, _tmpPpos) <= _enemy->radiusDetection && !plr_IsInvisible()) {
			_enemy->frameX = 5;
			_enemy->timerAnim = 0.f;
			_enemy->time_detection = 0.0f;
			_enemy->state = EN_STATE_DETECTING;
			_enemy->time_wait = 0.f;
		}
		break;
	case EN_STATE_DETECTING:
		_enemy->time_wait += _delta;
		if (_enemy->time_wait > 0.55f) {
			_enemy->time_wait = 0.f;
			_enemy->state = EN_STATE_WAIT;
		}
		break;
	case EN_STATE_WAIT:
		_enemy->time_wait += _delta;
		if (sfFloatRect_contains(&tongueArea, feetPos.x, feetPos.y) && !plr_IsInvisible()) {
			_enemy->time_wait = 0.f;
			_enemy->state = EN_STATE_ATTACK;
		}
		else if (getSqrMagnitudeBetween2Vectorsf(_enemy->pos, _tmpPpos) > _enemy->radiusDetection || plr_IsInvisible()) {
			_enemy->time_detection = 0.0f;
			_enemy->state = EN_STATE_BACK_HOME;
			_enemy->time_wait = 0.f;
		}
		break;
	case EN_STATE_ATTACK:
		_enemy->time_wait += _delta;
		if (_enemy->time_wait > 0.325f) {
			_enemy->time_wait = 0.f;
			_enemy->state = EN_STATE_LAUNCH_TONGUE;
		}
		break;
	case EN_STATE_LAUNCH_TONGUE:
		_enemy->time_wait += _delta;
		if (_enemy->time_wait > 1.f) {
			_enemy->time_wait = 0.f;
			_enemy->state = EN_STATE_BACK_TONGUE;
			if (plr_getPlayerIsPushed())
				plr_setPlayerIsPushed(sfFalse);
		}
		else {
			sfFloatRect tongueBounds = en_GetChameleonTongueBounds(_enemy);
			if (sfFloatRect_contains(&tongueBounds, feetPos.x, feetPos.y)) {
				plr_setPlayerIsPushed(sfTrue);
				plr_setPlayerVelocity(en_GetAppropriateVectorRotation(vector2f(0.f, 400.f), _enemy->rotation));
			}
		}
		break;
	case EN_STATE_BACK_TONGUE:
		_enemy->time_wait += _delta;
		if (_enemy->time_wait > 0.5f) {
			_enemy->time_wait = 0.f;
			if (sfFloatRect_contains(&tongueArea, feetPos.x, feetPos.y))
				_enemy->state = EN_STATE_LAUNCH_TONGUE;
			else {
				_enemy->state = EN_STATE_BACK_ATTACK;
				_enemy->frameX = 7;
			}
		}
		break;
	case EN_STATE_BACK_ATTACK:
		_enemy->time_wait += _delta;
		if (_enemy->time_wait > 0.325f) {
			_enemy->time_wait = 0.f;
			_enemy->state = EN_STATE_WAIT;
		}
		break;
	case EN_STATE_BACK_HOME:
		_enemy->time_wait += _delta;
		if (_enemy->time_wait > 0.55f) {
			_enemy->time_wait = 0.f;
			_enemy->state = EN_STATE_HIDDEN;
		}
		break;
	case EN_STATE_DAMAGING:
		_enemy->time_wait += _delta;
		if (_enemy->time_invincibility > 0.5f) {
			_enemy->time_wait = 0.f;
			if (_enemy->life > 0) {
				_enemy->state = EN_STATE_WAIT;
				_enemy->time_invincibility = 0.6f;
			}
			else {
				en_Erase(_i, _enemy->pos, FX_EXPLOSION, _window);
				return;
			}
		}
		break;
	default:
		_enemy->state = EN_STATE_WAIT;
		_enemy->time_wait = 0.f;
		break;
	}

	EnemiesState currentState = _enemy->state;

	sfFloatRect plrBounds = plr_getPlayerBoundsWithEnemies();
	sfFloatRect enBounds = en_getCollisionsBounds(_enemy);

	if (currentState != _tmpState && currentState != EN_STATE_DETECTING && currentState != EN_STATE_BACK_ATTACK) {
		_enemy->frameX = 0;
		_enemy->timerAnim = 0.f;
	}

	if (_enemy->life > 0 && currentState != EN_STATE_DAMAGING && currentState != EN_STATE_STUNNED && sfFloatRect_intersects(&plrBounds, &enBounds, NULL) && !plr_IsInvisible()) {
		plr_addDamagePlayerLife(_enemy->damage);
	}
}

void en_UpdateBat(EnemiesStruct* _enemy, EnemiesState _tmpState, int _i, float _delta, sfVector2f _tmpPpos, Window* _window)
{
	if (_enemy->state == EN_STATE_CHASE && _enemy->isOnRoof)
	{
		_enemy->time_sound += _delta;
		if (_enemy->time_sound > 8.f)
		{
			_enemy->time_sound = 0.0f;
			playSoundFX("SFX_Bat_Sleep");
		}
	}
	if (_enemy->state == EN_STATE_WALK || _enemy->state == EN_STATE_CHASE)
	{
		_enemy->time_sound += _delta;
		if (_enemy->time_sound > 2.f)
		{
			_enemy->time_sound = 0.0f;
			playSoundFX("SFX_Bat_Fly");
		}
	}
	_enemy->time_wait += _delta;

	if (_enemy->state != EN_STATE_CHASE)
		_enemy->forward = vec2f_lerp(_enemy->targetForward, _enemy->forward, 0.2f * _delta);
	else
		_enemy->forward = vec2f_lerp(_enemy->targetForward, _enemy->forward, 0.1f * _delta);

	if (_enemy->state == EN_STATE_WAIT)
	{
		if (getSqrMagnitudeBetween2Vectorsf(_enemy->pos, _tmpPpos) < 300.f * 300.f && !plr_IsInvisible() && _enemy->isOnRoof)
		{
			_enemy->time_wait = 0.f;
			_enemy->isOnRoof = sfFalse;
			_enemy->frameX = 0.f;
			_enemy->timerAnim = 0.f;
		}
		if (!_enemy->isOnRoof && _enemy->time_wait >= 1.2f)
		{
			_enemy->time_sound = 0.0f;
			sfSound_stop(GetSound("SFX_Bat_Sleep"));
			_enemy->time_wait = 0.f;
			_enemy->state = EN_STATE_CHASE;
			_enemy->frameX = 0.f;
			_enemy->timerAnim = 0.f;
			return;
		}
	}
	else if (_enemy->state == EN_STATE_CHASE)
	{
		_enemy->rotation = getAngleBetweenVectors(NULLVECTF, createVectorWithVectorf(_enemy->pos, _tmpPpos)) * RAD2DEG;
		_enemy->targetForward = normalizef(createVectorWithVectorf(_enemy->pos, _tmpPpos));
		_enemy->maxVelocity = 375.f;
		checkCollisions(_i, _delta);
		if (getSqrMagnitudeBetween2Vectorsf(_enemy->pos, _tmpPpos) < 80.f * 80.f || plr_IsInvisible())
		{
			plr_addDamagePlayerLife(2);
		}
		if (getSqrMagnitudeBetween2Vectorsf(_enemy->pos, _tmpPpos) > 300.f * 300.f || plr_IsInvisible())
		{
			_enemy->targetForward = normalizef(vector2f(rand_float(-1.f, 1.f), rand_float(-1.f, 1.f)));
			_enemy->time_wait = 0.0f;
			_enemy->frameX = 0.f;
			_enemy->timerAnim = 2.f;
			_enemy->state = EN_STATE_WALK;
			return;
		}
		if (getSqrMagnitudeBetween2Vectorsf(_enemy->pos, _enemy->spawnPos) > SPAWN_DIAMETER * 1.5)
		{
			_enemy->state = EN_STATE_BACK_HOME;
			_enemy->frameX = 0.f;
			_enemy->timerAnim = 0.f;
			_enemy->time_wait = 0.f;
			return;
		}
	}
	else if (_enemy->state == EN_STATE_BACK_HOME)
	{
		_enemy->maxVelocity = 125.f;
		_enemy->targetForward = normalizef(createVectorWithVectorf(_enemy->pos, _enemy->spawnPos));
		checkCollisions(_i, _delta);
		if (getSqrMagnitudeBetween2Vectorsf(_enemy->pos, _enemy->spawnPos) < 200.f * 200.0f)
		{
			_enemy->targetForward = normalizef(vector2f(rand_float(-1.f, 1.f), rand_float(-1.f, 1.f)));
			_enemy->time_wait = 0.0f;
			_enemy->frameX = 0.f;
			_enemy->timerAnim = 0.f;
			_enemy->state = EN_STATE_WALK;
			return;
		}
		if (getSqrMagnitudeBetween2Vectorsf(_enemy->pos, _tmpPpos) < 300.f * 300.f && !plr_IsInvisible() && _enemy->time_wait > 2.f)
		{
			_enemy->time_wait = 0.0f;
			_enemy->frameX = 0.f;
			_enemy->timerAnim = 0.f;
			_enemy->state = EN_STATE_CHASE;
			return;
		}
	}
	else if (_enemy->state == EN_STATE_WALK)
	{
		_enemy->maxVelocity = 125.f;
		checkCollisions(_i, _delta);
		if (getSqrMagnitudeBetween2Vectorsf(_enemy->pos, _tmpPpos) < 300.f * 300.f && !plr_IsInvisible())
		{
			_enemy->time_wait = 0.0f;
			_enemy->frameX = 0.f;
			_enemy->timerAnim = 0.f;
			_enemy->state = EN_STATE_CHASE;
			return;
		}
		if (getSqrMagnitudeBetween2Vectorsf(_enemy->pos, _enemy->spawnPos) > SPAWN_DIAMETER * 1.5)
		{
			_enemy->state = EN_STATE_BACK_HOME;
			_enemy->frameX = 0.f;
			_enemy->timerAnim = 0.f;
			_enemy->time_wait = 0.f;
			return;
		}
		if (_enemy->time_wait >= 3.f)
		{
			_enemy->targetForward = normalizef(vector2f(rand_float(-1.f, 1.f), rand_float(-1.f, 1.f)));
			_enemy->time_wait = 0.0f;
			return;
		}
	}
	else if (_enemy->state == EN_STATE_ATTACK)
	{
		//attack
		_enemy->targetForward = normalizef(createVectorWithVectorf(_enemy->pos, _tmpPpos));
		_enemy->maxVelocity = 0.f;
		if (_enemy->time_wait >= 1.f)
		{
			_enemy->time_wait = 0.f;
			_enemy->state = EN_STATE_CHASE;
			_enemy->frameX = 0.f;
			_enemy->timerAnim = 0.f;
			return;
		}
		else if (getSqrMagnitudeBetween2Vectorsf(_enemy->pos, _tmpPpos) > 300.f * 300.f || plr_IsInvisible())
		{
			_enemy->time_wait = 0.f;
			_enemy->targetForward = normalizef(vector2f(rand_float(-1.f, 1.f), rand_float(-1.f, 1.f)));
			_enemy->state = EN_STATE_WALK;
			_enemy->frameX = 0.f;
			_enemy->timerAnim = 0.f;
			return;
		}
	}
	if (_enemy->life <= 0)
	{
		if (_enemy->time_wait >= .2f)
		{
			sfSound_stop(GetSound("SFX_Bat_Fly"));
			en_Erase(_i, _enemy->pos, FX_EXPLOSION, _window);
			return;
		}
	}
}

void en_UpdateLittleGhostJoy(EnemiesStruct* _enemy, EnemiesState _tmpState, int _i, float _delta, sfVector2f _tmpPpos, Window* _window)
{
	if(_enemy->state != EN_STATE_CHASE)
		_enemy->maxVelocity = 100.f;
	else
		_enemy->maxVelocity = 200.f;

	_enemy->time_prt += _delta;
	if (_enemy->time_prt > 0.60f)
	{
		prt_CreateCircleParticles(vector2f(_enemy->pos.x, _enemy->pos.y + 30.f), 5, 4, sfColor_fromRGBA(248.f, 242.f, 46.f, 155.f), sfColor_fromRGBA(248.f, 242.f, 46.f, 100.f),
			0.9f, 8.f, 0.f, 200.f, 360.f, 65.f, 250.f, 0.6f, sfColor_fromRGBA(255.f, 255.f, 255.f, 105.f), sfColor_fromRGBA(255.f, 255.f, 255.f, 50.f), sfFalse, sfTrue, sfFalse, NULL, sfFalse, sfFalse, sfFalse, DUST);
		_enemy->time_prt = 0.f;
	}

	switch (_tmpState)
	{
	case EN_STATE_WALK:
		_enemy->time_wait += _delta;
		if (_enemy->time_wait > 5.f)
		{
			_enemy->targetForward = normalizef(vector2f(rand_float(-1.f, 1.f), rand_float(-1.f, 1.f)));
			_enemy->time_wait = 0.0f;
		}
		break;
	case EN_STATE_CHASE:
		_enemy->targetForward = normalizef(createVectorWithVectorf(_enemy->pos, _tmpPpos));

		if (getSqrMagnitudeBetween2Vectorsf(_enemy->pos, _tmpPpos) > 300.0f * 300.0f)
		{
			_enemy->state = EN_STATE_WALK;
			_enemy->time_wait = 6.0f;
		}
		else if (getSqrMagnitudeBetween2Vectorsf(_enemy->pos, _tmpPpos) < 100.0f * 100.0f)
		{
			_enemy->state = EN_STATE_ATTACK;
			_enemy->frameX = 0;
			_enemy->timerAnim = 0.0f;
			_enemy->time_wait = 0.0f;
		}
		break;
	case EN_STATE_ATTACK:
		_enemy->time_wait += _delta;
		if (_enemy->time_wait > 0.7f)
		{
			if (getSqrMagnitudeBetween2Vectorsf(_enemy->pos, _tmpPpos) < 100.0f * 100.0f)
			{
				plr_addDamagePlayerLife(1);
			}
			_enemy->state = EN_STATE_WALK;
			_enemy->frameX = 0.0f;
			_enemy->timerAnim = 0.0f;
			_enemy->time_wait = 0.0f;
		}

		break;
	case EN_STATE_DAMAGING:
		if (_enemy->time_invincibility > 0.2f)
		{
			_enemy->state = EN_STATE_DEAD;
			_enemy->time_wait = 0.0f;
			_enemy->frameX = 0;
			_enemy->timerAnim = 0.0f;
		}
		break;
	case EN_STATE_STUNNED:
		_enemy->currentVelocity = vector2f(0.0f, 0.0f);
		break;
	case EN_STATE_THROWN:
		checkCollisions(_i, _delta);
		_enemy->time_wait += _delta;
		if (_enemy->time_wait > 1.f) {
			_enemy->time_wait = 0.f;
			_enemy->state = EN_STATE_WALK;
		}
		break;
	case EN_STATE_DEAD:
		_enemy->time_wait += _delta;
		if (_enemy->time_wait <= 0.1f)
			playSoundFX("SFX_LittleGhost_Die");
		if (_enemy->time_wait > 1.2f) {
			en_Erase(_i, _enemy->pos, FX_EXPLOSION, _window);
			return;
		}
		break;
	}

	if (getSqrMagnitudeBetween2Vectorsf(_enemy->pos, _tmpPpos) < 300.0f * 300.0f && _enemy->state != EN_STATE_STUNNED && _enemy->state != EN_STATE_CHASE && _enemy->state != EN_STATE_ATTACK && _enemy->state != EN_STATE_DEAD && _enemy->state != EN_STATE_DAMAGING && _enemy->state != EN_STATE_THROWN)
	{
		playSoundFX("SFX_LittleGhost_Detect");
		_enemy->state = EN_STATE_CHASE;
	}



	_enemy->forward = vec2f_lerp(_enemy->targetForward, _enemy->forward, 0.2 * _delta);
	if (_enemy->state != EN_STATE_ATTACK && _enemy->state != EN_STATE_DEAD && _enemy->state != EN_STATE_DAMAGING && _enemy->state != EN_STATE_THROWN)
		checkCollisions(_i, _delta);
}

void en_UpdateBigGhostJoy(EnemiesStruct* _enemy, EnemiesState _tmpState, int _i, float _delta, sfVector2f _tmpPpos, Window* _window)
{
	_enemy->time_prt += _delta;
	if (_enemy->time_prt > 0.60f)
	{
		prt_CreateCircleParticles(vector2f(_enemy->pos.x, _enemy->pos.y + 30.f), 5, 4, sfColor_fromRGBA(248.f, 242.f, 46.f, 155.f), sfColor_fromRGBA(248.f, 242.f, 46.f, 100.f),
			0.9f, 8.f, 0.f, 200.f, 360.f, 65.f, 250.f, 0.6f, sfColor_fromRGBA(255.f, 255.f, 255.f, 105.f), sfColor_fromRGBA(255.f, 255.f, 255.f, 50.f), sfFalse, sfTrue, sfFalse, NULL, sfFalse, sfFalse, sfFalse, DUST);
		_enemy->time_prt = 0.f;
	}
	switch (_tmpState)
	{
	case EN_STATE_WAIT:
		_enemy->time_wait += _delta;
		if (_enemy->time_wait > 0.55f) { // one anim
			_enemy->time_wait = 0.f;
			_enemy->targetForward = normalizef(vector2f(rand_float(-1.f, 1.f), rand_float(-1.f, 1.f)));
			_enemy->state = EN_STATE_WALK;
		}
		break;
	case EN_STATE_DETECTING:
		_enemy->time_wait += _delta;
		_enemy->forward = normalizef(createVectorWithVectorf(_enemy->pos, _tmpPpos));
		_enemy->currentVelocity = NULLVECTF;
		if (_enemy->time_wait > 0.55f && getSqrMagnitudeBetween2Vectorsf(_enemy->pos, _tmpPpos) < _enemy->radiusDetectionSQRT * _enemy->radiusDetectionSQRT) { // one anim
			_enemy->time_wait = 0.f;
			playSoundFX("SFX_BigGhost_Detect");
			_enemy->timerAllAttacks = 0.f;
			_enemy->state = EN_STATE_CHASE;
		}
		break;
	case EN_STATE_WALK:
		_enemy->forward = vec2f_lerp(_enemy->forward, _enemy->targetForward, 0.8f * _delta);
		_enemy->time_wait += _delta;
		if (_enemy->time_wait > 1.55f) {
			_enemy->targetForward = normalizef(vector2f(rand_float(-1.f, 1.f), rand_float(-1.f, 1.f)));
			_enemy->time_wait = 0.f;
		}
		break;
	case EN_STATE_CHASE:
		_enemy->targetForward = normalizef(createVectorWithVectorf(_enemy->pos, _tmpPpos));
		_enemy->forward = vec2f_lerp(_enemy->forward, _enemy->targetForward, 5.f * _delta);

		_enemy->timerAllAttacks += _delta;

		if (_enemy->timerAllAttacks > 3.f) {
			_enemy->timerAllAttacks = 0.f;
			_enemy->state = EN_STATE_ATTRACTING;
			plr_setPlayerIsPushed(sfTrue);
		}
		break;
	case EN_STATE_ATTRACTING:
		if (getSqrMagnitudeBetween2Vectorsf(_enemy->pos, _tmpPpos) < 100.f)
			plr_setPlayerVelocity(NULLVECTF);
		else
			plr_setPlayerVelocity(multiplyVectorsrf(normalizef(createVectorWithVectorf(_tmpPpos, _enemy->pos)), 300.f));

		_enemy->timerAllAttacks += _delta;

		if (_enemy->timerAllAttacks > 0.65f) {
			plr_setPlayerIsPushed(sfFalse);
			_enemy->timerAllAttacks = 0.f;
			_enemy->time_wait = 0.f;
			_enemy->state = EN_STATE_WAIT;
		}
		break;
	case EN_STATE_PLR_RUSHING:
		_enemy->time_wait += _delta;
		if (_enemy->time_wait > 0.2f) {
			_enemy->time_wait = 0.f;
			_enemy->state = EN_STATE_STUNNED;
		}
		break;
	case EN_STATE_THROWN:
		_enemy->time_wait += _delta;
		if (_enemy->time_wait > 1.f) {
			_enemy->time_wait = 0.f;
			_enemy->state = EN_STATE_WAIT;
		}
		break;
	case EN_STATE_STUNNED:
		_enemy->time_wait += _delta;
		if (_enemy->time_wait > 2.95f) {
			_enemy->time_wait = 0.f;
			_enemy->state = EN_STATE_WAIT;
		}
		break;
	case EN_STATE_DAMAGING:
		if (_enemy->time_invincibility > 0.2f) {
			_enemy->time_wait = 0.f;
			if (_enemy->life <= 0) {
				_enemy->time_invincibility = 0.f;
				_enemy->frameX = 0;
				_enemy->state = EN_STATE_DEAD;
			}
			else {
				_enemy->time_invincibility = 0.6f;
				_enemy->state = EN_STATE_WAIT;
			}
		}
		break;
	case EN_STATE_DEAD:
		_enemy->time_wait += _delta;
		if (_enemy->time_wait < 0.1)
			playSoundFX("SFX_BigGhost_Die");

		if (_enemy->time_wait > 0.95f) {
			en_Erase(_i, _enemy->pos, FX_EXPLOSION, _window);
			return;
		}
		break;
	default:
		break;
	}

	if (_enemy->life > 0) {
		EnemiesState currentState = _enemy->state;

		sfFloatRect plrBounds = plr_getPlayerBoundsWithEnemies();
		sfFloatRect enBounds = en_getCollisionsBounds(_enemy);

		if (currentState != EN_STATE_PLR_RUSHING && plr_IsRushing() && sfFloatRect_intersects(&plrBounds, &enBounds, NULL) && !plr_IsInvisible()) {
			_enemy->state = EN_STATE_PLR_RUSHING;
			_enemy->hasBeenRushed = sfTrue;
			_enemy->time_wait = 0.f;
			currentState = _enemy->state;
		}

		if (getSqrMagnitudeBetween2Vectorsf(_enemy->pos, _tmpPpos) < _enemy->radiusDetection && !plr_IsInvisible()) {
			if (currentState == EN_STATE_WAIT || currentState == EN_STATE_WALK) {
				_enemy->time_wait = 0.f;
				_enemy->state = EN_STATE_DETECTING;
				currentState = _enemy->state;
			}
		}
		else if (currentState == EN_STATE_DETECTING || currentState == EN_STATE_CHASE) {
			_enemy->state = EN_STATE_WAIT;
			_enemy->time_wait = 0.f;
			currentState = _enemy->state;
		}

		if (currentState != EN_STATE_WAIT && currentState != EN_STATE_ATTRACTING && currentState != EN_STATE_DETECTING && currentState != EN_STATE_PLR_RUSHING && currentState != EN_STATE_STUNNED && currentState != EN_STATE_DAMAGING)
			checkCollisions(_i, _delta);

		if (_tmpState != currentState) {
			_enemy->frameX = 0;
			_enemy->timerAnim = 0.f;
		}

		if (sfFloatRect_intersects(&plrBounds, &enBounds, NULL) && !plr_IsInvisible() && currentState != EN_STATE_STUNNED && currentState != EN_STATE_PLR_RUSHING && currentState != EN_STATE_DAMAGING) {
			plr_addDamagePlayerLife(_enemy->damage);
		}
	}
}

void en_UpdateLittleGhostSadness(EnemiesStruct* _enemy, EnemiesState _tmpState, int _i, float _delta, sfVector2f _tmpPpos, Window* _window)
{
	//TODO DELETE WHEN PLAYER IS OK
	///
	if (customKeyboardIsKeyPressed(sfKeyJ, NULL) && !_enemy->haveToHome)
	{
		_enemy->haveToHome = sfTrue;
		_enemy->state = EN_STATE_CHASE;
	}
	if (_enemy->haveToHome)
	{
		_enemy->timerAllAttacks += _delta;
		if (_enemy->timerAllAttacks > 50.f)
		{
			_enemy->timerAllAttacks = 0.0f;
			_enemy->state = EN_STATE_WAIT;
			_enemy->haveToHome = sfFalse;
		}
	}
	///

	sfFloatRect tmpPBound = plr_getPlayerBoundsWithEnemies();
	if (sfFloatRect_intersects(&_enemy->collisionsBounds, &tmpPBound, NULL))
	{
		plr_addDamagePlayerLife(1);
	}

	if (_enemy->state != EN_STATE_DAMAGING && _enemy->state != EN_STATE_DEAD)
		_enemy->timerTears += _delta;
	if (_enemy->timerTears > 1.f)
	{
		if (_enemy->currentVelocity.x < 0)
			dbfT_addTearDebuff(vector2f(_enemy->pos.x, _enemy->pos.y + 40.0f));
		else
			dbfT_addTearDebuff(vector2f(_enemy->pos.x - 40.0f, _enemy->pos.y + 35.0f));
		_enemy->timerTears = 0.0f;
	}

	if (_enemy->state != EN_STATE_THROWN)
	{

		if (_enemy->state == EN_STATE_CHASE)
			_enemy->forward = vec2f_lerp(_enemy->targetForward, _enemy->forward, 0.1 * _delta);
		else
			_enemy->forward = vec2f_lerp(_enemy->targetForward, _enemy->forward, 0.2 * _delta);
	}

	if (_enemy->state != EN_STATE_ATTACK && _enemy->state != EN_STATE_DAMAGING && _enemy->state != EN_STATE_DEAD /*&& _enemy->state != EN_STATE_THROWN*/ && _enemy->state != EN_STATE_STUNNED)
		checkCollisions(_i, _delta);

	switch (_tmpState)
	{
	case EN_STATE_WALK:
		//_enemy->time
		_enemy->time_wait += _delta;
		if (_enemy->time_wait > 5.f)
		{
			_enemy->targetForward = normalizef(vector2f(rand_float(-1.f, 1.f), rand_float(-1.f, 1.f)));
			_enemy->time_wait = 0.0f;
		}
		break;
	case EN_STATE_WAIT:
		_enemy->time_wait += _delta;
		if (_enemy->time_wait > 2.f)
		{
			_enemy->maxVelocity = 30.0f;
			_enemy->time_wait = 0.0f;
			_enemy->state = EN_STATE_WALK;
		}
		break;
	case EN_STATE_CHASE:
		_enemy->targetForward = normalizef(createVectorWithVectorf(_enemy->pos, _tmpPpos));
		_enemy->maxVelocity = 50.0f;
		if (getSqrMagnitudeBetween2Vectorsf(_enemy->pos, _tmpPpos) < 100.0f * 100.0f)
		{
			_enemy->state = EN_STATE_ATTACK;
			_enemy->time_wait = 0.0f;
			_enemy->frameX = 0.0f;
			_enemy->timerAnim = 0.0f;
		}

		//if plus de debuff  ->  state wait;
		break;
	case EN_STATE_ATTACK:
		_enemy->time_wait += _delta;
		if (_enemy->time_wait > 1.4f)
		{
			if (getSqrMagnitudeBetween2Vectorsf(_enemy->pos, _tmpPpos) < 100.0f * 100.0f)
			{
				plr_addDamagePlayerLife(1);
			}
			_enemy->state = EN_STATE_CHASE;
			_enemy->frameX = 0;
			_enemy->timerAnim = 0.0f;
		}
		break;
	case EN_STATE_THROWN:
		_enemy->time_wait += _delta;
		if (_enemy->time_wait > 2.f)
		{
			_enemy->time_wait = 0.0f;
			_enemy->state = EN_STATE_WALK;
		}
		break;
	case EN_STATE_DAMAGING:
		if (!_enemy->isOnePassedPulse)
		{
			_enemy->isOnePassedPulse = sfTrue;
			_enemy->frameX = 0;
			_enemy->timerAnim = 0.0f;
			_enemy->time_invincibility = 0.0f;
		}
		_enemy->time_invincibility += _delta;
		if (_enemy->time_invincibility > .2f)
		{
			_enemy->time_invincibility = 0.0f;
			_enemy->state = EN_STATE_DEAD;
		}
		break;
	case EN_STATE_DEAD:
		_enemy->time_invincibility += _delta;
		if (_enemy->time_invincibility > 1.2f)
		{
			en_Erase(_i, _enemy->pos, FX_EXPLOSION, _window);
			return;
		}
		break;
	}
}

void en_UpdateBigGhostSadness(EnemiesStruct* _enemy, EnemiesState _tmpState, int _i, float _delta, sfVector2f _tmpPpos, Window* _window)
{

	if (_enemy->state == EN_STATE_WALK)
	{
		_enemy->timerAllAttacks += _delta;
		_enemy->time_invincibility -= _delta;
		if (_enemy->timerAllAttacks >= 5.0f)
		{
			_enemy->timerAllAttacks = 0.0f;
			_enemy->timerTears = 0.0f;
			if (!plr_getPlayerIsPushed())
				_enemy->state = EN_STATE_ATTRACTING;
		}

		_enemy->time_wait += _delta;
		if (_enemy->time_wait > 2.f)
		{
			_enemy->time_wait = 0.0f;
			_enemy->state = EN_STATE_WAIT;
		}
	}
	else if (_enemy->state == EN_STATE_WAIT)
	{

		_enemy->targetForward = normalizef(vector2f(rand_float(-1.0f, 1.0f), rand_float(-1.0f, 1.0f)));
		_enemy->state = EN_STATE_WALK;
	}
	else if (_enemy->state == EN_STATE_ATTRACTING)
	{
		if (!_enemy->isOnePassedPulse)
		{
			_enemy->isOnePassedPulse = sfTrue;
			_enemy->time_detection = 0.0f;
			plr_setPlayerIsPushed(sfTrue);
			plr_setPlayerVelocity(vector2f(0.0f, 0.0f));
		}
		_enemy->time_detection += _delta;
		if (/*!isCollisionPlayerBox() &&*/ !cll_RectMap(plr_getPlayerBounds(), vector2f(plr_getPlayerVelocity().x * _delta, plr_getPlayerVelocity().y * _delta)))
			plr_setPlayerVelocity(multiplyVectorsrf(normalizef(createVectorWithVectorf(_enemy->pos, _tmpPpos)), 300.0f));
		else
		{
			plr_setPlayerVelocity(vector2f(0.0f, 0.0f));
			plr_setPlayerIsPushed(sfFalse);
		}
		if (_enemy->time_detection / 1.0f >= 1)
		{
			plr_setPlayerIsPushed(sfFalse);
			_enemy->state = EN_STATE_WAIT;
			_enemy->isOnePassedPulse = sfFalse;
			_enemy->time_detection = 0.0f;
			_enemy->frameX = 0;
			_enemy->timerAnim = 0.0f;
		}
	}
	else if (_enemy->state == EN_STATE_CRYING)
	{
		_enemy->timerAllAttacks += _delta;
		if (_enemy->timerAllAttacks >= 10.0f)
		{
			_enemy->timerAllAttacks = 0.0f;
			_enemy->time_invincibility = 2.f;
			_enemy->state = EN_STATE_WAIT;
		}

		_enemy->targetForward = normalizef(createVectorWithVectorf(_enemy->pos, _tmpPpos));
		_enemy->timerAllAttacks += _delta;

		_enemy->timerTears += _delta;

		if (_enemy->timerTears > 0.2f)
		{
			sfVector2f posTearsDestination = polarCoord(_enemy->pos, rand_float(32.0f, 300.0f), rand_float(0.0f, 360.0f));
			trs_addTears(vector2f(rand_float(_enemy->pos.x + 5.f, _enemy->pos.x + 30.f), _enemy->pos.y), posTearsDestination, 2.0, SMALL_TEAR);

			_enemy->timerTears = 0.0f;
		}

		//if the player is close

		if (getSqrMagnitudeBetween2Vectorsf(_enemy->pos, _tmpPpos) < 100.0f * 100.0f)
		{
			if (/*!isCollisionPlayerBox() &&*/ !cll_RectMap(plr_getPlayerBounds(), vector2f(plr_getPlayerVelocity().x * _delta, plr_getPlayerVelocity().y * _delta)))
			{
				plr_setPlayerVelocity(multiplyVectorsrf(normalizef(createVectorWithVectorf(_enemy->pos, _tmpPpos)), 300.0f));
				plr_setPlayerIsPushed(sfTrue);
			}
		}
		else
			plr_setPlayerIsPushed(sfFalse);
	}
	else if (_enemy->state == EN_STATE_THROWN)
	{
		_enemy->time_wait += _delta;
		if (_enemy->time_wait <= EN_THROWN_DURATION)
			checkCollisions(_i, _delta);
		else if (_enemy->time_wait >= 3.f)
			_enemy->state = EN_STATE_WAIT;
	}
	else if (_enemy->state == EN_STATE_STUNNED)
	{
		_enemy->stunTimer -= _delta;

		if (_enemy->stunTimer <= 0.f)
			_enemy->state = EN_STATE_WAIT;
	}
	else if (_enemy->state == EN_STATE_DAMAGING)
	{
		if (!_enemy->isOnePassedPulse)
		{
			_enemy->time_invincibility = 0.0f;
			_enemy->isOnePassedPulse = sfTrue;
		}
		_enemy->time_invincibility += _delta;
		if (_enemy->time_invincibility > 0.2f) {
			_enemy->time_wait = 0.f;
			if (_enemy->life <= 0) {
				_enemy->time_invincibility = 0.f;
				_enemy->frameX = 0;
				_enemy->state = EN_STATE_DEAD;
			}
			else {
				_enemy->isOnePassedPulse = sfFalse;
				_enemy->time_invincibility = 0.0f;
				_enemy->state = EN_STATE_WAIT;
			}
		}
	}
	else if (_enemy->state == EN_STATE_DEAD)
	{
		_enemy->time_invincibility += _delta;
		if (_enemy->time_invincibility > 2.0f)
		{
			en_Erase(_i,_enemy->pos, FX_EXPLOSION, _window);
			return;
		}
	}

	//If the player is close of the ennemy, the ennemy use Pulse
	if (getSqrMagnitudeBetween2Vectorsf(_enemy->pos, _tmpPpos) < 100.0f * 100.0f && _enemy->state != EN_STATE_CRYING && _enemy->state != EN_STATE_CHASE && _enemy->state != EN_STATE_THROWN && _enemy->life > 0)
	{
		_enemy->state = EN_STATE_CHASE;
		_enemy->time_detection = 0.0f;
		plr_setPlayerIsPushed(sfTrue);
		plr_setPlayerVelocity(vector2f(0.0f, 0.0f));
	}

	if (_enemy->state == EN_STATE_CHASE)
	{
		if (!_enemy->isOnePassedPulse)
		{
			_enemy->time_detection = 0.0f;
			_enemy->frameX = 0;
			_enemy->timerAnim = 0.0f;
			_enemy->isOnePassedPulse = sfTrue;
			plr_setPlayerVelocity(vector2f(0.0f, 0.0f));
		}
		_enemy->time_detection += _delta;
		if (/*!isCollisionPlayerBox() &&*/ !cll_RectMap(plr_getPlayerBounds(), vector2f(plr_getPlayerVelocity().x * _delta, plr_getPlayerVelocity().y * _delta)) && _enemy->time_detection > 0.7f)
		{
			plr_setPlayerVelocity(multiplyVectorsrf(normalizef(createVectorWithVectorf(_enemy->pos, _tmpPpos)), 300.0f));

		}
		else
		{
			plr_setPlayerVelocity(vector2f(0.0f, 0.0f));
		}
		if (_enemy->time_detection / 1.5f >= 1)
		{
			plr_setPlayerIsPushed(sfFalse);
			_enemy->state = EN_STATE_CRYING;
			_enemy->isOnePassedPulse = sfFalse;
			_enemy->time_detection = 0.0f;
			_enemy->frameX = 0;
			_enemy->timerAnim = 0.0f;
		}
	}

	if (_enemy->state != EN_STATE_THROWN)
		_enemy->forward = vec2f_lerp(_enemy->targetForward, _enemy->forward, 0.2f * _delta);
	if (_enemy->state != EN_STATE_CHASE && _enemy->state != EN_STATE_DEAD && _enemy->state != EN_STATE_STUNNED && _enemy->state != EN_STATE_THROWN)
		checkCollisions(_i, _delta);
}

void en_UpdateImunity(EnemiesStruct* _enemy, EnemiesState _tmpState, int _i, float _delta, sfVector2f _tmpPpos)
{
	if (_enemy->state == EN_STATE_THROWN)
	{
		checkCollisions(_i, _delta);
		_enemy->time_wait += _delta;
		if (_enemy->time_wait > 1.f) {
			_enemy->time_wait = 0.f;
			if (map_CanFall(_enemy->pos))
				_enemy->state = EN_STATE_FALLING;
			else
				_enemy->state = EN_STATE_WAIT;
		}
	}
	else if (_enemy->state == EN_STATE_WAIT)
	{
		_enemy->time_wait += _delta;
		if (_enemy->time_wait > 0.5f) {
			_enemy->time_wait = 0.f;
			_enemy->forward = normalizef(createVectorWithVectorf(_enemy->pos, _enemy->spawnPos));
			_enemy->currentVelocity = vector2f(0.f, 0.f);
			_enemy->state = EN_STATE_CHASE;
		}
	}
	else
	{
		if (_enemy->state != EN_STATE_FALLING)
		{
			if (!equalsWithOffset(_enemy->pos, _enemy->spawnPos, 10.f))
				_enemy->haveToHome = sfTrue;
			else
				_enemy->haveToHome = sfFalse;

			if (!_enemy->haveToHome)
			{
				_enemy->timerAllAttacks += _delta;
				if (_enemy->timerAllAttacks > 5.f)
				{
					_enemy->state = EN_STATE_ATTACK;
					if (_enemy->timerAllAttacks > 5.6f)
					{
						_enemy->timerAllAttacks = 0.0f;
						blt_addBullets(_enemy->pos, normalizef(createVectorWithVectorf(_enemy->pos, _tmpPpos)), 3);
					}
				}
				else
					_enemy->state = EN_STATE_CHASE;
			}
			else
			{
				_enemy->targetForward = normalizef(createVectorWithVectorf(_enemy->pos, _enemy->spawnPos));
				_enemy->forward = vec2f_lerp(_enemy->forward, _enemy->targetForward, 0.8f * _delta);
				checkCollisions(_i, _delta);
				_enemy->state = EN_STATE_BACK_HOME;
				_enemy->time_wait = 0.f;
			}
		}
		else
		{
			_enemy->time_wait += _delta;
			if (_enemy->time_wait >= 0.8f)
				enemiesList->erase(&enemiesList, _i);
		}
	}
}

void en_UpdateSeagull(EnemiesStruct* _enemy, EnemiesState _tmpState, int _i, float _delta, sfVector2f _tmpPpos)
{
	_enemy->time_wait += _delta;

	if (_enemy->state == EN_STATE_WAIT || _enemy->state == EN_STATE_EAT)
	{
		if (getSqrMagnitudeBetween2Vectorsf(_tmpPpos, _enemy->pos) < 200.0f * 200.0f)
		{
			_enemy->time_wait = 0.0f;
			_enemy->frameX = 0;
			_enemy->timerAnim = 0.0f;
			_enemy->state = EN_STATE_GOING_TO_FLY;
		}
	}

	if (_enemy->state != EN_STATE_WAIT && _enemy->state != EN_STATE_EAT && _enemy->state != EN_STATE_GOING_TO_FLY)
		checkCollisions(_i, _delta);

	switch (_tmpState)
	{
	case EN_STATE_WAIT:
		if (_enemy->time_wait > _enemy->time_to_wait)
		{
			_enemy->time_wait = 0.0f;
			_enemy->frameX = 0;
			_enemy->timerAnim = 0.0f;
			_enemy->state = EN_STATE_EAT;
		}
		break;
	case EN_STATE_EAT:
		if (_enemy->time_wait > 4.2f)
		{
			_enemy->time_wait = 0.0f;
			_enemy->frameX = 0;
			_enemy->timerAnim = 0.0f;
			_enemy->time_to_wait = rand_float(5.f, 12.f);
			_enemy->state = EN_STATE_WAIT;
		}
		break;
	case EN_STATE_GOING_TO_FLY:
		if (_enemy->time_wait > 1.2f)
		{
			_enemy->time_wait = 0.0f;
			_enemy->frameX = 0;
			_enemy->timerAnim = 0.0f;
			_enemy->state = EN_STATE_FLY2;
			_enemy->forward = normalizef(createVectorWithVectorf(_tmpPpos, _enemy->pos));
			if (_tmpPpos.x < _enemy->pos.x)
				_enemy->haveToHome = sfTrue;
		}
		break;
	case EN_STATE_FLY:
		if (_enemy->time_wait > _enemy->time_to_wait)
		{
			_enemy->time_wait = 0.0f;
			_enemy->frameX = 0;
			_enemy->timerAnim = 0.0f;
			_enemy->state = EN_STATE_FLY2;
		}
		break;
	case EN_STATE_FLY2:
		if (_enemy->time_wait > 1.f)
		{
			_enemy->time_wait = 0.0f;
			_enemy->frameX = 0;
			_enemy->timerAnim = 0.0f;
			_enemy->time_to_wait = rand_float(2.f, 10.f);
			if (_enemy->countOfSeePlayer == 2)
				_enemy->state = EN_STATE_FLY;
			else
				_enemy->countOfSeePlayer++;

		}
		break;
	}
}

void en_DisplayEnemiesShadows(Window* _window)
{
	sfRectangleShape_setRotation(en_rectangleShape, 0.0f);
	for (int i = 0; i < enemiesList->size(enemiesList); i++)
	{
		sfRectangleShape_setOrigin(en_rectangleShape, GD_ENEMY->origin);
		en_DisplayShadows[GD_ENEMY->type](_window->rdrt, GD_ENEMY);
	}
}

void en_DisplayEnemies(Window* _window)
{
	sfVector2f tmpPpos = plr_GetPos();
	for (int i = 0; i < enemiesList->size(enemiesList); i++)
	{
		sfRectangleShape_setOrigin(en_rectangleShape, GD_ENEMY->origin);
		sfRectangleShape_setRotation(en_rectangleShape, GD_ENEMY->rotation);
		sfRectangleShape_setScale(en_rectangleShape, GD_ENEMY->scale);
		sfRectangleShape_setFillColor(en_rectangleShape, color(255, 255, 255, 255));
		en_DisplayEnemyType[GD_ENEMY->type](_window->rdrt, GD_ENEMY, tmpPpos);
		sfRectangleShape_setTexture(en_rectangleShape, en_Textures[GD_ENEMY->type], sfFalse);

		if (GD_ENEMY->isOnScreen)
			sfRenderTexture_drawRectangleShape(_window->rdrt, en_rectangleShape, NULL);

		if (GD_ENEMY->type == EN_CHAMELEON && (GD_ENEMY->state == EN_STATE_LAUNCH_TONGUE || GD_ENEMY->state == EN_STATE_BACK_TONGUE))
			en_DisplayChameleonTongue(_window->rdrt, GD_ENEMY);

		if (!toRemoveToggleDebugRect)
			continue;

		// debug
		sfRectangleShape_setRotation(en_rectangleShape, 0.f);
		sfRectangleShape_setOutlineColor(en_rectangleShape, color(255, 0, 0, 127));
		sfRectangleShape_setTexture(en_rectangleShape, NULL, sfFalse);
		sfRectangleShape_setFillColor(en_rectangleShape, color(0, 0, 0, 0));
		sfRectangleShape_setOutlineThickness(en_rectangleShape, 2.f);

		sfRectangleShape_setPosition(en_rectangleShape, addVectorsrf(GD_ENEMY->pos, vector2f(-6.f, -6.f)));
		sfRectangleShape_setOrigin(en_rectangleShape, vector2f(0.f, 0.f));
		sfRectangleShape_setSize(en_rectangleShape, vector2f(12.f, 12.f));
		sfRectangleShape_setScale(en_rectangleShape, GD_ENEMY->scale);
		sfRenderTexture_drawRectangleShape(_window->rdrt, en_rectangleShape, NULL);

		// chameleon debug
		if (GD_ENEMY->type == EN_CHAMELEON) {

			sfFloatRect fo = en_GetChameleonTongueArea(GD_ENEMY);
			sfRectangleShape_setPosition(en_rectangleShape, vector2f(fo.left, fo.top));
			sfRectangleShape_setOrigin(en_rectangleShape, vector2f(0.f, 0.f));
			sfRectangleShape_setSize(en_rectangleShape, vector2f(fo.width, fo.height));
			sfRectangleShape_setScale(en_rectangleShape, GD_ENEMY->scale);
			//sfRenderTexture_drawRectangleShape(_window->rdrt, en_rectangleShape, NULL);

			if (GD_ENEMY->state == EN_STATE_BACK_TONGUE || GD_ENEMY->state == EN_STATE_LAUNCH_TONGUE) {
				sfFloatRect f = en_GetChameleonTongueBounds(GD_ENEMY);
				sfRectangleShape_setPosition(en_rectangleShape, vector2f(f.left, f.top));
				sfRectangleShape_setOrigin(en_rectangleShape, vector2f(0.f, 0.f));
				sfRectangleShape_setSize(en_rectangleShape, vector2f(f.width, f.height));
				sfRectangleShape_setScale(en_rectangleShape, GD_ENEMY->scale);
				sfRenderTexture_drawRectangleShape(_window->rdrt, en_rectangleShape, NULL);

				sfFloatRect foo = en_GetChameleonTongueCollisions(GD_ENEMY);
				sfRectangleShape_setPosition(en_rectangleShape, vector2f(foo.left, foo.top));
				sfRectangleShape_setOrigin(en_rectangleShape, vector2f(0.f, 0.f));
				sfRectangleShape_setSize(en_rectangleShape, vector2f(foo.width, foo.height));
				sfRectangleShape_setScale(en_rectangleShape, GD_ENEMY->scale);
				sfRenderTexture_drawRectangleShape(_window->rdrt, en_rectangleShape, NULL);
			}

			sfRectangleShape_setPosition(en_rectangleShape, vector2f(tmpPpos.x - 3.f, tmpPpos.y - 3.f));
			sfRectangleShape_setOrigin(en_rectangleShape, vector2f(0.f, 0.f));
			sfRectangleShape_setSize(en_rectangleShape, vector2f(6.f, 6.f));
			sfRectangleShape_setScale(en_rectangleShape, GD_ENEMY->scale);
			sfRenderTexture_drawRectangleShape(_window->rdrt, en_rectangleShape, NULL);
		}
		//

		sfRectangleShape_setPosition(en_rectangleShape, addVectorsrf(addVectorsrf(GD_ENEMY->pos, vector2f(-GD_ENEMY->origin.x, -GD_ENEMY->origin.y)), vector2f(GD_ENEMY->collisionsBounds.left, GD_ENEMY->collisionsBounds.top)));
		sfRectangleShape_setOrigin(en_rectangleShape, vector2f(0.f, 0.f));
		sfRectangleShape_setSize(en_rectangleShape, vector2f(GD_ENEMY->collisionsBounds.width, GD_ENEMY->collisionsBounds.height));
		sfRectangleShape_setScale(en_rectangleShape, GD_ENEMY->scale);
		sfRenderTexture_drawRectangleShape(_window->rdrt, en_rectangleShape, NULL);

		sfFloatRect fooplr = plr_getPlayerBoundsWithEnemies();
		sfRectangleShape_setPosition(en_rectangleShape, vector2f(fooplr.left, fooplr.top));
		sfRectangleShape_setOrigin(en_rectangleShape, vector2f(0.f, 0.f));
		sfRectangleShape_setSize(en_rectangleShape, vector2f(fooplr.width, fooplr.height));
		sfRectangleShape_setScale(en_rectangleShape, GD_ENEMY->scale);
		sfRenderTexture_drawRectangleShape(_window->rdrt, en_rectangleShape, NULL);

		sfRectangleShape_setFillColor(en_rectangleShape, color(255, 255, 255, 255));
		sfRectangleShape_setOutlineThickness(en_rectangleShape, 0.f);
	}
}

void en_DisplayYoyoSpider(sfRenderTexture* _rdrt, EnemiesStruct* _enemy, sfVector2f _tmpPpos)
{
	sfVector2f offSetPos = NULLVECTF;
	switch (_enemy->state)
	{
	case EN_STATE_ESCAPING:
		if (_enemy->forward.x > 0.0f)
			sfRectangleShape_setScale(en_rectangleShape, vector2f(-1, 1));
		else
			sfRectangleShape_setScale(en_rectangleShape, vector2f(1, 1));
		_enemy->size.x = 80.f;
		_enemy->size.y = 62.f;
		sfRectangleShape_setTextureRect(en_rectangleShape, animatorAnim("spider_WALK", &_enemy->frameX, &_enemy->timerAnim, 0));
		break;
	case EN_STATE_BACK_HOME:
		if (_enemy->frameX == 3 && _enemy->timerAnim > 0.19 && !_enemy->isFallingAnimDone)
		{
			_enemy->frameX = 0;
			_enemy->timerAnim = 0.0f;
			_enemy->isFallingAnimDone = sfTrue;
		}

		if (!_enemy->isFallingAnimDone)
		{
			_enemy->size.x = 76.f;
			_enemy->size.y = 168.f;
			sfRectangleShape_setTextureRect(en_rectangleShape, animatorAnim("spider_FALL", &_enemy->frameX, &_enemy->timerAnim, 0));
		}
		else if (_enemy->isFallingAnimDone)
		{
			_enemy->size.x = 78.f;
			_enemy->size.y = 62.f;
			offSetPos.y += 150.f;
			sfRectangleShape_setPosition(en_rectangleShape, vector2f(_enemy->pos.x, _enemy->pos.y + 150.f));
			sfRectangleShape_setTextureRect(en_rectangleShape, IntRect(0, 112.f, 78.f, 62.f));
		}
		break;
	case EN_STATE_GOINGUP:
		offSetPos.x -= 2.f;
		_enemy->size.x = 76.f;
		_enemy->size.y = 130.f;
		sfRectangleShape_setTextureRect(en_rectangleShape, animatorAnim("spider_UP", &_enemy->frameX, &_enemy->timerAnim, 0));
		break;
	case EN_STATE_WAIT:
		if (_enemy->frameX >= 8 && _enemy->timerAnim >= 0.15f && !_enemy->isFallingAnimDone)
		{
			_enemy->frameX = 0;
			_enemy->timerAnim = 0.0f;
			_enemy->isFallingAnimDone = sfTrue;
		}
		if (!_enemy->isFallingAnimDone)
		{
			offSetPos.x -= 22.0f;
			_enemy->size.x = 96.f;
			_enemy->size.y = 130.f;
			sfRectangleShape_setPosition(en_rectangleShape, vector2f(_enemy->pos.x - 22.f, _enemy->pos.y));
			sfRectangleShape_setTextureRect(en_rectangleShape, animatorAnim("spider_DOWN", &_enemy->frameX, &_enemy->timerAnim, 0));
		}
		else
		{
			_enemy->size.x = 72.f;
			_enemy->size.y = 120.f;
			sfRectangleShape_setTextureRect(en_rectangleShape, animatorAnim("spider_IDLE", &_enemy->frameX, &_enemy->timerAnim, 0));
		}
		break;
	case EN_STATE_THROWN:
		sfRectangleShape_setTextureRect(en_rectangleShape, IntRect(0, 174, 80, 62));
		_enemy->size = vector2f(80.f, 62.f);
		break;
	case EN_STATE_DAMAGING:
		sfRectangleShape_setTextureRect(en_rectangleShape, IntRect(0, 112, 78, 62));
		_enemy->size = vector2f(78.f, 62.f);
		break;
	case EN_STATE_FALLING2:
		addVectorsf(&_enemy->pos, multiplyVectorsrf(_enemy->forward, 0.3f));
		sfRectangleShape_setTextureRect(en_rectangleShape, IntRect(0, 174, 80, 62));
		_enemy->size = vector2f(80.f, 62.f);
		if (_enemy->forward.x > 0.0f)
			sfRectangleShape_setScale(en_rectangleShape, vector2f(-_enemy->scale.x, _enemy->scale.y));
		else
			sfRectangleShape_setScale(en_rectangleShape, _enemy->scale);
		break;
	default:
		sfRectangleShape_setTextureRect(en_rectangleShape, IntRect(0, 174, 80, 62));
		_enemy->size = vector2f(80.f, 62.f);
		break;
	}
	sfRectangleShape_setSize(en_rectangleShape, _enemy->size);
	sfRectangleShape_setRotation(en_rectangleShape, _enemy->rotationFalling);
	sfRectangleShape_setPosition(en_rectangleShape, addVectorsrf(_enemy->pos, offSetPos));
	_enemy->size = vector2f(76.f, 120.f);
}

void en_DisplayVenimousYoyoSpider(sfRenderTexture* _rdrt, EnemiesStruct* _enemy, sfVector2f _tmpPpos)
{
	sfVector2f offSetPos = NULLVECTF;
	switch (_enemy->state)
	{
	case EN_STATE_ESCAPING:
		if (_enemy->forward.x > 0.0f)
			sfRectangleShape_setScale(en_rectangleShape, vector2f(-1, 1));
		else
			sfRectangleShape_setScale(en_rectangleShape, vector2f(1, 1));
		_enemy->size.x = 79.f;
		_enemy->size.y = 60.f;
		sfRectangleShape_setTextureRect(en_rectangleShape, animatorAnim("spider_BAD_WALK", &_enemy->frameX, &_enemy->timerAnim, 0));
		break;
	case EN_STATE_BACK_HOME:
		if (_enemy->frameX == 3 && _enemy->timerAnim > 0.19 && !_enemy->isFallingAnimDone)
		{
			_enemy->frameX = 0;
			_enemy->timerAnim = 0.0f;
			_enemy->isFallingAnimDone = sfTrue;
		}

		if (!_enemy->isFallingAnimDone)
		{
			_enemy->size.x = 76.f;
			_enemy->size.y = 168.f;
			sfRectangleShape_setTextureRect(en_rectangleShape, animatorAnim("spider_BAD_FALL", &_enemy->frameX, &_enemy->timerAnim, 0));
		}
		else if (_enemy->isFallingAnimDone)
		{
			_enemy->size.x = 78.f;
			_enemy->size.y = 62.f;
			offSetPos.y += 150.f;
			sfRectangleShape_setPosition(en_rectangleShape, vector2f(_enemy->pos.x, _enemy->pos.y + 150.f));
			sfRectangleShape_setTextureRect(en_rectangleShape, IntRect(0, 112.f, 78.f, 62.f));
		}
		break;
	case EN_STATE_GOINGUP:
		offSetPos.x -= 2.f;
		_enemy->size.x = 76.f;
		_enemy->size.y = 130.f;
		sfRectangleShape_setTextureRect(en_rectangleShape, animatorAnim("spider_BAD_UP", &_enemy->frameX, &_enemy->timerAnim, 0));
		break;
	case EN_STATE_WAIT:
		if (_enemy->frameX >= 8 && _enemy->timerAnim >= 0.15f && !_enemy->isFallingAnimDone)
		{
			_enemy->frameX = 0;
			_enemy->timerAnim = 0.0f;
			_enemy->isFallingAnimDone = sfTrue;
		}
		if (!_enemy->isFallingAnimDone)
		{
			offSetPos.x -= 22.0f;
			_enemy->size.x = 96.f;
			_enemy->size.y = 130.f;
			sfRectangleShape_setPosition(en_rectangleShape, vector2f(_enemy->pos.x - 22.f, _enemy->pos.y));
			sfRectangleShape_setTextureRect(en_rectangleShape, animatorAnim("spider_BAD_DOWN", &_enemy->frameX, &_enemy->timerAnim, 0));
		}
		else
		{
			_enemy->size.x = 72.f;
			_enemy->size.y = 120.f;
			sfRectangleShape_setTextureRect(en_rectangleShape, animatorAnim("spider_BAD_IDLE", &_enemy->frameX, &_enemy->timerAnim, 0));
		}
		break;
	case EN_STATE_THROWN:
		sfRectangleShape_setTextureRect(en_rectangleShape, IntRect(0, 174, 80, 62));
		_enemy->size = vector2f(80.f, 62.f);
		break;
	case EN_STATE_DAMAGING:
		sfRectangleShape_setTextureRect(en_rectangleShape, IntRect(0, 112, 78, 62));
		_enemy->size = vector2f(78.f, 62.f);
		break;
	case EN_STATE_FALLING2:
		addVectorsf(&_enemy->pos, multiplyVectorsrf(_enemy->forward, 0.3f));
		sfRectangleShape_setTextureRect(en_rectangleShape, IntRect(0, 174, 80, 62));
		_enemy->size = vector2f(80.f, 62.f);
		if (_enemy->forward.x > 0.0f)
			sfRectangleShape_setScale(en_rectangleShape, vector2f(-_enemy->scale.x, _enemy->scale.y));
		else
			sfRectangleShape_setScale(en_rectangleShape, _enemy->scale);
		break;
	default:
		sfRectangleShape_setTextureRect(en_rectangleShape, IntRect(0, 174, 80, 62));
		_enemy->size = vector2f(80.f, 62.f);
		break;
	}
	sfRectangleShape_setSize(en_rectangleShape, _enemy->size);
	sfRectangleShape_setPosition(en_rectangleShape, addVectorsrf(_enemy->pos, offSetPos));
	sfRectangleShape_setRotation(en_rectangleShape, _enemy->rotationFalling);
	_enemy->size = vector2f(76.f, 120.f);
}

void en_DisplayCrab(sfRenderTexture* _rdrt, EnemiesStruct* _enemy, sfVector2f _tmpPpos)
{

	sfVector2f offsetPos = NULLVECTF;

	if (_tmpPpos.x < _enemy->pos.x)
		sfRectangleShape_setScale(en_rectangleShape, vector2f(-_enemy->scale.x, _enemy->scale.y));
	else
		sfRectangleShape_setScale(en_rectangleShape, _enemy->scale);


	switch (_enemy->state)
	{
	case EN_STATE_WAIT:
		sfRectangleShape_setSize(en_rectangleShape, vector2f(70.0f, 58.f));
		_enemy->size = vector2f(70.0f, 58.f);
		sfRectangleShape_setTextureRect(en_rectangleShape, animatorAnim("crabIdle", &_enemy->frameX, &_enemy->timerAnim, 0));
		break;
	case EN_STATE_WALK:
		sfRectangleShape_setSize(en_rectangleShape, vector2f(70.f, 58.f));
		_enemy->size = vector2f(70.0f, 58.f);
		sfRectangleShape_setTextureRect(en_rectangleShape, animatorAnim("crabWalk", &_enemy->frameX, &_enemy->timerAnim, 0));
		break;
	case EN_STATE_DETECTING:
		sfRectangleShape_setSize(en_rectangleShape, vector2f(70.f, 58.f));
		_enemy->size = vector2f(70.0f, 58.f);
		sfRectangleShape_setTextureRect(en_rectangleShape, animatorAnim("crabWalk", &_enemy->frameX, &_enemy->timerAnim, 0));
		break;
	case EN_STATE_CHASE:
		sfRectangleShape_setSize(en_rectangleShape, vector2f(74.f, 64.f));
		_enemy->size = vector2f(74.0f, 64.f);
		sfRectangleShape_setTextureRect(en_rectangleShape, animatorAnim("crabOutHome", &_enemy->frameX, &_enemy->timerAnim, 0));
		break;
	case EN_STATE_BACK_HOME:
		offsetPos.x += 2.0f;
		sfRectangleShape_setSize(en_rectangleShape, vector2f(72.f, 64.f));
		_enemy->size = vector2f(72.0f, 64.f);
		sfRectangleShape_setTextureRect(en_rectangleShape, animatorAnim("crabInHome", &_enemy->frameX, &_enemy->timerAnim, 0));
		break;
	case EN_STATE_THROWN:
		sfRectangleShape_setSize(en_rectangleShape, vector2f(70.f, 58.f));
		_enemy->size = vector2f(70.0f, 58.f);
		sfRectangleShape_setTextureRect(en_rectangleShape, animatorAnim("crabStun", &_enemy->frameX, &_enemy->timerAnim, 0));
		break;
	case EN_STATE_STUNNED:
		sfRectangleShape_setSize(en_rectangleShape, vector2f(70.f, 62.f));
		_enemy->size = vector2f(70.0f, 62.f);
		sfRectangleShape_setTextureRect(en_rectangleShape, animatorAnim("crabAttract", &_enemy->frameX, &_enemy->timerAnim, 0));
		break;
	case EN_STATE_DAMAGING:
		sfRectangleShape_setTextureRect(en_rectangleShape, IntRect(0, 58, 66, 54));
		sfRectangleShape_setSize(en_rectangleShape, vector2f(66.f, 54.f));
		_enemy->size = vector2f(66.0f, 54.f);
		break;
	case EN_STATE_DEAD:

		sfRectangleShape_setTextureRect(en_rectangleShape, IntRect(0, 58, 66, 54));
		sfRectangleShape_setSize(en_rectangleShape, vector2f(66.f, 54.f));
		_enemy->size = vector2f(66.0f, 54.f);
		break;
	default:
		sfRectangleShape_setTextureRect(en_rectangleShape, IntRect(0, 112, 70, 58));
		sfRectangleShape_setSize(en_rectangleShape, vector2f(70.f, 58.f));
		_enemy->size = vector2f(7.0f, 58.f);
		break;
	}

	if (_enemy->currentVelocity.x > 0.5f || (_enemy->state == EN_STATE_DETECTING && _tmpPpos.x > _enemy->pos.x)) {
		sfRectangleShape_setScale(en_rectangleShape, vector2f(-_enemy->scale.x, _enemy->scale.y));
		offsetPos.x = -offsetPos.x;
	}
	else
		sfRectangleShape_setScale(en_rectangleShape, _enemy->scale);
	sfRectangleShape_setPosition(en_rectangleShape, addVectorsrf(_enemy->pos, offsetPos));
}

void en_DisplayGoose(sfRenderTexture* _rdrt, EnemiesStruct* _enemy, sfVector2f _tmpPpos)
{

	sfVector2f offSetPos = NULLVECTF;
	switch (_enemy->state)
	{
	case EN_STATE_WAIT:
		sfRectangleShape_setTextureRect(en_rectangleShape, animatorWithResize("Goose_Idle", &_enemy->frameX, &_enemy->timerAnim, 0, en_rectangleShape));
		break;
	case EN_STATE_ATTACK:
		offSetPos.x -= 5.0f;
		offSetPos.y -= 5.0f;
		sfRectangleShape_setTextureRect(en_rectangleShape, animatorWithResize("Goose_Attack", &_enemy->frameX, &_enemy->timerAnim, 0, en_rectangleShape));
		break;
	case EN_STATE_RUSH:
		offSetPos.x -= 50.0f;
		sfRectangleShape_setTextureRect(en_rectangleShape, animatorWithResize("Goose_Rush", &_enemy->frameX, &_enemy->timerAnim, 0, en_rectangleShape));
		break;
	case EN_STATE_PATROLING:
		if (_enemy->currentVelocity.x <= 0.5f)
			offSetPos.x -= 5;
		sfRectangleShape_setTextureRect(en_rectangleShape, animatorWithResize("Goose_Walk", &_enemy->frameX, &_enemy->timerAnim, 0, en_rectangleShape));
		break;
	case EN_STATE_CHASE:
		offSetPos.x -= 55.0f;
		offSetPos.y += 5.f;
		sfRectangleShape_setTextureRect(en_rectangleShape, animatorWithBeginFrame("Goose_Down", &_enemy->frameX, &_enemy->timerAnim, 0, 4));
		sfRectangleShape_setSize(en_rectangleShape, vector2f(119.f, 83.f));
		_enemy->size = vector2f(119.f, 83.f);
		break;
	case EN_STATE_DAMAGING:
		offSetPos.x -= 20.0f;
		sfRectangleShape_setSize(en_rectangleShape, vector2f(96.f, 90.0f));
		_enemy->size = vector2f(96.0f, 90.f);
		sfRectangleShape_setTextureRect(en_rectangleShape, IntRect(0, 536, 96, 90));
		sfRectangleShape_setSize(en_rectangleShape, _enemy->size);
		break;
	case EN_STATE_DEAD:
		offSetPos.x -= 20.0f;
		sfRectangleShape_setSize(en_rectangleShape, vector2f(96.f, 90.0f));
		_enemy->size = vector2f(96.0f, 90.f);
		sfRectangleShape_setTextureRect(en_rectangleShape, IntRect(0, 536, 96, 90));
		break;
	case EN_STATE_FALLING2:
		sfRectangleShape_setTextureRect(en_rectangleShape, IntRect(0, 0, 60, 88));
		_enemy->size = vector2f(60.0f, 88.f);
		sfRectangleShape_setSize(en_rectangleShape, vector2f(60.f, 88.0f));
		addVectorsf(&_enemy->pos, multiplyVectorsrf(_enemy->forward, 0.3f));
		/*if (_enemy->forward.x > 0.0f)
			sfRectangleShape_setScale(en_rectangleShape, vector2f(-_enemy->scale.x, _enemy->scale.y));
		else
			sfRectangleShape_setScale(en_rectangleShape, _enemy->scale);*/
		break;
	default:
		sfRectangleShape_setSize(en_rectangleShape, vector2f(60.0f, 88.0f));
		_enemy->size = vector2f(60.0f, 88.f);
		sfRectangleShape_setTextureRect(en_rectangleShape, IntRect(0, 0, 60, 88));
		break;
	}

	if (_enemy->currentVelocity.x > 0.5f)
	{
		sfRectangleShape_setScale(en_rectangleShape, vector2f(-_enemy->scale.x, _enemy->scale.y));
		offSetPos.x = -offSetPos.x;
	}
	else
		sfRectangleShape_setScale(en_rectangleShape, _enemy->scale);

	sfRectangleShape_setRotation(en_rectangleShape, _enemy->rotationFalling);
	sfRectangleShape_setPosition(en_rectangleShape, addVectorsrf(_enemy->pos, offSetPos));
}

void en_DisplayWhiteBillyGoat(sfRenderTexture* _rdrt, EnemiesStruct* _enemy, sfVector2f _tmpPpos)
{
	if (_enemy->forward.x > 0.0f)
		sfRectangleShape_setScale(en_rectangleShape, vector2f(-1, 1));
	else
		sfRectangleShape_setScale(en_rectangleShape, vector2f(1, 1));
	switch (_enemy->state)
	{
		sfRectangleShape_setTexture(en_rectangleShape, en_Textures[EN_WHITE_BILLY_GOAT], sfFalse);
	case EN_STATE_RUSH:
		sfRectangleShape_setTextureRect(en_rectangleShape, animatorAnim("WHITE_GOAT_RUSH", &_enemy->frameX, &_enemy->timerAnim, 0));
		break;
	case EN_STATE_BACK_HOME:
		sfRectangleShape_setTextureRect(en_rectangleShape, animatorAnim("WHITE_GOAT_WALK", &_enemy->frameX, &_enemy->timerAnim, 0));
		break;
	case EN_STATE_CHASE:
		sfRectangleShape_setTextureRect(en_rectangleShape, animatorAnim("WHITE_GOAT_CHARGE", &_enemy->frameX, &_enemy->timerAnim, 0));
		break;
	case EN_STATE_WAIT:
		sfRectangleShape_setTextureRect(en_rectangleShape, animatorAnim("WHITE_GOAT_IDLE", &_enemy->frameX, &_enemy->timerAnim, 0));
		break;
	case EN_STATE_DAMAGING:
		_enemy->spriteRect.left = 0.f;
		_enemy->spriteRect.top = 320.f;
		_enemy->spriteRect.width = 144.f;
		_enemy->spriteRect.height = 104.f;
		sfRectangleShape_setTextureRect(en_rectangleShape, _enemy->spriteRect);
		break;
	case EN_STATE_FALLING2:
		sfRectangleShape_setTextureRect(en_rectangleShape, IntRect(0, 0, 144, 108));
		addVectorsf(&_enemy->pos, multiplyVectorsrf(_enemy->forward, 0.5f));
		if (_enemy->forward.x > 0.0f)
			sfRectangleShape_setScale(en_rectangleShape, vector2f(-_enemy->scale.x, _enemy->scale.y));
		else
			sfRectangleShape_setScale(en_rectangleShape, _enemy->scale);
		break;
	default:
		sfRectangleShape_setTextureRect(en_rectangleShape, IntRect(0, 0, 144, 108));
		break;
	}
	sfRectangleShape_setPosition(en_rectangleShape, _enemy->pos);
	sfRectangleShape_setSize(en_rectangleShape, _enemy->size);
	sfRectangleShape_setRotation(en_rectangleShape, _enemy->rotationFalling);
	_enemy->size = vector2f(144.f, 108.f);
}

void en_DisplayBlackBillyGoat(sfRenderTexture* _rdrt, EnemiesStruct* _enemy, sfVector2f _tmpPpos)
{
	if (_enemy->forward.x > 0.0f)
		sfRectangleShape_setScale(en_rectangleShape, vector2f(-1, 1));
	else
		sfRectangleShape_setScale(en_rectangleShape, vector2f(1, 1));
	switch (_enemy->state)
	{
	case EN_STATE_RUSH:
		sfRectangleShape_setTextureRect(en_rectangleShape, animatorAnim("BLACK_GOAT_RUSH", &_enemy->frameX, &_enemy->timerAnim, 0));
		break;
	case EN_STATE_BACK_HOME:
		sfRectangleShape_setTextureRect(en_rectangleShape, animatorAnim("BLACK_GOAT_WALK", &_enemy->frameX, &_enemy->timerAnim, 0));
		break;
	case EN_STATE_CHASE:
		sfRectangleShape_setTextureRect(en_rectangleShape, animatorAnim("BLACK_GOAT_CHARGE", &_enemy->frameX, &_enemy->timerAnim, 0));
		break;
	case EN_STATE_WAIT:
		sfRectangleShape_setTextureRect(en_rectangleShape, animatorAnim("BLACK_GOAT_IDLE", &_enemy->frameX, &_enemy->timerAnim, 0));
		break;
	case EN_STATE_DAMAGING:
		_enemy->spriteRect.left = 0.f;
		_enemy->spriteRect.top = 744.f;
		_enemy->spriteRect.width = 144.f;
		_enemy->spriteRect.height = 104.f;
		sfRectangleShape_setTextureRect(en_rectangleShape, _enemy->spriteRect);
		break;
	case EN_STATE_FALLING2:
		sfRectangleShape_setTextureRect(en_rectangleShape, IntRect(0, 424, 144, 108));
		addVectorsf(&_enemy->pos, multiplyVectorsrf(_enemy->forward, 0.5f));
		if (_enemy->forward.x > 0.0f)
			sfRectangleShape_setScale(en_rectangleShape, vector2f(-_enemy->scale.x, _enemy->scale.y));
		else
			sfRectangleShape_setScale(en_rectangleShape, _enemy->scale);
		break;
	default:
		sfRectangleShape_setTextureRect(en_rectangleShape, IntRect(0, 424, 144, 108));
		break;
	}
	sfRectangleShape_setPosition(en_rectangleShape, _enemy->pos);
	sfRectangleShape_setSize(en_rectangleShape, _enemy->size);
	sfRectangleShape_setRotation(en_rectangleShape, _enemy->rotationFalling);
	_enemy->size = vector2f(144.f, 108.f);
}

void en_DisplaySnake(sfRenderTexture* _rdrt, EnemiesStruct* _enemy, sfVector2f _tmpPpos)
{
	sfVector2f offsetPos = NULLVECTF;

	switch (_enemy->state)
	{
	case EN_STATE_WAIT:
		sfRectangleShape_setTextureRect(en_rectangleShape, animatorAnim("snakeIdle", &_enemy->frameX, &_enemy->timerAnim, 0));
		_enemy->size = vector2f(90.f, 76.f);
		break;
	case EN_STATE_WALK:
		sfRectangleShape_setTextureRect(en_rectangleShape, animatorAnim("snakeWalk", &_enemy->frameX, &_enemy->timerAnim, 0));
		_enemy->size = vector2f(92.f, 84.f);
		offsetPos.y -= 8.f;
		offsetPos.x -= 2.f;
		break;
	case EN_STATE_ATTACK:
		sfRectangleShape_setTextureRect(en_rectangleShape, animatorAnim("snakeAttack", &_enemy->frameX, &_enemy->timerAnim, 0));
		_enemy->size = vector2f(106.f, 90.f);
		offsetPos.y -= 6.f;
		offsetPos.x -= 8.f;
		break;
	case EN_STATE_DAMAGING:
		sfRectangleShape_setTextureRect(en_rectangleShape, IntRect(0, 250, 106, 76));
		_enemy->size = vector2f(106.f, 76.f);
		offsetPos.x -= 10.f;
		break;
	case EN_STATE_FALLING2:
		sfRectangleShape_setTextureRect(en_rectangleShape, IntRect(0, 0, 90, 76));
		_enemy->size = vector2f(90.f, 76.f);
		addVectorsf(&_enemy->pos, multiplyVectorsrf(_enemy->forward, 0.3f));
		break;
	default:
		sfRectangleShape_setTextureRect(en_rectangleShape, IntRect(0, 0, 90, 76));
		_enemy->size = vector2f(90.f, 76.f);
		break;
	}

	sfRectangleShape_setSize(en_rectangleShape, _enemy->size);
	sfRectangleShape_setRotation(en_rectangleShape, _enemy->rotationFalling);

	if (_enemy->currentVelocity.x > 0.f || (_enemy->state == EN_STATE_ATTACK && _tmpPpos.x > _enemy->pos.x)) {
		sfRectangleShape_setScale(en_rectangleShape, vector2f(-_enemy->scale.x, _enemy->scale.y));
		offsetPos.x = -offsetPos.x;
	}
	else
		sfRectangleShape_setScale(en_rectangleShape, _enemy->scale);

	sfRectangleShape_setPosition(en_rectangleShape, addVectorsrf(_enemy->pos, offsetPos));
}

void en_DisplayMosquito(sfRenderTexture* _rdrt, EnemiesStruct* _enemy, sfVector2f _tmpPpos)
{
	sfVector2f offSetPos = NULLVECTF;

	sfBool isAttackCharged = sfFalse;
	if (_enemy->forward.x > 0.0f)
		sfRectangleShape_setScale(en_rectangleShape, vector2f(-1, 1));
	else
		sfRectangleShape_setScale(en_rectangleShape, vector2f(1, 1));
	if (_enemy->state != EN_STATE_ATTACK)
		isAttackCharged = sfFalse;
	switch (_enemy->state)
	{
	case EN_STATE_WALK:
		sfRectangleShape_setTextureRect(en_rectangleShape, animatorAnim("mosquito_WALK", &_enemy->frameX, &_enemy->timerAnim, 0));
		_enemy->size = vector2f(78.f, 96.f);
		break;
	case EN_STATE_CHASE:
		sfRectangleShape_setTextureRect(en_rectangleShape, animatorAnim("mosquito_WALK", &_enemy->frameX, &_enemy->timerAnim, 0));
		_enemy->size = vector2f(78.f, 96.f);
		break;
	case EN_STATE_BACK_HOME:
		sfRectangleShape_setTextureRect(en_rectangleShape, animatorAnim("mosquito_WALK", &_enemy->frameX, &_enemy->timerAnim, 0));
		_enemy->size = vector2f(78.f, 96.f);
		break;
	case EN_STATE_ATTACK:
		if (isAttackCharged)
		{
			sfRectangleShape_setTextureRect(en_rectangleShape, animatorAnim("mosquito_ATTACK", &_enemy->frameX, &_enemy->timerAnim, 0));
			_enemy->size = vector2f(78.f, 94.f);
		}
		else
		{
			if (_enemy->frameX == 4 && _enemy->timerAnim > 0.19 && !isAttackCharged)
				isAttackCharged = sfTrue;
			else
			{
				sfRectangleShape_setTextureRect(en_rectangleShape, animatorAnim("mosquito_CHARGE", &_enemy->frameX, &_enemy->timerAnim, 0));
				_enemy->size = vector2f(78.f, 92.f);
			}
		}
		break;
	case EN_STATE_DAMAGING:
		offSetPos.y += 10.0f;
		sfRectangleShape_setTextureRect(en_rectangleShape, IntRect(0, 282, 66, 84));
		_enemy->size = vector2f(66.f, 84.f);
		break;
	default:
		sfRectangleShape_setTextureRect(en_rectangleShape, IntRect(0, 282, 78, 96));
		_enemy->size = vector2f(78.f, 96.f);
		break;
	}
	sfRectangleShape_setSize(en_rectangleShape, _enemy->size);
	_enemy->size = vector2f(78.f, 96.f);
	sfRectangleShape_setPosition(en_rectangleShape, addVectorsrf(_enemy->pos, offSetPos));

}

void en_DisplayMole(sfRenderTexture* _rdrt, EnemiesStruct* _enemy, sfVector2f _tmpPpos)
{
	if (_enemy->state != EN_STATE_WALK)
	{
		if (_tmpPpos.x < _enemy->pos.x)
			sfRectangleShape_setScale(en_rectangleShape, vector2f(-_enemy->scale.x, _enemy->scale.y));
		else
			sfRectangleShape_setScale(en_rectangleShape, _enemy->scale);
	}
	sfRectangleShape_setSize(en_rectangleShape, vector2f(64.f, 64.f));
	_enemy->size = vector2f(64.0f, 64.f);
	sfRectangleShape_setPosition(en_rectangleShape, _enemy->startPos);

	switch (_enemy->state)
	{
	case EN_STATE_WAIT:
		if (_tmpPpos.y > _enemy->pos.y)
			sfRectangleShape_setTextureRect(en_rectangleShape, animatorAnim("moleFaceIdle", &_enemy->frameX, &_enemy->timerAnim, 0));
		else
			sfRectangleShape_setTextureRect(en_rectangleShape, animatorAnim("moleBackIdle", &_enemy->frameX, &_enemy->timerAnim, 0));
		break;
	case EN_STATE_WALK:
		sfRectangleShape_setTextureRect(en_rectangleShape, IntRect(0, 126, 60, 16));
		sfRectangleShape_setSize(en_rectangleShape, vector2f(55.f, 17.f));
		_enemy->size = vector2f(55.0f, 17.f);
		sfRectangleShape_setPosition(en_rectangleShape, vector2f(_enemy->pos.x, _enemy->pos.y + 45.f));
		break;
	case EN_STATE_CHASE:
		if (_tmpPpos.y > _enemy->pos.y)
			sfRectangleShape_setTextureRect(en_rectangleShape, animatorAnim("moleOutHoleFace", &_enemy->frameX, &_enemy->timerAnim, 0));
		else
			sfRectangleShape_setTextureRect(en_rectangleShape, animatorAnim("moleOutHoleBack", &_enemy->frameX, &_enemy->timerAnim, 0));
		break;
	case EN_STATE_BACK_HOME:
		if (_tmpPpos.y > _enemy->pos.y)
			sfRectangleShape_setTextureRect(en_rectangleShape, animatorAnim("moleInHoleFace", &_enemy->frameX, &_enemy->timerAnim, 0));
		else
			sfRectangleShape_setTextureRect(en_rectangleShape, animatorAnim("moleInHoleBack", &_enemy->frameX, &_enemy->timerAnim, 0));
		break;
	case EN_STATE_ATTACK:
		if (_tmpPpos.y > _enemy->pos.y)
			sfRectangleShape_setTextureRect(en_rectangleShape, animatorAnim("moleFaceAttack", &_enemy->frameX, &_enemy->timerAnim, 0));
		else
			sfRectangleShape_setTextureRect(en_rectangleShape, animatorAnim("moleBackAttack", &_enemy->frameX, &_enemy->timerAnim, 0));

		break;
	case EN_STATE_DAMAGING:
		if (_tmpPpos.y < _enemy->pos.y)
		{
			sfRectangleShape_setTextureRect(en_rectangleShape, IntRect(0, 200, 74, 52));
			sfRectangleShape_setSize(en_rectangleShape, vector2f(66.f, 64.f));
			_enemy->size = vector2f(66.0f, 64.f);
		}
		else
		{
			sfRectangleShape_setTextureRect(en_rectangleShape, IntRect(0, 142, 74, 58));
			sfRectangleShape_setSize(en_rectangleShape, vector2f(64.f, 64.f));
			_enemy->size = vector2f(64.0f, 64.f);
		}
		break;
	default:
		sfRectangleShape_setTextureRect(en_rectangleShape, IntRect(0, 252, 74, 66));
		sfRectangleShape_setSize(en_rectangleShape, vector2f(74.f, 66.f));
		_enemy->size = vector2f(74.0f, 66.f);
		break;
	}
}

void en_DisplayScolopendre(sfRenderTexture* _rdrt, EnemiesStruct* _enemy, sfVector2f _tmpPpos)
{
	sfBool isHorizontal = en_IsScolopendreHorizontal(_enemy->rotation);

	sfVector2f offsetPos = NULLVECTF;

	switch (_enemy->state)
	{
	case EN_STATE_HIDDEN:
		if (getState() == EDITOR) {
			if (isHorizontal) {
				sfRectangleShape_setTextureRect(en_rectangleShape, animatorAnim("scolopendreUp", &_enemy->frameX, &_enemy->timerAnim, 0));
				_enemy->size = vector2f(186.f, 46.f);
			}
			else {
				sfRectangleShape_setTextureRect(en_rectangleShape, animatorAnim("scolopendreVerticalUp", &_enemy->frameX, &_enemy->timerAnim, 0));
				_enemy->size = vector2f(66.f, 162.f);
			}
		}
		else {
			sfRectangleShape_setTextureRect(en_rectangleShape, IntRect(0, 0, 0, 0));
			_enemy->size = vector2f(0.f, 0.f);
		}
		break;
	case EN_STATE_GOINGUP:
		if (isHorizontal) {
			sfRectangleShape_setTextureRect(en_rectangleShape, animatorAnim("scolopendreUp", &_enemy->frameX, &_enemy->timerAnim, 0));
			_enemy->size = vector2f(186.f, 46.f);
		}
		else {
			sfRectangleShape_setTextureRect(en_rectangleShape, animatorAnim("scolopendreVerticalUp", &_enemy->frameX, &_enemy->timerAnim, 0));
			_enemy->size = vector2f(66.f, 162.f);
		}
		break;
	case EN_STATE_WALK:
		if (isHorizontal) {
			sfRectangleShape_setTextureRect(en_rectangleShape, animatorAnim("scolopendreWalk", &_enemy->frameX, &_enemy->timerAnim, 0));
			_enemy->size = vector2f(190.f, 46.f);
		}
		else {
			sfRectangleShape_setTextureRect(en_rectangleShape, animatorAnim("scolopendreVerticalWalk", &_enemy->frameX, &_enemy->timerAnim, 0));
			_enemy->size = vector2f(68.f, 192.f);
			offsetPos.x = -2.f;
		}
		break;
	case EN_STATE_GOINGDOWN:
		if (isHorizontal) {
			sfRectangleShape_setTextureRect(en_rectangleShape, animatorAnim("scolopendreDown", &_enemy->frameX, &_enemy->timerAnim, 0));
			_enemy->size = vector2f(182.f, 44.f);
		}
		else {
			sfRectangleShape_setTextureRect(en_rectangleShape, animatorAnim("scolopendreVerticalDown", &_enemy->frameX, &_enemy->timerAnim, 0));
			_enemy->size = vector2f(64.f, 206.f);
		}
		break;
	default:

		break;
	}

	sfRectangleShape_setSize(en_rectangleShape, _enemy->size);

	if (isHorizontal) {
		if (_enemy->rotation < 5.f)
			sfRectangleShape_setScale(en_rectangleShape, _enemy->scale);
		else
			sfRectangleShape_setScale(en_rectangleShape, vector2f(-_enemy->scale.x, _enemy->scale.y));
	}
	else {
		if (_enemy->rotation < 95.f)
			sfRectangleShape_setScale(en_rectangleShape, _enemy->scale);
		else
			sfRectangleShape_setScale(en_rectangleShape, vector2f(_enemy->scale.x, -_enemy->scale.y));
	}

	sfRectangleShape_setRotation(en_rectangleShape, 0.f);
	sfRectangleShape_setPosition(en_rectangleShape, addVectorsrf(_enemy->pos, offsetPos));

}

void en_DisplayChameleon(sfRenderTexture* _rdrt, EnemiesStruct* _enemy, sfVector2f _tmpPpos)
{
	switch (_enemy->state)
	{
	case EN_STATE_HIDDEN:
		if (getState() == EDITOR) {
			sfRectangleShape_setTextureRect(en_rectangleShape, animatorAnim("chameleonIdle", &_enemy->frameX, &_enemy->timerAnim, 0));
			_enemy->size = vector2f(88.f, 80.f);
		}
		else {
			sfRectangleShape_setTextureRect(en_rectangleShape, IntRect(0, 0, 0, 0));
			_enemy->size = NULLVECTF;
		}
		break;
	case EN_STATE_DETECTING:
		sfRectangleShape_setTextureRect(en_rectangleShape, animatorReverse("chameleonDetecting", &_enemy->frameX, &_enemy->timerAnim, 0, 0));
		_enemy->size = vector2f(88.f, 80.f);
		break;
	case EN_STATE_WAIT:
		sfRectangleShape_setTextureRect(en_rectangleShape, animatorAnim("chameleonIdle", &_enemy->frameX, &_enemy->timerAnim, 0));
		_enemy->size = vector2f(88.f, 80.f);
		break;
	case EN_STATE_ATTACK:
		sfRectangleShape_setTextureRect(en_rectangleShape, animatorAnim("chameleonAttack", &_enemy->frameX, &_enemy->timerAnim, 0));
		_enemy->size = vector2f(88.f, 92.f);
		break;
	case EN_STATE_LAUNCH_TONGUE:
	case EN_STATE_BACK_TONGUE:
		sfRectangleShape_setTextureRect(en_rectangleShape, IntRect(616, 316, 88, 92));
		_enemy->size = vector2f(88.f, 92.f);
		break;
	case EN_STATE_BACK_ATTACK:
		sfRectangleShape_setTextureRect(en_rectangleShape, animatorReverse("chameleonAttack", &_enemy->frameX, &_enemy->timerAnim, 0, 0));
		_enemy->size = vector2f(88.f, 92.f);
		break;
	case EN_STATE_BACK_HOME:
		sfRectangleShape_setTextureRect(en_rectangleShape, animatorAnim("chameleonDetecting", &_enemy->frameX, &_enemy->timerAnim, 0));
		_enemy->size = vector2f(88.f, 80.f);
		break;
	case EN_STATE_DAMAGING:
		sfRectangleShape_setTextureRect(en_rectangleShape, IntRect(0, 78, 86, 78));
		_enemy->size = vector2f(86.f, 78.f);
		break;
	default:
		sfRectangleShape_setTextureRect(en_rectangleShape, IntRect(0, 0, 88, 78));
		_enemy->size = vector2f(88.f, 78.f);
		break;
	}

	sfRectangleShape_setSize(en_rectangleShape, _enemy->size);

	sfRectangleShape_setPosition(en_rectangleShape, _enemy->pos);
}

void en_DisplayBat(sfRenderTexture* _rdrt, EnemiesStruct* _enemy, sfVector2f _tmpPpos)
{

	sfVector2f offSetPos = NULLVECTF;
	if (_enemy->forward.x > 0.0f)
	{
		sfRectangleShape_setScale(en_rectangleShape, vector2f(-1, 1));
	}
	else
		sfRectangleShape_setScale(en_rectangleShape, vector2f(1, 1));
	switch (_enemy->state)
	{
	case EN_STATE_WAIT:
		if (_enemy->isOnRoof)
		{
			sfRectangleShape_setTextureRect(en_rectangleShape, animatorAnim("bat_SLEEP", &_enemy->frameX, &_enemy->timerAnim, 0));
			_enemy->size = vector2f(50.f, 80.f);
		}
		else
		{
			offSetPos.x += 10.0f;
			sfRectangleShape_setTextureRect(en_rectangleShape, animatorAnim("bat_FALL", &_enemy->frameX, &_enemy->timerAnim, 0));
			_enemy->size = vector2f(98.f, 80.f);
		}
		break;
	case EN_STATE_WALK:
		sfRectangleShape_setTextureRect(en_rectangleShape, animatorAnim("bat_IDLE", &_enemy->frameX, &_enemy->timerAnim, 0));
		_enemy->size = vector2f(98.f, 54.f);
		break;
	case EN_STATE_CHASE:
		offSetPos.x -= 10.0f;
		if (_enemy->forward.x > 0.0f)
		{
			offSetPos.x = -offSetPos.x;
			sfRectangleShape_setScale(en_rectangleShape, vector2f(-1, 1));
		}
		else
			sfRectangleShape_setScale(en_rectangleShape, vector2f(-1, -1));
		sfRectangleShape_setTextureRect(en_rectangleShape, animatorAnim("bat_ATTACK", &_enemy->frameX, &_enemy->timerAnim, 0));
		_enemy->size = vector2f(94.f, 92.f);
		break;
	case EN_STATE_BACK_HOME:
		sfRectangleShape_setTextureRect(en_rectangleShape, animatorAnim("bat_IDLE", &_enemy->frameX, &_enemy->timerAnim, 0));
		_enemy->size = vector2f(98.f, 54.f);
		break;
	case EN_STATE_ATTACK:
		//if (_enemy->forward.x < 0.0f)
		//	sfRectangleShape_setScale(en_rectangleShape, vector2f(-1, 1));
		//else
		//	sfRectangleShape_setScale(en_rectangleShape, vector2f(1, 1));
	//	sfRectangleShape_setRotation(en_rectangleShape, _enemy->rotation);
		sfRectangleShape_setTextureRect(en_rectangleShape, animatorAnim("bat_ATTACK", &_enemy->frameX, &_enemy->timerAnim, 0));
		_enemy->size = vector2f(94.f, 92.f);
		break;
	case EN_STATE_DAMAGING:
		sfRectangleShape_setTextureRect(en_rectangleShape, IntRect(0, 134, 66, 50));
		_enemy->size = vector2f(66.f, 50.f);
		break;
	default:
		sfRectangleShape_setTextureRect(en_rectangleShape, IntRect(0, 80, 98, 54));
		_enemy->size = vector2f(98.f, 54.f);
		break;
	}

	sfRectangleShape_setRotation(en_rectangleShape, _enemy->rotation);
	sfRectangleShape_setSize(en_rectangleShape, _enemy->size);
	sfRectangleShape_setPosition(en_rectangleShape, addVectorsrf(_enemy->pos, offSetPos));
	_enemy->size = vector2f(98.f, 54.f);
	_enemy->rotation = 0.f;


}

void en_DisplayLittleGhostJoy(sfRenderTexture* _rdrt, EnemiesStruct* _enemy, sfVector2f _tmpPpos)
{

	sfVector2f offsetPos = NULLVECTF;
	switch (_enemy->state)
	{
	case EN_STATE_WALK:
		sfRectangleShape_setSize(en_rectangleShape, vector2f(110.0f, 130.f));
		_enemy->size = vector2f(110.0f, 130.f);
		sfRectangleShape_setTextureRect(en_rectangleShape, animatorAnim("littleJoyWalk", &_enemy->frameX, &_enemy->timerAnim, 0));
		break;
	case EN_STATE_DETECTING:

		break;
	case EN_STATE_CHASE:
		sfRectangleShape_setSize(en_rectangleShape, vector2f(110.0f, 130.f));
		_enemy->size = vector2f(110.0f, 130.f);
		sfRectangleShape_setTextureRect(en_rectangleShape, animatorAnim("littleJoyWalk", &_enemy->frameX, &_enemy->timerAnim, 0));
		break;
	case EN_STATE_BACK_HOME:

		break;
	case EN_STATE_ATTACK:
		offsetPos.x -= 20.0f;
		offsetPos.y += 5.f;
		sfRectangleShape_setSize(en_rectangleShape, vector2f(132.f, 126.f));
		_enemy->size = vector2f(132.0f, 126.f);
		sfRectangleShape_setTextureRect(en_rectangleShape, animatorAnim("littleJoyAttack", &_enemy->frameX, &_enemy->timerAnim, 0));
		break;
	case EN_STATE_DAMAGING:
		sfRectangleShape_setTextureRect(en_rectangleShape, IntRect(0, 403, 110, 126));
		sfRectangleShape_setSize(en_rectangleShape, vector2f(110.f, 126.f));
		_enemy->size = vector2f(110.0f, 126.f);
		break;
	case EN_STATE_DEAD:
		sfRectangleShape_setTextureRect(en_rectangleShape, animatorAnim("littleJoyDead", &_enemy->frameX, &_enemy->timerAnim, 0));
		sfRectangleShape_setSize(en_rectangleShape, vector2f(118.f, 146.f));
		_enemy->size = vector2f(118.0f, 146.f);
		break;
	default:
		sfRectangleShape_setTextureRect(en_rectangleShape, IntRect(0, 0, 110, 130));
		sfRectangleShape_setSize(en_rectangleShape, vector2f(110.f, 130.f));
		_enemy->size = vector2f(110.0f, 130.f);
		break;
	}


	if (_enemy->currentVelocity.x > 0.5f)
	{
		sfRectangleShape_setScale(en_rectangleShape, vector2f(-_enemy->scale.x, _enemy->scale.y));
		offsetPos.x = -offsetPos.x;
	}
	else
		sfRectangleShape_setScale(en_rectangleShape, _enemy->scale);
	sfRectangleShape_setPosition(en_rectangleShape, addVectorsrf(_enemy->pos, offsetPos));
}

void en_DisplayBigGhostJoy(sfRenderTexture* _rdrt, EnemiesStruct* _enemy, sfVector2f _tmpPpos)
{

	sfVector2f offsetPos = NULLVECTF;

	switch (_enemy->state)
	{
	case EN_STATE_WAIT:
	case EN_STATE_WALK:
		sfRectangleShape_setTextureRect(en_rectangleShape, animatorAnim("bigGhostJoyIdle", &_enemy->frameX, &_enemy->timerAnim, 0));
		_enemy->size = vector2f(150.f, 234.f);
		break;
	case EN_STATE_DETECTING:
		sfRectangleShape_setTextureRect(en_rectangleShape, animatorAnim("bigGhostJoyDetecting", &_enemy->frameX, &_enemy->timerAnim, 0));
		_enemy->size = vector2f(164.f, 228.f);
		offsetPos.x -= 14.f;
		break;
	case EN_STATE_CHASE:
		sfRectangleShape_setTexture(en_rectangleShape, en_Textures[EN_BIG_GHOST_JOY], sfFalse);
		sfRectangleShape_setPosition(en_rectangleShape, addVectorsrf(_enemy->pos, vector2f(0.f, 100.f)));
		sfRectangleShape_setTextureRect(en_rectangleShape, IntRect(0, 672, 302, 302));
		sfRectangleShape_setOrigin(en_rectangleShape, vector2f(151.f, 151.f));
		sfRectangleShape_setSize(en_rectangleShape, vector2f(302.f, 302.f));
		sfRectangleShape_setScale(en_rectangleShape, multiplyVectorsrf(_enemy->scale, cosf(_enemy->timerAllAttacks * 1.f)));
		sfRectangleShape_setFillColor(en_rectangleShape, color(255, 255, 255, (sfUint8)fabs((cosf(_enemy->timerAllAttacks * 1.f)) * 255.f)));
		sfRenderTexture_drawRectangleShape(_rdrt, en_rectangleShape, NULL);

		sfRectangleShape_setFillColor(en_rectangleShape, color(255, 255, 255, 255));
		sfRectangleShape_setOrigin(en_rectangleShape, _enemy->origin);
		sfRectangleShape_setTextureRect(en_rectangleShape, animatorAnim("bigGhostJoyChase", &_enemy->frameX, &_enemy->timerAnim, 0));
		_enemy->size = vector2f(156.f, 210.f);
		offsetPos.x -= 18.f;
		break;
	case EN_STATE_ATTRACTING:
		sfRectangleShape_setTextureRect(en_rectangleShape, animatorAnim("bigGhostJoyAttracting", &_enemy->frameX, &_enemy->timerAnim, 0));
		_enemy->size = vector2f(186.f, 188.f);
		offsetPos.x -= 46.f;
		offsetPos.y += 22.f;
		break;
	case EN_STATE_PLR_RUSHING:
		sfRectangleShape_setTextureRect(en_rectangleShape, IntRect(0, 1162, 172, 204));
		_enemy->size = vector2f(172.f, 204.f);
		offsetPos.y += 10.f;
		break;
	case EN_STATE_STUNNED:
		sfRectangleShape_setTextureRect(en_rectangleShape, animatorAnim("bigGhostJoyStunned", &_enemy->frameX, &_enemy->timerAnim, 0));
		_enemy->size = vector2f(166.f, 190.f);
		offsetPos.x -= 20.f;
		offsetPos.y += 46.f;
		break;
	case EN_STATE_DAMAGING:
		sfRectangleShape_setTextureRect(en_rectangleShape, IntRect(0, 1556, 142, 196));
		_enemy->size = vector2f(142.f, 196.f);
		offsetPos.x += 12.f;
		offsetPos.y += 24.f;
		break;
	case EN_STATE_DEAD:
		sfRectangleShape_setTextureRect(en_rectangleShape, animatorAnim("bigGhostJoyDead", &_enemy->frameX, &_enemy->timerAnim, 0));
		_enemy->size = vector2f(142.f, 196.f);
		offsetPos.x += 4.f;
		offsetPos.y += 20.f;
		break;
	default:
		sfRectangleShape_setTextureRect(en_rectangleShape, IntRect(0, 0, 150, 234));
		_enemy->size = vector2f(150.f, 234.f);
		break;
	}

	sfRectangleShape_setSize(en_rectangleShape, _enemy->size);

	if (_enemy->currentVelocity.x > 0.f || (_enemy->state == EN_STATE_DETECTING && _tmpPpos.x > _enemy->pos.x)) {
		sfRectangleShape_setScale(en_rectangleShape, vector2f(-_enemy->scale.x, _enemy->scale.y));
		offsetPos.x = -offsetPos.x;
	}
	else
		sfRectangleShape_setScale(en_rectangleShape, _enemy->scale);

	sfRectangleShape_setPosition(en_rectangleShape, addVectorsrf(_enemy->pos, offsetPos));
}

void en_DisplayLittleGhostSadness(sfRenderTexture* _rdrt, EnemiesStruct* _enemy, sfVector2f _tmpPpos)
{

	sfVector2f offsetPos = NULLVECTF;
	switch (_enemy->state)
	{
	case EN_STATE_WAIT:
		sfRectangleShape_setSize(en_rectangleShape, vector2f(108.0f, 90.f));
		_enemy->size = vector2f(108.0f, 90.f);
		sfRectangleShape_setTextureRect(en_rectangleShape, animatorAnim("SmallSadMove", &_enemy->frameX, &_enemy->timerAnim, 0));
		break;
	case EN_STATE_WALK:
		sfRectangleShape_setSize(en_rectangleShape, vector2f(108.0f, 90.f));
		_enemy->size = vector2f(108.0f, 90.f);
		sfRectangleShape_setTextureRect(en_rectangleShape, animatorAnim("SmallSadMove", &_enemy->frameX, &_enemy->timerAnim, 0));
		break;
	case EN_STATE_CHASE:
		sfRectangleShape_setSize(en_rectangleShape, vector2f(108.0f, 90.f));
		_enemy->size = vector2f(108.0f, 90.f);
		sfRectangleShape_setTextureRect(en_rectangleShape, animatorAnim("SmallSadMove", &_enemy->frameX, &_enemy->timerAnim, 0));
		break;
	case EN_STATE_ATTACK:
		offsetPos.x -= 30.0f;
		offsetPos.y -= 10.f;
		sfRectangleShape_setSize(en_rectangleShape, vector2f(132.f, 103.f));
		_enemy->size = vector2f(132.0f, 103.f);
		sfRectangleShape_setTextureRect(en_rectangleShape, animatorAnim("SmallSadAttack", &_enemy->frameX, &_enemy->timerAnim, 0));
		break;
	case EN_STATE_THROWN:
		//offsetPos.y -= 20.0f;
		sfRectangleShape_setTextureRect(en_rectangleShape, IntRect(0, 0, 108, 90));
		sfRectangleShape_setSize(en_rectangleShape, vector2f(108.f, 90.f));
		_enemy->size = vector2f(108.0f, 90.f);
		break;
	case EN_STATE_STUNNED:
		sfRectangleShape_setTextureRect(en_rectangleShape, IntRect(0, 0, 108, 90));
		sfRectangleShape_setSize(en_rectangleShape, vector2f(108.f, 90.f));
		_enemy->size = vector2f(108.0f, 90.f);
		break;
	case EN_STATE_DAMAGING:
		offsetPos.y -= 20.0f;
		sfRectangleShape_setTextureRect(en_rectangleShape, IntRect(0, 345, 108, 112));
		sfRectangleShape_setSize(en_rectangleShape, vector2f(108.f, 112.f));
		_enemy->size = vector2f(108.0f, 112.f);
		break;
	case EN_STATE_DEAD:
		offsetPos.y -= 20.0f;
		sfRectangleShape_setTextureRect(en_rectangleShape, animatorAnim("SmallSadDead", &_enemy->frameX, &_enemy->timerAnim, 0));
		sfRectangleShape_setSize(en_rectangleShape, vector2f(124.f, 136.f));
		_enemy->size = vector2f(124.0f, 136.f);
		break;
	default:
		sfRectangleShape_setTextureRect(en_rectangleShape, IntRect(0, 0, 108, 90));
		sfRectangleShape_setSize(en_rectangleShape, vector2f(108.f, 90.f));
		_enemy->size = vector2f(108.0f, 90.f);
		break;
	}


	if (_enemy->currentVelocity.x > 0.5f)
	{
		sfRectangleShape_setScale(en_rectangleShape, vector2f(-_enemy->scale.x, _enemy->scale.y));
		offsetPos.x = -offsetPos.x;
	}
	else
		sfRectangleShape_setScale(en_rectangleShape, _enemy->scale);
	sfRectangleShape_setPosition(en_rectangleShape, addVectorsrf(_enemy->pos, offsetPos));
}

void en_DisplayBigGhostSadness(sfRenderTexture* _rdrt, EnemiesStruct* _enemy, sfVector2f _tmpPpos)
{

	sfVector2f offsetPos = NULLVECTF;

	if (_tmpPpos.x < _enemy->pos.x)
		sfRectangleShape_setScale(en_rectangleShape, vector2f(-_enemy->scale.x, _enemy->scale.y));
	else
		sfRectangleShape_setScale(en_rectangleShape, _enemy->scale);

	sfRectangleShape_setSize(en_rectangleShape, vector2f(180.f, 178.f));
	_enemy->size = vector2f(180.0f, 178.f);

	switch (_enemy->state)
	{
	case EN_STATE_WAIT:
		sfRectangleShape_setSize(en_rectangleShape, vector2f(180.f, 178.f));
		_enemy->size = vector2f(180.0f, 178.f);
		sfRectangleShape_setTextureRect(en_rectangleShape, animatorAnim("bigSadIdle", &_enemy->frameX, &_enemy->timerAnim, 0));
		break;
	case EN_STATE_WALK:
		sfRectangleShape_setSize(en_rectangleShape, vector2f(180.f, 178.f));
		_enemy->size = vector2f(180.0f, 178.f);
		sfRectangleShape_setTextureRect(en_rectangleShape, animatorAnim("bigSadIdle", &_enemy->frameX, &_enemy->timerAnim, 0));
		break;
	case EN_STATE_CHASE:
		if (_enemy->frameX >= 6 && _enemy->frameX <= 7)
		{
			_enemy->timerTears += 610.0f * getDeltaTime();
			sfRectangleShape_setFillColor(en_rectangleShape, sfColor_fromRGBA(255, 255, 255, (sfUint8)_enemy->timerTears));
			sfRectangleShape_setSize(en_rectangleShape, vector2f(302.0f, 302.0f));
			offsetPos.x -= 50.0f;
			offsetPos.y -= 30.0f;

			if (_enemy->currentVelocity.x > 0.5f || (_enemy->state == EN_STATE_DETECTING && _tmpPpos.x > _enemy->pos.x)) {
				sfRectangleShape_setScale(en_rectangleShape, vector2f(-_enemy->scale.x, _enemy->scale.y));
				offsetPos.x = -offsetPos.x;
			}
			else

				sfRectangleShape_setScale(en_rectangleShape, _enemy->scale);

			sfRectangleShape_setPosition(en_rectangleShape, addVectorsrf(_enemy->pos, offsetPos));
			sfRectangleShape_setTexture(en_rectangleShape, en_Textures[EN_BIG_GHOST_SADNESS], sfTrue);
			sfRectangleShape_setTextureRect(en_rectangleShape, IntRect(0, 766, 302, 302));
			sfRenderTexture_drawRectangleShape(_rdrt, en_rectangleShape, NULL);
		}
		offsetPos = NULLVECTF;
		offsetPos.y -= 20.0f;
		offsetPos.x -= 10.0f;
		sfRectangleShape_setFillColor(en_rectangleShape, sfColor_fromRGBA(255, 255, 255, 255));

		sfRectangleShape_setSize(en_rectangleShape, vector2f(188.f, 203.f));
		_enemy->size = vector2f(188.0f, 203.f);
		sfRectangleShape_setTextureRect(en_rectangleShape, animatorAnim("bigSadRepulse2", &_enemy->frameX, &_enemy->timerAnim, 0));
		break;
	case EN_STATE_THROWN:
		sfRectangleShape_setTextureRect(en_rectangleShape, IntRect(0, 0, 180, 178));
		sfRectangleShape_setSize(en_rectangleShape, vector2f(180.f, 178.f));
		_enemy->size = vector2f(180.0f, 178.f);
		break;
	case EN_STATE_STUNNED:
		sfRectangleShape_setTextureRect(en_rectangleShape, IntRect(0, 0, 180, 178));
		sfRectangleShape_setSize(en_rectangleShape, vector2f(180.f, 178.f));
		_enemy->size = vector2f(180.0f, 178.f);
		break;
	case EN_STATE_CRYING:
		sfRectangleShape_setSize(en_rectangleShape, vector2f(158.f, 148.f));
		_enemy->size = vector2f(158.0f, 148.f);
		sfRectangleShape_setTextureRect(en_rectangleShape, animatorAnim("bigSadCry", &_enemy->frameX, &_enemy->timerAnim, 0));
		break;
	case EN_STATE_ATTRACTING:
		if (_enemy->frameX >= 3 && _enemy->frameX <= 4)
		{
			_enemy->timerTears += 610.0f * getDeltaTime();
			sfRectangleShape_setTexture(en_rectangleShape, en_Textures[EN_BIG_GHOST_SADNESS], sfFalse);
			sfRectangleShape_setFillColor(en_rectangleShape, sfColor_fromRGBA(255, 255, 255, (sfUint8)_enemy->timerTears));
			sfRectangleShape_setSize(en_rectangleShape, vector2f(302.0f, 302.0f));
			sfRectangleShape_setTextureRect(en_rectangleShape, IntRect(0, 766, 302, 302));
			offsetPos.x -= 50.0f;
			offsetPos.y -= 30.0f;

			if (_enemy->currentVelocity.x > 0.5f || (_enemy->state == EN_STATE_DETECTING && _tmpPpos.x > _enemy->pos.x))
			{
				sfRectangleShape_setScale(en_rectangleShape, vector2f(-_enemy->scale.x, _enemy->scale.y));
				offsetPos.x = -offsetPos.x;
			}
			else
				sfRectangleShape_setScale(en_rectangleShape, _enemy->scale);

			sfRectangleShape_setPosition(en_rectangleShape, addVectorsrf(_enemy->pos, offsetPos));
			sfRenderTexture_drawRectangleShape(_rdrt, en_rectangleShape, NULL);
		}
		offsetPos = NULLVECTF;
		sfRectangleShape_setFillColor(en_rectangleShape, sfColor_fromRGBA(255, 255, 255, 255));
		sfRectangleShape_setSize(en_rectangleShape, vector2f(196.f, 174.f));
		_enemy->size = vector2f(196.0f, 174.f);
		offsetPos.y += 5.0f;
		offsetPos.x -= 20.0f;
		sfRectangleShape_setTextureRect(en_rectangleShape, animatorAnim("bigSadRepulse1", &_enemy->frameX, &_enemy->timerAnim, 0));
		break;
	case EN_STATE_DAMAGING:
		offsetPos.y += 10.0f;
		sfRectangleShape_setTextureRect(en_rectangleShape, IntRect(0, 1248, 188, 136));
		sfRectangleShape_setSize(en_rectangleShape, vector2f(188.f, 136.f));
		_enemy->size = vector2f(188.0f, 136.f);
		break;
	case EN_STATE_DEAD:

		sfRectangleShape_setTextureRect(en_rectangleShape, animatorAnim("bigSadDead", &_enemy->frameX, &_enemy->timerAnim, 0));
		sfRectangleShape_setSize(en_rectangleShape, vector2f(188.f, 136.f));
		_enemy->size = vector2f(188.0f, 136.f);
		offsetPos.y += 10.0f;
		break;
	default:
		sfRectangleShape_setTextureRect(en_rectangleShape, IntRect(0, 0, 180, 178));
		sfRectangleShape_setSize(en_rectangleShape, vector2f(180.f, 178.f));
		_enemy->size = vector2f(180.0f, 178.f);
		break;
	}

	if (_enemy->currentVelocity.x > 0.5f || (_enemy->state == EN_STATE_DETECTING && _tmpPpos.x > _enemy->pos.x)) {
		sfRectangleShape_setScale(en_rectangleShape, vector2f(-_enemy->scale.x, _enemy->scale.y));
		offsetPos.x = -offsetPos.x;
	}
	else
		sfRectangleShape_setScale(en_rectangleShape, _enemy->scale);
	sfRectangleShape_setPosition(en_rectangleShape, addVectorsrf(_enemy->pos, offsetPos));
}

void en_DisplayChameleonTongue(sfRenderTexture* _rdrt, EnemiesStruct* _enemy)
{
	sfRectangleShape_setTexture(en_rectangleShape, en_Textures[EN_CHAMELEON], sfFalse);

	float tongueScale = en_GetChameleonTongueScale(_enemy);
	sfVector2f tongueOffsetPos = en_GetAppropriateVectorRotation(vector2f(13.f, 42.f), _enemy->rotation);

	sfRectangleShape_setTextureRect(en_rectangleShape, IntRect(0, 408, 4, 20));
	sfRectangleShape_setPosition(en_rectangleShape, addVectorsrf(_enemy->pos, tongueOffsetPos));
	sfRectangleShape_setSize(en_rectangleShape, vector2f(4.f, 20.f));
	sfRectangleShape_setOrigin(en_rectangleShape, vector2f(2.f, 0.f));
	sfRectangleShape_setRotation(en_rectangleShape, _enemy->rotation);
	sfRectangleShape_setScale(en_rectangleShape, vector2f(_enemy->scale.x, tongueScale));


	sfRenderTexture_drawRectangleShape(_rdrt, en_rectangleShape, NULL);

	sfRectangleShape_setTextureRect(en_rectangleShape, IntRect(0, 428, 20, 22));
	sfRectangleShape_setPosition(en_rectangleShape, addVectorsrf(addVectorsrf(_enemy->pos, tongueOffsetPos), en_GetAppropriateVectorRotation(vector2f(0.f, tongueScale * 20.f), _enemy->rotation)));
	sfRectangleShape_setSize(en_rectangleShape, vector2f(20.f, 22.f));
	sfRectangleShape_setOrigin(en_rectangleShape, vector2f(10.f, 0.f));
	sfRectangleShape_setRotation(en_rectangleShape, _enemy->rotation);
	sfRectangleShape_setScale(en_rectangleShape, _enemy->scale);
	sfRenderTexture_drawRectangleShape(_rdrt, en_rectangleShape, NULL);

	sfRectangleShape_setRotation(en_rectangleShape, 0.f);
}

void en_DisplayImunity(sfRenderTexture* _rdrt, EnemiesStruct* _enemy, sfVector2f _tmpPpos)
{
	sfRectangleShape_setPosition(en_rectangleShape, _enemy->pos);
	switch (_enemy->state)
	{
	case EN_STATE_WAIT:
		sfRectangleShape_setFillColor(en_rectangleShape, color(255, 255, 255, 255));
		_enemy->size = vector2f(146.f, 156.f);
		break;
	case EN_STATE_BACK_HOME:
		sfRectangleShape_setFillColor(en_rectangleShape, color(0, 0, 255, 255));
		_enemy->size = vector2f(146.f, 156.f);
		break;
	case EN_STATE_ATTACK:
		sfRectangleShape_setFillColor(en_rectangleShape, color(255, 0, 0, 255));
		_enemy->size = vector2f(146.f, 156.f);
		break;
	case EN_STATE_FALLING:
		sfRectangleShape_setFillColor(en_rectangleShape, color(0, 255, 0, 255));
		_enemy->size = vector2f(146.f, 156.f);
		break;
	case EN_STATE_THROWN:
		sfRectangleShape_setFillColor(en_rectangleShape, color(255, 255, 0, 255));
		_enemy->size = vector2f(146.f, 156.f);
		break;
	case EN_STATE_CHASE:
		sfRectangleShape_setFillColor(en_rectangleShape, color(128, 255, 56, 255));
		_enemy->size = vector2f(146.f, 156.f);
		break;
	}
	sfRectangleShape_setSize(en_rectangleShape, _enemy->size);
}

void en_DisplaySeagull(sfRenderTexture* _rdrt, EnemiesStruct* _enemy, sfVector2f _tmpPpos)
{
	sfRectangleShape_setFillColor(en_rectangleShape, color(255, 255, 255, 255));
	sfVector2f offsetPos = NULLVECTF;
	switch (_enemy->state)
	{
	case EN_STATE_WAIT:
		sfRectangleShape_setSize(en_rectangleShape, vector2f(100.0f, 60.f));
		_enemy->size = vector2f(100.0f, 60.f);
		sfRectangleShape_setTextureRect(en_rectangleShape, animatorAnim("seagullIdle", &_enemy->frameX, &_enemy->timerAnim, 0));
		break;
	case EN_STATE_EAT:
		sfRectangleShape_setSize(en_rectangleShape, vector2f(100.0f, 62.f));
		_enemy->size = vector2f(100.0f, 62.f);
		sfRectangleShape_setTextureRect(en_rectangleShape, animatorAnim("seagullEat", &_enemy->frameX, &_enemy->timerAnim, 0));
		break;
	case EN_STATE_GOING_TO_FLY:
		offsetPos.x -= 4.0f;
		offsetPos.y -= 12.0f;
		sfRectangleShape_setSize(en_rectangleShape, vector2f(110.0f, 72.f));
		_enemy->size = vector2f(110.0f, 72.f);
		sfRectangleShape_setTextureRect(en_rectangleShape, animatorAnim("seagullToFly", &_enemy->frameX, &_enemy->timerAnim, 0));
		break;
	case EN_STATE_FLY:
		offsetPos.y -= 12.0f;
		sfRectangleShape_setSize(en_rectangleShape, vector2f(108.0f, 66.f));
		_enemy->size = vector2f(108.0f, 66.f);
		sfRectangleShape_setTextureRect(en_rectangleShape, animatorAnim("seagullFly1", &_enemy->frameX, &_enemy->timerAnim, 0));
		break;
	case EN_STATE_FLY2:
		offsetPos.y -= 12.0f;
		sfRectangleShape_setSize(en_rectangleShape, vector2f(108.0f, 66.f));
		_enemy->size = vector2f(108.0f, 66.f);
		sfRectangleShape_setTextureRect(en_rectangleShape, animatorAnim("seagullFly2", &_enemy->frameX, &_enemy->timerAnim, 0));
		break;
	default:
		sfRectangleShape_setSize(en_rectangleShape, vector2f(100.0f, 62.f));
		_enemy->size = vector2f(100.0f, 62.f);
		sfRectangleShape_setTextureRect(en_rectangleShape, IntRect(0, 0, 100, 62));
		break;
	}


	if (_enemy->haveToHome)
	{
		sfRectangleShape_setScale(en_rectangleShape, vector2f(-_enemy->scale.x, _enemy->scale.y));
		offsetPos.x = -offsetPos.x;
	}
	else
		sfRectangleShape_setScale(en_rectangleShape, _enemy->scale);
	sfRectangleShape_setPosition(en_rectangleShape, addVectorsrf(_enemy->pos, offsetPos));
}

//Shadows mobs
void en_DisplayYoyoSpiderShadow(sfRenderTexture* _rdrt, EnemiesStruct* _enemy)
{
	if (_enemy->state == EN_STATE_BACK_HOME && _enemy->frameX > 0)
	{
		_enemy->time_detection += getDeltaTime();
		if (_enemy->currentVelocity.x > 0.5f)
			sfRectangleShape_setPosition(en_rectangleShape, vector2f(_enemy->pos.x - 22.0f, _enemy->pos.y + 180.0f));
		else
			sfRectangleShape_setPosition(en_rectangleShape, vector2f(_enemy->pos.x - 20.f, _enemy->pos.y + 180.0f));

		sfRectangleShape_setTexture(en_rectangleShape, en_TextureShadow, sfTrue);
		sfRectangleShape_setScale(en_rectangleShape, vector2f(1.f, 1.f));
		sfRectangleShape_setTextureRect(en_rectangleShape, IntRect(0, 0, 102, 36));
		sfRectangleShape_setSize(en_rectangleShape, vector2f(102, 36));
		sfRectangleShape_setFillColor(en_rectangleShape, color_lerp(color(255, 255, 255, 0), color(255, 255, 255, 255), _enemy->time_detection / 1.2f));
		sfRenderTexture_drawRectangleShape(_rdrt, en_rectangleShape, NULL);
	}
	else if (!_enemy->isOnString && _enemy->isFallingAnimDone && _enemy->state == EN_STATE_BACK_HOME)
	{
		if (_enemy->currentVelocity.x > 0.5f)
			sfRectangleShape_setPosition(en_rectangleShape, vector2f(_enemy->pos.x - 22.0f, _enemy->pos.y + 180.0f));
		else
			sfRectangleShape_setPosition(en_rectangleShape, vector2f(_enemy->pos.x - 20.f, _enemy->pos.y + 180.0f));

		sfRectangleShape_setTexture(en_rectangleShape, en_TextureShadow, sfTrue);
		sfRectangleShape_setScale(en_rectangleShape, vector2f(1.f, 1.f));
		sfRectangleShape_setTextureRect(en_rectangleShape, IntRect(0, 0, 102, 36));
		sfRectangleShape_setFillColor(en_rectangleShape, color(255, 255, 255, 255));
		sfRectangleShape_setSize(en_rectangleShape, vector2f(102, 36));

		sfRenderTexture_drawRectangleShape(_rdrt, en_rectangleShape, NULL);
	}
	else if (!_enemy->isOnString && _enemy->isFallingAnimDone /*_enemy->time_detection/0.8 >= 1.f*/)
	{
		if (_enemy->currentVelocity.x > 0.5f)
			sfRectangleShape_setPosition(en_rectangleShape, vector2f(_enemy->pos.x - 22.0f, _enemy->pos.y + 30.0f));
		else
			sfRectangleShape_setPosition(en_rectangleShape, vector2f(_enemy->pos.x - 20.f, _enemy->pos.y + 30.0f));

		sfRectangleShape_setTexture(en_rectangleShape, en_TextureShadow, sfTrue);
		sfRectangleShape_setScale(en_rectangleShape, vector2f(1.f, 1.f));
		sfRectangleShape_setTextureRect(en_rectangleShape, IntRect(0, 0, 102, 36));
		sfRectangleShape_setFillColor(en_rectangleShape, color(255, 255, 255, 255));
		sfRectangleShape_setSize(en_rectangleShape, vector2f(102, 36));

		sfRenderTexture_drawRectangleShape(_rdrt, en_rectangleShape, NULL);
	}
}
void en_DisplayVenimousYoyoSpiderShadow(sfRenderTexture* _rdrt, EnemiesStruct* _enemy)
{
	if (_enemy->state == EN_STATE_BACK_HOME && _enemy->frameX > 0)
	{
		_enemy->time_detection += getDeltaTime();
		if (_enemy->currentVelocity.x > 0.5f)
			sfRectangleShape_setPosition(en_rectangleShape, vector2f(_enemy->pos.x - 22.0f, _enemy->pos.y + 180.0f));
		else
			sfRectangleShape_setPosition(en_rectangleShape, vector2f(_enemy->pos.x - 20.f, _enemy->pos.y + 180.0f));

		sfRectangleShape_setTexture(en_rectangleShape, en_TextureShadow, sfTrue);
		sfRectangleShape_setScale(en_rectangleShape, vector2f(1.f, 1.f));
		sfRectangleShape_setTextureRect(en_rectangleShape, IntRect(0, 0, 102, 36));
		sfRectangleShape_setSize(en_rectangleShape, vector2f(102, 36));
		sfRectangleShape_setFillColor(en_rectangleShape, color_lerp(color(255, 255, 255, 0), color(255, 255, 255, 255), _enemy->time_detection / 1.2f));
		sfRenderTexture_drawRectangleShape(_rdrt, en_rectangleShape, NULL);
	}
	else if (!_enemy->isOnString && _enemy->isFallingAnimDone && _enemy->state == EN_STATE_BACK_HOME)
	{
		if (_enemy->currentVelocity.x > 0.5f)
			sfRectangleShape_setPosition(en_rectangleShape, vector2f(_enemy->pos.x - 22.0f, _enemy->pos.y + 180.0f));
		else
			sfRectangleShape_setPosition(en_rectangleShape, vector2f(_enemy->pos.x - 20.f, _enemy->pos.y + 180.0f));

		sfRectangleShape_setTexture(en_rectangleShape, en_TextureShadow, sfTrue);
		sfRectangleShape_setScale(en_rectangleShape, vector2f(1.f, 1.f));
		sfRectangleShape_setTextureRect(en_rectangleShape, IntRect(0, 0, 102, 36));
		sfRectangleShape_setFillColor(en_rectangleShape, color(255, 255, 255, 255));
		sfRectangleShape_setSize(en_rectangleShape, vector2f(102, 36));

		sfRenderTexture_drawRectangleShape(_rdrt, en_rectangleShape, NULL);
	}
	else if (!_enemy->isOnString && _enemy->isFallingAnimDone /*_enemy->time_detection/0.8 >= 1.f*/)
	{
		if (_enemy->currentVelocity.x > 0.5f)
			sfRectangleShape_setPosition(en_rectangleShape, vector2f(_enemy->pos.x - 22.0f, _enemy->pos.y + 30.0f));
		else
			sfRectangleShape_setPosition(en_rectangleShape, vector2f(_enemy->pos.x - 20.f, _enemy->pos.y + 30.0f));

		sfRectangleShape_setTexture(en_rectangleShape, en_TextureShadow, sfTrue);
		sfRectangleShape_setScale(en_rectangleShape, vector2f(1.f, 1.f));
		sfRectangleShape_setTextureRect(en_rectangleShape, IntRect(0, 0, 102, 36));
		sfRectangleShape_setFillColor(en_rectangleShape, color(255, 255, 255, 255));
		sfRectangleShape_setSize(en_rectangleShape, vector2f(102, 36));

		sfRenderTexture_drawRectangleShape(_rdrt, en_rectangleShape, NULL);
	}
}
void en_DisplayCrabShadow(sfRenderTexture* _rdrt, EnemiesStruct* _enemy)
{
	if (_enemy->state != EN_STATE_CHASE && _enemy->state != EN_STATE_BACK_HOME)
	{
		if (_enemy->currentVelocity.x > 0.5f)
			sfRectangleShape_setPosition(en_rectangleShape, vector2f(_enemy->pos.x - 20.0f, _enemy->pos.y + 30.0f));
		else
			sfRectangleShape_setPosition(en_rectangleShape, vector2f(_enemy->pos.x - 18.f, _enemy->pos.y + 30.0f));

		sfRectangleShape_setTexture(en_rectangleShape, en_TextureShadow, sfTrue);
		sfRectangleShape_setScale(en_rectangleShape, vector2f(1.f, 1.f));
		sfRectangleShape_setTextureRect(en_rectangleShape, IntRect(0, 0, 102, 36));
		sfRectangleShape_setSize(en_rectangleShape, vector2f(102, 36));
		sfRenderTexture_drawRectangleShape(_rdrt, en_rectangleShape, NULL);
	}
	else if (_enemy->state == EN_STATE_BACK_HOME && _enemy->frameX < 9)
	{
		_enemy->time_detection += getDeltaTime();

		if (_enemy->currentVelocity.x > 0.5f)
			sfRectangleShape_setPosition(en_rectangleShape, vector2f(_enemy->pos.x - 20.0f, _enemy->pos.y + 30.0f));
		else
			sfRectangleShape_setPosition(en_rectangleShape, vector2f(_enemy->pos.x - 18.f, _enemy->pos.y + 30.0f));

		sfRectangleShape_setTexture(en_rectangleShape, en_TextureShadow, sfTrue);
		sfRectangleShape_setScale(en_rectangleShape, vector2f(1.f, 1.f));
		sfRectangleShape_setTextureRect(en_rectangleShape, IntRect(0, 0, 102, 36));
		sfRectangleShape_setSize(en_rectangleShape, vector2f(102, 36));
		sfRectangleShape_setFillColor(en_rectangleShape, color_lerp(color(255, 255, 255, 255), color(255, 255, 255, 0), _enemy->time_detection / 1.8f));
		sfRenderTexture_drawRectangleShape(_rdrt, en_rectangleShape, NULL);
	}
	else if (_enemy->state == EN_STATE_CHASE && _enemy->frameX >= 7)
	{
		_enemy->time_detection += getDeltaTime();

		if (_enemy->currentVelocity.x > 0.5f)
			sfRectangleShape_setPosition(en_rectangleShape, vector2f(_enemy->pos.x - 20.0f, _enemy->pos.y + 30.0f));
		else
			sfRectangleShape_setPosition(en_rectangleShape, vector2f(_enemy->pos.x - 18.f, _enemy->pos.y + 30.0f));

		sfRectangleShape_setTexture(en_rectangleShape, en_TextureShadow, sfTrue);
		sfRectangleShape_setScale(en_rectangleShape, vector2f(1.f, 1.f));
		sfRectangleShape_setTextureRect(en_rectangleShape, IntRect(0, 0, 102, 36));
		sfRectangleShape_setSize(en_rectangleShape, vector2f(102, 36));
		sfRectangleShape_setFillColor(en_rectangleShape, color_lerp(color(255, 255, 255, 255), color(255, 255, 255, 0), 1.f - (_enemy->time_detection / 1.f)));
		sfRenderTexture_drawRectangleShape(_rdrt, en_rectangleShape, NULL);
	}

	sfRectangleShape_setFillColor(en_rectangleShape, color(255, 255, 255, 255));
}
void en_DisplayGooseShadow(sfRenderTexture* _rdrt, EnemiesStruct* _enemy) {
	if (_enemy->state == EN_STATE_FALLING2 && _enemy->time_detection / .5f < 1.f)
	{
		_enemy->time_detection += getDeltaTime();
		if (_enemy->currentVelocity.x > 0.5f)
			sfRectangleShape_setPosition(en_rectangleShape, vector2f(_enemy->pos.x - 22.0f, _enemy->pos.y + 70.0f));
		else
			sfRectangleShape_setPosition(en_rectangleShape, vector2f(_enemy->pos.x - 20.f, _enemy->pos.y + 70.0f));

		sfRectangleShape_setTexture(en_rectangleShape, en_TextureShadow, sfTrue);
		sfRectangleShape_setScale(en_rectangleShape, vector2f(1.f, 1.f));
		sfRectangleShape_setTextureRect(en_rectangleShape, IntRect(0, 0, 102, 36));
		sfRectangleShape_setSize(en_rectangleShape, vector2f(102, 36));
		sfRectangleShape_setFillColor(en_rectangleShape, color_lerp(color(255, 255, 255, 255), color(255, 255, 255, 0), _enemy->time_detection / .5f));

		sfRenderTexture_drawRectangleShape(_rdrt, en_rectangleShape, NULL);
	}
	else if (_enemy->state != EN_STATE_FALLING2)
	{
		if (_enemy->currentVelocity.x > 0.5f)
			sfRectangleShape_setPosition(en_rectangleShape, vector2f(_enemy->pos.x - 22.0f, _enemy->pos.y + 70.0f));
		else
			sfRectangleShape_setPosition(en_rectangleShape, vector2f(_enemy->pos.x - 20.f, _enemy->pos.y + 70.0f));

		sfRectangleShape_setTexture(en_rectangleShape, en_TextureShadow, sfTrue);
		sfRectangleShape_setScale(en_rectangleShape, vector2f(1.f, 1.f));
		sfRectangleShape_setTextureRect(en_rectangleShape, IntRect(0, 0, 102, 36));
		sfRectangleShape_setSize(en_rectangleShape, vector2f(102, 36));

		sfRenderTexture_drawRectangleShape(_rdrt, en_rectangleShape, NULL);
	}

};
void en_DisplayWhiteBillyGoatShadow(sfRenderTexture* _rdrt, EnemiesStruct* _enemy)
{
	if (_enemy->state == EN_STATE_FALLING2 && _enemy->time_detection / .5f < 1.f)
	{
		_enemy->time_detection += getDeltaTime();
		if (_enemy->forward.x > 0.0f)
			sfRectangleShape_setPosition(en_rectangleShape, vector2f(_enemy->pos.x - 45.0f, _enemy->pos.y + 80.0f));
		else
			sfRectangleShape_setPosition(en_rectangleShape, vector2f(_enemy->pos.x - 20.f, _enemy->pos.y + 80.0f));

		sfRectangleShape_setTexture(en_rectangleShape, en_TextureShadow, sfTrue);
		sfRectangleShape_setScale(en_rectangleShape, vector2f(0.8f, .8f));
		sfRectangleShape_setTextureRect(en_rectangleShape, IntRect(0, 118, 205, 45));
		sfRectangleShape_setSize(en_rectangleShape, vector2f(205.f, 45.f));
		sfRectangleShape_setFillColor(en_rectangleShape, color_lerp(color(255, 255, 255, 255), color(255, 255, 255, 0), _enemy->time_detection / .5f));

		sfRenderTexture_drawRectangleShape(_rdrt, en_rectangleShape, NULL);
	}
	else if (_enemy->state != EN_STATE_FALLING2)
	{
		if (_enemy->forward.x > 0.0f)
			sfRectangleShape_setPosition(en_rectangleShape, vector2f(_enemy->pos.x - 45.0f, _enemy->pos.y + 80.0f));
		else
			sfRectangleShape_setPosition(en_rectangleShape, vector2f(_enemy->pos.x - 20.f, _enemy->pos.y + 80.0f));

		sfRectangleShape_setTexture(en_rectangleShape, en_TextureShadow, sfTrue);
		sfRectangleShape_setScale(en_rectangleShape, vector2f(0.8f, .8f));
		sfRectangleShape_setTextureRect(en_rectangleShape, IntRect(0, 118, 205, 45));
		sfRectangleShape_setSize(en_rectangleShape, vector2f(205.f, 45.f));

		sfRenderTexture_drawRectangleShape(_rdrt, en_rectangleShape, NULL);
	}
};
void en_DisplayBlackBillyGoatShadow(sfRenderTexture* _rdrt, EnemiesStruct* _enemy)
{
	if (_enemy->state == EN_STATE_FALLING2 && _enemy->time_detection / .5f < 1.f)
	{
		_enemy->time_detection += getDeltaTime();
		if (_enemy->forward.x > 0.0f)
			sfRectangleShape_setPosition(en_rectangleShape, vector2f(_enemy->pos.x - 45.0f, _enemy->pos.y + 80.0f));
		else
			sfRectangleShape_setPosition(en_rectangleShape, vector2f(_enemy->pos.x - 20.f, _enemy->pos.y + 80.0f));

		sfRectangleShape_setTexture(en_rectangleShape, en_TextureShadow, sfTrue);
		sfRectangleShape_setScale(en_rectangleShape, vector2f(0.8f, .8f));
		sfRectangleShape_setTextureRect(en_rectangleShape, IntRect(0, 118, 205, 45));

		sfRectangleShape_setSize(en_rectangleShape, vector2f(205.f, 45.f));
		sfRectangleShape_setFillColor(en_rectangleShape, color_lerp(color(255, 255, 255, 255), color(255, 255, 255, 0), _enemy->time_detection / .5f));
		sfRenderTexture_drawRectangleShape(_rdrt, en_rectangleShape, NULL);
	}
	else if (_enemy->state != EN_STATE_FALLING2)
	{
		if (_enemy->forward.x > 0.0f)
			sfRectangleShape_setPosition(en_rectangleShape, vector2f(_enemy->pos.x - 45.0f, _enemy->pos.y + 80.0f));
		else
			sfRectangleShape_setPosition(en_rectangleShape, vector2f(_enemy->pos.x - 20.f, _enemy->pos.y + 80.0f));

		sfRectangleShape_setTexture(en_rectangleShape, en_TextureShadow, sfTrue);
		sfRectangleShape_setScale(en_rectangleShape, vector2f(0.8f, .8f));
		sfRectangleShape_setTextureRect(en_rectangleShape, IntRect(0, 118, 205, 45));
		sfRectangleShape_setSize(en_rectangleShape, vector2f(205.f, 45.f));

		sfRenderTexture_drawRectangleShape(_rdrt, en_rectangleShape, NULL);
	}


};
void en_DisplaySnakeShadow(sfRenderTexture* _rdrt, EnemiesStruct* _enemy) {
	if (_enemy->state == EN_STATE_FALLING2 && _enemy->time_detection / .5f < 1.f)
	{
		_enemy->time_detection += getDeltaTime();
		if (_enemy->currentVelocity.x > 0.0f)
			sfRectangleShape_setPosition(en_rectangleShape, vector2f(_enemy->pos.x - 5.0f, _enemy->pos.y + 55.0f));
		else
			sfRectangleShape_setPosition(en_rectangleShape, vector2f(_enemy->pos.x - 10.f, _enemy->pos.y + 55.0f));

		sfRectangleShape_setTexture(en_rectangleShape, en_TextureShadow, sfTrue);
		sfRectangleShape_setScale(en_rectangleShape, vector2f(1.f, 1.f));
		sfRectangleShape_setTextureRect(en_rectangleShape, IntRect(0, 0, 102, 36));
		sfRectangleShape_setSize(en_rectangleShape, vector2f(102, 36));
		sfRectangleShape_setFillColor(en_rectangleShape, color_lerp(color(255, 255, 255, 255), color(255, 255, 255, 0), _enemy->time_detection / .5f));

		sfRenderTexture_drawRectangleShape(_rdrt, en_rectangleShape, NULL);
	}
	else if (_enemy->state != EN_STATE_FALLING2)
	{
		if (_enemy->currentVelocity.x > 0.0f)
			sfRectangleShape_setPosition(en_rectangleShape, vector2f(_enemy->pos.x - 5.0f, _enemy->pos.y + 55.0f));
		else
			sfRectangleShape_setPosition(en_rectangleShape, vector2f(_enemy->pos.x - 10.f, _enemy->pos.y + 55.0f));

		sfRectangleShape_setTexture(en_rectangleShape, en_TextureShadow, sfTrue);
		sfRectangleShape_setScale(en_rectangleShape, vector2f(1.f, 1.f));
		sfRectangleShape_setTextureRect(en_rectangleShape, IntRect(0, 0, 102, 36));
		sfRectangleShape_setSize(en_rectangleShape, vector2f(102, 36));

		sfRenderTexture_drawRectangleShape(_rdrt, en_rectangleShape, NULL);
	}
};
void en_DisplayMosquitoShadow(sfRenderTexture* _rdrt, EnemiesStruct* _enemy) {
	if (_enemy->forward.x > 0.0f)
		sfRectangleShape_setPosition(en_rectangleShape, vector2f(_enemy->pos.x - 30.0f, _enemy->pos.y + 110.0f));
	else
		sfRectangleShape_setPosition(en_rectangleShape, vector2f(_enemy->pos.x - 5.f, _enemy->pos.y + 110.0f));

	sfRectangleShape_setTexture(en_rectangleShape, en_TextureShadow, sfTrue);
	sfRectangleShape_setScale(en_rectangleShape, vector2f(1.f, 1.f));
	sfRectangleShape_setTextureRect(en_rectangleShape, IntRect(0, 0, 102, 36));
	sfRectangleShape_setSize(en_rectangleShape, vector2f(102, 36));

	sfRenderTexture_drawRectangleShape(_rdrt, en_rectangleShape, NULL);
};
void en_DisplayMoleShadow(sfRenderTexture* _rdrt, EnemiesStruct* _enemy) {};
void en_DisplayScolopendreShadow(sfRenderTexture* _rdrt, EnemiesStruct* _enemy)
{
	if (_enemy->state != EN_STATE_WALK) return;

	sfBool isHorizontal = en_IsScolopendreHorizontal(_enemy->rotation);

	if (isHorizontal)
	{
		if (_enemy->forward.x > 0.0f)
			sfRectangleShape_setPosition(en_rectangleShape, vector2f(_enemy->pos.x - 25.0f, _enemy->pos.y + 5.0f));
		else
			sfRectangleShape_setPosition(en_rectangleShape, vector2f(_enemy->pos.x - 5.f, _enemy->pos.y + 5.0f));


		sfRectangleShape_setTexture(en_rectangleShape, en_TextureShadow, sfTrue);
		sfRectangleShape_setScale(en_rectangleShape, vector2f(1.f, 1.f));
		sfRectangleShape_setTextureRect(en_rectangleShape, IntRect(0, 118, 205, 45));
		sfRectangleShape_setSize(en_rectangleShape, vector2f(205.0f, 45.0f));

		sfRenderTexture_drawRectangleShape(_rdrt, en_rectangleShape, NULL);
	}
};
void en_DisplayChameleonShadow(sfRenderTexture* _rdrt, EnemiesStruct* _enemy)
{
	if (_enemy->state == EN_STATE_DETECTING)
	{
		_enemy->time_detection += getDeltaTime();

		sfRectangleShape_setPosition(en_rectangleShape, vector2f(_enemy->pos.x, _enemy->pos.y));
		sfRectangleShape_setTexture(en_rectangleShape, en_TextureShadow, sfTrue);
		sfRectangleShape_setScale(en_rectangleShape, vector2f(1.f, 1.f));
		sfRectangleShape_setTextureRect(en_rectangleShape, IntRect(0, 36, 82, 82));
		sfRectangleShape_setSize(en_rectangleShape, vector2f(82.f, 82.f));
		if (_enemy->time_detection / .6f < 1.f)
			sfRectangleShape_setFillColor(en_rectangleShape, color_lerp(color(255, 255, 255, 0), color(255, 255, 255, 255), _enemy->time_detection / .6f));

		sfRenderTexture_drawRectangleShape(_rdrt, en_rectangleShape, NULL);
	}
	else if (_enemy->state == EN_STATE_BACK_HOME)
	{
		_enemy->time_detection += getDeltaTime();
		sfRectangleShape_setPosition(en_rectangleShape, vector2f(_enemy->pos.x, _enemy->pos.y));
		sfRectangleShape_setTexture(en_rectangleShape, en_TextureShadow, sfTrue);
		sfRectangleShape_setScale(en_rectangleShape, vector2f(1.f, 1.f));
		sfRectangleShape_setTextureRect(en_rectangleShape, IntRect(0, 36, 82, 82));
		sfRectangleShape_setSize(en_rectangleShape, vector2f(82.f, 82.f));
		if (1 - (_enemy->time_detection / .6f) > 0.f)
			sfRectangleShape_setFillColor(en_rectangleShape, color_lerp(color(255, 255, 255, 0), color(255, 255, 255, 255), 1.f - (_enemy->time_detection / .6f)));

		sfRenderTexture_drawRectangleShape(_rdrt, en_rectangleShape, NULL);
	}
	else if (_enemy->state == EN_STATE_HIDDEN)
	{
		sfRectangleShape_setPosition(en_rectangleShape, vector2f(_enemy->pos.x, _enemy->pos.y));
		sfRectangleShape_setTexture(en_rectangleShape, en_TextureShadow, sfTrue);
		sfRectangleShape_setScale(en_rectangleShape, vector2f(1.f, 1.f));
		sfRectangleShape_setTextureRect(en_rectangleShape, IntRect(0, 36, 82, 82));
		sfRectangleShape_setSize(en_rectangleShape, vector2f(82.f, 82.f));
		sfRectangleShape_setFillColor(en_rectangleShape, color(255, 255, 255, 0));

		sfRenderTexture_drawRectangleShape(_rdrt, en_rectangleShape, NULL);
	}
	else
	{
		sfRectangleShape_setPosition(en_rectangleShape, vector2f(_enemy->pos.x, _enemy->pos.y));
		sfRectangleShape_setTexture(en_rectangleShape, en_TextureShadow, sfTrue);
		sfRectangleShape_setScale(en_rectangleShape, vector2f(1.f, 1.f));
		sfRectangleShape_setTextureRect(en_rectangleShape, IntRect(0, 36, 82, 82));
		sfRectangleShape_setSize(en_rectangleShape, vector2f(82.f, 82.f));
		sfRectangleShape_setFillColor(en_rectangleShape, color(255, 255, 255, 255));

		sfRenderTexture_drawRectangleShape(_rdrt, en_rectangleShape, NULL);
	}
};
void en_DisplayBatShadow(sfRenderTexture* _rdrt, EnemiesStruct* _enemy) {
	if (_enemy->state != EN_STATE_CHASE)
	{
		if (_enemy->forward.x > 0.0f)
			sfRectangleShape_setPosition(en_rectangleShape, vector2f(_enemy->pos.x - 30.0f, _enemy->pos.y + 110.0f));
		else
			sfRectangleShape_setPosition(en_rectangleShape, vector2f(_enemy->pos.x + 5.f, _enemy->pos.y + 110.0f));
	}
	else
	{
		if (_enemy->rotation > 90.f || _enemy->rotation < -90.f)
			sfRectangleShape_setPosition(en_rectangleShape, vector2f(_enemy->pos.x - 30.0f, _enemy->pos.y + 110.0f));
		else
			sfRectangleShape_setPosition(en_rectangleShape, vector2f(_enemy->pos.x - 5.f, _enemy->pos.y + 110.0f));
	}

	sfRectangleShape_setTexture(en_rectangleShape, en_TextureShadow, sfTrue);
	sfRectangleShape_setRotation(en_rectangleShape, 0.0f);
	sfRectangleShape_setScale(en_rectangleShape, vector2f(1.f, 1.f));
	sfRectangleShape_setTextureRect(en_rectangleShape, IntRect(0, 0, 102, 36));
	sfRectangleShape_setSize(en_rectangleShape, vector2f(102, 36));

	sfRenderTexture_drawRectangleShape(_rdrt, en_rectangleShape, NULL);
};
void en_DisplayLittleGhostJoyShadow(sfRenderTexture* _rdrt, EnemiesStruct* _enemy) {
	if (_enemy->currentVelocity.x > 0.5f)
		sfRectangleShape_setPosition(en_rectangleShape, vector2f(_enemy->pos.x - 20.0f, _enemy->pos.y + 110.0f));
	else
		sfRectangleShape_setPosition(en_rectangleShape, vector2f(_enemy->pos.x + 5.f, _enemy->pos.y + 110.0f));

	sfRectangleShape_setTexture(en_rectangleShape, en_TextureShadow, sfTrue);
	sfRectangleShape_setScale(en_rectangleShape, vector2f(1.f, 1.f));
	sfRectangleShape_setTextureRect(en_rectangleShape, IntRect(0, 0, 102, 36));
	sfRectangleShape_setSize(en_rectangleShape, vector2f(102, 36));

	sfRenderTexture_drawRectangleShape(_rdrt, en_rectangleShape, NULL);
};
void en_DisplayBigGhostJoyShadow(sfRenderTexture* _rdrt, EnemiesStruct* _enemy) {
	if (_enemy->currentVelocity.x > 0.0f)
		sfRectangleShape_setPosition(en_rectangleShape, vector2f(_enemy->pos.x - 37.0f, _enemy->pos.y + 190.0f));
	else
		sfRectangleShape_setPosition(en_rectangleShape, vector2f(_enemy->pos.x - 37.f, _enemy->pos.y + 190.0f));

	sfRectangleShape_setTexture(en_rectangleShape, en_TextureShadow, sfTrue);
	sfRectangleShape_setScale(en_rectangleShape, vector2f(1.f, 1.f));
	sfRectangleShape_setTextureRect(en_rectangleShape, IntRect(0, 118, 205, 45));
	sfRectangleShape_setSize(en_rectangleShape, vector2f(205.0f, 45.0f));

	sfRenderTexture_drawRectangleShape(_rdrt, en_rectangleShape, NULL);
};
void en_DisplayLittleGhostSadnessShadow(sfRenderTexture* _rdrt, EnemiesStruct* _enemy) {
	if (_enemy->currentVelocity.x > 0.5f)
		sfRectangleShape_setPosition(en_rectangleShape, vector2f(_enemy->pos.x - 40.0f, _enemy->pos.y + 70.0f));
	else
		sfRectangleShape_setPosition(en_rectangleShape, vector2f(_enemy->pos.x + 5.f, _enemy->pos.y + 70.0f));

	sfRectangleShape_setTexture(en_rectangleShape, en_TextureShadow, sfTrue);
	sfRectangleShape_setScale(en_rectangleShape, vector2f(1.f, 1.f));
	sfRectangleShape_setTextureRect(en_rectangleShape, IntRect(0, 0, 102, 36));
	sfRectangleShape_setSize(en_rectangleShape, vector2f(102, 36));

	sfRenderTexture_drawRectangleShape(_rdrt, en_rectangleShape, NULL);
};
void en_DisplayBigGhostSadnessShadow(sfRenderTexture* _rdrt, EnemiesStruct* _enemy) {
	if (_enemy->currentVelocity.x > 0.5f)
		sfRectangleShape_setPosition(en_rectangleShape, vector2f(_enemy->pos.x - 20.0f, _enemy->pos.y + 140.0f));
	else
		sfRectangleShape_setPosition(en_rectangleShape, vector2f(_enemy->pos.x - 5.f, _enemy->pos.y + 140.0f));

	sfRectangleShape_setTexture(en_rectangleShape, en_TextureShadow, sfTrue);
	sfRectangleShape_setScale(en_rectangleShape, vector2f(1.f, 1.f));
	sfRectangleShape_setTextureRect(en_rectangleShape, IntRect(0, 118, 205, 45));
	sfRectangleShape_setSize(en_rectangleShape, vector2f(205.0f, 45.0f));

	sfRenderTexture_drawRectangleShape(_rdrt, en_rectangleShape, NULL);
};
void en_DisplayImunityShadow(sfRenderTexture* _rdrt, EnemiesStruct* _enemy) {};
void en_DisplaySeagullShadow(sfRenderTexture* _rdrt, EnemiesStruct* _enemy)
{
	if (_enemy->state == EN_STATE_WAIT || _enemy->state == EN_STATE_EAT || (_enemy->state == EN_STATE_GOING_TO_FLY && _enemy->frameX <= 2))
	{
		sfRectangleShape_setPosition(en_rectangleShape, vector2f(_enemy->pos.x + 10.0f, _enemy->pos.y + 40.0f));
		sfRectangleShape_setTexture(en_rectangleShape, en_TextureShadow, sfTrue);
		sfRectangleShape_setScale(en_rectangleShape, vector2f(1.f, 1.f));
		sfRectangleShape_setTextureRect(en_rectangleShape, IntRect(0, 0, 102, 36));
		sfRectangleShape_setSize(en_rectangleShape, vector2f(102, 36));

		sfRenderTexture_drawRectangleShape(_rdrt, en_rectangleShape, NULL);
	}
	else if (_enemy->state == EN_STATE_GOING_TO_FLY && _enemy->frameX >= 3)
	{
		_enemy->time_detection += getDeltaTime();
		sfRectangleShape_setPosition(en_rectangleShape, vector2f(_enemy->pos.x + 10.0f, _enemy->pos.y + 40.0f));
		sfRectangleShape_setTexture(en_rectangleShape, en_TextureShadow, sfTrue);
		sfRectangleShape_setScale(en_rectangleShape, vector2f(1.f, 1.f));
		sfRectangleShape_setTextureRect(en_rectangleShape, IntRect(0, 0, 102, 36));
		sfRectangleShape_setSize(en_rectangleShape, vector2f(102, 36));
		sfRectangleShape_setFillColor(en_rectangleShape, color_lerp(color(255, 255, 255, 255), color(255, 255, 255, 0), _enemy->time_detection / 0.8f));
		sfRenderTexture_drawRectangleShape(_rdrt, en_rectangleShape, NULL);
	}
};

sfBool en_CanBeDetected(sfVector2f _lookingObject, sfVector2f _targetObject, sfVector2f _forwardVector, float _fov, float _maxRange)
{
	sfVector2f lookToTargetVector = createVectorWithVectorf(_lookingObject, _targetObject);
	float sqrDistance = getSqrMagnitudef(lookToTargetVector);
	if (sqrDistance > _maxRange * _maxRange)
		return sfFalse;

	sfVector2f lookToTargetNorm = normalizef(lookToTargetVector);

	float dotProduct = dotProductf(lookToTargetNorm, _forwardVector);
	if (dotProduct > cos(_fov * DEG2RAD * 0.5f))
		return sfTrue;
	return sfFalse;
}

void en_IsInDamageBounds(sfFloatRect* _damageBounds, sfBool _damaging, Window* _window)
{
	for (int i = 0; i < enemiesList->size(enemiesList); i++)
	{
		if (GD_ENEMY->type == EN_MOLE || GD_ENEMY->type == EN_SCOLOPENDRE || GD_ENEMY->type == EN_SEAGULL || (GD_ENEMY->type == EN_BIG_GHOST_SADNESS && GD_ENEMY->state != EN_STATE_THROWN) || (GD_ENEMY->type == EN_SNAKE && GD_ENEMY->timerAllAttacks < 0.5f)) continue;
		sfFloatRect enemyCollisionsBounds = en_getCollisionsBounds(GD_ENEMY);
		if (sfFloatRect_intersects(_damageBounds, &enemyCollisionsBounds, NULL)) {
			if (GD_ENEMY->state != EN_STATE_DAMAGING && GD_ENEMY->time_invincibility > 0.5f && GD_ENEMY->life > 0)
			{
				GD_ENEMY->time_invincibility = 0.f;
				if ((GD_ENEMY->type == EN_YOYO_SPIDER || GD_ENEMY->type == EN_VENIMOUS_YOYO_SPIDER) && GD_ENEMY->isOnString && _damaging)
				{
					GD_ENEMY->isOnString = sfFalse;
					GD_ENEMY->state = EN_STATE_BACK_HOME;
					GD_ENEMY->frameX = 0.f;
					GD_ENEMY->timerAnim = 0.f;
					GD_ENEMY->isFallingAnimDone = sfFalse;
				}
				else if (_damaging)
				{
					GD_ENEMY->state = EN_STATE_DAMAGING;
					GD_ENEMY->life--;
					if (GD_ENEMY->life <= 0)
					{
						switch (GD_ENEMY->type)
						{
						
						}
						//bestiaire
					}
					else if (GD_ENEMY->life > 0.f && GD_ENEMY->type != EN_GOOSE)
						prt_CreateDamagePlayerParticles(GD_ENEMY->pos);
				}
				GD_ENEMY->time_wait = 0.0f;
			}
			if (!_damaging && ((GD_ENEMY->type == EN_YOYO_SPIDER && GD_ENEMY->isFallingAnimDone && !GD_ENEMY->isOnString) || GD_ENEMY->type == EN_CRAB))
			{
				playSoundFX("SFX_Player_Capture");
				if (GD_ENEMY->type == EN_YOYO_SPIDER)
				{
					pause_ItemsDiscovered |= PAUSE_YOYO;
					pause_itemRecup(PAUSE_YOYO);
					qst_IncrementCount(QST_ID_S_GET_YOYO);
				}
				else if (GD_ENEMY->type == EN_CRAB)
				{
					pause_ItemsDiscovered |= PAUSE_CABORALS;
					pause_itemRecup(PAUSE_CABORALS);
				}
				if (GD_ENEMY->prtCapture)
				{
					prt_CreateCaptureParticles(GD_ENEMY->pos);
					GD_ENEMY->prtCapture = sfFalse;
				}
				en_Erase(i,GD_ENEMY->pos, FX_EXPLOSION, _window);
				continue;
			}
		}
	}
}

void blt_shouldSendBackNut(sfFloatRect* _damageBounds)
{
	for (int i = 0; i < bulletsList->size(bulletsList); i++)
	{
		if (sfFloatRect_intersects(_damageBounds, &STD_LIST_GETDATA(bulletsList, Bullets_struct, i)->colRect, NULL) && !STD_LIST_GETDATA(bulletsList, Bullets_struct, i)->isBack)
		{
			STD_LIST_GETDATA(bulletsList, Bullets_struct, i)->isBack = sfTrue;
			STD_LIST_GETDATA(bulletsList, Bullets_struct, i)->forward = vector2f(-STD_LIST_GETDATA(bulletsList, Bullets_struct, i)->forward.x, -STD_LIST_GETDATA(bulletsList, Bullets_struct, i)->forward.y);
		}
	}
}

void en_Erase(int _id, sfVector2f _pos, FXenemies_enum _type, Window* _window)
{
	if (getState() != EDITOR) {
		sfMutex_lock(mutexThread2);
		if (_id > enemiesList->size(enemiesList)) {
			sfMutex_unlock(mutexThread2);
			return;
		}

		if (_type == FX_STARS)
			FXe_addFXenemies(_pos, FX_STARS);
		else if(_type == FX_EXPLOSION)
		{
			playSoundFX("SFX_Pouf");
			FXe_addFXenemies(_pos, FX_EXPLOSION);
		}

		switch (STD_LIST_GETDATA(enemiesList, EnemiesStruct, _id)->type) 
		{
		case EN_YOYO_SPIDER:
			if (pause_ItemsDiscovered & (PAUSE_YOYO));
			else
			{
				HUD_typeOfAddedData = HUD_BESTIARY_BESTIARY;
				HUD_toggleAddedDataAppered(sfTrue);
			}
			pause_ItemsDiscovered |= PAUSE_YOYO;
			break;
		case EN_CRAB:
			if (pause_ItemsDiscovered & (PAUSE_CABORALS));
			else
			{
				HUD_typeOfAddedData = HUD_BESTIARY_BESTIARY;
				HUD_toggleAddedDataAppered(sfTrue);
			}
			pause_ItemsDiscovered |= PAUSE_CABORALS;
			break;
		case EN_VENIMOUS_YOYO_SPIDER:
			if (pause_EnnemisDiscovered & (PAUSE_POISON_SPIDER));
			else
			{
				HUD_typeOfAddedData = HUD_BESTIARY_BESTIARY;
				HUD_toggleAddedDataAppered(sfTrue);
			}

			pause_EnnemisDiscovered |= PAUSE_POISON_SPIDER;
			break;
		case EN_GOOSE:
			if (pause_EnnemisDiscovered & (PAUSE_GOOSE));
			else
			{
				HUD_typeOfAddedData = HUD_BESTIARY_BESTIARY;
				HUD_toggleAddedDataAppered(sfTrue);
			}
			pause_EnnemisDiscovered |= PAUSE_GOOSE;
			break;
		case EN_WHITE_BILLY_GOAT:
			if (pause_EnnemisDiscovered & (PAUSE_GOAT));
			else
			{
				HUD_typeOfAddedData = HUD_BESTIARY_BESTIARY;
				HUD_toggleAddedDataAppered(sfTrue);
			}
			pause_EnnemisDiscovered |= PAUSE_GOAT;
			break;
		case EN_BLACK_BILLY_GOAT:
			if (pause_EnnemisDiscovered & (PAUSE_GOAT_BLACK));
			else
			{
				HUD_typeOfAddedData = HUD_BESTIARY_BESTIARY;
				HUD_toggleAddedDataAppered(sfTrue);
			}
			pause_EnnemisDiscovered |= PAUSE_GOAT_BLACK;
			break;
		case EN_SNAKE:
			if (pause_EnnemisDiscovered & (PAUSE_SNAKE));
			else
			{
				HUD_typeOfAddedData = HUD_BESTIARY_BESTIARY;
				HUD_toggleAddedDataAppered(sfTrue);
			}
			pause_EnnemisDiscovered |= PAUSE_SNAKE;
			break;
		case EN_MOSQUITO:
			if (pause_EnnemisDiscovered & (PAUSE_MOSQUITO));
			else
			{
				HUD_typeOfAddedData = HUD_BESTIARY_BESTIARY;
				HUD_toggleAddedDataAppered(sfTrue);
			}
			pause_EnnemisDiscovered |= PAUSE_MOSQUITO;
			break;
		case EN_MOLE:
			if (pause_EnnemisDiscovered & (PAUSE_MOLE));
			else
			{
				HUD_typeOfAddedData = HUD_BESTIARY_BESTIARY;
				HUD_toggleAddedDataAppered(sfTrue);
			}
			pause_EnnemisDiscovered |= PAUSE_MOLE;
			break;
		case EN_CHAMELEON:
			if (pause_EnnemisDiscovered & (PAUSE_VARAN));
			else
			{
				HUD_typeOfAddedData = HUD_BESTIARY_BESTIARY;
				HUD_toggleAddedDataAppered(sfTrue);
			}
			pause_EnnemisDiscovered |= PAUSE_VARAN;
			break;
		case EN_BAT:
			if (pause_EnnemisDiscovered & (PAUSE_BAT));
			else
			{
				HUD_typeOfAddedData = HUD_BESTIARY_BESTIARY;
				HUD_toggleAddedDataAppered(sfTrue);
			}
			pause_EnnemisDiscovered |= PAUSE_BAT;
			break;
		case EN_LITTLE_GHOST_JOY:
			if (pause_EnnemisDiscovered & (PAUSE_LITTLE_JOY_DEMON));
			else
			{
				HUD_typeOfAddedData = HUD_BESTIARY_DEMON;
				HUD_toggleAddedDataAppered(sfTrue);
			}
			pause_EnnemisDiscovered |= PAUSE_LITTLE_JOY_DEMON;
			break;
		case EN_BIG_GHOST_JOY:
			if (pause_EnnemisDiscovered & (PAUSE_BIG_JOY_DEMON));
			else
			{
				HUD_typeOfAddedData = HUD_BESTIARY_DEMON;
				HUD_toggleAddedDataAppered(sfTrue);
			}
			pause_EnnemisDiscovered |= PAUSE_BIG_JOY_DEMON;
			break;
		case EN_LITTLE_GHOST_SADNESS:
			if (pause_EnnemisDiscovered & (PAUSE_LITTLE_SADNESS_DEMON));
			else
			{
				HUD_typeOfAddedData = HUD_BESTIARY_DEMON;
				HUD_toggleAddedDataAppered(sfTrue);
			}
			pause_EnnemisDiscovered |= PAUSE_LITTLE_SADNESS_DEMON;
			break;
		case EN_BIG_GHOST_SADNESS:
			if (pause_EnnemisDiscovered & (PAUSE_BIG_SADNESS_DEMON));
			else
			{
				HUD_typeOfAddedData = HUD_BESTIARY_DEMON;
				HUD_toggleAddedDataAppered(sfTrue);
			}
			pause_EnnemisDiscovered |= PAUSE_BIG_SADNESS_DEMON;
			break;
		}
		enemiesList->erase(&enemiesList, _id);
		sfMutex_unlock(mutexThread2);
	}
	else {
		if (_id > enemiesList->size(enemiesList)) {
			return;
		}

		enemiesList->erase(&enemiesList, _id);
	}
}

void en_Clear() {
	if (getState() != EDITOR) {
		if (!mutexThread2) return;
		sfMutex_lock(mutexThread2);
		if (!enemiesList) {
			sfMutex_unlock(mutexThread2);
			return;
		}
		enemiesList->clear(&enemiesList);
		if (!bulletsList) {
			sfMutex_unlock(mutexThread2);
			return;
		}
		bulletsList->clear(&bulletsList);
		sfMutex_unlock(mutexThread2);
	}
	else {
		if (!enemiesList) {
			return;
		}
		enemiesList->clear(&enemiesList);
		if (!bulletsList) {
			return;
		}
		bulletsList->clear(&bulletsList);
	}
}

sfVector2f en_GetClosestGivenEnemyPos(sfVector2f _startPos, EnemiesType _type)
{
	sfVector2f closestPos = vector2f(-100.f, -100.f);
	float dist = LLONG_MAX;

	for (int i = 0; i < enemiesList->size(enemiesList); i++)
	{
		if (GD_ENEMY->type == _type) {
			float mag = getSqrMagnitudef(createVectorWithVectorf(_startPos, GD_ENEMY->pos));
			if (mag < dist) {
				dist = mag;
				closestPos = GD_ENEMY->pos;
			}
		}
	}

	return closestPos;
}

sfFloatRect en_GetChameleonTongueArea(EnemiesStruct* _enemy)
{
	if (_enemy->rotation < 5.f)
		return FlRect(_enemy->pos.x - _enemy->origin.x + 12.f, _enemy->pos.y - _enemy->origin.y, 88.f, 500.f);
	else if (_enemy->rotation < 95.f)
		return FlRect(_enemy->pos.x - _enemy->origin.x + 88.f - 500.f, _enemy->pos.y - _enemy->origin.y + 12.f, 500.f, 88.f);
	else if (_enemy->rotation < 185.f)
		return FlRect(_enemy->pos.x - _enemy->origin.x - 12.f, _enemy->pos.y - _enemy->origin.y + 88.f - 500.f, 88.f, 500.f);
	else
		return FlRect(_enemy->pos.x - _enemy->origin.x, _enemy->pos.y - _enemy->origin.y - 12.f, 500.f, 88.f);
}

sfFloatRect en_GetChameleonTongueCollisions(EnemiesStruct* _enemy)
{
	float tongueScale = en_GetChameleonTongueScale(_enemy) * 20.f;

	if (_enemy->rotation < 5.f)
		return FlRect(_enemy->pos.x - _enemy->origin.x + 12.f, _enemy->pos.y - _enemy->origin.y + 88.f, 88.f, tongueScale + 108.f - 88.f);
	else if (_enemy->rotation < 95.f)
		return FlRect(_enemy->pos.x - _enemy->origin.x + 88.f - tongueScale - 108.f, _enemy->pos.y - _enemy->origin.y + 12.f, tongueScale + 108.f - 88.f, 88.f);
	else if (_enemy->rotation < 185.f)
		return FlRect(_enemy->pos.x - _enemy->origin.x - 12.f, _enemy->pos.y - _enemy->origin.y + 88.f - tongueScale - 108.f, 88.f, tongueScale + 108.f - 88.f);
	else
		return FlRect(_enemy->pos.x - _enemy->origin.x + 88.f, _enemy->pos.y - _enemy->origin.y - 12.f, tongueScale + 108.f - 88.f, 88.f);
}

sfFloatRect en_GetChameleonTongueBounds(EnemiesStruct* _enemy)
{
	sfVector2f tonguePos = addVectorsrf(en_GetAppropriateVectorRotation(vector2f(13.f, 42.f), _enemy->rotation), en_GetAppropriateVectorRotation(vector2f(0.f, en_GetChameleonTongueScale(_enemy) * 20.f), _enemy->rotation));

	if (_enemy->rotation < 5.f)
		return FlRect(_enemy->pos.x - _enemy->origin.x + tonguePos.x, _enemy->pos.y - _enemy->origin.y + tonguePos.y + 50.f, 88.f, 20.f);
	else if (_enemy->rotation < 95.f)
		return FlRect(_enemy->pos.x - _enemy->origin.x + tonguePos.x + 20.f, _enemy->pos.y - _enemy->origin.y + tonguePos.y, 20.f, 88.f);
	else if (_enemy->rotation < 185.f)
		return FlRect(_enemy->pos.x - _enemy->origin.x + tonguePos.x, _enemy->pos.y - _enemy->origin.y + tonguePos.y + 20.f, 88.f, 20.f);
	else
		return FlRect(_enemy->pos.x - _enemy->origin.x + tonguePos.x + 50.f, _enemy->pos.y - _enemy->origin.y + tonguePos.y, 20.f, 88.f);
}

sfVector2f en_GetAppropriateVectorRotation(sfVector2f _v, float _rotation)
{
	if (_rotation < 5.f)
		return vector2f(_v.x, _v.y);
	else if (_rotation < 95.f)
		return vector2f(-_v.y, _v.x);
	else if (_rotation < 185.f)
		return vector2f(-_v.x, -_v.y);
	else
		return vector2f(_v.y, -_v.x);
}

float en_GetChameleonTongueScale(EnemiesStruct* _enemy)
{
	if (_enemy->state == EN_STATE_LAUNCH_TONGUE)
		return lerp(0.f, 20.f, _enemy->time_wait);

	return lerp(20.f, 0.f, _enemy->time_wait * 2.f);
}

sfBool en_IsPlayerBlockedByChameleonTongue(sfVector2f _nextPlayerPos)
{
	sfMutex_lock(mutexThread2);
	for (int i = 0; i < enemiesList->size(enemiesList); i++)
	{
		if (GD_ENEMY->type == EN_CHAMELEON && (GD_ENEMY->state == EN_STATE_LAUNCH_TONGUE || GD_ENEMY->state == EN_STATE_BACK_TONGUE)) {
			sfFloatRect tongueCollisions = en_GetChameleonTongueCollisions(GD_ENEMY);
			if (sfFloatRect_contains(&tongueCollisions, _nextPlayerPos.x, _nextPlayerPos.y)) {
				sfMutex_unlock(mutexThread2);
				return sfTrue;
			}
		}
	}

	sfMutex_unlock(mutexThread2);
	return sfFalse;
}

sfBool en_IsScolopendreHorizontal(float _rotation)
{
	if (_rotation < 5.f || (_rotation > 175.f && _rotation < 185.f))
		return sfTrue;

	return sfFalse;
}

sfFloatRect en_GetAppropriateScolopendreBounds(EnemiesStruct* _enemy)
{
	sfBool isHorizontal = en_IsScolopendreHorizontal(_enemy->rotation);
	sfBool isRotated = sfFalse;
	if ((isHorizontal && _enemy->rotation > 95.f) || (!isHorizontal && _enemy->rotation > 195.f))
		isRotated = sfTrue;

	sfFloatRect bounds = FlRect(0.f, 0.f, 0.f, 0.f);

	switch (_enemy->state)
	{
	case EN_STATE_GOINGUP:
		if (isHorizontal) {
			switch (_enemy->frameX)
			{
			case 2: bounds = FlRect(146.f, 0.f, 40.f, 46.f); break;
			case 3: bounds = FlRect(118.f, 0.f, 68.f, 46.f); break;
			case 4: bounds = FlRect(82.f, 0.f, 104.f, 46.f); break;
			case 5: bounds = FlRect(0.f, 0.f, 168.f, 46.f); break;
			default: bounds = FlRect(0.f, 0.f, 0.f, 0.f); break;
			}
			if (isRotated)
				bounds.left = 0.f;
		}
		else {
			switch (_enemy->frameX)
			{
			case 2: bounds = FlRect(0.f, 136.f, 66.f, 26.f); break;
			case 3: bounds = FlRect(0.f, 126.f, 66.f, 36.f); break;
			case 4: bounds = FlRect(0.f, 74.f, 66.f, 88.f); break;
			case 5: bounds = FlRect(0.f, 38.f, 66.f, 124.f); break;
			case 6: bounds = FlRect(0.f, 0.f, 66.f, 162.f); break;
			default: bounds = FlRect(0.f, 0.f, 0.f, 0.f); break;
			}
			if (isRotated)
				bounds.top = 26.f;
		}
		break;
	case EN_STATE_WALK:
		if (isHorizontal) {
			bounds = FlRect(0.f, 0.f, 190.f, 46.f);
		}
		else {
			bounds = FlRect(-2.f, 0.f, 68.f, 192.f);
		}
		break;
	case EN_STATE_GOINGDOWN:
		if (isHorizontal) {
			switch (_enemy->frameX)
			{
			case 0: bounds = FlRect(0.f, 0.f, 182.f, 46.f); break;
			case 1: bounds = FlRect(20.f, 0.f, 142.f, 46.f); break;
			case 2: bounds = FlRect(20.f, 0.f, 118.f, 46.f); break;
			case 3: bounds = FlRect(20.f, 0.f, 58.f, 46.f); break;
			case 4:
			case 5:
			case 6: bounds = FlRect(20.f, 0.f, 40.f, 46.f); break;
			default: bounds = FlRect(0.f, 0.f, 0.f, 0.f); break;
			}
			if (isRotated)
				bounds.left = 192.f - bounds.width - bounds.left;
		}
		else {
			switch (_enemy->frameX)
			{
			case 0: bounds = FlRect(0.f, 16.f, 66.f, 190.f); break;
			case 1: bounds = FlRect(0.f, 16.f, 66.f, 158.f); break;
			case 2: bounds = FlRect(0.f, 16.f, 66.f, 120.f); break;
			case 3: bounds = FlRect(0.f, 16.f, 66.f, 84.f); break;
			case 4: bounds = FlRect(0.f, 16.f, 66.f, 58.f); break;
			case 5:
			case 6:
			case 7: bounds = FlRect(0.f, 0.f, 66.f, 46.f); break;
			default: bounds = FlRect(0.f, 0.f, 0.f, 0.f); break;
			}
			if (isRotated)
				bounds.top = 206.f - bounds.height - bounds.top - 16.f;
		}
		break;
	default:
		break;
	}

	return bounds;
}

void en_DestroyEnemyInEditor(Window* _window)
{
	for (int i = 0; i < enemiesList->size(enemiesList); i++)
	{
		sfFloatRect mousePos = { getfWorldMousePos(_window->renderWindow).x, getfWorldMousePos(_window->renderWindow).y, 1.f, 1.f };
		sfFloatRect ennemis = { STD_LIST_GETDATA(enemiesList, EnemiesStruct, i)->pos.x, STD_LIST_GETDATA(enemiesList, EnemiesStruct, i)->pos.y, STD_LIST_GETDATA(enemiesList, EnemiesStruct, i)->scale.x, STD_LIST_GETDATA(enemiesList, EnemiesStruct, i)->scale.y };
		if (sfFloatRect_intersects(&mousePos, &ennemis, NULL))
		{
			enemiesList->erase(&enemiesList, i);
		}
	}
}

sfFloatRect* en_getPointerBounds(int _id)
{
	sfFloatRect bounds = FlRect(0.f, 0.f, 0.f, 0.f);
	if (_id > enemiesList->size(enemiesList))
		return &bounds;

	bounds = FlRect(STD_LIST_GETDATA(enemiesList, EnemiesStruct, _id)->pos.x - STD_LIST_GETDATA(enemiesList, EnemiesStruct, _id)->origin.x, STD_LIST_GETDATA(enemiesList, EnemiesStruct, _id)->pos.y - STD_LIST_GETDATA(enemiesList, EnemiesStruct, _id)->origin.y, STD_LIST_GETDATA(enemiesList, EnemiesStruct, _id)->size.x, STD_LIST_GETDATA(enemiesList, EnemiesStruct, _id)->size.y);
	return &bounds;
}

EnemiesStructToSave* getMobsListToSave()
{
	if (enemiesList == NULL)
		return NULL;

	int len = enemiesList->size(enemiesList);
	EnemiesStructToSave* tmp = calloc(len, sizeof(EnemiesStructToSave));

	if (tmp == NULL)
		return NULL;

	for (int i = 0; i < len; i++)
	{
		tmp[i].position = STD_LIST_GETDATA(enemiesList, EnemiesStruct, i)->pos;
		tmp[i].type = STD_LIST_GETDATA(enemiesList, EnemiesStruct, i)->type;
		tmp[i].rotation = STD_LIST_GETDATA(enemiesList, EnemiesStruct, i)->rotation;
	}
	return tmp;
}

void checkCollisions(int _id, float _dt)
{
	if (getIsFreeingMap()) return;
	sfVector2f desiredVelocity = multiplyVectorsrf(STD_LIST_GETDATA(enemiesList, EnemiesStruct, _id)->forward, STD_LIST_GETDATA(enemiesList, EnemiesStruct, _id)->maxVelocity * _dt);
	sfVector2f steering = addVectorsrf(desiredVelocity, multiplyVectorsrf(STD_LIST_GETDATA(enemiesList, EnemiesStruct, _id)->currentVelocity, -1.f * _dt));
	AddForceWithoutMass(steering, &STD_LIST_GETDATA(enemiesList, EnemiesStruct, _id)->currentVelocity);

	float radius1 = STD_LIST_GETDATA(enemiesList, EnemiesStruct, _id)->radius;
	for (int i = 0; i < enemiesList->size(enemiesList); i++) {
		if (getIsFreeingMap()) return;
		if (i == _id)
			continue;
		float radius2 = GD_ENEMY->radius;

		if (getSqrMagnitudeBetween2Vectorsf(STD_LIST_GETDATA(enemiesList, EnemiesStruct, _id)->pos, GD_ENEMY->pos) < (radius1 + radius2) * (radius1 + radius2)) {

			sfVector2f desiredVelocity = multiplyVectorsrf(getNormBetween2Vectorsf(GD_ENEMY->pos, STD_LIST_GETDATA(enemiesList, EnemiesStruct, _id)->pos), STD_LIST_GETDATA(enemiesList, EnemiesStruct, _id)->maxVelocity * _dt);
			sfVector2f steering = addVectorsrf(desiredVelocity, multiplyVectorsrf(STD_LIST_GETDATA(enemiesList, EnemiesStruct, _id)->currentVelocity, -1.f * _dt));
			multiplyVectorsf(&steering, 50.f);
			if (!cll_RectMap(en_getCollisionsBounds(STD_LIST_GETDATA(enemiesList, EnemiesStruct, _id)), multiplyVectorsrf(steering, 1.f / STD_LIST_GETDATA(enemiesList, EnemiesStruct, _id)->mass)))
				AddForce(steering, &STD_LIST_GETDATA(enemiesList, EnemiesStruct, _id)->currentVelocity, STD_LIST_GETDATA(enemiesList, EnemiesStruct, _id)->mass);
		}
	}
	if (cll_RectMap(en_getCollisionsBounds(STD_LIST_GETDATA(enemiesList, EnemiesStruct, _id)), multiplyVectorsrf(STD_LIST_GETDATA(enemiesList, EnemiesStruct, _id)->currentVelocity, _dt))
		&& STD_LIST_GETDATA(enemiesList, EnemiesStruct, _id)->type != EN_BAT
		&& STD_LIST_GETDATA(enemiesList, EnemiesStruct, _id)->type != EN_MOSQUITO
		&& STD_LIST_GETDATA(enemiesList, EnemiesStruct, _id)->type != EN_SEAGULL)
	{
		STD_LIST_GETDATA(enemiesList, EnemiesStruct, _id)->forward = vector2f(-STD_LIST_GETDATA(enemiesList, EnemiesStruct, _id)->forward.x, -STD_LIST_GETDATA(enemiesList, EnemiesStruct, _id)->forward.y);
		if (STD_LIST_GETDATA(enemiesList, EnemiesStruct, _id)->type == EN_WHITE_BILLY_GOAT || STD_LIST_GETDATA(enemiesList, EnemiesStruct, _id)->type == EN_BLACK_BILLY_GOAT)
		{
			STD_LIST_GETDATA(enemiesList, EnemiesStruct, _id)->currentVelocity = vector2f(0.0f, 0.0f);
			STD_LIST_GETDATA(enemiesList, EnemiesStruct, _id)->state = EN_STATE_WAIT;
		}
		else
		{
			STD_LIST_GETDATA(enemiesList, EnemiesStruct, _id)->currentVelocity = vector2f(-STD_LIST_GETDATA(enemiesList, EnemiesStruct, _id)->currentVelocity.x, -STD_LIST_GETDATA(enemiesList, EnemiesStruct, _id)->currentVelocity.y);
		}
		if (STD_LIST_GETDATA(enemiesList, EnemiesStruct, _id)->state == EN_STATE_THROWN) {
			STD_LIST_GETDATA(enemiesList, EnemiesStruct, _id)->state = EN_STATE_WAIT;
			STD_LIST_GETDATA(enemiesList, EnemiesStruct, _id)->frameX = 0;
			STD_LIST_GETDATA(enemiesList, EnemiesStruct, _id)->time_wait = 0.f;
			STD_LIST_GETDATA(enemiesList, EnemiesStruct, _id)->stunTimer = 0.f;
		}
	}
	if (STD_LIST_GETDATA(enemiesList, EnemiesStruct, _id)->type == EN_WHITE_BILLY_GOAT || STD_LIST_GETDATA(enemiesList, EnemiesStruct, _id)->type == EN_YOYO_SPIDER || STD_LIST_GETDATA(enemiesList, EnemiesStruct, _id)->type == EN_VENIMOUS_YOYO_SPIDER || STD_LIST_GETDATA(enemiesList, EnemiesStruct, _id)->type == EN_BLACK_BILLY_GOAT || STD_LIST_GETDATA(enemiesList, EnemiesStruct, _id)->type == EN_GOOSE && STD_LIST_GETDATA(enemiesList, EnemiesStruct, _id)->state != EN_STATE_FALLING2)
	{
		sfVector2i tmpIndexPos = map_getBlockPos(STD_LIST_GETDATA(enemiesList, EnemiesStruct, _id)->pos);
		tmpIndexPos = vector2i(tmpIndexPos.y, tmpIndexPos.x);
		if (map_HasTileCollisions(tmpIndexPos) == CLD_VOID)
		{
			STD_LIST_GETDATA(enemiesList, EnemiesStruct, _id)->time_wait = 0.0f;
			STD_LIST_GETDATA(enemiesList, EnemiesStruct, _id)->time_detection = 0.0f;
			STD_LIST_GETDATA(enemiesList, EnemiesStruct, _id)->state = EN_STATE_FALLING2;
		}
	}

	addVectorsf(&STD_LIST_GETDATA(enemiesList, EnemiesStruct, _id)->pos, multiplyVectorsrf(STD_LIST_GETDATA(enemiesList, EnemiesStruct, _id)->currentVelocity, _dt));
}

EnemiesStruct* en_getClosestEnemy(sfVector2f _pos)
{
	if (enemiesList == NULL)
		return NULL;

	float lastDistance = LLONG_MAX;
	int idEnemy = -1;

	for (int i = 0; i < enemiesList->size(enemiesList); i++)
	{
		if (GD_ENEMY->type == EN_MOLE || GD_ENEMY->type == EN_SCOLOPENDRE || GD_ENEMY->type == EN_CHAMELEON || GD_ENEMY->type == EN_SEAGULL || GD_ENEMY->life <= 0 || (GD_ENEMY->type == EN_YOYO_SPIDER && GD_ENEMY->isOnString && GD_ENEMY->isFallingAnimDone) || (GD_ENEMY->type == EN_VENIMOUS_YOYO_SPIDER && GD_ENEMY->isOnString && GD_ENEMY->isFallingAnimDone))
			continue;

		float distanceBoxPlayer = getSqrMagnitudeBetween2Vectorsf(_pos, GD_ENEMY->pos);
		if (distanceBoxPlayer < lastDistance)
		{
			lastDistance = distanceBoxPlayer;
			idEnemy = i;
		}
	}
	if (idEnemy == -1)
		return NULL;
	return STD_LIST_GETDATA(enemiesList, EnemiesStruct, idEnemy);
}

sfFloatRect en_getBounds(EnemiesStruct* _enemy)
{
	if (_enemy == NULL)
		return (sfFloatRect) { 0 };

	return FlRect(_enemy->pos.x - _enemy->origin.x, _enemy->pos.y - _enemy->origin.y, _enemy->size.x, _enemy->size.y);
}

sfFloatRect en_getCollisionsBounds(EnemiesStruct* _enemy)
{
	if (_enemy == NULL)
		return (sfFloatRect) { 0 };

	return FlRect(_enemy->pos.x - _enemy->origin.x + _enemy->collisionsBounds.left, _enemy->pos.y - _enemy->origin.y + _enemy->collisionsBounds.top, _enemy->collisionsBounds.width, _enemy->collisionsBounds.height);
}

void en_AttachRing(sfBool _attach, EnemiesStruct* _enemy)
{
	if (_enemy == NULL)
		return;

	if (_attach) {
		_enemy->state = EN_STATE_STUNNED;
		_enemy->stunTimer = 20.f;
		_enemy->currentVelocity = vector2f(0.f, 0.f);
		if (plr_GetPos().x > _enemy->pos.x)
			_enemy->currentVelocity.x = 1.f;
		else
			_enemy->currentVelocity.x = -1.f;
	}
	else {
		if (_enemy->type != EN_YOYO_SPIDER && _enemy->type != EN_VENIMOUS_YOYO_SPIDER)
			_enemy->state = EN_STATE_WAIT;
		else if (_enemy->type == EN_LITTLE_GHOST_JOY)
			_enemy->state = EN_STATE_WALK;
		else
			_enemy->state = EN_STATE_ESCAPING;
		_enemy->stunTimer = 0.f;
		_enemy->currentVelocity = NULLVECTF;
	}
	_enemy->frameX = 0;
	_enemy->timerAnim = 0.f;
}

void en_Attract(EnemiesStruct* _enemy)
{
	if (_enemy == NULL)
		return;
	if (_enemy->type == EN_IMUNITY)
		return;

	_enemy->state = EN_STATE_THROWN;
	_enemy->stunTimer = EN_THROWN_DURATION;
	_enemy->forward = normalizef(createVectorWithVectorf(_enemy->pos, plr_GetFeetPos()));
	_enemy->currentVelocity = multiplyVectorsrf(_enemy->forward, 150.f);
	_enemy->time_wait = 0.f;
}

void en_Repulse(EnemiesStruct* _enemy)
{
	if (_enemy == NULL)
		return;

	_enemy->state = EN_STATE_THROWN;
	_enemy->stunTimer = EN_THROWN_DURATION;
	_enemy->forward = normalizef(createVectorWithVectorf(plr_GetFeetPos(), _enemy->pos));
	_enemy->currentVelocity = multiplyVectorsrf(_enemy->forward, 150.f);
	_enemy->time_wait = 0.f;
}

void en_Stasis(EnemiesStruct* _enemy)
{
	if (_enemy == NULL)
		return;

	_enemy->state = EN_STATE_STUNNED;
	_enemy->stunTimer = 5.f;
	_enemy->currentVelocity = vector2f(0.f, 0.f);
}

void en_Destroy(EnemiesStruct* _enemy)
{
	if (_enemy == NULL)
		return;

	_enemy->state = EN_STATE_DAMAGING;
	_enemy->time_wait = 0.f;
}

void en_choiceEnemyRect(sfRectangleShape* _rectangle, EnemiesStruct* _enemy)
{
	sfVector2f tmpPpos = plr_GetFeetPos();
	if (_enemy->state != EN_STATE_WALK)
	{
		if (tmpPpos.x < _enemy->pos.x)
			sfRectangleShape_setScale(en_rectangleShape, vector2f(-_enemy->scale.x, _enemy->scale.y));
		else
			sfRectangleShape_setScale(en_rectangleShape, _enemy->scale);
	}
	sfRectangleShape_setSize(en_rectangleShape, vector2f(64.f, 64.f));
	sfRectangleShape_setPosition(en_rectangleShape, vector2f(500.0f, 800.0f));
	_enemy->pos = vector2f(500.0f, 800.0f);
	switch (_enemy->state)
	{
	case EN_STATE_WAIT:
		if (tmpPpos.y > _enemy->pos.y)
			sfRectangleShape_setTextureRect(en_rectangleShape, animatorAnim("moleFaceIdle", &_enemy->frameX, &_enemy->timerAnim, 0));
		else
			sfRectangleShape_setTextureRect(en_rectangleShape, animatorAnim("moleBackIdle", &_enemy->frameX, &_enemy->timerAnim, 0));
		break;
	case EN_STATE_WALK:
		sfRectangleShape_setTextureRect(en_rectangleShape, IntRect(0, 126, 60, 16));
		sfRectangleShape_setSize(en_rectangleShape, vector2f(55.f, 17.f));
		sfRectangleShape_setPosition(en_rectangleShape, vector2f(500.0f, 845.0f));
		//sfRectangleShape_setScale(en_rectangleShape, vector2f(-_enemy->scale.x, _enemy->scale.y));
		break;
	case EN_STATE_CHASE:
		if (tmpPpos.y > _enemy->pos.y)
			sfRectangleShape_setTextureRect(en_rectangleShape, animatorAnim("moleOutHoleFace", &_enemy->frameX, &_enemy->timerAnim, 0));
		else
			sfRectangleShape_setTextureRect(en_rectangleShape, animatorAnim("moleOutHoleBack", &_enemy->frameX, &_enemy->timerAnim, 0));
		break;
	case EN_STATE_BACK_HOME:
		if (tmpPpos.y > _enemy->pos.y)
			sfRectangleShape_setTextureRect(en_rectangleShape, animatorAnim("moleInHoleFace", &_enemy->frameX, &_enemy->timerAnim, 0));
		else
			sfRectangleShape_setTextureRect(en_rectangleShape, animatorAnim("moleInHoleBack", &_enemy->frameX, &_enemy->timerAnim, 0));
		break;
	case EN_STATE_ATTACK:
		if (tmpPpos.y > _enemy->pos.y)
			sfRectangleShape_setTextureRect(en_rectangleShape, animatorAnim("moleFaceAttack", &_enemy->frameX, &_enemy->timerAnim, 0));
		else
			sfRectangleShape_setTextureRect(en_rectangleShape, animatorAnim("moleBackAttack", &_enemy->frameX, &_enemy->timerAnim, 0));

		break;
	case EN_STATE_THROWN:

		break;
	case EN_STATE_STUNNED:

		break;
	case EN_STATE_CRYING:

		break;
	case EN_STATE_ATTRACTING:

		break;
	case EN_STATE_DAMAGING:
		if (tmpPpos.y < _enemy->pos.y)
		{
			sfRectangleShape_setTextureRect(en_rectangleShape, IntRect(0, 200, 74, 52));
			sfRectangleShape_setSize(en_rectangleShape, vector2f(66.f, 64.f));
		}
		else
		{
			sfRectangleShape_setTextureRect(en_rectangleShape, IntRect(0, 142, 74, 58));
			sfRectangleShape_setSize(en_rectangleShape, vector2f(64.f, 64.f));
		}
		break;
	}



}

void deinitEnemies() {
	if (!en_Textures[EN_IMUNITY]);
	else if (!en_Rectangle ||
		!en_rectangleShape ||
		!en_rectangleTree ||
		!en_rectangleSpider ||
		!en_circleShape
		)
		return;
	sfRectangleShape_destroy(en_Rectangle);
	sfRectangleShape_destroy(en_rectangleShape);
	sfRectangleShape_destroy(en_rectangleTree);
	sfRectangleShape_destroy(en_rectangleSpider);
	sfCircleShape_destroy(en_circleShape);

}

sfBool en_isAnySpiderHere()
{
	for (int i = 0; i < enemiesList->size(enemiesList); i++)
	{
		if (GD_ENEMY->type == EN_YOYO_SPIDER || GD_ENEMY->type == EN_VENIMOUS_YOYO_SPIDER) return sfTrue;
	}
	return sfFalse;
}

void en_fallEntities(sfRectangleShape* _rectangle, sfVector2f* _pos, sfVector2f _forward, float _timer, float _timeofAnim)
{
	static float tmpRotation = 0.0f;
	 tmpRotation += _timer / 50.0f;
	sfVector2f tmpScale = vector2f(1.f - (_timer / _timeofAnim),1.f - (_timer / _timeofAnim));
	addVectorsf(_pos, multiplyVectorsrf(_forward, 0.5f));

	sfRectangleShape_setScale(_rectangle, tmpScale);
	sfRectangleShape_setRotation(_rectangle, tmpRotation);
	sfRectangleShape_setPosition(_rectangle, *_pos);
}

int getEnAmount() {
	return enemiesList->size(enemiesList);
}