#include "debuffTear.h"
#include "math.h"
#include "textureManager.h"
#include "player.h"

DebuffTear* beginDebuffTear = 0;
sfTexture* textDebuffTear;
sfRectangleShape* dbfT_rectangle;

float timerZimmer = 0.0f;
void dbfT_initTearDebuff()
{
	dbfT_rectangle = sfRectangleShape_create();
	textDebuffTear = GetSfTexture("SmallSadMove");
	sfRectangleShape_setTexture(dbfT_rectangle, textDebuffTear, sfTrue);
}

void dbfT_addTearDebuff(sfVector2f _pos)
{
	DebuffTear* tmpDebuffTear = malloc(sizeof(DebuffTear));
	tmpDebuffTear->pos = _pos;
	int whichOne = rand() % 5;
//	sfIntRect rectDebuffTear = IntRect(0,0,0,0);
	tmpDebuffTear->colRect = FlRect(0.0f,0.0f,0.0f,0.0f);
	tmpDebuffTear->alpha = 255.0f;

	if (whichOne == 0) {
		tmpDebuffTear->rect = IntRect(0, 332, 24, 10);
		tmpDebuffTear->radius = 10.f;
		tmpDebuffTear->timerToDie = 6.f;
	}
	else if (whichOne == 1) {
		tmpDebuffTear->rect = IntRect(58, 332, 18, 10);
		tmpDebuffTear->radius = 10.f;
		tmpDebuffTear->timerToDie = 5.f;
	}
	else if (whichOne == 2) {
		tmpDebuffTear->rect = IntRect(116, 332, 12, 10);
		tmpDebuffTear->radius = 5.f;
		tmpDebuffTear->timerToDie = 4.f;
	}
	else if (whichOne == 3) {
		tmpDebuffTear->rect = IntRect(174, 330, 32, 14);
		tmpDebuffTear->radius = 14.f;
		tmpDebuffTear->timerToDie = 7.f;
	}
	else if (whichOne == 4) {
		tmpDebuffTear->rect = IntRect(232, 335, 58, 8);
		tmpDebuffTear->radius = 20.f;
		tmpDebuffTear->timerToDie = 8.f;
	}

	tmpDebuffTear->pNext = beginDebuffTear;
	beginDebuffTear = tmpDebuffTear;
}



void dbfT_updateTearDebuff(Window* _window)
{
	float dt = getDeltaTime();
	timerZimmer += dt;

	if (customKeyboardIsKeyPressed(sfKeySpace, _window->renderWindow) && timerZimmer > 0.2f)
	{
		dbfT_addTearDebuff(plr_GetPos());
		timerZimmer = 0.0f;
	}
	sfFloatRect tmpPBound = plr_getPlayerBoundsWithEnemies();

	DebuffTear* tmpTearDebuff = beginDebuffTear;
	while (tmpTearDebuff != NULL) {

		if (sfFloatRect_intersects(&tmpPBound, &tmpTearDebuff->colRect, NULL))
		{
			//PLAYER DEBUFF;
		}

		tmpTearDebuff->timerToDie -= dt;
		if (tmpTearDebuff->timerToDie <= 0.f)
		{
			tmpTearDebuff->alpha -= 300.0f * dt;
			if(tmpTearDebuff->alpha < 2.f)
				tmpTearDebuff = deleteTearDebuff(tmpTearDebuff);
			else
				tmpTearDebuff = tmpTearDebuff->pNext;
		}
		else
			tmpTearDebuff = tmpTearDebuff->pNext;
	}
}

void dbfT_displayTearDebuff(Window* _window)
{
	DebuffTear* tmpTearDebuff = beginDebuffTear;

	while (tmpTearDebuff != NULL) {
		sfRectangleShape_setPosition(dbfT_rectangle, tmpTearDebuff->pos);
		sfRectangleShape_setTextureRect(dbfT_rectangle, tmpTearDebuff->rect);
		if (tmpTearDebuff->rect.width == 24)
			sfRectangleShape_setSize(dbfT_rectangle, vector2f(24.f, 10.0f));
		else if (tmpTearDebuff->rect.width == 18)
			sfRectangleShape_setSize(dbfT_rectangle, vector2f(18.f, 10.0f));
		else if (tmpTearDebuff->rect.width == 12)
			sfRectangleShape_setSize(dbfT_rectangle, vector2f(12.f, 10.0f));
		else if (tmpTearDebuff->rect.width == 32)
			sfRectangleShape_setSize(dbfT_rectangle, vector2f(32.f, 10.0f));
		else if (tmpTearDebuff->rect.width == 58)
			sfRectangleShape_setSize(dbfT_rectangle, vector2f(58.f, 10.0f));
		sfRectangleShape_setFillColor(dbfT_rectangle, sfColor_fromRGBA(255, 255, 255, (sfUint8)tmpTearDebuff->alpha));

		tmpTearDebuff->colRect = sfRectangleShape_getGlobalBounds(dbfT_rectangle);
		sfRenderTexture_drawRectangleShape(_window->rdrt, dbfT_rectangle, NULL);
		tmpTearDebuff = tmpTearDebuff->pNext;
	}
}

void dbfT_Deinit() {
	if (!dbfT_rectangle) return;
	sfRectangleShape_destroy(dbfT_rectangle);
}

DebuffTear* deleteTearDebuff(DebuffTear* _tear)
{
	if (_tear == beginDebuffTear)
	{
		DebuffTear* tempDebuffTear = _tear->pNext;
		beginDebuffTear = tempDebuffTear;
		free(_tear);
		return tempDebuffTear;
	}
	else
	{
		DebuffTear* tempDebuffTear = beginDebuffTear;
		while (tempDebuffTear->pNext != _tear)
		{
			tempDebuffTear = tempDebuffTear->pNext;
		}
		tempDebuffTear->pNext = _tear->pNext;
		free(_tear);
		return tempDebuffTear->pNext;
	}
}

