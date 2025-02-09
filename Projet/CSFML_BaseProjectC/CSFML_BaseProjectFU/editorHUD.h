#pragma once
#include "tools.h"
#include "windowManager.h"
#include "fontManager.h"
#include "mouseManager.h"
#include "viewManager.h"
#include "map.h"
#include "enemies.h"

/// <summary>
/// Enums for editorHUD
/// </summary>
typedef enum {
	EDITOR_STATE_TILES,
	EDITOR_STATE_TILES_HUD,
	EDITOR_STATE_ENTITY,
	EDITOR_STATE_PROPS,
	EDITOR_STATE_EFFECTS,
	EDITOR_STATE_CFGEDITOR
}EditorState;

typedef enum {
	EDITOR_LAYER_GROUND,
	EDITOR_LAYER_WALLS,
	EDITOR_LAYER_COLLISION
}EditorTilesLayer;

typedef enum {
	EDITOR_ENTITY_PLAYER,
	EDITOR_ENTITY_PNJ,
	EDITOR_ENTITY_MOBS
}EditorEntity;

typedef enum
{
	EDITOR_PNJ_ARNOLD = 1,
	EDITOR_PNJ_MENTOR = 3,
	EDITOR_PNJ_SHAMAN,
	EDITOR_STATUS
}NpcId;

typedef enum {
	EDITOR_EFFECTS_LIGHT = 1,
	EDITOR_EFFECTS_PARTICLES
}EditorEffects;

typedef enum {
	EDITOR_LIGHT_CIRCLE,
	EDITOR_LIGHT_RECTANGLE,
	EDITOR_LIGHT_CONE
}EditorLightForm;

typedef enum {
	EDITOR_GROUND_SAND,
	EDITOR_GROUND_ROCK,
	EDITOR_GROUND_GRASS,
	EDITOR_GROUND_WATER,
	EDITOR_GROUND_DIRT,
	EDITOR_GROUND_DUNGEON,
	EDITOR_GROUND_LEAF
}EditorGroundType;

typedef enum {
	EDITOR_WALL_INTERIOR,
	EDITOR_WALL_EXTERIOR
}EditorWallType;

/// <summary>
/// editorHUD variable initialisation function
/// </summary>
/// <returns></returns>
void initEditorHUD(Window* _window);
/// <summary>
/// function for interactions with the editorHUD
/// </summary>
/// <returns></returns>
void updateEditorHUD(Window* _window, float _dt);
/// <summary>
/// function with the editorHUD visual
/// </summary>
void displayEditorHUD(Window* _window);
/// <summary>
/// free memory
/// </summary>
void deinitHUD(Window* _window);
/// <summary>
/// function for changing between the first and second layer of the editor
/// </summary>
/// <param name="_state"></param>
void editorHud_ChangeLayer(Window* _window);

/// <summary>
/// functions for retrieving and modifying specific variables
/// </summary>
EditorState getEditorState();
void setEditorState(EditorState _val);
EditorTilesLayer getEditorTilesLayer();
EditorEntity getEditorEntityState();
EditorEffects getEditorEffectsState();
NpcId getEditorPnjChoice();
EditorGroundType getEditorGroundType();
sfVector3f getEditorLightColor();
sfVector2f getEditorLightSize();
EditorLightForm getEditorLightForm();
float getEditorConeAngle();
float getEditorConeRotation();
float getEditorlightStrength();
/// <summary>
/// functions for updating poster texts
/// </summary>
void setMapText(char* _txt);
void setMapTextID(char* _txt);
void setMapTextSave(char* _txt);
/// <summary>
/// functions for retrieving and modifying specific variables
/// </summary>
void setWantMapSave(sfBool _state);
sfBool getWantMapSave();
char editorHud_getFirstOrSecondLayer();
void setEditorWallType(EditorWallType _val);
PropsType getCurrentPropsType();
int getCurrentProps();
float getEditorViewSpeed();
EnemiesType getEditorHudMobType(); 
float getEditorHudMobRotation();
void setEditorHudMapEnlargement(int _val); 
int getEditorHudMapEnlargement();