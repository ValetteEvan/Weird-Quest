#pragma once
#include "tools.h"
#include "stateManager.h"
#include "textureManager.h"
#include "List.h"
#include "math.h"
#include "tears.h"
#include "player.h"

typedef struct {
	sfTexture* texture;  
	sfVector2f position; 
	sfVector2f size; 
	sfVector2f origin; 
	int life; 
	int cristalHP;  
	int firstHandHP; 
	int secondHandHP; 
	float animTimerBoss;
	float timerAnimHand;
	float timerAnimHand2;
	float timerAnimHand3; 
	float timerAnimHand4; 
	float timerAnimHand5; 
	float timerAnimHand6;
	float timerAnimHand7;
	int frameXBoss;  
	int frameXHand1;
	int frameXHand2;
	int frameXHand3;
	int frameXHand4;
	int frameXHand5; 
	int frameXHand6;
	int frameXHand7;
	sfIntRect bossRect; 
	sfBool disc; 
}BossTristesse;

typedef enum {
	HAND,
	DISC
}BossElementType;  

typedef struct {
	BossElementType type;  
	sfTexture* texture; 
	sfVector2f position;
	sfVector2f size; 
	sfVector2f origin; 
	sfIntRect elementRect; 
	sfBool isHere; 
	int life; 
	float timerfram; 
	int framX; 
}BossFragment; 

//Différente phase 
typedef enum {
	PHASE_SLEEP,
	PHASE_1, 
	PHASE_2,
	PHASE_3
}BossTristessePhase;

//Diférente attaque du boss
typedef enum {
	FIGHT_WAIT,
	FIGHT_0, 
	FIGHT_1,
	FIGHT_2, 
	FIGHT_3
}BossTristesseFight; 

BossTristesse bossTristesse; 
BossFragment BossAnneau;  


static BossTristessePhase BossState = PHASE_SLEEP;   
BossTristesseFight BossFight;  

void bossSadness_loadTexture();
BossTristesse initBoss(sfVector2f _position, int _life); 
BossFragment initBossFragment(sfVector2f _position, int life); 
void updateBoss(Window* window); 
void displayBoss(Window* window); 
void rotationHandBoss(sfVector2f* _pos_Hand_1T, sfVector2f* _pos_Hand_2T, float _distance, float _angle_Boss_Sadness, BossTristesse* _boss); 
void deinitBossTristesse();