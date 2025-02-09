#pragma once
#include "tools.h"
#include "textureManager.h"
#include "List.h"

#define TILE_SIZE 64.f
#define NB_MAX_TILES 30
#define NB_MAX_WALLS 71
#define NB_MAX_COLL_TYPE 25
#define NB_MAX_MAP_STATE 3
#define MAP_SAVE_PATH "../Ressources/Save/Map/"

/// <summary>
/// enums for map
/// </summary>
typedef enum {
	TILE_NO_TILE = -1, /// Void

	/// Bits 4-5 handle tile material
	TILE_MATERIAL_SAND = 1 << 4,
	TILE_MATERIAL_ROCK = 1 << 5,
	TILE_MATERIAL_GRASS = 1 << 6,
	TILE_MATERIAL_WATER = 1 << 7,
	TILE_MATERIAL_LEAF = 1 << 8,

	/// Bits 0-3 handle tile geometry
	TILE_FULL = 0,
	TILE_EDGE_T = 1,
	TILE_EDGE_B = 2,
	TILE_EDGE_L = 3,
	TILE_EDGE_R = 4,
	TILE_CORNER_TL = 5,
	TILE_CORNER_TR = 6,
	TILE_CORNER_BL = 7,
	TILE_CORNER_BR = 8,
	TILE_DIAG_TL = 9,
	TILE_DIAG_TR = 10,
	TILE_DIAG_BL = 11,
	TILE_DIAG_BR = 12,
	TILE_SOLO = 13,
	TILE_FRAME = 14,

	TILE_SAND_FULL = TILE_MATERIAL_SAND | TILE_FULL,
	TILE_SAND_FRAME = TILE_MATERIAL_SAND | TILE_FRAME,

	TILE_ROCK_FULL = TILE_MATERIAL_ROCK | TILE_FULL,
	TILE_ROCK_FRAME = TILE_MATERIAL_ROCK | TILE_FRAME,

	TILE_GRASS_FULL = TILE_MATERIAL_GRASS | TILE_FULL,
	TILE_GRASS_FRAME = TILE_MATERIAL_GRASS | TILE_FRAME,

	TILE_WATER_FULL = TILE_MATERIAL_WATER | TILE_FULL,
	TILE_WATER_FRAME = TILE_MATERIAL_WATER | TILE_FRAME,
	TILE_WATER_FULL_DEEP,

	TILE_LEAF_FULL = TILE_MATERIAL_LEAF | TILE_FULL,
	TILE_LEAF_FRAME = TILE_MATERIAL_LEAF | TILE_FRAME,

	TILE_GROUND_DIRT_1,
	TILE_GROUND_DIRT_2,
	TILE_GROUND_DIRT_3,
	TILE_GROUND_DIRT_4,

	TILE_GROUND_DUNGEON_1,
	TILE_GROUND_DUNGEON_2,
	TILE_GROUND_DUNGEON_3,
	TILE_GROUND_DUNGEON_4
}TilesType;

typedef enum {
	WALL_NO_TILE = -1, /// No wall

	/// Bit 7 handles tile material
	WALL_MATERIAL_OUT = 0 << 7, /// Outside (rock)
	WALL_MATERIAL_IN = 1 << 7, /// Temples (bricks)

	/// Bits 0-6 handle tile geometry
	WALL_TOP_DIAG_TL = 1,
	WALL_TOP_EDGE_T,
	WALL_TOP_DIAG_TR,
	WALL_TOP_EDGE_L,
	WALL_TOP_EDGE_T_FILLED,
	WALL_TOP_EDGE_R,
	WALL_TOP_DIAG_BL,
	WALL_TOP_DIAG_BL_WATER,
	WALL_TOP_DIAG_BL_VOID,
	WALL_TOP_EDGE_B,
	WALL_TOP_DIAG_BR,
	WALL_TOP_DIAG_BR_WATER,
	WALL_TOP_DIAG_BR_VOID,
	WALL_TOP_EDGE_L_FILLED,
	WALL_TOP_EDGE_R_FILLED,
	WALL_TOP_DIAG_BL_FILLED,
	WALL_TOP_DIAG_BR_FILLED,
	WALL_TOP_CORNER_TL,
	WALL_TOP_CORNER_TR,
	WALL_TOP_CORNER_BL,
	WALL_TOP_CORNER_BR,
	WALL_TOP_CORNER_TL_FILLED,
	WALL_TOP_CORNER_TR_FILLED,
	WALL_TOP_CORNER_BL_FILLED,
	WALL_TOP_CORNER_BR_FILLED,
	WALL_TOP_DIAG_TL_DARK,
	WALL_TOP_DIAG_TR_DARK,
	WALL_TOP_DIAG_BL_DARK,
	WALL_TOP_DIAG_BR_DARK,
	WALL_TOP_EDGE_T_DARK,
	WALL_TOP_EDGE_B_DARK,
	WALL_TOP_EDGE_L_DARK,
	WALL_TOP_EDGE_R_DARK,
	WALL_TOP_CORNER_TL_DARK,
	WALL_TOP_CORNER_TR_DARK,
	WALL_TOP_CORNER_BL_DARK,
	WALL_TOP_CORNER_BR_DARK,
	WALL_TOP_INNER_TL,
	WALL_TOP_INNER_TR,
	WALL_TOP_INNER_BL,
	WALL_TOP_INNER_BR,
	WALL_MID_DIAG_BL,
	WALL_MID_FULL,
	WALL_MID_DIAG_BR,
	WALL_MID_DIAG_BL_TRANSP,
	WALL_MID_DIAG_BL_WATER,
	WALL_MID_DIAG_BR_TRANSP,
	WALL_MID_DIAG_BR_WATER,
	WALL_MID_FULL_LEFT,
	WALL_MID_FULL_RIGHT,
	WALL_BASE_DIAG_BL_FULL,
	WALL_BASE_DIAG_BL_HALF_WATER,
	WALL_BASE_DIAG_BL_FULL_WATER,
	WALL_BASE_FULL_LEFT,
	WALL_BASE_FULL,
	WALL_BASE_FULL_RIGHT,
	WALL_BASE_HALF_WATER,
	WALL_BASE_DIAG_BR_FULL,
	WALL_BASE_DIAG_BR_HALF_WATER,
	WALL_BASE_DIAG_BR_FULL_WATER,
	WALL_VOID_DIAG_BL,
	WALL_VOID,
	WALL_VOID_DIAG_BR,
	WALL_VOID_GRAD_DIAG_BL,
	WALL_VOID_GRAD_DIAG_BR,
	WALL_VOID_GRAD_DIAG_BL_EDGE,
	WALL_VOID_GRAD_DIAG_BR_EDGE,
	WALL_SHADOW_FULL,
	WALL_SHADOW_HALF,
	WALL_SHADOW_T,
	WALL_SHADOW_B,
	WALL_BASE_VOID_CORNER_BL,
	WALL_BASE_VOID_CORNER_BR,
	WALL_FULL_STAIR,
	WALL_WATER_STAIR,
	WALL_TOP_STAIR

} WallsType;

typedef enum
{
	CLD_NO_COLL = 0, /// No collisions

	/// Bits 0-3 handle collision box orientation
	CLD_LEFT = 1 << 0,
	CLD_RIGHT = 1 << 1,
	CLD_TOP = 1 << 2,
	CLD_BOTTOM = 1 << 3,

	/// Bits 4-8 handle collision box geometry
	CLD_BOX = 1 << 4,
	CLD_DIAG = 1 << 5,
	CLD_CORN = 1 << 6,
	CLD_HALF = 1 << 7,
	CLD_ANGLE = 1 << 8,
	CLD_S_DIAG = 1 << 9,
	CLD_S_TRI = 1 << 10,

	CLD_DIAG_TOP_LEFT = CLD_DIAG | CLD_TOP | CLD_LEFT,
	CLD_DIAG_TOP_RIGHT = CLD_DIAG | CLD_TOP | CLD_RIGHT,
	CLD_DIAG_BOTTOM_LEFT = CLD_DIAG | CLD_BOTTOM | CLD_LEFT,
	CLD_DIAG_BOTTOM_RIGHT = CLD_DIAG | CLD_BOTTOM | CLD_RIGHT,
	CLD_CORN_TOP_LEFT = CLD_CORN | CLD_TOP | CLD_LEFT,
	CLD_CORN_TOP_RIGHT = CLD_CORN | CLD_TOP | CLD_RIGHT,
	CLD_CORN_BOTTOM_LEFT = CLD_CORN | CLD_BOTTOM | CLD_LEFT,
	CLD_CORN_BOTTOM_RIGHT = CLD_CORN | CLD_BOTTOM | CLD_RIGHT,
	CLD_HALF_LEFT = CLD_HALF | CLD_LEFT,
	CLD_HALF_RIGHT = CLD_HALF | CLD_RIGHT,
	CLD_HALF_TOP = CLD_HALF | CLD_TOP,
	CLD_HALF_BOTTOM = CLD_HALF | CLD_BOTTOM,

	CLD_ANGLE_TOP_LEFT = CLD_ANGLE | CLD_TOP | CLD_LEFT,
	CLD_ANGLE_TOP_RIGHT = CLD_ANGLE | CLD_TOP | CLD_RIGHT,
	CLD_ANGLE_BOTTOM_RIGHT = CLD_ANGLE | CLD_BOTTOM | CLD_LEFT,
	CLD_ANGLE_BOTTOM_LEFT = CLD_ANGLE | CLD_BOTTOM | CLD_RIGHT,

	CLD_SMALL_DIAG_TOP_LEFT = CLD_S_DIAG | CLD_LEFT,
	CLD_SMALL_DIAG_TOP_RIGHT = CLD_S_DIAG | CLD_RIGHT,

	CLD_SMALL_TRI_TL = CLD_S_TRI | CLD_TOP | CLD_LEFT,
	CLD_SMALL_TRI_TR = CLD_S_TRI | CLD_TOP | CLD_RIGHT,
	CLD_SMALL_TRI_BR = CLD_S_TRI | CLD_BOTTOM | CLD_RIGHT,
	CLD_SMALL_TRI_BL = CLD_S_TRI | CLD_BOTTOM | CLD_LEFT,

	CLD_VOID = 1100

} CollisionType;

typedef enum {
	PROPS_TYPE_NO_PROPS,
	PROPS_TYPE_GENERAL,
	PROPS_TYPE_FOREST,
	PROPS_TYPE_MOUTAIN,
	PROPS_TYPE_DUNGEON,
	PROPS_TYPE_BEACH
}PropsType;

typedef enum {
	PROPS_FOREST_TREE_1,
	PROPS_FOREST_TREE_2,
	PROPS_FOREST_TREE_3,
	PROPS_FOREST_TREE_4,
	PROPS_FOREST_TREE_5,
	PROPS_FOREST_TREE_6,
	PROPS_FOREST_TREE_SPIDER,
	PROPS_FOREST_TREE_ANIM,
	PROPS_FOREST_TRUNK_1,
	PROPS_FOREST_TRUNK_2,
	PROPS_FOREST_TRUNK_3,
	PROPS_FOREST_BIG_TRUNK_1,
	PROPS_FOREST_BIG_TRUNK_2,
	PROPS_FOREST_BIG_TRUNK_3,
	PROPS_FOREST_STRAIN_1,
	PROPS_FOREST_STRAIN_2,
	PROPS_FOREST_STRAIN_3,
	PROPS_FOREST_STICK,
	PROPS_FOREST_GRASS_1,
	PROPS_FOREST_GRASS_2,
	PROPS_FOREST_GRASS_3,
	PROPS_FOREST_CUTED_GRASS_1,
	PROPS_FOREST_CUTED_GRASS_2,
	PROPS_FOREST_CUTED_GRASS_3,
	PROPS_FOREST_SPLASH,
	PROPS_FOREST_WATERFALL
}PropsForest;

typedef enum {
	PROPS_MOUTAIN_BIG_CRISTAL_1,
	PROPS_MOUTAIN_BIG_CRISTAL_2,
	PROPS_MOUTAIN_BIG_CRISTAL_3,
	PROPS_MOUTAIN_BIG_CRISTAL_4,
	PROPS_MOUTAIN_BIG_CRISTAL_5,
	PROPS_MOUTAIN_BIG_CRISTAL_6,
	PROPS_MOUTAIN_BIG_CRISTAL_7,
	PROPS_MOUTAIN_LITTLE_CRISTAL_1,
	PROPS_MOUTAIN_LITTLE_CRISTAL_2,
	PROPS_MOUTAIN_LITTLE_CRISTAL_3,
	PROPS_MOUTAIN_LITTLE_CRISTAL_4,
	PROPS_MOUTAIN_ROCK_CRISTAL_1,
	PROPS_MOUTAIN_ROCK_CRISTAL_2,
	PROPS_MOUTAIN_ROCK_CRISTAL_3,
	PROPS_MOUTAIN_ROCK_CRISTAL_4,
	PROPS_MOUTAIN_ROCK_CRISTAL_5,
	PROPS_MOUTAIN_ROCK_1,
	PROPS_MOUTAIN_ROCK_2,
	PROPS_MOUTAIN_ROCK_3,
	PROPS_MOUTAIN_ROCK_4,
	PROPS_MOUTAIN_PLANT_1,
	PROPS_MOUTAIN_PLANT_2,
	PROPS_MOUTAIN_PLANT_3,
	PROPS_MOUTAIN_BUSH_1,
	PROPS_MOUTAIN_BUSH_2,
	PROPS_MOUTAIN_TRUNK,
	PROPS_MOUTAIN_DOOR_FRAME,
	PROPS_MOUTAIN_DOOR_FRAME_TOP,
	PROPS_MOUTAIN_DOOR_SHADOW,
	PROPS_MOUTAIN_DOOR_SHADOW_TOP,
	PROPS_MOUTAIN_DOOR_LIGHT,
	PROPS_MOUTAIN_DOOR_LIGHT_TOP
}PropsMoutain;

typedef enum {
	PROPS_DUNGEON_DOOR_TOP,
	PROPS_DUNGEON_DOOR,
	PROPS_DUNGEON_CLOSED_DOOR_TOP,
	PROPS_DUNGEON_CLOSED_DOOR,

	PROPS_DUNGEON_POT_1,
	PROPS_DUNGEON_POT_2,
	PROPS_DUNGEON_MOVING_PLATEFORM,

	PROPS_DUNGEON_MOVING_WALL_TOP,
	PROPS_DUNGEON_MOVING_WALL_MID,
	PROPS_DUNGEON_FLYER_BRIDGE_LEFT,
	PROPS_DUNGEON_FLYER_BRIDGE_RIGHT,
	PROPS_DUNGEON_FLYER_BRIDGE_TOP,
	PROPS_DUNGEON_FLYER_BRIDGE_BOT,
	PROPS_DUNGEON_LILYPAD,
	PROPS_DUNGEON_BOULDER,
	PROPS_DUNGEON_TRENCH_CORNER_TL,
	PROPS_DUNGEON_TRENCH_TOP_EDGE,
	PROPS_DUNGEON_TRENCH_CORNER_TR,
	PROPS_DUNGEON_TRENCH_LEFT_EDGE,
	PROPS_DUNGEON_TRENCH_RIGHT_EDGE,
	PROPS_DUNGEON_TRENCH_CORNER_BL,
	PROPS_DUNGEON_TRENCH_BOTTOM_EDGE,
	PROPS_DUNGEON_TRENCH_CORNER_BR,
	PROPS_DUNGEON_TRENCH_VB_END,
	PROPS_DUNGEON_TRENCH_VT_END,
	PROPS_DUNGEON_TRENCH_HL_END,
	PROPS_DUNGEON_TRENCH_HR_END,
	PROPS_DUNGEON_BOULDER_BTN,
	PROPS_DUNGEON_HINT_STATUE,
	PROPS_DUNGEON_STATUE_BASE_TORSO,
	PROPS_DUNGEON_STATUE_BASE_HEAD,
	PROPS_DUNGEON_STATUE_HALF_HEAD,
	PROPS_DUNGEON_STATUE_HALF_TORSO,
	PROPS_DUNGEON_STATUE_FULL,
	PROPS_DUNGEON_STAND,
	PROPS_DUNGEON_PAVING_STONE,

	PROPS_DUNGEON_BUILD_JOY,
	PROPS_DUNGEON_OUTLINE_DOOR_JOY,
	PROPS_DUNGEON_DOOR_JOY,
	PROPS_DUNGEON_ENTRANCE_JOY,
	PROPS_DUNGEON_ENTRANCE_PRESSURE_PLATE_JOY,
	PROPS_DUNGEON_BUILD_FRONT_SADNESS,
	PROPS_DUNGEON_BUILD_BACK_SADNESS,
	PROPS_DUNGEON_OUTLINE_DOOR_SADNESS,
	PROPS_DUNGEON_DOOR_SADNESS,
	PROPS_DUNGEON_ENTRANCE_SADNESS,
	PROPS_DUNGEON_ENTRANCE_PRESSURE_PLATE_SADNESS,
	PROPS_DUNGEON_BUILD_FRONT_FEAR,
	PROPS_DUNGEON_BUILD_BACK_FEAR,
	PROPS_DUNGEON_OUTLINE_DOOR_FEAR,
	PROPS_DUNGEON_DOOR_FEAR,
	PROPS_DUNGEON_ENTRANCE_FEAR,
	PROPS_DUNGEON_ENTRANCE_PRESSURE_PLATE_FEAR,
	PROPS_DUNGEON_BUILD_FRONT_ANGRY,
	PROPS_DUNGEON_BUILD_BACK_ANGRY,
	PROPS_DUNGEON_OUTLINE_DOOR_ANGRY,
	PROPS_DUNGEON_DOOR_ANGRY,
	PROPS_DUNGEON_ENTRANCE_ANGRY,
	PROPS_DUNGEON_ENTRANCE_PRESSURE_PLATE_ANGRY,

	PROPS_DUNGEON_PILLAR_TOP_1,
	PROPS_DUNGEON_PILLAR_TOP_2,
	PROPS_DUNGEON_PILLAR_LEFT_1,
	PROPS_DUNGEON_PILLAR_MID_1,
	PROPS_DUNGEON_PILLAR_RIGHT_1,
	PROPS_DUNGEON_PILLAR_MID_2,
	PROPS_DUNGEON_PILLAR_MID_3,
	PROPS_DUNGEON_PILLAR_MID_4,
	PROPS_DUNGEON_PILLAR_MID_5,
	PROPS_DUNGEON_PILLAR_MID_6,
	PROPS_DUNGEON_PILLAR_BOTTOM_1,
	PROPS_DUNGEON_PILLAR_BOTTOM_2,
	PROPS_DUNGEON_PILLAR_LEFT_2,
	PROPS_DUNGEON_PILLAR_MID_7,
	PROPS_DUNGEON_PILLAR_RIGHT_2,
	PROPS_DUNGEON_PRESSURE_PLATE,
	PROPS_DUNGEON_BOSS_DOOR_TOP,
	PROPS_DUNGEON_BOSS_DOOR,
	PROPS_DUNGEON_BRAZIER,
	PROPS_DUNGEON_BANNER,
	PROPS_DUNGEON_LITTLE_CHEST,
	PROPS_DUNGEON_BIG_CHEST
}PropsDungeon;

typedef enum {
	PROPS_GENERAL_FOUNTAIN,
	PROPS_GENERAL_STONE_1,
	PROPS_GENERAL_STONE_2,
	PROPS_GENERAL_STONE_3,
	PROPS_GENERAL_STONE_4,
	PROPS_GENERAL_STONE_5,
	PROPS_GENERAL_STONE_6,
	PROPS_GENERAL_STONE_7,
	PROPS_GENERAL_STONE_8,
	PROPS_GENERAL_STONE_9,
	PROPS_GENERAL_STONE_10,
	PROPS_GENERAL_STONE_11,
	PROPS_GENERAL_SIGN,
	PROPS_GENERAL_FLOWER,
	PROPS_GENERAL_TENT_ARNOLD,
	PROPS_GENERAL_POOL,
	PROPS_GENERAL_POOL_LILYPAD,
	PROPS_GENERAL_FISH,
	PROPS_GENERAL_BRIDGE,
	PROPS_GENERAL_TENT_SHAMAN,
	PROPS_GENERAL_FISHING_SPOT,
	PROPS_GENERAL_BUBBLES,
	PROPS_GENERAL_GROUND_NOTES,
	PROPS_GENERAL_FLOATING_NOTES
}PropsGeneral;

typedef enum {
	PROPS_BEACH_PALM_1,
	PROPS_BEACH_PALM_2,
	PROPS_BEACH_TRUNK_1,
	PROPS_BEACH_TRUNK_2,
	PROPS_BEACH_DEAD_CORAL_1,
	PROPS_BEACH_DEAD_CORAL_2,
	PROPS_BEACH_DEAD_CORAL_3,
	PROPS_BEACH_DEAD_CORAL_4,
	PROPS_BEACH_STONE_1,
	PROPS_BEACH_STONE_2,
	PROPS_BEACH_STAR_1,
	PROPS_BEACH_STAR_2,
	PROPS_BEACH_CORAL_1,
	PROPS_BEACH_CORAL_2,
	PROPS_BEACH_CORAL_3,
	PROPS_BEACH_CORAL_4,
	PROPS_BEACH_CORAL_5,
	PROPS_BEACH_CORAL_6,
	PROPS_BEACH_CORAL_7,
	PROPS_BEACH_CORAL_8,
	PROPS_BEACH_ROCK_1,
	PROPS_BEACH_ROCK_2,
	PROPS_BEACH_ROCK_3,
	PROPS_BEACH_ROCK_4,
	PROPS_BEACH_ROCK_5
}PropsBeach;

typedef enum {
	LAYER_GROUND_FIRST,
	LAYER_GROUND_SECOND,
	LAYER_WALLS_FIRST,
	LAYER_WALLS_SECOND,
	LAYER_PROPS
}Layers;

typedef enum {
	MAP_STATE_NO_MAP,
	MAP_STATE_OVERWORLD,
	MAP_STATE_FOREST,
	MAP_STATE_LIGHTHOUSE_BEDROOM,
	MAP_STATE_LIGHTHOUSE_HALL,
	MAP_STATE_LIGHTHOUSE_OUTSIDE,
	MAP_STATE_LIGHTHOUSE_MENTOR,
	MAP_STATE_LIGHTHOUSE_ANTECHAMBER,
	MAP_STATE_LIGHTHOUSE_ROOF,
	MAP_STATE_DUNGEON_JOY_1 = 10,
	MAP_STATE_DUNGEON_JOY_2,
	MAP_STATE_DUNGEON_JOY_3,
	MAP_STATE_DUNGEON_JOY_4,
	MAP_STATE_DUNGEON_JOY_5,
	MAP_STATE_DUNGEON_JOY_6,
	MAP_STATE_DUNGEON_JOY_BOSS,
	MAP_STATE_DUNGEON_JOY_FOUNTAIN,
	MAP_STATE_CAVE,
	MAP_STATE_MOUTAIN,
	MAP_STATE_BEACH,
	MAP_STATE_DUNGEON_SADNESS_1,
	MAP_STATE_DUNGEON_SADNESS_2,
	MAP_STATE_DUNGEON_SADNESS_BOSS
}MapState;

/// <summary>
/// struct for map
/// </summary>
typedef struct {
	TilesType type;
	char variant;
} LayerGround;

typedef struct {
	WallsType type;
	char variant;
} LayerWalls;

typedef struct {
	PropsType type;
	union {
		PropsForest forest;
		PropsMoutain moutain;
		PropsDungeon dungeon;
		PropsGeneral general;
		PropsBeach beach;
	};
	sfVector2f position;
	sfBool isBehind;
	int frameX;
	float timerAnim;
} LayerProps;

typedef struct {
	CollisionType type;
	sfVector2f pos;
} LayerCollision;

typedef struct {
	sfVector2i mapSize;
	sfBool isAlloc;
	/// Layer 1 = Ground
	LayerGround** layerGround_first;
	LayerGround** layerGround_second;
	/// Layer 2 = Walls
	LayerWalls** layerWalls_first;
	LayerWalls** layerWalls_second;
	/// Layer 3 = collision 
	LayerCollision** layerCollision;
	/// Layer 4 = Props
	LayerProps* layerProps;
}Map;

Map map;

static Biomes currentBiome = BIOME_ALL;

static MapState mapState = MAP_STATE_OVERWORLD;
int currentMap;

/// <summary>
/// map variable initialisation function
/// </summary>
/// <param name="_window"></param>
void map_InitMap(Window* _window);
/// <summary>
/// function for interactions with the map
/// </summary>
/// <param name="_window"></param>
void map_UpdateMap(Window* _window);
/// <summary>
/// function of the map visual behind the player
/// </summary>
/// <param name="_window"></param>
void map_DisplayMapBehindPlayer(Window* _window);
/// <summary>
/// function of the map visual in front the player
/// </summary>
/// <param name="_window"></param>
void map_DisplayMapInFrontOfPlayer(Window* _window);
/// <summary>
/// free memory
/// </summary>
void map_DeinitMap();

/// <summary>
/// function add tile prop
/// </summary>
/// <param name="_type"></param>
/// <param name="_props"></param>
/// <param name="_pos"></param>
/// <param name="_load"></param>
void map_addProps(PropsType _type, int _props, sfVector2f _pos, sfBool _load);

void map_delProps(sfVector2i _pos);
/// <summary>
/// functions for emptying the chained list of props
/// </summary>
void map_clearPropsList();

/// <summary>
/// default map variable initialisation function
/// </summary>
void defaultMap();

/// <summary>
/// Save current map (this function is called in editor state only !)
/// </summary>
void map_SaveMap();
/// <summary>
/// Load map
/// </summary>
/// <param name="_idMap">Index</param>
void map_LoadMap(int _idMap);
/// <summary>
/// functions for saving elements according to their type (map corruption protection)
/// </summary>
void map_SaveEnviro();
void map_SaveProps();
void map_SaveEntity();
/// <summary>
/// function for loading saved map elements according to their type (map corruption protection)
/// </summary>
void map_LoadEnviro();
void map_LoadProps();
void map_LoadEntity();

/// <summary>
/// Allocate memory for the 3 layers according to a size
/// </summary>
/// <param name="_mapSize"></param>
void map_CallocMap(sfVector2i _mapSize);

/// Destroys all resources allocated for the map.
void map_FreeMap();

/// <summary>
/// This function use read and fill the layers or get and write the three layers on a file
/// </summary>
/// <param name="_file">File to read or write</param>
/// <param name="_fill">sfTrue : use fread and fill the 3 layers
/// sfFalse : use fwrite get the 3 layers and write it</param>
void map_FillOrGetLayer(FILE* _file, sfBool _fill);

/// Checks if the given tile index corresponds to a collider and returns its type.
CollisionType map_HasTileCollisions(sfVector2i _index);

/// Checks if the given tile index for the snake corresponds to a collider and returns its type.
CollisionType map_SnakeHasTileCollisions(sfVector2i _index);

/// <summary>
/// This function checks whether an index exists in the array.
/// Usefull for avoiding crashes
/// </summary>
/// <param name="_index">Index</param>
/// <returns>sfTrue if the index exist, sfFalse otherwise</returns>
sfBool map_isIndexInArray(sfVector2i _index);
sfBool map_isIndexInArraySwapped(sfVector2i _index);

/// <summary>
/// This function transforms a world position into the array index.
/// Warning : Does not check if the index exists -> use map_isIndexInArray() if needed
/// </summary>
/// <param name="_pos">World Pos</param>
/// <returns>Index</returns>
sfVector2i map_getBlockPos(sfVector2f _pos);

/// <summary>
/// This function transforms an index into the world position
/// </summary>
/// <param name="_blockPos">Index</param>
/// <returns>World Pos at top left corner</returns>
sfVector2f map_getWorldPos(sfVector2i _blockPos);

/// Returns the texture rect corresponding to the tile at the given coords on the tileset.
sfIntRect map_GetTilesetRect(int _x, int _y);

/// <summary>
/// rect size recovery function for tiles
/// </summary>
/// <param name="_tile"></param>
/// <returns></returns>
sfIntRect map_GetTileTexRect(LayerGround _tile);
/// <summary>
/// rect size recovery function for walls
/// </summary>
/// <param name="_wall"></param>
/// <returns></returns>
sfIntRect map_GetWallTexRect(LayerWalls _wall); 
/// <summary>
/// rect size recovery function for props
/// </summary>
/// <param name="_props"></param>
/// <returns></returns>
sfIntRect map_GetPropsTexRect(LayerProps _props);

/// Snaps the given position onto the tile grid.
sfVector2f map_SnapToGrid(sfVector2f _pos);

///////////////////////////////////////////////
/// <summary>
/// pas compris
/// </summary>
/// <param name="_window"></param>
/// <param name="_state"></param>
void map_changeMapState(Window* _window, MapState _state);
///////////////////////////////////////////////

/// <summary>
/// map state recovery function
/// </summary>
/// <returns></returns>
MapState map_GetState();

/// <summary>
/// function for retrieving the chained list of props
/// </summary>
/// <returns></returns>
LayerProps* map_getPropsListToSave();
/// <summary>
/// changing the onepass value
/// </summary>
/// <param name="_value"></param>
void map_setOnePassMap(sfBool _value);
/// <summary>
/// function that lets you know if the enemy can fall
/// </summary>
/// <param name="_pos"></param>
/// <returns></returns>
sfBool map_CanFall(sfVector2f _pos);
/// <summary>
/// recovery of all the textures in a biome
/// </summary>
/// <param name="_layer"></param>
/// <param name="i"></param>
/// <param name="j"></param>
/// <param name="_props"></param>
/// <returns></returns>
sfTexture* map_getBiomeTexture(Layers _layer, int i, int j, LayerProps* _props);
/// <summary>
/// change of biome and change of all textures according to the changed biome
/// </summary>
/// <param name="_biome"></param>
void setCurrentBiome(Biomes _biome);
/// <summary>
/// recovery of the current biome
/// </summary>
/// <returns></returns>
Biomes getCurrentBiome();

void waterfallAnimator(sfIntRect* _rect, int _nbFrameX, int _nbFrameY, float _timeAnim, float* _timer);

sfBool map_IsPlayerInRealVoid(sfVector2i _pos);

int checkPlayerWalkType(sfVector2i _pos);
sfBool checkBoxVoidCollision(sfVector2i _pos);

sfBool map_IsIndexInWater(sfVector2i _index);

void fillTransitionZone();

sfVector2f map_getPlayerSpawnPos();

sfBool getIsFreeingMap();

void removeCollision(sfVector2i _index);

void mapReset(Window* _window);

stdList* map_GetPropsList();

void setFirstPassPotSpawn();

void map_setPlayerPos(MapState _state);