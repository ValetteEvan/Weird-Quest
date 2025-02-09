#include "grass.h"
#include "textureManager.h"
#include "player.h"
#include "enemies.h"
#include "math.h"
#include "viewManager.h"
#include "map.h"
#include "drop_items.h"
#include "particlesManager.h"
#include "soundManager.h"

sfTexture* textureGrass;
sfRectangleShape* rectGrass;

void getGrassTexture()
{
	textureGrass = GetSfTexture("grass1");
	sfRectangleShape_setTexture(rectGrass, textureGrass, sfTrue);
}

void initGrass()
{
	listGrass = stdList_Create(sizeof(Grass), 0);
	rectGrass = sfRectangleShape_create();
	sfRectangleShape_setSize(rectGrass, vector2f(64.f, 64.f));
}

void updateGrass()
{
	for (int i = 0; i < listGrass->size(listGrass); i++) {
		Grass* tmp = listGrass->getData(listGrass, i);
		if (tmp->isOnScreen) {
			if (tmp->isCut && tmp->frameX == 1 && !tmp->hasSpawnedSnake) {
				playSoundFX("SFX_Grass_Cut");
				int x = tmp->pos.x / TILE_SIZE;
				int y = tmp->pos.y / TILE_SIZE;
				map.layerCollision[y][x].type = CLD_NO_COLL;

				int randSnakeSpawn = rand() % 101;
				int randHeartSpawn = rand() % 101;
				if (randSnakeSpawn > 95)
					en_SpawnEnemy(addVectorsrf(tmp->pos, vector2f(32.f, 32.f)), EN_SNAKE, 0.f, sfFalse);
				else if (randHeartSpawn > 95)
					createItemsDrop(HEART, tmp->pos);
				tmp->hasSpawnedSnake = sfTrue;
				prt_CreateDestroyGrassParticles(vector2f(tmp->pos.x + 32.f, tmp->pos.y + 32.f));
			}
			else if (tmp->isOnScreen && !tmp->isCut && plr_IsSlashDamaging()) {
				sfFloatRect slashBounds = plr_GetDamageBounds();
				sfFloatRect grassBounds = FlRect(tmp->pos.x, tmp->pos.y, 64.f, 64.f);
				if (sfFloatRect_intersects(&slashBounds, &grassBounds, NULL)) {
					tmp->isCut = sfTrue;
					tmp->frameX = 0;
					tmp->timerAnim = 0.f;
				}
			}
		}
	}
}

void displayGrass(Window* _window)
{
	for (int i = 0; i < listGrass->size(listGrass); i++) {
		Grass* tmp = listGrass->getData(listGrass, i);
		if (tmp->pos.x > mainView->viewPos.x - (mainView->size.x / 2.f) - 64.f && tmp->pos.x < mainView->viewPos.x + (mainView->size.x / 2.f) + 64.f && tmp->pos.y > mainView->viewPos.y - (mainView->size.y / 2.f) - 64.f && tmp->pos.y < mainView->viewPos.y + (mainView->size.y / 2.f) + 64.f) {
			tmp->isOnScreen = sfTrue;
			if (tmp->isCut && tmp->frameX == 5) {
				tmp->timerAnim = 0.f;
			}
			sfRectangleShape_setPosition(rectGrass, tmp->pos);
			if (tmp->type == 0) {
				if (!tmp->isCut)
					sfRectangleShape_setTextureRect(rectGrass, IntRect(0, 384, 64, 64));
				else
					sfRectangleShape_setTextureRect(rectGrass, animatorAnim("grass1", &tmp->frameX, &tmp->timerAnim, 0));
			}
			else
			{
				if (!tmp->isCut)
					sfRectangleShape_setTextureRect(rectGrass, IntRect(0, 0, 64, 64));
				else
					sfRectangleShape_setTextureRect(rectGrass, animatorAnim("grass2", &tmp->frameX, &tmp->timerAnim, 0));
			}
			sfRenderTexture_drawRectangleShape(_window->rdrt, rectGrass, NULL);
		}
		else {
			tmp->isOnScreen = sfFalse;
		}
	}
}

void addGrass(sfVector2f _pos, int _type)
{
	Grass tmpGrass = { 0 };
	tmpGrass.pos = _pos;
	tmpGrass.frameX = 0;
	tmpGrass.timerAnim = 0.f;
	tmpGrass.type = _type;
	tmpGrass.isCut = sfFalse;
	tmpGrass.isOnScreen = sfTrue;
	tmpGrass.type = _type;
	tmpGrass.hasSpawnedSnake = sfFalse;

	listGrass->push_back(&listGrass, &tmpGrass);

	if (tmpGrass.type == 1)
	{
		int x = tmpGrass.pos.x / TILE_SIZE;
		int y = tmpGrass.pos.y / TILE_SIZE;
		map.layerCollision[y][x].type = CLD_BOX;
	}
}

void deleteGrass(sfVector2f _pos)
{

}

void clearGrass()
{
	if (!listGrass)return;
	listGrass->clear(&listGrass);
}

void shouldGrassBeCut(sfVector2f _posSlash)
{
}

sfBool isPlayerOnGrass(sfVector2i _posPlayerTile)
{
	if (getIsFreeingMap()) return sfFalse;

	for (int i = 0; i < listGrass->size(listGrass); i++) {
		if (getIsFreeingMap()) return sfFalse;
		Grass* tmp = listGrass->getData(listGrass, i);
		if (tmp->isOnScreen && !tmp->isCut) {
			sfVector2i posTileGrass = vector2i(tmp->pos.x / 64, tmp->pos.y / 64);
			if (_posPlayerTile.x == posTileGrass.x && _posPlayerTile.y == posTileGrass.y)
				return sfTrue;
		}
	}
	return sfFalse;
}
