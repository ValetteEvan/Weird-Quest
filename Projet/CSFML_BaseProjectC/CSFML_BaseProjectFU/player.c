#include "player.h"
#include "box.h"
#include "pot.h"
#include "collisionManager.h"
#include "controller.h"
#include "math.h"
#include "textureManager.h"
#include "game.h"
#include "viewManager.h"
#include "fontManager.h"
#include "soundManager.h"
#include "ring.h"
#include "enemies.h"
#include "particlesManager.h"
#include "bullets.h"
#include "dialogBox.h"
#include "statue.h"
#include "particlesManager.h"
#include "vibrationManager.h"
#include "FXenemies.h"
#include "game_over.h"
#include "fx_boss.h"
#include "vibrationManager.h"

#define CONTROLLER_OFFSET 0.2f

sfBool isPlayerDead;
Player player;
sfTexture* plr_Texture;
sfTexture* plr_TextureWater;
sfTexture* plr_TextureGrass;
sfTexture* plr_ShadowTexture;


Slash slash;

sfShader* shaderPlayer;
sfRenderStates renderstatePlayer;

sfRectangleShape* plr_Rectangle;
sfRectangleShape* fx_player; 
sfTexture* fx_playerTexture; 

// debug
sfVertexArray* plr_vArray; 

float plr_ClrA;

float timerAnimFx; 
int frameXFX; 
sfBool bool_fxPlayer; 

float plr_timerDebugInputs = 0.f;
sfBool slashBoundsDebug = sfFalse;

sfVector2f plr_TargetPos;

void plr_loadTexture()
{
	plr_TextureWater = GetSfTexture("phaosWater");
	plr_TextureGrass = GetSfTexture("phaosGrass");
}

void plr_Init()
{
	plr_Texture = GetSfTexture("phaos_IDLEB");
	plr_ShadowTexture = GetSfTexture("shadows");
	fx_playerTexture = GetSfTexture("feedback_exclam_player"); 

	plr_TargetPos = vector2f(400.f, 400.f);

	plr_Rectangle = sfRectangleShape_create();
	sfRectangleShape_setFillColor(plr_Rectangle, sfWhite);
	sfRectangleShape_setPosition(plr_Rectangle, vector2f(900.f, 540.f));
	sfRectangleShape_setSize(plr_Rectangle, vector2f(PLR_X_SIZE, PLR_Y_SIZE));



	plr_vArray = sfVertexArray_create();
	sfVertexArray_setPrimitiveType(plr_vArray, sfTrianglesFan);

	player.hp = 12;
	player.timerFall = 0.f;
	player.maxHP = 12;
	player.velocity = vector2f(0.f, 0.f);
	player.animTimer = 0.f;
	player.frameX = 0;
	player.animRect = IntRect(0, 0, PLR_X_SIZE, PLR_Y_SIZE);
	player.pos = vector2f(960.f, 540.f);
	player.forward = vector2f(0.f, 0.f);
	player.oldForward = vector2f(0.f, 1.f);
	player.pigHatTimer = PLR_HOOD_DURATION * 2.f;
	player.invisibilityTimer = PLR_HOOD_DURATION;
	player.isFlying = sfFalse;
	player.flyTimer = 12.f;
	player.ringPower = PLR_NO_POWER;
	player.animPigTimer = 0.f;
	player.frameXPig = 0;
	player.unlockState = PLRUNL_NONE;
	player.interactStatue = sfFalse;
	player.watchDir = vector2f(0.f, 1.f);
	player.oldVelocity = NULLVECTF;
	player.timerButtonPressed = 0.f;
	player.canMove = sfFalse;
	player.isPushing = sfFalse;
	player.timerAttack = -10.f;
	player.timerCatch = -10.f;
	player.timerThrowRing = -10.f;
	player.timerStunned = -10.f;
	player.timerDamaged = -10.f;
	player.timerGetObject = -10.f;
	player.timerDeath = -10.f;
	player.timerWalkPart = 0.f;
	player.timerGrassParticles = 0.f;
	player.timerWaterParticles = 0.f;
	player.pushedTimer = -10.f;
	player.canBePushedTimer = -10.f;
	player.fallOffsetPos = NULLVECTF;
	player.oldPos = player.pos;
	player.timerSaveOldPos = 0.f;
	player.hasObtainedNewItem = sfFalse;
	player.timerGameOver = -10.f;

	plr_ClrA = 255.f;

	player.timerPush = 0.f;

	plr_loadTexture();
	
	fx_player = sfRectangleShape_create();
	sfRectangleShape_setFillColor(fx_player, sfWhite); 
	sfRectangleShape_setPosition(fx_player, vector2f(player.pos.x, player.pos.y));  
	sfRectangleShape_setSize(fx_player, vector2f(64.f, 64.f));  
	sfRectangleShape_setTexture(fx_player, fx_playerTexture, sfTrue);  

	slash.direction = SLASH_UP;
	slash.pos = NULLVECTF;
	slash.velocity = NULLVECTF;
	slash.frameX = 0;
	slash.animTimer = 0.f;
	slash.lifeTime = -10.f;
	slash.state = SLASH_NO_STATE;

	player.walkType = PLR_GROUND;

	timerAnimFx = 0.f; 
	frameXFX = 0; 
	bool_fxPlayer = sfFalse; 
}

void plr_UpdateInteractions(Window* _window)
{
	if (customKeyboardIsKeyPressed(sfKeyV, _window->renderWindow))
		player.unlockState = PLRUNL_DESTRUCT;

	if (sfMouse_isButtonPressed(sfMouseRight) && sfRenderWindow_hasFocus(_window->renderWindow)) {
		player.pos = getfWorldMousePos(_window->renderWindow);
	}

	if (!getIsFreeingMap()) {
		float dt = getDeltaTimeThread2();

		player.timerSaveOldPos += dt;
		if (player.timerSaveOldPos > PLR_SAVE_OLDPOS_DURATION && player.walkType != PLR_VOID) {
			player.timerSaveOldPos = 0.f;
			player.oldPos = player.pos;
		}
		
		player.timerGetObject -= dt;
		if (player.timerGetObject <= 0.f && player.timerGetObject > -5.f) {
			player.timerGetObject = -10.f;
			player.frameX = 0;
			player.animTimer = 0.f;
		}
		
		if (player.timerDamaged > 0.f && !getPause()) {
			SetVibration(0, VIBRATION_IMPACT);
		}

		player.timerDamaged -= dt;
		if (player.timerDamaged <= 0.f && player.timerDamaged > -5.f) {
			player.timerDamaged = -10.f;
			SetVibration(0, VIBRATION_NONE);
			player.frameX = 0;
			player.animTimer = 0.f;

			if (player.hp <= 0) {
				sfVector2i pPosOnMap = vector2i(player.pos.x / 64.f, player.pos.y / 64.f);
				player.walkType = checkPlayerWalkType(pPosOnMap);
				if (player.walkType == PLR_GRASS || player.walkType == PLR_WATER)
					player.timerDeath = PLR_DEATH_NO_GROUND_DURATION - 0.05f;
				else
					player.timerDeath = PLR_DEATH_DURATION - 0.05f;
			}
		}

		slash.lifeTime -= dt;
		if (slash.lifeTime <= 0.f && slash.lifeTime > -5.f) {
			slash.lifeTime = -10.f;
			slash.state = SLASH_NO_STATE;
		}

		if (player.pushedTimer > 0.f) {

		}
		else if (player.interactStatue || player.timerDeath > 0.f || db_IsUp() || (player.walkType == PLR_VOID))
		{
			player.velocity = NULLVECTF;

			if (!player.interactStatue && !db_IsUp()) {
				player.timerDeath -= dt;
				if (player.timerDeath <= 0.f && player.timerDeath > -5.f) {
					player.timerDeath = -10.f;
					player.frameX = 0;
					player.animTimer = 0.f;
					// debug only :
					toggle_gameOver();
					player.timerGameOver = PLR_GAME_OVER_DURATION;
					//player.hp = 12;
					//player.pos = map_getPlayerSpawnPos();
				}

				if (player.walkType == PLR_VOID) {
					player.timerFall += dt;
					if (player.timerFall >= 2.f && player.timerFall < 3.f) {
						player.timerFall = 3.f;
						FXe_addFXenemies(addVectorsrf(player.pos, player.fallOffsetPos), FX_STARS);
					}
					if (player.timerFall >= 3.9f)
					{
						MapState tmpMapState = map_GetState();
						if (tmpMapState == MAP_STATE_OVERWORLD || tmpMapState == MAP_STATE_FOREST)
							player.pos = player.oldPos;
						else
							player.pos = map_getPlayerSpawnPos();
						player.walkType = PLR_GROUND;
						player.pigHatTimer = PLR_HOOD_DURATION * 2.f;
						plr_addDamagePlayerLife(2);
						return;
					}
				}
			}
		}
		else
		{
			// debug
			plr_timerDebugInputs += dt;
			/*if (customKeyboardIsKeyPressed(sfKeyNumpad7, _window->renderWindow) && plr_timerDebugInputs > 0.2f) {
				slashBoundsDebug = !slashBoundsDebug;
				plr_timerDebugInputs = 0.f;
			}*/

			player.timerStunned -= dt;
			if (player.timerStunned <= 0.f && player.timerStunned > -5.f) {
				player.timerStunned = -10.f;
				player.frameX = 0;
				player.animTimer = 0.f;
			}

			if (player.pigHatTimer < PLR_HOOD_DURATION * 2.f) {
				if (player.pigHatTimer < PLR_HOOD_DURATION) {
					if (sfSound_getStatus(GetSound("SFX_Capuche_Cochon")) == sfStopped) {
						playSoundFX("SFX_Capuche_Cochon");
					}
					if (player.pigHatTimer + dt >= PLR_HOOD_DURATION) {
						sfSound_stop(GetSound("SFX_Capuche_Cochon"));
					}
				}
				player.pigHatTimer += dt;
			}

			if (player.flyTimer < PLR_HOOD_DURATION * 2.f) {
				player.flyTimer += dt;
			}

			if (player.invisibilityTimer < PLR_HOOD_DURATION * 2.f) {
				player.invisibilityTimer += dt;
				if (player.invisibilityTimer >= PLR_HOOD_DURATION)
					plr_ClrA = 255.f;
			}

			player.timerThrowRing -= dt;
			if (player.timerThrowRing <= 0.f && player.timerThrowRing > -5.f) {
				player.timerThrowRing = -10.f;
				player.frameX = 0;
				player.animTimer = 0.f;
			}

			if (ring_CanThrow() && Gamepad_getTriggerPos(0, sfFalse, _window->renderWindow) > 0.0f && plr_CanDoAction()) {
				if (player.ringPower == PLR_ATTRACT || player.ringPower == PLR_REPULSE) {
					player.timerThrowRing = PLR_RING_THROW_DURATION - 0.05f;
					player.frameX = 0;
					player.animTimer = 0.f;
					player.velocity = NULLVECTF;

					slash.direction = plr_GetSlashDirection();
					slash.pos = plr_GetSlashStartPos();
					slash.velocity = plr_GetSlashStartVelocity();
					slash.frameX = 0;
					slash.animTimer = 0.f;
					slash.lifeTime = -10.f;
					if (player.ringPower == PLR_ATTRACT)
						slash.state = SLASH_ATTRACT;
					else if (player.ringPower == PLR_REPULSE)
						slash.state = SLASH_REPULSE;
				}
			}

			if (ring_CanThrow() && player.timerThrowRing > 0.f && player.frameX >= 3 && player.ringPower > PLR_NO_POWER) {
				ring_Throw(normalizef(plr_GetSlashStartVelocity())/* player.oldForward*/, player.ringPower);
			}


			if (player.velocity.x != 0 || player.velocity.y != 0)
				player.watchDir = normalizef(player.velocity);
			/*else
				watchDir = vector2f(0.f, 1.f);*/

				// attack
			player.timerAttack -= dt;
			if (player.timerAttack <= 0.f && player.timerAttack > -5.f) {
				player.timerAttack = -10.f;
				player.frameX = 0;
				player.animTimer = 0.f;
			}

			if (Gamepad_isButtonPressed(0, X, _window->renderWindow) && player.unlockState >= PLRUNL_STAFF && plr_CanDoAction()) {
				playSoundFX("SFX_Player_Attack");
				player.timerAttack = PLR_ATTACK_DURATION - 0.05f;
				player.frameX = 0;
				player.animTimer = 0.f;
				player.velocity = NULLVECTF;

				slash.direction = plr_GetSlashDirection();
				slash.pos = plr_GetSlashStartPos();
				slash.velocity = plr_GetSlashStartVelocity();
				slash.frameX = 0;
				slash.animTimer = 0.f;
				slash.lifeTime = -10.f;
				slash.state = SLASH_ATTACK;
			}

			// catch
			player.timerCatch -= dt;
			if (player.timerCatch <= 0.f && player.timerCatch > -5.f) {
				player.timerCatch = -10.f;
				player.frameX = 0;
				player.animTimer = 0.f;
			}

			if (Gamepad_isButtonPressed(0, Y, _window->renderWindow) && player.unlockState >= PLRUNL_RING_CAPTURE && plr_CanDoAction()) {
				playSoundFX("SFX_Player_AttackMagic");
				player.timerCatch = PLR_CATCH_DURATION - 0.05f;
				player.frameX = 0;
				player.animTimer = 0.f;
				player.velocity = NULLVECTF;

				slash.direction = plr_GetSlashDirection();
				slash.pos = plr_GetSlashStartPos();
				slash.velocity = plr_GetSlashStartVelocity();
				slash.frameX = 0;
				slash.animTimer = 0.f;
				slash.lifeTime = -10.f;
				slash.state = SLASH_CATCH;
			}

			if (slash.state != SLASH_NO_STATE && slash.lifeTime < 0.f && (player.timerAttack > 0.f || player.timerCatch > 0.f) && player.frameX >= 3) {
				slash.lifeTime = PLR_SLASH_LIFETIME;
			}

			if ((player.timerAttack > 0.f || player.timerCatch > 0.f || player.timerThrowRing > 0.f) && player.frameX >= 2 && slash.lifeTime > 0.f) {
				addVectorsf(&slash.pos, multiplyVectorsrf(slash.velocity, dt));
			}

			if (player.timerAttack > 0.f || player.timerCatch > 0.f || slash.lifeTime > 0.f) {
				sfFloatRect slashBounds = plr_GetDamageBounds();

				if (slash.state == SLASH_ATTACK) {
					if (isNutHere())
						blt_shouldSendBackNut(&slashBounds);

					en_IsInDamageBounds(&slashBounds, sfTrue, _window);
				}
				else if (slash.state == SLASH_CATCH)
					en_IsInDamageBounds(&slashBounds, sfFalse, _window);
			}

			//		sfVector2f pFuturPos = addVectorsrf(player.pos, multiplyVectorsrf(player.velocity, dt));

			/// Totem hood selection
			if (plr_CanDoAction() && ring_CanThrow())
				plr_SelectPower();

			if (Gamepad_getTriggerPos(0, sfTrue, _window->renderWindow) > 0.0f && player.invisibilityTimer >= PLR_HOOD_DURATION * 2.f && player.unlockState >= PLRUNL_INVISIBILITY_HAT) {
				player.invisibilityTimer = 0.f;
				plr_ClrA = 100.f;
			}

			player.pigHatTimer += dt;

			if (Gamepad_isButtonPressed(0, LB, _window->renderWindow) && player.pigHatTimer >= PLR_HOOD_DURATION * 2.f && player.unlockState >= PLRUNL_PIG_HAT) {
				player.pigHatTimer = 0.f;
				playSoundFX("SFX_Capuche_Cochon");
				player.animPigTimer = 0.f;
				player.frameXPig = 0;
			}

			// can't interact with statues if forced anim
			if (plr_CanDoAction() && stt_GetInteractCooldown() < .01f && !player.interactStatue) {
				for (int i = 0; i < stt_List->size(stt_List); i++) {
					Statue* tmpStatue = STD_LIST_GETDATA(stt_List, Statue, i);
					// Custom rect for interaction with the statue
					sfFloatRect tmpRectStatue = FlRect(tmpStatue->pos.x + tmpStatue->size.x * 0.25f, tmpStatue->pos.y + tmpStatue->size.y / 2, tmpStatue->size.x * 0.85f, tmpStatue->size.y / 2);
					/*if (Gamepad_isButtonPressed(0, A, _window->renderWindow) && sfFloatRect_contains(&tmpRectStatue, player.pos.x, player.pos.y) && stt_GetInteractCooldown() < .01f) {
						player.interactStatue = sfTrue;
						stt_SetInteractCooldown(.5f);
					}*/
				}
			}
			sfVector2i pPosOnMap = vector2i(player.pos.x / 64.f, player.pos.y / 64.f);

			player.walkType = checkPlayerWalkType(pPosOnMap);
			MapState tmpMState = map_GetState();
			if (tmpMState >= MAP_STATE_DUNGEON_JOY_1 && tmpMState <= MAP_STATE_DUNGEON_JOY_FOUNTAIN) {
				if (player.walkType == PLR_VOID) {
					sfVector2i pPosOnMap2 = vector2i(player.pos.x / 64.f, (player.pos.y - 20.f) / 64.f);
					player.walkType = checkPlayerWalkType(pPosOnMap2);
					if (player.walkType == PLR_VOID) {
						sfVector2i pPosOnMap3 = vector2i((player.pos.x - 20.f) / 64.f, (player.pos.y - 10.f) / 64.f);
						player.walkType = checkPlayerWalkType(pPosOnMap3);
						if (player.walkType == PLR_VOID) {
							sfVector2i pPosOnMap4 = vector2i((player.pos.x + 20.f) / 64.f, (player.pos.y - 10.f) / 64.f);
							player.walkType = checkPlayerWalkType(pPosOnMap4);
							if (player.walkType == PLR_VOID) {
								player.timerFall = 0.f;
								player.fallOffsetPos = multiplyVectorsrf(player.oldForward, 32.f);
							}
						}
					}
				}
			}
			else {
				if (player.walkType == PLR_VOID) {
					player.timerFall = 0.f;
					player.fallOffsetPos = multiplyVectorsrf(player.oldForward, 32.f);
				}
			}
		}

	}
}

void plr_UpdateMovement(Window* _window)
{
	plr_toggleFX(sfFalse);

	float dt = getDeltaTime();
	player.timerWaterParticles += dt;
	player.timerGrassParticles += dt;
	player.timerWalkPart += dt;
	player.canBePushedTimer -= dt;

	if (player.timerGameOver > 0.f)
		return;

	if (player.pushedTimer > 0.f)
	{
		player.pushedTimer -= dt;
		// if collisions with nextPos -> player.pushedTimer = 0.f and player.velocity = NULLVECTF ?
		sfVector2f playerNextPos = addVectorsrf(player.pos, multiplyVectorsrf(player.velocity, dt));

		sfFloatRect rectPlayer = FlRect(player.pos.x - 28.f, player.pos.y - 56.f, 56.f, 56.f);

		if (cll_RectMap(rectPlayer, vector2f(player.velocity.x * dt, 0.f))) {
			player.velocity.x = 0.f;
			if (player.isDashing) {
				player.isDashing = sfFalse;
				player.timerChargeRush = 0.f;
			}
			else if (player.isRushing) {
				player.isRushing = sfFalse;
			}
			player.pushedTimer = 0.f;
		}

		if (cll_RectMap(rectPlayer, vector2f(0.f, player.velocity.y * dt))) {
			player.velocity.y = 0.f;
			if (player.isDashing) {
				player.isDashing = sfFalse;
				player.timerChargeRush = 0.f;
			}

			else if (player.isRushing) {
				player.isRushing = sfFalse;
			}
			player.pushedTimer = 0.f;
		}
	}
	else if (player.interactStatue || plr_IsDoingAction() || player.timerDamaged > 0.f || player.timerGetObject > 0.f || player.timerDeath > 0.f || db_IsUp() || player.walkType == PLR_VOID)
	{
		player.velocity = NULLVECTF;
		player.isDashing = sfFalse;
		player.isRushing = sfFalse;
		player.timerButtonPressed = 0.f;
		player.timerChargeRush = 0.f;
	}
	else {
		player.forward = Gamepad_getNormalizedStickPos(0, STICKL, _window->renderWindow);

		sfBool isXMoving = sfFalse;
		sfBool isYMoving = sfFalse;

		if (player.isPushing) {
			player.timerPush += dt;
		}

		//debut de la gestion du rush du joueur
		player.rushIsCharging = sfFalse;
		if (Gamepad_isButtonPressed(0, B, _window->renderWindow) && !player.isRushing && !player.isDashing && player.unlockState >= PLRUNL_SHOULDER_CHARGE && player.walkType == PLR_GROUND) {
			player.rushIsCharging = sfTrue;
			player.timerButtonPressed += dt;
			plr_ChargeRush();
		}
		else if (!player.rushIsCharging && player.timerChargeRush > 0.f && (player.timerButtonPressed > 0.4f || player.isRushing)) {
			plr_Rush(player.oldForward, &isXMoving, &isYMoving);
			sfBool wasRushing = player.isRushing;
			player.isRushing = sfTrue;
			player.timerChargeRush -= dt * 2.f;
			if (player.walkType != PLR_GROUND)
				player.timerChargeRush = 0.f;
			player.timerButtonPressed = 0.f;
			if (player.timerChargeRush <= 0.f) {
				player.isRushing = sfFalse;
				sfSound_stop(GetSound("SFX_Player_Run"));
				player.timerChargeRush = 0.f;
			}
			if (!wasRushing) {
				playSoundFX("SFX_Player_Run");

				player.frameX = 0;
				player.animTimer = 0.f;
			}
		}
		else if (!player.rushIsCharging && player.timerChargeRush > 0.f && player.timerButtonPressed <= 0.4f && !player.isDashing && !player.isRushing) {
			player.timerChargeRush = 0.f;
			player.isDashing = sfTrue;
			playSoundFX("SFX_Player_Dash");
			player.frameX = 0;
			player.animTimer = 0.f;
		}
		else if (!Gamepad_isButtonPressed(0, B, _window->renderWindow))
			player.timerButtonPressed = 0.f;


		// si le joueur est en train de rush alors il ne peut pas controler son personnage
		if (!player.isRushing && !player.rushIsCharging && !player.isDashing)
		{
			sfMutex_lock(mutexThread2);
			if (player.forward.x > CONTROLLER_OFFSET) {
				isXMoving = sfTrue;
				player.velocity.x = PLR_SPEED;
				player.oldForward = player.forward;
			}
			else if (player.forward.x < -CONTROLLER_OFFSET) {
				isXMoving = sfTrue;
				player.velocity.x = -PLR_SPEED;
				player.oldForward = player.forward;

			}
			else player.velocity.x = 0.f;

			if (isXMoving) { // diagonal movement
				if (player.forward.y > CONTROLLER_OFFSET) {
					player.velocity.y = PLR_SPEED;
				}
				else if (player.forward.y < -CONTROLLER_OFFSET) {
					player.velocity.y = -PLR_SPEED;
				}
				else {
					player.velocity.y = 0.f;
				}
			}
			else if (player.forward.y > CONTROLLER_OFFSET) {
				isYMoving = sfTrue;
				player.velocity.y = PLR_SPEED;
				player.oldForward = player.forward;
			}
			else if (player.forward.y < -CONTROLLER_OFFSET) {
				isYMoving = sfTrue;
				player.velocity.y = -PLR_SPEED;

				player.oldForward = player.forward;

			}
			else {
				player.velocity.y = 0.f;
			}
			sfMutex_unlock(mutexThread2);
		}

		if (player.isDashing) {
			if (player.walkType != PLR_GROUND) {
				player.isDashing = sfFalse;
				player.timerChargeRush = 0.f;

			}
		}

		/// Old movement code
		// if the player is not using an ability
		int xMove = player.oldForward.x > 0 ? 1 : -1;
		int yMove = player.oldForward.y > 0 ? 1 : -1;

		if (player.isRushing)
			player.velocity = multiplyVectorsrf(normalizef(player.velocity), PLR_DASH_SPEED - 300.f);
		else if (!player.isDashing)
			player.velocity = multiplyVectorsrf(normalizef(player.velocity), PLR_SPEED);
		else {
			player.timerChargeRush += dt;
			if (player.oldForward.x > CONTROLLER_OFFSET) {
				isXMoving = sfTrue;
				player.velocity.x = PLR_DASH_SPEED;
			}
			else if (player.oldForward.x < -CONTROLLER_OFFSET) {
				isXMoving = sfTrue;
				player.velocity.x = -PLR_DASH_SPEED;
			}
			if (isXMoving) {
				if (player.oldForward.y > CONTROLLER_OFFSET) {
					player.velocity.y = PLR_DASH_SPEED;
				}
				else if (player.oldForward.y < -CONTROLLER_OFFSET) {
					player.velocity.y = -PLR_DASH_SPEED;
				}
				else {
					player.velocity.y = 0.f;
				}
			}
			else if (player.oldForward.y > CONTROLLER_OFFSET) {
				isYMoving = sfTrue;
				player.velocity.y = PLR_DASH_SPEED;
			}
			else if (player.oldForward.y < -CONTROLLER_OFFSET) {
				isYMoving = sfTrue;
				player.velocity.y = -PLR_DASH_SPEED;
			}
			player.velocity = multiplyVectorsrf(normalizef(player.velocity), float_lerp_invCube(PLR_DASH_SPEED, PLR_SPEED, player.timerChargeRush));
			if (player.timerChargeRush > 1.f) {
				player.isDashing = sfFalse;
				player.timerChargeRush = 0.f;
			}
		}

		if (!isXMoving && !isYMoving) {
			player.velocity = vector2f(0.f, 0.f);
		}

		sfVector2f playerNextPos = addVectorsrf(player.pos, multiplyVectorsrf(player.velocity, dt));

		/// Interactions with pushable boxes
		sfBool hasPushedABox = sfFalse;
		// if player.ispushing
		for (int i = 0; i < boxList->size(boxList); i++) {
			if (BOX_GETDATA->hasFallen && BOX_GETDATA->frameX == 5)
				continue;
			sfVector2f vectCompar = playerNextPos;
			if (player.pos.y > BOX_GETDATA->pos.y)
				vectCompar = addVectorsrf(playerNextPos, vector2f(-PLR_X_SIZE / 2.f, -PLR_Y_SIZE / 2.f));
			else
				vectCompar = addVectorsrf(playerNextPos, vector2f(-PLR_X_SIZE / 2.f, -PLR_Y_SIZE));

			if (cll_BoxBox(vectCompar, BOX_GETDATA)) {
				if (player.watchDir.x == 1.f || player.watchDir.x == -1.f || player.watchDir.y == 1.f || player.watchDir.y == -1.f) {
					/// Applies a sudden surge of speed if lunged into
					if (player.isRushing) {
						ShoulderHitBox(player.watchDir, BOX_GETDATA);
						player.timerChargeRush = 0.f;
						player.isRushing = sfFalse;
						sfSound_stop(GetSound("SFX_Player_Run"));
					}
					else if (player.isDashing) {
						player.isDashing = sfFalse;
						player.timerChargeRush = 0.f;
					}
					/// Just pushes the box in the direction the player is looking at
					else {
						PushBox(player.oldForward, BOX_GETDATA);
						if (sfSound_getStatus(GetSound("SFX_Player_BoxPushed")) == sfStopped) {
							playSoundFX("SFX_Player_BoxPushed");
						}
						player.timerPush = 0.f;
						if (!player.isPushing) {
							player.isPushing = sfTrue;
							player.frameX = 0;
							player.animTimer = 0.f;
							hasPushedABox = sfTrue;
						}
					}
				}
				/// In any case, the player stops moving
				if (player.isDashing) {
					player.isDashing = sfFalse;
					player.timerChargeRush = 0.f;
				}
				player.velocity = vector2f(0.f, 0.f);
			}
		}
		if (!hasPushedABox && player.isPushing && player.timerPush > 0.2f) {
			player.isPushing = sfFalse;
			sfSound_stop(GetSound("SFX_Player_BoxPushed"));
			player.frameX = 0;
			player.animTimer = 0.f;
		}
		if (player.timerPush > 0.8f) {
			player.timerPush = 0.f;
		}


		for (int i = 0; i < pot_List->size(pot_List); i++) {
			PotData* tmp = POT_GETDATA;

			if (!tmp->isOnScreen) continue;

			if (pot_PlayerCollision(tmp, multiplyVectorsrf(vector2f(player.velocity.x, 0.f), dt))) {
				if (player.isRushing) {
					if (tmp->type == POT_REG || (tmp->type == POT_HARD && tmp->hp == 2)) tmp->hp--;
					player.isRushing = sfFalse;
					player.timerChargeRush = 0.f;
				}
				else if (player.isDashing) {
					player.isDashing = sfFalse;
					player.timerChargeRush = 0.f;
				}
				player.velocity.x = 0.f;
				player.pushedTimer = 0.f;
			}
			if (pot_PlayerCollision(tmp, multiplyVectorsrf(vector2f(0.f, player.velocity.y), dt))) {
				if (player.isRushing) {
					if (tmp->type == POT_REG || (tmp->type == POT_HARD && tmp->hp == 2)) tmp->hp--;
					player.isRushing = sfFalse;
					player.timerChargeRush = 0.f;
				}
				else if (player.isDashing) {
					player.isDashing = sfFalse;
					player.timerChargeRush = 0.f;
				}
				player.velocity.y = 0.f;
				player.pushedTimer = 0.f;
			}
		}

		if (en_IsPlayerBlockedByChameleonTongue(playerNextPos)) {
			player.velocity = vector2f(0.f, 0.f);
		}

		sfFloatRect rectPlayer = FlRect(player.pos.x - 28.f, player.pos.y - 56.f, 56.f, 56.f);

		if (cll_RectMap(rectPlayer, vector2f(player.velocity.x * dt, 0.f))) {
			player.velocity.x = 0.f;
			if (player.isDashing) {
				player.isDashing = sfFalse;
				player.timerChargeRush = 0.f;
			}
			else if (player.isRushing) {
				player.isRushing = sfFalse;
				sfSound_stop(GetSound("SFX_Player_Run"));
				player.timerChargeRush = 0.f;
			}
		}

		if (cll_RectMap(rectPlayer, vector2f(0.f, player.velocity.y * dt))) {
			player.velocity.y = 0.f;
			if (player.isDashing) {
				player.isDashing = sfFalse;
				player.timerChargeRush = 0.f;
			}

			else if (player.isRushing) {
				player.isRushing = sfFalse;
				player.timerChargeRush = 0.f;
				sfSound_stop(GetSound("SFX_Player_Run"));
			}
		}
	}

	if (!equals(player.oldVelocity, player.velocity)) {

		player.oldVelocity = player.velocity;
		if (equals(player.velocity, NULLVECTF) && !player.isPushing)
		{
			player.frameX = 0;
			player.animTimer = 0.f;
		}
	}

	if (!player.canMove) player.velocity = NULLVECTF;
	if (!player.isDashing && !player.isRushing && (player.velocity.x != 0.f || player.velocity.y != 0.f)) {
		if (!GetSound("SFX_Player_Walk")) return;
		if (sfSound_getStatus(GetSound("SFX_Player_Walk")) == sfStopped) {
			sfSound_setPitch(GetSound("SFX_Player_Walk"), rand_float(1.f, 2.f));
			playSoundFX("SFX_Player_Walk");
		}
	}
	addVectorsf(&player.pos, multiplyVectorsrf(player.velocity, dt));
	player.canMove = sfTrue;

	if (player.hp > player.maxHP) player.hp = player.maxHP;

	sfRectangleShape_setPosition(fx_player, vector2f(player.pos.x, player.pos.y + 100.f));
	sfRectangleShape_setSize(fx_player, vector2f(64.f, 64.f)); 
	sfRectangleShape_setOrigin(fx_player, vector2f(64.f / 2, 64.f / 2)); 
	sfRectangleShape_setTextureRect(fx_player, animatorAnim("feedback_exclam_player", &frameXFX, &timerAnimFx, 0)); 
}

void plr_Display(Window* _window, sfSprite* spr)
{
	if (player.hasObtainedNewItem && player.frameX == 14)
	{
		player.hasObtainedNewItem = sfFalse;
		prt_CreateGainParticles(vector2f(plr_GetPos().x - 10.f, plr_GetPos().y - 130.f));
	}

	sfRectangleShape_setFillColor(plr_Rectangle, sfColor_fromRGBA(255, 255, 255, plr_ClrA));
	sfRectangleShape_setPosition(plr_Rectangle, player.pos);
	sfRectangleShape_setOutlineThickness(plr_Rectangle, 0.f);


	if (player.walkType == PLR_WATER)
		sfRectangleShape_setTexture(plr_Rectangle, plr_TextureWater, sfFalse);
	else if (player.walkType == PLR_GRASS)
		sfRectangleShape_setTexture(plr_Rectangle, plr_TextureGrass, sfFalse);
	else
		sfRectangleShape_setTexture(plr_Rectangle, plr_Texture, sfFalse);


	if (getState() != EDITOR)
		sfMutex_lock(mutexThread2);
	if (player.timerGameOver > 0.f) {
		if (player.walkType == PLR_GRASS || player.walkType == PLR_WATER) {
			sfRectangleShape_setSize(plr_Rectangle, NULLVECTF);
		}
		else {
			sfRectangleShape_setTextureRect(plr_Rectangle, animatorWithResizeAndOriginOffset("phaos_DEAD", &player.frameX, &player.animTimer, 0, plr_Rectangle, sfFalse, vector2f(-33.f, -22.f)));
			sfRectangleShape_setTextureRect(plr_Rectangle, IntRect(2400, 5326, 150, 138));
			sfRectangleShape_setSize(plr_Rectangle, vector2f(150, 138));
			//sfRectangleShape_setOrigin(plr_Rectangle, vector2f(-33.f, -22.f));
			sfRectangleShape_setScale(plr_Rectangle, vector2f(1.f, 1.f));
			sfRectangleShape_setRotation(plr_Rectangle, 0.f);
		}
	}
	else if (player.timerDeath > 0.f) {
		sfRectangleShape_setTextureRect(plr_Rectangle, animatorWithResizeAndOriginOffset("phaos_DEAD", &player.frameX, &player.animTimer, 0, plr_Rectangle, sfFalse, vector2f(-33.f, -22.f)));
		sfRectangleShape_setScale(plr_Rectangle, vector2f(1.f, 1.f));
		//if (player.frameX >= 14 && !get_gameOverState())
		//	toggle_gameOver();
	}
	else if (player.walkType == PLR_VOID) {
		if (player.timerFall < 2.f) {
			sfVector2f scale;
			scale.x = 1.f - (player.timerFall / 2.f);
			scale.y = 1.f - (player.timerFall / 2.f);
			float rotation = (player.timerFall * 150.f);
			sfRectangleShape_setPosition(plr_Rectangle, addVectorsrf(player.pos, player.fallOffsetPos));
			sfRectangleShape_setScale(plr_Rectangle, scale);
			sfRectangleShape_setRotation(plr_Rectangle, rotation);
			sfRectangleShape_setTextureRect(plr_Rectangle, IntRect(0, 0, PLR_X_SIZE, PLR_Y_SIZE));
			sfRectangleShape_setSize(plr_Rectangle, vector2f(PLR_X_SIZE, PLR_Y_SIZE));
			sfRectangleShape_setOrigin(plr_Rectangle, vector2f(PLR_X_SIZE / 2.f, PLR_Y_SIZE / 2.f));
		}
		else {
			sfRectangleShape_setSize(plr_Rectangle, NULLVECTF);
		}
	}
	else if (player.timerGetObject > PLR_HOLD_OBJECT_DURATION) {
		sfRectangleShape_setTextureRect(plr_Rectangle, animatorWithResizeAndOrigin("phaos_PICKUP", &player.frameX, &player.animTimer, 0, plr_Rectangle, sfTrue, NULLVECTF));
		sfRectangleShape_setScale(plr_Rectangle, vector2f(1.f, 1.f));
	}
	else if (player.timerGetObject > 0.f) {
		sfRectangleShape_setTextureRect(plr_Rectangle, IntRect(2016, 2996, 144, 130));
		sfRectangleShape_setSize(plr_Rectangle, vector2f(144.f, 130.f));
		sfRectangleShape_setOrigin(plr_Rectangle, vector2f(72.f, 130.f));
		sfRectangleShape_setScale(plr_Rectangle, vector2f(1.f, 1.f));
	}
	else if (player.velocity.y == 0.f && player.velocity.x == 0.f) {
		if (player.oldForward.y != 0.f) {
			if (player.oldForward.y > 0.f) {

				if (player.isPushing)
					sfRectangleShape_setTextureRect(plr_Rectangle, animatorWithResizeAndOriginOffset("phaos_PUSHF", &player.frameX, &player.animTimer, 0, plr_Rectangle, sfFalse, vector2f(5.f, 0.f)));
				else if (player.timerDamaged > 0.f) {
					sfRectangleShape_setTextureRect(plr_Rectangle, IntRect(128, 5464, 64, 116));
					sfRectangleShape_setSize(plr_Rectangle, vector2f(66.f, 116.f));
					sfRectangleShape_setOrigin(plr_Rectangle, vector2f(32.f, 116.f));
				}
				else if (player.timerStunned > 0.f)
					sfRectangleShape_setTextureRect(plr_Rectangle, animatorWithResizeAndOrigin("phaos_STUNF", &player.frameX, &player.animTimer, 0, plr_Rectangle, sfTrue, NULLVECTF));
				else if (player.timerAttack > 0.f)
					sfRectangleShape_setTextureRect(plr_Rectangle, animatorWithResizeAndOriginOffset("phaos_ATTACKF", &player.frameX, &player.animTimer, 0, plr_Rectangle, sfFalse, vector2f(-10.f, -42.f)));
				else if (player.timerThrowRing > 0.f) {
					if (player.ringPower == PLR_ATTRACT)
						sfRectangleShape_setTextureRect(plr_Rectangle, animatorWithResizeAndOriginOffset("phaos_ATTRACTF", &player.frameX, &player.animTimer, 0, plr_Rectangle, sfFalse, vector2f(-10.f, -42.f)));
					else if (player.ringPower == PLR_REPULSE)
						sfRectangleShape_setTextureRect(plr_Rectangle, animatorWithResizeAndOriginOffset("phaos_REPULSEF", &player.frameX, &player.animTimer, 0, plr_Rectangle, sfFalse, vector2f(-10.f, -42.f)));
				}
				else if (player.timerCatch > 0.f) {
					sfRectangleShape_setTextureRect(plr_Rectangle, animatorWithResizeAndOriginOffset("phaos_CATCHF", &player.frameX, &player.animTimer, 0, plr_Rectangle, sfFalse, vector2f(-10.f, -42.f)));
				}
				else if (player.rushIsCharging) {
					if (player.timerChargeRush < 1.f)
						sfRectangleShape_setTextureRect(plr_Rectangle, animatorWithResizeAndOriginOffset("phaos_SHOULDCF", &player.frameX, &player.animTimer, 0, plr_Rectangle, sfFalse, vector2f(0.f, -4.f)));
					else
						sfRectangleShape_setTextureRect(plr_Rectangle, animatorWithResizeAndOriginOffset("phaos_SHOULDRF", &player.frameX, &player.animTimer, 0, plr_Rectangle, sfFalse, vector2f(-1.f, -2.f)));
				}
				else
					sfRectangleShape_setTextureRect(plr_Rectangle, animatorWithResizeAndOrigin("phaos_IDLEF", &player.frameX, &player.animTimer, 0, plr_Rectangle, sfTrue, NULLVECTF));

			}
			else {
				if (player.isPushing)
					sfRectangleShape_setTextureRect(plr_Rectangle, animatorWithResizeAndOriginOffset("phaos_PUSHB", &player.frameX, &player.animTimer, 0, plr_Rectangle, sfFalse, vector2f(5.f, 0.f)));
				else if (player.timerDamaged > 0.f) {
					
					sfRectangleShape_setTextureRect(plr_Rectangle, IntRect(128, 5580, 64, 116));
					sfRectangleShape_setSize(plr_Rectangle, vector2f(66.f, 116.f));
					sfRectangleShape_setOrigin(plr_Rectangle, vector2f(32.f, 116.f));
				}
				else if (player.timerStunned > 0.f)
					sfRectangleShape_setTextureRect(plr_Rectangle, animatorWithResizeAndOrigin("phaos_STUNB", &player.frameX, &player.animTimer, 0, plr_Rectangle, sfTrue, NULLVECTF));
				else if (player.timerAttack > 0.f)
					sfRectangleShape_setTextureRect(plr_Rectangle, animatorWithResizeAndOriginOffset("phaos_ATTACKB", &player.frameX, &player.animTimer, 0, plr_Rectangle, sfFalse, vector2f(8.f, 0.f)));
				else if (player.timerThrowRing > 0.f) {
					if (player.ringPower == PLR_ATTRACT)
						sfRectangleShape_setTextureRect(plr_Rectangle, animatorWithResizeAndOriginOffset("phaos_ATTRACTB", &player.frameX, &player.animTimer, 0, plr_Rectangle, sfFalse, vector2f(8.f, 0.f)));
					else if (player.ringPower == PLR_REPULSE)
						sfRectangleShape_setTextureRect(plr_Rectangle, animatorWithResizeAndOriginOffset("phaos_REPULSEB", &player.frameX, &player.animTimer, 0, plr_Rectangle, sfFalse, vector2f(8.f, 0.f)));
				}
				else if (player.timerCatch > 0.f) {
					sfRectangleShape_setTextureRect(plr_Rectangle, animatorWithResizeAndOriginOffset("phaos_CATCHB", &player.frameX, &player.animTimer, 0, plr_Rectangle, sfFalse, vector2f(8.f, 0.f)));
				}
				else if (player.rushIsCharging) {
					if (player.timerChargeRush < 1.f)
						sfRectangleShape_setTextureRect(plr_Rectangle, animatorWithResizeAndOriginOffset("phaos_SHOULDCB", &player.frameX, &player.animTimer, 0, plr_Rectangle, sfFalse, vector2f(0.f, -4.f)));
					else
						sfRectangleShape_setTextureRect(plr_Rectangle, animatorWithResizeAndOriginOffset("phaos_SHOULDRB", &player.frameX, &player.animTimer, 0, plr_Rectangle, sfFalse, vector2f(-1.f, -2.f)));

				}
				else
					sfRectangleShape_setTextureRect(plr_Rectangle, animatorWithResizeAndOrigin("phaos_IDLEB", &player.frameX, &player.animTimer, 0, plr_Rectangle, sfTrue, NULLVECTF));

			}
		}
		else {
			if (player.isPushing) {
				sfRectangleShape_setTextureRect(plr_Rectangle, animatorWithResizeAndOrigin("phaos_PUSHS", &player.frameX, &player.animTimer, 0, plr_Rectangle, sfTrue, NULLVECTF));
			}
			else if (player.timerDamaged > 0.f) {
				shakingView(player.timerDamaged);
				sfRectangleShape_setTextureRect(plr_Rectangle, IntRect(120, 5696, 60, 116));
				sfRectangleShape_setSize(plr_Rectangle, vector2f(60.f, 116.f));
				sfRectangleShape_setOrigin(plr_Rectangle, vector2f(29.f, 116.f));
			}
			else if (player.timerStunned > 0.f)
				sfRectangleShape_setTextureRect(plr_Rectangle, animatorWithResizeAndOriginOffset("phaos_STUN", &player.frameX, &player.animTimer, 0, plr_Rectangle, sfFalse, vector2f(-1.f, 0.f)));
			else if (player.timerAttack > 0.f) {
				if (player.oldForward.x > 0.f) {
					sfRectangleShape_setTextureRect(plr_Rectangle, animatorWithResizeAndOriginOffset("phaos_ATTACKSR", &player.frameX, &player.animTimer, 0, plr_Rectangle, sfFalse, vector2f(-30.f, 0.f)));
				}
				else {
					if (player.walkType != PLR_WATER && player.walkType != PLR_GRASS)
						sfRectangleShape_setTextureRect(plr_Rectangle, animatorWithResizeAndOriginOffset("phaos_ATTACKSL", &player.frameX, &player.animTimer, 0, plr_Rectangle, sfFalse, vector2f(37.f, 0.f)));
					else
						sfRectangleShape_setTextureRect(plr_Rectangle, animatorWithResizeAndOriginOffset("phaos_ATTACKSL2", &player.frameX, &player.animTimer, 0, plr_Rectangle, sfFalse, vector2f(36.f, 0.f)));
				}
			}
			else if (player.timerThrowRing > 0.f) {
				if (player.oldForward.x > 0.f) {
					if (player.ringPower == PLR_ATTRACT)
						sfRectangleShape_setTextureRect(plr_Rectangle, animatorWithResizeAndOriginOffset("phaos_ATTRACTSR", &player.frameX, &player.animTimer, 0, plr_Rectangle, sfFalse, vector2f(-30.f, 0.f)));
					else if (player.ringPower == PLR_REPULSE) {
						sfRectangleShape_setTextureRect(plr_Rectangle, animatorWithResizeAndOriginOffset("phaos_REPULSESR", &player.frameX, &player.animTimer, 0, plr_Rectangle, sfFalse, vector2f(-30.f, 0.f)));
					}
				}
				else {
					if (player.ringPower == PLR_ATTRACT)
					{
						if (player.walkType != PLR_WATER && player.walkType != PLR_GRASS)
							sfRectangleShape_setTextureRect(plr_Rectangle, animatorWithResizeAndOriginOffset("phaos_ATTRACTSL", &player.frameX, &player.animTimer, 0, plr_Rectangle, sfFalse, vector2f(37.f, 0.f)));
						else
							sfRectangleShape_setTextureRect(plr_Rectangle, animatorWithResizeAndOriginOffset("phaos_ATTRACTSL2", &player.frameX, &player.animTimer, 0, plr_Rectangle, sfFalse, vector2f(36.f, 0.f)));
					}
					else if (player.ringPower == PLR_REPULSE) {
						if (player.walkType != PLR_WATER && player.walkType != PLR_GRASS)
							sfRectangleShape_setTextureRect(plr_Rectangle, animatorWithResizeAndOriginOffset("phaos_REPULSESL", &player.frameX, &player.animTimer, 0, plr_Rectangle, sfFalse, vector2f(37.f, 0.f)));
						else
							sfRectangleShape_setTextureRect(plr_Rectangle, animatorWithResizeAndOriginOffset("phaos_REPULSESL2", &player.frameX, &player.animTimer, 0, plr_Rectangle, sfFalse, vector2f(36.f, 0.f)));

					}
				}
			}
			else if (player.timerCatch > 0.f) {
				if (player.oldForward.x > 0.f)
					sfRectangleShape_setTextureRect(plr_Rectangle, animatorWithResizeAndOriginOffset("phaos_CATCHSR", &player.frameX, &player.animTimer, 0, plr_Rectangle, sfFalse, vector2f(-30.f, 0.f)));
				else {
					if (player.walkType != PLR_WATER && player.walkType != PLR_GRASS)
						sfRectangleShape_setTextureRect(plr_Rectangle, animatorWithResizeAndOriginOffset("phaos_CATCHSL", &player.frameX, &player.animTimer, 0, plr_Rectangle, sfFalse, vector2f(37.f, 0.f)));
					else
						sfRectangleShape_setTextureRect(plr_Rectangle, animatorWithResizeAndOriginOffset("phaos_CATCHSL2", &player.frameX, &player.animTimer, 0, plr_Rectangle, sfFalse, vector2f(36.f, 0.f)));
				}
			}
			else if (player.rushIsCharging) {
				if (player.timerChargeRush < 1.f)
					sfRectangleShape_setTextureRect(plr_Rectangle, animatorWithResizeAndOriginOffset("phaos_SHOULDCS", &player.frameX, &player.animTimer, 0, plr_Rectangle, sfFalse, vector2f(5.f, -6.f)));
				else
					sfRectangleShape_setTextureRect(plr_Rectangle, animatorWithResizeAndOriginOffset("phaos_SHOULDRS", &player.frameX, &player.animTimer, 0, plr_Rectangle, sfFalse, vector2f(2.f, -2.f)));
			}
			else {
				sfRectangleShape_setTextureRect(plr_Rectangle, animatorWithResizeAndOrigin("phaos_IDLES", &player.frameX, &player.animTimer, 0, plr_Rectangle, sfTrue, NULLVECTF));

			}
			if (player.oldForward.x > 0.f)
				sfRectangleShape_setScale(plr_Rectangle, vector2f(1.f, 1.f));
			else
				sfRectangleShape_setScale(plr_Rectangle, vector2f(-1.f, 1.f));

		}
	}
	else {
		if (player.velocity.y != 0.f) {
			if (player.velocity.y > 0.f) {
				if (player.isDashing) {
					sfRectangleShape_setTextureRect(plr_Rectangle, animatorWithResizeAndOriginOffset("phaos_DASHF", &player.frameX, &player.animTimer, 0, plr_Rectangle, sfFalse, vector2f(6.f, 8.f)));
				}
				else if (player.isPushing) {
					sfRectangleShape_setTextureRect(plr_Rectangle, animatorWithResizeAndOriginOffset("phaos_PUSHF", &player.frameX, &player.animTimer, 0, plr_Rectangle, sfFalse, vector2f(5.f, 0.f)));
				}
				else if (player.isRushing) {
					sfRectangleShape_setTextureRect(plr_Rectangle, animatorWithResizeAndOriginOffset("phaos_SHOULDAF", &player.frameX, &player.animTimer, 0, plr_Rectangle, sfFalse, vector2f(8.f, -2.f)));
				}
				else {
					sfRectangleShape_setTextureRect(plr_Rectangle, animatorWithResizeAndOrigin("phaos_WALKF", &player.frameX, &player.animTimer, 0, plr_Rectangle, sfTrue, NULLVECTF));
					float angleptr = getAngleBetweenVectors(vector2f(1.f, 0.f), vector2f(-player.forward.x, -player.forward.y));
					angleptr *= RAD2DEG;
					if (player.walkType != PLR_WATER && player.walkType != PLR_GRASS)
					{
						if (player.timerWalkPart > 0.3)
						{
							prt_CreateParticlesPlayerDust(plr_GetPos(), angleptr);
							player.timerWalkPart = 0.f;
						}
					}
					else if (player.walkType == PLR_GRASS)
					{
						if (player.timerGrassParticles > 0.15)
						{
							prt_CreateGrassParticles(plr_GetPos(), angleptr);
							player.timerGrassParticles = 0.f;
						}
					}
					else if (player.walkType == PLR_WATER)
					{
						if (player.timerWaterParticles > 0.15)
						{
							prt_CreateWaterSplahParticles(plr_GetPos(), angleptr);
							player.timerWaterParticles = 0.f;
						}
					}
				}
			}
			else {
				if (player.isDashing) {
					sfRectangleShape_setTextureRect(plr_Rectangle, animatorWithResizeAndOriginOffset("phaos_DASHB", &player.frameX, &player.animTimer, 0, plr_Rectangle, sfFalse, vector2f(5.f, -12.f)));
				}
				else if (player.isRushing) {
					sfRectangleShape_setTextureRect(plr_Rectangle, animatorWithResizeAndOriginOffset("phaos_SHOULDAB", &player.frameX, &player.animTimer, 0, plr_Rectangle, sfFalse, vector2f(8.f, -2.f)));
				}
				else if (player.isPushing) {
					sfRectangleShape_setTextureRect(plr_Rectangle, animatorWithResizeAndOriginOffset("phaos_PUSHB", &player.frameX, &player.animTimer, 0, plr_Rectangle, sfFalse, vector2f(5.f, 0.f)));
				}
				else
					sfRectangleShape_setTextureRect(plr_Rectangle, animatorWithResizeAndOrigin("phaos_WALKD", &player.frameX, &player.animTimer, 0, plr_Rectangle, sfTrue, NULLVECTF));
				float angleptr = getAngleBetweenVectors(vector2f(1.f, 0.f), vector2f(-player.forward.x, -player.forward.y));
				angleptr *= RAD2DEG;
				if (player.walkType != PLR_WATER && player.walkType != PLR_GRASS)
				{
					if (player.timerWalkPart > 0.3)
					{
						prt_CreateParticlesPlayerDust(plr_GetPos(), angleptr);
						player.timerWalkPart = 0.f;
					}
				}
				else if (player.walkType == PLR_GRASS)
				{
					if (player.timerGrassParticles > 0.15)
					{
						prt_CreateGrassParticles(plr_GetPos(), angleptr);
						player.timerGrassParticles = 0.f;
					}
				}
				else if (player.walkType == PLR_WATER)
				{
					if (player.timerWaterParticles > 0.15)
					{
						prt_CreateWaterSplahParticles(plr_GetPos(), angleptr);
						player.timerWaterParticles = 0.f;
					}
				}
			}


		}
		else {

			if (player.isDashing) {
				sfRectangleShape_setTextureRect(plr_Rectangle, animatorWithResizeAndOrigin("phaos_DASHS", &player.frameX, &player.animTimer, 0, plr_Rectangle, sfTrue, NULLVECTF));
			}
			else if (player.isRushing) {
				sfRectangleShape_setTextureRect(plr_Rectangle, animatorWithResizeAndOriginOffset("phaos_SHOULDAS", &player.frameX, &player.animTimer, 0, plr_Rectangle, sfFalse, vector2f(0.f, -10.f)));
			}
			else if (player.isPushing) {
				sfRectangleShape_setTextureRect(plr_Rectangle, animatorWithResizeAndOrigin("phaos_PUSHS", &player.frameX, &player.animTimer, 0, plr_Rectangle, sfTrue, NULLVECTF));
			}
			else {
				sfRectangleShape_setTextureRect(plr_Rectangle, animatorWithResizeAndOrigin("phaos_WALKS", &player.frameX, &player.animTimer, 0, plr_Rectangle, sfTrue, NULLVECTF));
				float angleptr = getAngleBetweenVectors(vector2f(1.f, 0.f), vector2f(-player.forward.x, -player.forward.y));
				angleptr *= RAD2DEG;
				if (player.walkType != PLR_WATER && player.walkType != PLR_GRASS)
				{
					if (player.timerWalkPart > 0.3)
					{
						prt_CreateParticlesPlayerDust(plr_GetPos(), angleptr);
						player.timerWalkPart = 0.f;
					}
				}
				else if (player.walkType == PLR_GRASS)
				{
					if (player.timerGrassParticles > 0.15)
					{
						prt_CreateGrassParticles(plr_GetPos(), angleptr);
						player.timerGrassParticles = 0.f;
					}
				}
				else if (player.walkType == PLR_WATER)
				{
					if (player.timerWaterParticles > 0.15)
					{
						prt_CreateWaterSplahParticles(plr_GetPos(), angleptr);
						player.timerWaterParticles = 0.f;
					}
				}
			}
			if (player.oldForward.x > 0.f)
				sfRectangleShape_setScale(plr_Rectangle, vector2f(1.f, 1.f));
			else
				sfRectangleShape_setScale(plr_Rectangle, vector2f(-1.f, 1.f));
		}
	}

	if (player.timerAttack > 0.f || player.timerThrowRing > 0.f || player.timerCatch > 0.f)
		sfRectangleShape_setScale(plr_Rectangle, vector2f(1.f, 1.f));
	if (getState() != EDITOR)
		sfMutex_unlock(mutexThread2);

	sfRenderTexture_drawRectangleShape(_window->rdrt, plr_Rectangle, NULL);
	sfRectangleShape_setScale(plr_Rectangle, vector2f(1.f, 1.f));
	sfRectangleShape_setRotation(plr_Rectangle, 0.f);

	if (player.walkType == PLR_WATER || player.walkType == PLR_GRASS)
		sfRectangleShape_setTexture(plr_Rectangle, plr_Texture, sfTrue);

	//	sfRenderTexture_drawVertexArray(_window->rdrt, plr_vArray, NULL);
	//	sfVertexArray_clear(plr_vArray);

	if (player.flyTimer < PLR_HOOD_DURATION) {
		sfRectangleShape_setFillColor(plr_Rectangle, sfBlack);
		sfRectangleShape_setOutlineColor(plr_Rectangle, sfMagenta);
		sfRectangleShape_setOutlineThickness(plr_Rectangle, 50.f);
		sfRectangleShape_setPosition(plr_Rectangle, vector2f(900.f, 540.f));
		sfRectangleShape_setSize(plr_Rectangle, vector2f(100.f, 50.f));
		sfRenderTexture_drawRectangleShape(_window->rdrt, plr_Rectangle, NULL);


		sfRectangleShape_setFillColor(plr_Rectangle, sfWhite);
		sfRectangleShape_setOutlineThickness(plr_Rectangle, 0.f);
		sfRectangleShape_setPosition(plr_Rectangle, vector2f(900.f, 540.f));
		sfRectangleShape_setSize(plr_Rectangle, vector2f(player.flyTimer * 10.f, 50.f));
		sfRenderTexture_drawRectangleShape(_window->rdrt, plr_Rectangle, NULL);
	}

	sfRectangleShape_setFillColor(plr_Rectangle, color(255, 255, 255, 255));

	// debug
	if (slashBoundsDebug && (player.timerAttack > 0.f || player.timerCatch > 0.f)) {
		sfRectangleShape_setTexture(plr_Rectangle, NULL, sfFalse);
		sfFloatRect tmpSlashBounds = plr_GetDamageBounds();
		sfRectangleShape_setPosition(plr_Rectangle, vector2f(tmpSlashBounds.left, tmpSlashBounds.top));
		sfRectangleShape_setSize(plr_Rectangle, vector2f(tmpSlashBounds.width, tmpSlashBounds.height));
		sfRectangleShape_setOrigin(plr_Rectangle, NULLVECTF);
		sfRectangleShape_setFillColor(plr_Rectangle, color(255, 0, 0, 50));
		sfRectangleShape_setScale(plr_Rectangle, vector2f(1.f, 1.f));
		sfRenderTexture_drawRectangleShape(_window->rdrt, plr_Rectangle, NULL);
		sfRectangleShape_setFillColor(plr_Rectangle, color(255, 255, 255, 255));
	}
}

sfVector2f plr_GetFeetPos()
{
	return addVectorsrf(player.pos, vector2f(0.f, -PLR_Y_SIZE / 2.f));
}

void plr_SetPos(sfVector2f _pos)
{
	player.pos = _pos;
}

sfBool isGameOver()
{
	return isPlayerDead;
}

void deinitPlayer()
{
	if (!shaderPlayer || !plr_vArray || !plr_Rectangle) return;
	sfVertexArray_destroy(plr_vArray);
	sfShader_destroy(shaderPlayer);
	sfRectangleShape_destroy(plr_Rectangle);
}

sfVector2f plr_getPlayerVelocity()
{
	return player.velocity;
}

sfVector2f* plr_getPointerPlayerVelocity()
{
	return &player.velocity;
}

void plr_setPlayerVelocity(sfVector2f _velocity)
{
	if (player.timerDamaged <= 0.f && player.timerGetObject <= 0.f && player.hp > 0 && player.canBePushedTimer <= 0.f) {
		player.velocity = _velocity;
		player.canBePushedTimer = 1.f;
	}
}

sfFloatRect plr_getPlayerBounds()
{
	return FlRect(player.pos.x - PLR_X_ORIGIN, player.pos.y - PLR_Y_ORIGIN, PLR_X_SIZE, PLR_Y_SIZE);
}

sfFloatRect* getPointerPlayerBounds()
{
	//	sfFloatRect flRect = FlRect(player.pos.x - PLR_X_ORIGIN, player.pos.y - PLR_Y_ORIGIN, PLR_X_SIZE, PLR_Y_SIZE);
	sfFloatRect rectPlayer = FlRect(player.pos.x - 28.f, player.pos.y - 56.f, 56.f, 56.f);
	return &rectPlayer;
}

sfFloatRect* plr_getPointerAllPlayerBounds()
{
	sfFloatRect tmp = FlRect(player.pos.x - PLR_X_ORIGIN, player.pos.y - PLR_Y_ORIGIN, PLR_X_SIZE, PLR_Y_SIZE);
	return &tmp;
}

sfFloatRect plr_getPlayerBoundsWithEnemies()
{
	return FlRect(player.pos.x - 28.f, player.pos.y - 56.f, 56.f, 56.f);
}

void plr_ChargeRush()
{
	player.timerChargeRush += getDeltaTime();
	if (player.timerChargeRush > 1.f) {
		player.timerChargeRush = 1.f;
	}
}

void plr_Rush(sfVector2f _direction, sfBool* isXMoving, sfBool* isYMoving)
{
	// fait un dash dans la direction de _direction
	if (_direction.x > CONTROLLER_OFFSET) {
		*isXMoving = sfTrue;
		player.velocity.x = PLR_DASH_SPEED;
	}
	else if (_direction.x < -CONTROLLER_OFFSET) {
		*isXMoving = sfTrue;
		player.velocity.x = -PLR_DASH_SPEED;
	}
	if (*isXMoving) { // diagonal movement
		if (_direction.y > CONTROLLER_OFFSET) {
			player.velocity.y = PLR_DASH_SPEED;
		}
		else if (_direction.y < -CONTROLLER_OFFSET) {
			player.velocity.y = -PLR_DASH_SPEED;
		}
		else {
			player.velocity.y = 0.f;
		}
	}
	else if (_direction.y > CONTROLLER_OFFSET) {
		*isYMoving = sfTrue;
		player.velocity.y = PLR_DASH_SPEED;
	}
	else if (_direction.y < -CONTROLLER_OFFSET) {
		*isYMoving = sfTrue;
		player.velocity.y = -PLR_DASH_SPEED;
	}

}

sfBool plr_IsInvisible()
{
	if (player.invisibilityTimer < PLR_HOOD_DURATION)
		return sfTrue;
	return sfFalse;
}


void plr_SelectPower() {
	if (player.unlockState >= PLRUNL_ATTRACT && Gamepad_isButtonPressedNF(0, DPAD_DOWN)) player.ringPower = PLR_ATTRACT;
	else if (player.unlockState >= PLRUNL_REPULSE && Gamepad_isButtonPressedNF(0, DPAD_RIGHT)) player.ringPower = PLR_REPULSE;
	else if (player.unlockState >= PLRUNL_STASIS && Gamepad_isButtonPressedNF(0, DPAD_LEFT)) player.ringPower = PLR_STATIS;
	else if (player.unlockState >= PLRUNL_DESTRUCT && Gamepad_isButtonPressedNF(0, DPAD_UP)) player.ringPower = PLR_DESTRUCT;
}

PlrUnlockOrder plr_GetUnlockState()
{
	return player.unlockState;
}

void plr_SetUnlockState(PlrUnlockOrder _newUnlock)
{
	if (_newUnlock > player.unlockState) {
		player.unlockState = _newUnlock;
		plr_PlayGetObjectAnim();
	}
}

sfBool plr_IsRushing()
{
	return player.isRushing;
}

sfVector2f plr_GetForward()
{
	return player.forward;
}

sfVector2f plr_GetWatchDir()
{
	return player.oldForward;
}

int getCurrentPowerPlayer()
{
	return player.ringPower;
}

void plr_setPlayerIsPushed(sfBool _isPushed)
{
	if (_isPushed && player.timerDamaged <= 0.f && player.timerGetObject <= 0.f && player.hp > 0) {
		plr_stopPlayerActions();
		player.pushedTimer = PLR_MAX_PUSH_DURATION;
	}
	if (!_isPushed)
		player.pushedTimer = -10.f;

}

sfBool plr_getPlayerIsPushed()
{
	if (player.pushedTimer > 0.0f)
		return sfTrue;
	else
		return sfFalse;
}

void plr_StunPlayer(sfBool _stun)
{
	if (_stun && player.timerGetObject <= 0.f && player.timerDeath <= 0.f) {
		player.timerStunned = PLR_STUNNED_DURATION - 0.05f;
		player.frameX = 0;
		player.animTimer = 0.f;
		player.velocity = NULLVECTF;
	}
}

sfBool plr_IsDoingAction()
{
	if (player.timerAttack > 0.f || player.timerCatch > 0.f || player.timerStunned > 0.f || player.timerDamaged > 0.f || player.timerThrowRing > 0.f || player.timerGetObject > 0.f || player.timerDeath > 0.f)
		return sfTrue;

	return sfFalse;
}

sfBool plr_CanDoAction()
{
	if (plr_IsDoingAction() || player.isRushing || player.isDashing || player.isPushing)
		return sfFalse;

	return sfTrue;
}

void plr_addDamagePlayerLife(int _damage)
{
	if (player.timerDamaged < -11.f && player.timerGetObject <= 0.f && player.hp > 0) {
		player.hp -= _damage;
		player.timerDamaged = PLR_DAMAGED_DURATION - 0.05f;
		player.velocity = NULLVECTF;
		prt_CreateDamagePlayerParticles(vector2f(player.pos.x, player.pos.y - 50.f));
		shakingView(0.25f);
	}
}

sfBool plr_IsInteractingWithStatue()
{
	return sfFalse;
}

void plr_SetInteractStatue(sfBool _state)
{
	//player.interactStatue = _state;
}

sfVector2f plr_GetPos()
{
	return player.pos;
}

sfVector2f plr_GetVel()
{
	return player.velocity;
}

void plr_SetVel(sfVector2f _spd)
{
	player.velocity = _spd;
}

void plr_ForceMove(sfVector2f _spd)
{
	for (int i = 0; i < pot_List->size(pot_List); i++) {
		if (!POT_GETDATA->isOnScreen)
			continue;
		if (pot_PlayerCollision(POT_GETDATA, _spd)) return;
	}

	if (cll_RectMap(FlRect(player.pos.x - 28.f, player.pos.y - 56.f, 56.f, 56.f), _spd)) return;

	MapState tmpMapState = map_GetState();
	if (tmpMapState >= MAP_STATE_LIGHTHOUSE_BEDROOM && tmpMapState <= MAP_STATE_LIGHTHOUSE_ROOF) {
		player.isPushing = sfTrue;
		player.timerPush = 0.15f;
		if (sfSound_getStatus(GetSound("SFX_Player_BoxPushed")) == sfStopped) {
			playSoundFX("SFX_Player_BoxPushed");
		}
		if (player.frameX > 3) {
			player.frameX = 0;
			player.animTimer = 0.f;
		}
	}
	addVectorsf(&player.pos, _spd);
}

sfBool plr_IsInFOV(sfVector2f _pos) {
	return isInFOV(_pos, addVectorsrf(player.pos, vector2f(0.f, -PLR_Y_SIZE / 2.f)), player.watchDir, 70.f, 90.f);
}

void plr_StopMoving() {
	player.canMove = sfFalse;
}

int plr_GetHeart() {
	return player.hp;
}

PlrRingPowers plr_GetPower() {
	return player.ringPower;
}

int plr_GetMaxHeart() {
	return player.maxHP;
}

SlashDirection plr_GetSlashDirection()
{
	if (player.oldForward.y == 0.f) {
		if (player.oldForward.x < 0.f)
			return SLASH_LEFT;
		else
			return SLASH_RIGHT;
	}
	else {
		if (player.oldForward.y < 0.f) {
			if (player.oldForward.x < -0.1f)
				return SLASH_UP_LEFT;
			else if (player.oldForward.x > 0.1f)
				return SLASH_UP_RIGHT;
			else
				return SLASH_UP;
		}
		else {
			if (player.oldForward.x < -0.1f)
				return SLASH_BOTTOM_LEFT;
			else if (player.oldForward.x > 0.1f)
				return SLASH_BOTTOM_RIGHT;
			else
				return SLASH_DOWN;
		}
	}

	return SLASH_UP;
}

sfVector2f plr_GetSlashStartPos()
{
	if (slash.direction == SLASH_UP)
		return vector2f(player.pos.x, player.pos.y - 70.f);
	else if (slash.direction == SLASH_DOWN)
		return vector2f(player.pos.x, player.pos.y + 30.f);
	else if (slash.direction == SLASH_LEFT)
		return vector2f(player.pos.x - 70.f, player.pos.y - 40.f);
	else if (slash.direction == SLASH_RIGHT)
		return vector2f(player.pos.x + 70.f, player.pos.y - 40.f);
	else if (slash.direction == SLASH_UP_LEFT)
		return vector2f(player.pos.x - 30.f, player.pos.y - 70.f);
	else if (slash.direction == SLASH_UP_RIGHT)
		return vector2f(player.pos.x + 30.f, player.pos.y - 70.f);
	else if (slash.direction == SLASH_BOTTOM_LEFT)
		return vector2f(player.pos.x - 30.f, player.pos.y + 30.f);
	else if (slash.direction == SLASH_BOTTOM_RIGHT)
		return vector2f(player.pos.x + 30.f, player.pos.y + 30.f);

	return NULLVECTF;
}

sfVector2f plr_GetSlashStartVelocity()
{
	if (slash.direction == SLASH_UP)
		return vector2f(0.f, -PLR_SLASH_SPEED);
	else if (slash.direction == SLASH_DOWN)
		return vector2f(0.f, PLR_SLASH_SPEED);
	else if (slash.direction == SLASH_LEFT)
		return vector2f(-PLR_SLASH_SPEED, 0.f);
	else if (slash.direction == SLASH_RIGHT)
		return vector2f(PLR_SLASH_SPEED, 0.f);
	if (slash.direction == SLASH_UP_LEFT)
		return polarCoord(NULLVECTF, PLR_SLASH_SPEED, 225.f * DEG2RAD);
	else if (slash.direction == SLASH_UP_RIGHT)
		return polarCoord(NULLVECTF, PLR_SLASH_SPEED, 315.f * DEG2RAD);
	else if (slash.direction == SLASH_BOTTOM_LEFT)
		return polarCoord(NULLVECTF, PLR_SLASH_SPEED, 135.f * DEG2RAD);
	else if (slash.direction == SLASH_BOTTOM_RIGHT)
		return polarCoord(NULLVECTF, PLR_SLASH_SPEED, 45.f * DEG2RAD);

	return NULLVECTF;
}

sfFloatRect plr_GetDamageBounds()
{
	if (slash.lifeTime > 0.f) {
		if (slash.direction == SLASH_UP || slash.direction == SLASH_DOWN)
			return FlRect(slash.pos.x - 33.f, slash.pos.y - 19.f, 66.f, 38.f);
		else if (slash.direction == SLASH_LEFT || slash.direction == SLASH_RIGHT)
			return FlRect(slash.pos.x - 19.f, slash.pos.y - 33.f, 38.f, 66.f);
		else
			return FlRect(slash.pos.x - 30.f, slash.pos.y - 30.f, 60.f, 60.f);
	}
	else if ((player.timerAttack > 0.4f || player.timerCatch > 0.4f) && player.frameX == 2) {
		return FlRect(player.pos.x - 30.f, player.pos.y - 60.f, 60.f, 60.f);
	}

	return FlRect(0.f, 0.f, 0.f, 0.f);
}

sfBool plr_IsSlashDamaging()
{
	if (slash.state == SLASH_ATTACK)
		return sfTrue;

	return sfFalse;
}

void plr_DisplaySlash(Window* _window)
{
	// slash
	if (slash.lifeTime > 0.f) {
		sfRectangleShape_setTexture(plr_Rectangle, plr_Texture, sfFalse);
		sfRectangleShape_setPosition(plr_Rectangle, slash.pos);
		sfRectangleShape_setOrigin(plr_Rectangle, vector2f(19.f, 33.f));

		if (slash.state == SLASH_ATTACK || slash.state == SLASH_CATCH) {
			sfRectangleShape_setRotation(plr_Rectangle, 0.f);
			sfRectangleShape_setTextureRect(plr_Rectangle, animatorWithResize("phaos_SLASHATTACK", &slash.frameX, &slash.animTimer, 0, plr_Rectangle));
		}
		// currently impossible
		//else if (slash.state == SLASH_ATTRACT) {
		//	sfRectangleShape_setRotation(plr_Rectangle, 0.f);
		//	sfRectangleShape_setTextureRect(plr_Rectangle, animatorWithResize("phaos_SLASHATTRACT", &slash.frameX, &slash.animTimer, 0, plr_Rectangle));
		//}
		//else if (slash.state == SLASH_REPULSE) {
		//	sfRectangleShape_setRotation(plr_Rectangle, 0.f);
		//	sfRectangleShape_setTextureRect(plr_Rectangle, animatorWithResize("phaos_SLASHREPULSE", &slash.frameX, &slash.animTimer, 0, plr_Rectangle));
		//}

		if (slash.direction == SLASH_UP) {
			sfRectangleShape_setRotation(plr_Rectangle, 90.f);
			sfRectangleShape_setScale(plr_Rectangle, vector2f(1.f, -1.f));
		}
		else if (slash.direction == SLASH_DOWN) {
			sfRectangleShape_setRotation(plr_Rectangle, 270.f);
			sfRectangleShape_setScale(plr_Rectangle, vector2f(1.f, -1.f));
		}
		else if (slash.direction == SLASH_LEFT) {
			sfRectangleShape_setRotation(plr_Rectangle, 0.f);
			sfRectangleShape_setScale(plr_Rectangle, vector2f(1.f, -1.f));
		}
		else if (slash.direction == SLASH_RIGHT) {
			sfRectangleShape_setRotation(plr_Rectangle, 0.f);
			sfRectangleShape_setScale(plr_Rectangle, vector2f(-1.f, 1.f));
		}
		else if (slash.direction == SLASH_UP_LEFT) {
			sfRectangleShape_setRotation(plr_Rectangle, 45.f);
			sfRectangleShape_setScale(plr_Rectangle, vector2f(1.f, -1.f));
		}
		else if (slash.direction == SLASH_UP_RIGHT) {
			sfRectangleShape_setRotation(plr_Rectangle, 135.f);
			sfRectangleShape_setScale(plr_Rectangle, vector2f(1.f, -1.f));
		}
		else if (slash.direction == SLASH_BOTTOM_LEFT) {
			sfRectangleShape_setRotation(plr_Rectangle, 315.f);
			sfRectangleShape_setScale(plr_Rectangle, vector2f(1.f, -1.f));
		}
		else if (slash.direction == SLASH_BOTTOM_RIGHT) {
			sfRectangleShape_setRotation(plr_Rectangle, 225.f);
			sfRectangleShape_setScale(plr_Rectangle, vector2f(1.f, -1.f));
		}

		sfRenderTexture_drawRectangleShape(_window->rdrt, plr_Rectangle, NULL);

		sfRectangleShape_setRotation(plr_Rectangle, 0.f);
		sfRectangleShape_setScale(plr_Rectangle, vector2f(1.f, 1.f));
	}
}

void plr_DisplayHood(Window* _window)
{
	//sfX Player  
	if (bool_fxPlayer) { 
		sfRectangleShape_setPosition(fx_player, vector2f(player.pos.x, player.pos.y - 155.f)); 
		sfRectangleShape_setSize(fx_player, vector2f(64.f, 64.f)); 
		sfRectangleShape_setOrigin(fx_player, vector2f(64.f / 2, 64.f / 2)); 
		sfRectangleShape_setTextureRect(fx_player, animatorAnim("feedback_exclam_player", &frameXFX, &timerAnimFx, 0)); 
		sfRenderTexture_drawRectangleShape(_window->rdrt, fx_player, NULL); 
	}
	// pig hat
	if (player.pigHatTimer < PLR_HOOD_DURATION) {
		sfRectangleShape_setTexture(plr_Rectangle, plr_Texture, sfFalse);
		sfRectangleShape_setRotation(plr_Rectangle, 0.f);
		sfRectangleShape_setScale(plr_Rectangle, vector2f(1.f, 1.f));

		sfRectangleShape_setTextureRect(plr_Rectangle, animatorWithResizeAndOrigin("phaos_PIGHAT", &player.frameXPig, &player.animPigTimer, 0, plr_Rectangle, sfTrue, NULLVECTF));

		float sqrDistToTarget = getSqrMagnitudeBetween2Vectorsf(player.pos, plr_TargetPos);
		if (sqrDistToTarget > 12000.f * 12000.f)
			sqrDistToTarget = 12000.f * 12000.f;
		float t = 1 - (sqrDistToTarget / (12000.f * 12000.f));

		float timeScaleBonus = lerp(0, 20, t);
		player.animPigTimer += timeScaleBonus * getDeltaTime();

		if (player.velocity.y != 0.f) {
			sfRectangleShape_setPosition(plr_Rectangle, addVectorsrf(player.pos, vector2f(0.f, -60.f)));
		}
		else {
			if (player.velocity.x == 0.f)
				sfRectangleShape_setPosition(plr_Rectangle, addVectorsrf(player.pos, vector2f(0.f, -60.f)));
			else
				sfRectangleShape_setPosition(plr_Rectangle, addVectorsrf(player.pos, vector2f(0.f, -60.f)));
		}
		sfRenderTexture_drawRectangleShape(_window->rdrt, plr_Rectangle, NULL);
	}
}

void plr_DisplayEnduranceBar(Window* _window)
{
	// endurance bar
	if (player.pigHatTimer >= PLR_HOOD_DURATION && player.pigHatTimer < PLR_HOOD_DURATION * 2.f) {

		sfRectangleShape_setTexture(plr_Rectangle, plr_Texture, sfFalse);
		sfRectangleShape_setTextureRect(plr_Rectangle, IntRect(0, 6370, 104, 24));
		sfRectangleShape_setOrigin(plr_Rectangle, vector2f(0.f, 0.f));
		sfRectangleShape_setScale(plr_Rectangle, vector2f(1.f, 1.f));
		if (player.oldForward.y != 0.f) {
			sfRectangleShape_setPosition(plr_Rectangle, addVectorsrf(player.pos, vector2f(-52.f, 5.f)));
		}
		else
			sfRectangleShape_setPosition(plr_Rectangle, addVectorsrf(player.pos, vector2f(fabsf(player.oldForward.x) * -52.f, 5.f)));
		sfRectangleShape_setSize(plr_Rectangle, vector2f(104.f, 24.f));
		sfRenderTexture_drawRectangleShape(_window->rdrt, plr_Rectangle, NULL);


		sfRectangleShape_setOrigin(plr_Rectangle, vector2f(0.f, 0.f));
		sfRectangleShape_setScale(plr_Rectangle, vector2f(1.f, 1.f));
		float size = lerp(0, 88, (player.pigHatTimer - PLR_HOOD_DURATION) / PLR_HOOD_DURATION);

		sfRectangleShape_setTextureRect(plr_Rectangle, IntRect(0, 6394, size, 8));
		sfRectangleShape_setSize(plr_Rectangle, vector2f(size, 8.f));
		if (player.oldForward.y != 0.f) {
			sfRectangleShape_setPosition(plr_Rectangle, addVectorsrf(player.pos, vector2f(-44.f, 13.f)));
		}
		else {
			if (player.oldForward.x < 0.f)
				sfRectangleShape_setPosition(plr_Rectangle, addVectorsrf(player.pos, vector2f(-44.f, 13.f)));
			else
				sfRectangleShape_setPosition(plr_Rectangle, addVectorsrf(player.pos, vector2f(-44.f, 13.f)));

		}

		sfRenderTexture_drawRectangleShape(_window->rdrt, plr_Rectangle, NULL);
	}
}

void plr_DisplayShadow(Window* _window)
{
	if (player.walkType == PLR_GROUND) {
		sfRectangleShape_setTexture(plr_Rectangle, plr_ShadowTexture, sfFalse);
		sfRectangleShape_setTextureRect(plr_Rectangle, IntRect(0, 0, 102, 36));
		sfRectangleShape_setSize(plr_Rectangle, vector2f(102.f, 36.f));
		sfRectangleShape_setOrigin(plr_Rectangle, vector2f(51.f, 17.f));
		if ((player.timerDeath > 0.f && player.frameX >= 7) || player.timerGameOver > 0.f)
			sfRectangleShape_setPosition(plr_Rectangle, vector2f(player.pos.x + 30.f, player.pos.y));
		else if ((player.timerAttack > 0.f || player.timerThrowRing > 0.f || player.timerCatch > 0.f) && player.frameX >= 2 && player.frameX <= 6 && player.oldForward.y == 0.f && player.timerDeath <= 0.f)
			sfRectangleShape_setPosition(plr_Rectangle, vector2f(player.pos.x + (player.oldForward.x > 0.f ? 18.f : -18.f), player.pos.y));
		else
			sfRectangleShape_setPosition(plr_Rectangle, vector2f(player.pos.x, player.pos.y));
		sfRectangleShape_setScale(plr_Rectangle, vector2f(1.f, 1.f));
		sfRenderTexture_drawRectangleShape(_window->rdrt, plr_Rectangle, NULL);
	}
}

sfBool plr_GetCanMove() {
	return player.canMove;
}

void plr_SetCanMove(sfBool _canMove) {
	player.canMove = _canMove;
}

void plr_SetAllTimer(float _time) {
	player.timerAttack = _time;
	player.timerCatch = _time;
	player.timerThrowRing = _time;
	player.timerStunned = _time;
	player.timerDamaged = _time;
	player.timerGetObject = _time;
	player.timerDeath = _time;
}

void plr_SetHoodTargetPos(sfVector2f _pos) { plr_TargetPos = _pos; }
sfVector2f plr_GetHoodTargetPos() { return plr_TargetPos; }

void plr_stopPlayerActions()
{
	player.timerAttack = -10.f;
	player.timerCatch = -10.f;
	player.timerButtonPressed = 0.f;
	player.timerChargeRush = 0.f;
	if (player.isRushing) {
		if (sfSound_getStatus(GetSound("SFX_Player_Run")) == sfPlaying)
			sfSound_stop(GetSound("SFX_Player_Run"));
	}
	player.isRushing = sfFalse;
	player.isDashing = sfFalse;
	
	player.timerGetObject = -10.f;
	player.timerDamaged = -10.f;
	player.timerStunned = -10.f;
	player.timerThrowRing = -10.f;
	ring_Detach();
}

void plr_resetPlayer()
{
	plr_stopPlayerActions();
	player.velocity = NULLVECTF;
	player.frameX = 0;
	player.animTimer = 0.f;
	slash.lifeTime = -10.f;
	slash.state = SLASH_NO_STATE;
	player.oldPos = player.pos;
	player.walkType = PLR_GROUND;
	ring_Reset();
}

void plr_AddLife(int _life)
{
	player.hp += _life;
	if (player.hp > player.maxHP)
		player.hp = player.maxHP;
	prt_CreateHealParticles(vector2f(player.pos.x, player.pos.y - 50.f));
}

void plr_SetWalkType(PlrWalkType _type) { player.walkType = _type; }

void plr_toggleFX(sfBool _boolToggle)
{ 
	bool_fxPlayer = _boolToggle;  
}
void plr_UpdateGameOver(Window* _window)
{
	player.timerGameOver -= getDeltaTimeThread2();
	if (player.timerGameOver <= 0.f && player.timerGameOver > -5.f) {
		player.timerGameOver = -10.f;
		player.frameX = 0;
		player.animTimer = 0.f;
		player.hp = player.maxHP;
		player.pos = map_getPlayerSpawnPos();
		player.forward = vector2f(0.f, 1.f);
		player.oldForward = vector2f(0.f, 1.f);
		plr_resetPlayer();
		mapReset(_window);
	}
}

void plr_PlayGetObjectAnim()
{
	player.timerGetObject = PLR_GET_OBJECT_DURATION + PLR_HOLD_OBJECT_DURATION - 0.05f;
	player.frameX = 0;
	player.animTimer = 0.f;
	player.hasObtainedNewItem = sfTrue;
	playSoundFX("SFX_pick_item");
}

