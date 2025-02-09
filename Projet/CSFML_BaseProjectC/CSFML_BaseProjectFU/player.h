#pragma once
#include "windowManager.h"
#include "tools.h"

#define PLR_X_SIZE 68
#define PLR_Y_SIZE 116

#define PLR_X_ORIGIN PLR_X_SIZE / 2
#define PLR_Y_ORIGIN PLR_Y_SIZE

#define PLR_X_SIZE2 54

#define PLR_X_ORIGIN2 PLR_X_SIZE2 / 2

#define PLR_X_SIZE3 78

#define PLR_X_ORIGIN3 PLR_X_SIZE2 / 2

#define PLR_SPEED 320.f
#define PLR_SHOULDER_SPEED 440.f
#define PLR_DASH_SPEED 700.f

#define PLR_HOOD_DURATION 10.f

#define PLR_MAX_PUSH_DURATION 3.f
#define PLR_ATTACK_DURATION 0.8f
#define PLR_CATCH_DURATION 0.8f
#define PLR_RING_THROW_DURATION 0.8f
#define PLR_STUNNED_DURATION 0.6f
#define PLR_DAMAGED_DURATION 0.5f
#define PLR_GET_OBJECT_DURATION 1.5f
#define PLR_HOLD_OBJECT_DURATION 3.f
#define PLR_DEATH_DURATION 3.4f
#define PLR_DEATH_NO_GROUND_DURATION 3.0f
#define PLR_FALL_DURATION 2.f;
#define PLR_STAR_FALL_DURATION 2.f;
#define PLR_STAR_FALL_OFFSET 2.f;
#define PLR_GAME_OVER_DURATION 5.5f;

#define PLR_SAVE_OLDPOS_DURATION 3.f

#define PLR_SLASH_LIFETIME 0.45f
#define PLR_SLASH_SPEED 300.f

typedef enum {
	LEFT,
	RIGHT
}Direction;

typedef enum {
	PLR_NO_POWER = -1,
	PLR_ATTRACT,
	PLR_REPULSE,
	PLR_STATIS,
	PLR_DESTRUCT
}PlrRingPowers;

typedef enum {
	PLRUNL_NONE,
	PLRUNL_STAFF,
	PLRUNL_RING_CAPTURE,
	PLRUNL_SHOULDER_CHARGE,
	PLRUNL_PIG_HAT,
	PLRUNL_ATTRACT,
	PLRUNL_POLPO_HAT,
	PLRUNL_REPULSE,
	PLRUNL_EAGLE_HAT,
	PLRUNL_STASIS,
	PLRUNL_INVISIBILITY_HAT,
	PLRUNL_DESTRUCT
}PlrUnlockOrder;

typedef enum {
	PLR_BOX_SEARCH,
	PLR_SPIDER_SEARCH,
	PLR_CRAB_SEARCH,
	PLR_GOOSE_SEARCH,
	PLR_SANKE_SEARCH,
	PLR_MOSKITO_SEARCH,
	PLR_MOLE_SEARCH,
	PLR_SCOLOPENDRE_SEARCH,
	PLR_CHAMELON_SEARCH,
	PLR_BAT_SEARCH,
	PLR_GOAT_SEARCH
}PlrSearchedObject;

typedef enum {
	PLR_VOID,
	PLR_GROUND,
	PLR_GRASS,
	PLR_WATER
}PlrWalkType;

typedef struct {
	float hp;
	float maxHP;
	sfVector2f velocity;
	sfVector2f oldVelocity;
	float animTimer;
	float animPigTimer;
	int frameX;
	int frameXPig;
	sfIntRect animRect;
	sfVector2f pos;
	sfVector2f forward;
	sfVector2f oldForward;
	float timerChargeRush;
	float timerButtonPressed;
	
	float pigHatTimer;
	float invisibilityTimer;
	float flyTimer;
	PlrUnlockOrder unlockState;
	
	PlrRingPowers ringPower;
	sfBool interactStatue;
	sfVector2f watchDir;
	sfBool canMove;
	float pushedTimer;
	float canBePushedTimer;

	sfBool rushIsCharging;
	sfBool isRushing;
	sfBool isFlying;
	sfBool isDashing;
	sfBool isPushing;
	sfBool hasObtainedNewItem;
	float timerPush;

	float timerAttack;
	float timerCatch;
	float timerThrowRing;
	float timerStunned;
	float timerDamaged;
	float timerGetObject;
	float timerDeath;
	float timerWalkAnim;
	float timerWalkPart;
	float timerGrassParticles;
	float timerWaterParticles;
	float timerFall;
	PlrWalkType walkType;

	sfVector2f fallOffsetPos;
	sfVector2f oldPos;
	float timerSaveOldPos;
	float timerGameOver;
}Player;

typedef enum {
	SLASH_NO_STATE,
	SLASH_ATTACK,
	SLASH_CATCH,
	SLASH_ATTRACT,
	SLASH_REPULSE
}SlashState;

typedef enum {
	SLASH_UP,
	SLASH_DOWN,
	SLASH_LEFT,
	SLASH_RIGHT,
	SLASH_UP_LEFT,
	SLASH_UP_RIGHT,
	SLASH_BOTTOM_LEFT,
	SLASH_BOTTOM_RIGHT
}SlashDirection;

typedef struct {
	SlashDirection direction;
	sfVector2f pos;
	sfVector2f velocity;
	int frameX;
	float animTimer;
	float lifeTime;
	SlashState state;
}Slash;

void plr_loadTexture();
/// <summary>
/// init the player
/// </summary>
void plr_Init();

/// <summary>
/// update the player
/// </summary>
/// <param name="_window">the current window</param>
void plr_UpdateInteractions(Window* _window);
void plr_UpdateMovement(Window* _window);

/// <summary>
/// display the player
/// </summary>
/// <param name="_window">the current window</param>
/// <param name="spr">the sprite of the game state</param>
void plr_Display(Window* _window, sfSprite* spr);

/// <summary>
/// get the player pos
/// </summary>
/// <returns>the player pos</returns>
sfVector2f plr_GetFeetPos();

/// <summary>
/// Set the player pos
/// </summary>
/// <param name="_pos">Position</param>
void plr_SetPos(sfVector2f _pos);

/// <summary>
/// check if the player is dead
/// </summary>
/// <returns>true if he is dead false if otherwise</returns>
sfBool isGameOver();

/// <summary>
/// deinit the player
/// </summary>
void deinitPlayer();

/// \return Player's current movement speed
sfVector2f plr_getPlayerVelocity();
sfVector2f* plr_getPointerPlayerVelocity();
void plr_setPlayerVelocity(sfVector2f _velocity);

sfFloatRect plr_getPlayerBounds();
sfFloatRect* getPointerPlayerBounds();

/// grow the player timer for the Rush

/// </summary>
void plr_ChargeRush();


/// make the player Rush in the direction given

/// </summary>
void plr_Rush(sfVector2f _dir, sfBool* isXMoving, sfBool* isYMoving);

sfBool plr_IsInvisible();

/// \return Whether the player is currently rushing
sfBool plr_IsRushing();

/// <summary>
/// return a vector2f of the player forward
/// </summary>
sfVector2f plr_GetForward();
sfVector2f plr_GetWatchDir();
void plr_SelectPower();

PlrUnlockOrder plr_GetUnlockState();
void plr_SetUnlockState(PlrUnlockOrder _newUnlock);

int getCurrentPowerPlayer();
void plr_setPlayerIsPushed(sfBool _isPushed);
sfFloatRect* plr_getPointerAllPlayerBounds();
sfFloatRect plr_getPlayerBoundsWithEnemies();

/// <summary>
/// add the damage to the player's life
/// </summary>
/// <param name="_damage">the damage (1/4 of heart)</param>
void plr_addDamagePlayerLife(int _damage);

sfBool plr_IsInteractingWithStatue();
void plr_SetInteractStatue(sfBool _state);

/// \return Current player position
sfVector2f plr_GetPos();

/// \return Current player movement speed
sfVector2f plr_GetVel();

/// Sets the player's movement speed to a given value.
/// \deprecated Doesn't work due to the way player updates are computed. Will have to come back to it later - Priz
void plr_SetVel(sfVector2f _spd);

/// Forces the player to move following the given velocity vector.
/// This function takes the environment into account, preventing movement through a few world objects such as pots and walls (easily expandable to other kinds of objects).
void plr_ForceMove(sfVector2f _spd);

/// Similar to isiInFov(), except that it comes pre-configured with all the data of the player's FOV.
sfBool plr_IsInFOV(sfVector2f _pos);

/// Marks a flag indicating the player is colliding with an object, for handling of collisions outside of classic colliders.
void plr_StopMoving();

/// <summary>
/// This function returns if the player is phushed or not
/// </summary>
/// <returns>if the player is pushed</returns>
sfBool plr_getPlayerIsPushed();

/// Stuns the player for a certain duration
/// \param _stun - Whether you want to stun
void plr_StunPlayer(sfBool _stun);

/// \return Whether a forced action is playing, does NOT include dash, rush and push
sfBool plr_IsDoingAction();

/// \return Whether NO forced action is playing
sfBool plr_CanDoAction();

/// <summary>
/// This function returns the number of heart the player currently has
/// </summary>
/// <returns>the number of heart the player currently has</returns>
int plr_GetHeart();

/// <summary>
/// This function returns the ring the player is currently using
/// </summary>
/// <returns>the ring the player is currently using</returns>
PlrRingPowers plr_GetPower();

/// <summary>
/// This function returns the number of heart the player can have
/// </summary>
/// <returns>the number of heart the player can have</returns>
int plr_GetMaxHeart();

/// \return the slash's directionh
SlashDirection plr_GetSlashDirection();

/// \return the slash's start position
sfVector2f plr_GetSlashStartPos();

/// \return the slash's start velocity
sfVector2f plr_GetSlashStartVelocity();

/// \return the slash's collisions bounds, only if the player is attacking/catching
sfFloatRect plr_GetDamageBounds();

/// \return whether the slash could do damages (not catch)
sfBool plr_IsSlashDamaging();

/// Displays the player slash
void plr_DisplaySlash(Window* _window);

/// Displays the player hood
void plr_DisplayHood(Window* _window);

/// Displays the player endurance bar
void plr_DisplayEnduranceBar(Window* _window);

/// Displays the player shadow
void plr_DisplayShadow(Window* _window);

sfBool plr_GetCanMove();

void plr_SetCanMove(sfBool _canMove);

void plr_SetAllTimer(float _time);

/// Sets the given map position as the pig totem hood's target.
void plr_SetHoodTargetPos(sfVector2f _pos);

/// \return Position currently pointed to by the pig totem hood
sfVector2f plr_GetHoodTargetPos();

/// Stops all actions so the player can prioritize a new one
void plr_stopPlayerActions();

/// Reset all player's actions as if he had just spawned
void plr_resetPlayer();

/// Heal the player
/// \param _life - a quarter of a heart times _life
void plr_AddLife(int _life);

void plr_toggleFX(sfBool _boolToggle);

void plr_SetWalkType(PlrWalkType _type);

void plr_toggleFX(sfBool _boolToggle);

void plr_setCanFall(sfBool _canFall);

void plr_UpdateGameOver(Window* _window);

void plr_PlayGetObjectAnim();

void plr_toggleFX(sfBool _boolToggle);