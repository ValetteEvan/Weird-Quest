#include "bullets.h"
#include "math.h"
#include "player.h"
#include "collisionManager.h"
#include "game.h"
#include "textureManager.h"
#include "viewManager.h"

#define GT_BULLETS STD_LIST_GETDATA(bulletsList, Bullets_struct, i)

sfRectangleShape* blt_rectangle;
sfTexture* blt_texture;

void blt_initBullets()
{
	blt_texture = GetSfTexture("moleBullets");
	blt_rectangle = sfRectangleShape_create();
	sfRectangleShape_setSize(blt_rectangle, vector2f(32.f, 32.f));
	sfRectangleShape_setOrigin(blt_rectangle, vector2f(32.0f / 2.0f, 32.0f / 2.0f));
	sfRectangleShape_setTexture(blt_rectangle, blt_texture, sfTrue);
	bulletsList = STD_LIST_CREATE(Bullets_struct, 0);
}

void blt_addBullets(sfVector2f _position, sfVector2f _forward, TypeBulletsEnum _type)
{
	Bullets_struct tmp = { 0 };

	tmp.colRect = FlRect(0.0f, 0.0f, 0.0f, 0.0f);
	tmp.forward = _forward;
	tmp.position = addVectorsrf(_position, multiplyVectorsrf(_forward, 45.0f));
	tmp.isBack = sfFalse;
	tmp.type = _type;
	tmp.rotation = 0.0f;
	switch (_type)
	{
	case BLT_DIRT:
		blt_texture = GetSfTexture("moleBullets");
		sfRectangleShape_setTexture(blt_rectangle, blt_texture, sfTrue);
		tmp.velocity = 200.0f;
		tmp.rect = IntRect(0,520,40,36);
		tmp.isHoming = sfFalse;
		break;
	case BLT_STONE:
		blt_texture = GetSfTexture("moleBullets");
		sfRectangleShape_setTexture(blt_rectangle, blt_texture, sfTrue);
		tmp.velocity = 350.0f;
		tmp.rect = IntRect(0, 564, 42, 34);
		tmp.isHoming = sfFalse;
		break; 
	case BLT_CRYSTAL:
		blt_texture = GetSfTexture("moleBullets");
		sfRectangleShape_setTexture(blt_rectangle, blt_texture, sfTrue);
		tmp.velocity = 500.0f;
		tmp.rect = IntRect(0, 598, 40, 36);
		tmp.isHoming = sfFalse;
		break;
	case BLT_ENERGY:
		blt_texture = GetSfTexture("moleBullets");
		sfRectangleShape_setTexture(blt_rectangle, blt_texture, sfTrue);
		tmp.velocity = 600.0f;
		tmp.rect = IntRect(0, 598, 40, 36);
		tmp.isHoming = sfTrue;
		break;
	default:
		break;
	}

	bulletsList->push_back(&bulletsList, &tmp);

}

void blt_updateBullets(sfVector2f _playerPos)
{
	float delta = getDeltaTimeThread2();
	for (int i = 0; i < bulletsList->size(bulletsList); i++)
	{
		if (GT_BULLETS->isHoming)
		{
			GT_BULLETS->forward = normalizef(createVectorWithVectorf(GT_BULLETS->position, _playerPos));
		}

		addVectorsf(&GT_BULLETS->position, multiplyVectorsrf(GT_BULLETS->forward, GT_BULLETS->velocity * delta));

		if (GT_BULLETS->position.x  < mainView->viewPos.x - 1000.0f || GT_BULLETS->position.x > mainView->viewPos.x + 1000.0f  || GT_BULLETS->position.y < mainView->viewPos.y - 600.0f || GT_BULLETS->position.y > mainView->viewPos.y + 600.0f)
		{
			bulletsList->erase(&bulletsList, i);
			continue;
		}
			
		if (GT_BULLETS->type == BLT_DIRT || GT_BULLETS->type == BLT_STONE)
		{
			GT_BULLETS->rotation += 1000.0f * delta;
		}
		else
		{
			GT_BULLETS->rotation = getAngleBetweenVectors(vector2f(1.0f,0.0f), GT_BULLETS->forward) * RAD2DEG;
		}
		sfVector2f NextPos = multiplyVectorsrf(GT_BULLETS->forward, GT_BULLETS->velocity * delta);
		if (sfFloatRect_intersects(&GT_BULLETS->colRect, plr_getPointerAllPlayerBounds(), NULL))
		{
			bulletsList->erase(&bulletsList, i);
			plr_addDamagePlayerLife(1);
			break;
		}
		else if (cll_RectMap(GT_BULLETS->colRect, NextPos))
		{
			bulletsList->erase(&bulletsList, i);
			break;
		}
		if (GT_BULLETS->isBack == sfTrue && GT_BULLETS->type == BLT_ENERGY)
		{
			bulletsList->erase(&bulletsList, i);
		}
	}
}
void blt_displayBullets(Window* _window)
{
	for (int i = 0; i < bulletsList->size(bulletsList); i++)
	{
		sfRectangleShape_setPosition(blt_rectangle, GT_BULLETS->position);
		sfRectangleShape_setTextureRect(blt_rectangle, GT_BULLETS->rect);
		sfRectangleShape_setRotation(blt_rectangle, GT_BULLETS->rotation);

		GT_BULLETS->colRect = sfRectangleShape_getGlobalBounds(blt_rectangle);

		sfRenderTexture_drawRectangleShape(_window->rdrt, blt_rectangle, NULL);
	}
}

sfBool isNutHere()
{
	if (bulletsList->size(bulletsList) > 0) return sfTrue;
	else return sfFalse;
}

sfFloatRect* blt_getNutFloatRect()
{
	/// This looks kinda malformed to me
	for (int i = 0; i < bulletsList->size(bulletsList); i++)
	{
		sfFloatRect tmp = GT_BULLETS->colRect;
		return &tmp;
	}
	return NULL;
}

sfFloatRect blt_GetNutBounds(Bullets_struct* _nut)
{
	if (!_nut)
		return FlRect(0.f, 0.f, 0.f, 0.f);

	if (_nut->type == BLT_DIRT)
		return FlRect(_nut->position.x, _nut->position.y, 40, 36);
	else if (_nut->type == BLT_STONE)
		return FlRect(_nut->position.x, _nut->position.y, 42, 34);
	else if (_nut->type == BLT_CRYSTAL)
		return FlRect(_nut->position.x, _nut->position.y, 40, 36);
	
	return FlRect(0.f, 0.f, 0.f, 0.f);
}

void blt_deinitBullets() {
	if (!blt_rectangle) return;
	sfRectangleShape_destroy(blt_rectangle);
}