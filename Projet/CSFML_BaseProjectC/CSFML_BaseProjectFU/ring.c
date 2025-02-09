#include "ring.h"
#include "math.h"
#include "box.h"
#include "controller.h"
#include "enemies.h"
#include "moving_platform.h"
#include "expandingTile.h"
#include "map.h"
#include "particlesManager.h"
#include "soundManager.h"

#define RING_X_SIZE 32
#define RING_Y_SIZE 32
#define RING_X_ORIGIN RING_X_SIZE / 2
#define RING_Y_ORIGIN RING_Y_SIZE / 2

#define RING_BASE_SPEED 500.f
#define RING_LIFETIME 2.f

RingData ring;
sfRectangleShape* ring_RenderRect;
sfTexture* ring_SparksTexture;

void (*ring_box_functions[4])(BoxData*) = { box_Attract, box_Repulse, box_Stasis, box_Destroy }; /// Power functions for box entities
void (*ring_enemies_functions[4])(EnemiesStruct*) = { en_Attract, en_Repulse, en_Stasis, en_Destroy }; /// Power functions for enemy entities
void (*ring_mvpl_functions[4])(mvplData*) = { mvpl_Attract, mvpl_Repulse, mvpl_Stasis, mvpl_Destroy }; /// Power functions for moving platform entities
void (*ring_expt_functions[4])(exptData*) = { expt_Attract, expt_Repulse, expt_Stasis, expt_Destroy }; /// Power functions for expandable tile entities

void ring_Init()
{
	ring_RenderRect = sfRectangleShape_create();
	sfRectangleShape_setTexture(ring_RenderRect, GetSfTexture("ringAttract"), sfFalse);
	sfRectangleShape_setPosition(ring_RenderRect, vector2f(960.f, 540.f));
	sfRectangleShape_setOrigin(ring_RenderRect, vector2f(RING_X_ORIGIN, RING_Y_ORIGIN));
	sfRectangleShape_setSize(ring_RenderRect, vector2f(RING_X_SIZE, RING_Y_SIZE));

	ring_SparksTexture = GetSfTexture("sparksAttract");

	ring.pos = vector2f(0.f, 0.f);
	ring.velocity = vector2f(0.f, 0.f);
	ring.isThrown = sfFalse;
	ring.isAttached = 0;
	ring.attachment = RING_ATCH_NOTHING;
	ring.returnTimer = 0.f;
	ring.canStunPlayer = sfFalse;
	ring.power = PLR_NO_POWER;
	ring.frameX = 0;
	ring.animTimer = 0.f;
	ring.particlesTimer = 0.f;

}

void ring_Update(Window* _window)
{
	float dt = getDeltaTime();

	/// Computes a few updates based on what the ring is currently latching onto
	if (ring.isAttached == 1) {
		BoxData* closestBox = NULL;
		EnemiesStruct* closestEnemy = NULL;
		mvplData* closestPlatform = NULL;
		exptData* closestExpt = NULL;
		/// Updates the ring's position relatively to what it has latched onto
		if (ring.attachment == RING_ATCH_BOX) {
			closestBox = getClosestBox(ring.pos);
			if (!closestBox)
				ring_Detach();
			else
				ring.pos = addVectorsrf(closestBox->pos, vector2f(BOX_X_SIZE / 2.f, BOX_Y_SIZE / 4.f));
		}
		else if (ring.attachment == RING_ATCH_ENEMY) {
			closestEnemy = en_getClosestEnemy(ring.pos);
			if (!closestEnemy)
				ring_Detach();
			else
				ring.pos = closestEnemy->pos;
		}
		else if (ring.attachment == RING_ATCH_MVPL) {
			closestPlatform = mvpl_GetClosestPlatform(ring.pos);
			if (!closestPlatform)
				ring_Detach();
			else
				ring.pos = addVectorsrf(closestPlatform->pos, vector2f(TILE_SIZE / 2.f, TILE_SIZE / 2.f));
		}
		else if (ring.attachment == RING_ATCH_EXPT) {
			closestExpt = expt_GetClosest(ring.pos);
			if (!closestExpt)
				ring_Detach();
			else
				ring.pos = addVectorsrf(closestExpt->posEndpoint, vector2f(TILE_SIZE / 2.f, TILE_SIZE / 2.f));
		}

		/// A few special functions to be called whenever the ring is latching onto boxes or enemies
		ring.startPos = ring.pos;
		ring.returnTimer += dt;
		if (ring.returnTimer > 5.f) {
			if (ring.attachment == RING_ATCH_BOX)
				box_AttachRing(sfFalse, closestBox);
			else if (ring.attachment == RING_ATCH_ENEMY)
				en_AttachRing(sfFalse, closestEnemy);
			ring.isAttached = 2;
			ring.canStunPlayer = sfTrue;
		}

		/// Checks if the ring is being released, calling various functions upon what it was latched onto
		if (Gamepad_getTriggerPos(0, sfFalse, _window->renderWindow)) {
			int plrPowTmp = getCurrentPowerPlayer();
			sfBool resetEnemy = sfTrue;
			if (plrPowTmp > -1) {
				if (ring.attachment == RING_ATCH_BOX) {
					box_AttachRing(sfFalse, closestBox);
					ring_box_functions[plrPowTmp](closestBox);
				}
				else if (ring.attachment == RING_ATCH_ENEMY) {
					ring_enemies_functions[plrPowTmp](closestEnemy);
					resetEnemy = sfFalse;
				}
				else if (ring.attachment == RING_ATCH_MVPL) {
					ring_mvpl_functions[plrPowTmp](closestPlatform);
				}
				else if (ring.attachment == RING_ATCH_EXPT) {
					ring_expt_functions[plrPowTmp](closestExpt);
				}
			}
			if (ring.attachment == RING_ATCH_ENEMY && resetEnemy)
				en_AttachRing(sfFalse, closestEnemy);

			ring.isAttached = 2;
		}
	}
	/// If the ring is currently traveling through the air, but isn't latching onto anything just yet
	else if (ring.isThrown) {
		ring.timer += dt;

		/// To
		if (ring.timer < RING_LIFETIME / 2.f) {
			addVectorsf(&ring.pos, multiplyVectorsrf(ring.velocity, dt * (1.f - ring.timer)));
			ring.startPos = ring.pos;
		}
		/// Fro
		else {
			ring.pos = vec2f_lerp(ring.startPos, plr_GetFeetPos(), ring.timer - 1.f);
			ring.velocity = multiplyVectorsrf(normalizef(createVectorWithVectorf(ring.pos, plr_GetFeetPos())), RING_BASE_SPEED);
			if (ring.timer >= RING_LIFETIME) {
				ring.isThrown = sfFalse;
				if (ring.canStunPlayer)
					plr_StunPlayer(sfTrue);
			}
		}

		/// Checks if anything is in the ring's path, and latches onto whatever is found
		if (ring.isAttached != 2) {
			sfFloatRect ringBounds = ring_GetBounds();

			BoxData* closestBox = getClosestBox(ring.pos);
			sfFloatRect boxBounds = box_GetBounds(closestBox);
			if (sfFloatRect_intersects(&ringBounds, &boxBounds, NULL)) {
				ring.attachment = RING_ATCH_BOX;
				ring.isAttached = 1;
				box_AttachRing(sfTrue, closestBox);
				ring.timer = RING_LIFETIME / 2.f;
			}

			EnemiesStruct* closestEnemy = en_getClosestEnemy(ring.pos);
			sfFloatRect enemyBounds = en_getBounds(closestEnemy);
			if (sfFloatRect_intersects(&ringBounds, &enemyBounds, NULL)) {
				ring.attachment = RING_ATCH_ENEMY;
				ring.isAttached = 1;
				en_AttachRing(sfTrue, closestEnemy);
				ring.timer = RING_LIFETIME / 2.f;
			}

			mvplData* closestPlatform = mvpl_GetClosestPlatform(ring.pos);
			sfFloatRect mvplBounds = mvpl_GetBounds(closestPlatform);
			if (sfFloatRect_intersects(&ringBounds, &mvplBounds, NULL)) {
				ring.attachment = RING_ATCH_MVPL;
				ring.isAttached = 1;
				ring.timer = RING_LIFETIME / 2.f;
			}

			exptData* closestExpt = expt_GetClosest(ring.pos);
			sfFloatRect exptBounds = expt_GetEndpointBounds(closestExpt);
			if (sfFloatRect_intersects(&ringBounds, &exptBounds, NULL)) {
				ring.attachment = RING_ATCH_EXPT;
				ring.isAttached = 1;
				ring.timer = RING_LIFETIME / 2.f;
			}
		}
	}

	if (ring.isThrown && ring.isAttached != 2) {
		ring.particlesTimer += dt;
		if (ring.particlesTimer > 0.05f) {
			float reverseAngle = getAngleBetweenVectors(vector2f(1.f, 0.f), vector2f(-ring.velocity.x, -ring.velocity.y)) * RAD2DEG;
			float offsetAngle = ring.isAttached == 1 ? 180.f : 45.f;
			float speed = ring.isAttached == 1 ? 50.f : 200.f;
			char* particleName = ring.power == PLR_REPULSE ? "sparksRepulse" : "sparksAttract";
			if (ring.power == PLR_REPULSE)
				createAnimatedParticles(ring.pos, 1, sfGreen, sfCyan, 0.8f, ring_SparksTexture, 1.f, 1.f, reverseAngle - offsetAngle, reverseAngle + offsetAngle, speed, rand_float(-100.f, 100.f), sfFalse, sfTrue, sfFalse, particleName, sfTrue, sfFalse);
			if (ring.power != PLR_REPULSE)
				createAnimatedParticles(ring.pos, 1, sfYellow, sfRed, 0.8f, ring_SparksTexture, 1.f, 1.f, reverseAngle - offsetAngle, reverseAngle + offsetAngle, speed, rand_float(-100.f, 100.f), sfFalse, sfTrue, sfFalse, particleName, sfTrue, sfFalse);
			ring.particlesTimer = 0.f;
		}
	}
}


void ring_Display(Window* _window)
{
	if (ring.isThrown) {
		sfRectangleShape_setPosition(ring_RenderRect, ring.pos);

		if (ring.isAttached != 2) {
			sfRectangleShape_setSize(ring_RenderRect, vector2f(65.f, 62.f));
			sfRectangleShape_setOrigin(ring_RenderRect, vector2f(32.f, 31.f));
			if (ring.power == PLR_ATTRACT)
				sfRectangleShape_setTextureRect(ring_RenderRect, animatorAnim("ringAttract", &ring.frameX, &ring.animTimer, 0));
			else if (ring.power == PLR_REPULSE)
				sfRectangleShape_setTextureRect(ring_RenderRect, animatorAnim("ringRepulse", &ring.frameX, &ring.animTimer, 0));
			else if (ring.power == PLR_STATIS)
				sfRectangleShape_setTextureRect(ring_RenderRect, animatorAnim("ringStasis", &ring.frameX, &ring.animTimer, 0));
			else if (ring.power == PLR_DESTRUCT)
				sfRectangleShape_setTextureRect(ring_RenderRect, animatorAnim("ringDestruct", &ring.frameX, &ring.animTimer, 0));
			sfRenderTexture_drawRectangleShape(_window->rdrt, ring_RenderRect, NULL);
		}

		sfRectangleShape_setSize(ring_RenderRect, vector2f(RING_X_SIZE, RING_Y_SIZE));
		sfRectangleShape_setOrigin(ring_RenderRect, vector2f(RING_X_ORIGIN, RING_Y_ORIGIN));
		if (ring.power == PLR_ATTRACT)
			sfRectangleShape_setTextureRect(ring_RenderRect, IntRect(0, 0, RING_X_SIZE, RING_Y_SIZE));
		else if (ring.power == PLR_REPULSE)
			sfRectangleShape_setTextureRect(ring_RenderRect, IntRect(RING_X_SIZE, 0, RING_X_SIZE, RING_Y_SIZE));
		else if (ring.power == PLR_STATIS)
			sfRectangleShape_setTextureRect(ring_RenderRect, IntRect(RING_X_SIZE * 2, 0, RING_X_SIZE, RING_Y_SIZE));
		else if (ring.power == PLR_DESTRUCT)
			sfRectangleShape_setTextureRect(ring_RenderRect, IntRect(RING_X_SIZE * 3, 0, RING_X_SIZE, RING_Y_SIZE));
		sfRenderTexture_drawRectangleShape(_window->rdrt, ring_RenderRect, NULL);
	}
}

void ring_Deinit() {
	if (!ring_RenderRect) return;
	sfRectangleShape_destroy(ring_RenderRect);
}

void ring_Throw(sfVector2f _fwd, PlrRingPowers _power)
{
	ring.isThrown = sfTrue;
	playSoundFX("SFX_Player_AttackMagic");
	ring.pos = plr_GetSlashStartPos();
	ring.velocity = multiplyVectorsrf(_fwd, RING_BASE_SPEED);
	ring.timer = 0.f;
	ring.startPos = ring.pos;
	ring.isAttached = 0;
	ring.returnTimer = 0.f;
	ring.canStunPlayer = sfFalse;
	ring.power = _power;
	ring.frameX = 0;
	ring.animTimer = 0.f;
	ring.particlesTimer = 0.f;
}

sfBool ring_CanThrow()
{
	return !ring.isThrown;
}

sfVector2f ring_GetPos()
{
	return ring.pos;
}

sfFloatRect ring_GetBounds()
{
	sfFloatRect bounds = FlRect(0.f, 0.f, 0.f, 0.f);

	if (ring_CanAttach())
		bounds = FlRect(ring.pos.x - RING_X_ORIGIN, ring.pos.y - RING_Y_ORIGIN, RING_X_SIZE, RING_Y_SIZE);

	return bounds;
}

PlrRingPowers ring_GetRingPower()
{
	return ring.power;
}

sfBool ring_CanAttach()
{
	if (ring.isThrown && ring.isAttached == 0)
		return sfTrue;

	return sfFalse;
}

void ring_SetAttachment(RingAttachment _attachment)
{
	if (ring_CanAttach() && ring.attachment == RING_ATCH_NOTHING)
		ring.attachment = _attachment;
}

void ring_SetPos(sfVector2f _pos)
{
	ring.pos = _pos;
}

void ring_Detach()
{
	if (ring.isAttached == 1) {
		ring.isAttached = 2;
		ring.startPos = ring.pos;
	}
}

void ring_Reset()
{
	if (ring.isThrown) {
		ring.canStunPlayer = sfFalse;
		ring.isAttached = 0;
		ring.attachment = RING_ATCH_NOTHING;
		ring.isThrown = sfFalse;
	}
}
