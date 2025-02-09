#include "particlesManager.h"
#include "tools.h"
#include "list.h"
#include "math.h"
#include "ressourcesManager.h"
#include "textureManager.h"
#include "viewManager.h"
#include "particlesManager.h"
#include "shaderManager.h"
#include "map.h"

stdList* particlesList;

sfCircleShape* circleShpParticles;
sfRectangleShape* rectParticles;

sfRenderStates renderStateParticles;
sfShader* shaderParticles;

void prt_InitParticles()
{
	particlesList = stdList_Create(sizeof(particles), 0);
	circleShpParticles = sfCircleShape_create();
	rectParticles = sfRectangleShape_create();

	shaderParticles = NULL;

	if (sfShader_isAvailable()) {
		shaderParticles = sfShader_createFromFile(NULL, NULL, "..\\Ressources\\Shaders\\spriteRecolor.frag");
		renderStateParticles.shader = shaderParticles;
		renderStateParticles.blendMode = sfBlendAlpha;
		renderStateParticles.transform = sfTransform_Identity;
		renderStateParticles.texture = NULL;
	}
}

void createParticles(sfVector2f _spawnPos, int _particlesAmount, sfColor _colorStart, sfColor _colorEnd, float _timeToDie,
	sfTexture* _texture, float _radiusStart, float _radiusEnd, float _angleMin, float _angleMax,
	float _velocity, float _rotateVelocity, float _outline, sfColor _outlineColorStart, sfColor _outlineColorEnd, sfPrimitiveType _partType,
	sfBool _hasGravity, sfBool _levitation, sfBool _isGenerative, sfBool _isSpriteRecolored, int _countPoint, sfVector2f _sizeStart, sfVector2f _sizeEnd, sfBool _hasFriction, sfBool _isInFront, wichParticles _wichParticles)
{
	for (int i = 0; i < _particlesAmount; i++)
	{
		particles tempParticle = { 0 };
		tempParticle.colorStart = _colorStart;
		tempParticle.animName = NULL;
		tempParticle.colorEnd = _colorEnd;
		tempParticle.radiusStart = _radiusStart;
		tempParticle.radiusEnd = _radiusEnd;
		tempParticle.timer = 0.f;
		tempParticle.timerFirefly = 0.f;
		tempParticle.teta = 0.f;
		tempParticle.texture = _texture;
		tempParticle.pos = _spawnPos;
		tempParticle.forward = polarCoord(vector2f(0.f, 0.f), 1.f, rand_float(_angleMin, _angleMax) * DEG2RAD);
		tempParticle.timeToDie = _timeToDie;
		tempParticle.velocity = _velocity;
		tempParticle.rotateVelocity = _rotateVelocity;
		tempParticle.particleType = _partType;
		tempParticle.gravity = _hasGravity;
		tempParticle.levitation = _levitation;
		tempParticle.isGenerative = _isGenerative;
		tempParticle.hasGenerate = sfFalse;
		tempParticle.timerSpawn = 0.f;
		tempParticle.outlineThickness = _outline;
		tempParticle.outlineColorStart = _outlineColorStart;
		tempParticle.outlineColorEnd = _outlineColorEnd;
		tempParticle.isSpriteRecolored = _isSpriteRecolored;
		tempParticle.countPoint = _countPoint;
		tempParticle.sizeStart = _sizeStart;
		tempParticle.sizeEnd = _sizeEnd;
		tempParticle.hasFriction = _hasFriction;
		tempParticle.isInFront = _isInFront;
		tempParticle.wichParticles = _wichParticles;

		tempParticle.frameX = 0;
		tempParticle.animTimer = 0.f;


		particlesList->push_back(&particlesList, &tempParticle);
	}
}

void prt_CreateCircleParticles(sfVector2f _spawnPos, int _particlesAmount, int _countPoint, sfColor _colorStart, sfColor _colorEnd, float _timeToDie, float _radiusStart,
	float _radiusEnd, float _angleMin, float _angleMax, float _velocity, float _rotateVelocity, float _outline, sfColor _outlineColorStart, sfColor _outlineColorEnd,
	sfBool _hasGravity, sfBool _levitation, sfBool _isGenerative, sfTexture* _texture, sfBool _isSpriteRecolored, sfBool _hasFriction, sfBool _isInFront, wichParticles _wichParticles)
{
	createParticles(_spawnPos, _particlesAmount, _colorStart, _colorEnd, _timeToDie, _texture, _radiusStart, _radiusEnd, _angleMin, _angleMax, _velocity,
		_rotateVelocity, _outline, _outlineColorStart, _outlineColorEnd, CIRCLE, _hasGravity, _levitation, _isGenerative, _isSpriteRecolored, _countPoint, vector2f(0.f, 0.f), vector2f(0.f, 0.f), _hasFriction, _isInFront, _wichParticles);
}

void prt_CreateSquareParticles(sfVector2f _spawnPos, int _particlesAmount, sfColor _colorStart, sfColor _colorEnd, float _timeToDie,
	sfVector2f _sizeStart, sfVector2f _sizeEnd, float _angleMin, float _angleMax, float _velocity, float _rotateVelocity, float _outline,
	sfColor _outlineColorStart, sfColor _outlineColorEnd, sfBool _hasGravity, sfBool _levitation, sfBool _isGenerative, sfTexture* _texture, sfBool _isSpriteRecolored, sfBool _hasFriction, sfBool _isInFront, wichParticles _wichParticles)
{
	createParticles(_spawnPos, _particlesAmount, _colorStart, _colorEnd, _timeToDie, _texture, 0.f, 0.f, _angleMin, _angleMax, _velocity,
		_rotateVelocity, _outline, _outlineColorStart, _outlineColorEnd, SQUARE, _hasGravity, _levitation, _isGenerative, _isSpriteRecolored, 0, _sizeStart, _sizeEnd, _hasFriction, _isInFront, _wichParticles);
}

void prt_CreateLoadingParticles(sfVector2f _spawnPos)
{
	prt_CreateCircleParticles(_spawnPos, 2, 8, sfColor_fromRGBA(51.f, 144.f, 139.f, 200.f), sfColor_fromRGBA(51.f, 144.f, 139.f, 150.f), 0.5f, 16.f, 0.f, 180.f, 180.f, 105.f, 0.f, 0.3f,
		sfColor_fromRGBA(0.f, 0.f, 0.f, 200.f), sfColor_fromRGBA(0.f, 0.f, 0.f, 150.f), sfTrue, sfFalse, sfFalse, GetSfTexture("Dust_Player"), sfFalse, sfFalse, sfTrue, LOADING);
}

void prt_CreateLeafYoyoParticles(sfVector2f _spawnPos)
{
	for (int i = 0; i < 6; i++)
	{
		if (i == 0 || i == 1)
			prt_CreateCircleParticles(vector2f(rand_float(_spawnPos.x - 50.f, _spawnPos.x + 50.f), rand_float(_spawnPos.y - 10.f, _spawnPos.y + 50.f)), 1, 8, sfColor_fromRGBA(36.f, 165.f, 15.f, 200.f),
				sfColor_fromRGBA(36.f, 165.f, 15.f, 150.f), 1.f, 6.f, 0.f, 90.f, 90.f, 30.f, 200.f, 0.3f, sfColor_fromRGBA(255.f, 255.f, 255.f, 200.f), sfColor_fromRGBA(255.f, 255.f, 255.f, 150.f),
				sfTrue, sfFalse, sfFalse, GetSfTexture("leaf_Particles1"), sfFalse, sfFalse, sfTrue, LEAF);
		if (i == 2)
			prt_CreateCircleParticles(vector2f(rand_float(_spawnPos.x - 50.f, _spawnPos.x + 50.f), rand_float(_spawnPos.y - 10.f, _spawnPos.y + 50.f)), 1, 8, sfColor_fromRGBA(36.f, 165.f, 15.f, 200.f),
				sfColor_fromRGBA(36.f, 165.f, 15.f, 150.f), 1.f, 6.f, 0.f, 90.f, 90.f, 30.f, 200.f, 0.3f, sfColor_fromRGBA(255.f, 255.f, 255.f, 200.f), sfColor_fromRGBA(255.f, 255.f, 255.f, 150.f),
				sfTrue, sfFalse, sfFalse, GetSfTexture("leaf_Particles2"), sfFalse, sfFalse, sfTrue, LEAF);
		if (i == 3)
			prt_CreateCircleParticles(vector2f(rand_float(_spawnPos.x - 50.f, _spawnPos.x + 50.f), rand_float(_spawnPos.y - 10.f, _spawnPos.y + 50.f)), 1, 8, sfColor_fromRGBA(36.f, 165.f, 15.f, 200.f),
				sfColor_fromRGBA(36.f, 165.f, 15.f, 150.f), 1.f, 6.f, 0.f, 90.f, 90.f, 30.f, 200.f, 0.3f, sfColor_fromRGBA(255.f, 255.f, 255.f, 200.f), sfColor_fromRGBA(255.f, 255.f, 255.f, 150.f),
				sfTrue, sfFalse, sfFalse, GetSfTexture("leaf_Particles3"), sfFalse, sfFalse, sfTrue, LEAF);
		if (i == 4)
			prt_CreateCircleParticles(vector2f(rand_float(_spawnPos.x - 50.f, _spawnPos.x + 50.f), rand_float(_spawnPos.y - 10.f, _spawnPos.y + 50.f)), 1, 8, sfColor_fromRGBA(36.f, 165.f, 15.f, 200.f),
				sfColor_fromRGBA(36.f, 165.f, 15.f, 150.f), 1.f, 6.f, 0.f, 90.f, 90.f, 30.f, 200.f, 0.3f, sfColor_fromRGBA(255.f, 255.f, 255.f, 200.f), sfColor_fromRGBA(255.f, 255.f, 255.f, 150.f),
				sfTrue, sfFalse, sfFalse, GetSfTexture("leaf_Particles4"), sfFalse, sfFalse, sfTrue, LEAF);
		if (i == 5)
			prt_CreateCircleParticles(vector2f(rand_float(_spawnPos.x - 50.f, _spawnPos.x + 50.f), rand_float(_spawnPos.y - 10.f, _spawnPos.y + 50.f)), 1, 8, sfColor_fromRGBA(36.f, 165.f, 15.f, 200.f),
				sfColor_fromRGBA(36.f, 165.f, 15.f, 150.f), 1.f, 6.f, 0.f, 90.f, 90.f, 30.f, 200.f, 0.3f, sfColor_fromRGBA(255.f, 255.f, 255.f, 200.f), sfColor_fromRGBA(255.f, 255.f, 255.f, 150.f),
				sfTrue, sfFalse, sfFalse, GetSfTexture("leaf_Particles5"), sfFalse, sfFalse, sfTrue, LEAF);
	}
}

void prt_CreateLeafTreeParticles(sfVector2f _spawnPos)
{
	for (int i = 0; i < 10; i++)
	{
		if (i == 0 || i == 1)
			prt_CreateCircleParticles(vector2f(rand_float(_spawnPos.x - 150.f, _spawnPos.x + 150.f), rand_float(_spawnPos.y - 10.f, _spawnPos.y + 150.f)), 1, 8, sfColor_fromRGBA(36.f, 165.f, 15.f, 200.f),
				sfColor_fromRGBA(36.f, 165.f, 15.f, 150.f), 3.f, 12.f, 5.f, 90.f, 90.f, 30.f, 200.f, 0.3f, sfColor_fromRGBA(255.f, 255.f, 255.f, 200.f), sfColor_fromRGBA(255.f, 255.f, 255.f, 150.f),
				sfTrue, sfFalse, sfFalse, GetSfTexture("leaf_Particles1"), sfFalse, sfFalse, sfTrue, LEAF);
		if (i == 2 || i == 6)
			prt_CreateCircleParticles(vector2f(rand_float(_spawnPos.x - 150.f, _spawnPos.x + 150.f), rand_float(_spawnPos.y - 10.f, _spawnPos.y + 150.f)), 1, 8, sfColor_fromRGBA(36.f, 165.f, 15.f, 200.f),
				sfColor_fromRGBA(36.f, 165.f, 15.f, 150.f), 3.f, 12.f, 5.f, 90.f, 90.f, 30.f, 200.f, 0.3f, sfColor_fromRGBA(255.f, 255.f, 255.f, 200.f), sfColor_fromRGBA(255.f, 255.f, 255.f, 150.f),
				sfTrue, sfFalse, sfFalse, GetSfTexture("leaf_Particles2"), sfFalse, sfFalse, sfTrue, LEAF);
		if (i == 3 || i == 7)
			prt_CreateCircleParticles(vector2f(rand_float(_spawnPos.x - 150.f, _spawnPos.x + 150.f), rand_float(_spawnPos.y - 10.f, _spawnPos.y + 150.f)), 1, 8, sfColor_fromRGBA(36.f, 165.f, 15.f, 200.f),
				sfColor_fromRGBA(36.f, 165.f, 15.f, 150.f), 3.f, 12.f, 5.f, 90.f, 90.f, 30.f, 200.f, 0.3f, sfColor_fromRGBA(255.f, 255.f, 255.f, 200.f), sfColor_fromRGBA(255.f, 255.f, 255.f, 150.f),
				sfTrue, sfFalse, sfFalse, GetSfTexture("leaf_Particles3"), sfFalse, sfFalse, sfTrue, LEAF);
		if (i == 4 || i == 8)
			prt_CreateCircleParticles(vector2f(rand_float(_spawnPos.x - 150.f, _spawnPos.x + 150.f), rand_float(_spawnPos.y - 10.f, _spawnPos.y + 150.f)), 1, 8, sfColor_fromRGBA(36.f, 165.f, 15.f, 200.f),
				sfColor_fromRGBA(36.f, 165.f, 15.f, 150.f), 3.f, 12.f, 5.f, 90.f, 90.f, 30.f, 200.f, 0.3f, sfColor_fromRGBA(255.f, 255.f, 255.f, 200.f), sfColor_fromRGBA(255.f, 255.f, 255.f, 150.f),
				sfTrue, sfFalse, sfFalse, GetSfTexture("leaf_Particles4"), sfFalse, sfFalse, sfTrue, LEAF);
		if (i == 5 || i == 9)
			prt_CreateCircleParticles(vector2f(rand_float(_spawnPos.x - 150.f, _spawnPos.x + 150.f), rand_float(_spawnPos.y - 10.f, _spawnPos.y + 150.f)), 1, 8, sfColor_fromRGBA(36.f, 165.f, 15.f, 200.f),
				sfColor_fromRGBA(36.f, 165.f, 15.f, 150.f), 3.f, 12.f, 5.f, 90.f, 90.f, 30.f, 200.f, 0.3f, sfColor_fromRGBA(255.f, 255.f, 255.f, 200.f), sfColor_fromRGBA(255.f, 255.f, 255.f, 150.f),
				sfTrue, sfFalse, sfFalse, GetSfTexture("leaf_Particles5"), sfFalse, sfFalse, sfTrue, LEAF);
	}
}

void prt_CreateGainParticles(sfVector2f _spawnPos)
{
	prt_CreateCircleParticles(_spawnPos, 25, 8, sfColor_fromRGBA(248.f, 242.f, 46.f, 200.f),
		sfColor_fromRGBA(248.f, 242.f, 46.f, 150.f), 1.5f, 6.f, 0.f, 0.f, 360.f, 130.f, 200.f, 0.3f, sfColor_fromRGBA(255.f, 255.f, 255.f, 200.f), sfColor_fromRGBA(255.f, 255.f, 255.f, 150.f),
		sfFalse, sfFalse, sfFalse, NULL, sfFalse, sfFalse, sfFalse, GAIN);
}

void prt_CreateParticlesPlayerDust(sfVector2f _spawnPos, float _angle)
{
	prt_CreateCircleParticles(_spawnPos, 1, 8, sfColor_fromRGBA(248.f, 242.f, 46.f, 200.f), sfColor_fromRGBA(248.f, 242.f, 46.f, 150.f), 0.75f, 16.f, 0.f, _angle, _angle, 100.f, 0.f, 0.3f,
		sfColor_fromRGBA(255.f, 255.f, 255.f, 200.f), sfColor_fromRGBA(255.f, 255.f, 255.f, 150.f), sfTrue, sfFalse, sfFalse, GetSfTexture("Dust_Player"), sfFalse, sfFalse, sfFalse, DUST);
}

void prt_CreateParticlesPotDust(sfVector2f _spawnPos, float _angle)
{
	prt_CreateCircleParticles(_spawnPos, 3, 20, sfColor_fromRGBA(248.f, 242.f, 46.f, 200.f), sfColor_fromRGBA(248.f, 242.f, 46.f, 150.f), 0.55f, 16.f, 0.f, _angle, _angle, 100.f, 0.f, 0.3f,
		sfColor_fromRGBA(255.f, 255.f, 255.f, 200.f), sfColor_fromRGBA(255.f, 255.f, 255.f, 150.f), sfTrue, sfFalse, sfFalse, GetSfTexture("Dust_Player"), sfFalse, sfTrue, sfFalse, DUST);
}

void prt_CreateFireflyParticles(sfVector2f _spawnPos)
{
	prt_CreateCircleParticles(_spawnPos, 1, 8, sfColor_fromRGBA(248.f, 242.f, 46.f, 200.f), sfColor_fromRGBA(248.f, 242.f, 46.f, 150.f), 60.f, 8.f, 8.f, 90.f, 90.f, 10.f, 0.f, 0.3f,
		sfColor_fromRGBA(255.f, 255.f, 255.f, 200.f), sfColor_fromRGBA(255.f, 255.f, 255.f, 150.f), sfFalse, sfFalse, sfFalse, GetSfTexture("firefly_Particles1"), sfFalse, sfFalse, sfTrue, FIREFLY);
	prt_CreateCircleParticles(vector2f(_spawnPos.x + 300.f, _spawnPos.y), 1, 8, sfColor_fromRGBA(248.f, 242.f, 46.f, 200.f), sfColor_fromRGBA(248.f, 242.f, 46.f, 150.f), 60.f, 8.f, 8.f, 90.f, 90.f, 10.f, 0.f, 0.3f,
		sfColor_fromRGBA(255.f, 255.f, 255.f, 200.f), sfColor_fromRGBA(255.f, 255.f, 255.f, 150.f), sfFalse, sfFalse, sfFalse, GetSfTexture("firefly_Particles2"), sfFalse, sfFalse, sfTrue, FIREFLY);
}

void prt_CreateCaptureParticles(sfVector2f _spawnPos)
{
	prt_CreateCircleParticles(vector2f(_spawnPos.x + 30.f, _spawnPos.y - 60.f), 1, 8, sfColor_fromRGBA(192.f, 192.f, 192.f, 200.f), sfColor_fromRGBA(192.f, 192.f, 192.f, 150.f)
		, 3.f, 15.f, 10.f, 120.f, 120.f, 60.f, 0.f, 0.3f, sfWhite, sfWhite, sfFalse, sfFalse, sfFalse, NULL, sfFalse, sfFalse, sfFalse, CATCH);
	prt_CreateCircleParticles(vector2f(_spawnPos.x + 60.f, _spawnPos.y - 30.f), 1, 8, sfColor_fromRGBA(192.f, 192.f, 192.f, 200.f), sfColor_fromRGBA(192.f, 192.f, 192.f, 150.f)
		, 3.f, 15.f, 10.f, 150.f, 150.f, 60.f, 0.f, 0.3f, sfWhite, sfWhite, sfFalse, sfFalse, sfFalse, NULL, sfFalse, sfFalse, sfFalse, CATCH);
	prt_CreateCircleParticles(vector2f(_spawnPos.x, _spawnPos.y - 75.f), 1, 8, sfColor_fromRGBA(192.f, 192.f, 192.f, 200.f), sfColor_fromRGBA(192.f, 192.f, 192.f, 150.f)
		, 3.f, 15.f, 10.f, 90.f, 90.f, 60.f, 0.f, 0.3f, sfWhite, sfWhite, sfFalse, sfFalse, sfFalse, NULL, sfFalse, sfFalse, sfFalse, CATCH);
	prt_CreateCircleParticles(vector2f(_spawnPos.x - 30.f, _spawnPos.y - 60.f), 1, 8, sfColor_fromRGBA(192.f, 192.f, 192.f, 200.f), sfColor_fromRGBA(192.f, 192.f, 192.f, 150.f)
		, 3.f, 15.f, 10.f, 60.f, 60.f, 60.f, 0.f, 0.3f, sfWhite, sfWhite, sfFalse, sfFalse, sfFalse, NULL, sfFalse, sfFalse, sfFalse, CATCH);
	prt_CreateCircleParticles(vector2f(_spawnPos.x - 60.f, _spawnPos.y - 30.f), 1, 8, sfColor_fromRGBA(192.f, 192.f, 192.f, 200.f), sfColor_fromRGBA(192.f, 192.f, 192.f, 150.f)
		, 3.f, 15.f, 10.f, 30.f, 30.f, 60.f, 0.f, 0.3f, sfWhite, sfWhite, sfFalse, sfFalse, sfFalse, NULL, sfFalse, sfFalse, sfFalse, CATCH);
	prt_CreateCircleParticles(vector2f(_spawnPos.x - 75.f, _spawnPos.y), 1, 8, sfColor_fromRGBA(192.f, 192.f, 192.f, 200.f), sfColor_fromRGBA(192.f, 192.f, 192.f, 150.f)
		, 3.f, 15.f, 10.f, 360.f, 360.f, 60.f, 0.f, 0.3f, sfWhite, sfWhite, sfFalse, sfFalse, sfFalse, NULL, sfFalse, sfFalse, sfFalse, CATCH);
	prt_CreateCircleParticles(vector2f(_spawnPos.x - 60.f, _spawnPos.y + 30.f), 1, 8, sfColor_fromRGBA(192.f, 192.f, 192.f, 200.f), sfColor_fromRGBA(192.f, 192.f, 192.f, 150.f)
		, 3.f, 15.f, 10.f, 330.f, 330.f, 60.f, 0.f, 0.3f, sfWhite, sfWhite, sfFalse, sfFalse, sfFalse, NULL, sfFalse, sfFalse, sfFalse, CATCH);
	prt_CreateCircleParticles(vector2f(_spawnPos.x - 30.f, _spawnPos.y + 60.f), 1, 8, sfColor_fromRGBA(192.f, 192.f, 192.f, 200.f), sfColor_fromRGBA(192.f, 192.f, 192.f, 150.f)
		, 3.f, 15.f, 10.f, 300.f, 300.f, 60.f, 0.f, 0.3f, sfWhite, sfWhite, sfFalse, sfFalse, sfFalse, NULL, sfFalse, sfFalse, sfFalse, CATCH);
	prt_CreateCircleParticles(vector2f(_spawnPos.x, _spawnPos.y + 75.f), 1, 8, sfColor_fromRGBA(192.f, 192.f, 192.f, 200.f), sfColor_fromRGBA(192.f, 192.f, 192.f, 150.f)
		, 3.f, 15.f, 10.f, 270.f, 270.f, 60.f, 0.f, 0.3f, sfWhite, sfWhite, sfFalse, sfFalse, sfFalse, NULL, sfFalse, sfFalse, sfFalse, CATCH);
	prt_CreateCircleParticles(vector2f(_spawnPos.x + 30.f, _spawnPos.y + 60.f), 1, 8, sfColor_fromRGBA(192.f, 192.f, 192.f, 200.f), sfColor_fromRGBA(192.f, 192.f, 192.f, 150.f)
		, 3.f, 15.f, 10.f, 240.f, 240.f, 60.f, 0.f, 0.3f, sfWhite, sfWhite, sfFalse, sfFalse, sfFalse, NULL, sfFalse, sfFalse, sfFalse, CATCH);
	prt_CreateCircleParticles(vector2f(_spawnPos.x + 60.f, _spawnPos.y + 30.f), 1, 8, sfColor_fromRGBA(192.f, 192.f, 192.f, 200.f), sfColor_fromRGBA(192.f, 192.f, 192.f, 150.f)
		, 3.f, 15.f, 10.f, 210.f, 210.f, 60.f, 0.f, 0.3f, sfWhite, sfWhite, sfFalse, sfFalse, sfFalse, NULL, sfFalse, sfFalse, sfFalse, CATCH);
	prt_CreateCircleParticles(vector2f(_spawnPos.x + 75.f, _spawnPos.y), 1, 8, sfColor_fromRGBA(192.f, 192.f, 192.f, 200.f), sfColor_fromRGBA(192.f, 192.f, 192.f, 150.f)
		, 3.f, 15.f, 10.f, 180.f, 180.f, 60.f, 0.f, 0.3f, sfWhite, sfWhite, sfFalse, sfFalse, sfFalse, NULL, sfFalse, sfFalse, sfFalse, CATCH);
}

void prt_CreateGrassParticles(sfVector2f _spawnPos, float _angle)
{
	int rdmGrass = rand_int(0, 6);
	if (rdmGrass == 0)
		prt_CreateCircleParticles(_spawnPos, 1, 8, sfColor_fromRGBA(248.f, 242.f, 46.f, 200.f), sfColor_fromRGBA(248.f, 242.f, 46.f, 150.f), 0.5f, 4.f, 4.f, _angle, _angle, 100.f, 0.f, 0.3f,
			sfColor_fromRGBA(255.f, 255.f, 255.f, 200.f), sfColor_fromRGBA(255.f, 255.f, 255.f, 150.f), sfTrue, sfFalse, sfFalse, GetSfTexture("grass_Particles1"), sfFalse, sfFalse, sfFalse, GRASS);
	else if (rdmGrass == 1)
		prt_CreateCircleParticles(vector2f(_spawnPos.x, _spawnPos.y), 1, 8, sfColor_fromRGBA(248.f, 242.f, 46.f, 200.f), sfColor_fromRGBA(248.f, 242.f, 46.f, 150.f), 0.75f, 2.f, 2.f, _angle, _angle, 100.f, 0.f, 0.3f,
			sfColor_fromRGBA(255.f, 255.f, 255.f, 200.f), sfColor_fromRGBA(255.f, 255.f, 255.f, 150.f), sfTrue, sfFalse, sfFalse, GetSfTexture("grass_Particles2"), sfFalse, sfFalse, sfFalse, GRASS);
	else if (rdmGrass == 2)
		prt_CreateCircleParticles(_spawnPos, 1, 8, sfColor_fromRGBA(255.f, 153.f, 0.f, 200.f), sfColor_fromRGBA(255.f, 153.f, 0.f, 150.f), 0.5f, 3.f, 3.f, _angle, _angle, 100.f, 0.f, 0.3f,
			sfColor_fromRGBA(255.f, 255.f, 255.f, 200.f), sfColor_fromRGBA(255.f, 255.f, 255.f, 150.f), sfTrue, sfFalse, sfFalse, GetSfTexture("grass_Particles3"), sfFalse, sfFalse, sfFalse, GRASS);
	else if (rdmGrass == 3)
		prt_CreateCircleParticles(_spawnPos, 1, 8, sfColor_fromRGBA(248.f, 242.f, 46.f, 200.f), sfColor_fromRGBA(248.f, 242.f, 46.f, 150.f), 0.5f, 4.f, 4.f, _angle, _angle, 100.f, 0.f, 0.3f,
			sfColor_fromRGBA(255.f, 255.f, 255.f, 200.f), sfColor_fromRGBA(255.f, 255.f, 255.f, 150.f), sfTrue, sfFalse, sfFalse, GetSfTexture("grass_Particles4"), sfFalse, sfFalse, sfFalse, GRASS);
	else if (rdmGrass == 4)
		prt_CreateCircleParticles(_spawnPos, 1, 8, sfColor_fromRGBA(255.f, 153.f, 0.f, 200.f), sfColor_fromRGBA(255.f, 153.f, 0.f, 150.f), 0.5f, 2.f, 2.f, _angle, _angle, 100.f, 0.f, 0.3f,
			sfColor_fromRGBA(255.f, 255.f, 255.f, 200.f), sfColor_fromRGBA(255.f, 255.f, 255.f, 150.f), sfTrue, sfFalse, sfFalse, GetSfTexture("grass_Particles5"), sfFalse, sfFalse, sfFalse, GRASS);
	else if (rdmGrass == 5)
		prt_CreateCircleParticles(_spawnPos, 1, 8, sfColor_fromRGBA(255.f, 153.f, 0.f, 200.f), sfColor_fromRGBA(255.f, 153.f, 0.f, 150.f), 0.5f, 3.f, 3.f, _angle, _angle, 100.f, 0.f, 0.3f,
			sfColor_fromRGBA(255.f, 255.f, 255.f, 200.f), sfColor_fromRGBA(255.f, 255.f, 255.f, 150.f), sfTrue, sfFalse, sfFalse, GetSfTexture("grass_Particles6"), sfFalse, sfFalse, sfFalse, GRASS);
}

void prt_CreateDestroyGrassParticles(sfVector2f _spawnPos)
{
	prt_CreateCircleParticles(_spawnPos, 1, 8, sfColor_fromRGBA(255.f, 153.f, 0.f, 200.f), sfColor_fromRGBA(255.f, 153.f, 0.f, 150.f), 0.5f, 3.f, 3.f, 0.f, 30.f, 30.f, 0.f, 0.3f, sfColor_fromRGBA(255.f, 255.f, 255.f, 200.f), sfColor_fromRGBA(255.f, 255.f, 255.f, 150.f), sfTrue, sfFalse, sfFalse, GetSfTexture("grass_Particles5"), sfFalse, sfFalse, sfFalse, GRASS);
	prt_CreateCircleParticles(_spawnPos, 1, 8, sfColor_fromRGBA(255.f, 153.f, 0.f, 200.f), sfColor_fromRGBA(255.f, 153.f, 0.f, 150.f), 0.5f, 3.f, 3.f, 0.f, 60.f, 60.f, 0.f, 0.3f, sfColor_fromRGBA(255.f, 255.f, 255.f, 200.f), sfColor_fromRGBA(255.f, 255.f, 255.f, 150.f), sfTrue, sfFalse, sfFalse, GetSfTexture("grass_Particles6"), sfFalse, sfFalse, sfFalse, GRASS);
	prt_CreateCircleParticles(_spawnPos, 1, 8, sfColor_fromRGBA(255.f, 153.f, 0.f, 200.f), sfColor_fromRGBA(255.f, 153.f, 0.f, 150.f), 0.5f, 3.f, 3.f, 0.f, 90.f, 90.f, 0.f, 0.3f, sfColor_fromRGBA(255.f, 255.f, 255.f, 200.f), sfColor_fromRGBA(255.f, 255.f, 255.f, 150.f), sfTrue, sfFalse, sfFalse, GetSfTexture("grass_Particles4"), sfFalse, sfFalse, sfFalse, GRASS);
	prt_CreateCircleParticles(_spawnPos, 1, 8, sfColor_fromRGBA(255.f, 153.f, 0.f, 200.f), sfColor_fromRGBA(255.f, 153.f, 0.f, 150.f), 0.5f, 3.f, 3.f, 0.f, 120.f, 120.f, 0.f, 0.3f, sfColor_fromRGBA(255.f, 255.f, 255.f, 200.f), sfColor_fromRGBA(255.f, 255.f, 255.f, 150.f), sfTrue, sfFalse, sfFalse, GetSfTexture("grass_Particles5"), sfFalse, sfFalse, sfFalse, GRASS);
	prt_CreateCircleParticles(_spawnPos, 1, 8, sfColor_fromRGBA(255.f, 153.f, 0.f, 200.f), sfColor_fromRGBA(255.f, 153.f, 0.f, 150.f), 0.5f, 3.f, 3.f, 0.f, 150.f, 150.f, 0.f, 0.3f, sfColor_fromRGBA(255.f, 255.f, 255.f, 200.f), sfColor_fromRGBA(255.f, 255.f, 255.f, 150.f), sfTrue, sfFalse, sfFalse, GetSfTexture("grass_Particles6"), sfFalse, sfFalse, sfFalse, GRASS);
	prt_CreateCircleParticles(_spawnPos, 1, 8, sfColor_fromRGBA(255.f, 153.f, 0.f, 200.f), sfColor_fromRGBA(255.f, 153.f, 0.f, 150.f), 0.5f, 3.f, 3.f, 0.f, 180.f, 180.f, 0.f, 0.3f, sfColor_fromRGBA(255.f, 255.f, 255.f, 200.f), sfColor_fromRGBA(255.f, 255.f, 255.f, 150.f), sfTrue, sfFalse, sfFalse, GetSfTexture("grass_Particles4"), sfFalse, sfFalse, sfFalse, GRASS);
	prt_CreateCircleParticles(_spawnPos, 1, 8, sfColor_fromRGBA(255.f, 153.f, 0.f, 200.f), sfColor_fromRGBA(255.f, 153.f, 0.f, 150.f), 0.5f, 3.f, 3.f, 0.f, 210.f, 210.f, 0.f, 0.3f, sfColor_fromRGBA(255.f, 255.f, 255.f, 200.f), sfColor_fromRGBA(255.f, 255.f, 255.f, 150.f), sfTrue, sfFalse, sfFalse, GetSfTexture("grass_Particles5"), sfFalse, sfFalse, sfFalse, GRASS);
	prt_CreateCircleParticles(_spawnPos, 1, 8, sfColor_fromRGBA(255.f, 153.f, 0.f, 200.f), sfColor_fromRGBA(255.f, 153.f, 0.f, 150.f), 0.5f, 3.f, 3.f, 0.f, 240.f, 240.f, 0.f, 0.3f, sfColor_fromRGBA(255.f, 255.f, 255.f, 200.f), sfColor_fromRGBA(255.f, 255.f, 255.f, 150.f), sfTrue, sfFalse, sfFalse, GetSfTexture("grass_Particles6"), sfFalse, sfFalse, sfFalse, GRASS);
	prt_CreateCircleParticles(_spawnPos, 1, 8, sfColor_fromRGBA(255.f, 153.f, 0.f, 200.f), sfColor_fromRGBA(255.f, 153.f, 0.f, 150.f), 0.5f, 3.f, 3.f, 0.f, 270.f, 270.f, 0.f, 0.3f, sfColor_fromRGBA(255.f, 255.f, 255.f, 200.f), sfColor_fromRGBA(255.f, 255.f, 255.f, 150.f), sfTrue, sfFalse, sfFalse, GetSfTexture("grass_Particles6"), sfFalse, sfFalse, sfFalse, GRASS);
	prt_CreateCircleParticles(_spawnPos, 1, 8, sfColor_fromRGBA(255.f, 153.f, 0.f, 200.f), sfColor_fromRGBA(255.f, 153.f, 0.f, 150.f), 0.5f, 3.f, 3.f, 0.f, 300.f, 300.f, 0.f, 0.3f, sfColor_fromRGBA(255.f, 255.f, 255.f, 200.f), sfColor_fromRGBA(255.f, 255.f, 255.f, 150.f), sfTrue, sfFalse, sfFalse, GetSfTexture("grass_Particles6"), sfFalse, sfFalse, sfFalse, GRASS);
	prt_CreateCircleParticles(_spawnPos, 1, 8, sfColor_fromRGBA(255.f, 153.f, 0.f, 200.f), sfColor_fromRGBA(255.f, 153.f, 0.f, 150.f), 0.5f, 3.f, 3.f, 0.f, 330.f, 330.f, 0.f, 0.3f, sfColor_fromRGBA(255.f, 255.f, 255.f, 200.f), sfColor_fromRGBA(255.f, 255.f, 255.f, 150.f), sfTrue, sfFalse, sfFalse, GetSfTexture("grass_Particles6"), sfFalse, sfFalse, sfFalse, GRASS);
	prt_CreateCircleParticles(_spawnPos, 1, 8, sfColor_fromRGBA(255.f, 153.f, 0.f, 200.f), sfColor_fromRGBA(255.f, 153.f, 0.f, 150.f), 0.5f, 3.f, 3.f, 0.f, 360.f, 360.f, 0.f, 0.3f, sfColor_fromRGBA(255.f, 255.f, 255.f, 200.f), sfColor_fromRGBA(255.f, 255.f, 255.f, 150.f), sfTrue, sfFalse, sfFalse, GetSfTexture("grass_Particles6"), sfFalse, sfFalse, sfFalse, GRASS);
}

void prt_CreateBubbleParticlesInFront(sfVector2f _spawnPos)
{
	int rdmBubble = rand_int(0, 3);
	int rdmBehind = rand_int(0, 2);
	int rdmTime2Die = rand_int(0, 2);
	if (rdmTime2Die == 0)
	{
		if (rdmBehind == 0)
		{
			if (rdmBubble == 0)
				prt_CreateCircleParticles(vector2f(_spawnPos.x + rand_float(-20.f, 20.f), _spawnPos.y), 1, 8, sfColor_fromRGBA(248.f, 242.f, 46.f, 200.f), sfColor_fromRGBA(248.f, 242.f, 46.f, 150.f), 60.f, 5.f, 5.f, 270.f, 270.f, 75.f, 50.f, 0.3f,
					sfColor_fromRGBA(255.f, 255.f, 255.f, 200.f), sfColor_fromRGBA(255.f, 255.f, 255.f, 150.f), sfFalse, sfFalse, sfFalse, GetSfTexture("bubble_Particle1"), sfFalse, sfFalse, sfTrue, BUBBLE);
			if (rdmBubble == 1)
				prt_CreateCircleParticles(vector2f(_spawnPos.x + rand_float(-20.f, 20.f), _spawnPos.y), 1, 8, sfColor_fromRGBA(248.f, 242.f, 46.f, 200.f), sfColor_fromRGBA(248.f, 242.f, 46.f, 150.f), 60.f, 12.f, 12.f, 270.f, 270.f, 75.f, 50.f, 0.3f,
					sfColor_fromRGBA(255.f, 255.f, 255.f, 200.f), sfColor_fromRGBA(255.f, 255.f, 255.f, 150.f), sfFalse, sfFalse, sfFalse, GetSfTexture("bubble_Particle2"), sfFalse, sfFalse, sfTrue, BUBBLE);
			if (rdmBubble == 2)
				prt_CreateCircleParticles(vector2f(_spawnPos.x + rand_float(-20.f, 20.f), _spawnPos.y), 1, 8, sfColor_fromRGBA(248.f, 242.f, 46.f, 200.f), sfColor_fromRGBA(248.f, 242.f, 46.f, 150.f), 60.f, 8.f, 8.f, 270.f, 270.f, 75.f, 50.f, 0.3f,
					sfColor_fromRGBA(255.f, 255.f, 255.f, 200.f), sfColor_fromRGBA(255.f, 255.f, 255.f, 150.f), sfFalse, sfFalse, sfFalse, GetSfTexture("bubble_Particle3"), sfFalse, sfFalse, sfTrue, BUBBLE);
		}
		else
		{
			if (rdmBubble == 0)
				prt_CreateCircleParticles(vector2f(_spawnPos.x + rand_float(-20.f, 20.f), _spawnPos.y), 1, 8, sfColor_fromRGBA(248.f, 242.f, 46.f, 200.f), sfColor_fromRGBA(248.f, 242.f, 46.f, 150.f), 60.f, 5.f, 5.f, 270.f, 270.f, 75.f, 50.f, 0.3f,
					sfColor_fromRGBA(255.f, 255.f, 255.f, 200.f), sfColor_fromRGBA(255.f, 255.f, 255.f, 150.f), sfFalse, sfFalse, sfFalse, GetSfTexture("bubble_Particle1"), sfFalse, sfFalse, sfFalse, BUBBLE);
			if (rdmBubble == 1)
				prt_CreateCircleParticles(vector2f(_spawnPos.x + rand_float(-20.f, 20.f), _spawnPos.y), 1, 8, sfColor_fromRGBA(248.f, 242.f, 46.f, 200.f), sfColor_fromRGBA(248.f, 242.f, 46.f, 150.f), 60.f, 12.f, 12.f, 270.f, 270.f, 75.f, 50.f, 0.3f,
					sfColor_fromRGBA(255.f, 255.f, 255.f, 200.f), sfColor_fromRGBA(255.f, 255.f, 255.f, 150.f), sfFalse, sfFalse, sfFalse, GetSfTexture("bubble_Particle2"), sfFalse, sfFalse, sfFalse, BUBBLE);
			if (rdmBubble == 2)
				prt_CreateCircleParticles(vector2f(_spawnPos.x + rand_float(-20.f, 20.f), _spawnPos.y), 1, 8, sfColor_fromRGBA(248.f, 242.f, 46.f, 200.f), sfColor_fromRGBA(248.f, 242.f, 46.f, 150.f), 60.f, 8.f, 8.f, 270.f, 270.f, 75.f, 50.f, 0.3f,
					sfColor_fromRGBA(255.f, 255.f, 255.f, 200.f), sfColor_fromRGBA(255.f, 255.f, 255.f, 150.f), sfFalse, sfFalse, sfFalse, GetSfTexture("bubble_Particle3"), sfFalse, sfFalse, sfFalse, BUBBLE);
		}
	}
	else {
		if (rdmBehind == 0)
		{
			if (rdmBubble == 0)
				prt_CreateCircleParticles(vector2f(_spawnPos.x + rand_float(-20.f, 20.f), _spawnPos.y), 1, 8, sfColor_fromRGBA(248.f, 242.f, 46.f, 200.f), sfColor_fromRGBA(248.f, 242.f, 46.f, 150.f), rand_float(3.f, 10.f), 5.f, 5.f, 270.f, 270.f, 75.f, 50.f, 0.3f,
					sfColor_fromRGBA(255.f, 255.f, 255.f, 200.f), sfColor_fromRGBA(255.f, 255.f, 255.f, 150.f), sfFalse, sfFalse, sfFalse, GetSfTexture("bubble_Particle1"), sfFalse, sfFalse, sfTrue, BUBBLE);
			if (rdmBubble == 1)
				prt_CreateCircleParticles(vector2f(_spawnPos.x + rand_float(-20.f, 20.f), _spawnPos.y), 1, 8, sfColor_fromRGBA(248.f, 242.f, 46.f, 200.f), sfColor_fromRGBA(248.f, 242.f, 46.f, 150.f), rand_float(3.f, 10.f), 12.f, 12.f, 270.f, 270.f, 75.f, 50.f, 0.3f,
					sfColor_fromRGBA(255.f, 255.f, 255.f, 200.f), sfColor_fromRGBA(255.f, 255.f, 255.f, 150.f), sfFalse, sfFalse, sfFalse, GetSfTexture("bubble_Particle2"), sfFalse, sfFalse, sfTrue, BUBBLE);
			if (rdmBubble == 2)
				prt_CreateCircleParticles(vector2f(_spawnPos.x + rand_float(-20.f, 20.f), _spawnPos.y), 1, 8, sfColor_fromRGBA(248.f, 242.f, 46.f, 200.f), sfColor_fromRGBA(248.f, 242.f, 46.f, 150.f), rand_float(3.f, 10.f), 8.f, 8.f, 270.f, 270.f, 75.f, 50.f, 0.3f,
					sfColor_fromRGBA(255.f, 255.f, 255.f, 200.f), sfColor_fromRGBA(255.f, 255.f, 255.f, 150.f), sfFalse, sfFalse, sfFalse, GetSfTexture("bubble_Particle3"), sfFalse, sfFalse, sfTrue, BUBBLE);
		}
		else
		{
			if (rdmBubble == 0)
				prt_CreateCircleParticles(vector2f(_spawnPos.x + rand_float(-20.f, 20.f), _spawnPos.y), 1, 8, sfColor_fromRGBA(248.f, 242.f, 46.f, 200.f), sfColor_fromRGBA(248.f, 242.f, 46.f, 150.f), rand_float(3.f, 10.f), 5.f, 5.f, 270.f, 270.f, 75.f, 50.f, 0.3f,
					sfColor_fromRGBA(255.f, 255.f, 255.f, 200.f), sfColor_fromRGBA(255.f, 255.f, 255.f, 150.f), sfFalse, sfFalse, sfFalse, GetSfTexture("bubble_Particle1"), sfFalse, sfFalse, sfFalse, BUBBLE);
			if (rdmBubble == 1)
				prt_CreateCircleParticles(vector2f(_spawnPos.x + rand_float(-20.f, 20.f), _spawnPos.y), 1, 8, sfColor_fromRGBA(248.f, 242.f, 46.f, 200.f), sfColor_fromRGBA(248.f, 242.f, 46.f, 150.f), rand_float(3.f, 10.f), 12.f, 12.f, 270.f, 270.f, 75.f, 50.f, 0.3f,
					sfColor_fromRGBA(255.f, 255.f, 255.f, 200.f), sfColor_fromRGBA(255.f, 255.f, 255.f, 150.f), sfFalse, sfFalse, sfFalse, GetSfTexture("bubble_Particle2"), sfFalse, sfFalse, sfFalse, BUBBLE);
			if (rdmBubble == 2)
				prt_CreateCircleParticles(vector2f(_spawnPos.x + rand_float(-20.f, 20.f), _spawnPos.y), 1, 8, sfColor_fromRGBA(248.f, 242.f, 46.f, 200.f), sfColor_fromRGBA(248.f, 242.f, 46.f, 150.f), rand_float(3.f, 10.f), 8.f, 8.f, 270.f, 270.f, 75.f, 50.f, 0.3f,
					sfColor_fromRGBA(255.f, 255.f, 255.f, 200.f), sfColor_fromRGBA(255.f, 255.f, 255.f, 150.f), sfFalse, sfFalse, sfFalse, GetSfTexture("bubble_Particle3"), sfFalse, sfFalse, sfFalse, BUBBLE);
		}
	}
}

void prt_CreateBubbleFishParticles(sfVector2f _spawnPos)
{
	prt_CreateCircleParticles(vector2f(_spawnPos.x + rand_float(-20.f, 20.f), _spawnPos.y), 1, 8, sfColor_fromRGBA(248.f, 242.f, 46.f, 200.f), sfColor_fromRGBA(248.f, 242.f, 46.f, 150.f), 60.f, 5.f, 5.f, 270.f, 270.f, 75.f, 50.f, 0.3f,
		sfColor_fromRGBA(255.f, 255.f, 255.f, 200.f), sfColor_fromRGBA(255.f, 255.f, 255.f, 150.f), sfFalse, sfFalse, sfFalse, GetSfTexture("bubble_Particle1"), sfFalse, sfFalse, sfFalse, BUBBLE);
}

void prt_CreateWaterSplahParticles(sfVector2f _spawnPos, float _angle)
{
	int rdmWater = rand_int(0, 4);
	if (rdmWater == 0)
		prt_CreateCircleParticles(_spawnPos, 1, 8, sfColor_fromRGBA(248.f, 242.f, 46.f, 200.f), sfColor_fromRGBA(248.f, 242.f, 46.f, 150.f), 0.5f, 4.f, 4.f, _angle, _angle, 100.f, 0.f, 0.3f,
			sfColor_fromRGBA(255.f, 255.f, 255.f, 200.f), sfColor_fromRGBA(255.f, 255.f, 255.f, 150.f), sfTrue, sfFalse, sfFalse, GetSfTexture("water_Particle1"), sfFalse, sfFalse, sfFalse, WATER);
	else if (rdmWater == 1)
		prt_CreateCircleParticles(_spawnPos, 1, 8, sfColor_fromRGBA(248.f, 242.f, 46.f, 200.f), sfColor_fromRGBA(248.f, 242.f, 46.f, 150.f), 0.5f, 4.f, 4.f, _angle, _angle, 25.f, 0.f, 0.3f,
			sfColor_fromRGBA(255.f, 255.f, 255.f, 200.f), sfColor_fromRGBA(255.f, 255.f, 255.f, 150.f), sfTrue, sfFalse, sfFalse, GetSfTexture("water_Particle2"), sfFalse, sfFalse, sfFalse, WATER);
	else if (rdmWater == 2)
		prt_CreateCircleParticles(_spawnPos, 1, 8, sfColor_fromRGBA(255.f, 153.f, 0.f, 200.f), sfColor_fromRGBA(255.f, 153.f, 0.f, 150.f), 0.5f, 4.f, 4.f, _angle, _angle, 25.f, 0.f, 0.3f,
			sfColor_fromRGBA(255.f, 255.f, 255.f, 200.f), sfColor_fromRGBA(255.f, 255.f, 255.f, 150.f), sfTrue, sfFalse, sfFalse, GetSfTexture("water_Particle3"), sfFalse, sfFalse, sfFalse, WATER);
	else if (rdmWater == 3)
		prt_CreateCircleParticles(_spawnPos, 1, 8, sfColor_fromRGBA(248.f, 242.f, 46.f, 200.f), sfColor_fromRGBA(248.f, 242.f, 46.f, 150.f), 0.5f, 4.f, 4.f, _angle, _angle, 25.f, 0.f, 0.3f,
			sfColor_fromRGBA(255.f, 255.f, 255.f, 200.f), sfColor_fromRGBA(255.f, 255.f, 255.f, 150.f), sfTrue, sfFalse, sfFalse, GetSfTexture("water_Particle4"), sfFalse, sfFalse, sfFalse, WATER);
	int rdmWater2 = rand_int(0, 4);
	if (rdmWater2 == 0)
		prt_CreateCircleParticles(_spawnPos, 1, 8, sfColor_fromRGBA(248.f, 242.f, 46.f, 200.f), sfColor_fromRGBA(248.f, 242.f, 46.f, 150.f), 0.5f, 4.f, 4.f, _angle, _angle, 100.f, 0.f, 0.3f,
			sfColor_fromRGBA(255.f, 255.f, 255.f, 200.f), sfColor_fromRGBA(255.f, 255.f, 255.f, 150.f), sfTrue, sfFalse, sfFalse, GetSfTexture("water_Particle1"), sfFalse, sfFalse, sfFalse, WATER);
	else if (rdmWater2 == 1)
		prt_CreateCircleParticles(_spawnPos, 1, 8, sfColor_fromRGBA(248.f, 242.f, 46.f, 200.f), sfColor_fromRGBA(248.f, 242.f, 46.f, 150.f), 0.5f, 4.f, 4.f, _angle, _angle, 25.f, 0.f, 0.3f,
			sfColor_fromRGBA(255.f, 255.f, 255.f, 200.f), sfColor_fromRGBA(255.f, 255.f, 255.f, 150.f), sfTrue, sfFalse, sfFalse, GetSfTexture("water_Particle2"), sfFalse, sfFalse, sfFalse, WATER);
	else if (rdmWater2 == 2)
		prt_CreateCircleParticles(_spawnPos, 1, 8, sfColor_fromRGBA(255.f, 153.f, 0.f, 200.f), sfColor_fromRGBA(255.f, 153.f, 0.f, 150.f), 0.5f, 4.f, 4.f, _angle, _angle, 25.f, 0.f, 0.3f,
			sfColor_fromRGBA(255.f, 255.f, 255.f, 200.f), sfColor_fromRGBA(255.f, 255.f, 255.f, 150.f), sfTrue, sfFalse, sfFalse, GetSfTexture("water_Particle3"), sfFalse, sfFalse, sfFalse, WATER);
	else if (rdmWater2 == 3)
		prt_CreateCircleParticles(_spawnPos, 1, 8, sfColor_fromRGBA(248.f, 242.f, 46.f, 200.f), sfColor_fromRGBA(248.f, 242.f, 46.f, 150.f), 0.5f, 4.f, 4.f, _angle, _angle, 25.f, 0.f, 0.3f,
			sfColor_fromRGBA(255.f, 255.f, 255.f, 200.f), sfColor_fromRGBA(255.f, 255.f, 255.f, 150.f), sfTrue, sfFalse, sfFalse, GetSfTexture("water_Particle4"), sfFalse, sfFalse, sfFalse, WATER);
}

void prt_CreateFireParticlesBrazier(sfVector2f _spawnPos)
{
	prt_CreateCircleParticles(_spawnPos, 2, 4, sfColor_fromRGBA(255.f, 102.f, 46.f, 255.f), sfColor_fromRGBA(255.f, 102.f, 46.f, 255.f), 1.5f, 2.f, 1.f, 240.f, 300.f, 75.f, 0.f, 0.f,
		sfWhite, sfWhite, sfFalse, sfTrue, sfFalse, NULL, sfFalse, sfFalse, sfTrue, FIRE);
}

void prt_CreateHeartDropParticles(sfVector2f _spawnPos)
{
	prt_CreateCircleParticles(_spawnPos, 1, 2, sfRed, sfMagenta, 1.5f, 3.f, 1.f, 240.f, 240.f, 75.f, 0.f, 0.f, sfWhite, sfWhite, sfFalse, sfFalse, sfFalse, NULL, sfFalse, sfTrue, sfTrue, prt_HEART);
	prt_CreateCircleParticles(_spawnPos, 1, 2, sfRed, sfMagenta, 1.5f, 3.f, 1.f, 300.f, 300.f, 75.f, 0.f, 0.f, sfWhite, sfWhite, sfFalse, sfFalse, sfFalse, NULL, sfFalse, sfTrue, sfTrue, prt_HEART);
	prt_CreateCircleParticles(_spawnPos, 1, 2, sfRed, sfMagenta, 1.5f, 3.f, 1.f, 270.f, 270.f, 75.f, 0.f, 0.f, sfWhite, sfWhite, sfFalse, sfFalse, sfFalse, NULL, sfFalse, sfTrue, sfTrue, prt_HEART);
	prt_CreateCircleParticles(_spawnPos, 1, 2, sfRed, sfMagenta, 1.5f, 3.f, 1.f, 330.f, 330.f, 75.f, 0.f, 0.f, sfWhite, sfWhite, sfFalse, sfFalse, sfFalse, NULL, sfFalse, sfTrue, sfTrue, prt_HEART);
	prt_CreateCircleParticles(_spawnPos, 1, 2, sfRed, sfMagenta, 1.5f, 3.f, 1.f, 360.f, 360.f, 75.f, 0.f, 0.f, sfWhite, sfWhite, sfFalse, sfFalse, sfFalse, NULL, sfFalse, sfTrue, sfTrue, prt_HEART);
	prt_CreateCircleParticles(_spawnPos, 1, 2, sfRed, sfMagenta, 1.5f, 3.f, 1.f, 210.f, 210.f, 75.f, 0.f, 0.f, sfWhite, sfWhite, sfFalse, sfFalse, sfFalse, NULL, sfFalse, sfTrue, sfTrue, prt_HEART);
	prt_CreateCircleParticles(_spawnPos, 1, 2, sfRed, sfMagenta, 1.5f, 3.f, 1.f, 180.f, 180.f, 75.f, 0.f, 0.f, sfWhite, sfWhite, sfFalse, sfFalse, sfFalse, NULL, sfFalse, sfTrue, sfTrue, prt_HEART);
	prt_CreateCircleParticles(_spawnPos, 1, 2, sfRed, sfMagenta, 1.5f, 3.f, 1.f, 150.f, 150.f, 75.f, 0.f, 0.f, sfWhite, sfWhite, sfFalse, sfFalse, sfFalse, NULL, sfFalse, sfTrue, sfTrue, prt_HEART);
	prt_CreateCircleParticles(_spawnPos, 1, 2, sfRed, sfMagenta, 1.5f, 3.f, 1.f, 120.f, 120.f, 75.f, 0.f, 0.f, sfWhite, sfWhite, sfFalse, sfFalse, sfFalse, NULL, sfFalse, sfTrue, sfTrue, prt_HEART);
	prt_CreateCircleParticles(_spawnPos, 1, 2, sfRed, sfMagenta, 1.5f, 3.f, 1.f, 90.f, 90.f, 75.f, 0.f, 0.f, sfWhite, sfWhite, sfFalse, sfFalse, sfFalse, NULL, sfFalse, sfTrue, sfTrue, prt_HEART);
	prt_CreateCircleParticles(_spawnPos, 1, 2, sfRed, sfMagenta, 1.5f, 3.f, 1.f, 60.f, 60.f, 75.f, 0.f, 0.f, sfWhite, sfWhite, sfFalse, sfFalse, sfFalse, NULL, sfFalse, sfTrue, sfTrue, prt_HEART);
	prt_CreateCircleParticles(_spawnPos, 1, 2, sfRed, sfMagenta, 1.5f, 3.f, 1.f, 30.f, 30.f, 75.f, 0.f, 0.f, sfWhite, sfWhite, sfFalse, sfFalse, sfFalse, NULL, sfFalse, sfTrue, sfTrue, prt_HEART);
}

void prt_CreateDamagePlayerParticles(sfVector2f _spawnPos)
{
	prt_CreateCircleParticles(_spawnPos, 1, 3, sfRed, sfRed, 0.5f, 6.f, 6.f, 240.f, 240.f, 100.f, 1000.f, 0.f, sfWhite, sfWhite, sfFalse, sfFalse, sfFalse, NULL, sfFalse, sfTrue, sfTrue, DAMAGE);
	prt_CreateCircleParticles(_spawnPos, 1, 3, sfRed, sfRed, 0.5f, 6.f, 6.f, 300.f, 300.f, 100.f, 1000.f, 0.f, sfWhite, sfWhite, sfFalse, sfFalse, sfFalse, NULL, sfFalse, sfTrue, sfTrue, DAMAGE);
	prt_CreateCircleParticles(_spawnPos, 1, 3, sfRed, sfRed, 0.5f, 6.f, 6.f, 270.f, 270.f, 100.f, 1000.f, 0.f, sfWhite, sfWhite, sfFalse, sfFalse, sfFalse, NULL, sfFalse, sfTrue, sfTrue, DAMAGE);
	prt_CreateCircleParticles(_spawnPos, 1, 3, sfRed, sfRed, 0.5f, 6.f, 6.f, 330.f, 330.f, 100.f, 1000.f, 0.f, sfWhite, sfWhite, sfFalse, sfFalse, sfFalse, NULL, sfFalse, sfTrue, sfTrue, DAMAGE);
	prt_CreateCircleParticles(_spawnPos, 1, 3, sfRed, sfRed, 0.5f, 6.f, 6.f, 360.f, 360.f, 100.f, 1000.f, 0.f, sfWhite, sfWhite, sfFalse, sfFalse, sfFalse, NULL, sfFalse, sfTrue, sfTrue, DAMAGE);
	prt_CreateCircleParticles(_spawnPos, 1, 3, sfRed, sfRed, 0.5f, 6.f, 6.f, 210.f, 210.f, 100.f, 1000.f, 0.f, sfWhite, sfWhite, sfFalse, sfFalse, sfFalse, NULL, sfFalse, sfTrue, sfTrue, DAMAGE);
	prt_CreateCircleParticles(_spawnPos, 1, 3, sfRed, sfRed, 0.5f, 6.f, 6.f, 180.f, 180.f, 100.f, 1000.f, 0.f, sfWhite, sfWhite, sfFalse, sfFalse, sfFalse, NULL, sfFalse, sfTrue, sfTrue, DAMAGE);
	prt_CreateCircleParticles(_spawnPos, 1, 3, sfRed, sfRed, 0.5f, 6.f, 6.f, 150.f, 150.f, 100.f, 1000.f, 0.f, sfWhite, sfWhite, sfFalse, sfFalse, sfFalse, NULL, sfFalse, sfTrue, sfTrue, DAMAGE);
	prt_CreateCircleParticles(_spawnPos, 1, 3, sfRed, sfRed, 0.5f, 6.f, 6.f, 120.f, 120.f, 100.f, 1000.f, 0.f, sfWhite, sfWhite, sfFalse, sfFalse, sfFalse, NULL, sfFalse, sfTrue, sfTrue, DAMAGE);
	prt_CreateCircleParticles(_spawnPos, 1, 3, sfRed, sfRed, 0.5f, 6.f, 6.f, 90.f, 90.f, 100.f, 1000.f, 0.f, sfWhite, sfWhite, sfFalse, sfFalse, sfFalse, NULL, sfFalse, sfTrue, sfTrue, DAMAGE);
	prt_CreateCircleParticles(_spawnPos, 1, 3, sfRed, sfRed, 0.5f, 6.f, 6.f, 60.f, 60.f, 100.f, 1000.f, 0.f, sfWhite, sfWhite, sfFalse, sfFalse, sfFalse, NULL, sfFalse, sfTrue, sfTrue, DAMAGE);
	prt_CreateCircleParticles(_spawnPos, 1, 3, sfRed, sfRed, 0.5f, 6.f, 6.f, 30.f, 30.f, 100.f, 1000.f, 0.f, sfWhite, sfWhite, sfFalse, sfFalse, sfFalse, NULL, sfFalse, sfTrue, sfTrue, DAMAGE);
}

void prt_CreateTableParticles(sfVector2f _spawnPos)
{
	createAnimatedParticles(_spawnPos, 1, sfGreen, sfCyan, 1.f, GetSfTexture("sparksAttract"), 1.f, 1.f, 0.f, 0.f, 100.f, rand_float(-100.f, 100.f), sfFalse, sfTrue, sfFalse, "sparksAttract", sfTrue, sfFalse);
	createAnimatedParticles(_spawnPos, 1, sfGreen, sfCyan, 1.f, GetSfTexture("sparksAttract"), 1.f, 1.f, 30.f, 30.f, 100.f, rand_float(-100.f, 100.f), sfFalse, sfTrue, sfFalse, "sparksAttract", sfTrue, sfFalse);
	createAnimatedParticles(_spawnPos, 1, sfGreen, sfCyan, 1.f, GetSfTexture("sparksAttract"), 1.f, 1.f, 60.f, 60.f, 100.f, rand_float(-100.f, 100.f), sfFalse, sfTrue, sfFalse, "sparksAttract", sfTrue, sfFalse);
	createAnimatedParticles(_spawnPos, 1, sfGreen, sfCyan, 1.f, GetSfTexture("sparksAttract"), 1.f, 1.f, 90.f, 90.f, 100.f, rand_float(-100.f, 100.f), sfFalse, sfTrue, sfFalse, "sparksAttract", sfTrue, sfFalse);
	createAnimatedParticles(_spawnPos, 1, sfGreen, sfCyan, 1.f, GetSfTexture("sparksAttract"), 1.f, 1.f, 120.f, 120.f, 100.f, rand_float(-100.f, 100.f), sfFalse, sfTrue, sfFalse, "sparksAttract", sfTrue, sfFalse);
	createAnimatedParticles(_spawnPos, 1, sfGreen, sfCyan, 1.f, GetSfTexture("sparksAttract"), 1.f, 1.f, 150.f, 150.f, 100.f, rand_float(-100.f, 100.f), sfFalse, sfTrue, sfFalse, "sparksAttract", sfTrue, sfFalse);
	createAnimatedParticles(_spawnPos, 1, sfGreen, sfCyan, 1.f, GetSfTexture("sparksAttract"), 1.f, 1.f, 180.f, 180.f, 100.f, rand_float(-100.f, 100.f), sfFalse, sfTrue, sfFalse, "sparksAttract", sfTrue, sfFalse);
	createAnimatedParticles(_spawnPos, 1, sfGreen, sfCyan, 1.f, GetSfTexture("sparksAttract"), 1.f, 1.f, 210.f, 210.f, 100.f, rand_float(-100.f, 100.f), sfFalse, sfTrue, sfFalse, "sparksAttract", sfTrue, sfFalse);
	createAnimatedParticles(_spawnPos, 1, sfGreen, sfCyan, 1.f, GetSfTexture("sparksAttract"), 1.f, 1.f, 240.f, 240.f, 100.f, rand_float(-100.f, 100.f), sfFalse, sfTrue, sfFalse, "sparksAttract", sfTrue, sfFalse);
	createAnimatedParticles(_spawnPos, 1, sfGreen, sfCyan, 1.f, GetSfTexture("sparksAttract"), 1.f, 1.f, 270.f, 270.f, 100.f, rand_float(-100.f, 100.f), sfFalse, sfTrue, sfFalse, "sparksAttract", sfTrue, sfFalse);
	createAnimatedParticles(_spawnPos, 1, sfGreen, sfCyan, 1.f, GetSfTexture("sparksAttract"), 1.f, 1.f, 300.f, 300.f, 100.f, rand_float(-100.f, 100.f), sfFalse, sfTrue, sfFalse, "sparksAttract", sfTrue, sfFalse);
	createAnimatedParticles(_spawnPos, 1, sfGreen, sfCyan, 1.f, GetSfTexture("sparksAttract"), 1.f, 1.f, 330.f, 330.f, 100.f, rand_float(-100.f, 100.f), sfFalse, sfTrue, sfFalse, "sparksAttract", sfTrue, sfFalse);
}

void prt_CreateBossDeathParticles(sfVector2f _spawnPos)
{
	createAnimatedParticles(_spawnPos, 1, sfColor_fromRGB(136.f, 85.f, 186.f), sfColor_fromRGB(136.f, 85.f, 186.f), 1.f, GetSfTexture("sparksAttract"), 1.f, 1.f, 0.f, 0.f, 100.f, rand_float(-100.f, 100.f), sfFalse, sfTrue, sfFalse, "sparksAttract", sfTrue, sfFalse);
	createAnimatedParticles(_spawnPos, 1, sfColor_fromRGB(136.f, 85.f, 186.f), sfColor_fromRGB(136.f, 85.f, 186.f), 1.f, GetSfTexture("sparksAttract"), 1.f, 1.f, 30.f, 30.f, 100.f, rand_float(-100.f, 100.f), sfFalse, sfTrue, sfFalse, "sparksAttract", sfTrue, sfFalse);
	createAnimatedParticles(_spawnPos, 1, sfColor_fromRGB(136.f, 85.f, 186.f), sfColor_fromRGB(136.f, 85.f, 186.f), 1.f, GetSfTexture("sparksAttract"), 1.f, 1.f, 60.f, 60.f, 100.f, rand_float(-100.f, 100.f), sfFalse, sfTrue, sfFalse, "sparksAttract", sfTrue, sfFalse);
	createAnimatedParticles(_spawnPos, 1, sfColor_fromRGB(136.f, 85.f, 186.f), sfColor_fromRGB(136.f, 85.f, 186.f), 1.f, GetSfTexture("sparksAttract"), 1.f, 1.f, 90.f, 90.f, 100.f, rand_float(-100.f, 100.f), sfFalse, sfTrue, sfFalse, "sparksAttract", sfTrue, sfFalse);
	createAnimatedParticles(_spawnPos, 1, sfColor_fromRGB(136.f, 85.f, 186.f), sfColor_fromRGB(136.f, 85.f, 186.f), 1.f, GetSfTexture("sparksAttract"), 1.f, 1.f, 120.f, 120.f, 100.f, rand_float(-100.f, 100.f), sfFalse, sfTrue, sfFalse, "sparksAttract", sfTrue, sfFalse);
	createAnimatedParticles(_spawnPos, 1, sfColor_fromRGB(136.f, 85.f, 186.f), sfColor_fromRGB(136.f, 85.f, 186.f), 1.f, GetSfTexture("sparksAttract"), 1.f, 1.f, 150.f, 150.f, 100.f, rand_float(-100.f, 100.f), sfFalse, sfTrue, sfFalse, "sparksAttract", sfTrue, sfFalse);
	createAnimatedParticles(_spawnPos, 1, sfColor_fromRGB(136.f, 85.f, 186.f), sfColor_fromRGB(136.f, 85.f, 186.f), 1.f, GetSfTexture("sparksAttract"), 1.f, 1.f, 180.f, 180.f, 100.f, rand_float(-100.f, 100.f), sfFalse, sfTrue, sfFalse, "sparksAttract", sfTrue, sfFalse);
	createAnimatedParticles(_spawnPos, 1, sfColor_fromRGB(136.f, 85.f, 186.f), sfColor_fromRGB(136.f, 85.f, 186.f), 1.f, GetSfTexture("sparksAttract"), 1.f, 1.f, 210.f, 210.f, 100.f, rand_float(-100.f, 100.f), sfFalse, sfTrue, sfFalse, "sparksAttract", sfTrue, sfFalse);
	createAnimatedParticles(_spawnPos, 1, sfColor_fromRGB(136.f, 85.f, 186.f), sfColor_fromRGB(136.f, 85.f, 186.f), 1.f, GetSfTexture("sparksAttract"), 1.f, 1.f, 240.f, 240.f, 100.f, rand_float(-100.f, 100.f), sfFalse, sfTrue, sfFalse, "sparksAttract", sfTrue, sfFalse);
	createAnimatedParticles(_spawnPos, 1, sfColor_fromRGB(136.f, 85.f, 186.f), sfColor_fromRGB(136.f, 85.f, 186.f), 1.f, GetSfTexture("sparksAttract"), 1.f, 1.f, 270.f, 270.f, 100.f, rand_float(-100.f, 100.f), sfFalse, sfTrue, sfFalse, "sparksAttract", sfTrue, sfFalse);
	createAnimatedParticles(_spawnPos, 1, sfColor_fromRGB(136.f, 85.f, 186.f), sfColor_fromRGB(136.f, 85.f, 186.f), 1.f, GetSfTexture("sparksAttract"), 1.f, 1.f, 300.f, 300.f, 100.f, rand_float(-100.f, 100.f), sfFalse, sfTrue, sfFalse, "sparksAttract", sfTrue, sfFalse);
	createAnimatedParticles(_spawnPos, 1, sfColor_fromRGB(136.f, 85.f, 186.f), sfColor_fromRGB(136.f, 85.f, 186.f), 1.f, GetSfTexture("sparksAttract"), 1.f, 1.f, 330.f, 330.f, 100.f, rand_float(-100.f, 100.f), sfFalse, sfTrue, sfFalse, "sparksAttract", sfTrue, sfFalse);
}

void prt_CreateFireParticles(sfVector2f _spawnPos)
{
	prt_CreateCircleParticles(_spawnPos, 3, 4, sfRed, sfYellow, 0.75f, 4.f, 1.f, 240.f, 300.f, 75.f, 0.f, 0.f, sfWhite, sfWhite, sfFalse, sfTrue, sfFalse, NULL, sfFalse, sfFalse, sfTrue, FIRE);
	prt_CreateCircleParticles(_spawnPos, 2, 4, sfColor_fromRGBA(255.f, 162.f, 46.f, 255.f), sfColor_fromRGBA(255.f, 162.f, 46.f, 255.f), 0.5f, 2.f, 1.f, 240.f, 300.f, 75.f, 0.f, 0.f, sfWhite, sfWhite, sfFalse, sfTrue, sfFalse, NULL, sfFalse, sfFalse, sfTrue, FIRE);
}

void prt_UpdateParticles()
{
	if (getIsFreeingMap()) return;

	float dt = getDeltaTime();

	for (int i = 0; i < particlesList->size(particlesList); i++)
	{
		if (getIsFreeingMap()) return;
		GETPARTICLEDATA->timer += dt;
		GETPARTICLEDATA->timerFirefly += dt;

		float tea = GETPARTICLEDATA->timer / GETPARTICLEDATA->timeToDie;
		float size = lerp(GETPARTICLEDATA->radiusStart, GETPARTICLEDATA->radiusEnd, tea);
		sfVector2f rectSize = vector2f(lerp(GETPARTICLEDATA->sizeStart.x, GETPARTICLEDATA->sizeEnd.x, tea), lerp(GETPARTICLEDATA->sizeStart.y, GETPARTICLEDATA->sizeEnd.y, tea));
		if (GETPARTICLEDATA->isGenerative && tea > 0.5f && !GETPARTICLEDATA->hasGenerate)
		{
			createParticles(GETPARTICLEDATA->pos, 20, GETPARTICLEDATA->colorStart, GETPARTICLEDATA->colorEnd, GETPARTICLEDATA->timeToDie / 2.f, GETPARTICLEDATA->texture, size / 1.5f,
				GETPARTICLEDATA->radiusEnd / 2.f, 0.f, 360.f, GETPARTICLEDATA->velocity * 2.f, GETPARTICLEDATA->rotateVelocity * 2.f, GETPARTICLEDATA->outlineThickness, GETPARTICLEDATA->outlineColorStart,
				GETPARTICLEDATA->outlineColorEnd, GETPARTICLEDATA->particleType, GETPARTICLEDATA->gravity, GETPARTICLEDATA->levitation, sfFalse, GETPARTICLEDATA->isSpriteRecolored, GETPARTICLEDATA->countPoint,
				vector2f(rectSize.x / 1.5, rectSize.y / 1.5), vector2f(GETPARTICLEDATA->sizeEnd.x / 2.f, GETPARTICLEDATA->sizeEnd.y / 2.f), GETPARTICLEDATA->hasFriction, GETPARTICLEDATA->isInFront, GETPARTICLEDATA->wichParticles);
			GETPARTICLEDATA->hasGenerate = sfTrue;
		}
		//Gravity test
		if (GETPARTICLEDATA->gravity)
			GETPARTICLEDATA->forward.y += 2.f * dt;
		if (GETPARTICLEDATA->levitation)
			GETPARTICLEDATA->forward.y -= 2.f * dt;

		if (GETPARTICLEDATA->hasFriction)
			multiplyVectorsf(&GETPARTICLEDATA->forward, pow(0.1, dt));

		if (GETPARTICLEDATA->wichParticles == FIREFLY)
		{
			if (GETPARTICLEDATA->timerFirefly < 5.5f && GETPARTICLEDATA->timerFirefly > 4.5f)
				GETPARTICLEDATA->forward.x = 0.f;
			if (GETPARTICLEDATA->timerFirefly > 9.5f || GETPARTICLEDATA->timerFirefly < 0.5f)
				GETPARTICLEDATA->forward.x = 0.f;
			if (GETPARTICLEDATA->timerFirefly > 10.f)
				GETPARTICLEDATA->timerFirefly = 0.f;
			else if (GETPARTICLEDATA->timerFirefly < 5.f)
				GETPARTICLEDATA->forward.x = 0.125f;
			else if (GETPARTICLEDATA->timerFirefly > 5.f)
				GETPARTICLEDATA->forward.x = -0.125f;
		}

		if (GETPARTICLEDATA->wichParticles == BUBBLE)
		{
			if (GETPARTICLEDATA->pos.y < 170.f)
			{
				particlesList->erase(&particlesList, i);
				continue;
			}
		}
		GETPARTICLEDATA->pos = addVectorsrf(GETPARTICLEDATA->pos, multiplyVectorsrf(GETPARTICLEDATA->forward, GETPARTICLEDATA->velocity * dt));

		GETPARTICLEDATA->teta += GETPARTICLEDATA->rotateVelocity * dt;

		if (GETPARTICLEDATA->timer > GETPARTICLEDATA->timeToDie)
		{
			particlesList->erase(&particlesList, i);
		}
	}
}

void prt_DisplayParticlesBehind(Window* _window)
{

	float dt = getDeltaTime();
	if (getIsFreeingMap()) return;
	for (int i = 0; i < particlesList->size(particlesList); i++)
	{
		if (getIsFreeingMap()) return;
		if (!GETPARTICLEDATA->isInFront)
		{
			float tea = GETPARTICLEDATA->timer / GETPARTICLEDATA->timeToDie;
			sfColor partColor = color_lerp_no_opacity(GETPARTICLEDATA->colorStart, GETPARTICLEDATA->colorEnd, tea);
			sfColor partOutlineColor = color_lerp_no_opacity(GETPARTICLEDATA->outlineColorStart, GETPARTICLEDATA->outlineColorEnd, tea);
			float size = lerp(GETPARTICLEDATA->radiusStart, GETPARTICLEDATA->radiusEnd, tea);
			sfVector2f rectSize = vector2f(lerp(GETPARTICLEDATA->sizeStart.x, GETPARTICLEDATA->sizeEnd.x, tea), lerp(GETPARTICLEDATA->sizeStart.y, GETPARTICLEDATA->sizeEnd.y, tea));

			if (GETPARTICLEDATA->isSpriteRecolored && GETPARTICLEDATA->texture != NULL) {
				if (sfShader_isAvailable()) {
					sfShader_setTextureUniform(shaderParticles, "currentTexture", GETPARTICLEDATA->texture);
					sfVector3f vec3 = { partColor.r / 255.f, partColor.g / 255.f, partColor.b / 255.f };
					sfShader_setVec3Uniform(shaderParticles, "currentColor", vec3);
					sfShader_setFloatUniform(shaderParticles, "a", lerp(0.f, 1.f, (float)partColor.a / 255.f));
				}
			}
			if (GETPARTICLEDATA->wichParticles == FIREFLY)
			{
				sfShader* shaderLights = GetShader("light");
				if (shaderLights)
				{
					sfShader_setVec2Uniform(shaderLights, "u_pos", GETPARTICLEDATA->pos);
					sfShader_setVec2Uniform(shaderLights, "u_vpos", mainView->viewPos);
					//printf("posLight : %.2f / %.2f\n", posLightScreen.x, posLightScreen.y);
					sfVector3f vec3 = { partColor.r / 255.f, partColor.g / 255.f, partColor.b / 255.f };
					sfShader_setVec2Uniform(shaderLights, "u_size", rectSize);
					sfShader_setVec3Uniform(shaderLights, "u_color", vec3);
					sfShader_setTextureUniform(shaderLights, "u_texture", sfRenderTexture_getTexture(_window->rdrt));
					sfShader_setIntUniform(shaderLights, "u_lightForm", CIRCLE);
					sfShader_setFloatUniform(shaderLights, "u_angle", 360.f);
					sfShader_setFloatUniform(shaderLights, "u_rotation", 0.f);
					sfShader_setFloatUniform(shaderLights, "u_strength", 50.f);
					sfShader_setVec2Uniform(shaderLights, "u_vsize", mainView->size);
				}
			}


			if (GETPARTICLEDATA->particleType == CIRCLE) {
				if (GETPARTICLEDATA->countPoint >= 3)
					sfCircleShape_setPointCount(circleShpParticles, GETPARTICLEDATA->countPoint);
				sfCircleShape_setFillColor(circleShpParticles, sfWhite);
				sfCircleShape_setRadius(circleShpParticles, size);
				sfCircleShape_setPosition(circleShpParticles, GETPARTICLEDATA->pos);
				sfCircleShape_setOrigin(circleShpParticles, vector2f(size, size));
				sfCircleShape_setRotation(circleShpParticles, GETPARTICLEDATA->teta);
				sfCircleShape_setOutlineThickness(circleShpParticles, GETPARTICLEDATA->outlineThickness);
				sfCircleShape_setOutlineColor(circleShpParticles, partOutlineColor);
				if (GETPARTICLEDATA->texture == NULL) {
					sfCircleShape_setFillColor(circleShpParticles, partColor);
					sfCircleShape_setTexture(circleShpParticles, NULL, sfTrue);
					sfRenderTexture_drawCircleShape(_window->rdrt, circleShpParticles, NULL);
				}

				else {
					sfCircleShape_setTexture(circleShpParticles, GETPARTICLEDATA->texture, sfTrue);
					sfCircleShape_setOutlineThickness(circleShpParticles, 0.f);
					if (GETPARTICLEDATA->isSpriteRecolored)
						sfRenderTexture_drawCircleShape(_window->rdrt, circleShpParticles, &renderStateParticles);
					else if (GETPARTICLEDATA->wichParticles == FIREFLY)
						sfRenderTexture_drawCircleShape(_window->rdrt, circleShpParticles, GetRenderState("light"));
					else if (GETPARTICLEDATA->wichParticles == BUBBLE)
						sfRenderTexture_drawCircleShape(_window->rdrtShader, circleShpParticles, NULL);
					else
						sfRenderTexture_drawCircleShape(_window->rdrt, circleShpParticles, NULL);
				}
			}
			else if (GETPARTICLEDATA->particleType == SQUARE) {
				sfRectangleShape_setSize(rectParticles, rectSize);
				sfRectangleShape_setOrigin(rectParticles, vector2f(rectSize.x / 2.f, rectSize.y / 2.f));
				sfRectangleShape_setFillColor(rectParticles, sfWhite);
				sfRectangleShape_setRotation(rectParticles, GETPARTICLEDATA->teta);
				sfRectangleShape_setPosition(rectParticles, GETPARTICLEDATA->pos);
				sfRectangleShape_setOutlineThickness(rectParticles, GETPARTICLEDATA->outlineThickness);
				sfRectangleShape_setOutlineColor(rectParticles, partOutlineColor);
				if (GETPARTICLEDATA->texture == NULL) {
					sfRectangleShape_setFillColor(rectParticles, partColor);
					sfRectangleShape_setTexture(rectParticles, NULL, sfFalse);
					sfRenderTexture_drawRectangleShape(_window->rdrt, rectParticles, NULL);
				}
				else {
					sfRectangleShape_setOutlineThickness(rectParticles, 0.f);
					sfRectangleShape_setTexture(rectParticles, GETPARTICLEDATA->texture, sfFalse);
					if (GETPARTICLEDATA->isSpriteRecolored)
						sfRenderTexture_drawRectangleShape(_window->rdrt, rectParticles, &renderStateParticles);
					else if (GETPARTICLEDATA->wichParticles == FIREFLY)
						sfRenderTexture_drawRectangleShape(_window->rdrt, rectParticles, GetRenderState("light"));
					else if (GETPARTICLEDATA->wichParticles == BUBBLE)
						sfRenderTexture_drawRectangleShape(_window->rdrtShader, rectParticles, NULL);
					else
						sfRenderTexture_drawRectangleShape(_window->rdrt, rectParticles, NULL);
				}
			}
			else if (GETPARTICLEDATA->particleType == ANIMATION_PARTICLE) {
				sfIntRect rect;
				int frameXMax;
				GetParticleAnimationDatas(GETPARTICLEDATA->animName, &frameXMax, &rect);
				GETPARTICLEDATA->animTimer += dt;
				if (GETPARTICLEDATA->animTimer > GETPARTICLEDATA->timeToDie / (frameXMax + 1))
				{
					GETPARTICLEDATA->animTimer = 0.f;
					GETPARTICLEDATA->frameX++;
				}

				sfRectangleShape_setSize(rectParticles, vector2f(rect.width, rect.height));
				sfRectangleShape_setScale(rectParticles, rectSize);
				sfRectangleShape_setOrigin(rectParticles, vector2f(rect.width / 2.f, rect.height / 2.f));
				sfRectangleShape_setFillColor(rectParticles, partColor);
				sfRectangleShape_setRotation(rectParticles, GETPARTICLEDATA->teta);
				sfRectangleShape_setPosition(rectParticles, GETPARTICLEDATA->pos);
				sfRectangleShape_setOutlineThickness(rectParticles, 0.f);

				sfRectangleShape_setTexture(rectParticles, GETPARTICLEDATA->texture, sfFalse);
				if (frameXMax != 0)
					rect.left = rect.width * GETPARTICLEDATA->frameX;
				sfRectangleShape_setTextureRect(rectParticles, rect);
				if (GETPARTICLEDATA->isSpriteRecolored)
					sfRenderTexture_drawRectangleShape(_window->rdrt, rectParticles, &renderStateParticles);
				else if (GETPARTICLEDATA->wichParticles == FIREFLY)
					sfRenderTexture_drawRectangleShape(_window->rdrt, rectParticles, GetRenderState("light"));
				else if (GETPARTICLEDATA->wichParticles == BUBBLE)
					sfRenderTexture_drawRectangleShape(_window->rdrtShader, rectParticles, NULL);
				else
					sfRenderTexture_drawRectangleShape(_window->rdrt, rectParticles, NULL);
			}
		}
	}
}

void prt_DisplayParticlesInFront(Window* _window)
{

	float dt = getDeltaTime();

	for (int i = 0; i < particlesList->size(particlesList); i++)
	{
		if (getIsFreeingMap()) return;
		if (GETPARTICLEDATA->isInFront)
		{
			float tea = GETPARTICLEDATA->timer / GETPARTICLEDATA->timeToDie;
			sfColor partColor = color_lerp_no_opacity(GETPARTICLEDATA->colorStart, GETPARTICLEDATA->colorEnd, tea);
			sfColor partOutlineColor = color_lerp_no_opacity(GETPARTICLEDATA->outlineColorStart, GETPARTICLEDATA->outlineColorEnd, tea);
			float size = lerp(GETPARTICLEDATA->radiusStart, GETPARTICLEDATA->radiusEnd, tea);
			sfVector2f rectSize = vector2f(lerp(GETPARTICLEDATA->sizeStart.x, GETPARTICLEDATA->sizeEnd.x, tea), lerp(GETPARTICLEDATA->sizeStart.y, GETPARTICLEDATA->sizeEnd.y, tea));

			if (GETPARTICLEDATA->isSpriteRecolored && GETPARTICLEDATA->texture != NULL) {
				if (sfShader_isAvailable()) {
					sfShader_setTextureUniform(shaderParticles, "currentTexture", GETPARTICLEDATA->texture);
					sfVector3f vec3 = { partColor.r / 255.f, partColor.g / 255.f, partColor.b / 255.f };
					sfShader_setVec3Uniform(shaderParticles, "currentColor", vec3);
					sfShader_setFloatUniform(shaderParticles, "a", lerp(0.f, 1.f, (float)partColor.a / 255.f));
				}
			}
			if (GETPARTICLEDATA->wichParticles == FIREFLY)
			{
				sfShader* shaderLights = GetShader("light");
				if (shaderLights)
				{
					sfShader_setVec2Uniform(shaderLights, "u_pos", GETPARTICLEDATA->pos);
					sfShader_setVec2Uniform(shaderLights, "u_vpos", mainView->viewPos);
					//printf("posLight : %.2f / %.2f\n", posLightScreen.x, posLightScreen.y);
					sfVector3f vec3 = { partColor.r / 255.f, partColor.g / 255.f, partColor.b / 255.f };
					sfShader_setVec2Uniform(shaderLights, "u_size", rectSize);
					sfShader_setVec3Uniform(shaderLights, "u_color", vec3);
					sfShader_setTextureUniform(shaderLights, "u_texture", sfRenderTexture_getTexture(_window->rdrt));
					sfShader_setIntUniform(shaderLights, "u_lightForm", CIRCLE);
					sfShader_setFloatUniform(shaderLights, "u_angle", 360.f);
					sfShader_setFloatUniform(shaderLights, "u_rotation", 0.f);
					sfShader_setFloatUniform(shaderLights, "u_strength", 50.f);
					sfShader_setVec2Uniform(shaderLights, "u_vsize", mainView->size);
				}
			}


			if (GETPARTICLEDATA->particleType == CIRCLE) {
				if (GETPARTICLEDATA->countPoint >= 3)
					sfCircleShape_setPointCount(circleShpParticles, GETPARTICLEDATA->countPoint);
				sfCircleShape_setFillColor(circleShpParticles, sfWhite);
				sfCircleShape_setRadius(circleShpParticles, size);
				sfCircleShape_setPosition(circleShpParticles, GETPARTICLEDATA->pos);
				sfCircleShape_setOrigin(circleShpParticles, vector2f(size, size));
				sfCircleShape_setRotation(circleShpParticles, GETPARTICLEDATA->teta);
				sfCircleShape_setOutlineThickness(circleShpParticles, GETPARTICLEDATA->outlineThickness);
				sfCircleShape_setOutlineColor(circleShpParticles, partOutlineColor);
				if (GETPARTICLEDATA->texture == NULL) {
					sfCircleShape_setFillColor(circleShpParticles, partColor);
					sfCircleShape_setTexture(circleShpParticles, NULL, sfTrue);
					sfRenderTexture_drawCircleShape(_window->rdrt, circleShpParticles, NULL);
				}
				else {
					sfCircleShape_setTexture(circleShpParticles, GETPARTICLEDATA->texture, sfTrue);
					sfCircleShape_setOutlineThickness(circleShpParticles, 0.f);
					if (GETPARTICLEDATA->isSpriteRecolored)
						sfRenderTexture_drawCircleShape(_window->rdrt, circleShpParticles, &renderStateParticles);
					else if (GETPARTICLEDATA->wichParticles == FIREFLY)
						sfRenderTexture_drawCircleShape(_window->rdrt, circleShpParticles, GetRenderState("light"));
					else if (GETPARTICLEDATA->wichParticles == BUBBLE)
						sfRenderTexture_drawCircleShape(_window->rdrtShader, circleShpParticles, NULL);
					else
						sfRenderTexture_drawCircleShape(_window->rdrt, circleShpParticles, NULL);
				}
			}
			else if (GETPARTICLEDATA->particleType == SQUARE) {
				sfRectangleShape_setSize(rectParticles, rectSize);
				sfRectangleShape_setOrigin(rectParticles, vector2f(rectSize.x / 2.f, rectSize.y / 2.f));
				sfRectangleShape_setFillColor(rectParticles, sfWhite);
				sfRectangleShape_setRotation(rectParticles, GETPARTICLEDATA->teta);
				sfRectangleShape_setPosition(rectParticles, GETPARTICLEDATA->pos);
				sfRectangleShape_setOutlineThickness(rectParticles, GETPARTICLEDATA->outlineThickness);
				sfRectangleShape_setOutlineColor(rectParticles, partOutlineColor);
				if (GETPARTICLEDATA->texture == NULL) {
					sfRectangleShape_setFillColor(rectParticles, partColor);
					sfRectangleShape_setTexture(rectParticles, NULL, sfFalse);
					sfRenderTexture_drawRectangleShape(_window->rdrt, rectParticles, NULL);
				}
				else {
					sfRectangleShape_setOutlineThickness(rectParticles, 0.f);
					sfRectangleShape_setTexture(rectParticles, GETPARTICLEDATA->texture, sfFalse);
					if (GETPARTICLEDATA->isSpriteRecolored)
						sfRenderTexture_drawRectangleShape(_window->rdrt, rectParticles, &renderStateParticles);
					else if (GETPARTICLEDATA->wichParticles == FIREFLY)
						sfRenderTexture_drawRectangleShape(_window->rdrt, rectParticles, GetRenderState("light"));
					else if (GETPARTICLEDATA->wichParticles == BUBBLE)
						sfRenderTexture_drawRectangleShape(_window->rdrtShader, rectParticles, NULL);
					else
						sfRenderTexture_drawRectangleShape(_window->rdrt, rectParticles, NULL);
				}
			}
			else if (GETPARTICLEDATA->particleType == ANIMATION_PARTICLE) {
				sfIntRect rect;
				int frameXMax;
				GetParticleAnimationDatas(GETPARTICLEDATA->animName, &frameXMax, &rect);
				GETPARTICLEDATA->animTimer += dt;
				if (GETPARTICLEDATA->animTimer > GETPARTICLEDATA->timeToDie / (frameXMax + 1))
				{
					GETPARTICLEDATA->animTimer = 0.f;
					GETPARTICLEDATA->frameX++;
				}

				sfRectangleShape_setSize(rectParticles, vector2f(rect.width, rect.height));
				sfRectangleShape_setScale(rectParticles, rectSize);
				sfRectangleShape_setOrigin(rectParticles, vector2f(rect.width / 2.f, rect.height / 2.f));
				sfRectangleShape_setFillColor(rectParticles, partColor);
				sfRectangleShape_setRotation(rectParticles, GETPARTICLEDATA->teta);
				sfRectangleShape_setPosition(rectParticles, GETPARTICLEDATA->pos);
				sfRectangleShape_setOutlineThickness(rectParticles, 0.f);

				sfRectangleShape_setTexture(rectParticles, GETPARTICLEDATA->texture, sfFalse);
				if (frameXMax != 0)
					rect.left = rect.width * GETPARTICLEDATA->frameX;
				sfRectangleShape_setTextureRect(rectParticles, rect);
				if (GETPARTICLEDATA->isSpriteRecolored)
					sfRenderTexture_drawRectangleShape(_window->rdrt, rectParticles, &renderStateParticles);
				else if (GETPARTICLEDATA->wichParticles == FIREFLY)
					sfRenderTexture_drawRectangleShape(_window->rdrt, rectParticles, GetRenderState("light"));
				else if (GETPARTICLEDATA->wichParticles == BUBBLE)
					sfRenderTexture_drawRectangleShape(_window->rdrtShader, rectParticles, NULL);
				else
					sfRenderTexture_drawRectangleShape(_window->rdrt, rectParticles, NULL);
			}
		}
	}
}

void createAnimatedParticles(sfVector2f _spawnPos, int _particlesAmount, sfColor _colorStart, sfColor _colorEnd, float _timeToDie, sfTexture* _texture, float _scaleStart, float _scaleEnd,
	float _angleMin, float _angleMax, float _velocity, float _rotateVelocity, sfBool _hasGravity, sfBool _isSpriteRecolored, sfBool _hasFriction, char* _animationName, sfBool _isInFront, sfBool _levitation)
{
	for (int i = 0; i < _particlesAmount; i++)
	{
		particles tempParticle = { 0 };
		tempParticle.colorStart = _colorStart;
		tempParticle.colorEnd = _colorEnd;
		tempParticle.radiusStart = _scaleStart;
		tempParticle.radiusEnd = _scaleEnd;
		tempParticle.timer = 0.f;
		tempParticle.teta = 0.f;
		tempParticle.texture = _texture;
		tempParticle.pos = _spawnPos;
		tempParticle.forward = polarCoord(vector2f(0.f, 0.f), 1.f, rand_float(_angleMin, _angleMax) * DEG2RAD);
		tempParticle.timeToDie = _timeToDie;
		tempParticle.velocity = _velocity;
		tempParticle.rotateVelocity = _rotateVelocity;
		tempParticle.particleType = ANIMATION_PARTICLE;
		tempParticle.gravity = _hasGravity;
		tempParticle.levitation = _levitation;
		tempParticle.isGenerative = sfFalse;
		tempParticle.hasGenerate = sfFalse;
		tempParticle.timerSpawn = 0.f;
		tempParticle.outlineThickness = 0;
		tempParticle.outlineColorStart = sfTransparent;
		tempParticle.outlineColorEnd = sfTransparent;
		tempParticle.isSpriteRecolored = _isSpriteRecolored;
		tempParticle.countPoint = 4;
		tempParticle.sizeStart = vector2f(_scaleStart, _scaleStart);
		tempParticle.sizeEnd = vector2f(_scaleEnd, _scaleEnd);
		tempParticle.hasFriction = _hasFriction;
		tempParticle.animName = _animationName;
		tempParticle.frameX = 0;
		tempParticle.animTimer = 0.f;
		tempParticle.isInFront = _isInFront;

		particlesList->push_back(&particlesList, &tempParticle);
	}
}

void prt_clear()
{
	if (!particlesList) return;
	particlesList->clear(&particlesList);
}

void prt_CreateStunParticles(sfVector2f _spawnpoint)
{
	createAnimatedParticles(_spawnpoint, 3, sfColor_fromRGBA(255.f, 0.f, 0.f, 255.f), sfColor_fromRGBA(255.f, 0.f, 0.f, 255.f), 1.f, GetSfTexture("mob_stun"),
		0.75f, 0.f, 0.f, 360.f, 50.f, 50.f, sfFalse, sfTrue, sfFalse, "mob_stun", sfFalse, sfFalse);
}

void prt_CreateHealParticles(sfVector2f _spawnPos)
{
	createAnimatedParticles(_spawnPos, 1, sfColor_fromRGB(0.f, 150.f, 0.f), sfGreen, 0.25f, GetSfTexture("sparksAttract"), 1.f, 1.f, 0.f, 0.f, 100.f, rand_float(-100.f, 100.f), sfFalse, sfTrue, sfFalse, "sparksAttract", sfTrue, sfFalse);
	createAnimatedParticles(_spawnPos, 1, sfColor_fromRGB(0.f, 150.f, 0.f), sfGreen, 0.25f, GetSfTexture("sparksAttract"), 1.f, 1.f, 30.f, 30.f, 100.f, rand_float(-100.f, 100.f), sfFalse, sfTrue, sfFalse, "sparksAttract", sfTrue, sfFalse);
	createAnimatedParticles(_spawnPos, 1, sfColor_fromRGB(0.f, 150.f, 0.f), sfGreen, 0.25f, GetSfTexture("sparksAttract"), 1.f, 1.f, 60.f, 60.f, 100.f, rand_float(-100.f, 100.f), sfFalse, sfTrue, sfFalse, "sparksAttract", sfTrue, sfFalse);
	createAnimatedParticles(_spawnPos, 1, sfColor_fromRGB(0.f, 150.f, 0.f), sfGreen, 0.25f, GetSfTexture("sparksAttract"), 1.f, 1.f, 90.f, 90.f, 100.f, rand_float(-100.f, 100.f), sfFalse, sfTrue, sfFalse, "sparksAttract", sfTrue, sfFalse);
	createAnimatedParticles(_spawnPos, 1, sfColor_fromRGB(0.f, 150.f, 0.f), sfGreen, 0.25f, GetSfTexture("sparksAttract"), 1.f, 1.f, 120.f, 120.f, 100.f, rand_float(-100.f, 100.f), sfFalse, sfTrue, sfFalse, "sparksAttract", sfTrue, sfFalse);
	createAnimatedParticles(_spawnPos, 1, sfColor_fromRGB(0.f, 150.f, 0.f), sfGreen, 0.25f, GetSfTexture("sparksAttract"), 1.f, 1.f, 150.f, 150.f, 100.f, rand_float(-100.f, 100.f), sfFalse, sfTrue, sfFalse, "sparksAttract", sfTrue, sfFalse);
	createAnimatedParticles(_spawnPos, 1, sfColor_fromRGB(0.f, 150.f, 0.f), sfGreen, 0.25f, GetSfTexture("sparksAttract"), 1.f, 1.f, 180.f, 180.f, 100.f, rand_float(-100.f, 100.f), sfFalse, sfTrue, sfFalse, "sparksAttract", sfTrue, sfFalse);
	createAnimatedParticles(_spawnPos, 1, sfColor_fromRGB(0.f, 150.f, 0.f), sfGreen, 0.25f, GetSfTexture("sparksAttract"), 1.f, 1.f, 210.f, 210.f, 100.f, rand_float(-100.f, 100.f), sfFalse, sfTrue, sfFalse, "sparksAttract", sfTrue, sfFalse);
	createAnimatedParticles(_spawnPos, 1, sfColor_fromRGB(0.f, 150.f, 0.f), sfGreen, 0.25f, GetSfTexture("sparksAttract"), 1.f, 1.f, 240.f, 240.f, 100.f, rand_float(-100.f, 100.f), sfFalse, sfTrue, sfFalse, "sparksAttract", sfTrue, sfFalse);
	createAnimatedParticles(_spawnPos, 1, sfColor_fromRGB(0.f, 150.f, 0.f), sfGreen, 0.25f, GetSfTexture("sparksAttract"), 1.f, 1.f, 270.f, 270.f, 100.f, rand_float(-100.f, 100.f), sfFalse, sfTrue, sfFalse, "sparksAttract", sfTrue, sfFalse);
	createAnimatedParticles(_spawnPos, 1, sfColor_fromRGB(0.f, 150.f, 0.f), sfGreen, 0.25f, GetSfTexture("sparksAttract"), 1.f, 1.f, 300.f, 300.f, 100.f, rand_float(-100.f, 100.f), sfFalse, sfTrue, sfFalse, "sparksAttract", sfTrue, sfFalse);
	createAnimatedParticles(_spawnPos, 1, sfColor_fromRGB(0.f, 150.f, 0.f), sfGreen, 0.25f, GetSfTexture("sparksAttract"), 1.f, 1.f, 330.f, 330.f, 100.f, rand_float(-100.f, 100.f), sfFalse, sfTrue, sfFalse, "sparksAttract", sfTrue, sfFalse);
}