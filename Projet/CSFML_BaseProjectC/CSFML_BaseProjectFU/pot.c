#include "pot.h"
#include "game.h"
#include "player.h"
#include "collisionManager.h"
#include "particlesManager.h"
#include "viewManager.h"
#include "drop_items.h"
#include "game_HUD.h"
#include "soundManager.h"

sfSprite* pot_RenderSprite;
sfBool isKeyDropped;

void pot_Init() {
	static sfBool onePass = sfTrue;
	if (onePass) {
		onePass = sfFalse;
		pot_List = STD_LIST_CREATE(PotData, 0);

		pot_RenderSprite = sfSprite_create();
		sfSprite_setTexture(pot_RenderSprite, GetSfTexture("pot_n_break"), sfTrue);
	}
}

void pot_Update() {
	sfFloatRect slashBounds = plr_GetDamageBounds();
	sfBool isSlashDamaging = plr_IsSlashDamaging();
	if (getIsFreeingMap())
		return;
	for (int i = 0; i < pot_List->size(pot_List); i++) {
		if (getIsFreeingMap())
			return;
		if (POT_GETDATA->isOnScreen) {
			if (isSlashDamaging && POT_GETDATA->hp == 1) {
				sfFloatRect potBounds = pot_GetBounds(POT_GETDATA);
				if (sfFloatRect_intersects(&slashBounds, &potBounds, NULL)) {
					POT_GETDATA->hp--;
					POT_GETDATA->animFrame = POT_GETDATA->type == POT_HARD ? 1 : 2;
				}
			}
			if (POT_GETDATA->hp == 0 && !POT_GETDATA->isBroken) {
				prt_CreateCircleParticles(vector2f(POT_GETDATA->pos.x +32.f, POT_GETDATA->pos.y +32.f), 5, 8, sfColor_fromRGBA(36.f, 165.f, 15.f, 200.f), sfColor_fromRGBA(36.f, 165.f, 15.f, 150.f), 1.f, 8.f, 5.f, 0.f, 360.f, 90.f, 200.f, 0.3f,
					sfColor_fromRGBA(255.f, 255.f, 255.f, 200.f), sfColor_fromRGBA(255.f, 255.f, 255.f, 150.f), sfTrue, sfFalse, sfFalse, GetSfTexture("pot_n_break"), sfFalse, sfFalse, sfFalse, POT);  
				pot_Break(POT_GETDATA);
			}
			if (getIsFreeingMap())
				return;
			if ((!getIsFreeingMap() && POT_GETDATA->animFrame >= 6 && POT_GETDATA->timerBroken > 0.05f) || POT_GETDATA->animFrame >= 7) {
				sfMutex_lock(mutexThread2);
				pot_List->erase(&pot_List, i);
				sfMutex_unlock(mutexThread2);
			}
		}
	}
}

void pot_Display(Window* _window) {
	if (!pot_List || getIsFreeingMap()) return;

	for (int i = 0; i < pot_List->size(pot_List); i++) {
		PotData* tmp = POT_GETDATA;
		
		if (getIsFreeingMap()) return;
		if (tmp->pos.x < mainView->viewPos.x - (mainView->size.x / 2.f) - 64.f || tmp->pos.x > mainView->viewPos.x + (mainView->size.x / 2.f) + 64.f || tmp->pos.y < mainView->viewPos.y - (mainView->size.y / 2.f) - 64.f || tmp->pos.y > mainView->viewPos.y + (mainView->size.y / 2.f) + 64.f) {
			POT_GETDATA->isOnScreen = sfFalse;
			continue;
		}
		else if (!POT_GETDATA->isOnScreen)
			POT_GETDATA->isOnScreen = sfTrue;

		sfSprite_setPosition(pot_RenderSprite, tmp->pos);
		sfIntRect rect = IntRect(0, 0, 64, 64);
		if (tmp->type == POT_HARD) {
			rect.top += 64;
			if (tmp->hp == 1) rect.left += 64;
		}
		if (tmp->isBroken) rect = animatorAnim( tmp->type == POT_HARD ? "pot_h_break" : "pot_n_break", &tmp->animFrame, &tmp->timerBroken, 0);
		sfSprite_setTextureRect(pot_RenderSprite, rect);
		sfRenderTexture_drawSprite(_window->rdrt, pot_RenderSprite, NULL);
	}
}

void pot_Create(sfVector2f _pos, sfBool _pos_snap, PotType _type) {
	if (pot_List == NULL) return;

	PotData tmp = { 0 };
	tmp.type = _type;
	tmp.pos = _pos_snap ? map_SnapToGrid(_pos) : _pos;
	tmp.spd = NULLVECTF;
	tmp.mass = 2.f;
	tmp.isBroken = sfFalse;
	tmp.timerBroken = 0.f;
	tmp.hp = _type == POT_HARD ? 2 : 1;
	tmp.isOnScreen = sfTrue;
	tmp.haveKey = sfFalse;

	if (map_GetState() == MAP_STATE_DUNGEON_JOY_2)
	{
		for (int i = 0; i < pot_List->size(pot_List); i++)
		{
			PotData* tmp = STD_LIST_GETDATA(pot_List, PotData, i);
			if (tmp->haveKey == sfTrue)
				isKeyDropped = sfTrue;

			if (!isKeyDropped && i == pot_List->size(pot_List) - 1) {
				isKeyDropped = sfTrue;
				((PotData*)pot_List->getData(pot_List, 0))->haveKey = sfTrue;

			}
		}
		if (!isKeyDropped)
		{
			
			int randKey = rand() % 101;
			if (randKey > 65)
				tmp.haveKey = sfTrue;
		}
	}
	pot_List->push_back(&pot_List, &tmp);
}

sfBool pot_PlayerCollision(PotData* _pot, sfVector2f _vitesse_plr) {
	if (!_pot || _pot->isBroken) return sfFalse;

	sfFloatRect potBounds = pot_GetBounds(_pot);
	sfFloatRect plrBounds = *getPointerPlayerBounds();
	plrBounds.left += _vitesse_plr.x;
	plrBounds.top += _vitesse_plr.y;

	return sfFloatRect_intersects(&plrBounds, &potBounds, NULL);
}

void pot_Break(PotData* _pot) {
	if (!_pot || _pot->isBroken) return;
	/// Spawn item or sth

	int randHeartSpawn = rand() % 101;
	if (randHeartSpawn > 85 || map_GetState() == MAP_STATE_DUNGEON_JOY_BOSS)
		createItemsDrop(HEART, _pot->pos);
	else if (map_GetState() == MAP_STATE_DUNGEON_JOY_2)
	{
		if (getNbJoyKey() < 1) {
			if (!isKeyDropped)
			{
				HUD_addKey();
				prt_CreateGainParticles(_pot->pos);
			}
			else if (_pot->haveKey == sfTrue)
			{
				HUD_addKey();
				prt_CreateGainParticles(_pot->pos);
			}
		}
	}
	_pot->isBroken = sfTrue;
	playSoundFX("SFX_Jar_Break");
}

PotToSave* getPotListToSave() {
	if (!pot_List) return NULL;

	int len = pot_List->size(pot_List);
	PotToSave* tmp = calloc(len, sizeof(PotToSave));
	if (!tmp) return NULL;

	for (int i = 0; i < len; i++)
	{
		tmp[i].pos = STD_LIST_GETDATA(pot_List, PotData, i)->pos;
		tmp[i].type = STD_LIST_GETDATA(pot_List, PotData, i)->type;
	}
	return tmp;
}

void pot_Clear() {
	if (!pot_List) return;
	pot_List->clear(&pot_List);
}

sfFloatRect pot_GetBounds(PotData* _pot) {
	if (!pot_List || !_pot) return FlRect(0.f, 0.f, 0.f, 0.f);

	return FlRect(_pot->pos.x, _pot->pos.y, POT_X_SIZE, POT_Y_SIZE);
}