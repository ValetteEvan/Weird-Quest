#pragma once
#include "tools.h"
#include "windowManager.h"
#define  GETPARTICLEDATA STD_LIST_GETDATA(particlesList, particles, i)

typedef enum {
	CIRCLE,
	SQUARE,
	ANIMATION_PARTICLE
}particlesType;

typedef enum {
	LOADING,
	STUN,
	LEAF,
	GAIN,
	FIREFLY,
	WALK,
	DUST,
	FEATHER,
	GRASS,
	BUBBLE,
	WATER,
	FIRE,
	prt_HEART, 
	CATCH, 
	POT,
	DAMAGE
}wichParticles;

typedef struct {
	sfVector2f pos;
	sfColor colorStart;
	sfColor colorEnd;
	float timeToDie;
	float timer;
	float timerFirefly;
	sfTexture* texture;
	float radiusStart;
	float radiusEnd;
	sfVector2f forward;
	sfVector2f sizeStart;
	sfVector2f sizeEnd;
	float velocity;
	float rotateVelocity;
	particlesType particleType;
	float teta;
	float width;
	float height;
	sfBool gravity;
	sfBool levitation;
	sfBool isGenerative;
	sfBool hasGenerate;
	sfBool isSpriteRecolored;
	sfBool hasFriction;
	sfBool isInFront;
	float timerSpawn;
	float outlineThickness;
	sfColor outlineColorStart;
	sfColor outlineColorEnd;
	int countPoint;
	int frameX;
	float animTimer;
	char* animName;
	wichParticles wichParticles;
}particles;

/// <summary>
/// init the particle manager
/// </summary>
void prt_InitParticles();

/// <summary>
/// create particles at a given position
/// </summary>
/// <param name="_spawnPos">the particle spawn pos</param>
/// <param name="_particlesAmount">the amount of particles</param>
/// <param name="_colorStart">the color when the particle spawn</param>
/// <param name="_colorEnd">the color when the particle die</param>
/// <param name="_timeToDie">the time before the particle die</param>
/// <param name="_texture">Texture of the particle (can be null)</param>
/// <param name="_radiusStart">the radius of the circle particle/the size of the square particle when it spawns</param>
/// <param name="_radiusEnd">the radius of the circle particle/the size of the square particle when it die</param>
/// <param name="_angleMin">the min hook_Angle in the direction where the particle will go</param>
/// <param name="_angleMax">the max hook_Angle in the direction where the particle will go</param>
/// <param name="_velocity">the particle velocity</param>
/// <param name="_rotateVelocity">the particle rotation velocity</param>
/// <param name="_outline">the outline thickness</param>
/// <param name="_outlineColorStart">the outline color when it spawns</param>
/// <param name="_outlineColorEnd">the outline color when it dies</param>
/// <param name="_partType">the particle type (circle square or sprite)</param>
/// <param name="_hasGravity">does the particle has gravity apllied to it</param>
/// <param name="_isGenerative">is the particle generating other smaller particles</param>
/// <param name="_isSpriteRecolored">is the sprite recolored (if yes the color start and end of the begining will be applied)</param>
void createParticles(sfVector2f _spawnPos, int _particlesAmount, sfColor _colorStart, sfColor _colorEnd, float _timeToDie,
	sfTexture* _texture, float _radiusStart, float _radiusEnd, float _angleMin, float _angleMax,
	float _velocity, float _rotateVelocity, float _outline, sfColor _outlineColorStart, sfColor _outlineColorEnd, sfPrimitiveType _partType,
	sfBool _hasGravity, sfBool _levitation, sfBool _isGenerative, sfBool _isSpriteRecolored, int _countPoint, sfVector2f _sizeStart, sfVector2f _sizeEnd, sfBool _hasFriction, sfBool _isInFront);

/// <summary>
/// create circle particles at a given position
/// </summary>
/// <param name="_spawnPos">the particle spawn pos</param>
/// <param name="_particlesAmount">the amount of particles</param>
/// <param name="_countPoint">the amount of side for polygon <=3 for circle</param>
/// <param name="_colorStart">the color when the particle spawn</param>
/// <param name="_colorEnd">the color when the particle die</param>
/// <param name="_timeToDie">the time before the particle die</param>
/// <param name="_radiusStart">the radius of the circle particle when it spawns</param>
/// <param name="_radiusEnd">the radius of the circle particle when it die</param>
/// <param name="_angleMin">the min hook_Angle in the direction where the particle will go</param>
/// <param name="_angleMax">the max hook_Angle in the direction where the particle will go</param>
/// <param name="_velocity">the particle velocity</param>
/// <param name="_outline">the outline thickness</param>
/// <param name="_outlineColorStart">the outline color when it spawns</param>
/// <param name="_outlineColorEnd">the outline color when it dies</param>
/// <param name="_hasGravity">does the particle has gravity apllied to it</param>
/// <param name="_isGenerative">is the particle generating other smaller particles</param>
void prt_CreateCircleParticles(sfVector2f _spawnPos, int _particlesAmount, int _countPoint, sfColor _colorStart, sfColor _colorEnd, float _timeToDie,
	float _radiusStart, float _radiusEnd, float _angleMin, float _angleMax, float _velocity, float _rotateVelocity, float _outline,
	sfColor _outlineColorStart, sfColor _outlineColorEnd, sfBool hasGravity, sfBool _levitation, sfBool _isGenerative, sfTexture* _texture, sfBool _isSpriteRecolored, 
	sfBool _hasFriction, sfBool _isInFront, wichParticles _wichParticles);

/// <summary>
/// create square particles at a given position
/// </summary>
/// <param name="_spawnPos">the particle spawn pos</param>
/// <param name="_particlesAmount">the amount of particles</param>
/// <param name="_colorStart">the color when the particle spawn</param>
/// <param name="_colorEnd">the color when the particle die</param>
/// <param name="_timeToDie">the time before the particle die</param>
/// <param name="_radiusStart">the size of the square particle when it spawns</param>
/// <param name="_radiusEnd">the size of the square particle when it die</param>
/// <param name="_angleMin">the min hook_Angle in the direction where the particle will go</param>
/// <param name="_angleMax">the max hook_Angle in the direction where the particle will go</param>
/// <param name="_velocity">the particle velocity</param>
/// <param name="_rotateVelocity">the particle rotation velocity</param>
/// <param name="_outline">the outline thickness</param>
/// <param name="_outlineColorStart">the outline color when it spawns</param>
/// <param name="_outlineColorEnd">the outline color when it dies</param>
/// <param name="_hasGravity">does the particle has gravity apllied to it</param>
/// <param name="_isGenerative">is the particle generating other smaller particles</param>
void prt_CreateSquareParticles(sfVector2f _spawnPos, int _particlesAmount, sfColor _colorStart, sfColor _colorEnd, float _timeToDie,
	sfVector2f _sizeStart, sfVector2f _sizeEnd, float _angleMin, float _angleMax, float _velocity, float _rotateVelocity, float _outline,
	sfColor _outlineColorStart, sfColor _outlineColorEnd, sfBool _hasGravity, sfBool _levitation, sfBool _isGenerative, sfTexture* _texture, 
	sfBool _isSpriteRecolored, sfBool _hasFriction, wichParticles _wichParticles);

void prt_CreateLoadingParticles(sfVector2f _spawnPos);

void prt_CreateStunParticles(sfVector2f _spawnpoint);

void prt_CreateLeafYoyoParticles(sfVector2f _spawnPos);

void prt_CreateLeafTreeParticles(sfVector2f _spawnPos);

void prt_CreateGainParticles(sfVector2f _spawnPos);

void prt_CreateFireflyParticles(sfVector2f _spawnPos);

void prt_CreateCaptureParticles(sfVector2f _spawnPos);

void prt_CreateGrassParticles(sfVector2f _spawnPos, float _angle);

void prt_CreateDestroyGrassParticles(sfVector2f _spawnPos);

void prt_CreateBubbleParticlesInFront(sfVector2f _spawnPos);

void prt_CreateBubbleFishParticles(sfVector2f _spawnPos);

void prt_CreateWaterSplahParticles(sfVector2f _spawnPos, float _angle);

void prt_CreateTableParticles(sfVector2f _spawnPos);

void prt_CreateBossDeathParticles(sfVector2f _spawnPos);

void prt_CreateFireParticles(sfVector2f _spawnPos);

void prt_CreateFireParticlesBrazier(sfVector2f _spawnPos);

void prt_CreateHeartDropParticles(sfVector2f _spawnPos);

void prt_CreateHealParticles(sfVector2f _spawnPos);

void prt_CreateParticlesPlayerDust(sfVector2f _spawnPos, float _angle);

void prt_CreateDamagePlayerParticles(sfVector2f _spawnPos);

void prt_CreateParticlesPotDust(sfVector2f _spawnPos, float _angle); 
/// <summary>
/// update every particles
/// </summary>
void prt_UpdateParticles();

/// <summary>
/// display every particles
/// </summary>
/// <param name="_window">the current window</param>
void prt_DisplayParticlesBehind(Window* _window);

void prt_DisplayParticlesInFront(Window* _window);


/// <summary>
/// create particles at a given position
/// </summary>
/// <param name="_spawnPos">the particle spawn pos</param>
/// <param name="_particlesAmount">the amount of particles</param>
/// <param name="_colorStart">the color when the particle spawn</param>
/// <param name="_colorEnd">the color when the particle die</param>
/// <param name="_timeToDie">the time before the particle die</param>
/// <param name="_texture">Texture of the particle (can be null)</param>
/// <param name="_radiusStart">the radius of the circle particle/the size of the square particle when it spawns</param>
/// <param name="_radiusEnd">the radius of the circle particle/the size of the square particle when it die</param>
/// <param name="_angleMin">the min hook_Angle in the direction where the particle will go</param>
/// <param name="_angleMax">the max hook_Angle in the direction where the particle will go</param>
/// <param name="_velocity">the particle velocity</param>
/// <param name="_rotateVelocity">the particle rotation velocity</param>
/// <param name="_outline">the outline thickness</param>
/// <param name="_outlineColorStart">the outline color when it spawns</param>
/// <param name="_outlineColorEnd">the outline color when it dies</param>
/// <param name="_partType">the particle type (circle square or sprite)</param>
/// <param name="_hasGravity">does the particle has gravity apllied to it</param>
/// <param name="_isGenerative">is the particle generating other smaller particles</param>
/// <param name="_isSpriteRecolored">is the sprite recolored (if yes the color start and end of the begining will be applied)</param>
void createAnimatedParticles(sfVector2f _spawnPos, int _particlesAmount, sfColor _colorStart, sfColor _colorEnd, float _timeToDie,
	sfTexture* _texture, float _scaleStart, float _scaleEnd, float _angleMin, float _angleMax,
	float _velocity, float _rotateVelocity, sfBool _hasGravity, sfBool _isSpriteRecolored, sfBool _hasFriction, char* _animationName, sfBool _isInFront, sfBool _levitation);

void prt_clear();