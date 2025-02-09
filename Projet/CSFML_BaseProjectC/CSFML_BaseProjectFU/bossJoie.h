#pragma once
#include "tools.h"
#include "windowManager.h"
#include "math.h"
#include "List.h"

stdList* bossJoieFX;
#define GETDATA_FX_BOSS_JOIE STD_LIST_GETDATA(bossJoieFX, BossJoieFX, i)

typedef struct {
    sfTexture* texture;
    sfTexture* textureHand;
    sfTexture* textureDisc;
    sfTexture* textureHalo;
    sfRectangleShape* bossSPR;
    sfRectangleShape* hand1SPR;
    sfRectangleShape* hand2SPR;
    sfRectangleShape* discSpr;
    sfRectangleShape* haloSpr;

    sfVector2f position;
    sfVector2f hand1pos;
    sfVector2f hand2pos;
    sfVector2f discPos;
    sfVector2f haloPos;
    sfVector2f fxHandPos;
    sfFloatRect hand1Bounds;
    sfFloatRect hand2Bounds;
    sfFloatRect bossBounds;
    sfFloatRect discBounds;
    sfVector2f origin;
    sfVector2f originHand;
    sfVector2f originDisc;
    sfVector2f originHalo;
    sfVector2f velocity;
    sfVector2f forward;
    sfVector2f oldForward;
    sfVector2f tempPlayerPos;
    sfFloatRect getPlayerBounds;
    float angle;
    int frameXboss;
    int frameXhand1;
    int frameXhand2;
    int frameXdisc;
    int frameXhalo;
    int frameXjump;
    int frameXdeath;
    int frameXattractF;
    int frameXattractD;
    float timerAnimBoss;
    float timerAnimHand1;
    float timerAnimHand2;
    float timerAnimDisc;
    float timerAnimHalo;
    float timerAnimJump;
    float timerAnimDeath;
    float timerAnimAttractF;
    float timerAnimAttractD;

    float timerAtk1;
    float timerAtk2;
    float timerAtk3;
    float angleAtk2;

    int bossLife;
    int hand1Life;
    int hand2Life;
    int discLife;
    sfBool isBossDamagable;
    sfBool isHand1Damagable;
    sfBool isHand2Damagable;
    sfBool isDiscDamagable;
    sfBool isBossStuned;
    sfBool isDead;
    sfBool hasFirstHand;
    sfBool hasSecondHand;
    sfBool hasHalo;
    sfBool hasTookDamage;
    sfBool hand1hasTookDamage;
    sfBool hand2hasTookDamage;
    sfBool discHasTookDamage;
    sfBool bossHasTookDamage;
    sfBool hasDisc;
    sfBool cristal;
    sfBool isLoadingAttract;
    sfBool isAttractingPlayer;
    sfBool isJumping;
    sfBool isPlayerCloseSpawn;
    sfBool animSpawnBoss;
    sfBool animSpawnDisc;
    sfBool animSpawnHand;
    sfBool sfxLoadingAttract;
    sfBool sfxAttract;
    sfBool sfxHalo;
    sfBool sfxJump;
    sfBool sfxDeath;
    sfBool fxHand1;
    sfBool fxHand2;
    sfBool fxDisc;
} BossJoie;

typedef enum {
    BJ_FX_HAND,
    BJ_FX_STUN,
    BJ_FX_JUMP,
    BJ_FX_SLASH
} BossJoieFXEnum;

typedef struct {
    sfVector2f pos;
    int frameX;
    float animTime;
    BossJoieFXEnum type;
    char* texture;
} BossJoieFX;

//Différente phase 
typedef enum {
    BJ_PHASE_SPAWN,
    BJ_PHASE_1,
    BJ_PHASE_2,
    BJ_PHASE_3,
    BJ_PHASE_DEATH
}BossJoiePhase;

//Diférente attaque du boss
typedef enum {
    BJ_ATK_NONE,
    BJ_ATK_1,
    BJ_ATK_2,
    BJ_ATK_3
}BossJoieFight;

typedef enum {
    ABJ_IDLE_F,
    ABJ_IDLE_TFL,
    ABJ_IDLE_TFR,
    ABJ_IDLE_TDL,
    ABJ_IDLE_TDR,
    ABJ_SPAWN,
    ABJ_ATTRACT_LOAD_F,
    ABJ_ATTRACT_LOAD_TFL,
    ABJ_ATTRACT_LOAD_TFR,
    ABJ_ATTRACT_LOAD_TDL,
    ABJ_ATTRACT_LOAD_TDR,
    ABJ_ATTRACT_F,
    ABJ_ATTRACT_TFL,
    ABJ_ATTRACT_TFR,
    ABJ_ATTRACT_TDL,
    ABJ_ATTRACT_TDR,
    ABJ_JUMP_F,
    ABJ_JUMP_TFL,
    ABJ_JUMP_TFR,
    ABJ_JUMP_TDL,
    ABJ_JUMP_TDR,
    ABJ_DEATH
}BossJoieAnimate;

typedef enum {
    BJ_HAND_1,
    BJ_HAND_2,
    BJ_DISC,
    BJ_CORE
}BossJoieParts;

BossJoie entity_BossJoie;
BossJoiePhase BossJoieState;
BossJoieFight bossJoieFight;

BossJoie initBossJoie(sfVector2f _position, int _life);
void bossJoie_restartBoss(BossJoie* _boss, sfVector2f _pos);
void rotationHandBossJoie(sfVector2f* _posHand1, sfVector2f* _posHand2, float _distance, float _angleBoss, BossJoie* _boss);
void updateBossJoie(BossJoie* _boss, Window* _window);
void displayBossJoie(Window* _w, BossJoie* _boss);
void deinitBossJoie(BossJoie* _boss);

void bossJoie_initFx();
void bossJoie_displayFX(Window* _w, BossJoie* _boss);
void bossJoie_movments(BossJoie* _boss);
void bossJoie_Atk1(BossJoie* _boss);
void bossJoie_Atk2(BossJoie* _boss);
void bossJoie_Atk3(BossJoie* _boss);
void bossJoie_resetHandPos(BossJoie* _boss);
void bossJoie_Animation(BossJoieAnimate _animationType, BossJoie* _boss);
void bossJoie_updateDamage(BossJoie* _boss);
void bossJoie_takeDamage(BossJoieParts _part, BossJoie* _boss);
void bossJoie_attachRing(BossJoie* _boss);
void bossJoie_stun(BossJoie* _boss);