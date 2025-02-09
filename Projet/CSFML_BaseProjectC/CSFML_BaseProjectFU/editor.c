#include "editor.h"
#include "stateManager.h"
#include "particlesManager.h"
#include "enemies.h"
#include "box.h"
#include "light.h"
#include "npc.h"
#include "pot.h"
#include "quest.h"
#include "player.h"
#include "editorHUD.h"
#include "chest.h"
#include "moving_platform.h"
#include "openDoors.h"
#include "spidersTrees.h"
#include "statue.h"
#include "grass.h"
#include "note.h"
#include "fishing_spot.h"
#include "brazier.h"
#include "particlesManager.h"
#include "dialogBox.h"
#include "particlesManager.h"

#define EDITOR_HUD_XPOS 1650.f

#pragma warning (disable: 6308)

TilesType editorCurrentTile;
WallsType editorCurrentWall;
WallsType editorWallMaterial;
CollisionType editorCurrentCollision;

sfRectangleShape* rectTileCursor;
sfRectangleShape* editorPreviewTile;

EditorState editorState;
EditorEntity editorEntity;
EditorEffects editorEffects;

sfVector2f playerPosition;

sfSprite* editorPropsCursor;
sfSprite* editorPreviewProps;
sfSprite* editorPlayer;

int indexMap;
int x;
int y;
int brushSize = 1;
int groundMaterialValue;
int wallMaterialValue;

int editorValueRateTile1;
int editorValueRateTile2;
int editorValueRateTile3;
int editorValueRateTile4;

float leaveTilesHudTimer;

sfBool lightOrShadow;

void editor_Init(Window* _window)
{
	map_setOnePassMap(sfTrue);
	prt_InitParticles();
	npc_Init(_window);
	box_Init();
	initLights();
	en_InitEnemies();
	prt_InitParticles();
	brz_Init();
	pot_Init();
	qst_Init();
	plr_Init();
	en_InitEnemies();
	chest_InitChest();
	opDoors_InitDoor(_window);
	stree_initSpidersTrees();
	stt_Init(_window);
	note_InitNote();
	db_Init();
	initGrass();
	fishspot_Init();
	note_InitNote();
	prt_InitParticles();

	mvpl_Init();
	map_InitMap(_window);
	initEditorHUD(_window);

	editorPropsCursor = sfSprite_create();
	editorPreviewProps = sfSprite_create();

	rectTileCursor = sfRectangleShape_create();
	sfRectangleShape_setSize(rectTileCursor, vector2f(TILE_SIZE, TILE_SIZE));
	editorPreviewTile = sfRectangleShape_create();
	sfRectangleShape_setSize(editorPreviewTile, vector2f(TILE_SIZE, TILE_SIZE));

	editorCurrentTile = TILE_SAND_FULL;
	editorCurrentWall = 1;
	editorEntity = EDITOR_ENTITY_PLAYER;
	editorWallMaterial = WALL_MATERIAL_OUT;
	editorEffects = -1;
	editorCurrentCollision = CLD_BOX;

	indexMap = 1;
	groundMaterialValue = TILE_SAND_FULL;
	wallMaterialValue = 0;

	editorValueRateTile1 = 25;
	editorValueRateTile2 = 25;
	editorValueRateTile3 = 25;
	editorValueRateTile4 = 25;
	leaveTilesHudTimer = 0.f;

	editorPlayer = sfSprite_create();
	playerPosition = plr_GetPos();

	lightOrShadow = sfTrue;
}

void editor_Update(Window* _window)
{
	float dt = getDeltaTime();
	static float timer = 0.f;
	timer += dt;
	static float timerInput = 0.f;
	timerInput += dt;
	
	leaveTilesHudTimer += dt;

	updateEditorView(getEditorViewSpeed(), _window->renderWindow);
	updateEditorHUD(_window, dt);
	map_UpdateMap(_window);

	if (customKeyboardIsKeyPressed(sfKeyEscape, _window->renderWindow) && timerInput > 0.2f)
	{
		togglePause();
		timerInput = 0.f;
	}

	editorState = getEditorState();

	/// CHANGE MAPS
	editor_MapHandler(_window);

	if (editorState == EDITOR_STATE_TILES || editorState == EDITOR_STATE_TILES_HUD)
		/// CHANGE TILE
		editor_ChangeTile(_window);

	if (editorState == EDITOR_STATE_TILES)
	{
		/// RESIZE MAP
		editor_ResizeMap(_window);
		/// CHANGE LAYER
		editorHud_ChangeLayer(_window);
		/// CHANGE BRUSH
		editor_ChangeBrush(_window);

		sfVector2f mousePos = getfWorldMousePos(_window->renderWindow);
		if (mousePos.x > 0.f && mousePos.y > 0.f && mousePos.x < TILE_SIZE * map.mapSize.x && mousePos.y < TILE_SIZE * map.mapSize.y)
		{
			x = getfWorldMousePos(_window->renderWindow).x / TILE_SIZE;
			y = getfWorldMousePos(_window->renderWindow).y / TILE_SIZE;

			if (x + (brushSize - 1) >= map.mapSize.x)
				x = map.mapSize.x - brushSize;
			if (y + (brushSize - 1) >= map.mapSize.y)
				y = map.mapSize.y - brushSize;
			if (x - (brushSize - 2) <= 0.f)
				x = brushSize - 1;
			if (y - (brushSize - 2) <= 0.f)
				y = brushSize - 1;

			/// ERASE
			if (rightClick(_window->renderWindow) && getfMousePos(_window->renderWindow).x < EDITOR_HUD_XPOS)
			{
				editorCurrentTile = TILE_NO_TILE;
				editorCurrentWall = WALL_NO_TILE;
				editorCurrentCollision = CLD_NO_COLL;
			}

			/// PLACE
			if (leftClick(_window->renderWindow) && getfMousePos(_window->renderWindow).x < EDITOR_HUD_XPOS && leaveTilesHudTimer > 0.f)
			{
				editor_PlaceTile(x, y, brushSize);
				/*
				if (getEditorTilesLayer() == EDITOR_LAYER_WALLS)
					editor_UpdateMap(_window);
				*/
			}
			sfRectangleShape_setPosition(rectTileCursor, vector2f(x * TILE_SIZE, y * TILE_SIZE));

			/// PICK A TILE
			if (sfMouse_isButtonPressed(sfMouseMiddle) && getfMousePos(_window->renderWindow).x < EDITOR_HUD_XPOS && sfRenderWindow_hasFocus(_window->renderWindow))
			{
				switch (getEditorTilesLayer())
				{
				case EDITOR_LAYER_GROUND:
					if (editorHud_getFirstOrSecondLayer() == 1) editorCurrentTile = map.layerGround_first[y][x].type;
					else editorCurrentTile = map.layerGround_second[y][x].type;
					break;
				case EDITOR_LAYER_WALLS:
					if (editorHud_getFirstOrSecondLayer() == 1) editorCurrentWall = map.layerWalls_first[y][x].type;
					else editorCurrentWall = map.layerWalls_second[y][x].type;
					break;
				case EDITOR_LAYER_COLLISION: editorCurrentCollision = map.layerCollision[y][x].type; break;
				}

				if (getEditorTilesLayer() == EDITOR_LAYER_GROUND)
				{
					if (editorCurrentTile >= TILE_SAND_FULL && editorCurrentTile <= TILE_SAND_FRAME)
						sfRectangleShape_setTexture(rectTileCursor, GetSfTexture("ts_ground_sand"), sfTrue);
					else if (editorCurrentTile >= TILE_ROCK_FULL && editorCurrentTile <= TILE_ROCK_FRAME)
						sfRectangleShape_setTexture(rectTileCursor, GetSfTexture("ts_ground_rock"), sfTrue);
					else if (editorCurrentTile >= TILE_GRASS_FULL && editorCurrentTile <= TILE_GRASS_FRAME)
						sfRectangleShape_setTexture(rectTileCursor, GetSfTexture("ts_ground_grass"), sfTrue);
					else if (editorCurrentTile >= TILE_WATER_FULL && editorCurrentTile <= TILE_WATER_FULL_DEEP)
						sfRectangleShape_setTexture(rectTileCursor, GetSfTexture("ts_ground_water"), sfTrue);
					else if (editorCurrentTile >= TILE_GROUND_DIRT_1 && editorCurrentTile <= TILE_GROUND_DIRT_4)
						sfRectangleShape_setTexture(rectTileCursor, GetSfTexture("ts_ground_dirt"), sfTrue);
					else if (editorCurrentTile >= TILE_GROUND_DUNGEON_1 && editorCurrentTile <= TILE_GROUND_DUNGEON_4)
						sfRectangleShape_setTexture(rectTileCursor, GetSfTexture("ts_ground_dungeon"), sfTrue);
					else if (editorCurrentTile >= TILE_LEAF_FULL && editorCurrentTile <= TILE_LEAF_FRAME)
						sfRectangleShape_setTexture(rectTileCursor, GetSfTexture("ts_ground_leaf"), sfTrue);
				}
				else if (getEditorTilesLayer() == EDITOR_LAYER_WALLS)
				{
					wallMaterialValue = editorCurrentWall & 128;
					if (editorCurrentWall & WALL_MATERIAL_IN)
					{
						sfRectangleShape_setTexture(rectTileCursor, GetSfTexture("ts_wall_int"), sfTrue);
						setEditorWallType(EDITOR_WALL_INTERIOR);
					}
					else
					{
						sfRectangleShape_setTexture(rectTileCursor, GetSfTexture("ts_wall_ext"), sfTrue);
						setEditorWallType(EDITOR_WALL_EXTERIOR);
					}
				}
				else if (getEditorTilesLayer() == EDITOR_LAYER_COLLISION)
					sfRectangleShape_setTexture(rectTileCursor, GetSfTexture("collision"), sfTrue);
			}
		}
	}
	else if (editorState == EDITOR_STATE_ENTITY)
	{
		editorEntity = getEditorEntityState();
		if (editorEntity == EDITOR_ENTITY_PLAYER)
		{
			/// PLACE
			if (sfMouse_isButtonPressed(sfMouseLeft) && getfMousePos(_window->renderWindow).x < EDITOR_HUD_XPOS && sfRenderWindow_hasFocus(_window->renderWindow))
			{
				playerPosition = getfWorldMousePos(_window->renderWindow);
				plr_SetPos(playerPosition);
			}
		}
		else if (editorEntity == EDITOR_ENTITY_PNJ)
		{
			if (sfMouse_isButtonPressed(sfMouseLeft) && getfMousePos(_window->renderWindow).x < EDITOR_HUD_XPOS && timer > 0.2f && sfRenderWindow_hasFocus(_window->renderWindow))
			{
				timer = 0.f;
				npc_Create(getfWorldMousePos(_window->renderWindow), getEditorPnjChoice());
			}

			if (sfMouse_isButtonPressed(sfMouseRight) && getfMousePos(_window->renderWindow).x < EDITOR_HUD_XPOS && sfRenderWindow_hasFocus(_window->renderWindow))
			{
				for (int i = 0; i < npc_List->size(npc_List); i++)
				{
					sfFloatRect mousePos = { getfWorldMousePos(_window->renderWindow).x, getfWorldMousePos(_window->renderWindow).y, 1.f, 1.f };
					sfFloatRect pnj = { STD_LIST_GETDATA(npc_List, Npc, i)->position.x, STD_LIST_GETDATA(npc_List, Npc, i)->position.y, STD_LIST_GETDATA(npc_List, Npc, i)->interact.width, STD_LIST_GETDATA(npc_List, Npc, i)->interact.height };
					if (sfFloatRect_intersects(&mousePos, &pnj, NULL))
						npc_List->erase(&npc_List, i);
				}
			}
		}
		else if (editorEntity == EDITOR_ENTITY_MOBS)
		{
			if (sfMouse_isButtonPressed(sfMouseLeft) && getfMousePos(_window->renderWindow).x < EDITOR_HUD_XPOS && timer > 0.2f && sfRenderWindow_hasFocus(_window->renderWindow))
			{
				timer = 0.f;
				en_SpawnEnemy(getfWorldMousePos(_window->renderWindow), getEditorHudMobType(), getEditorHudMobRotation(), sfFalse);
			}
			if (sfMouse_isButtonPressed(sfMouseRight) && getfMousePos(_window->renderWindow).x < EDITOR_HUD_XPOS && sfRenderWindow_hasFocus(_window->renderWindow))
			{
				for (int i = 0; i < enemiesList->size(enemiesList); i++)
				{
					sfFloatRect mousePos = { getfWorldMousePos(_window->renderWindow).x, getfWorldMousePos(_window->renderWindow).y, 1.f, 1.f };
					sfFloatRect ennemis = { STD_LIST_GETDATA(enemiesList, EnemiesStruct, i)->pos.x - (STD_LIST_GETDATA(enemiesList, EnemiesStruct, i)->size.x / 2.f), STD_LIST_GETDATA(enemiesList, EnemiesStruct, i)->pos.y - (STD_LIST_GETDATA(enemiesList, EnemiesStruct, i)->size.y / 2.f), STD_LIST_GETDATA(enemiesList, EnemiesStruct, i)->size.x, STD_LIST_GETDATA(enemiesList, EnemiesStruct, i)->size.y };
					if (sfFloatRect_intersects(&mousePos, &ennemis, NULL))
						enemiesList->erase(&enemiesList, i);
				}
			}
		}
	}
	else if (editorState == EDITOR_STATE_PROPS)
	{
		if (sfMouse_isButtonPressed(sfMouseLeft) && getfMousePos(_window->renderWindow).x < EDITOR_HUD_XPOS && timer > 0.2f && sfRenderWindow_hasFocus(_window->renderWindow))
		{
			timer = 0.f;
			map_addProps(getCurrentPropsType(), getCurrentProps(), getfWorldMousePos(_window->renderWindow), sfFalse);
		}
		if (sfMouse_isButtonPressed(sfMouseRight) && getfMousePos(_window->renderWindow).x < EDITOR_HUD_XPOS && sfRenderWindow_hasFocus(_window->renderWindow))
			map_delProps(vector2i(getfWorldMousePos(_window->renderWindow).x, getfWorldMousePos(_window->renderWindow).y));

	}
	else if (editorState == EDITOR_STATE_EFFECTS)
	{
		editorEffects = getEditorEffectsState();
		if (editorEffects == EDITOR_EFFECTS_LIGHT)
		{
			if (sfMouse_isButtonPressed(sfMouseLeft) && getfMousePos(_window->renderWindow).x < EDITOR_HUD_XPOS && timer > 0.2f && sfRenderWindow_hasFocus(_window->renderWindow))
			{
				timer = 0.f;
				addLight(getfWorldMousePos(_window->renderWindow), getEditorLightColor(), getEditorLightSize(), getEditorLightForm(), getEditorConeAngle() * DEG2RAD, getEditorConeRotation() * DEG2RAD, getEditorlightStrength());
			}

			if (sfMouse_isButtonPressed(sfMouseRight) && getfMousePos(_window->renderWindow).x < EDITOR_HUD_XPOS && sfRenderWindow_hasFocus(_window->renderWindow))
			{
				for (int i = 0; i < lightList->size(lightList); i++)
				{
					sfFloatRect mousePos = { getfWorldMousePos(_window->renderWindow).x, getfWorldMousePos(_window->renderWindow).y, 1.f, 1.f };
					sfFloatRect box = { STD_LIST_GETDATA(lightList, Light, i)->pos.x, STD_LIST_GETDATA(lightList, Light, i)->pos.y, 50.f, 50.f };
					if (sfFloatRect_intersects(&mousePos, &box, NULL))
						lightList->erase(&lightList, i);
				}
			}
		}
	}
}

void editor_Display(Window* _window)
{
	map_DisplayMapBehindPlayer(_window);
	map_DisplayMapInFrontOfPlayer(_window);
	if (editorState == EDITOR_STATE_TILES)
	{
		if ((getEditorTilesLayer() == EDITOR_LAYER_GROUND && editorCurrentTile == TILE_NO_TILE) ||
			(getEditorTilesLayer() == EDITOR_LAYER_WALLS && editorCurrentWall == WALL_NO_TILE) ||
			(getEditorTilesLayer() == EDITOR_LAYER_COLLISION && editorCurrentCollision == CLD_NO_COLL))
		{
			sfRectangleShape_setFillColor(rectTileCursor, color(255, 0, 0, 150));
			sfRectangleShape_setTexture(rectTileCursor, NULL, sfTrue);

			sfRectangleShape_setFillColor(editorPreviewTile, color(255, 0, 0, 150));
			sfRectangleShape_setTexture(editorPreviewTile, NULL, sfTrue);
		}
		if (editorCurrentTile != TILE_NO_TILE || editorCurrentWall != WALL_NO_TILE || editorCurrentCollision != CLD_NO_COLL)
		{
			sfRectangleShape_setFillColor(rectTileCursor, color(255, 255, 255, 255));

			sfRectangleShape_setFillColor(editorPreviewTile, color(255, 255, 255, 255));
		}
		if (getEditorTilesLayer() != EDITOR_LAYER_COLLISION)
		{
			for (int i = 1 - brushSize; i < brushSize; i++)
			{
				for (int j = 1 - brushSize; j < brushSize; j++)
				{
					sfRectangleShape_setPosition(rectTileCursor, vector2f((x + i) * TILE_SIZE, (y + j) * TILE_SIZE));
					sfRenderTexture_drawRectangleShape(_window->rdrt, rectTileCursor, NULL);
				}
			}
		}
		else
		{
			sfRectangleShape_setTextureRect(rectTileCursor, IntRect((editorCurrentCollision - 1) * TILE_SIZE, 0.f, editorCurrentCollision ? TILE_SIZE : 0.f, editorCurrentCollision ? TILE_SIZE : 0.f));
			for (int i = 1 - brushSize; i < brushSize; i++)
			{
				for (int j = 1 - brushSize; j < brushSize; j++)
				{
					sfRectangleShape_setPosition(rectTileCursor, vector2f((x + i) * TILE_SIZE, (y + j) * TILE_SIZE));
					sfRenderTexture_drawRectangleShape(_window->rdrt, rectTileCursor, NULL);
				}
			}
			sfRectangleShape_setTextureRect(editorPreviewTile, IntRect((editorCurrentCollision - 1) * TILE_SIZE, 0.f, editorCurrentCollision ? TILE_SIZE : 0.f, editorCurrentCollision ? TILE_SIZE : 0.f));
		}
	}
	else if (editorState == EDITOR_STATE_PROPS)
	{
		///box_Display(_window);
		sfTexture* tex = NULL;
		/// Load texture tiles
		PropsType propType = getCurrentPropsType();
		if (propType == PROPS_TYPE_FOREST)
		{
			int currentProps = getCurrentProps();
			if (currentProps == PROPS_FOREST_SPLASH)
				tex = GetSfTexture("assets_splash");
			else if (currentProps == PROPS_FOREST_WATERFALL)
				tex = GetSfTexture("assets_waterfall");
			else if (currentProps == PROPS_FOREST_GRASS_1 || currentProps == PROPS_FOREST_GRASS_2)
				tex = GetSfTexture("grass1");
			else
				tex = GetSfTexture("assets_forest");
		}
		else if (propType == PROPS_TYPE_MOUTAIN) tex = GetSfTexture("assets_moutain");
		else if (propType == PROPS_TYPE_DUNGEON)
		{
			int currentProps = getCurrentProps();
			if (currentProps >= PROPS_DUNGEON_DOOR_TOP && currentProps <= PROPS_DUNGEON_PAVING_STONE)	tex = GetSfTexture("assets_dungeon");
			else if (currentProps >= PROPS_DUNGEON_PILLAR_TOP_1 && currentProps <= PROPS_DUNGEON_BIG_CHEST) tex = GetSfTexture("assets_dungeon_biome");
			else if (currentProps >= PROPS_DUNGEON_BUILD_JOY && currentProps <= PROPS_DUNGEON_ENTRANCE_PRESSURE_PLATE_JOY) tex = GetSfTexture("assets_dj_joy");
			else if (currentProps >= PROPS_DUNGEON_BUILD_FRONT_SADNESS && currentProps <= PROPS_DUNGEON_ENTRANCE_PRESSURE_PLATE_SADNESS) tex = GetSfTexture("assets_dj_sadness");
			else if (currentProps >= PROPS_DUNGEON_BUILD_FRONT_FEAR && currentProps <= PROPS_DUNGEON_ENTRANCE_PRESSURE_PLATE_FEAR) tex = GetSfTexture("assets_dj_fear");
			else if (currentProps >= PROPS_DUNGEON_BUILD_FRONT_ANGRY && currentProps <= PROPS_DUNGEON_ENTRANCE_PRESSURE_PLATE_ANGRY) tex = GetSfTexture("assets_dj_angry");
			else if (currentProps >= PROPS_DUNGEON_STATUE_BASE_HEAD && currentProps <= PROPS_DUNGEON_STATUE_FULL) tex = GetSfTexture("assets_statue_golem");
		}
		else if (propType == PROPS_TYPE_BEACH) tex = GetSfTexture("assets_beach");
		else if (propType == PROPS_TYPE_GENERAL) tex = GetSfTexture("assets_general");
		sfSprite_setTexture(editorPropsCursor, tex, sfTrue);
		sfSprite_setTexture(editorPreviewProps, tex, sfTrue);

		if (propType == PROPS_TYPE_FOREST || propType == PROPS_TYPE_BEACH)
		{
			switch (propType)
			{
			case PROPS_TYPE_FOREST:
				switch (getCurrentProps())
				{
				case PROPS_FOREST_TREE_1:
					sfSprite_setTextureRect(editorPropsCursor, map_GetPropsTexRect((LayerProps) { propType, PROPS_FOREST_TRUNK_1, 0, 0.f }));
					sfSprite_setPosition(editorPropsCursor, addVectorsrf(getfWorldMousePos(_window->renderWindow), vector2f(93.f, 252.f)));
					sfSprite_setTextureRect(editorPreviewProps, map_GetPropsTexRect((LayerProps) { propType, PROPS_FOREST_TRUNK_1, 0, 0.f }));
					break;
				case PROPS_FOREST_TREE_2:
					sfSprite_setTextureRect(editorPropsCursor, map_GetPropsTexRect((LayerProps) { propType, PROPS_FOREST_TRUNK_1, 0, 0.f }));
					sfSprite_setPosition(editorPropsCursor, addVectorsrf(getfWorldMousePos(_window->renderWindow), vector2f(93.f, 252.f)));
					sfSprite_setTextureRect(editorPreviewProps, map_GetPropsTexRect((LayerProps) { propType, PROPS_FOREST_TRUNK_1, 0, 0.f }));
					break;
				case PROPS_FOREST_TREE_3:
					sfSprite_setTextureRect(editorPropsCursor, map_GetPropsTexRect((LayerProps) { propType, PROPS_FOREST_TRUNK_2, 0, 0.f }));
					sfSprite_setPosition(editorPropsCursor, addVectorsrf(getfWorldMousePos(_window->renderWindow), vector2f(52.f, 232.f)));
					sfSprite_setTextureRect(editorPreviewProps, map_GetPropsTexRect((LayerProps) { propType, PROPS_FOREST_TRUNK_2, 0, 0.f }));
					break;
				case PROPS_FOREST_TREE_4:
					sfSprite_setTextureRect(editorPropsCursor, map_GetPropsTexRect((LayerProps) { propType, PROPS_FOREST_TRUNK_2, 0, 0.f }));
					sfSprite_setPosition(editorPropsCursor, addVectorsrf(getfWorldMousePos(_window->renderWindow), vector2f(52.f, 232.f)));
					sfSprite_setTextureRect(editorPreviewProps, map_GetPropsTexRect((LayerProps) { propType, PROPS_FOREST_TRUNK_2, 0, 0.f }));
					break;
				case PROPS_FOREST_TREE_5:
					sfSprite_setTextureRect(editorPropsCursor, map_GetPropsTexRect((LayerProps) { propType, PROPS_FOREST_TRUNK_3, 0, 0.f }));
					sfSprite_setPosition(editorPropsCursor, addVectorsrf(getfWorldMousePos(_window->renderWindow), vector2f(158.f, 354.f)));
					sfSprite_setTextureRect(editorPreviewProps, map_GetPropsTexRect((LayerProps) { propType, PROPS_FOREST_TRUNK_3, 0, 0.f }));
					break;
				case PROPS_FOREST_TREE_6:
					sfSprite_setTextureRect(editorPropsCursor, map_GetPropsTexRect((LayerProps) { propType, PROPS_FOREST_TRUNK_3, 0, 0.f }));
					sfSprite_setPosition(editorPropsCursor, addVectorsrf(getfWorldMousePos(_window->renderWindow), vector2f(158.f, 354.f)));
					sfSprite_setTextureRect(editorPreviewProps, map_GetPropsTexRect((LayerProps) { propType, PROPS_FOREST_TRUNK_3, 0, 0.f }));
					break;
				case PROPS_FOREST_TREE_SPIDER:
					sfSprite_setTextureRect(editorPropsCursor, map_GetPropsTexRect((LayerProps) { propType, PROPS_FOREST_TRUNK_3, 0, 0.f }));
					sfSprite_setPosition(editorPropsCursor, addVectorsrf(getfWorldMousePos(_window->renderWindow), vector2f(160.f, 354.f)));
					sfSprite_setTextureRect(editorPreviewProps, map_GetPropsTexRect((LayerProps) { propType, PROPS_FOREST_TRUNK_3, 0, 0.f }));
					break;
				case PROPS_FOREST_TREE_ANIM:
					sfSprite_setTextureRect(editorPropsCursor, map_GetPropsTexRect((LayerProps) { propType, PROPS_FOREST_TRUNK_2, 0, 0.f }));
					sfSprite_setPosition(editorPropsCursor, addVectorsrf(getfWorldMousePos(_window->renderWindow), vector2f(52.f, 232.f)));
					sfSprite_setTextureRect(editorPreviewProps, map_GetPropsTexRect((LayerProps) { propType, PROPS_FOREST_TRUNK_2, 0, 0.f }));
					break;
				default:
					sfSprite_setTextureRect(editorPropsCursor, map_GetPropsTexRect((LayerProps) { propType, getCurrentProps(), 0, 0.f }));
					sfSprite_setPosition(editorPropsCursor, getfWorldMousePos(_window->renderWindow));
					sfSprite_setTextureRect(editorPreviewProps, map_GetPropsTexRect((LayerProps) { propType, getCurrentProps(), 0, 0.f }));
					break;
				}
				break;
			case PROPS_TYPE_BEACH:
				switch (getCurrentProps())
				{
				case PROPS_BEACH_PALM_1:
					sfSprite_setTextureRect(editorPropsCursor, map_GetPropsTexRect((LayerProps) { propType, PROPS_BEACH_TRUNK_1, 0, 0.f }));
					sfSprite_setPosition(editorPropsCursor, addVectorsrf(getfWorldMousePos(_window->renderWindow), vector2f(118.f, 234.f)));
					sfSprite_setTextureRect(editorPreviewProps, map_GetPropsTexRect((LayerProps) { propType, PROPS_BEACH_TRUNK_1, 0, 0.f }));
					break;
				case PROPS_BEACH_PALM_2:
					sfSprite_setTextureRect(editorPropsCursor, map_GetPropsTexRect((LayerProps) { propType, PROPS_BEACH_TRUNK_2, 0, 0.f }));
					sfSprite_setPosition(editorPropsCursor, addVectorsrf(getfWorldMousePos(_window->renderWindow), vector2f(156.f, 220.f)));
					sfSprite_setTextureRect(editorPreviewProps, map_GetPropsTexRect((LayerProps) { propType, PROPS_BEACH_TRUNK_2, 0, 0.f }));
					break;
				default:
					sfSprite_setTextureRect(editorPropsCursor, map_GetPropsTexRect((LayerProps) { propType, getCurrentProps(), 0, 0.f }));
					sfSprite_setPosition(editorPropsCursor, getfWorldMousePos(_window->renderWindow));
					sfSprite_setTextureRect(editorPreviewProps, map_GetPropsTexRect((LayerProps) { propType, getCurrentProps(), 0, 0.f }));
					break;
				}
				break;
			}
			sfRenderTexture_drawSprite(_window->rdrt, editorPropsCursor, NULL);
		}
		sfSprite_setTextureRect(editorPropsCursor, map_GetPropsTexRect((LayerProps) { propType, getCurrentProps(), 0, 0.f }));
		sfSprite_setPosition(editorPropsCursor, getfWorldMousePos(_window->renderWindow));
		sfRenderTexture_drawSprite(_window->rdrt, editorPropsCursor, NULL);
		sfSprite_setTextureRect(editorPreviewProps, map_GetPropsTexRect((LayerProps) { propType, getCurrentProps(), 0, 0.f }));
	}
	else if (editorState == EDITOR_STATE_EFFECTS)
		displayLights(_window);

	en_DisplayEnemies(_window);
	pot_Display(_window);
	npc_Display(_window);
	plr_Display(_window, editorPlayer);
	displayEditorHUD(_window);

	if (editorState == EDITOR_STATE_TILES)
	{
		sfRenderTexture_setView(_window->rdrt, sfRenderTexture_getDefaultView(_window->rdrt));
		sfRectangleShape_setPosition(editorPreviewTile, vector2f(1745.f, 380.f));
		sfRectangleShape_setOutlineThickness(editorPreviewTile, 3.f);
		sfRectangleShape_setOutlineColor(editorPreviewTile, sfColor_fromRGBA(0, 0, 255, 255));
		sfRectangleShape_setScale(editorPreviewTile, vector2f(1.5f, 1.5f));
		sfRenderTexture_drawRectangleShape(_window->rdrt, editorPreviewTile, NULL);
		sfRenderTexture_setView(_window->rdrt, mainView->view);
	}
	else if (editorState == EDITOR_STATE_TILES_HUD && getEditorTilesLayer() == EDITOR_LAYER_GROUND && (getEditorGroundType() == EDITOR_GROUND_SAND || getEditorGroundType() == EDITOR_GROUND_ROCK 
		|| getEditorGroundType() == EDITOR_GROUND_GRASS || getEditorGroundType() == EDITOR_GROUND_LEAF))
	{
		sfRenderTexture_setView(_window->rdrt, sfRenderTexture_getDefaultView(_window->rdrt));
		if (getEditorGroundType() == EDITOR_GROUND_SAND)
			sfRectangleShape_setTexture(editorPreviewTile, GetSfTexture("ts_ground_sand"), sfTrue);
		else if (getEditorGroundType() == EDITOR_GROUND_ROCK)
			sfRectangleShape_setTexture(editorPreviewTile, GetSfTexture("ts_ground_rock"), sfTrue);
		else if (getEditorGroundType() == EDITOR_GROUND_GRASS)
			sfRectangleShape_setTexture(editorPreviewTile, GetSfTexture("ts_ground_grass"), sfTrue);
		else if (getEditorGroundType() == EDITOR_GROUND_WATER)
			sfRectangleShape_setTexture(editorPreviewTile, GetSfTexture("ts_ground_water"), sfTrue);
		else if (getEditorGroundType() == EDITOR_GROUND_DIRT)
			sfRectangleShape_setTexture(editorPreviewTile, GetSfTexture("ts_ground_dirt"), sfTrue);
		else if (getEditorGroundType() == EDITOR_GROUND_DUNGEON)
			sfRectangleShape_setTexture(editorPreviewTile, GetSfTexture("ts_ground_dungeon"), sfTrue);
		else if (getEditorGroundType() == EDITOR_GROUND_LEAF) 
			sfRectangleShape_setTexture(editorPreviewTile, GetSfTexture("ts_ground_leaf"), sfTrue);
		for (int i = 0; i < 4; i++)
		{
			switch (i)
			{
			case 0:
				sfRectangleShape_setTextureRect(editorPreviewTile, map_GetTilesetRect(1, 1));
				break; 
			case 1:
				sfRectangleShape_setTextureRect(editorPreviewTile, map_GetTilesetRect(4, 1));
				break;
			case 2:
				sfRectangleShape_setTextureRect(editorPreviewTile, map_GetTilesetRect(5, 3));
				break;
			case 3:
				sfRectangleShape_setTextureRect(editorPreviewTile, map_GetTilesetRect(5, 4));
				break;
			}
			sfRectangleShape_setPosition(editorPreviewTile, vector2f(1840.f, 280.f + (i * 50.f)));
			sfRectangleShape_setOutlineThickness(editorPreviewTile, 0.f);
			sfRectangleShape_setScale(editorPreviewTile, vector2f(0.6f, 0.6f));
			sfRenderTexture_drawRectangleShape(_window->rdrt, editorPreviewTile, NULL);
		}
		sfRenderTexture_setView(_window->rdrt, mainView->view);
	}
	else if (editorState == EDITOR_STATE_PROPS)
	{
		sfRenderTexture_setView(_window->rdrt, sfRenderTexture_getDefaultView(_window->rdrt));
		sfSprite_setPosition(editorPreviewProps, vector2f(1710.f, 250.f));
		if (getCurrentPropsType() == PROPS_TYPE_GENERAL && getCurrentProps() == PROPS_GENERAL_TENT_ARNOLD)
			sfSprite_setScale(editorPreviewProps, vector2f(0.15f, 0.15f));
		else if (getCurrentPropsType() == PROPS_TYPE_GENERAL && getCurrentProps() == PROPS_GENERAL_FLOWER)
			sfSprite_setScale(editorPreviewProps, vector2f(2.f, 2.f));
		else if (getCurrentPropsType() == PROPS_TYPE_GENERAL && getCurrentProps() == PROPS_GENERAL_POOL)
			sfSprite_setScale(editorPreviewProps, vector2f(0.2f, 0.2f));
		else if (getCurrentPropsType() == PROPS_TYPE_GENERAL && getCurrentProps() == PROPS_GENERAL_TENT_SHAMAN)
			sfSprite_setScale(editorPreviewProps, vector2f(0.2f, 0.2f));
		else if (getCurrentPropsType() == PROPS_TYPE_GENERAL && getCurrentProps() == PROPS_GENERAL_BUBBLES)
			sfSprite_setScale(editorPreviewProps, vector2f(1.2f, 1.2f));
		else if (getCurrentPropsType() == PROPS_TYPE_DUNGEON && (getCurrentProps() == PROPS_DUNGEON_BUILD_JOY || getCurrentProps() == PROPS_DUNGEON_BUILD_FRONT_SADNESS 
			|| getCurrentProps() == PROPS_DUNGEON_BUILD_BACK_SADNESS || getCurrentProps() == PROPS_DUNGEON_BUILD_FRONT_FEAR || getCurrentProps() == PROPS_DUNGEON_BUILD_BACK_FEAR
			|| getCurrentProps() == PROPS_DUNGEON_BUILD_FRONT_ANGRY || getCurrentProps() == PROPS_DUNGEON_BUILD_BACK_ANGRY))
			sfSprite_setScale(editorPreviewProps, vector2f(0.2f, 0.2f));
		else 
			sfSprite_setScale(editorPreviewProps, vector2f(0.6f, 0.6f)); 
		sfRenderTexture_drawSprite(_window->rdrt, editorPreviewProps, NULL);
		sfRenderTexture_setView(_window->rdrt, mainView->view);
	}
}

void editor_Deinit()
{
	map_DeinitMap();
	sfRectangleShape_destroy(rectTileCursor);
	RemoveAllTextureButALL();
	RemoveAllSoundsNotNeededInEveryStates();
}

void editor_UpdateMap()
{
	for (int i = 0; i < map.mapSize.y; i++)
	{
		for (int j = 0; j < map.mapSize.x; j++)
		{
			/// Applying a binary mask to keep only the first 7 bits of the value, which determine tile geometry.
			/// Bit 7 handles the tile's material (rock or brick), which doesn't matter for collision computation
			WallsType wall = map.layerWalls_first[i][j].type & 0b01111111; /// = 127
			switch (wall)
			{
			case WALL_TOP_DIAG_TL:				map.layerCollision[i][j].type = CLD_SMALL_DIAG_TOP_RIGHT;	break;
			case WALL_TOP_EDGE_T:				map.layerCollision[i][j].type = CLD_HALF_TOP;				break;
			case WALL_TOP_DIAG_TR:				map.layerCollision[i][j].type = CLD_SMALL_DIAG_TOP_LEFT;	break;
			case WALL_TOP_EDGE_L:				map.layerCollision[i][j].type = CLD_HALF_LEFT;				break;
			case WALL_TOP_EDGE_T_FILLED:		map.layerCollision[i][j].type = CLD_HALF_TOP;				break;
			case WALL_TOP_EDGE_R:				map.layerCollision[i][j].type = CLD_HALF_RIGHT;				break;
			case WALL_TOP_DIAG_BL:				map.layerCollision[i][j].type = CLD_DIAG_BOTTOM_LEFT;		break;
			case WALL_TOP_DIAG_BL_WATER:		map.layerCollision[i][j].type = CLD_DIAG_BOTTOM_LEFT;		break;
			case WALL_TOP_DIAG_BL_VOID:			map.layerCollision[i][j].type = CLD_DIAG_BOTTOM_LEFT;		break;
			case WALL_TOP_EDGE_B:				map.layerCollision[i][j].type = CLD_HALF_BOTTOM;			break;
			case WALL_TOP_DIAG_BR:				map.layerCollision[i][j].type = CLD_DIAG_BOTTOM_RIGHT;		break;
			case WALL_TOP_DIAG_BR_WATER:		map.layerCollision[i][j].type = CLD_DIAG_BOTTOM_RIGHT;		break;
			case WALL_TOP_DIAG_BR_VOID:			map.layerCollision[i][j].type = CLD_DIAG_BOTTOM_RIGHT;		break;
			case WALL_TOP_EDGE_L_FILLED:		map.layerCollision[i][j].type = CLD_HALF_LEFT;				break;
			case WALL_TOP_EDGE_R_FILLED:		map.layerCollision[i][j].type = CLD_HALF_RIGHT;				break;
			case WALL_TOP_DIAG_BL_FILLED:		map.layerCollision[i][j].type = CLD_DIAG_BOTTOM_LEFT;		break;
			case WALL_TOP_DIAG_BR_FILLED:		map.layerCollision[i][j].type = CLD_DIAG_BOTTOM_RIGHT;		break;
			case WALL_TOP_CORNER_TL:			map.layerCollision[i][j].type = CLD_ANGLE_TOP_LEFT;			break;
			case WALL_TOP_CORNER_TR:			map.layerCollision[i][j].type = CLD_ANGLE_TOP_RIGHT;		break;
			case WALL_TOP_CORNER_BL:			map.layerCollision[i][j].type = CLD_ANGLE_BOTTOM_LEFT;		break;
			case WALL_TOP_CORNER_BR:			map.layerCollision[i][j].type = CLD_ANGLE_BOTTOM_RIGHT;		break;
			case WALL_TOP_CORNER_TL_FILLED:		map.layerCollision[i][j].type = CLD_ANGLE_TOP_LEFT;			break;
			case WALL_TOP_CORNER_TR_FILLED:		map.layerCollision[i][j].type = CLD_ANGLE_TOP_RIGHT;		break;
			case WALL_TOP_CORNER_BL_FILLED:		map.layerCollision[i][j].type = CLD_ANGLE_BOTTOM_LEFT;		break;
			case WALL_TOP_CORNER_BR_FILLED:		map.layerCollision[i][j].type = CLD_ANGLE_BOTTOM_RIGHT;		break;
			case WALL_TOP_DIAG_TL_DARK:			map.layerCollision[i][j].type = CLD_DIAG_BOTTOM_RIGHT;		break;
			case WALL_TOP_DIAG_TR_DARK:			map.layerCollision[i][j].type = CLD_DIAG_BOTTOM_LEFT;		break;
			case WALL_TOP_DIAG_BL_DARK:			map.layerCollision[i][j].type = CLD_DIAG_BOTTOM_LEFT;		break;
			case WALL_TOP_DIAG_BR_DARK:			map.layerCollision[i][j].type = CLD_DIAG_BOTTOM_RIGHT;		break;
			case WALL_TOP_EDGE_T_DARK:			map.layerCollision[i][j].type = CLD_HALF_TOP;				break;
			case WALL_TOP_EDGE_B_DARK:			map.layerCollision[i][j].type = CLD_HALF_BOTTOM;			break;
			case WALL_TOP_EDGE_L_DARK:			map.layerCollision[i][j].type = CLD_HALF_LEFT;				break;
			case WALL_TOP_EDGE_R_DARK:			map.layerCollision[i][j].type = CLD_HALF_RIGHT;				break;
			case WALL_TOP_CORNER_TL_DARK:		map.layerCollision[i][j].type = CLD_ANGLE_TOP_LEFT;			break;
			case WALL_TOP_CORNER_TR_DARK:		map.layerCollision[i][j].type = CLD_ANGLE_TOP_RIGHT;		break;
			case WALL_TOP_CORNER_BL_DARK:		map.layerCollision[i][j].type = CLD_ANGLE_BOTTOM_LEFT;		break;
			case WALL_TOP_CORNER_BR_DARK:		map.layerCollision[i][j].type = CLD_ANGLE_BOTTOM_RIGHT;		break;
			case WALL_TOP_INNER_TL:				map.layerCollision[i][j].type = CLD_CORN_TOP_LEFT;			break;
			case WALL_TOP_INNER_TR:				map.layerCollision[i][j].type = CLD_CORN_TOP_RIGHT;			break;
			case WALL_TOP_INNER_BL:				map.layerCollision[i][j].type = CLD_CORN_BOTTOM_LEFT;		break;
			case WALL_TOP_INNER_BR:				map.layerCollision[i][j].type = CLD_CORN_BOTTOM_RIGHT;		break;
			case WALL_MID_DIAG_BL:				map.layerCollision[i][j].type = CLD_BOX;					break;
			case WALL_MID_FULL:					map.layerCollision[i][j].type = CLD_BOX;					break;
			case WALL_MID_DIAG_BR:				map.layerCollision[i][j].type = CLD_BOX;					break;
			case WALL_MID_DIAG_BL_TRANSP:		map.layerCollision[i][j].type = CLD_BOX;					break;
			case WALL_MID_DIAG_BL_WATER:		map.layerCollision[i][j].type = CLD_BOX;					break;
			case WALL_MID_DIAG_BR_TRANSP:		map.layerCollision[i][j].type = CLD_BOX;					break;
			case WALL_MID_DIAG_BR_WATER:		map.layerCollision[i][j].type = CLD_BOX;					break;
			case WALL_MID_FULL_LEFT:			map.layerCollision[i][j].type = CLD_BOX;					break;
			case WALL_MID_FULL_RIGHT:			map.layerCollision[i][j].type = CLD_BOX;					break;
			case WALL_BASE_DIAG_BL_FULL:		map.layerCollision[i][j].type = CLD_DIAG_TOP_RIGHT;			break;
			case WALL_BASE_DIAG_BL_HALF_WATER:	map.layerCollision[i][j].type = CLD_SMALL_TRI_TR;			break;
			case WALL_BASE_DIAG_BL_FULL_WATER:	map.layerCollision[i][j].type = CLD_SMALL_TRI_TR;			break;
			case WALL_BASE_FULL_LEFT:			map.layerCollision[i][j].type = CLD_BOX;					break;
			case WALL_BASE_FULL:				map.layerCollision[i][j].type = CLD_BOX;					break;
			case WALL_BASE_FULL_RIGHT:			map.layerCollision[i][j].type = CLD_BOX;					break;
			case WALL_BASE_HALF_WATER:			map.layerCollision[i][j].type = CLD_HALF_TOP;				break;
			case WALL_BASE_DIAG_BR_FULL:		map.layerCollision[i][j].type = CLD_DIAG_TOP_LEFT;			break;
			case WALL_BASE_DIAG_BR_HALF_WATER:	map.layerCollision[i][j].type = CLD_SMALL_TRI_TL;			break;
			case WALL_BASE_DIAG_BR_FULL_WATER:	map.layerCollision[i][j].type = CLD_SMALL_TRI_TL;			break;
			case WALL_VOID_DIAG_BL:				map.layerCollision[i][j].type = CLD_BOX;					break;
			case WALL_VOID:						map.layerCollision[i][j].type = CLD_BOX;					break;
			case WALL_VOID_DIAG_BR:				map.layerCollision[i][j].type = CLD_BOX;					break;
			case WALL_VOID_GRAD_DIAG_BL:		map.layerCollision[i][j].type = CLD_BOX;					break;
			case WALL_VOID_GRAD_DIAG_BR:		map.layerCollision[i][j].type = CLD_BOX;					break;
			case WALL_VOID_GRAD_DIAG_BL_EDGE:	map.layerCollision[i][j].type = CLD_DIAG_BOTTOM_LEFT;		break;
			case WALL_VOID_GRAD_DIAG_BR_EDGE:	map.layerCollision[i][j].type = CLD_DIAG_BOTTOM_RIGHT;		break;
			case WALL_BASE_VOID_CORNER_BL:		map.layerCollision[i][j].type = CLD_SMALL_TRI_BL;			break;
			case WALL_BASE_VOID_CORNER_BR:		map.layerCollision[i][j].type = CLD_SMALL_TRI_BR;			break;
			case WALL_FULL_STAIR:				map.layerCollision[i][j].type = CLD_NO_COLL;				break;
			case WALL_WATER_STAIR:				map.layerCollision[i][j].type = CLD_NO_COLL;				break;
			case WALL_TOP_STAIR:				map.layerCollision[i][j].type = CLD_NO_COLL;				break;
			case WALL_NO_TILE:					map.layerCollision[i][j].type = CLD_NO_COLL;				break;
			}
		}
	}
}

void editor_ResizeMap(Window* _window)
{
	static float timer = 0.f;
	timer += getDeltaTime();

	if (customKeyboardIsKeyPressed(sfKeyLeft, _window->renderWindow) && timer > 0.05f)
	{
		timer = 0.f;
		editor_RemoveColumn();
	}
	else if (customKeyboardIsKeyPressed(sfKeyRight, _window->renderWindow) && timer > 0.05f)
	{
		timer = 0.f;
		editor_AddColumn();
	}
	if (customKeyboardIsKeyPressed(sfKeyUp, _window->renderWindow) && timer > 0.05f)
	{
		timer = 0.f;
		editor_RemoveRow();
	}
	else if (customKeyboardIsKeyPressed(sfKeyDown, _window->renderWindow) && timer > 0.05f)
	{
		timer = 0.f;
		editor_AddRow();
	}
}

void editor_MapHandler(Window* _window)
{
	static float timer = 0.f;
	timer += getDeltaTime();

	int lastIndexMap = indexMap;
	if (customKeyboardIsKeyPressed(sfKeyB, _window->renderWindow) && timer > 0.2f)
	{
		if (indexMap > 1)
			indexMap--;
		else indexMap = 1;

		timer = 0.f;
	}
	else if (customKeyboardIsKeyPressed(sfKeyN, _window->renderWindow) && timer > 0.2f)
		indexMap++;

	if (map_GetState() != indexMap) map_changeMapState(_window, indexMap);

	if ((customKeyboardIsKeyPressed(sfKeyLShift, _window->renderWindow)) || getWantMapSave())
	{
		map_SaveMap(playerPosition);
		setMapText("Map saved !");
		setMapTextSave("Map saved !");
		setWantMapSave(sfFalse);
		timer = -0.8f;
	}

	if (timer >= 0.f && timer < 1.f)
	{
		char tmp[30] = "";
		sprintf(tmp, "Map : %d (LShift to save)", indexMap);
		setMapText(tmp);
		sprintf(tmp, "Map %d : ", indexMap);
		setMapTextID(tmp);
		setMapTextSave("Press to Save");
	}
}

void editor_ChangeTile(Window* _window)
{
	static float timer = 0.f;
	timer += getDeltaTime();
	EditorTilesLayer tilesLayer = getEditorTilesLayer();
	if (editorState == EDITOR_STATE_TILES)
	{
		if (tilesLayer == EDITOR_LAYER_GROUND)
		{
			if (editorCurrentTile >= TILE_SAND_FULL && editorCurrentTile <= TILE_SAND_FRAME)
			{
				sfRectangleShape_setTexture(rectTileCursor, GetSfTexture("ts_ground_sand"), sfTrue);
				sfRectangleShape_setTexture(editorPreviewTile, GetSfTexture("ts_ground_sand"), sfTrue);
			}
			else if (editorCurrentTile >= TILE_ROCK_FULL && editorCurrentTile <= TILE_ROCK_FRAME)
			{
				sfRectangleShape_setTexture(rectTileCursor, GetSfTexture("ts_ground_rock"), sfTrue);
				sfRectangleShape_setTexture(editorPreviewTile, GetSfTexture("ts_ground_rock"), sfTrue);
			}
			else if (editorCurrentTile >= TILE_GRASS_FULL && editorCurrentTile <= TILE_GRASS_FRAME)
			{
				sfRectangleShape_setTexture(rectTileCursor, GetSfTexture("ts_ground_grass"), sfTrue);
				sfRectangleShape_setTexture(editorPreviewTile, GetSfTexture("ts_ground_grass"), sfTrue);
			}
			else if (editorCurrentTile >= TILE_WATER_FULL && editorCurrentTile <= TILE_WATER_FULL_DEEP)
			{
				sfRectangleShape_setTexture(rectTileCursor, GetSfTexture("ts_ground_water"), sfTrue);
				sfRectangleShape_setTexture(editorPreviewTile, GetSfTexture("ts_ground_water"), sfTrue);
			}
			else if (editorCurrentTile >= TILE_LEAF_FULL && editorCurrentTile <= TILE_LEAF_FRAME)
			{
				sfRectangleShape_setTexture(rectTileCursor, GetSfTexture("ts_ground_leaf"), sfTrue);
				sfRectangleShape_setTexture(editorPreviewTile, GetSfTexture("ts_ground_leaf"), sfTrue);
			}
			else if (editorCurrentTile >= TILE_GROUND_DIRT_1 && editorCurrentTile <= TILE_GROUND_DIRT_4)
			{
				sfRectangleShape_setTexture(rectTileCursor, GetSfTexture("ts_ground_dirt"), sfTrue);
				sfRectangleShape_setTexture(editorPreviewTile, GetSfTexture("ts_ground_dirt"), sfTrue);
			}
			else if (editorCurrentTile >= TILE_GROUND_DUNGEON_1 && editorCurrentTile <= TILE_GROUND_DUNGEON_4)
			{
				sfRectangleShape_setTexture(rectTileCursor, GetSfTexture("ts_ground_dungeon"), sfTrue);
				sfRectangleShape_setTexture(editorPreviewTile, GetSfTexture("ts_ground_dungeon"), sfTrue);
			}

			/// SET TILE CURSOR RECT
			sfRectangleShape_setTextureRect(rectTileCursor, map_GetTileTexRect((LayerGround) { editorCurrentTile }));
			sfRectangleShape_setTextureRect(editorPreviewTile, map_GetTileTexRect((LayerGround) { editorCurrentTile }));
		}
		else if (tilesLayer == EDITOR_LAYER_WALLS)
		{
			if (wallMaterialValue == WALL_MATERIAL_OUT)
			{
				sfRectangleShape_setTexture(rectTileCursor, GetSfTexture("ts_wall_ext"), sfTrue);
				sfRectangleShape_setTexture(editorPreviewTile, GetSfTexture("ts_wall_ext"), sfTrue);
			}
			else
			{
				sfRectangleShape_setTexture(rectTileCursor, GetSfTexture("ts_wall_int"), sfTrue);
				sfRectangleShape_setTexture(editorPreviewTile, GetSfTexture("ts_wall_int"), sfTrue);
			}

			/// SET TILE CURSOR RECT
			sfRectangleShape_setTextureRect(rectTileCursor, map_GetWallTexRect((LayerWalls) { editorCurrentWall }));
			sfRectangleShape_setTextureRect(editorPreviewTile, map_GetWallTexRect((LayerWalls) { editorCurrentWall }));
		}
		else
		{
			sfRectangleShape_setTexture(rectTileCursor, GetSfTexture("collision"), sfTrue);
			sfRectangleShape_setTexture(editorPreviewTile, GetSfTexture("collision"), sfTrue);
			if (customKeyboardIsKeyPressed(sfKeyNumpad1, _window->renderWindow) && timer > 0.1f)
			{
				timer = 0.f;
				editorCurrentCollision--;
				if (editorCurrentCollision < 1)
					editorCurrentCollision = NB_MAX_COLL_TYPE - 1;
			}
			else if (customKeyboardIsKeyPressed(sfKeyNumpad2, _window->renderWindow) && timer > 0.1f)
			{
				timer = 0.f;
				editorCurrentCollision++;
				if (editorCurrentCollision > NB_MAX_COLL_TYPE - 1)
					editorCurrentCollision = 1;
			}
		}
	}
	else
	{
		if (leftClick(_window->renderWindow))
		{
			timer = 0.f;
			float mousePosX = getfMousePos(_window->renderWindow).x;
			float mousePosY = getfMousePos(_window->renderWindow).y;
			if (tilesLayer == EDITOR_LAYER_GROUND)
			{
				int x = 5;
				int y = 3;
				EditorGroundType groundType = getEditorGroundType();
				if (groundType == EDITOR_GROUND_DIRT)
				{
					x = 1;
					y = 1;
				}
				if (groundType == EDITOR_GROUND_DUNGEON)
				{
					x = 1;
					y = 4;
				}

				if (groundType == EDITOR_GROUND_WATER)
				{
					if ((mousePosX > 4 * TILE_SIZE && mousePosX < 5 * TILE_SIZE) && (mousePosY > 1 * TILE_SIZE && mousePosY < 2 * TILE_SIZE))
					{
						editorCurrentTile = TILE_WATER_FULL_DEEP;
						leaveTilesHudTimer = -0.3f;
						setEditorState(EDITOR_STATE_TILES);
						return;
					}
				}

				if (groundType == EDITOR_GROUND_DIRT)
				{
					if ((mousePosX > 0 * TILE_SIZE && mousePosX < 1 * TILE_SIZE) && (mousePosY > 0 * TILE_SIZE && mousePosY < 1 * TILE_SIZE))
					{
						editorCurrentTile = TILE_GROUND_DIRT_1;
						leaveTilesHudTimer = -0.3f;
						setEditorState(EDITOR_STATE_TILES);
					}
					return;
				}
				else if (groundType == EDITOR_GROUND_DUNGEON)
				{
					if ((mousePosX > 0 * TILE_SIZE && mousePosX < 1 * TILE_SIZE) && (mousePosY > 0 * TILE_SIZE && mousePosY < 1 * TILE_SIZE))
					{
						editorCurrentTile = TILE_GROUND_DUNGEON_1;
						leaveTilesHudTimer = -0.3f;
						setEditorState(EDITOR_STATE_TILES);
					}
					else if ((mousePosX > 0 * TILE_SIZE && mousePosX < 1 * TILE_SIZE) && (mousePosY > 1 * TILE_SIZE && mousePosY < 2 * TILE_SIZE))
					{
						editorCurrentTile = TILE_GROUND_DUNGEON_2;
						leaveTilesHudTimer = -0.3f;
						setEditorState(EDITOR_STATE_TILES);
					}
					else if ((mousePosX > 0 * TILE_SIZE && mousePosX < 1 * TILE_SIZE) && (mousePosY > 2 * TILE_SIZE && mousePosY < 3 * TILE_SIZE))
					{
						editorCurrentTile = TILE_GROUND_DUNGEON_3;
						leaveTilesHudTimer = -0.3f;
						setEditorState(EDITOR_STATE_TILES);
					}
					else if ((mousePosX > 0 * TILE_SIZE && mousePosX < 1 * TILE_SIZE) && (mousePosY > 3 * TILE_SIZE && mousePosY < 4 * TILE_SIZE))
					{
						editorCurrentTile = TILE_GROUND_DUNGEON_4;
						leaveTilesHudTimer = -0.3f;
						setEditorState(EDITOR_STATE_TILES);
					}
					return;
				}

				for (int i = 0; i < x; i++) {
					for (int j = 0; j < y; j++) {
						if ((mousePosX > i * TILE_SIZE && mousePosX < (i + 1) * TILE_SIZE) && (mousePosY > j * TILE_SIZE && mousePosY < (j + 1) * TILE_SIZE))
						{
							editorCurrentTile = i * y + j + groundMaterialValue;
							leaveTilesHudTimer = -0.3f;
							setEditorState(EDITOR_STATE_TILES);
						}
					}
				}
			}
			else if (tilesLayer == EDITOR_LAYER_WALLS)
			{
				pickWallsTiles(vector2f(mousePosX, mousePosY));
			}
			else if (tilesLayer == EDITOR_LAYER_COLLISION)
			{
				for (int i = 0; i < 25; i++)
				{
					if ((mousePosX > 0 && mousePosX < (i + 1) * TILE_SIZE) && (mousePosY > 0 && mousePosY < TILE_SIZE))
					{
						editorCurrentCollision = (mousePosX / TILE_SIZE) + 1;
						leaveTilesHudTimer = -0.3f;
						setEditorState(EDITOR_STATE_TILES);
					}
				}
			}
		}
	}
}

void editor_ChangeBrush(Window* _window)
{
	static float timer = 0.f;
	timer += getDeltaTime();

	if (timer > 0.2f) {
		if (customKeyboardIsKeyPressed(sfKeyG, _window->renderWindow) && brushSize < map.mapSize.x / 2 && brushSize < map.mapSize.y / 2) {
			timer = 0.f;
			brushSize++;
		}
		else if (customKeyboardIsKeyPressed(sfKeyH, _window->renderWindow) && brushSize > 1) {
			timer = 0.f;
			brushSize--;
		}
	}
}

void editor_AddRow() {
	map.mapSize.y++;
	map.layerGround_first = (LayerGround**)realloc(map.layerGround_first, map.mapSize.y * sizeof(LayerGround*));
	map.layerGround_second = (LayerGround**)realloc(map.layerGround_second, map.mapSize.y * sizeof(LayerGround*));
	map.layerWalls_first = (LayerWalls**)realloc(map.layerWalls_first, map.mapSize.y * sizeof(LayerWalls*));
	map.layerWalls_second = (LayerWalls**)realloc(map.layerWalls_second, map.mapSize.y * sizeof(LayerWalls*));
	map.layerCollision = (LayerCollision**)realloc(map.layerCollision, map.mapSize.y * sizeof(LayerCollision*));

	map.layerGround_first[map.mapSize.y - 1] = (LayerGround*)calloc(map.mapSize.x, sizeof(LayerGround));
	map.layerGround_second[map.mapSize.y - 1] = (LayerGround*)calloc(map.mapSize.x, sizeof(LayerGround));
	map.layerWalls_first[map.mapSize.y - 1] = (LayerWalls*)calloc(map.mapSize.x, sizeof(LayerWalls));
	map.layerWalls_second[map.mapSize.y - 1] = (LayerWalls*)calloc(map.mapSize.x, sizeof(LayerWalls));
	map.layerCollision[map.mapSize.y - 1] = (LayerCollision*)calloc(map.mapSize.x, sizeof(LayerCollision));

	for (int i = 0; i < map.mapSize.x; i++)
	{
		map.layerGround_first[map.mapSize.y - 1][i].type = TILE_SAND_FULL;
		map.layerGround_second[map.mapSize.y - 1][i].type = TILE_NO_TILE;

		map.layerWalls_first[map.mapSize.y - 1][i].type = WALL_NO_TILE;
		map.layerWalls_second[map.mapSize.y - 1][i].type = WALL_NO_TILE;

		map.layerCollision[map.mapSize.y - 1][i].type = CLD_NO_COLL;
	}
}

void editor_RemoveRow()
{
	map.mapSize.y--;
	if (map.mapSize.y < 1)
		map.mapSize.y = 1;
	else
	{
		free(map.layerGround_first[map.mapSize.y]);
		free(map.layerGround_second[map.mapSize.y]);
		free(map.layerWalls_first[map.mapSize.y]);
		free(map.layerWalls_second[map.mapSize.y]);
		free(map.layerCollision[map.mapSize.y]);

		map.layerGround_first = (LayerGround**)realloc(map.layerGround_first, map.mapSize.y * sizeof(LayerGround*));
		map.layerGround_second = (LayerGround**)realloc(map.layerGround_second, map.mapSize.y * sizeof(LayerGround*));
		map.layerWalls_first = (LayerWalls**)realloc(map.layerWalls_first, map.mapSize.y * sizeof(LayerWalls*));
		map.layerWalls_second = (LayerWalls**)realloc(map.layerWalls_second, map.mapSize.y * sizeof(LayerWalls*));
		map.layerCollision = (LayerCollision**)realloc(map.layerCollision, map.mapSize.y * sizeof(LayerCollision*));
	}
}

void editor_AddColumn()
{
	map.mapSize.x++;
	for (int i = 0; i < map.mapSize.y; i++)
	{
		map.layerGround_first[i] = (LayerGround*)realloc(map.layerGround_first[i], map.mapSize.x * sizeof(LayerGround));
		map.layerGround_second[i] = (LayerGround*)realloc(map.layerGround_second[i], map.mapSize.x * sizeof(LayerGround));
		map.layerGround_first[i][map.mapSize.x - 1].type = TILE_SAND_FULL;
		map.layerGround_second[i][map.mapSize.x - 1].type = TILE_NO_TILE;

		map.layerWalls_first[i] = (LayerWalls*)realloc(map.layerWalls_first[i], map.mapSize.x * sizeof(LayerWalls));
		map.layerWalls_first[i][map.mapSize.x - 1].type = WALL_NO_TILE;
		map.layerWalls_second[i] = (LayerWalls*)realloc(map.layerWalls_second[i], map.mapSize.x * sizeof(LayerWalls));
		map.layerWalls_second[i][map.mapSize.x - 1].type = WALL_NO_TILE;

		map.layerCollision[i] = (LayerCollision*)realloc(map.layerCollision[i], map.mapSize.x * sizeof(LayerCollision));
		map.layerCollision[i][map.mapSize.x - 1].type = CLD_NO_COLL;
	}
}

void editor_RemoveColumn()
{
	map.mapSize.x--;
	if (map.mapSize.x < 1)
		map.mapSize.x = 1;
	for (int i = 0; i < map.mapSize.y; i++)
	{
		map.layerGround_first[i] = (LayerGround*)realloc(map.layerGround_first[i], map.mapSize.x * sizeof(LayerGround));
		map.layerGround_second[i] = (LayerGround*)realloc(map.layerGround_second[i], map.mapSize.x * sizeof(LayerGround));
		map.layerWalls_first[i] = (LayerWalls*)realloc(map.layerWalls_first[i], map.mapSize.x * sizeof(LayerWalls));
		map.layerWalls_second[i] = (LayerWalls*)realloc(map.layerWalls_second[i], map.mapSize.x * sizeof(LayerWalls));
		map.layerCollision[i] = (LayerCollision*)realloc(map.layerCollision[i], map.mapSize.x * sizeof(LayerCollision));
	}
}

void editor_PlaceTile(int _x, int _y, int _brushSize)
{
	switch (getEditorTilesLayer())
	{
	case EDITOR_LAYER_GROUND:
		for (int i = 1 - _brushSize; i < _brushSize; i++) {
			for (int j = 1 - _brushSize; j < _brushSize; j++) {
				if (editorHud_getFirstOrSecondLayer() == 1)
				{
					if (editorCurrentTile < TILE_LEAF_FULL || editorCurrentTile > TILE_LEAF_FRAME)
					{
						if (editorCurrentTile >= TILE_GROUND_DIRT_1 && editorCurrentTile <= TILE_GROUND_DIRT_4)
							editorCurrentTile = rand_int(TILE_GROUND_DIRT_1, TILE_GROUND_DIRT_4 + 1);
						map.layerGround_first[_y + j][_x + i].type = editorCurrentTile;
						if (editorCurrentTile != TILE_WATER_FULL && editorCurrentTile != TILE_WATER_FULL_DEEP)
							map.layerGround_first[_y + j][_x + i].variant = editor_SelectVariant(vector2i(_x + i, _y + j), editorCurrentTile);
						else
							map.layerGround_first[_y + j][_x + i].variant = 0;
					}
				}
				else
				{
					if (editorCurrentTile >= TILE_GROUND_DIRT_1 && editorCurrentTile <= TILE_GROUND_DIRT_4)
						editorCurrentTile = rand_int(TILE_GROUND_DIRT_1, TILE_GROUND_DIRT_4 + 1);
					map.layerGround_second[_y + j][_x + i].type = editorCurrentTile;
					if (editorCurrentTile != TILE_WATER_FULL && editorCurrentTile != TILE_WATER_FULL_DEEP)
						map.layerGround_second[_y + j][_x + i].variant = editor_SelectVariant(vector2i(_x + i, _y + j), editorCurrentTile);
					else
						map.layerGround_second[_y + j][_x + i].variant = 0;
				}
			}
		}
		break;
	case EDITOR_LAYER_WALLS:
		for (int i = 1 - _brushSize; i < _brushSize; i++) {
			for (int j = 1 - _brushSize; j < _brushSize; j++) {
				if (editorHud_getFirstOrSecondLayer() == 1)
				{
					map.layerWalls_first[_y + j][_x + i].type = editorCurrentWall;
					map.layerWalls_first[_y + j][_x + i].variant = editor_SelectVariant(vector2i(_x + i, _y + j), editorCurrentWall);
				}
				else
				{
					map.layerWalls_second[_y + j][_x + i].type = editorCurrentWall;
					map.layerWalls_second[_y + j][_x + i].variant = editor_SelectVariant(vector2i(_x + i, _y + j), editorCurrentWall);
				}
			}
		}
		break;
	case EDITOR_LAYER_COLLISION: 
		for (int i = 1 - _brushSize; i < _brushSize; i++) {
			for (int j = 1 - _brushSize; j < _brushSize; j++) {
				map.layerCollision[_y + j][_x + i].type = editor_IdToCld(editorCurrentCollision);
			}
		}
		break;
	}
}

void setGroundMaterialValue(int _val)
{
	groundMaterialValue = _val;
}

int getGroundMaterialValue()
{
	return groundMaterialValue;
}

void setWallMaterialValue(int _val)
{
	wallMaterialValue = _val;
}

int getWallMaterialValue()
{
	return wallMaterialValue;
}

TilesType getEditorCurrentTile()
{
	return editorCurrentTile;
}

void setEditorCurrentTile(int _val)
{
	editorCurrentTile = _val;
}

void setEditorBrushSize(int _val)
{
	brushSize = _val;
}

int getEditorBrushSize()
{
	return brushSize;
}

void setEditorIndexMap(int _val)
{
	indexMap = _val;
}

int getEditorIndexMap()
{
	return indexMap;
}

int editor_CldToId(CollisionType _type) {
	switch (_type) {
	default:						return 0;
	case CLD_BOX:					return 1;
	case CLD_DIAG_BOTTOM_RIGHT:		return 2;
	case CLD_DIAG_BOTTOM_LEFT:		return 3;
	case CLD_DIAG_TOP_LEFT:			return 4;
	case CLD_DIAG_TOP_RIGHT:		return 5;
	case CLD_CORN_TOP_RIGHT:		return 6;
	case CLD_CORN_TOP_LEFT:			return 7;
	case CLD_CORN_BOTTOM_RIGHT:		return 8;
	case CLD_CORN_BOTTOM_LEFT:		return 9;
	case CLD_HALF_LEFT:				return 10;
	case CLD_HALF_RIGHT:			return 11;
	case CLD_HALF_TOP:				return 12;
	case CLD_HALF_BOTTOM:			return 13;
	case CLD_ANGLE_TOP_LEFT:	    return 14;
	case CLD_ANGLE_TOP_RIGHT:	    return 15;
	case CLD_ANGLE_BOTTOM_RIGHT:	return 16;
	case CLD_ANGLE_BOTTOM_LEFT:	    return 17;
	case CLD_SMALL_DIAG_TOP_LEFT:	return 18;
	case CLD_SMALL_DIAG_TOP_RIGHT:	return 19;
	case CLD_SMALL_TRI_TL:	        return 20;
	case CLD_SMALL_TRI_TR:	        return 21;
	case CLD_SMALL_TRI_BR:	        return 22;
	case CLD_SMALL_TRI_BL:	        return 23;
	case CLD_VOID:					return 24;
	}
}

CollisionType editor_IdToCld(int _id) {
	switch (_id) {
	default:	return CLD_NO_COLL;
	case 1:		return CLD_BOX;
	case 2:		return CLD_DIAG_BOTTOM_RIGHT;
	case 3:		return CLD_DIAG_BOTTOM_LEFT;
	case 4:		return CLD_DIAG_TOP_LEFT;
	case 5:		return CLD_DIAG_TOP_RIGHT;
	case 6:		return CLD_CORN_TOP_RIGHT;
	case 7:		return CLD_CORN_TOP_LEFT;
	case 8:		return CLD_CORN_BOTTOM_RIGHT;
	case 9:		return CLD_CORN_BOTTOM_LEFT;
	case 10:	return CLD_HALF_LEFT;
	case 11:	return CLD_HALF_RIGHT;
	case 12:	return CLD_HALF_TOP;
	case 13:	return CLD_HALF_BOTTOM;
	case 14:	return CLD_ANGLE_TOP_LEFT;
	case 15:	return CLD_ANGLE_TOP_RIGHT;
	case 16:	return CLD_ANGLE_BOTTOM_RIGHT;
	case 17:	return CLD_ANGLE_BOTTOM_LEFT;
	case 18:	return CLD_SMALL_DIAG_TOP_LEFT;
	case 19:	return CLD_SMALL_DIAG_TOP_RIGHT;
	case 20:	return CLD_SMALL_TRI_TL;
	case 21:	return CLD_SMALL_TRI_TR;
	case 22:	return CLD_SMALL_TRI_BR;
	case 23:	return CLD_SMALL_TRI_BL;
	case 24:	return CLD_VOID;
	}
}

char editor_SelectVariant(sfVector2i _coords, TilesType _type) {
	int geom = _type & 0b1111;
	int rdz = rand();
	int total = editorValueRateTile1 + editorValueRateTile2 + editorValueRateTile3 + editorValueRateTile4;
	if (total == 0)
		return 0;
	if (geom == TILE_FULL) {
		int rMod = rdz % total;
		if (rMod < editorValueRateTile1) return 0;
		else if (rMod < editorValueRateTile1 + editorValueRateTile2) return 1;
		else if (rMod < editorValueRateTile1 + editorValueRateTile2 + editorValueRateTile3) return 2;
		else return 3;
	}
	if (geom >= TILE_EDGE_T && geom <= TILE_DIAG_BR) return rdz % 2;
	return 0;
}

void setEditorValueRateTiles(int _val1, int _val2, int _val3, int _val4)
{
	editorValueRateTile1 = _val1;
	editorValueRateTile2 = _val2;
	editorValueRateTile3 = _val3;
	editorValueRateTile4 = _val4;
}

int getEditorValueRateTile1()
{
	return editorValueRateTile1;
}

int getEditorValueRateTile2()
{
	return editorValueRateTile2;
}

int getEditorValueRateTile3()
{
	return editorValueRateTile3;
}

int getEditorValueRateTile4()
{
	return editorValueRateTile4;
}

void setLightOrShadow(sfBool _val)
{
	lightOrShadow = _val;
}

sfBool getLightOrShadow()
{
	return lightOrShadow;
}

void pickWallsTiles(sfVector2f _mouse)
{
	WallsType wallType = WALL_NO_TILE;
	if ((_mouse.x >= 0.f && _mouse.x <= 576.f) && (_mouse.y >= 0.f && _mouse.y <= 576.f))
	{
		/// FIRST ROW
		if ((_mouse.x >= 0.f * TILE_SIZE && _mouse.x <= 1.f * TILE_SIZE) && (_mouse.y >= 0.f * TILE_SIZE && _mouse.y <= 1.f * TILE_SIZE))
			wallType = WALL_TOP_DIAG_TL;
		else if ((_mouse.x >= 1.f * TILE_SIZE && _mouse.x <= 2.f * TILE_SIZE) && (_mouse.y >= 0.f * TILE_SIZE && _mouse.y <= 1.f * TILE_SIZE))
			wallType = WALL_TOP_EDGE_T;
		else if ((_mouse.x >= 2.f * TILE_SIZE && _mouse.x <= 3.f * TILE_SIZE) && (_mouse.y >= 0.f * TILE_SIZE && _mouse.y <= 1.f * TILE_SIZE))
			wallType = WALL_TOP_DIAG_TR;
		else if ((_mouse.x >= 3.f * TILE_SIZE && _mouse.x <= 4.f * TILE_SIZE) && (_mouse.y >= 0.f * TILE_SIZE && _mouse.y <= 1.f * TILE_SIZE))
			wallType = WALL_TOP_EDGE_L_FILLED;
		else if ((_mouse.x >= 4.f * TILE_SIZE && _mouse.x <= 5.f * TILE_SIZE) && (_mouse.y >= 0.f * TILE_SIZE && _mouse.y <= 1.f * TILE_SIZE))
			wallType = WALL_TOP_EDGE_R_FILLED;
		else if ((_mouse.x >= 5.f * TILE_SIZE && _mouse.x <= 6.f * TILE_SIZE) && (_mouse.y >= 0.f * TILE_SIZE && _mouse.y <= 1.f * TILE_SIZE))
			wallType = WALL_TOP_CORNER_TL;
		else if ((_mouse.x >= 6.f * TILE_SIZE && _mouse.x <= 7.f * TILE_SIZE) && (_mouse.y >= 0.f * TILE_SIZE && _mouse.y <= 1.f * TILE_SIZE))
			wallType = WALL_TOP_CORNER_TR;
		else if ((_mouse.x >= 7.f * TILE_SIZE && _mouse.x <= 8.f * TILE_SIZE) && (_mouse.y >= 0.f * TILE_SIZE && _mouse.y <= 1.f * TILE_SIZE))
			wallType = WALL_TOP_CORNER_TL_FILLED;
		else if ((_mouse.x >= 8.f * TILE_SIZE && _mouse.x <= 9.f * TILE_SIZE) && (_mouse.y >= 0.f * TILE_SIZE && _mouse.y <= 1.f * TILE_SIZE))
			wallType = WALL_TOP_CORNER_TR_FILLED;

		/// SECOND ROW
		else if ((_mouse.x >= 0.f * TILE_SIZE && _mouse.x <= 1.f * TILE_SIZE) && (_mouse.y >= 1.f * TILE_SIZE && _mouse.y <= 2.f * TILE_SIZE))
			wallType = WALL_TOP_EDGE_L;
		else if ((_mouse.x >= 1.f * TILE_SIZE && _mouse.x <= 2.f * TILE_SIZE) && (_mouse.y >= 1.f * TILE_SIZE && _mouse.y <= 2.f * TILE_SIZE))
			wallType = WALL_TOP_EDGE_T_FILLED;
		else if ((_mouse.x >= 2.f * TILE_SIZE && _mouse.x <= 3.f * TILE_SIZE) && (_mouse.y >= 1.f * TILE_SIZE && _mouse.y <= 2.f * TILE_SIZE))
			wallType = WALL_TOP_EDGE_R;
		else if ((_mouse.x >= 3.f * TILE_SIZE && _mouse.x <= 4.f * TILE_SIZE) && (_mouse.y >= 1.f * TILE_SIZE && _mouse.y <= 2.f * TILE_SIZE))
			wallType = WALL_TOP_DIAG_BL_FILLED;
		else if ((_mouse.x >= 4.f * TILE_SIZE && _mouse.x <= 5.f * TILE_SIZE) && (_mouse.y >= 1.f * TILE_SIZE && _mouse.y <= 2.f * TILE_SIZE))
			wallType = WALL_TOP_DIAG_BR_FILLED;
		else if ((_mouse.x >= 5.f * TILE_SIZE && _mouse.x <= 6.f * TILE_SIZE) && (_mouse.y >= 1.f * TILE_SIZE && _mouse.y <= 2.f * TILE_SIZE))
			wallType = WALL_TOP_CORNER_BL;
		else if ((_mouse.x >= 6.f * TILE_SIZE && _mouse.x <= 7.f * TILE_SIZE) && (_mouse.y >= 1.f * TILE_SIZE && _mouse.y <= 2.f * TILE_SIZE))
			wallType = WALL_TOP_CORNER_BR;
		else if ((_mouse.x >= 7.f * TILE_SIZE && _mouse.x <= 8.f * TILE_SIZE) && (_mouse.y >= 1.f * TILE_SIZE && _mouse.y <= 2.f * TILE_SIZE))
			wallType = WALL_TOP_CORNER_BL_FILLED;
		else if ((_mouse.x >= 8.f * TILE_SIZE && _mouse.x <= 9.f * TILE_SIZE) && (_mouse.y >= 1.f * TILE_SIZE && _mouse.y <= 2.f * TILE_SIZE))
			wallType = WALL_TOP_CORNER_BR_FILLED;

		/// THIRD ROW
		else if ((_mouse.x >= 0.f * TILE_SIZE && _mouse.x <= 1.f * TILE_SIZE) && (_mouse.y >= 2.f * TILE_SIZE && _mouse.y <= 3.f * TILE_SIZE))
			wallType = WALL_TOP_DIAG_BL;
		else if ((_mouse.x >= 1.f * TILE_SIZE && _mouse.x <= 2.f * TILE_SIZE) && (_mouse.y >= 2.f * TILE_SIZE && _mouse.y <= 3.f * TILE_SIZE))
			wallType = WALL_TOP_EDGE_B;
		else if ((_mouse.x >= 2.f * TILE_SIZE && _mouse.x <= 3.f * TILE_SIZE) && (_mouse.y >= 2.f * TILE_SIZE && _mouse.y <= 3.f * TILE_SIZE))
			wallType = WALL_TOP_DIAG_BR;
		else if ((_mouse.x >= 3.f * TILE_SIZE && _mouse.x <= 4.f * TILE_SIZE) && (_mouse.y >= 2.f * TILE_SIZE && _mouse.y <= 3.f * TILE_SIZE))
			wallType = WALL_MID_DIAG_BL_TRANSP;
		else if ((_mouse.x >= 4.f * TILE_SIZE && _mouse.x <= 5.f * TILE_SIZE) && (_mouse.y >= 2.f * TILE_SIZE && _mouse.y <= 3.f * TILE_SIZE))
			wallType = WALL_MID_DIAG_BR_TRANSP;
		else if ((_mouse.x >= 5.f * TILE_SIZE && _mouse.x <= 6.f * TILE_SIZE) && (_mouse.y >= 2.f * TILE_SIZE && _mouse.y <= 3.f * TILE_SIZE))
			wallType = WALL_MID_FULL;
		else if ((_mouse.x >= 6.f * TILE_SIZE && _mouse.x <= 7.f * TILE_SIZE) && (_mouse.y >= 2.f * TILE_SIZE && _mouse.y <= 3.f * TILE_SIZE))
			wallType = WALL_MID_FULL;
		else if ((_mouse.x >= 7.f * TILE_SIZE && _mouse.x <= 8.f * TILE_SIZE) && (_mouse.y >= 2.f * TILE_SIZE && _mouse.y <= 3.f * TILE_SIZE))
			wallType = WALL_MID_FULL_LEFT;
		else if ((_mouse.x >= 8.f * TILE_SIZE && _mouse.x <= 9.f * TILE_SIZE) && (_mouse.y >= 2.f * TILE_SIZE && _mouse.y <= 3.f * TILE_SIZE))
			wallType = WALL_MID_FULL_RIGHT;

		/// FOURTH ROW
		else if ((_mouse.x >= 0.f * TILE_SIZE && _mouse.x <= 1.f * TILE_SIZE) && (_mouse.y >= 3.f * TILE_SIZE && _mouse.y <= 4.f * TILE_SIZE))
			wallType = WALL_MID_DIAG_BL;
		else if ((_mouse.x >= 1.f * TILE_SIZE && _mouse.x <= 2.f * TILE_SIZE) && (_mouse.y >= 3.f * TILE_SIZE && _mouse.y <= 4.f * TILE_SIZE))
			wallType = WALL_MID_FULL;
		else if ((_mouse.x >= 2.f * TILE_SIZE && _mouse.x <= 3.f * TILE_SIZE) && (_mouse.y >= 3.f * TILE_SIZE && _mouse.y <= 4.f * TILE_SIZE))
			wallType = WALL_MID_DIAG_BR;
		else if ((_mouse.x >= 3.f * TILE_SIZE && _mouse.x <= 4.f * TILE_SIZE) && (_mouse.y >= 3.f * TILE_SIZE && _mouse.y <= 4.f * TILE_SIZE))
			wallType = WALL_TOP_INNER_TL;
		else if ((_mouse.x >= 4.f * TILE_SIZE && _mouse.x <= 5.f * TILE_SIZE) && (_mouse.y >= 3.f * TILE_SIZE && _mouse.y <= 4.f * TILE_SIZE))
			wallType = WALL_TOP_INNER_TR;
		else if ((_mouse.x >= 5.f * TILE_SIZE && _mouse.x <= 6.f * TILE_SIZE) && (_mouse.y >= 3.f * TILE_SIZE && _mouse.y <= 4.f * TILE_SIZE))
			wallType = WALL_BASE_FULL;
		else if ((_mouse.x >= 6.f * TILE_SIZE && _mouse.x <= 7.f * TILE_SIZE) && (_mouse.y >= 3.f * TILE_SIZE && _mouse.y <= 4.f * TILE_SIZE))
			wallType = WALL_BASE_FULL;
		else if ((_mouse.x >= 7.f * TILE_SIZE && _mouse.x <= 8.f * TILE_SIZE) && (_mouse.y >= 3.f * TILE_SIZE && _mouse.y <= 4.f * TILE_SIZE))
			wallType = WALL_BASE_FULL_LEFT;
		else if ((_mouse.x >= 8.f * TILE_SIZE && _mouse.x <= 9.f * TILE_SIZE) && (_mouse.y >= 3.f * TILE_SIZE && _mouse.y <= 4.f * TILE_SIZE))
			wallType = WALL_BASE_FULL_RIGHT;

		/// FIFTH ROW
		else if ((_mouse.x >= 0.f * TILE_SIZE && _mouse.x <= 1.f * TILE_SIZE) && (_mouse.y >= 4.f * TILE_SIZE && _mouse.y <= 5.f * TILE_SIZE))
			wallType = WALL_BASE_DIAG_BL_FULL;
		else if ((_mouse.x >= 1.f * TILE_SIZE && _mouse.x <= 2.f * TILE_SIZE) && (_mouse.y >= 4.f * TILE_SIZE && _mouse.y <= 5.f * TILE_SIZE))
			wallType = WALL_BASE_FULL;
		else if ((_mouse.x >= 2.f * TILE_SIZE && _mouse.x <= 3.f * TILE_SIZE) && (_mouse.y >= 4.f * TILE_SIZE && _mouse.y <= 5.f * TILE_SIZE))
			wallType = WALL_BASE_DIAG_BR_FULL;
		else if ((_mouse.x >= 3.f * TILE_SIZE && _mouse.x <= 4.f * TILE_SIZE) && (_mouse.y >= 4.f * TILE_SIZE && _mouse.y <= 5.f * TILE_SIZE))
			wallType = WALL_TOP_INNER_BL;
		else if ((_mouse.x >= 4.f * TILE_SIZE && _mouse.x <= 5.f * TILE_SIZE) && (_mouse.y >= 4.f * TILE_SIZE && _mouse.y <= 5.f * TILE_SIZE))
			wallType = WALL_TOP_INNER_BR;
		else if ((_mouse.x >= 5.f * TILE_SIZE && _mouse.x <= 6.f * TILE_SIZE) && (_mouse.y >= 4.f * TILE_SIZE && _mouse.y <= 5.f * TILE_SIZE))
			wallType = WALL_BASE_HALF_WATER;
		else if ((_mouse.x >= 6.f * TILE_SIZE && _mouse.x <= 7.f * TILE_SIZE) && (_mouse.y >= 4.f * TILE_SIZE && _mouse.y <= 5.f * TILE_SIZE))
			wallType = WALL_BASE_HALF_WATER;
		else if ((_mouse.x >= 7.f * TILE_SIZE && _mouse.x <= 8.f * TILE_SIZE) && (_mouse.y >= 4.f * TILE_SIZE && _mouse.y <= 5.f * TILE_SIZE))
			wallType = WALL_VOID;
		else if ((_mouse.x >= 8.f * TILE_SIZE && _mouse.x <= 9.f * TILE_SIZE) && (_mouse.y >= 4.f * TILE_SIZE && _mouse.y <= 5.f * TILE_SIZE))
			wallType = WALL_VOID;

		/// SIXTH ROW
		else if ((_mouse.x >= 0.f * TILE_SIZE && _mouse.x <= 1.f * TILE_SIZE) && (_mouse.y >= 5.f * TILE_SIZE && _mouse.y <= 6.f * TILE_SIZE))
			wallType = WALL_VOID_GRAD_DIAG_BL;
		else if ((_mouse.x >= 1.f * TILE_SIZE && _mouse.x <= 2.f * TILE_SIZE) && (_mouse.y >= 5.f * TILE_SIZE && _mouse.y <= 6.f * TILE_SIZE))
			wallType = WALL_VOID_GRAD_DIAG_BR;
		else if ((_mouse.x >= 2.f * TILE_SIZE && _mouse.x <= 3.f * TILE_SIZE) && (_mouse.y >= 5.f * TILE_SIZE && _mouse.y <= 6.f * TILE_SIZE))
			wallType = WALL_VOID_GRAD_DIAG_BL_EDGE;
		else if ((_mouse.x >= 3.f * TILE_SIZE && _mouse.x <= 4.f * TILE_SIZE) && (_mouse.y >= 5.f * TILE_SIZE && _mouse.y <= 6.f * TILE_SIZE))
			wallType = WALL_WATER_STAIR;
		else if ((_mouse.x >= 4.f * TILE_SIZE && _mouse.x <= 5.f * TILE_SIZE) && (_mouse.y >= 5.f * TILE_SIZE && _mouse.y <= 6.f * TILE_SIZE))
			wallType = WALL_VOID_GRAD_DIAG_BR_EDGE;
		else if ((_mouse.x >= 5.f * TILE_SIZE && _mouse.x <= 6.f * TILE_SIZE) && (_mouse.y >= 5.f * TILE_SIZE && _mouse.y <= 6.f * TILE_SIZE))
			wallType = WALL_SHADOW_B;
		else if ((_mouse.x >= 6.f * TILE_SIZE && _mouse.x <= 7.f * TILE_SIZE) && (_mouse.y >= 5.f * TILE_SIZE && _mouse.y <= 6.f * TILE_SIZE))
			wallType = WALL_SHADOW_HALF;
		else if ((_mouse.x >= 7.f * TILE_SIZE && _mouse.x <= 8.f * TILE_SIZE) && (_mouse.y >= 5.f * TILE_SIZE && _mouse.y <= 6.f * TILE_SIZE))
			wallType = WALL_TOP_DIAG_TL_DARK;
		else if ((_mouse.x >= 8.f * TILE_SIZE && _mouse.x <= 9.f * TILE_SIZE) && (_mouse.y >= 5.f * TILE_SIZE && _mouse.y <= 6.f * TILE_SIZE))
			wallType = WALL_TOP_DIAG_TR_DARK;

		/// SEVENTH ROW
		else if ((_mouse.x >= 0.f * TILE_SIZE && _mouse.x <= 1.f * TILE_SIZE) && (_mouse.y >= 6.f * TILE_SIZE && _mouse.y <= 7.f * TILE_SIZE))
			wallType = WALL_VOID_DIAG_BL;
		else if ((_mouse.x >= 1.f * TILE_SIZE && _mouse.x <= 2.f * TILE_SIZE) && (_mouse.y >= 6.f * TILE_SIZE && _mouse.y <= 7.f * TILE_SIZE))
			wallType = WALL_VOID_DIAG_BR;
		else if ((_mouse.x >= 2.f * TILE_SIZE && _mouse.x <= 3.f * TILE_SIZE) && (_mouse.y >= 6.f * TILE_SIZE && _mouse.y <= 7.f * TILE_SIZE))
			wallType = WALL_NO_TILE;														
		else if ((_mouse.x >= 3.f * TILE_SIZE && _mouse.x <= 4.f * TILE_SIZE) && (_mouse.y >= 6.f * TILE_SIZE && _mouse.y <= 7.f * TILE_SIZE))
			wallType = WALL_VOID;
		else if ((_mouse.x >= 4.f * TILE_SIZE && _mouse.x <= 5.f * TILE_SIZE) && (_mouse.y >= 6.f * TILE_SIZE && _mouse.y <= 7.f * TILE_SIZE))
			wallType = WALL_NO_TILE;														
		else if ((_mouse.x >= 5.f * TILE_SIZE && _mouse.x <= 6.f * TILE_SIZE) && (_mouse.y >= 6.f * TILE_SIZE && _mouse.y <= 7.f * TILE_SIZE))
			wallType = WALL_SHADOW_FULL;
		else if ((_mouse.x >= 6.f * TILE_SIZE && _mouse.x <= 7.f * TILE_SIZE) && (_mouse.y >= 6.f * TILE_SIZE && _mouse.y <= 7.f * TILE_SIZE))
			wallType = WALL_SHADOW_T;
		else if ((_mouse.x >= 7.f * TILE_SIZE && _mouse.x <= 8.f * TILE_SIZE) && (_mouse.y >= 6.f * TILE_SIZE && _mouse.y <= 7.f * TILE_SIZE))
			wallType = WALL_TOP_EDGE_L_DARK;
		else if ((_mouse.x >= 8.f * TILE_SIZE && _mouse.x <= 9.f * TILE_SIZE) && (_mouse.y >= 6.f * TILE_SIZE && _mouse.y <= 7.f * TILE_SIZE))
			wallType = WALL_TOP_EDGE_R_DARK;

		/// EIGHTH ROW
		else if ((_mouse.x >= 0.f * TILE_SIZE && _mouse.x <= 1.f * TILE_SIZE) && (_mouse.y >= 7.f * TILE_SIZE && _mouse.y <= 8.f * TILE_SIZE))
			wallType = WALL_MID_DIAG_BL_WATER;
		else if ((_mouse.x >= 1.f * TILE_SIZE && _mouse.x <= 2.f * TILE_SIZE) && (_mouse.y >= 7.f * TILE_SIZE && _mouse.y <= 8.f * TILE_SIZE))
			wallType = WALL_MID_DIAG_BR_WATER;
		else if ((_mouse.x >= 2.f * TILE_SIZE && _mouse.x <= 3.f * TILE_SIZE) && (_mouse.y >= 7.f * TILE_SIZE && _mouse.y <= 8.f * TILE_SIZE))
			wallType = WALL_TOP_DIAG_BL_WATER;
		else if ((_mouse.x >= 3.f * TILE_SIZE && _mouse.x <= 4.f * TILE_SIZE) && (_mouse.y >= 7.f * TILE_SIZE && _mouse.y <= 8.f * TILE_SIZE))
			wallType = WALL_TOP_STAIR;
		else if ((_mouse.x >= 4.f * TILE_SIZE && _mouse.x <= 5.f * TILE_SIZE) && (_mouse.y >= 7.f * TILE_SIZE && _mouse.y <= 8.f * TILE_SIZE))
			wallType = WALL_TOP_DIAG_BR_WATER;
		else if ((_mouse.x >= 5.f * TILE_SIZE && _mouse.x <= 6.f * TILE_SIZE) && (_mouse.y >= 7.f * TILE_SIZE && _mouse.y <= 8.f * TILE_SIZE))
			wallType = WALL_TOP_CORNER_TL_DARK;
		else if ((_mouse.x >= 6.f * TILE_SIZE && _mouse.x <= 7.f * TILE_SIZE) && (_mouse.y >= 7.f * TILE_SIZE && _mouse.y <= 8.f * TILE_SIZE))
			wallType = WALL_TOP_CORNER_TR_DARK;
		else if ((_mouse.x >= 7.f * TILE_SIZE && _mouse.x <= 8.f * TILE_SIZE) && (_mouse.y >= 7.f * TILE_SIZE && _mouse.y <= 8.f * TILE_SIZE))
			wallType = WALL_TOP_DIAG_BL_DARK;
		else if ((_mouse.x >= 8.f * TILE_SIZE && _mouse.x <= 9.f * TILE_SIZE) && (_mouse.y >= 7.f * TILE_SIZE && _mouse.y <= 8.f * TILE_SIZE))
			wallType = WALL_TOP_DIAG_BR_DARK;

		/// NINTH ROW
		else if ((_mouse.x >= 0.f * TILE_SIZE && _mouse.x <= 1.f * TILE_SIZE) && (_mouse.y >= 8.f * TILE_SIZE && _mouse.y <= 9.f * TILE_SIZE))
			wallType = WALL_BASE_DIAG_BL_HALF_WATER;
		else if ((_mouse.x >= 1.f * TILE_SIZE && _mouse.x <= 2.f * TILE_SIZE) && (_mouse.y >= 8.f * TILE_SIZE && _mouse.y <= 9.f * TILE_SIZE))
			wallType = WALL_BASE_DIAG_BR_HALF_WATER;
		else if ((_mouse.x >= 2.f * TILE_SIZE && _mouse.x <= 3.f * TILE_SIZE) && (_mouse.y >= 8.f * TILE_SIZE && _mouse.y <= 9.f * TILE_SIZE))
			wallType = WALL_BASE_DIAG_BL_FULL_WATER;
		else if ((_mouse.x >= 3.f * TILE_SIZE && _mouse.x <= 4.f * TILE_SIZE) && (_mouse.y >= 8.f * TILE_SIZE && _mouse.y <= 9.f * TILE_SIZE))
			wallType = WALL_BASE_HALF_WATER;
		else if ((_mouse.x >= 4.f * TILE_SIZE && _mouse.x <= 5.f * TILE_SIZE) && (_mouse.y >= 8.f * TILE_SIZE && _mouse.y <= 9.f * TILE_SIZE))
			wallType = WALL_BASE_DIAG_BR_FULL_WATER;
		else if ((_mouse.x >= 5.f * TILE_SIZE && _mouse.x <= 6.f * TILE_SIZE) && (_mouse.y >= 8.f * TILE_SIZE && _mouse.y <= 9.f * TILE_SIZE))
			wallType = WALL_TOP_CORNER_BL_DARK;
		else if ((_mouse.x >= 6.f * TILE_SIZE && _mouse.x <= 7.f * TILE_SIZE) && (_mouse.y >= 8.f * TILE_SIZE && _mouse.y <= 9.f * TILE_SIZE))
			wallType = WALL_TOP_CORNER_BR_DARK;
		else if ((_mouse.x >= 7.f * TILE_SIZE && _mouse.x <= 8.f * TILE_SIZE) && (_mouse.y >= 8.f * TILE_SIZE && _mouse.y <= 9.f * TILE_SIZE))
			wallType = WALL_TOP_EDGE_T_DARK;
		else if ((_mouse.x >= 8.f * TILE_SIZE && _mouse.x <= 9.f * TILE_SIZE) && (_mouse.y >= 8.f * TILE_SIZE && _mouse.y <= 9.f * TILE_SIZE))
			wallType = WALL_FULL_STAIR;

		editorCurrentWall = wallType + wallMaterialValue;
		leaveTilesHudTimer = -0.3f;
		setEditorState(EDITOR_STATE_TILES);
	}
}