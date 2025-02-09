#include "map.h"
#include "viewManager.h"
#include "player.h"
#include "editor.h"
#include "enemies.h"
#include "box.h"
#include "light.h"
#include "npc.h"
#include "math.h"
#include "fadeManager.h"
#include "pot.h"

#include "overworld.h"
#include "dungeon.h"
#include "lighthouse.h"
#include "bossTristesse.h"
#include "game_HUD.h"
#include "chest.h"
#include "bigRock.h"
#include "statue.h"
#include "expandingTile.h"
#include "moving_platform.h"
#include "chest.h"
#include "grass.h"
#include "openDoors.h"
#include "spidersTrees.h"
#include "statue.h"
#include "grass.h"
#include "fishing_spot.h"
#include "note.h"
#include "particlesManager.h"
#include "brazier.h"
#include "bossJoie.h"
#include "quest.h"
#include "menu.h"

sfRectangleShape* mapRect;
sfVector2f position;

sfSprite* editorCollSprite;
sfSprite* mapAssetsSprite;
sfBool onePassMap = sfTrue;
sfIntRect waterfallRect;

stdList* propsList;

sfBool showCollision;
int nbDifferentAcces;

sfRectangleShape* changeMapRectangle;
sfIntRect* changeMapRect;

sfBool showWallLayerFirst;
sfBool showGroundLayerSecond;

sfVector2f playerSpawnPos;
sfTexture* map_TexAssetsDefault = NULL;

#define MAP_SCREEN_SIZE_X 1920.f	
#define MAP_SCREEN_SIZE_Y 1080.f

sfBool isFreeingMap;
float timerWaterFAAAAALLLL;
float timerParticlesBrasier;

sfVector2f map_PosPressurePlateJoy;

static sfBool firstPassPotSpawn = sfFalse;

void map_InitMap(Window* _window)
{
	if (onePassMap)
	{
		timerWaterFAAAAALLLL = 0.f;
		timerParticlesBrasier = 0.f;
		onePassMap = sfFalse;
		propsList = STD_LIST_CREATE(LayerProps, 0);

		mapAssetsSprite = sfSprite_create();

		editorCollSprite = sfSprite_create();
		sfSprite_setTexture(editorCollSprite, GetSfTexture("collision"), sfTrue);

		mapRect = sfRectangleShape_create();
		sfRectangleShape_setSize(mapRect, vector2f(TILE_SIZE, TILE_SIZE));

		waterfallRect = IntRect(0, 0, 488, 488);

		if (getState() == GAME)
		{
			lhloc_Begin(_window);
			showCollision = sfFalse;
		}
		else
		{
			map_changeMapState(_window, MAP_STATE_OVERWORLD);
			showCollision = sfTrue;
		}

		showWallLayerFirst = sfTrue;
		showGroundLayerSecond = sfTrue;

		/// TEMPORAIRE 
		changeMapRectangle = sfRectangleShape_create();
		sfRectangleShape_setFillColor(changeMapRectangle, sfTransparent);
		sfRectangleShape_setOutlineColor(changeMapRectangle, sfBlue);
		sfRectangleShape_setOutlineThickness(changeMapRectangle, 5.f);
		/// TEMPORAIRE 

		isFreeingMap = sfFalse;
		map_TexAssetsDefault = GetSfTexture("assets_general");

		map_PosPressurePlateJoy = NULLVECTF;
	}
	else {
		switch (mapState) {
		case MAP_STATE_OVERWORLD:
		case MAP_STATE_FOREST: overWorld_initOverWorld(_window); break;

		case MAP_STATE_LIGHTHOUSE_BEDROOM:
		case MAP_STATE_LIGHTHOUSE_HALL:
		case MAP_STATE_LIGHTHOUSE_OUTSIDE:
		case MAP_STATE_LIGHTHOUSE_MENTOR:
		case MAP_STATE_LIGHTHOUSE_ANTECHAMBER:
		case MAP_STATE_LIGHTHOUSE_ROOF: loclh_Init(_window); break;

		case MAP_STATE_DUNGEON_JOY_1:
		case MAP_STATE_DUNGEON_JOY_2:
		case MAP_STATE_DUNGEON_JOY_3:
		case MAP_STATE_DUNGEON_JOY_4:
		case MAP_STATE_DUNGEON_JOY_5:
		case MAP_STATE_DUNGEON_JOY_6:
		case MAP_STATE_DUNGEON_JOY_BOSS:
		case MAP_STATE_DUNGEON_JOY_FOUNTAIN: dungeon_initDungeon(_window); break;
		case MAP_STATE_DUNGEON_SADNESS_BOSS: dungeon_initDungeon(_window); break;
		}
	}
}

void map_UpdateMap(Window* _window)
{
	if (getIsFreeingMap()) return;
	float dt = getDeltaTime();
	static float timer = 0.f;
	timer += dt;
	timerParticlesBrasier += dt;

	updateGrass();
	opDoors_UpdateDoor(_window);
	brz_Update();

	/// Change map if the player is in a special collider
	if (getState() == GAME)
	{
		for (int i = 0; i < nbDifferentAcces; i++)
		{
			switch (currentMap)
			{
			case MAP_STATE_OVERWORLD:
				if (sfIntRect_contains(&changeMapRect[i], plr_GetPos().x, plr_GetPos().y))
				{
					if (i == 0) map_changeMapState(_window, MAP_STATE_FOREST);
					else if(i == 1) map_changeMapState(_window, MAP_STATE_DUNGEON_JOY_1);
					else map_changeMapState(_window, MAP_STATE_CAVE);
				}
				break;
			case MAP_STATE_FOREST:
				if (sfIntRect_contains(&changeMapRect[i], plr_GetPos().x, plr_GetPos().y))
					map_changeMapState(_window, MAP_STATE_OVERWORLD);
				break;
			case MAP_STATE_DUNGEON_JOY_1:
				if (sfIntRect_contains(&changeMapRect[i], plr_GetPos().x, plr_GetPos().y))
				{
					if (i == 0) map_changeMapState(_window, MAP_STATE_OVERWORLD);
					else if (i == 1) map_changeMapState(_window, MAP_STATE_DUNGEON_JOY_2);
					else if (i == 2) map_changeMapState(_window, MAP_STATE_DUNGEON_JOY_BOSS);
				}
				break;
			case MAP_STATE_DUNGEON_JOY_2:
				if (sfIntRect_contains(&changeMapRect[i], plr_GetPos().x, plr_GetPos().y))
				{
					if (i == 0) map_changeMapState(_window, MAP_STATE_DUNGEON_JOY_1);
					else if (i == 1) map_changeMapState(_window, MAP_STATE_DUNGEON_JOY_3);
				}
				break;
			case MAP_STATE_DUNGEON_JOY_3:
				if (sfIntRect_contains(&changeMapRect[i], plr_GetPos().x, plr_GetPos().y))
				{
					if (i == 0) map_changeMapState(_window, MAP_STATE_DUNGEON_JOY_2);
					else if (i == 1) map_changeMapState(_window, MAP_STATE_DUNGEON_JOY_4);
					else if (i == 2) map_changeMapState(_window, MAP_STATE_DUNGEON_JOY_6);
				}
				break;
			case MAP_STATE_DUNGEON_JOY_4:
				if (sfIntRect_contains(&changeMapRect[i], plr_GetPos().x, plr_GetPos().y))
				{
					if (i == 0) map_changeMapState(_window, MAP_STATE_DUNGEON_JOY_3);
					else if (i == 1) map_changeMapState(_window, MAP_STATE_DUNGEON_JOY_5);
				}
				break;
			case MAP_STATE_DUNGEON_JOY_5:
				if (sfIntRect_contains(&changeMapRect[i], plr_GetPos().x, plr_GetPos().y))
				{
					if (i == 0) map_changeMapState(_window, MAP_STATE_DUNGEON_JOY_4);
					else if (i == 1) map_changeMapState(_window, MAP_STATE_DUNGEON_JOY_6);
				}
				break;
			case MAP_STATE_DUNGEON_JOY_6:
				if (sfIntRect_contains(&changeMapRect[i], plr_GetPos().x, plr_GetPos().y))
				{
					if (i == 0) map_changeMapState(_window, MAP_STATE_DUNGEON_JOY_5);
					else if (i == 1) map_changeMapState(_window, MAP_STATE_DUNGEON_JOY_3);
				}
				break;
			case MAP_STATE_DUNGEON_JOY_BOSS:
				if (sfIntRect_contains(&changeMapRect[i], plr_GetPos().x, plr_GetPos().y))
				{
					if (i == 0) map_changeMapState(_window, MAP_STATE_DUNGEON_JOY_1);
					else if (i == 1) map_changeMapState(_window, MAP_STATE_DUNGEON_JOY_FOUNTAIN);
				}
				break;
			case MAP_STATE_DUNGEON_JOY_FOUNTAIN:
				if (sfIntRect_contains(&changeMapRect[i], plr_GetPos().x, plr_GetPos().y))
					if (i == 0) map_changeMapState(_window, MAP_STATE_DUNGEON_JOY_BOSS);
				break;
			case MAP_STATE_LIGHTHOUSE_OUTSIDE:
				if (sfIntRect_contains(&changeMapRect[i], plr_GetPos().x, plr_GetPos().y))
					if (i == 0)
					{
						qst_Create(QST_ID_P_EXPLORE, PAUSE_MENTOR);
						map_changeMapState(_window, MAP_STATE_OVERWORLD);
						stopMusicIfPlaying("Music_River");
						stopMusicIfPlaying("Music_Phare");
						playMusicIfNotPlaying("Music_Main");
					}
				break;
			case MAP_STATE_CAVE:
				if (sfIntRect_contains(&changeMapRect[i], plr_GetPos().x, plr_GetPos().y))
					if (i == 0)  map_changeMapState(_window, MAP_STATE_MOUTAIN);
					else if (i == 1) map_changeMapState(_window, MAP_STATE_OVERWORLD);
				break;
			case MAP_STATE_MOUTAIN:
				if (sfIntRect_contains(&changeMapRect[i], plr_GetPos().x, plr_GetPos().y))
					if (i == 0)  map_changeMapState(_window, MAP_STATE_CAVE);
					else if (i == 1) map_changeMapState(_window, MAP_STATE_DUNGEON_SADNESS_1);
				break;
			case MAP_STATE_DUNGEON_SADNESS_1:
				if (sfIntRect_contains(&changeMapRect[i], plr_GetPos().x, plr_GetPos().y))
					if (i == 0)  map_changeMapState(_window, MAP_STATE_MOUTAIN);
					else if (i == 1) map_changeMapState(_window, MAP_STATE_DUNGEON_SADNESS_2);
				break;
			case MAP_STATE_DUNGEON_SADNESS_2:
				if (sfIntRect_contains(&changeMapRect[i], plr_GetPos().x, plr_GetPos().y))
					if (i == 0)  map_changeMapState(_window, MAP_STATE_DUNGEON_SADNESS_1);
					else if (i == 1) map_changeMapState(_window, MAP_STATE_DUNGEON_SADNESS_BOSS);
				break;
			case MAP_STATE_DUNGEON_SADNESS_BOSS:
				if (sfIntRect_contains(&changeMapRect[i], plr_GetPos().x, plr_GetPos().y))
					if (i == 0)  map_changeMapState(_window, MAP_STATE_DUNGEON_SADNESS_2);
				break;
			}
		}
	}
	if (getState() == EDITOR)
	{
		/// Display or not layerWall first
		if (customKeyboardIsKeyPressed(sfKeyC, _window->renderWindow) && timer > 0.2f)
		{
			timer = 0.f;
			showWallLayerFirst = !showWallLayerFirst;
		}

		if (customKeyboardIsKeyPressed(sfKeyX, _window->renderWindow) && timer > 0.2f)
		{
			timer = 0.f;
			showGroundLayerSecond = !showGroundLayerSecond;
		}
	}

	/// Display or not collision
	if (customKeyboardIsKeyPressed(sfKeyI, _window->renderWindow) && timer > 0.2f)
	{
		timer = 0.f;
		showCollision = !showCollision;
	}

	sfVector2f plrPos = plr_GetPos();
	sfFloatRect plateHitboxJoy = FlRect(map_PosPressurePlateJoy.x, map_PosPressurePlateJoy.y, 128.f, 128.f);
	static sfBool isGateOpenJoy = sfFalse;

	///Penser � faire la condition qui ouvre la porte du donjon de joie
	///Cas general pour les autres donjons potentiellemet a ignorer pour la pres
	if (sfFloatRect_contains(&plateHitboxJoy, plrPos.x, plrPos.y) && !isGateOpenJoy && STD_LIST_GETDATA(itemList, Itemlist, 10)->nbObject == 3)
	{
		for (int i = 0; i < propsList->size(propsList); i++)
		{
			if (getIsFreeingMap()) return;

			LayerProps* tmpDungeon = STD_LIST_GETDATA(propsList, LayerProps, i);
			if (tmpDungeon->dungeon == PROPS_DUNGEON_DOOR_JOY)
			{
				tmpDungeon->position.y -= 50.f * getDeltaTime();
				plr_StopMoving();
				sfVector2f posDungeon = vector2f(0.f, 0.f);
				for (int i = 0; i < propsList->size(propsList); i++)
				{
					LayerProps* tmpDungeon2 = STD_LIST_GETDATA(propsList, LayerProps, i);
					if (tmpDungeon2->dungeon == PROPS_DUNGEON_BUILD_JOY)
					{
						posDungeon = tmpDungeon2->position;
						break;
					}
				}
				if (tmpDungeon->position.y < posDungeon.y + 1152.f) {
					propsList->erase(&propsList, i);
					isGateOpenJoy = sfTrue;
				}
			}
		}
	}

	/// Computing collisions with dungeon doors
	for (int i = 0; i < propsList->size(propsList); i++) {
		if (getIsFreeingMap()) return;
		LayerProps* tmp = STD_LIST_GETDATA(propsList, LayerProps, i);
		if (tmp->dungeon == PROPS_DUNGEON_DOOR_JOY || tmp->dungeon == PROPS_DUNGEON_DOOR_SADNESS || tmp->dungeon == PROPS_DUNGEON_DOOR_FEAR || tmp->dungeon == PROPS_DUNGEON_DOOR_ANGRY) {
			sfFloatRect rectDoor = FlRect(tmp->position.x, tmp->position.y, 228.f, 186.f);
			sfVector2f plrPos2 = addVectorsrf(plr_GetPos(), multiplyVectorsrf(vector2f(plr_GetVel().x, 0.f), getDeltaTime()));
			if (sfFloatRect_contains(&rectDoor, plrPos2.x, plrPos2.y)) {
				plr_ForceMove(vector2f(plr_GetVel().x * -getDeltaTime(), 0.f));
			}

			plrPos2 = addVectorsrf(plr_GetPos(), multiplyVectorsrf(vector2f(0.f, plr_GetVel().y), getDeltaTime()));
			if (sfFloatRect_contains(&rectDoor, plrPos2.x, plrPos2.y)) {
				plr_ForceMove(vector2f(0.f, plr_GetVel().y * -getDeltaTime()));
			}
		}
	}

	switch (mapState) {
	case MAP_STATE_OVERWORLD:
	case MAP_STATE_FOREST: overWorld_updateOverWorld(_window); break;

	case MAP_STATE_LIGHTHOUSE_BEDROOM:
	case MAP_STATE_LIGHTHOUSE_HALL:
	case MAP_STATE_LIGHTHOUSE_OUTSIDE:
	case MAP_STATE_LIGHTHOUSE_MENTOR:
	case MAP_STATE_LIGHTHOUSE_ANTECHAMBER:
	case MAP_STATE_LIGHTHOUSE_ROOF: loclh_Update(_window); break;

	case MAP_STATE_DUNGEON_JOY_1:
	case MAP_STATE_DUNGEON_JOY_2:
	case MAP_STATE_DUNGEON_JOY_3:
	case MAP_STATE_DUNGEON_JOY_4:
	case MAP_STATE_DUNGEON_JOY_5:
	case MAP_STATE_DUNGEON_JOY_6:
	case MAP_STATE_DUNGEON_JOY_BOSS:
	case MAP_STATE_DUNGEON_JOY_FOUNTAIN: dungeon_updateDungeon(_window); break;
	case MAP_STATE_DUNGEON_SADNESS_BOSS: dungeon_updateDungeon(_window); break;
	}
	
	if(DEBUG_STATE)
		printf("x : %.2f // y : %.2f\n", getfWorldMousePos(_window->renderWindow).x, getfWorldMousePos(_window->renderWindow).y);
}

void map_DisplayMapBehindPlayer(Window* _window)
{
	float dtAnim = getDeltaTime();
	if (mapState != MAP_STATE_NO_MAP)
	{
		/// DISPLAY MAP STATE		
		switch (mapState) {
			case MAP_STATE_OVERWORLD:
			case MAP_STATE_FOREST: overWorld_displayOverWorld(_window); break;

			case MAP_STATE_LIGHTHOUSE_BEDROOM:
			case MAP_STATE_LIGHTHOUSE_HALL:
			case MAP_STATE_LIGHTHOUSE_OUTSIDE:
			case MAP_STATE_LIGHTHOUSE_MENTOR:
			case MAP_STATE_LIGHTHOUSE_ANTECHAMBER:
			case MAP_STATE_LIGHTHOUSE_ROOF: loclh_DisplayBG(_window); break;

			case MAP_STATE_DUNGEON_JOY_1:
			case MAP_STATE_DUNGEON_JOY_2:
			case MAP_STATE_DUNGEON_JOY_3:
			case MAP_STATE_DUNGEON_JOY_4:
			case MAP_STATE_DUNGEON_JOY_5:
			case MAP_STATE_DUNGEON_JOY_6:
			case MAP_STATE_DUNGEON_JOY_BOSS:
			case MAP_STATE_DUNGEON_JOY_FOUNTAIN: dungeon_displayDungeon(_window); break;
			case MAP_STATE_DUNGEON_SADNESS_BOSS: dungeon_displayDungeon(_window); break;
		}

		/// OPTIMIZE THE FOR LOOP
		if (getIsFreeingMap()) return;

		sfVector2f viewPos = getViewPosition(mainView);
		sfVector2f viewSize = sfView_getSize(mainView->view);

		sfVector2i startValue = vector2i(0, 0);
		sfVector2i endValue = vector2i((viewPos.x / TILE_SIZE) + (viewSize.x / (TILE_SIZE * 2)) + 1, (viewPos.y / TILE_SIZE) + (viewSize.y / (TILE_SIZE * 2)) + 1);

		if (viewPos.x > viewSize.x / 2.f)
			startValue.x = (viewPos.x / TILE_SIZE) - (viewSize.x / (TILE_SIZE * 2));


		if (endValue.x > map.mapSize.x)
			endValue.x = map.mapSize.x;

		if (viewPos.y > viewSize.y / 2.f)
			startValue.y = (viewPos.y / TILE_SIZE) - (viewSize.y / (TILE_SIZE * 2));

		if (endValue.y > map.mapSize.y)
			endValue.y = map.mapSize.y;

		/// DISPLAY TILES/WALLS
		for (int i = startValue.y; i < endValue.y; i++)
		{
			for (int j = startValue.x; j < endValue.x; j++)
			{
				if (getIsFreeingMap()) return;
				/// Load texture tiles
				sfRectangleShape_setTexture(mapRect, map_getBiomeTexture(LAYER_GROUND_FIRST, i, j, NULL), sfFalse);
				sfRectangleShape_setTextureRect(mapRect, map_GetTileTexRect(map.layerGround_first[i][j]));
				sfRectangleShape_setScale(mapRect, vector2f(1.05, 1.05)); 
				sfRectangleShape_setPosition(mapRect, vector2f(j * TILE_SIZE, i * TILE_SIZE));
				sfRenderTexture_drawRectangleShape(_window->rdrt, mapRect, NULL);

				if (showGroundLayerSecond)
				{
					sfRectangleShape_setTexture(mapRect, map_getBiomeTexture(LAYER_GROUND_SECOND, i, j, NULL), sfFalse);
					sfRectangleShape_setTextureRect(mapRect, map_GetTileTexRect(map.layerGround_second[i][j]));
					sfRectangleShape_setScale(mapRect, vector2f(1.05, 1.05));
					sfRectangleShape_setPosition(mapRect, vector2f(j * TILE_SIZE, i * TILE_SIZE));
					sfRenderTexture_drawRectangleShape(_window->rdrt, mapRect, NULL);
				}

				if (showWallLayerFirst)
				{
					/// Load texture walls
					sfRectangleShape_setTexture(mapRect, map_getBiomeTexture(LAYER_WALLS_FIRST, i, j, NULL), sfTrue);
					sfRectangleShape_setTextureRect(mapRect, map_GetWallTexRect(map.layerWalls_first[i][j]));
					sfRectangleShape_setScale(mapRect, vector2f(1.05, 1.05)); 
					sfRectangleShape_setPosition(mapRect, vector2f(j * TILE_SIZE, i * TILE_SIZE));
					sfRenderTexture_drawRectangleShape(_window->rdrt, mapRect, NULL);
				}
			}
		}

		/// DISPLAY PROPS
		for (int i = 0; i < propsList->size(propsList); i++)
		{
			if (getIsFreeingMap()) return;
			LayerProps* tmp = STD_LIST_GETDATA(propsList, LayerProps, i);
			if (tmp->isBehind)
			{
				sfIntRect propsSize = map_GetPropsTexRect(*tmp);

				if (tmp->position.x < startValue.x * TILE_SIZE - propsSize.width || tmp->position.x > endValue.x * TILE_SIZE + propsSize.width || tmp->position.y < startValue.y * TILE_SIZE - propsSize.height || tmp->position.y > endValue.y * TILE_SIZE + propsSize.width) {
					if (tmp->forest != PROPS_FOREST_WATERFALL)
						continue;
				}
				/// Load texture props
				sfSprite_setTexture(mapAssetsSprite, map_getBiomeTexture(LAYER_PROPS, 0, 0, tmp), sfTrue);
				switch (tmp->type)
				{
				case PROPS_TYPE_FOREST:
					if (tmp->forest == PROPS_FOREST_TREE_ANIM)
					{
						sfSprite_setTextureRect(mapAssetsSprite, animatorAnim("forest_tree_1", &tmp->frameX, &tmp->timerAnim, 0));
					}
					else if (tmp->forest == PROPS_FOREST_SPLASH)
					{
						sfSprite_setTextureRect(mapAssetsSprite, animatorAnim("splashWaterAnim", &tmp->frameX, &tmp->timerAnim, 0));
					}
					else if (tmp->forest == PROPS_FOREST_WATERFALL && getState() != EDITOR)
					{
						timerWaterFAAAAALLLL += getDeltaTime() / 3.f;
						waterfallAnimator(&waterfallRect, 6, 2, 0.2f, &timerWaterFAAAAALLLL);
						sfSprite_setTextureRect(mapAssetsSprite, waterfallRect);
					}
					else
					{
						sfSprite_setTextureRect(mapAssetsSprite, propsSize);
					}
					break;
				case PROPS_TYPE_BEACH:
					switch (tmp->beach)
					{
					case PROPS_BEACH_TRUNK_1:
						sfSprite_setTextureRect(mapAssetsSprite, animatorAnim("beach_palm_trunk_1", &tmp->frameX, &tmp->timerAnim, 0));
						break;
					case PROPS_BEACH_TRUNK_2:
						sfSprite_setTextureRect(mapAssetsSprite, animatorAnim("beach_palm_trunk_2", &tmp->frameX, &tmp->timerAnim, 0));
						break;
					default:
						sfSprite_setTextureRect(mapAssetsSprite, propsSize);
						break;
					}
					break;
				case PROPS_TYPE_GENERAL:
					if (tmp->general == PROPS_GENERAL_FOUNTAIN)
					{
						sfSprite_setTextureRect(mapAssetsSprite, animatorAnim("general_fountain", &tmp->frameX, &tmp->timerAnim, 0));
					}
					else if (tmp->general == PROPS_GENERAL_FLOWER)
					{
						sfSprite_setTextureRect(mapAssetsSprite, animatorAnim("general_flower", &tmp->frameX, &tmp->timerAnim, 0));
					}
					else if (tmp->general == PROPS_GENERAL_FISHING_SPOT)
					{
						sfSprite_setTextureRect(mapAssetsSprite, animatorAnim("general_fishing_spot", &tmp->frameX, &tmp->timerAnim, 0));
					}
					else if (tmp->general == PROPS_GENERAL_BUBBLES)
					{
						sfSprite_setTextureRect(mapAssetsSprite, animatorAnim("general_bubbles", &tmp->frameX, &tmp->timerAnim, 0));
					}
					else if (tmp->general == PROPS_GENERAL_GROUND_NOTES)
					{
						sfSprite_setTextureRect(mapAssetsSprite, animatorAnim("general_ground_notes", &tmp->frameX, &tmp->timerAnim, 0));
					}
					else if (tmp->general == PROPS_GENERAL_FLOATING_NOTES)
					{
						sfSprite_setTextureRect(mapAssetsSprite, animatorAnim("general_floating_notes", &tmp->frameX, &tmp->timerAnim, 0));
					}
					else
					{
						sfSprite_setTextureRect(mapAssetsSprite, propsSize);
					}
					break;
				case PROPS_TYPE_DUNGEON:
					switch (tmp->dungeon)
					{
					case PROPS_DUNGEON_BOULDER:
						sfSprite_setTextureRect(mapAssetsSprite, animatorAnim("dungeon_boulder", &tmp->frameX, &tmp->timerAnim, 0));
						break;
					case PROPS_DUNGEON_PAVING_STONE:
						sfSprite_setTextureRect(mapAssetsSprite, animatorAnim("dungeon_pav_stone", &tmp->frameX, &tmp->timerAnim, 0));
						break;
					case PROPS_DUNGEON_PRESSURE_PLATE:
						sfSprite_setTextureRect(mapAssetsSprite, animatorAnim("dungeonPlcH_Press_plate", &tmp->frameX, &tmp->timerAnim, 0));
						break;
					case PROPS_DUNGEON_BRAZIER:
						sfSprite_setTextureRect(mapAssetsSprite, animatorAnim("dungeonPlcH_Brazier", &tmp->frameX, &tmp->timerAnim, 0));
						break;
					case PROPS_DUNGEON_BANNER:
						sfSprite_setTextureRect(mapAssetsSprite, animatorAnim("dungeonPlcH_Banner", &tmp->frameX, &tmp->timerAnim, 0));
						break;
					case PROPS_DUNGEON_LITTLE_CHEST:
						sfSprite_setTextureRect(mapAssetsSprite, animatorAnim("dungeonPlcH_litlChest", &tmp->frameX, &tmp->timerAnim, 0));
						break;
					case PROPS_DUNGEON_BIG_CHEST:
						sfSprite_setTextureRect(mapAssetsSprite, animatorAnim("dungeonPlcH_bigChest", &tmp->frameX, &tmp->timerAnim, 0));
						break;
					case PROPS_DUNGEON_ENTRANCE_PRESSURE_PLATE_JOY:
						sfSprite_setTextureRect(mapAssetsSprite, map_GetPropsTexRect(*tmp));
						//sfSprite_setTextureRect(mapAssetsSprite, animatorAnim("dungeon_entrance_plate_joy", &tmp->frameX, &tmp->timerAnim, 0));
						break;
					case PROPS_DUNGEON_ENTRANCE_PRESSURE_PLATE_SADNESS:
						sfSprite_setTextureRect(mapAssetsSprite, animatorAnim("dungeon_entrance_plate_sadness", &tmp->frameX, &tmp->timerAnim, 0));
						break;
					case PROPS_DUNGEON_ENTRANCE_PRESSURE_PLATE_FEAR:
						sfSprite_setTextureRect(mapAssetsSprite, animatorAnim("dungeon_entrance_plate_fear", &tmp->frameX, &tmp->timerAnim, 0));
						break;
					case PROPS_DUNGEON_ENTRANCE_PRESSURE_PLATE_ANGRY:
						sfSprite_setTextureRect(mapAssetsSprite, animatorAnim("dungeon_entrance_plate_angry", &tmp->frameX, &tmp->timerAnim, 0));
						break;
					case PROPS_DUNGEON_DOOR_JOY:
						//sfkeym
						sfSprite_setTextureRect(mapAssetsSprite, IntRect(0, 0, 0, 0));
						sfVector2f posDungeon = vector2f(0.f, 0.f);
						for (int i = 0; i < propsList->size(propsList); i++)
						{
							LayerProps* tmpDungeon2 = STD_LIST_GETDATA(propsList, LayerProps, i);
							if (tmpDungeon2->dungeon == PROPS_DUNGEON_BUILD_JOY)
							{
								posDungeon = tmpDungeon2->position;
							}
						}
						float calcul = 1338 - (tmp->position.y - posDungeon.y);
						if (calcul > 186.f)
							calcul = 186.f;
						sfSprite_setTextureRect(mapAssetsSprite, IntRect(0, 1802 + calcul, 228, 186 - calcul));
						sfSprite_setPosition(mapAssetsSprite, addVectorsrf(tmp->position, vector2f(0.f, calcul)));
						break;
					case PROPS_DUNGEON_DOOR_SADNESS:
						sfSprite_setTextureRect(mapAssetsSprite, IntRect(0, 0, 0, 0));
						sfVector2f posDungeonSadness = vector2f(0.f, 0.f);
						for (int i = 0; i < propsList->size(propsList); i++)
						{
							LayerProps* tmpDungeon2 = STD_LIST_GETDATA(propsList, LayerProps, i);
							if (tmpDungeon2->dungeon == PROPS_DUNGEON_BUILD_FRONT_SADNESS)
							{
								posDungeonSadness = tmpDungeon2->position;
							}
						}
						float calculSadness = 1338 - (tmp->position.y - posDungeonSadness.y);
						if (calculSadness > 186.f)
							calculSadness = 186.f;
						sfSprite_setTextureRect(mapAssetsSprite, IntRect(0, 1802 + calculSadness, 228, 186 - calculSadness));
						sfSprite_setPosition(mapAssetsSprite, addVectorsrf(tmp->position, vector2f(0.f, calculSadness)));
						break;
					case PROPS_DUNGEON_DOOR_FEAR:
						sfSprite_setTextureRect(mapAssetsSprite, IntRect(0, 0, 0, 0));
						sfVector2f posDungeonFear = vector2f(0.f, 0.f);
						for (int i = 0; i < propsList->size(propsList); i++)
						{
							LayerProps* tmpDungeon2 = STD_LIST_GETDATA(propsList, LayerProps, i);
							if (tmpDungeon2->dungeon == PROPS_DUNGEON_BUILD_FRONT_FEAR)
							{
								posDungeonFear = tmpDungeon2->position;
							}
						}
						float calculFear = 1338 - (tmp->position.y - posDungeonFear.y);
						if (calculFear > 186.f)
							calculFear = 186.f;
						sfSprite_setTextureRect(mapAssetsSprite, IntRect(0, 1802 + calculFear, 228, 186 - calculFear));
						sfSprite_setPosition(mapAssetsSprite, addVectorsrf(tmp->position, vector2f(0.f, calculFear)));
						break;
					case PROPS_DUNGEON_DOOR_ANGRY:
						sfSprite_setTextureRect(mapAssetsSprite, IntRect(0, 0, 0, 0));
						sfVector2f posDungeonAngry = vector2f(0.f, 0.f);
						for (int i = 0; i < propsList->size(propsList); i++)
						{
							LayerProps* tmpDungeon2 = STD_LIST_GETDATA(propsList, LayerProps, i);
							if (tmpDungeon2->dungeon == PROPS_DUNGEON_BUILD_FRONT_ANGRY)
							{
								posDungeonAngry = tmpDungeon2->position;
							}
						}
						float calculAngry = 1338 - (tmp->position.y - posDungeonAngry.y);
						if (calculAngry > 186.f)
							calculAngry = 186.f;
						sfSprite_setTextureRect(mapAssetsSprite, IntRect(0, 1802 + calculAngry, 228, 186 - calculAngry));
						sfSprite_setPosition(mapAssetsSprite, addVectorsrf(tmp->position, vector2f(0.f, calculAngry)));
						break;
					default:
						sfSprite_setTextureRect(mapAssetsSprite, propsSize);
						break;
					}
					break;
				default:
					sfSprite_setTextureRect(mapAssetsSprite, propsSize);
					break;
				}
				if (tmp->dungeon != PROPS_DUNGEON_DOOR_JOY && tmp->dungeon != PROPS_DUNGEON_DOOR_SADNESS && tmp->dungeon != PROPS_DUNGEON_DOOR_FEAR && tmp->dungeon != PROPS_DUNGEON_DOOR_ANGRY)
					sfSprite_setPosition(mapAssetsSprite, tmp->position);
				sfRenderTexture_drawSprite(_window->rdrt, mapAssetsSprite, NULL);
			}
		}

		sfSprite_setTexture(mapAssetsSprite, map_TexAssetsDefault, sfTrue);
		for (int i = 0; i < note_ListNote->size(note_ListNote); i++)
		{
			Note* tmp = STD_LIST_GETDATA(note_ListNote, Note, i); /// Alias to make things more readable

			sfSprite_setPosition(mapAssetsSprite, tmp->pos);
			sfSprite_setTextureRect(mapAssetsSprite, animatorAnim(tmp->type == NOTE_DRY ? "general_ground_notes" : "general_floating_notes", &tmp->frameX, &tmp->timerAnim, 0));
			sfRenderTexture_drawSprite(_window->rdrt, mapAssetsSprite, NULL);
		}

		if (DEBUG_STATE)
		{
			for (int i = 0; i < nbDifferentAcces; i++)
			{
				sfRectangleShape_setPosition(changeMapRectangle, vector2f(changeMapRect[i].left, changeMapRect[i].top));
				sfRectangleShape_setSize(changeMapRectangle, vector2f(changeMapRect[i].width, changeMapRect[i].height));
				sfRenderTexture_drawRectangleShape(_window->rdrt, changeMapRectangle, NULL);
			}
		}

		if (getState() != EDITOR)
		{
			if (getIsFreeingMap()) return;
			pot_Display(_window);
			stt_Display(_window);
			chest_DisplayChest(_window);
			box_Display(_window);
			displayGrass(_window);
			brz_Display(_window);

			brz_Display(_window);
			opDoors_DisplayDoor(_window);
			stree_displaySpidersTreesBehindPlayer(_window);
			fishspot_Display(_window);
		}
	}
}

void map_DisplayMapInFrontOfPlayer(Window* _window)
{
	if (getIsFreeingMap()) return;
	if (mapState != MAP_STATE_NO_MAP)
	{
		/// OPTIMIZE THE FOR LOOP
		sfVector2f viewPos = getViewPosition(mainView);
		sfVector2f viewSize = sfView_getSize(mainView->view);

		sfVector2i startValue = vector2i(0, 0);

		if (getIsFreeingMap()) return;
		sfVector2i endValue = vector2i((viewPos.x / TILE_SIZE) + (viewSize.x / (TILE_SIZE * 2)) + 1, (viewPos.y / TILE_SIZE) + (viewSize.y / (TILE_SIZE * 2)) + 1);

		if (viewPos.x > viewSize.x / 2.f)
			startValue.x = (viewPos.x / TILE_SIZE) - (viewSize.x / (TILE_SIZE * 2));

		if (endValue.x > map.mapSize.x)
			endValue.x = map.mapSize.x;

		if (viewPos.y > viewSize.y / 2.f)
			startValue.y = (viewPos.y / TILE_SIZE) - (viewSize.y / (TILE_SIZE * 2));

		if (endValue.y > map.mapSize.y)
			endValue.y = map.mapSize.y;

		/// DISPLAY SECOND LAYER WALLS
		for (int i = startValue.y; i < endValue.y; i++)
		{
			for (int j = startValue.x; j < endValue.x; j++)
			{
				if (getIsFreeingMap()) return;
				/// Load texture walls
				sfRectangleShape_setTexture(mapRect, map_getBiomeTexture(LAYER_WALLS_SECOND, i, j, NULL), sfTrue);
				sfRectangleShape_setTextureRect(mapRect, map_GetWallTexRect(map.layerWalls_second[i][j]));
				sfRectangleShape_setPosition(mapRect, vector2f(j * TILE_SIZE, i * TILE_SIZE));
				sfRenderTexture_drawRectangleShape(_window->rdrt, mapRect, NULL);
			}
		}

		/// DISPLAY PROPS SECONDS
		for (int i = 0; i < propsList->size(propsList); i++)
		{
			if (getIsFreeingMap()) return;
			LayerProps* tmp = STD_LIST_GETDATA(propsList, LayerProps, i);

			if (!tmp->isBehind)
			{
				sfIntRect propsSize = map_GetPropsTexRect(*tmp);

				if (tmp->position.x < startValue.x * TILE_SIZE - propsSize.width || tmp->position.x > endValue.x * TILE_SIZE + propsSize.width || tmp->position.y < startValue.y * TILE_SIZE - propsSize.height || tmp->position.y > endValue.y * TILE_SIZE + propsSize.width)
					continue;

				sfSprite_setTexture(mapAssetsSprite, map_getBiomeTexture(LAYER_PROPS, 0, 0, tmp), sfTrue);
				switch (tmp->type)
				{
				case PROPS_TYPE_FOREST:
					if (tmp->forest == PROPS_FOREST_TREE_ANIM)
					{
						sfSprite_setTextureRect(mapAssetsSprite, animatorAnim("forest_tree_1", &tmp->frameX, &tmp->timerAnim, 0));
					}
					else
					{
						sfSprite_setTextureRect(mapAssetsSprite, propsSize);
					}
					break;
				case PROPS_TYPE_BEACH:
					switch (tmp->beach)
					{
					case PROPS_BEACH_PALM_1:
						sfSprite_setTextureRect(mapAssetsSprite, animatorAnim("beach_palm_1", &tmp->frameX, &tmp->timerAnim, 0));
						break;
					case PROPS_BEACH_PALM_2:
						sfSprite_setTextureRect(mapAssetsSprite, animatorAnim("beach_palm_2", &tmp->frameX, &tmp->timerAnim, 0));
						break;
					default:
						sfSprite_setTextureRect(mapAssetsSprite, propsSize);
						break;
					}
					break;
				case PROPS_TYPE_DUNGEON:
					switch (tmp->dungeon)
					{
					case PROPS_DUNGEON_HINT_STATUE:
						sfSprite_setTextureRect(mapAssetsSprite, animatorAnim("dungeon_hint_statue", &tmp->frameX, &tmp->timerAnim, 0));
						break;
					default:
						sfSprite_setTextureRect(mapAssetsSprite, propsSize);
						break;
					}
					break;
				default:
					sfSprite_setTextureRect(mapAssetsSprite, propsSize);
					break;
				}
				sfSprite_setPosition(mapAssetsSprite, tmp->position);
				sfRenderTexture_drawSprite(_window->rdrt, mapAssetsSprite, NULL);
			}
		}

		/// DISPLAY LEAF TILES
		for (int i = startValue.y; i < endValue.y; i++)
		{
			if (getIsFreeingMap()) return;
			for (int j = startValue.x; j < endValue.x; j++)
			{
				if (getIsFreeingMap()) return;
				if (map.layerGround_second[i][j].type >= TILE_LEAF_FULL && map.layerGround_second[i][j].type < TILE_GROUND_DIRT_1)
				{
					sfRectangleShape_setTexture(mapRect, map_getBiomeTexture(LAYER_GROUND_SECOND, i, j, NULL), sfFalse);
					sfRectangleShape_setTextureRect(mapRect, map_GetTileTexRect(map.layerGround_second[i][j]));
					sfRectangleShape_setPosition(mapRect, vector2f(j * TILE_SIZE, i * TILE_SIZE));
					sfRenderTexture_drawRectangleShape(_window->rdrt, mapRect, NULL);
				}
			}
		}

		/// DISPLAY COLLISIONS
		if ((DEBUG_STATE || getState() == EDITOR) && showCollision)
		{
			for (int i = startValue.y; i < endValue.y; i++)
			{
				for (int j = startValue.x; j < endValue.x; j++)
				{
					sfIntRect rect = { 0 };
					switch (map.layerCollision[i][j].type)
					{
					case CLD_BOX:					rect = map_GetTilesetRect(0, 0);  break;
					case CLD_DIAG_TOP_LEFT:			rect = map_GetTilesetRect(3, 0);  break;
					case CLD_DIAG_TOP_RIGHT:		rect = map_GetTilesetRect(4, 0);  break;
					case CLD_DIAG_BOTTOM_RIGHT:		rect = map_GetTilesetRect(1, 0);  break;
					case CLD_DIAG_BOTTOM_LEFT:		rect = map_GetTilesetRect(2, 0);  break;
					case CLD_CORN_TOP_LEFT:			rect = map_GetTilesetRect(6, 0);  break;
					case CLD_CORN_TOP_RIGHT:		rect = map_GetTilesetRect(5, 0);  break;
					case CLD_CORN_BOTTOM_RIGHT:		rect = map_GetTilesetRect(7, 0);  break;
					case CLD_CORN_BOTTOM_LEFT:		rect = map_GetTilesetRect(8, 0);  break;
					case CLD_HALF_LEFT:				rect = map_GetTilesetRect(9, 0);  break;
					case CLD_HALF_RIGHT:			rect = map_GetTilesetRect(10, 0); break;
					case CLD_HALF_TOP:				rect = map_GetTilesetRect(11, 0); break;
					case CLD_HALF_BOTTOM:			rect = map_GetTilesetRect(12, 0); break;
					case CLD_ANGLE_TOP_LEFT:		rect = map_GetTilesetRect(13, 0); break;
					case CLD_ANGLE_TOP_RIGHT:		rect = map_GetTilesetRect(14, 0); break;
					case CLD_ANGLE_BOTTOM_RIGHT:	rect = map_GetTilesetRect(15, 0); break;
					case CLD_ANGLE_BOTTOM_LEFT:		rect = map_GetTilesetRect(16, 0); break;
					case CLD_SMALL_DIAG_TOP_LEFT:	rect = map_GetTilesetRect(17, 0); break;
					case CLD_SMALL_DIAG_TOP_RIGHT:	rect = map_GetTilesetRect(18, 0); break;
					case CLD_SMALL_TRI_TL:			rect = map_GetTilesetRect(19, 0); break;
					case CLD_SMALL_TRI_TR:			rect = map_GetTilesetRect(20, 0); break;
					case CLD_SMALL_TRI_BR:			rect = map_GetTilesetRect(21, 0); break;
					case CLD_SMALL_TRI_BL:			rect = map_GetTilesetRect(22, 0); break;
					case CLD_VOID:			        rect = map_GetTilesetRect(23, 0); break;
					default: break;
					}
					sfSprite_setTextureRect(editorCollSprite, rect);
					sfSprite_setPosition(editorCollSprite, vector2f(j * TILE_SIZE, i * TILE_SIZE));
					if (getState() != EDITOR) sfSprite_setColor(editorCollSprite, color(255, 255, 255, 150));
					sfRenderTexture_drawSprite(_window->rdrt, editorCollSprite, NULL);
					sfSprite_setColor(editorCollSprite, color(255, 255, 255, 255));
				}
			}
		}
		/// DISPLAY LIGHTHOUSE 

		switch (mapState) {
		case MAP_STATE_OVERWORLD:
		case MAP_STATE_FOREST: overWorld_displayOverWorld(_window); break;

		case MAP_STATE_LIGHTHOUSE_BEDROOM:
		case MAP_STATE_LIGHTHOUSE_HALL:
		case MAP_STATE_LIGHTHOUSE_OUTSIDE:
		case MAP_STATE_LIGHTHOUSE_MENTOR:
		case MAP_STATE_LIGHTHOUSE_ANTECHAMBER:
		case MAP_STATE_LIGHTHOUSE_ROOF: loclh_DisplayFG(_window); break;

		case MAP_STATE_DUNGEON_JOY_1:
		case MAP_STATE_DUNGEON_JOY_2:
		case MAP_STATE_DUNGEON_JOY_3:
		case MAP_STATE_DUNGEON_JOY_4:
		case MAP_STATE_DUNGEON_JOY_5:
		case MAP_STATE_DUNGEON_JOY_6:
		case MAP_STATE_DUNGEON_JOY_BOSS:
		case MAP_STATE_DUNGEON_JOY_FOUNTAIN: dungeon_displayDungeon(_window); break;
		case MAP_STATE_DUNGEON_SADNESS_BOSS: dungeon_displayDungeon(_window); break;
		}

		/// DISPLAY BOX
		box_DisplayAbove(_window);
		stree_displaySpiderTreesInFrontOfPlayer(_window);
	}
}

void map_DeinitMap()
{
	if (mapState >= MAP_STATE_LIGHTHOUSE_BEDROOM) loclh_Deinit();
	/// Deinit many linked list and pointers
	//Enviro
	map_FreeMap();

	//Props
	map_clearPropsList();
	box_Clear();
	pot_Clear();
	mvpl_clearList();
	expt_clearList();
	chest_clearChestList();
	clearGrass();
	fishspot_Clear();
	stree_clearSpidersTrees();
	note_clearNoteList();
	brz_Clear();
	opDoors_clearDoorsList();
	stt_clear();

	//Entity
	light_Clear();
	en_Clear();
	npc_Clear();

	prt_clear();
}

void map_addProps(PropsType _type, int _props, sfVector2f _pos, sfBool _load)
{
	LayerProps tmp;
	tmp.type = _type;
	tmp.isBehind = sfTrue;
	tmp.position = _pos;
	tmp.frameX = 0;
	tmp.timerAnim = 0.f;
	int x = 0;
	int y = 0;
	if (_pos.x >= 0.f && _pos.y >= 0.f && _pos.x < TILE_SIZE * map.mapSize.x && _pos.y < TILE_SIZE * map.mapSize.y)
	{
		x = _pos.x / TILE_SIZE;
		y = _pos.y / TILE_SIZE;
	}

	switch (tmp.type)
	{
	case PROPS_TYPE_FOREST:
		tmp.forest = _props;
		switch (tmp.forest)
		{
		case PROPS_FOREST_GRASS_1: tmp.position = vector2f(x * TILE_SIZE, y * TILE_SIZE); break;
		case PROPS_FOREST_GRASS_2: tmp.position = vector2f(x * TILE_SIZE, y * TILE_SIZE); break;
		case PROPS_FOREST_GRASS_3: tmp.position = vector2f(x * TILE_SIZE, y * TILE_SIZE); break;
		default: tmp.position = _pos; break;
		}
		break;
	case PROPS_TYPE_DUNGEON:
		tmp.dungeon = _props;
		switch (tmp.dungeon)
		{
		case PROPS_DUNGEON_POT_1: tmp.position = vector2f(x * TILE_SIZE, y * TILE_SIZE); break;
		case PROPS_DUNGEON_POT_2: tmp.position = vector2f(x * TILE_SIZE, y * TILE_SIZE); break;
		case PROPS_DUNGEON_MOVING_WALL_TOP: tmp.position = vector2f(x * TILE_SIZE, y * TILE_SIZE); break;
		case PROPS_DUNGEON_MOVING_WALL_MID: tmp.position = vector2f(x * TILE_SIZE, y * TILE_SIZE); break;
		case PROPS_DUNGEON_MOVING_PLATEFORM: tmp.position = vector2f(x * TILE_SIZE, y * TILE_SIZE); break;
		case PROPS_DUNGEON_FLYER_BRIDGE_LEFT: tmp.position = vector2f(x * TILE_SIZE, y * TILE_SIZE); break;
		case PROPS_DUNGEON_FLYER_BRIDGE_RIGHT: tmp.position = vector2f(x * TILE_SIZE, y * TILE_SIZE); break;
		case PROPS_DUNGEON_FLYER_BRIDGE_TOP: tmp.position = vector2f(x * TILE_SIZE, y * TILE_SIZE); break;
		case PROPS_DUNGEON_FLYER_BRIDGE_BOT: tmp.position = vector2f(x * TILE_SIZE, y * TILE_SIZE); break;
		case PROPS_DUNGEON_BOULDER: tmp.position = vector2f(x * TILE_SIZE, y * TILE_SIZE); break;
		case PROPS_DUNGEON_TRENCH_CORNER_TL: tmp.position = vector2f(x * TILE_SIZE, y * TILE_SIZE); break;
		case PROPS_DUNGEON_TRENCH_TOP_EDGE: tmp.position = vector2f(x * TILE_SIZE, y * TILE_SIZE); break;
		case PROPS_DUNGEON_TRENCH_CORNER_TR: tmp.position = vector2f(x * TILE_SIZE, y * TILE_SIZE); break;
		case PROPS_DUNGEON_TRENCH_LEFT_EDGE: tmp.position = vector2f(x * TILE_SIZE, y * TILE_SIZE); break;
		case PROPS_DUNGEON_TRENCH_RIGHT_EDGE: tmp.position = vector2f(x * TILE_SIZE, y * TILE_SIZE); break;
		case PROPS_DUNGEON_TRENCH_CORNER_BL: tmp.position = vector2f(x * TILE_SIZE, y * TILE_SIZE); break;
		case PROPS_DUNGEON_TRENCH_BOTTOM_EDGE: tmp.position = vector2f(x * TILE_SIZE, y * TILE_SIZE); break;
		case PROPS_DUNGEON_TRENCH_CORNER_BR: tmp.position = vector2f(x * TILE_SIZE, y * TILE_SIZE); break;
		case PROPS_DUNGEON_TRENCH_VB_END: tmp.position = vector2f(x * TILE_SIZE, y * TILE_SIZE); break;
		case PROPS_DUNGEON_TRENCH_VT_END: tmp.position = vector2f(x * TILE_SIZE, y * TILE_SIZE); break;
		case PROPS_DUNGEON_TRENCH_HL_END: tmp.position = vector2f(x * TILE_SIZE, y * TILE_SIZE); break;
		case PROPS_DUNGEON_TRENCH_HR_END: tmp.position = vector2f(x * TILE_SIZE, y * TILE_SIZE); break;
		case PROPS_DUNGEON_PILLAR_TOP_1: tmp.isBehind = sfFalse; tmp.position = vector2f(x * TILE_SIZE, y * TILE_SIZE); break;
		case PROPS_DUNGEON_PILLAR_TOP_2:  tmp.isBehind = sfFalse; tmp.position = vector2f(x * TILE_SIZE, y * TILE_SIZE);  break;
		case PROPS_DUNGEON_PILLAR_LEFT_1: tmp.position = vector2f(x * TILE_SIZE, y * TILE_SIZE); break;
		case PROPS_DUNGEON_PILLAR_MID_1: tmp.position = vector2f(x * TILE_SIZE, y * TILE_SIZE); break;
		case PROPS_DUNGEON_PILLAR_RIGHT_1: tmp.position = vector2f(x * TILE_SIZE, y * TILE_SIZE); break;
		case PROPS_DUNGEON_PILLAR_MID_2: tmp.isBehind = sfFalse; tmp.position = vector2f(x * TILE_SIZE, y * TILE_SIZE); break;
		case PROPS_DUNGEON_PILLAR_MID_3: tmp.isBehind = sfFalse; tmp.position = vector2f(x * TILE_SIZE, y * TILE_SIZE); break;
		case PROPS_DUNGEON_PILLAR_MID_4: tmp.isBehind = sfFalse; tmp.position = vector2f(x * TILE_SIZE, y * TILE_SIZE); break;
		case PROPS_DUNGEON_PILLAR_MID_5: tmp.isBehind = sfFalse; tmp.position = vector2f(x * TILE_SIZE, y * TILE_SIZE); break;
		case PROPS_DUNGEON_PILLAR_MID_6: tmp.isBehind = sfFalse; tmp.isBehind = sfFalse; tmp.position = vector2f(x * TILE_SIZE, y * TILE_SIZE); break;
		case PROPS_DUNGEON_PILLAR_BOTTOM_1: tmp.position = vector2f(x * TILE_SIZE, y * TILE_SIZE); break;
		case PROPS_DUNGEON_PILLAR_BOTTOM_2: tmp.position = vector2f(x * TILE_SIZE, y * TILE_SIZE); break;
		case PROPS_DUNGEON_PILLAR_LEFT_2: tmp.position = vector2f(x * TILE_SIZE, y * TILE_SIZE); break;
		case PROPS_DUNGEON_PILLAR_MID_7: tmp.position = vector2f(x * TILE_SIZE, y * TILE_SIZE); break;
		case PROPS_DUNGEON_PILLAR_RIGHT_2: tmp.position = vector2f(x * TILE_SIZE, y * TILE_SIZE); break;
		case PROPS_DUNGEON_DOOR_TOP: tmp.position = vector2f(x * TILE_SIZE, y * TILE_SIZE); break;
		case PROPS_DUNGEON_DOOR: tmp.position = vector2f(x * TILE_SIZE, y * TILE_SIZE); break;
		case PROPS_DUNGEON_CLOSED_DOOR_TOP: tmp.position = vector2f(x * TILE_SIZE, y * TILE_SIZE); break;
		case PROPS_DUNGEON_CLOSED_DOOR: tmp.position = vector2f(x * TILE_SIZE, y * TILE_SIZE); break;
		case PROPS_DUNGEON_BOSS_DOOR_TOP: tmp.position = vector2f(x * TILE_SIZE, y * TILE_SIZE); break;
		case PROPS_DUNGEON_BOSS_DOOR: tmp.position = vector2f(x * TILE_SIZE, y * TILE_SIZE); break;
		case PROPS_DUNGEON_PRESSURE_PLATE: tmp.position = vector2f(x * TILE_SIZE, y * TILE_SIZE); break;
		case PROPS_DUNGEON_BRAZIER: tmp.position = vector2f(x * TILE_SIZE, y * TILE_SIZE); break;
		case PROPS_DUNGEON_PAVING_STONE: tmp.position = vector2f(x * TILE_SIZE, y * TILE_SIZE); break;
		default: tmp.position = _pos; break;
		}
	}

	switch (tmp.type)
	{
	case PROPS_TYPE_FOREST:
		tmp.forest = _props;
		switch (tmp.forest)
		{
			case PROPS_FOREST_TREE_1:
				tmp.isBehind = sfFalse;
				map_addProps(tmp.type, PROPS_FOREST_TRUNK_1, addVectorsrf(_pos, vector2f(93.f, 252.f)), sfFalse); break;
			case PROPS_FOREST_TREE_2:
				tmp.isBehind = sfFalse;
				map_addProps(tmp.type, PROPS_FOREST_TRUNK_1, addVectorsrf(_pos, vector2f(93.f, 252.f)), sfFalse); break;
			case PROPS_FOREST_TREE_3:
				tmp.isBehind = sfFalse;
				map_addProps(tmp.type, PROPS_FOREST_TRUNK_2, addVectorsrf(_pos, vector2f(52.f, 232.f)), sfFalse); break;
			case PROPS_FOREST_TREE_4:
				tmp.isBehind = sfFalse;
				map_addProps(tmp.type, PROPS_FOREST_TRUNK_2, addVectorsrf(_pos, vector2f(52.f, 232.f)), sfFalse); break;
			case PROPS_FOREST_TREE_5:
				tmp.isBehind = sfFalse;
				map_addProps(tmp.type, PROPS_FOREST_TRUNK_3, addVectorsrf(_pos, vector2f(158.f, 354.f)), sfFalse); break;
			case PROPS_FOREST_TREE_6:
				tmp.isBehind = sfFalse;
				map_addProps(tmp.type, PROPS_FOREST_TRUNK_3, addVectorsrf(_pos, vector2f(158.f, 354.f)), sfFalse); break;
			case PROPS_FOREST_TREE_SPIDER:
				if (!_load) break;
				stree_addSpidersTrees(tmp.position); return;
			case PROPS_FOREST_TREE_ANIM:
				tmp.isBehind = sfFalse;
				map_addProps(tmp.type, PROPS_FOREST_TRUNK_2, addVectorsrf(_pos, vector2f(52.f, 232.f)), sfFalse); break;
			case PROPS_FOREST_GRASS_1:
				if (!_load) break;
				addGrass(tmp.position, 0); return;
			case PROPS_FOREST_GRASS_2:
				if (!_load) break;
				addGrass(tmp.position, 1); return;
		}
		break;
	case PROPS_TYPE_BEACH:
		tmp.beach = _props;
		switch (tmp.beach)
		{
		case PROPS_BEACH_PALM_1:
			tmp.isBehind = sfFalse;
			map_addProps(tmp.type, PROPS_BEACH_TRUNK_1, addVectorsrf(_pos, vector2f(118.f, 234.f)), sfFalse); break;
		case PROPS_BEACH_PALM_2:
			tmp.isBehind = sfFalse;
			map_addProps(tmp.type, PROPS_BEACH_TRUNK_2, addVectorsrf(_pos, vector2f(156.f, 220.f)), sfFalse); break;
		}
		break;
	case PROPS_TYPE_DUNGEON:
		tmp.dungeon = _props;
		switch (tmp.dungeon)
		{
		case PROPS_DUNGEON_BRAZIER:
			if (!_load) break;
			brz_CreateBrazier(tmp.position); return;
		case PROPS_DUNGEON_HINT_STATUE: tmp.isBehind = sfFalse; break;
		case PROPS_DUNGEON_PAVING_STONE:
			if (!_load) break;
			box_Create(tmp.position); return;
		case PROPS_DUNGEON_LITTLE_CHEST:
			if (!_load) break;
			chest_AddChest(CHEST_LITTLE, tmp.position); return;
		case PROPS_DUNGEON_BIG_CHEST:
			if (!_load) break;
			chest_AddChest(CHEST_BIG, tmp.position); return;
		case PROPS_DUNGEON_POT_1:
			//if (!_load) break;
			pot_Create(tmp.position, sfFalse, POT_REG); return;
		case PROPS_DUNGEON_POT_2:
			if (!_load) break;
			pot_Create(tmp.position, sfFalse, POT_HARD); return;
		case PROPS_DUNGEON_MOVING_PLATEFORM:
			if (!_load) break;
			mvpl_CreatePlatform(tmp.position); return;
		case PROPS_DUNGEON_FLYER_BRIDGE_LEFT:
			if (!_load) break;
			expt_Create(tmp.position, DIR_LEFT); return;
		case PROPS_DUNGEON_FLYER_BRIDGE_RIGHT:
			if (!_load) break;
			expt_Create(tmp.position, DIR_RIGHT); return;
		case PROPS_DUNGEON_FLYER_BRIDGE_TOP:
			if (!_load) break;
			expt_Create(tmp.position, DIR_UP); return;
		case PROPS_DUNGEON_FLYER_BRIDGE_BOT:
			if (!_load) break;
			expt_Create(tmp.position, DIR_DOWN); return;
		case PROPS_DUNGEON_BUILD_JOY:
			propsList->push_back(&propsList, &tmp);
			map_addProps(tmp.type, PROPS_DUNGEON_ENTRANCE_JOY, addVectorsrf(_pos, vector2f(774.f, 1338.f)), sfFalse);
			map_addProps(tmp.type, PROPS_DUNGEON_DOOR_JOY, addVectorsrf(_pos, vector2f(774.f, 1338.f)), sfFalse);
			map_addProps(tmp.type, PROPS_DUNGEON_OUTLINE_DOOR_JOY, addVectorsrf(_pos, vector2f(668.f, 1262.f)), sfFalse);
			break;
		case PROPS_DUNGEON_BUILD_FRONT_SADNESS:
			propsList->push_back(&propsList, &tmp);
			map_addProps(tmp.type, PROPS_DUNGEON_ENTRANCE_SADNESS, addVectorsrf(_pos, vector2f(774.f, 1338.f)), sfFalse);
			map_addProps(tmp.type, PROPS_DUNGEON_DOOR_SADNESS, addVectorsrf(_pos, vector2f(774.f, 1338.f)), sfFalse);
			map_addProps(tmp.type, PROPS_DUNGEON_OUTLINE_DOOR_SADNESS, addVectorsrf(_pos, vector2f(668.f, 1262.f)), sfFalse);
			break;
		case PROPS_DUNGEON_BUILD_FRONT_FEAR:
			propsList->push_back(&propsList, &tmp);
			map_addProps(tmp.type, PROPS_DUNGEON_ENTRANCE_FEAR, addVectorsrf(_pos, vector2f(774.f, 1338.f)), sfFalse);
			map_addProps(tmp.type, PROPS_DUNGEON_DOOR_FEAR, addVectorsrf(_pos, vector2f(774.f, 1338.f)), sfFalse);
			map_addProps(tmp.type, PROPS_DUNGEON_OUTLINE_DOOR_FEAR, addVectorsrf(_pos, vector2f(668.f, 1262.f)), sfFalse);
			break;
		case PROPS_DUNGEON_BUILD_FRONT_ANGRY:
			propsList->push_back(&propsList, &tmp);
			map_addProps(tmp.type, PROPS_DUNGEON_ENTRANCE_ANGRY, addVectorsrf(_pos, vector2f(774.f, 1338.f)), sfFalse);
			map_addProps(tmp.type, PROPS_DUNGEON_DOOR_ANGRY, addVectorsrf(_pos, vector2f(774.f, 1338.f)), sfFalse);
			map_addProps(tmp.type, PROPS_DUNGEON_OUTLINE_DOOR_ANGRY, addVectorsrf(_pos, vector2f(668.f, 1262.f)), sfFalse);
			break;
		case PROPS_DUNGEON_STATUE_HALF_HEAD: tmp.isBehind = sfFalse; break;
		case PROPS_DUNGEON_STATUE_HALF_TORSO: tmp.isBehind = sfFalse; break;
		case PROPS_DUNGEON_DOOR:
			if (!_load) break;
			opDoors_addDoors(DOOR, tmp.position); return;
		case PROPS_DUNGEON_DOOR_TOP:
			if (!_load) break;
			opDoors_addDoors(DOOR_TOP, tmp.position); return;
		case PROPS_DUNGEON_BOSS_DOOR:
			if (!_load) break;
			opDoors_addDoors(DOOR_BOSS, tmp.position); return;
		case PROPS_DUNGEON_BOSS_DOOR_TOP:
			if (!_load) break;
			opDoors_addDoors(DOOR_BOSS_TOP, tmp.position); return;
		case PROPS_DUNGEON_ENTRANCE_PRESSURE_PLATE_JOY:
			map_PosPressurePlateJoy = tmp.position;
			break;
		}
		break;
	case PROPS_TYPE_GENERAL:
		tmp.general = _props;
		switch (tmp.general)
		{
		case PROPS_GENERAL_FOUNTAIN:
			if (!_load) break;
			stt_Create(STATUE_TYPE_1, tmp.position); return;
		case PROPS_GENERAL_POOL: map_addProps(tmp.type, PROPS_GENERAL_POOL_LILYPAD, addVectorsrf(_pos, vector2f(100.f, 143.f)), sfFalse); break;
		case PROPS_GENERAL_POOL_LILYPAD: tmp.isBehind = sfFalse; break;
		case PROPS_GENERAL_FISHING_SPOT:
			if (!_load) break;
			fishspot_AddSplash(tmp.position); return;

		case PROPS_GENERAL_GROUND_NOTES:
		case PROPS_GENERAL_FLOATING_NOTES:
			note_AddNote(tmp.position);
			return;
		}

		break;
	case PROPS_TYPE_MOUTAIN:
		tmp.moutain = _props;
		switch (tmp.moutain)
		{
		case PROPS_MOUTAIN_DOOR_FRAME:
			if (!_load)
			{
				if (getLightOrShadow())
					map_addProps(tmp.type, PROPS_MOUTAIN_DOOR_SHADOW, addVectorsrf(_pos, vector2f(28.f, 42.f)), sfFalse);
				else
					map_addProps(tmp.type, PROPS_MOUTAIN_DOOR_LIGHT, addVectorsrf(_pos, vector2f(28.f, 42.f)), sfFalse);
			}
			break;
		case PROPS_MOUTAIN_DOOR_FRAME_TOP:
			if (!_load)
			{
				if (getLightOrShadow())
					map_addProps(tmp.type, PROPS_MOUTAIN_DOOR_SHADOW_TOP, addVectorsrf(_pos, vector2f(16.f, -21.f)), sfFalse);
				else
					map_addProps(tmp.type, PROPS_MOUTAIN_DOOR_LIGHT_TOP, addVectorsrf(_pos, vector2f(16.f, -21.f)), sfFalse);
			}
			break;
		}
		break;
	default: return;
	}
	for (int i = 0; i < propsList->size(propsList); i++)
	{
		LayerProps* tmpProps = STD_LIST_GETDATA(propsList, LayerProps, i);
		if (tmp.position.x == tmpProps->position.x && tmp.position.y == tmpProps->position.y)
		{
			switch (tmp.type)
			{
			case PROPS_TYPE_BEACH:
				if (tmp.beach == tmpProps->beach)
					return;
				break;
			case PROPS_TYPE_DUNGEON:
				if (tmp.dungeon == tmpProps->dungeon)
					return;
				break;
			case PROPS_TYPE_FOREST:
				if (tmp.forest == tmpProps->forest)
					return;
				break;
			case PROPS_TYPE_GENERAL:
				if (tmp.general == tmpProps->general)
					return;
				break;
			case PROPS_TYPE_MOUTAIN:
				if (tmp.moutain == tmpProps->moutain)
					return;
				break;
			}
		}
	}

	if (tmp.dungeon != PROPS_DUNGEON_BUILD_JOY && tmp.dungeon != PROPS_DUNGEON_BUILD_FRONT_SADNESS && tmp.dungeon != PROPS_DUNGEON_BUILD_FRONT_FEAR && tmp.dungeon != PROPS_DUNGEON_BUILD_FRONT_ANGRY)
		propsList->push_back(&propsList, &tmp);
}

void map_delProps(sfVector2i _pos)
{
	for (int i = 0; i < propsList->size(propsList); i++)
	{
		LayerProps* tmp = STD_LIST_GETDATA(propsList, LayerProps, i);
		sfIntRect rect = map_GetPropsTexRect(*tmp);
		rect.left = tmp->position.x;
		rect.top = tmp->position.y;
		if (sfIntRect_contains(&rect, _pos.x, _pos.y))
		{
			propsList->erase(&propsList, i);
			return;
		}
	}
}

void map_clearPropsList()
{
	if (!propsList) return;
	propsList->clear(&propsList);
}

void defaultMap()
{
	map.mapSize = vector2i(26, 16);
	map_CallocMap(map.mapSize);

	for (int i = 0; i < map.mapSize.y; i++)
	{
		for (int j = 0; j < map.mapSize.x; j++)
		{
			/// LAYER 1 (ground)
			map.layerGround_first[i][j].type = TILE_GRASS_FULL;
			map.layerGround_second[i][j].type = TILE_NO_TILE;

			map.layerWalls_first[i][j].type = WALL_NO_TILE;
			map.layerWalls_second[i][j].type = WALL_NO_TILE;

			map.layerCollision[i][j].type = CLD_NO_COLL;
		}
	}
}

void map_SaveMap()
{
	/// Save biome
	FILE* file;
	char path[40] = "";

	sprintf(path, MAP_SAVE_PATH"biome_%d.bin", currentMap);

	file = fopen(path, "wb");
	fwrite(&currentBiome, sizeof(Biomes), 1, file);
	fclose(file);

	map_SaveEnviro();
	map_SaveProps();
	map_SaveEntity();
}

void map_LoadMap(int _idMap)
{
	currentMap = _idMap;
	fillTransitionZone();

	FILE* file;
	char path[40] = "";

	sprintf(path, MAP_SAVE_PATH"biome_%d.bin", currentMap);

	file = fopen(path, "rb");
	Biomes tmpBiome = BIOME_GRASSLAND;

	if (file == NULL)
	{
		file = fopen(path, "wb");
		fwrite(&tmpBiome, sizeof(Biomes), 1, file);
	}
	else
		fread(&tmpBiome, sizeof(Biomes), 1, file);
	fclose(file);

	setCurrentBiome(tmpBiome);

	map_LoadEnviro();
	map_LoadProps();
	map_LoadEntity();
	mvpl_ConstituteGroups();
	getGrassTexture();
	plr_loadTexture();

	if (currentMap == MAP_STATE_OVERWORLD)
	{
		note_AddNote(vector2f(4700.f, 1350.f));
		note_AddNote(vector2f(6050.f, 8110.f));
		note_AddNote(vector2f(9250.f, 9450.f));
	}
}

void map_SaveEnviro()
{
	FILE* file;
	char path[40] = "";

	sprintf(path, MAP_SAVE_PATH"enviro_%d.bin", currentMap);

	file = fopen(path, "wb");
	if (file)
	{
		/// Save map
		fwrite(&map.mapSize, sizeof(sfVector2i), 1, file);
		map_FillOrGetLayer(file, sfFalse);
		fclose(file);
	}
}

void map_SaveProps()
{
	FILE* file;
	char path[40] = "";

	sprintf(path, MAP_SAVE_PATH"props_%d.bin", currentMap);

	file = fopen(path, "wb");
	if (file) {
		/// Save props
		int propsLen = propsList->size(propsList);
		fwrite(&propsLen, sizeof(int), 1, file);
		if (propsLen > 0)
		{
			LayerProps* tmp = map_getPropsListToSave();
			fwrite(tmp, sizeof(LayerProps), propsLen, file);
		}
		fclose(file);
	}
}

void map_SaveEntity()
{
	FILE* file;
	char path[40] = "";

	sprintf(path, MAP_SAVE_PATH"entity_%d.bin", currentMap);

	file = fopen(path, "wb");
	if (file != NULL)
	{
		sfVector2f playerPos = plr_GetPos();
		/// Save player pos
		fwrite(&playerPos, sizeof(sfVector2f), 1, file);
		///________SAVE ENTITY___________
		/// Save NPC
		int npcLen = npc_List->size(npc_List);
		fwrite(&npcLen, sizeof(int), 1, file);
		if (npcLen > 0)
		{
			NpcToSave* tmp = getNpcListToSave();
			fwrite(tmp, sizeof(NpcToSave), npcLen, file);
		}
		/// Save mobs
		int mobsLen = enemiesList->size(enemiesList);
		fwrite(&mobsLen, sizeof(int), 1, file);
		if (mobsLen > 0)
		{
			EnemiesStructToSave* tmp = getMobsListToSave();
			fwrite(tmp, sizeof(EnemiesStructToSave), mobsLen, file);
		}
		///________SAVE EFFECTS___________
		/// Save lights
		int lightLen = lightList->size(lightList);
		fwrite(&lightLen, sizeof(int), 1, file);
		if (lightLen > 0)
		{
			Light* tmp = getLightListToSave();
			fwrite(tmp, sizeof(Light), lightLen, file);
		}
		int potLen = pot_List->size(pot_List);
		fwrite(&potLen, sizeof(int), 1, file);
		if (potLen > 0)
		{
			PotToSave* tmp = getPotListToSave();
			fwrite(tmp, sizeof(PotToSave), potLen, file);
		}
		fclose(file);
	}
}

void map_LoadEnviro()
{
	FILE* file;
	char path[40] = "";

	sprintf(path, MAP_SAVE_PATH"enviro_%d.bin", currentMap);

	file = fopen(path, "rb");
	if (file == NULL)
	{
		defaultMap();
		file = fopen(path, "wb");
		fwrite(&map.mapSize, sizeof(sfVector2i), 1, file);
		map_FillOrGetLayer(file, sfFalse);
	}
	else
	{
		fread(&map.mapSize, sizeof(sfVector2i), 1, file);
		map_CallocMap(map.mapSize);
		map_FillOrGetLayer(file, sfTrue);
	}
	fclose(file);
}

void map_LoadProps()
{
	sfBool isGame = sfTrue;
	if (getState() == EDITOR)
		isGame = sfFalse;

	FILE* file;
	char path[40] = "";

	sprintf(path, MAP_SAVE_PATH"props_%d.bin", currentMap);

	file = fopen(path, "rb");
	if (file == NULL)
	{
		file = fopen(path, "wb");
		int len = 0;
		fwrite(&len, sizeof(int), 1, file);
		fwrite(&len, sizeof(int), 1, file);
	}
	else
	{
		int len = 0;
		fread(&len, sizeof(int), 1, file);
		if (len > 0)
		{
			LayerProps tmp;
			for (int i = 0; i < len; i++)
			{
				fread(&tmp, sizeof(LayerProps), 1, file);
				switch (tmp.type)
				{
				case PROPS_TYPE_FOREST:  map_addProps(tmp.type, tmp.forest, tmp.position, isGame);  break;
				case PROPS_TYPE_BEACH:   map_addProps(tmp.type, tmp.beach, tmp.position, isGame);   break;
				case PROPS_TYPE_DUNGEON: map_addProps(tmp.type, tmp.dungeon, tmp.position, isGame); break;
				case PROPS_TYPE_GENERAL: map_addProps(tmp.type, tmp.general, tmp.position, isGame); break;
				case PROPS_TYPE_MOUTAIN: map_addProps(tmp.type, tmp.moutain, tmp.position, isGame); break;
				}
			}
		}
	}
	fclose(file);
}

void map_LoadEntity()
{
	FILE* file;
	char path[40] = "";

	sprintf(path, MAP_SAVE_PATH"entity_%d.bin", currentMap);

	int len = 0;
	file = fopen(path, "rb");
	if (file == NULL)
	{
		file = fopen(path, "wb");
		fwrite(&playerSpawnPos, sizeof(sfVector2f), 1, file);
		fwrite(&len, sizeof(int), 1, file);
		fwrite(&len, sizeof(int), 1, file);
		fwrite(&len, sizeof(int), 1, file);
		fwrite(&len, sizeof(int), 1, file);
	}
	else
	{
		/// PLAYER POS
		fread(&playerSpawnPos, sizeof(sfVector2f), 1, file);
		plr_SetPos(playerSpawnPos);
		plr_resetPlayer();
		/// NPC 
		fread(&len, sizeof(int), 1, file);
		if (len > 0)
		{
			NpcToSave tmp;
			for (int i = 0; i < len; i++)
			{
				fread(&tmp, sizeof(NpcToSave), 1, file);
				npc_Create(tmp.position, tmp.id);
			}
		}
		/// ENEMIES
		fread(&len, sizeof(int), 1, file);
		if (len > 0)
		{
			EnemiesStructToSave tmp;
			for (int i = 0; i < len; i++)
			{
				fread(&tmp, sizeof(EnemiesStructToSave), 1, file);
				en_SpawnEnemy(tmp.position, tmp.type, tmp.rotation, sfFalse);
			}
		}
		/// LIGHT
		fread(&len, sizeof(int), 1, file);
		if (len > 0)
		{
			Light tmp;
			for (int i = 0; i < len; i++)
			{
				fread(&tmp, sizeof(Light), 1, file);
				addLight(tmp.pos, tmp.color, tmp.size, tmp.form, tmp.angle, tmp.rotation, tmp.strength);
			}
		}
		fread(&len, sizeof(int), 1, file);
		if (len > 0)
		{
			PotToSave tmp;
			for (int i = 0; i < len; i++)
			{
				fread(&tmp, sizeof(PotToSave), 1, file);
				pot_Create(tmp.pos, sfFalse, tmp.type);
			}
		}
	}
	fclose(file);
}

void map_CallocMap(sfVector2i _mapSize)
{
	if (!map.isAlloc) {
		map.mapSize = _mapSize;

		map.layerGround_first = (LayerGround**)calloc(map.mapSize.y, sizeof(LayerGround*));
		map.layerGround_second = (LayerGround**)calloc(map.mapSize.y, sizeof(LayerGround*));
		map.layerWalls_first = (LayerWalls**)calloc(map.mapSize.y, sizeof(LayerWalls*));
		map.layerWalls_second = (LayerWalls**)calloc(map.mapSize.y, sizeof(LayerWalls*));
		map.layerCollision = (LayerCollision**)calloc(map.mapSize.y, sizeof(LayerCollision*));

		for (int i = 0; i < map.mapSize.y; i++)
		{
			map.layerGround_first[i] = (LayerGround*)calloc(map.mapSize.x, sizeof(LayerGround));
			map.layerGround_second[i] = (LayerGround*)calloc(map.mapSize.x, sizeof(LayerGround));
			map.layerWalls_first[i] = (LayerWalls*)calloc(map.mapSize.x, sizeof(LayerWalls));
			map.layerWalls_second[i] = (LayerWalls*)calloc(map.mapSize.x, sizeof(LayerWalls));
			map.layerCollision[i] = (LayerCollision*)calloc(map.mapSize.x, sizeof(LayerCollision));
		}
	}

	map.isAlloc = sfTrue;
}

void map_FreeMap() {
	if (map.isAlloc) {
		for (int i = 0; i < map.mapSize.y; i++) {
			if (map.layerGround_first && map.layerGround_first[i])		free(map.layerGround_first[i]);
			if (map.layerGround_second && map.layerGround_second[i])	free(map.layerGround_second[i]);
			if (map.layerWalls_first && map.layerWalls_first[i])		free(map.layerWalls_first[i]);
			if (map.layerWalls_second && map.layerWalls_second[i])		free(map.layerWalls_second[i]);
		}

		if (map.layerGround_first)	free(map.layerGround_first);
		if (map.layerGround_second)	free(map.layerGround_second);
		if (map.layerWalls_first)	free(map.layerWalls_first);
		if (map.layerWalls_second)	free(map.layerWalls_second);
		if (map.layerCollision)		free(map.layerCollision);
	}

	map.isAlloc = sfFalse;
}

void map_FillOrGetLayer(FILE* _file, sfBool _fill)
{
	for (int i = 0; i < map.mapSize.y; i++)
	{
		for (int j = 0; j < map.mapSize.x; j++)
		{
			/// READ
			if (_fill) {
				fread(&map.layerGround_first[i][j], sizeof(LayerGround), 1, _file);
				fread(&map.layerGround_second[i][j], sizeof(LayerGround), 1, _file);
				fread(&map.layerWalls_first[i][j], sizeof(LayerWalls), 1, _file);
				fread(&map.layerWalls_second[i][j], sizeof(LayerWalls), 1, _file);
				fread(&map.layerCollision[i][j], sizeof(LayerCollision), 1, _file);
			}
			/// WRITE
			else {
				fwrite(&map.layerGround_first[i][j], sizeof(LayerGround), 1, _file);
				fwrite(&map.layerGround_second[i][j], sizeof(LayerGround), 1, _file);
				fwrite(&map.layerWalls_first[i][j], sizeof(LayerWalls), 1, _file);
				fwrite(&map.layerWalls_second[i][j], sizeof(LayerWalls), 1, _file);
				fwrite(&map.layerCollision[i][j], sizeof(LayerCollision), 1, _file);
			}
		}
	}
}

CollisionType map_HasTileCollisions(sfVector2i _index)
{
	if (isFreeingMap || !map.isAlloc)
		return CLD_NO_COLL;
	if ((_index.x < 0 || _index.x >= map.mapSize.x) || (_index.y < 0 || _index.y >= map.mapSize.y) || !map.layerCollision[_index.y][_index.x].type) return CLD_NO_COLL;

	return map.layerCollision[_index.y][_index.x].type;
}

CollisionType map_SnakeHasTileCollisions(sfVector2i _index)
{
	if (!map_isIndexInArray(_index))
		return CLD_NO_COLL;

	return map.layerCollision[_index.x][_index.y].type;
}

sfBool map_isIndexInArray(sfVector2i _index)
{
	if (_index.x < 0 || _index.y < 0 || _index.x >= map.mapSize.y || _index.y >= map.mapSize.x)
		return sfFalse;
	return sfTrue;
}

sfBool map_isIndexInArraySwapped(sfVector2i _index)
{
	if (_index.x < 0 || _index.y < 0 || _index.x >= map.mapSize.x || _index.y >= map.mapSize.y)
		return sfFalse;
	return sfTrue;
}

sfVector2i map_getBlockPos(sfVector2f _pos)
{
	sfVector2i v;
	v.y = (int)(_pos.x / TILE_SIZE);
	v.x = (int)(_pos.y / TILE_SIZE);
	return v;
}

sfVector2f map_getWorldPos(sfVector2i _blockPos)
{
	sfVector2f v;
	v.y = (float)(_blockPos.x * TILE_SIZE);
	v.x = (float)(_blockPos.y * TILE_SIZE);
	return v;
}

sfIntRect map_GetTilesetRect(int _x, int _y)
{
	return IntRect(_x * TILE_SIZE, _y * TILE_SIZE, TILE_SIZE, TILE_SIZE);
}

sfIntRect map_GetTileTexRect(LayerGround _tile)
{
	if (_tile.type == TILE_WATER_FULL_DEEP)
		return map_GetTilesetRect(5, 3);

	if (_tile.type < TILE_GROUND_DIRT_1)
	{
		TilesType type = _tile.type & 0b1111;

		switch (type)
		{
		default:				return IntRect(0, 0, 0, 0);
		case TILE_FULL:
			switch (_tile.variant) {
			default:				return map_GetTilesetRect(1, 1);
			case 1:					return map_GetTilesetRect(4, 1);
			case 2:					return map_GetTilesetRect(5, 3);
			case 3:					return map_GetTilesetRect(5, 4);
			}
			break;
		case TILE_EDGE_T:			return map_GetTilesetRect(1 + _tile.variant * 3, 0);
		case TILE_EDGE_B:			return map_GetTilesetRect(1 + _tile.variant * 3, 2);
		case TILE_EDGE_L:			return map_GetTilesetRect(0 + _tile.variant * 3, 1);
		case TILE_EDGE_R:			return map_GetTilesetRect(2 + _tile.variant * 3, 1);
		case TILE_CORNER_TL:		return map_GetTilesetRect(0 + _tile.variant * 3, 0);
		case TILE_CORNER_TR:		return map_GetTilesetRect(2 + _tile.variant * 3, 0);
		case TILE_CORNER_BL:		return map_GetTilesetRect(0 + _tile.variant * 3, 2);
		case TILE_CORNER_BR:		return map_GetTilesetRect(2 + _tile.variant * 3, 2);
		case TILE_DIAG_TL:			return map_GetTilesetRect(0 + _tile.variant * 3, 3);
		case TILE_DIAG_TR:			return map_GetTilesetRect(1 + _tile.variant * 3, 3);
		case TILE_DIAG_BL:			return map_GetTilesetRect(0 + _tile.variant * 3, 4);
		case TILE_DIAG_BR:			return map_GetTilesetRect(1 + _tile.variant * 3, 4);
		case TILE_SOLO:				return map_GetTilesetRect(2, 3);
		case TILE_FRAME:			return map_GetTilesetRect(2, 4);
		}
	}
	else
	{
		switch (_tile.type)
		{
		default:					return IntRect(0, 0, 0, 0);
		case TILE_GROUND_DIRT_1:	return map_GetTilesetRect(0, 0);
		case TILE_GROUND_DIRT_2:	return map_GetTilesetRect(0, 1);
		case TILE_GROUND_DIRT_3:	return map_GetTilesetRect(1, 0);
		case TILE_GROUND_DIRT_4:	return map_GetTilesetRect(1, 1);
		case TILE_GROUND_DUNGEON_1: return map_GetTilesetRect(0, 0);
		case TILE_GROUND_DUNGEON_2: return map_GetTilesetRect(0, 1);
		case TILE_GROUND_DUNGEON_3: return map_GetTilesetRect(1, 0);
		case TILE_GROUND_DUNGEON_4: return map_GetTilesetRect(1, 1);
		}
	}
}

sfIntRect map_GetWallTexRect(LayerWalls _wall) {
	WallsType sel = _wall.type & 0b01111111;
	switch (sel) {
	default:									return IntRect(0, 0, 0, 0);
	case WALL_TOP_DIAG_TL:						return map_GetTilesetRect(0, 0);
	case WALL_TOP_EDGE_T:						return map_GetTilesetRect(1, 0);
	case WALL_TOP_DIAG_TR:						return map_GetTilesetRect(2, 0);
	case WALL_TOP_EDGE_L:						return map_GetTilesetRect(0, 1);
	case WALL_TOP_EDGE_T_FILLED:				return map_GetTilesetRect(1, 1);
	case WALL_TOP_EDGE_R:						return map_GetTilesetRect(2, 1);
	case WALL_TOP_DIAG_BL:						return map_GetTilesetRect(0, 2);
	case WALL_TOP_DIAG_BL_WATER:				return map_GetTilesetRect(2, 7);
	case WALL_TOP_DIAG_BL_VOID:					return map_GetTilesetRect(2, 5);
	case WALL_TOP_EDGE_B:						return map_GetTilesetRect(1, 2);
	case WALL_TOP_DIAG_BR:						return map_GetTilesetRect(2, 2);
	case WALL_TOP_DIAG_BR_WATER:				return map_GetTilesetRect(4, 7);
	case WALL_TOP_DIAG_BR_VOID:					return map_GetTilesetRect(4, 5);
	case WALL_TOP_EDGE_L_FILLED:				return map_GetTilesetRect(3, 0);
	case WALL_TOP_EDGE_R_FILLED:				return map_GetTilesetRect(4, 0);
	case WALL_TOP_DIAG_BL_FILLED:				return map_GetTilesetRect(3, 1);
	case WALL_TOP_DIAG_BR_FILLED:				return map_GetTilesetRect(4, 1);
	case WALL_TOP_CORNER_TL:					return map_GetTilesetRect(5, 0);
	case WALL_TOP_CORNER_TR:					return map_GetTilesetRect(6, 0);
	case WALL_TOP_CORNER_BL:					return map_GetTilesetRect(5, 1);
	case WALL_TOP_CORNER_BR:					return map_GetTilesetRect(6, 1);
	case WALL_TOP_CORNER_TL_FILLED:				return map_GetTilesetRect(7, 0);
	case WALL_TOP_CORNER_TR_FILLED:				return map_GetTilesetRect(8, 0);
	case WALL_TOP_CORNER_BL_FILLED:				return map_GetTilesetRect(7, 1);
	case WALL_TOP_CORNER_BR_FILLED:				return map_GetTilesetRect(8, 1);
	case WALL_TOP_DIAG_TL_DARK:					return map_GetTilesetRect(7, 5);
	case WALL_TOP_DIAG_TR_DARK:					return map_GetTilesetRect(8, 5);
	case WALL_TOP_DIAG_BL_DARK:					return map_GetTilesetRect(7, 7);
	case WALL_TOP_DIAG_BR_DARK:					return map_GetTilesetRect(8, 7);
	case WALL_TOP_EDGE_T_DARK:					return map_GetTilesetRect(7, 8);
	case WALL_TOP_EDGE_L_DARK:					return map_GetTilesetRect(7, 6);
	case WALL_TOP_EDGE_R_DARK:					return map_GetTilesetRect(8, 6);
	case WALL_TOP_CORNER_TL_DARK:				return map_GetTilesetRect(5, 7);
	case WALL_TOP_CORNER_TR_DARK:				return map_GetTilesetRect(6, 7);
	case WALL_TOP_CORNER_BL_DARK:				return map_GetTilesetRect(5, 8);
	case WALL_TOP_CORNER_BR_DARK:				return map_GetTilesetRect(6, 8);
	case WALL_TOP_INNER_TL:						return map_GetTilesetRect(3, 3);
	case WALL_TOP_INNER_TR:						return map_GetTilesetRect(4, 3);
	case WALL_TOP_INNER_BL:						return map_GetTilesetRect(3, 4);
	case WALL_TOP_INNER_BR:						return map_GetTilesetRect(4, 4);
	case WALL_MID_DIAG_BL:						return map_GetTilesetRect(0, 3);
	case WALL_MID_FULL:
		switch (_wall.variant) {
		default:				return map_GetTilesetRect(1, 3);
		case 1:					return map_GetTilesetRect(5, 2);
		case 2:					return map_GetTilesetRect(6, 2);
		}
		break;
	case WALL_MID_DIAG_BR:						return map_GetTilesetRect(2, 3);
	case WALL_MID_DIAG_BL_TRANSP:				return map_GetTilesetRect(3, 2);
	case WALL_MID_DIAG_BL_WATER:				return map_GetTilesetRect(0, 7);
	case WALL_MID_DIAG_BR_TRANSP:				return map_GetTilesetRect(4, 2);
	case WALL_MID_DIAG_BR_WATER:				return map_GetTilesetRect(1, 7);
	case WALL_MID_FULL_LEFT:					return map_GetTilesetRect(7, 2);
	case WALL_MID_FULL_RIGHT:					return map_GetTilesetRect(8, 2);
	case WALL_BASE_DIAG_BL_FULL:				return map_GetTilesetRect(0, 4);
	case WALL_BASE_DIAG_BL_HALF_WATER:			return map_GetTilesetRect(0, 8);
	case WALL_BASE_DIAG_BL_FULL_WATER:			return map_GetTilesetRect(2, 8);
	case WALL_BASE_FULL_LEFT:					return map_GetTilesetRect(7, 3);
	case WALL_BASE_FULL:
		switch (_wall.variant) {
		default:				return map_GetTilesetRect(1, 4);
		case 1:					return map_GetTilesetRect(5, 3);
		case 2:					return map_GetTilesetRect(6, 3);
		}
		break;
	case WALL_BASE_FULL_RIGHT:					return map_GetTilesetRect(8, 3);
	case WALL_BASE_HALF_WATER:					return map_GetTilesetRect(3, 8);
	case WALL_BASE_DIAG_BR_FULL:				return map_GetTilesetRect(2, 4);
	case WALL_BASE_DIAG_BR_HALF_WATER:			return map_GetTilesetRect(1, 8);
	case WALL_BASE_DIAG_BR_FULL_WATER:			return map_GetTilesetRect(4, 8);
	case WALL_VOID_DIAG_BL:						return map_GetTilesetRect(0, 6);
	case WALL_VOID:								return map_GetTilesetRect(3, 6);
	case WALL_VOID_DIAG_BR:						return map_GetTilesetRect(1, 6);
	case WALL_VOID_GRAD_DIAG_BL:				return map_GetTilesetRect(0, 5);
	case WALL_VOID_GRAD_DIAG_BR:				return map_GetTilesetRect(1, 5);
	case WALL_VOID_GRAD_DIAG_BL_EDGE:			return map_GetTilesetRect(2, 5);
	case WALL_VOID_GRAD_DIAG_BR_EDGE:			return map_GetTilesetRect(4, 5);
	case WALL_SHADOW_FULL:						return map_GetTilesetRect(5, 6);
	case WALL_SHADOW_HALF:						return map_GetTilesetRect(6, 5);
	case WALL_SHADOW_T:							return map_GetTilesetRect(6, 6);
	case WALL_SHADOW_B:							return map_GetTilesetRect(5, 5);

	case WALL_BASE_VOID_CORNER_BL:				return map_GetTilesetRect(0, 8);
	case WALL_BASE_VOID_CORNER_BR:				return map_GetTilesetRect(1, 8);
	case WALL_FULL_STAIR:						return map_GetTilesetRect(8, 8);
	case WALL_WATER_STAIR:						return map_GetTilesetRect(3, 5);
	case WALL_TOP_STAIR:						return map_GetTilesetRect(3, 7);
	}
}

sfIntRect map_GetPropsTexRect(LayerProps _props)
{
	switch (_props.type)
	{
	case PROPS_TYPE_NO_PROPS: return IntRect(0, 0, 0, 0);
	case PROPS_TYPE_GENERAL:
		switch (_props.general)
		{
		case PROPS_GENERAL_FOUNTAIN:		return IntRect(0, 0, 312, 324);
		case PROPS_GENERAL_STONE_1:			return IntRect(0, 324, 24, 24);
		case PROPS_GENERAL_STONE_2:			return IntRect(24, 324, 16, 14);
		case PROPS_GENERAL_STONE_3:			return IntRect(40, 324, 24, 20);
		case PROPS_GENERAL_STONE_4:			return IntRect(64, 324, 28, 34);
		case PROPS_GENERAL_STONE_5:			return IntRect(0, 358, 42, 52);
		case PROPS_GENERAL_STONE_6:			return IntRect(42, 359, 54, 50);
		case PROPS_GENERAL_STONE_7:			return IntRect(96, 358, 48, 62);
		case PROPS_GENERAL_STONE_8:			return IntRect(144, 324, 112, 94);
		case PROPS_GENERAL_STONE_9:			return IntRect(256, 324, 80, 58);
		case PROPS_GENERAL_STONE_10:		return IntRect(336, 324, 62, 46);
		case PROPS_GENERAL_STONE_11:		return IntRect(398, 324, 120, 92);
		case PROPS_GENERAL_SIGN:			return IntRect(0, 420, 316, 254);
		case PROPS_GENERAL_FLOWER:			return IntRect(0, 674, 14, 14);
		case PROPS_GENERAL_TENT_ARNOLD:		return IntRect(0, 690, 1224, 642);
		case PROPS_GENERAL_POOL:			return IntRect(0, 1332, 636, 598);
		case PROPS_GENERAL_POOL_LILYPAD:	return IntRect(636, 1332, 480, 306);
		case PROPS_GENERAL_FISH:			return IntRect(0, 1930, 64, 42);
		case PROPS_GENERAL_BRIDGE:			return IntRect(636, 1638, 256, 192);
		case PROPS_GENERAL_TENT_SHAMAN:		return IntRect(0, 1972, 792, 632);
		case PROPS_GENERAL_FISHING_SPOT:	return IntRect(0, 2604, 252, 192);
		case PROPS_GENERAL_BUBBLES:			return IntRect(46 * 5, 2769, 46, 44);
		case PROPS_GENERAL_GROUND_NOTES:    return IntRect(0, 2840, 60, 98);
		case PROPS_GENERAL_FLOATING_NOTES:  return IntRect(0, 2938, 84, 86);
		default:							return IntRect(0, 0, 0, 0);
		}
		break;
	case PROPS_TYPE_FOREST:
		switch (_props.forest)
		{
		case PROPS_FOREST_TREE_1:		 return IntRect(0, 0, 264, 344);
		case PROPS_FOREST_TREE_2:		 return IntRect(264, 0, 264, 344);
		case PROPS_FOREST_TREE_3:		 return IntRect(656, 0, 258, 364);
		case PROPS_FOREST_TREE_4:		 return IntRect(914, 0, 258, 364);
		case PROPS_FOREST_TREE_5:		 return IntRect(0, 728, 398, 558);
		case PROPS_FOREST_TREE_6:		 return IntRect(398, 728, 398, 558);
		case PROPS_FOREST_TREE_SPIDER:	 return IntRect(796, 728, 400, 556);
		case PROPS_FOREST_TREE_ANIM:	 return IntRect(0, 364, 258, 364);
		case PROPS_FOREST_TRUNK_1:		 return IntRect(528, 0, 128, 64);
		case PROPS_FOREST_TRUNK_2:		 return IntRect(1172, 0, 192, 64);
		case PROPS_FOREST_TRUNK_3:		 return IntRect(1196, 728, 148, 64);
		case PROPS_FOREST_BIG_TRUNK_1:	 return IntRect(0, 1286, 316, 380);
		case PROPS_FOREST_BIG_TRUNK_2:	 return IntRect(316, 1286, 312, 384);
		case PROPS_FOREST_BIG_TRUNK_3:	 return IntRect(628, 1286, 314, 380);
		case PROPS_FOREST_STRAIN_1:		 return IntRect(418, 1670, 152, 156);
		case PROPS_FOREST_STRAIN_2:		 return IntRect(570, 1670, 100, 116);
		case PROPS_FOREST_STRAIN_3:		 return IntRect(670, 1670, 220, 182);
		case PROPS_FOREST_STICK:		 return IntRect(356, 1670, 64, 64);
		case PROPS_FOREST_GRASS_1:		 return IntRect(0 * TILE_SIZE, 6 * TILE_SIZE, TILE_SIZE, TILE_SIZE);
		case PROPS_FOREST_GRASS_2:		 return IntRect(0 * TILE_SIZE, 0 * TILE_SIZE, TILE_SIZE, TILE_SIZE);
		case PROPS_FOREST_GRASS_3:		 return IntRect(256, 1670, 60, 54);
		case PROPS_FOREST_CUTED_GRASS_1: return IntRect(64, 1670, 64, 40);
		case PROPS_FOREST_CUTED_GRASS_2: return IntRect(192, 1670, 64, 34);
		case PROPS_FOREST_CUTED_GRASS_3: return IntRect(316, 1670, 42, 38);
		case PROPS_FOREST_SPLASH:        return IntRect(0, 0, 564, 184);
		case PROPS_FOREST_WATERFALL:     return IntRect(0, 0, 488, 488);
		default:						 return IntRect(0, 0, 0, 0);
		}
		break;
	case PROPS_TYPE_MOUTAIN:
		switch (_props.moutain)
		{
		case PROPS_MOUTAIN_BIG_CRISTAL_1:	 return IntRect(0, 0, 106, 132);
		case PROPS_MOUTAIN_BIG_CRISTAL_2:	 return IntRect(106, 0, 112, 116);
		case PROPS_MOUTAIN_BIG_CRISTAL_3:	 return IntRect(268, 0, 58, 126);
		case PROPS_MOUTAIN_BIG_CRISTAL_4:	 return IntRect(326, 0, 38, 94);
		case PROPS_MOUTAIN_BIG_CRISTAL_5:	 return IntRect(364, 0, 110, 106);
		case PROPS_MOUTAIN_BIG_CRISTAL_6:	 return IntRect(474, 0, 44, 116);
		case PROPS_MOUTAIN_BIG_CRISTAL_7:	 return IntRect(0, 132, 96, 126);
		case PROPS_MOUTAIN_LITTLE_CRISTAL_1: return IntRect(218, 0, 42, 48);
		case PROPS_MOUTAIN_LITTLE_CRISTAL_2: return IntRect(218, 48, 38, 26);
		case PROPS_MOUTAIN_LITTLE_CRISTAL_3: return IntRect(218, 74, 50, 52);
		case PROPS_MOUTAIN_LITTLE_CRISTAL_4: return IntRect(326, 106, 32, 26);
		case PROPS_MOUTAIN_ROCK_CRISTAL_1:	 return IntRect(106, 132, 118, 122);
		case PROPS_MOUTAIN_ROCK_CRISTAL_2:	 return IntRect(224, 132, 64, 60);
		case PROPS_MOUTAIN_ROCK_CRISTAL_3:	 return IntRect(288, 132, 62, 128);
		case PROPS_MOUTAIN_ROCK_CRISTAL_4:	 return IntRect(350, 132, 110, 112);
		case PROPS_MOUTAIN_ROCK_CRISTAL_5:	 return IntRect(50, 260, 76, 66);
		case PROPS_MOUTAIN_ROCK_1:			 return IntRect(50, 326, 28, 30);
		case PROPS_MOUTAIN_ROCK_2:			 return IntRect(28, 324, 38, 40);
		case PROPS_MOUTAIN_ROCK_3:			 return IntRect(76, 258, 40, 82);
		case PROPS_MOUTAIN_ROCK_4:			 return IntRect(116, 258, 58, 116);
		case PROPS_MOUTAIN_PLANT_1:			 return IntRect(174, 254, 70, 132);
		case PROPS_MOUTAIN_PLANT_2:			 return IntRect(244, 260, 84, 146);
		case PROPS_MOUTAIN_PLANT_3:			 return IntRect(328, 260, 120, 178);
		case PROPS_MOUTAIN_BUSH_1:			 return IntRect(0, 364, 56, 56);
		case PROPS_MOUTAIN_BUSH_2:			 return IntRect(56, 374, 64, 66);
		case PROPS_MOUTAIN_DOOR_FRAME:		 return IntRect(0, 450, 140, 150);
		case PROPS_MOUTAIN_DOOR_FRAME_TOP:	 return IntRect(140, 450, 140, 50);
		case PROPS_MOUTAIN_DOOR_SHADOW:		 return IntRect(368, 438, 88, 124);
		case PROPS_MOUTAIN_DOOR_SHADOW_TOP:	 return IntRect(140, 530, 108, 30);
		case PROPS_MOUTAIN_DOOR_LIGHT:		 return IntRect(280, 438, 88, 124);
		case PROPS_MOUTAIN_DOOR_LIGHT_TOP:	 return IntRect(140, 500, 108, 30);
		case PROPS_MOUTAIN_TRUNK:            return IntRect(120, 386, 74, 64);
		default:							 return IntRect(0, 0, 0, 0);
		}
		break;
	case PROPS_TYPE_DUNGEON:
		switch (_props.dungeon)
		{
		case PROPS_DUNGEON_DOOR_TOP:						return IntRect(7 * TILE_SIZE, 0 * TILE_SIZE, 1 * TILE_SIZE, 1 * TILE_SIZE);
		case PROPS_DUNGEON_DOOR:							return IntRect(7 * TILE_SIZE, 1 * TILE_SIZE, 1 * TILE_SIZE, 1 * TILE_SIZE);
		case PROPS_DUNGEON_CLOSED_DOOR_TOP:					return IntRect(8 * TILE_SIZE, 0 * TILE_SIZE, 1 * TILE_SIZE, 1 * TILE_SIZE);
		case PROPS_DUNGEON_CLOSED_DOOR:						return IntRect(8 * TILE_SIZE, 1 * TILE_SIZE, 1 * TILE_SIZE, 1 * TILE_SIZE);

		case PROPS_DUNGEON_POT_1:							return IntRect(0, 0, TILE_SIZE, TILE_SIZE);
		case PROPS_DUNGEON_POT_2:							return IntRect(0, TILE_SIZE, TILE_SIZE, TILE_SIZE);
		case PROPS_DUNGEON_MOVING_PLATEFORM:				return IntRect(0, 128, TILE_SIZE, TILE_SIZE);

		case PROPS_DUNGEON_MOVING_WALL_TOP:					return IntRect(0 * TILE_SIZE, 3 * TILE_SIZE, 1 * TILE_SIZE, 1 * TILE_SIZE);
		case PROPS_DUNGEON_MOVING_WALL_MID:					return IntRect(0 * TILE_SIZE, 4 * TILE_SIZE, 1 * TILE_SIZE, 1 * TILE_SIZE);
		case PROPS_DUNGEON_FLYER_BRIDGE_LEFT:				return IntRect(2 * TILE_SIZE, 4 * TILE_SIZE, 1 * TILE_SIZE, 1 * TILE_SIZE);
		case PROPS_DUNGEON_FLYER_BRIDGE_RIGHT:				return IntRect(2 * TILE_SIZE, 3 * TILE_SIZE, 1 * TILE_SIZE, 1 * TILE_SIZE);
		case PROPS_DUNGEON_FLYER_BRIDGE_TOP:				return IntRect(4 * TILE_SIZE, 2 * TILE_SIZE, 1 * TILE_SIZE, 1 * TILE_SIZE);
		case PROPS_DUNGEON_FLYER_BRIDGE_BOT:				return IntRect(4 * TILE_SIZE, 3 * TILE_SIZE, 1 * TILE_SIZE, 1 * TILE_SIZE);
		case PROPS_DUNGEON_LILYPAD:							return IntRect(6 * TILE_SIZE, 2 * TILE_SIZE, 2 * TILE_SIZE, 2 * TILE_SIZE);
		case PROPS_DUNGEON_BOULDER:							return IntRect(0 * TILE_SIZE, 5 * TILE_SIZE, 3 * TILE_SIZE, 3 * TILE_SIZE);
		case PROPS_DUNGEON_TRENCH_CORNER_TL:				return IntRect(0 * TILE_SIZE, 8 * TILE_SIZE, 1 * TILE_SIZE, 1 * TILE_SIZE);
		case PROPS_DUNGEON_TRENCH_TOP_EDGE:					return IntRect(1 * TILE_SIZE, 8 * TILE_SIZE, 1 * TILE_SIZE, 1 * TILE_SIZE);
		case PROPS_DUNGEON_TRENCH_CORNER_TR:				return IntRect(2 * TILE_SIZE, 8 * TILE_SIZE, 1 * TILE_SIZE, 1 * TILE_SIZE);
		case PROPS_DUNGEON_TRENCH_LEFT_EDGE:				return IntRect(0 * TILE_SIZE, 9 * TILE_SIZE, 1 * TILE_SIZE, 1 * TILE_SIZE);
		case PROPS_DUNGEON_TRENCH_RIGHT_EDGE:				return IntRect(2 * TILE_SIZE, 9 * TILE_SIZE, 1 * TILE_SIZE, 1 * TILE_SIZE);
		case PROPS_DUNGEON_TRENCH_CORNER_BL:				return IntRect(0 * TILE_SIZE, 10 * TILE_SIZE, 1 * TILE_SIZE, 1 * TILE_SIZE);
		case PROPS_DUNGEON_TRENCH_BOTTOM_EDGE:				return IntRect(1 * TILE_SIZE, 10 * TILE_SIZE, 1 * TILE_SIZE, 1 * TILE_SIZE);
		case PROPS_DUNGEON_TRENCH_CORNER_BR:				return IntRect(2 * TILE_SIZE, 10 * TILE_SIZE, 1 * TILE_SIZE, 1 * TILE_SIZE);
		case PROPS_DUNGEON_TRENCH_VB_END:					return IntRect(3 * TILE_SIZE, 8 * TILE_SIZE, 1 * TILE_SIZE, 1 * TILE_SIZE);
		case PROPS_DUNGEON_TRENCH_VT_END:					return IntRect(4 * TILE_SIZE, 8 * TILE_SIZE, 1 * TILE_SIZE, 1 * TILE_SIZE);
		case PROPS_DUNGEON_TRENCH_HL_END:					return IntRect(3 * TILE_SIZE, 9 * TILE_SIZE, 1 * TILE_SIZE, 1 * TILE_SIZE);
		case PROPS_DUNGEON_TRENCH_HR_END:					return IntRect(4 * TILE_SIZE, 9 * TILE_SIZE, 1 * TILE_SIZE, 1 * TILE_SIZE);
		case PROPS_DUNGEON_BOULDER_BTN:						return IntRect(3 * TILE_SIZE, 10 * TILE_SIZE, 1 * TILE_SIZE, 1 * TILE_SIZE);
		case PROPS_DUNGEON_PAVING_STONE:					return IntRect(0 * TILE_SIZE, 11 * TILE_SIZE, 1 * TILE_SIZE, 2 * TILE_SIZE);

		case PROPS_DUNGEON_PILLAR_TOP_1:					return IntRect(0 * TILE_SIZE, 0 * TILE_SIZE, 1 * TILE_SIZE, 1 * TILE_SIZE);
		case PROPS_DUNGEON_PILLAR_TOP_2:					return IntRect(1 * TILE_SIZE, 0 * TILE_SIZE, 1 * TILE_SIZE, 1 * TILE_SIZE);
		case PROPS_DUNGEON_PILLAR_LEFT_1:					return IntRect(2 * TILE_SIZE, 0 * TILE_SIZE, 1 * TILE_SIZE, 1 * TILE_SIZE);
		case PROPS_DUNGEON_PILLAR_MID_1:					return IntRect(3 * TILE_SIZE, 0 * TILE_SIZE, 1 * TILE_SIZE, 1 * TILE_SIZE);
		case PROPS_DUNGEON_PILLAR_RIGHT_1:					return IntRect(4 * TILE_SIZE, 0 * TILE_SIZE, 1 * TILE_SIZE, 1 * TILE_SIZE);
		case PROPS_DUNGEON_PILLAR_MID_2:					return IntRect(0 * TILE_SIZE, 1 * TILE_SIZE, 1 * TILE_SIZE, 1 * TILE_SIZE);
		case PROPS_DUNGEON_PILLAR_MID_3:					return IntRect(1 * TILE_SIZE, 1 * TILE_SIZE, 1 * TILE_SIZE, 1 * TILE_SIZE);
		case PROPS_DUNGEON_PILLAR_MID_4:					return IntRect(2 * TILE_SIZE, 1 * TILE_SIZE, 1 * TILE_SIZE, 1 * TILE_SIZE);
		case PROPS_DUNGEON_PILLAR_MID_5:					return IntRect(3 * TILE_SIZE, 1 * TILE_SIZE, 1 * TILE_SIZE, 1 * TILE_SIZE);
		case PROPS_DUNGEON_PILLAR_MID_6:					return IntRect(4 * TILE_SIZE, 1 * TILE_SIZE, 1 * TILE_SIZE, 1 * TILE_SIZE);
		case PROPS_DUNGEON_PILLAR_BOTTOM_1:					return IntRect(0 * TILE_SIZE, 2 * TILE_SIZE, 1 * TILE_SIZE, 1 * TILE_SIZE);
		case PROPS_DUNGEON_PILLAR_BOTTOM_2:					return IntRect(1 * TILE_SIZE, 2 * TILE_SIZE, 1 * TILE_SIZE, 1 * TILE_SIZE);
		case PROPS_DUNGEON_PILLAR_LEFT_2:					return IntRect(2 * TILE_SIZE, 2 * TILE_SIZE, 1 * TILE_SIZE, 1 * TILE_SIZE);
		case PROPS_DUNGEON_PILLAR_MID_7:					return IntRect(3 * TILE_SIZE, 2 * TILE_SIZE, 1 * TILE_SIZE, 1 * TILE_SIZE);
		case PROPS_DUNGEON_PILLAR_RIGHT_2:					return IntRect(4 * TILE_SIZE, 2 * TILE_SIZE, 1 * TILE_SIZE, 1 * TILE_SIZE);
		case PROPS_DUNGEON_PRESSURE_PLATE:					return IntRect(0 * TILE_SIZE, 3 * TILE_SIZE, 1 * TILE_SIZE, 1 * TILE_SIZE);
		case PROPS_DUNGEON_BOSS_DOOR_TOP:					return IntRect(0 * TILE_SIZE, 4 * TILE_SIZE, 1 * TILE_SIZE, 1 * TILE_SIZE);
		case PROPS_DUNGEON_BOSS_DOOR:						return IntRect(0 * TILE_SIZE, 5 * TILE_SIZE, 6 * TILE_SIZE, 2 * TILE_SIZE);
		case PROPS_DUNGEON_BRAZIER:							return IntRect(0 * TILE_SIZE, 7 * TILE_SIZE, 1 * TILE_SIZE, 1 * TILE_SIZE);
		case PROPS_DUNGEON_BANNER:							return IntRect(0 * TILE_SIZE, 8 * TILE_SIZE, 1 * TILE_SIZE, 2 * TILE_SIZE);
		case PROPS_DUNGEON_LITTLE_CHEST:					return IntRect(0 * TILE_SIZE, 10 * TILE_SIZE, 1 * TILE_SIZE, 1 * TILE_SIZE);
		case PROPS_DUNGEON_BIG_CHEST:						return IntRect(0 * TILE_SIZE, 11 * TILE_SIZE, 2 * TILE_SIZE, 2 * TILE_SIZE);

		case PROPS_DUNGEON_HINT_STATUE:						return IntRect(0, 1204, 96, 198);
		case PROPS_DUNGEON_STATUE_BASE_HEAD:				return IntRect(224, 878, 148, 104);
		case PROPS_DUNGEON_STATUE_BASE_TORSO:				return IntRect(372, 878, 148, 104);
		case PROPS_DUNGEON_STATUE_HALF_HEAD:				return IntRect(224, 982, 164, 110);
		case PROPS_DUNGEON_STATUE_HALF_TORSO:				return IntRect(388, 982, 164, 110);
		case PROPS_DUNGEON_STATUE_FULL:						return IntRect(0, 878, 224, 326);
		case PROPS_DUNGEON_STAND:							return IntRect(0, 1402, 132, 128);

		case PROPS_DUNGEON_BUILD_JOY:						return IntRect(0, 0, 1802, 1534);
		case PROPS_DUNGEON_OUTLINE_DOOR_JOY:				return IntRect(0, 1534, 404, 268);
		case PROPS_DUNGEON_DOOR_JOY:						return IntRect(0, 1802, 228, 186);
		case PROPS_DUNGEON_ENTRANCE_JOY:					return IntRect(0, 1988, 228, 186);
		case PROPS_DUNGEON_ENTRANCE_PRESSURE_PLATE_JOY:		return IntRect(0, 2174, 128, 128);

		case PROPS_DUNGEON_BUILD_FRONT_SADNESS:				return IntRect(0, 0, 1802, 1534);
		case PROPS_DUNGEON_BUILD_BACK_SADNESS:				return IntRect(1802, 0, 1802, 1556);
		case PROPS_DUNGEON_OUTLINE_DOOR_SADNESS:			return IntRect(0, 1534, 404, 268);
		case PROPS_DUNGEON_DOOR_SADNESS:					return IntRect(0, 1802, 228, 186);
		case PROPS_DUNGEON_ENTRANCE_SADNESS:				return IntRect(0, 1988, 228, 186);
		case PROPS_DUNGEON_ENTRANCE_PRESSURE_PLATE_SADNESS: return IntRect(0, 2174, 128, 128);

		case PROPS_DUNGEON_BUILD_FRONT_FEAR:				return IntRect(0, 0, 1802, 1534);
		case PROPS_DUNGEON_BUILD_BACK_FEAR:					return IntRect(1802, 0, 1802, 1556);
		case PROPS_DUNGEON_OUTLINE_DOOR_FEAR:				return IntRect(0, 1534, 404, 268);
		case PROPS_DUNGEON_DOOR_FEAR:						return IntRect(0, 1802, 228, 186);
		case PROPS_DUNGEON_ENTRANCE_FEAR:					return IntRect(0, 1988, 228, 186);
		case PROPS_DUNGEON_ENTRANCE_PRESSURE_PLATE_FEAR:	return IntRect(0, 2174, 128, 128);

		case PROPS_DUNGEON_BUILD_FRONT_ANGRY:				return IntRect(0, 0, 1802, 1534);
		case PROPS_DUNGEON_BUILD_BACK_ANGRY:				return IntRect(1802, 0, 1802, 1556);
		case PROPS_DUNGEON_OUTLINE_DOOR_ANGRY:				return IntRect(0, 1534, 404, 268);
		case PROPS_DUNGEON_DOOR_ANGRY:						return IntRect(0, 1802, 228, 186);
		case PROPS_DUNGEON_ENTRANCE_ANGRY:					return IntRect(0, 1988, 228, 186);
		case PROPS_DUNGEON_ENTRANCE_PRESSURE_PLATE_ANGRY:	return IntRect(0, 2174, 128, 128);

		default:											return IntRect(0, 0, 0, 0);
		}
		break;
	case PROPS_TYPE_BEACH:
		switch (_props.beach)
		{
		case PROPS_BEACH_PALM_1:		return IntRect(0, 0, 206, 394);
		case PROPS_BEACH_PALM_2:		return IntRect(0, 458, 248, 388);
		case PROPS_BEACH_TRUNK_1:		return IntRect(0, 394, 42, 64);
		case PROPS_BEACH_TRUNK_2:		return IntRect(0, 846, 40, 64);
		case PROPS_BEACH_DEAD_CORAL_1:	return IntRect(0, 910, 52, 38);
		case PROPS_BEACH_DEAD_CORAL_2:	return IntRect(52, 910, 58, 38);
		case PROPS_BEACH_DEAD_CORAL_3:	return IntRect(110, 910, 44, 42);
		case PROPS_BEACH_DEAD_CORAL_4:	return IntRect(154, 910, 46, 46);
		case PROPS_BEACH_STONE_1:		return IntRect(200, 910, 36, 38);
		case PROPS_BEACH_STONE_2:		return IntRect(236, 910, 42, 40);
		case PROPS_BEACH_STAR_1:		return IntRect(278, 910, 56, 52);
		case PROPS_BEACH_STAR_2:		return IntRect(334, 910, 56, 52);
		case PROPS_BEACH_CORAL_1:		return IntRect(390, 910, 82, 82);
		case PROPS_BEACH_CORAL_2:		return IntRect(472, 910, 84, 80);
		case PROPS_BEACH_CORAL_3:		return IntRect(556, 910, 72, 88);
		case PROPS_BEACH_CORAL_4:		return IntRect(628, 910, 72, 88);
		case PROPS_BEACH_CORAL_5:		return IntRect(0, 948, 80, 70);
		case PROPS_BEACH_CORAL_6:		return IntRect(80, 956, 78, 68);
		case PROPS_BEACH_CORAL_7:		return IntRect(158, 956, 64, 68);
		case PROPS_BEACH_CORAL_8:		return IntRect(222, 962, 66, 66);
		case PROPS_BEACH_ROCK_1:		return IntRect(0, 1024, 128, 166);
		case PROPS_BEACH_ROCK_2:		return IntRect(128, 1028, 190, 160);
		case PROPS_BEACH_ROCK_3:		return IntRect(318, 992, 54, 50);
		case PROPS_BEACH_ROCK_4:		return IntRect(318, 1042, 80, 58);
		case PROPS_BEACH_ROCK_5:		return IntRect(0, 1190, 558, 378);
		default:						return IntRect(0, 0, 0, 0);
		}
		break;
	default:							return IntRect(0, 0, 0, 0);
	}
}

sfVector2f map_SnapToGrid(sfVector2f _pos)
{
	return vectorSnap(_pos, TILE_SIZE);
}

void map_changeMapState(Window* _window, MapState _state)
{
	
	isFreeingMap = sfTrue;
	if (_state >= MAP_STATE_DUNGEON_JOY_1 && state <= MAP_STATE_DUNGEON_JOY_FOUNTAIN) {
		stopMusicIfPlaying("Music_Main");
		playMusicIfNotPlaying("Music_Joie");
	}
	else if(_state < MAP_STATE_LIGHTHOUSE_BEDROOM || _state > MAP_STATE_LIGHTHOUSE_ROOF) {
		stopMusicIfPlaying("Music_Joie");
		playMusicIfNotPlaying("Music_Main"); 
	}

	setOnePassOPDoors();
	for (int i = 0; i < 100; i++) {
		getMagnitudeBetween2Vectorsf(vector2f(100, 100), vector2f(400.f, 400.f));
	}
	map_DeinitMap();
	map_setPlayerPos(_state);
	mapState = _state;
	map_InitMap(_window);
	map_LoadMap(mapState);
	chest_loadTexture();
	brz_TextureOnLoad();
	opDoors_loadTexture();
	isFreeingMap = sfFalse;

	if (_state == MAP_STATE_LIGHTHOUSE_BEDROOM && !firstPassPotSpawn) {
		pot_Create(vector2f(600.f, 720.f), sfFalse, 0);
		pot_Create(vector2f(664.f, 720.f), sfFalse, 0);
		pot_Create(vector2f(664.f, 784.f), sfFalse, 0);
		pot_Create(vector2f(600.f, 784.f), sfFalse, 0);
		firstPassPotSpawn = sfTrue;
	}

	if (_state >= MAP_STATE_LIGHTHOUSE_BEDROOM && _state <= MAP_STATE_LIGHTHOUSE_ROOF) {
		plr_SetPos(vector2f(1230.f, 700.f));
	}
}

MapState map_GetState() {
	return mapState;
}

LayerProps* map_getPropsListToSave()
{
	if (propsList == NULL)
		return NULL;

	int len = propsList->size(propsList);
	LayerProps* tmp = calloc(len, sizeof(LayerProps));

	if (!tmp) return NULL;

	for (int i = 0; i < len; i++) tmp[i] = *STD_LIST_GETDATA(propsList, LayerProps, i);
	return tmp;
}

void map_setOnePassMap(sfBool _value)
{
	onePassMap = _value;
}

sfBool map_CanFall(sfVector2f _pos)
{
	sfVector2i index = map_getBlockPos(_pos);
	if (map_isIndexInArray(index))
	{
		if (map.layerGround_first[index.x][index.y].type == TILE_NO_TILE && map.layerGround_second[index.x][index.y].type == TILE_NO_TILE && map.layerWalls_first[index.x][index.y].type == WALL_NO_TILE && map.layerWalls_second[index.x][index.y].type == WALL_NO_TILE)
			return sfTrue;
	}
	return sfFalse;
}

sfTexture* map_getBiomeTexture(Layers _layer, int i, int j, LayerProps* _props)
{
	sfTexture* tex = NULL;
	switch (_layer)
	{
	case LAYER_GROUND_FIRST:
		if (map.layerGround_first[i][j].type < TILE_GROUND_DIRT_1)
		{
			if (map.layerGround_first[i][j].type & TILE_MATERIAL_SAND) tex = GetSfTexture("ts_ground_sand");
			else if (map.layerGround_first[i][j].type & TILE_MATERIAL_ROCK) tex = GetSfTexture("ts_ground_rock");
			else if (map.layerGround_first[i][j].type & TILE_MATERIAL_GRASS) tex = GetSfTexture("ts_ground_grass");
			else if (map.layerGround_first[i][j].type & TILE_MATERIAL_WATER) tex = GetSfTexture("ts_ground_water");
		}
		else
		{
			if (map.layerGround_first[i][j].type >= TILE_GROUND_DIRT_1 && map.layerGround_first[i][j].type <= TILE_GROUND_DIRT_4) tex = GetSfTexture("ts_ground_dirt");
			else if (map.layerGround_first[i][j].type >= TILE_GROUND_DUNGEON_1 || map.layerGround_first[i][j].type <= TILE_GROUND_DUNGEON_4) tex = GetSfTexture("ts_ground_dungeon");
		}
		break;
	case LAYER_GROUND_SECOND:
		if (map.layerGround_second[i][j].type < TILE_GROUND_DIRT_1)
		{
			if (map.layerGround_second[i][j].type & TILE_MATERIAL_SAND) tex = GetSfTexture("ts_ground_sand");
			else if (map.layerGround_second[i][j].type & TILE_MATERIAL_ROCK) tex = GetSfTexture("ts_ground_rock");
			else if (map.layerGround_second[i][j].type & TILE_MATERIAL_GRASS) tex = GetSfTexture("ts_ground_grass");
			else if (map.layerGround_second[i][j].type & TILE_MATERIAL_WATER) tex = GetSfTexture("ts_ground_water");
			else if (map.layerGround_second[i][j].type & TILE_MATERIAL_LEAF) tex = GetSfTexture("ts_ground_leaf");
		}
		else
		{
			if (map.layerGround_second[i][j].type >= TILE_GROUND_DIRT_1 && map.layerGround_second[i][j].type <= TILE_GROUND_DIRT_4) tex = GetSfTexture("ts_ground_dirt");
			else if (map.layerGround_second[i][j].type >= TILE_GROUND_DUNGEON_1 || map.layerGround_second[i][j].type <= TILE_GROUND_DUNGEON_4) tex = GetSfTexture("ts_ground_dungeon");
		}
		break;
	case LAYER_WALLS_FIRST:
		if (map.layerWalls_first[i][j].type & WALL_MATERIAL_IN) tex = GetSfTexture("ts_wall_int");
		else tex = GetSfTexture("ts_wall_ext");
		break;
	case LAYER_WALLS_SECOND:
		if (map.layerWalls_second[i][j].type & WALL_MATERIAL_IN) tex = GetSfTexture("ts_wall_int");
		else tex = GetSfTexture("ts_wall_ext");
		break;
	case LAYER_PROPS:
		if (_props->type == PROPS_TYPE_FOREST)
		{
			if (_props->forest == PROPS_FOREST_SPLASH)
				tex = GetSfTexture("assets_splash");
			else if (_props->forest == PROPS_FOREST_WATERFALL)
				tex = GetSfTexture("assets_waterfall");
			else if (_props->forest == PROPS_FOREST_GRASS_1 || _props->forest == PROPS_FOREST_GRASS_2)
				tex = GetSfTexture("grass1");
			else
				tex = GetSfTexture("assets_forest");
		}
		else if (_props->type == PROPS_TYPE_MOUTAIN) tex = GetSfTexture("assets_moutain");
		else if (_props->type == PROPS_TYPE_BEACH) tex = GetSfTexture("assets_beach");
		else if (_props->type == PROPS_TYPE_GENERAL) tex = GetSfTexture("assets_general");
		else if (_props->type == PROPS_TYPE_DUNGEON)
		{
			if (_props->dungeon >= PROPS_DUNGEON_DOOR_TOP && _props->dungeon <= PROPS_DUNGEON_PAVING_STONE)	tex = GetSfTexture("assets_dungeon");
			else if (_props->dungeon >= PROPS_DUNGEON_PILLAR_TOP_1 && _props->dungeon <= PROPS_DUNGEON_BIG_CHEST) tex = GetSfTexture("assets_dungeon_biome");
			else if (_props->dungeon >= PROPS_DUNGEON_BUILD_JOY && _props->dungeon <= PROPS_DUNGEON_ENTRANCE_PRESSURE_PLATE_JOY) tex = GetSfTexture("assets_dj_joy");
			else if (_props->dungeon >= PROPS_DUNGEON_BUILD_FRONT_SADNESS && _props->dungeon <= PROPS_DUNGEON_ENTRANCE_PRESSURE_PLATE_SADNESS) tex = GetSfTexture("assets_dj_sadness");
			else if (_props->dungeon >= PROPS_DUNGEON_BUILD_FRONT_FEAR && _props->dungeon <= PROPS_DUNGEON_ENTRANCE_PRESSURE_PLATE_FEAR) tex = GetSfTexture("assets_dj_fear");
			else if (_props->dungeon >= PROPS_DUNGEON_BUILD_FRONT_ANGRY && _props->dungeon <= PROPS_DUNGEON_ENTRANCE_PRESSURE_PLATE_ANGRY) tex = GetSfTexture("assets_dj_angry");
		}
		break;
	}
	return tex;
}

void setCurrentBiome(Biomes _biome)
{
	RemoveAllBiomeTexture(currentBiome);
	currentBiome = _biome;
	BiomeTexture_Onload(GAME, currentBiome);
}

Biomes getCurrentBiome()
{
	return currentBiome;
}

void waterfallAnimator(sfIntRect* _rect, int _nbFrameX, int _nbFrameY, float _timeAnim, float* _timer)
{
	if (*_timer > _timeAnim)
	{
		if (_rect->left < _rect->width * _nbFrameX)
			_rect->left += _rect->width;
		else if (_rect->left >= _rect->width * _nbFrameX)
		{
			_rect->left = 0;
			_rect->top += _rect->height;
		}
		if (_rect->top > _rect->height * _nbFrameY)
		{
			_rect->left = 0;
			_rect->top = 0;
		}
		*_timer = 0.f;
	}
}

void fillTransitionZone()
{
	//	if (currentMap >= MAP_STATE_LIGHTHOUSE_BEDROOM && currentMap <= MAP_STATE_LIGHTHOUSE_ROOF) return;
	if (changeMapRect) free(changeMapRect);
	switch (currentMap)
	{
		case MAP_STATE_OVERWORLD:	
			nbDifferentAcces = 3;
			changeMapRect = (sfIntRect*)calloc(nbDifferentAcces, sizeof(sfIntRect));
			if (changeMapRect != NULL)
			{
				changeMapRect[0] = IntRect(2295, 2, 492, 262);
				changeMapRect[1] = IntRect(6457, 10810, 221, 139);
				changeMapRect[2] = IntRect(7888, 2753, 73, 95);
			}
			break;
		case MAP_STATE_FOREST:
			nbDifferentAcces = 1;
			changeMapRect = (sfIntRect*)calloc(nbDifferentAcces, sizeof(sfIntRect));
			if (changeMapRect != NULL)
			{
				changeMapRect[0] = IntRect(2930, 5314, 270, 111);
			}
			break;
		case MAP_STATE_DUNGEON_JOY_1:
			nbDifferentAcces = 3;
			changeMapRect = (sfIntRect*)calloc(nbDifferentAcces, sizeof(sfIntRect));
			if (changeMapRect != NULL)
			{
				changeMapRect[0] = IntRect(832, 1024, 256, 53);
				changeMapRect[1] = IntRect(0, 445, 56, 264);
				changeMapRect[2] = IntRect(768, 0, 382, 184);
			}
			break;
		case MAP_STATE_DUNGEON_JOY_2:
			nbDifferentAcces = 2;
			changeMapRect = (sfIntRect*)calloc(nbDifferentAcces, sizeof(sfIntRect));
			if (changeMapRect != NULL)
			{
				changeMapRect[0] = IntRect(1863, 447, 51, 264);
				changeMapRect[1] = IntRect(833, 5, 253, 143);
			}
			break;
		case MAP_STATE_DUNGEON_JOY_3:
			nbDifferentAcces = 3;
			changeMapRect = (sfIntRect*)calloc(nbDifferentAcces, sizeof(sfIntRect));
			if (changeMapRect != NULL)
			{
				changeMapRect[0] = IntRect(831, 1037, 254, 42);
				changeMapRect[1] = IntRect(1867, 445, 51, 264);
				changeMapRect[2] = IntRect(839, 2, 250, 165);
			}
			break;
		case MAP_STATE_DUNGEON_JOY_4:
			nbDifferentAcces = 2;
			changeMapRect = (sfIntRect*)calloc(nbDifferentAcces, sizeof(sfIntRect));
			if (changeMapRect != NULL)
			{
				changeMapRect[0] = IntRect(0, 450, 51, 264);
				changeMapRect[1] = IntRect(835, 0, 246, 152);
			}
			break;
		case MAP_STATE_DUNGEON_JOY_5:
			nbDifferentAcces = 2;
			changeMapRect = (sfIntRect*)calloc(nbDifferentAcces, sizeof(sfIntRect));
			if (changeMapRect != NULL)
			{
				changeMapRect[0] = IntRect(1531, 1035, 257, 43);
				changeMapRect[1] = IntRect(0, 451, 50, 264);
			}
			break;
		case MAP_STATE_DUNGEON_JOY_6:
			nbDifferentAcces = 2;
			changeMapRect = (sfIntRect*)calloc(nbDifferentAcces, sizeof(sfIntRect));
			if (changeMapRect != NULL)
			{
				changeMapRect[0] = IntRect(1870, 445, 48, 264);
				changeMapRect[1] = IntRect(383, 1040, 266, 37);
			}
			break;
		case MAP_STATE_DUNGEON_JOY_BOSS:
			nbDifferentAcces = 2;
			changeMapRect = (sfIntRect*)calloc(nbDifferentAcces, sizeof(sfIntRect));
			if (changeMapRect != NULL)
			{
				changeMapRect[0] = IntRect(1075, 2117, 310, 53);
				changeMapRect[1] = IntRect(1075, 190, 310, 170);
			}
			break;
		case MAP_STATE_DUNGEON_JOY_FOUNTAIN:
			nbDifferentAcces = 1;
			changeMapRect = (sfIntRect*)calloc(nbDifferentAcces, sizeof(sfIntRect));
			if (changeMapRect != NULL)
				changeMapRect[0] = IntRect(763, 1033, 399, 45);
			break;
		case MAP_STATE_LIGHTHOUSE_OUTSIDE:
			nbDifferentAcces = 1;
			changeMapRect = (sfIntRect*)calloc(nbDifferentAcces, sizeof(sfIntRect));
			if (changeMapRect != NULL)
				changeMapRect[0] = IntRect(800, 780, 320, 100);
			break;
		case MAP_STATE_CAVE:
			nbDifferentAcces = 2;
			changeMapRect = (sfIntRect*)calloc(nbDifferentAcces, sizeof(sfIntRect));
			if (changeMapRect != NULL)
			{
				changeMapRect[0] = IntRect(5268, 87, 159, 50);
				changeMapRect[1] = IntRect(603, 2013, 197, 40);
			}
			break;
		case MAP_STATE_MOUTAIN:
			nbDifferentAcces = 2;
			changeMapRect = (sfIntRect*)calloc(nbDifferentAcces, sizeof(sfIntRect));
			if (changeMapRect != NULL)
			{
				changeMapRect[0] = IntRect(353, 3130, 83, 78);
				changeMapRect[1] = IntRect(5671, 1424, 109, 676);
			}
			break;
		case MAP_STATE_DUNGEON_SADNESS_1:
			nbDifferentAcces = 2;
			changeMapRect = (sfIntRect*)calloc(nbDifferentAcces, sizeof(sfIntRect));
			if (changeMapRect != NULL)
			{
				changeMapRect[0] = IntRect(812, 1049, 301, 78);
				changeMapRect[1] = IntRect(802, 24, 326, 119);
			}
			break;
		case MAP_STATE_DUNGEON_SADNESS_2:
			nbDifferentAcces = 2;
			changeMapRect = (sfIntRect*)calloc(nbDifferentAcces, sizeof(sfIntRect));
			if (changeMapRect != NULL)
			{
				changeMapRect[0] = IntRect(805, 1050, 306, 32);
				changeMapRect[1] = IntRect(737, 30, 434, 88);
			}
			break;
		case MAP_STATE_DUNGEON_SADNESS_BOSS:
			nbDifferentAcces = 1;
			changeMapRect = (sfIntRect*)calloc(nbDifferentAcces, sizeof(sfIntRect));
			if (changeMapRect != NULL)
				changeMapRect[0] = IntRect(1059, 2138, 391, 48);
			break;
		default:
			nbDifferentAcces = 0;
			changeMapRect = (sfIntRect*)calloc(nbDifferentAcces, sizeof(sfIntRect));
			break;
	}
}

int checkPlayerWalkType(sfVector2i _pos)
{
	if (isFreeingMap || !map.isAlloc)
		return 1;
	if (mvpl_IsPlayerWalkingOnPlatform()) return 1; // TODO add tests for bridges
	sfVector2i tmpPos = vector2i(_pos.y, _pos.x);
	if (map_isIndexInArray(tmpPos))
	{
		if (map.layerCollision[_pos.y][_pos.x].type == CLD_VOID)
			return 0;
		if (map.layerGround_first[_pos.y][_pos.x].type >= TILE_WATER_FULL
			&& map.layerGround_first[_pos.y][_pos.x].type < TILE_WATER_FULL_DEEP)
			return 3;
		if (isPlayerOnGrass(_pos))
			return 2;
	}
	return 1;
}

sfBool map_IsPlayerInRealVoid(sfVector2i _pos)
{
	if (isFreeingMap || !map.isAlloc)
		return sfTrue;
	sfVector2i tmpPos = vector2i(_pos.y, _pos.x);
	if (map_isIndexInArray(tmpPos))
	{
		if (map.layerGround_first[_pos.y][_pos.x].type == TILE_WATER_FULL_DEEP)
			return sfFalse;
		if (isPlayerOnGrass(_pos))
			return sfFalse;
	}
	return sfTrue;
}

sfBool checkBoxVoidCollision(sfVector2i _pos)
{
	if (isFreeingMap)
		return sfFalse;
	sfVector2i tmpPos = vector2i(_pos.y, _pos.x);
	if (map_isIndexInArray(tmpPos))
	{
		if (map.layerCollision[_pos.y][_pos.x].type == CLD_VOID) {
			map.layerCollision[_pos.y][_pos.x].type = CLD_NO_COLL;
			return sfTrue;
		}
	}
	return sfFalse;
}

sfBool map_IsIndexInWater(sfVector2i _index)
{
	if (!map_isIndexInArray(_index))
		return sfFalse;
	if (map.layerGround_first[_index.y][_index.x].type == TILE_WATER_FULL)
		return sfTrue;

	return sfFalse;
}

sfVector2f map_getPlayerSpawnPos()
{
	return playerSpawnPos;
}

sfBool getIsFreeingMap()
{
	return isFreeingMap;
}

void removeCollision(sfVector2i _index) {
	sfVector2i tmpPos = vector2i(_index.y, _index.x);
	if (!map_isIndexInArray(tmpPos))
		return;
	map.layerCollision[_index.y][_index.x].type = CLD_NO_COLL;
}

void mapReset(Window* _window) {
	isFreeingMap = sfTrue;
	for (int i = 0; i < 300; i++) {
		getMagnitudef(createVectorWithVectorf(vector2f(200.f, 400.f), vector2f(800.f, 700.f)));
	}
	map_clearPropsList();
	box_Clear();
	pot_Clear();
	mvpl_clearList();
	expt_clearList();
	chest_clearChestList();
	clearGrass();
	fishspot_Clear();
	stree_clearSpidersTrees();
	note_clearNoteList();
	brz_Clear();
	opDoors_clearDoorsList();
	stt_clear();

	light_Clear();
	en_Clear();
	npc_Clear();

	prt_clear();

	map_LoadProps();
	map_LoadEntity();
	mvpl_ConstituteGroups();
	getGrassTexture();
	plr_loadTexture();

	if (map_GetState() == MAP_STATE_DUNGEON_JOY_BOSS) bossJoie_restartBoss(&entity_BossJoie, vector2f(1247.f, 1116.f));

	isFreeingMap = sfFalse;
}

stdList* map_GetPropsList() { return propsList; }

void setFirstPassPotSpawn() {
	firstPassPotSpawn = sfFalse;
}

void map_setPlayerPos(MapState _state)
{
	switch (_state)
	{
	case MAP_STATE_OVERWORLD:
		if (mapState >= MAP_STATE_LIGHTHOUSE_BEDROOM && mapState <= MAP_STATE_LIGHTHOUSE_ROOF) playerSpawnPos = vector2f(0.f, 0.f);
		else if (mapState == MAP_STATE_FOREST) playerSpawnPos = vector2f(0.f, 0.f);
		else if (mapState == MAP_STATE_CAVE) playerSpawnPos = vector2f(0.f, 0.f);
		else if (mapState == MAP_STATE_DUNGEON_JOY_1) playerSpawnPos = vector2f(0.f, 0.f);
		break;
	case MAP_STATE_DUNGEON_JOY_1:
		if (mapState == MAP_STATE_OVERWORLD) playerSpawnPos = vector2f(0.f, 0.f);
		else if (mapState == MAP_STATE_DUNGEON_JOY_2) playerSpawnPos = vector2f(0.f, 0.f);
		else if (mapState == MAP_STATE_DUNGEON_JOY_BOSS) playerSpawnPos = vector2f(0.f, 0.f);
		break;
	case MAP_STATE_DUNGEON_JOY_2:
		if (mapState == MAP_STATE_DUNGEON_JOY_1) playerSpawnPos = vector2f(0.f, 0.f);
		else if (mapState == MAP_STATE_DUNGEON_JOY_3) playerSpawnPos = vector2f(0.f, 0.f);
		break;
	case MAP_STATE_DUNGEON_JOY_3:
		if (mapState == MAP_STATE_DUNGEON_JOY_2) playerSpawnPos = vector2f(0.f, 0.f);
		else if (mapState == MAP_STATE_DUNGEON_JOY_4) playerSpawnPos = vector2f(0.f, 0.f);
		else if (mapState == MAP_STATE_DUNGEON_JOY_6) playerSpawnPos = vector2f(0.f, 0.f);
		break;
	case MAP_STATE_DUNGEON_JOY_4:
		if (mapState == MAP_STATE_DUNGEON_JOY_3) playerSpawnPos = vector2f(0.f, 0.f);
		else if (mapState == MAP_STATE_DUNGEON_JOY_5) playerSpawnPos = vector2f(0.f, 0.f);
		break;
	case MAP_STATE_DUNGEON_JOY_5:
		if (mapState == MAP_STATE_DUNGEON_JOY_4) playerSpawnPos = vector2f(0.f, 0.f);
		else if (mapState == MAP_STATE_DUNGEON_JOY_6) playerSpawnPos = vector2f(0.f, 0.f);
		break;
	case MAP_STATE_DUNGEON_JOY_6:
		if (mapState == MAP_STATE_DUNGEON_JOY_3) playerSpawnPos = vector2f(0.f, 0.f);
		else if (mapState == MAP_STATE_DUNGEON_JOY_5) playerSpawnPos = vector2f(0.f, 0.f);
		break;
	case MAP_STATE_DUNGEON_JOY_BOSS:
		if (mapState == MAP_STATE_DUNGEON_JOY_1) playerSpawnPos = vector2f(0.f, 0.f);
		else if (mapState == MAP_STATE_DUNGEON_JOY_FOUNTAIN) playerSpawnPos = vector2f(0.f, 0.f);
		break;
	case MAP_STATE_CAVE:
		if (mapState == MAP_STATE_OVERWORLD) playerSpawnPos = vector2f(0.f, 0.f);
		else if (mapState == MAP_STATE_MOUTAIN) playerSpawnPos = vector2f(0.f, 0.f);
		break;
	case MAP_STATE_MOUTAIN:
		if (mapState == MAP_STATE_CAVE) playerSpawnPos = vector2f(0.f, 0.f);
		else if (mapState == MAP_STATE_DUNGEON_SADNESS_1) playerSpawnPos = vector2f(0.f, 0.f);
		break;
	case MAP_STATE_DUNGEON_SADNESS_1:
		if (mapState == MAP_STATE_MOUTAIN) playerSpawnPos = vector2f(0.f, 0.f);
		else if (mapState == MAP_STATE_DUNGEON_SADNESS_2) playerSpawnPos = vector2f(0.f, 0.f);
		break;
	case MAP_STATE_DUNGEON_SADNESS_2:
		if (mapState == MAP_STATE_DUNGEON_SADNESS_1) playerSpawnPos = vector2f(0.f, 0.f);
		else if (mapState == MAP_STATE_DUNGEON_SADNESS_BOSS) playerSpawnPos = vector2f(0.f, 0.f);
		break;
	default: break;
	}
}