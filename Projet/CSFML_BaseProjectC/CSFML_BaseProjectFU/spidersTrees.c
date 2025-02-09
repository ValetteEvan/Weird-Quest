#include "spidersTrees.h"
#include "enemies.h"
#include "player.h"
#include "textureManager.h"
#include "particlesManager.h"

float timerSpawnPartTree;

#define GT_STREE STD_LIST_GETDATA(spidersTreesList, SpidersTreeStruct, i)

sfTexture* stree_texture;
sfRectangleShape* stree_rectangle;

void stree_initSpidersTrees()
{
	timerSpawnPartTree = 0.f;
	stree_rectangle = sfRectangleShape_create();
	stree_texture = GetSfTexture("assets_forest");
	sfRectangleShape_setTexture(stree_rectangle, stree_texture, sfTrue);
	spidersTreesList = STD_LIST_CREATE(SpidersTreeStruct,0);
}

void stree_addSpidersTrees(sfVector2f _pos)
{
	SpidersTreeStruct tmp;

	tmp.pos = _pos;
	tmp.isTouched = sfFalse;
	tmp.bound = FlRect(0.0f, 0.0f, 0.0f, 0.0f);
	tmp.rect = IntRect(0, 0, 0, 0);

	spidersTreesList->push_back(&spidersTreesList, &tmp);
}

void stree_updateSpidersTrees(Window* _window)
{

	static float timer = 0.f;
	timer += getDeltaTime();
	timerSpawnPartTree += getDeltaTime();
	sfFloatRect tmpPBounds = plr_getPlayerBounds();
	if (customKeyboardIsKeyPressed(sfKeyU, _window->renderWindow) && timer > 0.2f)
	{
		timer = 0.f;
		stree_addSpidersTrees(vector2f(200.0f, 200.0f));
	}
	int a = spidersTreesList->size(spidersTreesList);
	for(int i = 0; i < spidersTreesList->size(spidersTreesList); i++)
	{
		GT_STREE->bound = FlRect(GT_STREE->pos.x + 140.0f, GT_STREE->pos.y+250.0f, 190.f,160.f);
		if (sfFloatRect_intersects(&tmpPBounds, &GT_STREE->bound, NULL) && plr_IsRushing() && !GT_STREE->isTouched && timerSpawnPartTree > 0.5f)
		{
			timerSpawnPartTree = 0.f;
			prt_CreateLeafTreeParticles(vector2f(GT_STREE->pos.x + 250.0f, GT_STREE->pos.y));
			if (en_isAnySpiderHere()) continue;
			GT_STREE->isTouched = sfTrue;
			int rdm = rand() % 2;
			if (rdm == 0)
			{
				en_SpawnEnemy(vector2f(GT_STREE->pos.x + 130.0f, GT_STREE->pos.y + 260.0f), EN_YOYO_SPIDER, 0, sfFalse);
				prt_CreateLeafYoyoParticles(vector2f(GT_STREE->pos.x + 110.0f, GT_STREE->pos.y + 240.0f));
			}
			else
			{
				en_SpawnEnemy(vector2f(GT_STREE->pos.x + 130.0f, GT_STREE->pos.y + 260.0f), EN_VENIMOUS_YOYO_SPIDER, 0, sfFalse);
				prt_CreateLeafYoyoParticles(vector2f(GT_STREE->pos.x + 110.0f, GT_STREE->pos.y + 240.0f));
			}
		}
		if(!sfFloatRect_intersects(&tmpPBounds, &GT_STREE->bound, NULL) && !plr_IsRushing() && GT_STREE->isTouched)
			GT_STREE->isTouched = sfFalse;
	}
}

void stree_displaySpidersTreesBehindPlayer(Window* _window)
{
	for (int i = 0; i < spidersTreesList->size(spidersTreesList); i++)
	{
		sfRectangleShape_setSize(stree_rectangle, vector2f(148.f,64.f));
		sfRectangleShape_setTextureRect(stree_rectangle, IntRect(1196, 728, 148, 64));
		sfRectangleShape_setPosition(stree_rectangle, vector2f(GT_STREE->pos.x + 160.0f, GT_STREE->pos.y + 354.f));
		sfRenderTexture_drawRectangleShape(_window->rdrt, stree_rectangle, NULL);

	}
}

void stree_displaySpiderTreesInFrontOfPlayer(Window* _window)
{
	for (int i = 0; i < spidersTreesList->size(spidersTreesList); i++)
	{
		sfRectangleShape_setTextureRect(stree_rectangle, IntRect(796, 728, 400, 556));
		sfRectangleShape_setSize(stree_rectangle, vector2f(400.f, 556.f));
		sfRectangleShape_setPosition(stree_rectangle, GT_STREE->pos);
		sfRenderTexture_drawRectangleShape(_window->rdrt, stree_rectangle, NULL);

	}
}

void stree_clearSpidersTrees()
{
	if (!spidersTreesList) return;
	spidersTreesList->clear(&spidersTreesList);
}
