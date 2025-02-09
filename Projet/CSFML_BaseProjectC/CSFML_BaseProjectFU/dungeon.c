#include "dungeon.h"
#include "bossJoie.h"
#include "bossTristesse.h"
#include "player.h"
#include "map.h"
#include "fish_tank.h"
#include "controller.h"
#include "pause.h"
#include "dialogBox.h"

void dungeon_initDungeon(Window* _window)
{
	if (map_GetState() == MAP_STATE_DUNGEON_JOY_BOSS) {
		if (getState() != EDITOR) entity_BossJoie.position = vector2f(1247.f, 1116.f);
	}
	else if (map_GetState() == MAP_STATE_DUNGEON_SADNESS_BOSS) {
		if (getState() != EDITOR) initBoss(vector2f(1247.f, 1116.f), 15.f);
	}
	if (map_GetState() == MAP_STATE_DUNGEON_JOY_FOUNTAIN) {
		tank_Init();
	}
}

void dungeon_updateDungeon(Window* _window)
{
	if (map_GetState() == MAP_STATE_DUNGEON_JOY_BOSS) {
		if (getState() != EDITOR) updateBossJoie(&entity_BossJoie, _window);
	}
	else if (map_GetState() == MAP_STATE_DUNGEON_SADNESS_BOSS) {
		if (getState() != EDITOR) 
			updateBoss(_window);
	}

	if (map_GetState() == MAP_STATE_DUNGEON_JOY_FOUNTAIN) {
		static sfBool fishReleased = sfFalse;
		static sfBool flagDB = sfFalse;
		static float timerRelease = 0.f;
		if (STD_LIST_GETDATA(itemList, Itemlist, 10)->nbObject == 3) { /// Has 3 fish of joy
			if (Gamepad_isButtonPressed(0, A, _window->renderWindow)) {
				pause_itemRemove(PAUSE_JOY_FISH, 3);
				tank_SetReleaseStatus(sfTrue);
				flagDB = sfTrue;
				db_Queue("Fountain", "Fish released", plr_GetPos());
			}
		}

		if (flagDB) {
			timerRelease += getDeltaTime();
			if (timerRelease >= 2.f) flagDB = db_IsUp();
			if (!flagDB) tank_SetReleaseStatus(sfFalse);
		}

		tank_Update(_window);
	}

}

void dungeon_displayDungeon(Window* _window)
{
	if (map_GetState() == MAP_STATE_DUNGEON_JOY_BOSS) {
		if (getState() != EDITOR) displayBossJoie(_window, &entity_BossJoie);
	}
	else if (map_GetState() == MAP_STATE_DUNGEON_SADNESS_BOSS) {
		if (getState() != EDITOR) displayBoss(_window);
	}

	if (map_GetState() == MAP_STATE_DUNGEON_JOY_FOUNTAIN) {
		tank_Display(_window);
	}
}

void dungeon_deinitDungeon(Window* _window)
{
}
