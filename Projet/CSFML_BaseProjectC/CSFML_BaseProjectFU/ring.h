#pragma once
#include "tools.h"
#include "windowManager.h"
#include "player.h"

typedef enum {
	RING_ATCH_NOTHING,
	RING_ATCH_BOX,
	RING_ATCH_ENEMY,
	RING_ATCH_MVPL,
	RING_ATCH_EXPT,
	RING_ATCH_BOSS
}RingAttachment;

typedef struct RingData RingData;

struct RingData {
	sfVector2f pos;
	sfVector2f velocity;
	sfBool isThrown;
	float timer;
	sfVector2f startPos;
	int isAttached;
	float returnTimer;
	RingAttachment attachment;
	sfBool canStunPlayer;
	PlrRingPowers power;
	int frameX;
	float animTimer;
	float particlesTimer;
};

/// Initializes the ring.
void ring_Init();

///Updates the ring.
void ring_Update(Window* _window);

///Renders the ring.
void ring_Display(Window* _window);

/// Throws the ring in the direction of the thrower's forward.
/// \param _fwd - The forward vector relatively to which the ring will be propelled
/// \param _power - The power used
void ring_Throw(sfVector2f _fwd, PlrRingPowers _power);

/// \return Whether the ring can be thrown right now
sfBool ring_CanThrow();

/// \return Ring's current position
sfVector2f ring_GetPos();

/// \return Ring's bounds, only when could attach
sfFloatRect ring_GetBounds();

/// \return Ring's power
PlrRingPowers ring_GetRingPower();

/// \return Whether the ring can attach to something
sfBool ring_CanAttach();

/// Forces ring attachment, only works if the ring isn't attached already
/// \param _attachment - The appropriate attachment
void ring_SetAttachment(RingAttachment _attachment);

/// Forces ring position
/// \param _pos - The new ring's position
void ring_SetPos(sfVector2f _pos);

/// Forces ring to detach
void ring_Detach();

/// Forces ring to instant comeback to the player
void ring_Reset();
