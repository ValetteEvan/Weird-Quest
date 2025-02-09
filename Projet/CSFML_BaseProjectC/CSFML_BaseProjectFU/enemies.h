#pragma once
#include "tools.h"
#include "windowManager.h"
#include "List.h"
#include "mouseManager.h"
#include "FXenemies.h"


#define GD_ENEMY STD_LIST_GETDATA(enemiesList, EnemiesStruct, i)
#define EN_THROWN_DURATION 1.f
#define EN_NB_ENEMIES_TYPES 18

typedef enum EnemiesType {
	EN_YOYO_SPIDER,
	EN_VENIMOUS_YOYO_SPIDER,
	EN_CRAB,
	EN_GOOSE,
	EN_WHITE_BILLY_GOAT,
	EN_BLACK_BILLY_GOAT,
	EN_SNAKE,
	EN_MOSQUITO,
	EN_MOLE,
	EN_SCOLOPENDRE,
	EN_CHAMELEON,
	EN_BAT,
	EN_LITTLE_GHOST_JOY,
	EN_BIG_GHOST_JOY,
	EN_LITTLE_GHOST_SADNESS,
	EN_BIG_GHOST_SADNESS,
	EN_IMUNITY,
	EN_SEAGULL
}EnemiesType;

typedef enum {
	EN_STATE_WAIT,
	EN_STATE_WALK,
	EN_STATE_CHASE,
	EN_STATE_BACK_HOME,
	EN_STATE_ATTACK,
	EN_STATE_THROWN,
	EN_STATE_STUNNED,
	EN_STATE_CRYING,
	EN_STATE_ATTRACTING,
	EN_STATE_ESCAPING,
	EN_STATE_RUSH,
	EN_STATE_DAMAGING,
	EN_STATE_GOINGUP,
	EN_STATE_FALLING,
	EN_STATE_DETECTING,
	EN_STATE_PLR_RUSHING,
	EN_STATE_PATROLING,
	EN_STATE_REVEALING,
	EN_STATE_HIDDEN,
	EN_STATE_LAUNCH_TONGUE,
	EN_STATE_BACK_TONGUE,
	EN_STATE_BACK_ATTACK,
	EN_STATE_GOINGDOWN,
	EN_STATE_DEAD,
	EN_STATE_GOING_TO_FLY,
	EN_STATE_FLY,
	EN_STATE_FLY2,
	EN_STATE_EAT,
	EN_STATE_FALLING2,
}EnemiesState;

typedef struct {
	sfVector2f pos;
	sfVector2f origin;
	sfVector2f spawnPos;
	sfVector2f startPos;
	sfVector2f endPos;
	sfVector2f currentVelocity;
	sfVector2f startVelocity;
	float maxVelocity;
	sfVector2f forward;
	sfVector2f targetForward;
	sfVector2f scale;
	sfVector2f size;
	float time_wait;
	float time_to_wait;
	float time_invincibility;
	float time_detection;
	float time_prt;
	sfBool prtCapture;
	float time_sound;
	float speed;
	float radiusDetection;
	EnemiesType type;
	EnemiesState state;
	sfColor color;
	float stunTimer;
	int frameX;
	float timerAnim;
	int countOfSeePlayer;
	sfBool isOnString;
	sfBool isFalling;
	sfBool isFallingAnimDone;
	sfFloatRect colRect;
	sfIntRect spriteRect;
	float timerTears;
	float timerAllAttacks;
	float mass;
	float radius;
	sfBool isInHole;
	sfBool isOutHole;
	sfBool haveToHome;
	sfBool rebootFrameX;
	sfBool rebootFrameX2;
	sfBool isOnRoof;
	int life;
	sfBool hasBeenRushed;
	sfFloatRect collisionsBounds;
	sfVector2f Patrolpoint[4];
	int CurrentPatrolPoint;
	sfVector2f OldPos;
	float rotation;
	sfBool isOnePassedPulse;
	sfBool isOnScreen;
	int damage;
	float detectionAngle;
	float radiusDetectionSQRT;
	float rotationFalling;
	sfBool isHooking;

}EnemiesStruct;

typedef struct {
	sfVector2f position;
	EnemiesType type;
	float rotation;
}EnemiesStructToSave;

stdList* enemiesList;

void en_loadTexture();
void en_InitEnemies();
void en_SpawnEnemy(sfVector2f _pos, EnemiesType _type, float _rotation, sfBool _isFloorSpider);
void en_UpdateEnemies(Window* _window);
void en_UpdateYoyoSpider(EnemiesStruct* _enemy, EnemiesState _tmpState, int _i, float _delta, sfVector2f _tmpPpos, Window* _window);
void en_UpdateVenimousYoyoSpider(EnemiesStruct* _enemy, EnemiesState _tmpState, int _i, float _delta, sfVector2f _tmpPpos, Window* _window);
void en_UpdateCrab(EnemiesStruct* _enemy, EnemiesState _tmpState, int _i, float _delta, sfVector2f _tmpPpos, Window* _window);
void en_UpdateGoose(EnemiesStruct* _enemy, EnemiesState _tmpState, int _i, float _delta, sfVector2f _tmpPpos, Window* _window);
void en_UpdateWhiteBillyGoat(EnemiesStruct* _enemy, EnemiesState _tmpState, int _i, float _delta, sfVector2f _tmpPpos, Window* _window);
void en_UpdateBlackBillyGoat(EnemiesStruct* _enemy, EnemiesState _tmpState, int _i, float _delta, sfVector2f _tmpPpos, Window* _window);
void en_UpdateSnake(EnemiesStruct* _enemy, EnemiesState _tmpState, int _i, float _delta, sfVector2f _tmpPpos, Window* _window);
void en_UpdateMosquito(EnemiesStruct* _enemy, EnemiesState _tmpState, int _i, float _delta, sfVector2f _tmpPpos, Window* _window);
void en_UpdateMole(EnemiesStruct* _enemy, EnemiesState _tmpState, int _i, float _delta, sfVector2f _tmpPpos, Window* _window);
void en_UpdateScolopendre(EnemiesStruct* _enemy, EnemiesState _tmpState, int _i, float _delta, sfVector2f _tmpPpos);
void en_UpdateChameleon(EnemiesStruct* _enemy, EnemiesState _tmpState, int _i, float _delta, sfVector2f _tmpPpos, Window* _window);
void en_UpdateBat(EnemiesStruct* _enemy, EnemiesState _tmpState, int _i, float _delta, sfVector2f _tmpPpos, Window* _window);
void en_UpdateLittleGhostJoy(EnemiesStruct* _enemy, EnemiesState _tmpState, int _i, float _delta, sfVector2f _tmpPpos, Window* _window);
void en_UpdateBigGhostJoy(EnemiesStruct* _enemy, EnemiesState _tmpState, int _i, float _delta, sfVector2f _tmpPpos, Window* _window);
void en_UpdateLittleGhostSadness(EnemiesStruct* _enemy, EnemiesState _tmpState, int _i, float _delta, sfVector2f _tmpPpos, Window* _window);
void en_UpdateBigGhostSadness(EnemiesStruct* _enemy, EnemiesState _tmpState, int _i, float _delta, sfVector2f _tmpPpos, Window* _window);
void en_UpdateImunity(EnemiesStruct* _enemy, EnemiesState _tmpState, int _i, float _delta, sfVector2f _tmpPpos);
void en_UpdateSeagull(EnemiesStruct* _enemy, EnemiesState _tmpState, int _i, float _delta, sfVector2f _tmpPpos);

void en_DisplayEnemiesShadows(Window* _window);
void en_DisplayEnemies(Window* _window);
void en_DisplayYoyoSpider(sfRenderTexture* _rdrt, EnemiesStruct* _enemy, sfVector2f _tmpPpos);
void en_DisplayVenimousYoyoSpider(sfRenderTexture* _rdrt, EnemiesStruct* _enemy, sfVector2f _tmpPpos);
void en_DisplayCrab(sfRenderTexture* _rdrt, EnemiesStruct* _enemy, sfVector2f _tmpPpos);
void en_DisplayGoose(sfRenderTexture* _rdrt, EnemiesStruct* _enemy, sfVector2f _tmpPpos);
void en_DisplayWhiteBillyGoat(sfRenderTexture* _rdrt, EnemiesStruct* _enemy, sfVector2f _tmpPpos);
void en_DisplayBlackBillyGoat(sfRenderTexture* _rdrt, EnemiesStruct* _enemy, sfVector2f _tmpPpos);
void en_DisplaySnake(sfRenderTexture* _rdrt, EnemiesStruct* _enemy, sfVector2f _tmpPpos);
void en_DisplayMosquito(sfRenderTexture* _rdrt, EnemiesStruct* _enemy, sfVector2f _tmpPpos);
void en_DisplayMole(sfRenderTexture* _rdrt, EnemiesStruct* _enemy, sfVector2f _tmpPpos);
void en_DisplayScolopendre(sfRenderTexture* _rdrt, EnemiesStruct* _enemy, sfVector2f _tmpPpos);
void en_DisplayChameleon(sfRenderTexture* _rdrt, EnemiesStruct* _enemy, sfVector2f _tmpPpos);
void en_DisplayBat(sfRenderTexture* _rdrt, EnemiesStruct* _enemy, sfVector2f _tmpPpos);
void en_DisplayLittleGhostJoy(sfRenderTexture* _rdrt, EnemiesStruct* _enemy, sfVector2f _tmpPpos);
void en_DisplayBigGhostJoy(sfRenderTexture* _rdrt, EnemiesStruct* _enemy, sfVector2f _tmpPpos);
void en_DisplayLittleGhostSadness(sfRenderTexture* _rdrt, EnemiesStruct* _enemy, sfVector2f _tmpPpos);
void en_DisplayBigGhostSadness(sfRenderTexture* _rdrt, EnemiesStruct* _enemy, sfVector2f _tmpPpos);
void en_DisplayChameleonTongue(sfRenderTexture* _rdrt, EnemiesStruct* _enemy);
void en_DisplayImunity(sfRenderTexture* _rdrt, EnemiesStruct* _enemy, sfVector2f _tmpPpos);
void en_DisplaySeagull(sfRenderTexture* _rdrt, EnemiesStruct* _enemy, sfVector2f _tmpPpos);

void en_DisplayYoyoSpiderShadow(sfRenderTexture* _rdrt, EnemiesStruct* _enemy);
void en_DisplayVenimousYoyoSpiderShadow(sfRenderTexture* _rdrt, EnemiesStruct* _enemy);
void en_DisplayCrabShadow(sfRenderTexture* _rdrt, EnemiesStruct* _enemy);
void en_DisplayGooseShadow(sfRenderTexture* _rdrt, EnemiesStruct* _enemy);
void en_DisplayWhiteBillyGoatShadow(sfRenderTexture* _rdrt, EnemiesStruct* _enemy);
void en_DisplayBlackBillyGoatShadow(sfRenderTexture* _rdrt, EnemiesStruct* _enemy);
void en_DisplaySnakeShadow(sfRenderTexture* _rdrt, EnemiesStruct* _enemy);
void en_DisplayMosquitoShadow(sfRenderTexture* _rdrt, EnemiesStruct* _enemy);
void en_DisplayMoleShadow(sfRenderTexture* _rdrt, EnemiesStruct* _enemy);
void en_DisplayScolopendreShadow(sfRenderTexture* _rdrt, EnemiesStruct* _enemy);
void en_DisplayChameleonShadow(sfRenderTexture* _rdrt, EnemiesStruct* _enemy);
void en_DisplayBatShadow(sfRenderTexture* _rdrt, EnemiesStruct* _enemy);
void en_DisplayLittleGhostJoyShadow(sfRenderTexture* _rdrt, EnemiesStruct* _enemy);
void en_DisplayBigGhostJoyShadow(sfRenderTexture* _rdrt, EnemiesStruct* _enemy);
void en_DisplayLittleGhostSadnessShadow(sfRenderTexture* _rdrt, EnemiesStruct* _enemy);
void en_DisplayBigGhostSadnessShadow(sfRenderTexture* _rdrt, EnemiesStruct* _enemy);
void en_DisplayImunityShadow(sfRenderTexture* _rdrt, EnemiesStruct* _enemy);
void en_DisplaySeagullShadow(sfRenderTexture* _rdrt, EnemiesStruct* _enemy);

sfBool en_CanBeDetected(sfVector2f _lookingObject, sfVector2f _targetObject, sfVector2f _forwardVector, float _fov, float _maxRange);

/// <summary>
/// Checks if all enemies intersects with the slash
/// </summary>
/// <param name="_damageBounds">collisions of the slash</param>
/// <param name="_damaging">sfTrue if damaging, sfFalse if catching</param>
void en_IsInDamageBounds(sfFloatRect* _damageBounds, sfBool _damaging, Window* _window);
void en_DestroyEnemyInEditor(Window* _window);
sfFloatRect* en_getPointerBounds(int _id);
EnemiesStructToSave* getMobsListToSave();
void checkCollisions(int _id, float _dt);
EnemiesStruct* en_getClosestEnemy(sfVector2f _pos);
sfFloatRect en_getBounds(EnemiesStruct* _enemy);
sfFloatRect en_getCollisionsBounds(EnemiesStruct* _enemy);
void en_AttachRing(sfBool _attach, EnemiesStruct* _enemy);
void en_Attract(EnemiesStruct* _enemy);
void en_Repulse(EnemiesStruct* _enemy);
void en_Stasis(EnemiesStruct* _enemy);
void en_Destroy(EnemiesStruct* _enemy);
void blt_shouldSendBackNut(sfFloatRect* _damageBounds);
void en_Erase(int _id, sfVector2f _pos, FXenemies_enum _type, Window* _window);
void en_Clear();

/// <summary>
/// Gets the closest Enemy position of a certain type from a start position
/// </summary>
/// <param name="_startPos">the start position</param>
/// <param name="_type">the wanted type</param>
/// <returns>the world position of the closest enemy with the given type, or {-100, -100} if none is found</returns>
sfVector2f en_GetClosestGivenEnemyPos(sfVector2f _startPos, EnemiesType _type);
sfFloatRect en_GetChameleonTongueArea(EnemiesStruct* _enemy);
sfFloatRect en_GetChameleonTongueBounds(EnemiesStruct* _enemy);
sfFloatRect en_GetChameleonTongueCollisions(EnemiesStruct* _enemy);

/// <summary>
/// Gets the appropriate vector considering the rotation
/// </summary>
/// <param name="_v">the vector as it was a 0.f angle</param>
/// <param name="_rotation">the enemy rotation</param>
/// <returns>the proper vector</returns>
sfVector2f en_GetAppropriateVectorRotation(sfVector2f _v, float _rotation);
float en_GetChameleonTongueScale(EnemiesStruct* _enemy);
sfBool en_IsPlayerBlockedByChameleonTongue(sfVector2f _nextPlayerPos);
sfBool en_IsScolopendreHorizontal(float _rotation);
sfFloatRect en_GetAppropriateScolopendreBounds(EnemiesStruct* _enemy);
void deinitEnemies();
sfBool en_isAnySpiderHere();

int getEnAmount();
