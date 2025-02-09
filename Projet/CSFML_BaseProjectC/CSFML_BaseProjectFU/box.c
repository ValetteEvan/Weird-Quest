#include "box.h"
#include "math.h"
#include "player.h"
#include "collisionManager.h"
#include "viewManager.h"
#include "textureManager.h"


#define BOX_X_ORIGIN 0
#define BOX_Y_ORIGIN 0

sfRectangleShape* box_RenderRect;
sfTexture* box_Texture;


void box_Init()
{
	boxList = STD_LIST_CREATE(BoxData, 0);

	box_RenderRect = sfRectangleShape_create();
	sfRectangleShape_setSize(box_RenderRect, vector2f(BOX_X_SIZE, BOX_Y_SIZE));

	box_Texture = GetSfTexture("dungeon_boulder");

	sfRectangleShape_setTexture(box_RenderRect, box_Texture, sfTrue);
}

void box_Update()
{
	if (boxList == NULL)
		return;

	for (int i = 0; i < boxList->size(boxList); i++)
	{
		BoxData* tmp = BOX_GETDATA;

		if (tmp->timeScale != 0.f && tmp->timeScale2 != 0.f && !tmp->hasFallen) {

			if (tmp->timer < 10.f) {
				sfVector2i intTmpPos = vector2i((tmp->pos.x + 32) / 64.f, (tmp->pos.y + 32)/64.f);
				if (checkBoxVoidCollision(intTmpPos)) {
					tmp->pos = vector2f(intTmpPos.x * 64.f, intTmpPos.y * 64.f - 64.f);
					tmp->hasFallen = sfTrue;

					continue;
				}

				float dt = getDeltaTime() * tmp->timeScale;
				tmp->timer += dt;

				sfVector2f nextBoxPos = vec2f_lerp_invCube(tmp->startPos, tmp->destinationPos, (tmp->timer + dt) / 2.f);
				sfVector2f velocityBox = addVectorsrf(nextBoxPos, multiplyVectorsrf(tmp->pos, -1.f));

				multiplyVectorsf(&velocityBox, 1 / dt);

				sfFloatRect cld_boxCollide = FlRect(nextBoxPos.x, nextBoxPos.y, BOX_X_SIZE, BOX_Y_SIZE/2.f);

				if (tmp->timer >= 10.f) {
					tmp->destinationPos = tmp->pos;
					tmp->timer = 10.f;
				}

				else if (cll_RectMap(cld_boxCollide, vector2f(velocityBox.x * getDeltaTime(), 0.f))) {
					tmp->destinationPos = tmp->pos;
					tmp->timer = 10.f;
				}
				else if (cll_RectMap(cld_boxCollide, vector2f(0.f, velocityBox.y * getDeltaTime()))) {
					tmp->destinationPos = tmp->pos;
					tmp->timer = 10.f;
				}
				else tmp->pos = vec2f_lerp_invCube(tmp->startPos, tmp->destinationPos, tmp->timer / 2.f);
			}

			if (box_PlayerCollision(tmp, NULLVECTF)) {
				tmp->timer = 10.f;
				tmp->destinationPos = tmp->pos;
			}
		}
		else if (tmp->timeScale == 0.f) {
			float udt = getDeltaTime();
			tmp->timerUnscaled += udt;
			if (tmp->timerUnscaled > 10.f) {
				tmp->timeScale = 1.f;
			}
		}
	}
}

void box_Display(Window* _window)
{
	if (boxList == NULL)
		return;

	sfRectangleShape_setSize(box_RenderRect, vector2f(BOX_X_SIZE, BOX_Y_SIZE));


	for (int i = 0; i < boxList->size(boxList); i++)
	{
		BoxData* tmp = BOX_GETDATA;
		if (tmp->pos.x < mainView->viewPos.x - (mainView->size.x / 2.f) - 64.f || tmp->pos.x > mainView->viewPos.x + (mainView->size.x / 2.f) + 64.f || tmp->pos.y < mainView->viewPos.y - (mainView->size.y / 2.f) - 64.f || tmp->pos.y > mainView->viewPos.y + (mainView->size.y / 2.f) + 64.f)
			continue;
		if (!tmp->hasFallen) {
			

			sfRectangleShape_setPosition(box_RenderRect, tmp->pos);
			sfRectangleShape_setOrigin(box_RenderRect, vector2f(0.f, 64.f));
			sfRectangleShape_setTextureRect(box_RenderRect, tmp->rect);
			sfRenderTexture_drawRectangleShape(_window->rdrt, box_RenderRect, NULL);
		}
		else {
			sfRectangleShape_setPosition(box_RenderRect, tmp->pos);
			sfRectangleShape_setOrigin(box_RenderRect, vector2f(0.f, 0.f));
			sfRectangleShape_setTextureRect(box_RenderRect, animatorWithResize("dungeon_pav_stone", &tmp->frameX, &tmp->timerAnim, 0, box_RenderRect));
			if (tmp->frameX == 5)
				tmp->timerAnim = 0.f;
			sfRenderTexture_drawRectangleShape(_window->rdrt, box_RenderRect, NULL);
		}
		
	}
}

void box_DisplayAbove(Window* _window)
{
	if (boxList == NULL)
		return;

	sfRectangleShape_setSize(box_RenderRect, vector2f(BOX_X_SIZE, 64.f));

	for (int i = 0; i < boxList->size(boxList); i++)
	{
		BoxData* tmp = BOX_GETDATA;
		if (tmp->pos.x < mainView->viewPos.x - (mainView->size.x / 2.f) - 64.f || tmp->pos.x > mainView->viewPos.x + (mainView->size.x / 2.f) + 64.f || tmp->pos.y < mainView->viewPos.y - (mainView->size.y / 2.f) - 64.f || tmp->pos.y > mainView->viewPos.y + (mainView->size.y / 2.f) + 64.f)
			continue;
		if (!tmp->hasFallen) {		
			sfRectangleShape_setPosition(box_RenderRect, addVectorsrf(tmp->pos, vector2f(0.f, -64.f)));
			sfRectangleShape_setOrigin(box_RenderRect, vector2f(0.f, 0.f));
			sfIntRect tmpRectBox = tmp->rect;
			tmpRectBox.height -= 64.f;
			sfRectangleShape_setTextureRect(box_RenderRect, tmpRectBox);
			sfRenderTexture_drawRectangleShape(_window->rdrt, box_RenderRect, NULL);
		}
		
	}
}

void box_Create(sfVector2f _pos)
{
	if (boxList == NULL)
		return;

	BoxData tmp;
	tmp.pos = _pos;
	tmp.destinationPos = _pos;
	tmp.startPos = _pos;
	tmp.timeScale = 1.f;
	tmp.timeScale2 = 1.f;
	tmp.mass = 2.f;
	tmp.timerUnscaled = 0.f;
	tmp.timer = 10.f;
	tmp.timerAnim = 0.f;
	tmp.frameX = 0;
	tmp.rect = IntRect(0, 704, 64, 128);
	tmp.hasFallen = sfFalse;

	boxList->push_back(&boxList, &tmp);
}

sfBool box_PlayerCollision(BoxData* _box, sfVector2f _vitesse_plr)
{
	if (_box == NULL || _box->hasFallen)
		return sfFalse;
	sfVector2f nextBoxPos = vec2f_lerp_invCube(_box->startPos, _box->destinationPos, (_box->timer + getDeltaTime()) / 2.f);
	sfFloatRect boxBounds = FlRect(nextBoxPos.x, nextBoxPos.y, BOX_X_SIZE, BOX_Y_SIZE/2.f);
	sfFloatRect plrBounds = *getPointerPlayerBounds();
	plrBounds.left += _vitesse_plr.x;
	plrBounds.top += _vitesse_plr.y;

	return sfFloatRect_intersects(getPointerPlayerBounds(), &boxBounds, NULL);
}

sfBool box_BoxCollision(BoxData* _box, sfVector2f _vitesse)
{
	if (_box == NULL)
		return sfFalse;

	sfFloatRect boxBounds = FlRect(_box->pos.x + _vitesse.x, _box->pos.y + _vitesse.y, BOX_X_SIZE, BOX_Y_SIZE);

	for (int i = 0; i < boxList->size(boxList); i++)
	{
		if (BOX_GETDATA == _box) continue;
		sfFloatRect colliderBounds = FlRect(BOX_GETDATA->pos.x, BOX_GETDATA->pos.y, BOX_X_SIZE, BOX_Y_SIZE);
		sfBool canMove = sfFloatRect_intersects(&colliderBounds, &boxBounds, NULL);
		if (canMove) return sfTrue;
	}
	return sfFalse;
}

sfFloatRect box_GetBounds(BoxData* _box)
{
	if (_box == NULL)
		return (sfFloatRect){ 0 };

	return FlRect(_box->pos.x, _box->pos.y, BOX_X_SIZE, BOX_Y_SIZE / 2.f);
}

sfVector2f getStuntBoxOffset(sfVector2f _ringPos, BoxData* _box)
{
	if (_box == NULL)
		return NULLVECTF;

	return vector2f(_ringPos.x -_box->pos.x, _ringPos.y - _box->pos.y);
}

sfVector2f getStuntBoxPos(sfVector2f _offset, BoxData* _box)
{
	if (_box == NULL)
		return NULLVECTF;

	return addVectorsrf(_box->pos, _offset);
}

void box_Attract(BoxData* _box)
{
	if (_box == NULL)
		return;

	sfVector2f newForward = getNormBetween2Vectorsf(plr_GetFeetPos(), _box->pos);

	_box->timer = 0.f;
	_box->startPos = _box->pos, vector2f(BOX_X_SIZE / 2.f, BOX_Y_SIZE / 2.f);
	addVectorsf(&_box->destinationPos, multiplyVectorsrf(newForward, -128.f / _box->mass));
}

void box_Repulse(BoxData* _box)
{
	if (_box == NULL)
		return;

	sfVector2f newForward = getNormBetween2Vectorsf(plr_GetFeetPos(), _box->pos);
	_box->timer = 0.f;
	_box->startPos = _box->pos;
	addVectorsf(&_box->destinationPos, multiplyVectorsrf(newForward, 128.f / _box->mass));
}

void box_Stasis(BoxData* _box)
{
	if (_box == NULL)
		return;

	_box->timeScale = 0.f;
	_box->timerUnscaled = 0.f;
}

void box_Destroy(BoxData* _box)
{
	if (_box == NULL)
		return;

	for (int i = 0; i < boxList->size(boxList); i++) {
		if (BOX_GETDATA == _box) {
			boxList->erase(&boxList, i);
			return;
		}
	}

}

void box_Clear() {
	if (!boxList) return;

	for (int i = 0; i < boxList->size(boxList); i++)
		boxList->erase(&boxList, i);
}

void box_AttachRing(sfBool _bool, BoxData* _box)
{
	if (_box == NULL)
		return;

	if (_bool)
		_box->timeScale2 = 0.f;
	else
		_box->timeScale2 = 1.f;
}

void PushBox(sfVector2f _pushDir, BoxData* _box)
{
	if (_box == NULL)
		return;
	
	sfVector2f newForward = _pushDir;
	_box->timer = 0.f;
	_box->startPos = _box->pos;
	addVectorsf(&_box->destinationPos, multiplyVectorsrf(newForward, 40.f * getDeltaTime()));

	/*sfFloatRect cld_boxCollide = FlRect(_box->pos.x, _box->pos.y, BOX_X_SIZE, BOX_Y_SIZE/2.f);
	sfVector2f spd = multiplyVectorsrf(_pushDir, 50.f * _box->timeScale * _box->timeScale2);

	if (cll_RectMap(cld_boxCollide, vector2f(spd.x * getDeltaTime(), 0.f))) {
		return;
	}
	if (cll_RectMap(cld_boxCollide, vector2f(0.f, spd.y * getDeltaTime()))) {
		return;
	}

	multiplyVectorsf(&spd, getDeltaTime());

	if (box_BoxCollision(_box, spd)) {
		return;
	}

	_box->pos = addVectorsrf(_box->pos, spd);*/
}

void ShoulderHitBox(sfVector2f _pushDir, BoxData* _box)
{
	if (_box == NULL)
		return;

	sfVector2f newForward = _pushDir;
	_box->timer = 0.f;
	_box->startPos = _box->pos;
	addVectorsf(&_box->destinationPos, multiplyVectorsrf(newForward, 200.f / _box->mass));
}

sfVector2f* getBoxCollisionPoints(BoxData* _box)
{
	if (_box == NULL)
		return NULL;

	sfVector2f tab[4] = { 0 };
	tab[0] = _box->pos;
	tab[1] = addVectorsrf(_box->pos, vector2f(BOX_X_SIZE, 0.f));
	tab[2] = addVectorsrf(_box->pos, vector2f(BOX_X_SIZE, BOX_Y_SIZE/2.f));
	tab[3] = addVectorsrf(_box->pos, vector2f(0.f, BOX_Y_SIZE/2.f));
	return tab;
}

sfVector2f getBoxCollisionSinglePoint(int _point, BoxData* _box)
{
	if (_box == NULL)
		return NULLVECTF;

	switch (_point)
	{
		case 0: return _box->pos;
		case 1: return addVectorsrf(_box->pos, vector2f(BOX_X_SIZE, 0.f));
		case 2: return addVectorsrf(_box->pos, vector2f(BOX_X_SIZE, BOX_Y_SIZE/2.f));
		case 3: return addVectorsrf(_box->pos, vector2f(0.f, BOX_Y_SIZE/2.f));
		default:
			return _box->pos;
			break;
	}
}

BoxData* getClosestBox(sfVector2f _pos)
{
	if (boxList == NULL)
		return NULL;

	float lastDistance = LLONG_MAX;
	int idBox = 0;

	for (int i = 0; i < boxList->size(boxList); i++)
	{
		float distanceBoxPlayer = getSqrMagnitudeBetween2Vectorsf(_pos, BOX_GETDATA->pos);
		if (distanceBoxPlayer < lastDistance)
		{
			lastDistance = distanceBoxPlayer;
			idBox = i;
		}
	}
	if (STD_LIST_GETDATA(boxList, BoxData, idBox) == NULL)
		return NULL;
	return STD_LIST_GETDATA(boxList, BoxData, idBox);
}

BoxDataToSave* getBoxListToSave()
{
	if (boxList == NULL)
		return NULL;

	int len = boxList->size(boxList);
	BoxDataToSave* tmp = calloc(len, sizeof(BoxDataToSave));

	if (tmp == NULL)
		return NULL;

	for (int i = 0; i < len; i++)
	{
		tmp[i].pos = STD_LIST_GETDATA(boxList, BoxDataToSave, i)->pos;
	}
	return tmp;
}
