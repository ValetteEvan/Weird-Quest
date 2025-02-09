#pragma once
#include "tools.h"
#include "windowManager.h"
#include "mouseManager.h"
#include "viewManager.h"
#include "soundManager.h"
#include "pause.h"
#include "map.h"
#include "editorHUD.h"
#include "List.h"
#include "npc.h"

/// <summary>
/// editor variable initialisation function
/// </summary>
/// <param name="_window"></param>
void editor_Init(Window* _window);
/// <summary>
/// function for interactions with the editor
/// </summary>
/// <param name="_window"></param>
void editor_Update(Window* _window);
/// <summary>
/// function with the visual editor
/// </summary>
/// <param name="_window"></param>
void editor_Display(Window* _window);
/// <summary>
/// free memory 
/// </summary>
void editor_Deinit();

/// <summary>
/// This function is called in editor state only !
/// It used to update in real time the map when you changes tiles
/// </summary>
void editor_UpdateMap();

/// <summary>
/// map size management
/// </summary>
/// <param name="_window"></param>
void editor_ResizeMap(Window* _window);
/// <summary>
/// creating new maps, navigating between existing maps and saving maps
/// </summary>
/// <param name="_window"></param>
void editor_MapHandler(Window* _window);
/// <summary>
/// changing the tile to be placed
/// </summary>
/// <param name="_window"></param>
void editor_ChangeTile(Window* _window);
/// <summary>
/// changing the size of the tile placement brush
/// </summary>
/// <param name="_window"></param>
void editor_ChangeBrush(Window* _window);

/// <summary>
/// function for adding a line to the map size
/// </summary>
void editor_AddRow();
/// <summary>
/// line deletion function at map size
/// </summary>
void editor_RemoveRow();
/// <summary>
/// function for adding a column to the map size
/// </summary>
void editor_AddColumn();
/// <summary>
/// column deletion function at map size
/// </summary>
void editor_RemoveColumn();
/// <summary>
/// placing the tile in the map
/// </summary>
/// <param name="_x">index X tab</param>
/// <param name="_y">index Y tab</param>
/// <param name="_brushSize">brush size</param>
void editor_PlaceTile(int _x, int _y, int _brushSize);

/// <summary>
/// functions for retrieving and modifying specific variables
/// </summary>
/// <param name="_val"></param>
void setGroundMaterialValue(int _val);
int getGroundMaterialValue();

void setWallMaterialValue(int _val);
int getWallMaterialValue();

TilesType getEditorCurrentTile();
void setEditorCurrentTile(int _val);

void setEditorBrushSize(int _val);
int getEditorBrushSize();

void setEditorIndexMap(int _val);
int getEditorIndexMap();

/// <summary>
/// recovering the collision id
/// </summary>
/// <param name="_type"></param>
/// <returns></returns>
int editor_CldToId(CollisionType _type);
/// <summary>
/// collision recovery via id
/// </summary>
/// <param name="_id">id de la collision voulu</param>
/// <returns></returns>
CollisionType editor_IdToCld(int _id);
/// <summary>
/// random tile placement management based on available variation
/// </summary>
/// <param name="_coords"></param>
/// <param name="_type"></param>
/// <returns></returns>
char editor_SelectVariant(sfVector2i _coords, TilesType _type);
/// <summary>
/// changing the weight of each tile to affect the randomness of tile placement
/// </summary>
/// <param name="_val1">variante 1</param>
/// <param name="_val2">variante 2</param>
/// <param name="_val3">variante 3</param>
/// <param name="_val4">variante 4</param>
void setEditorValueRateTiles(int _val1, int _val2, int _val3, int _val4);
/// <summary>
/// functions for recovering specific variables
/// </summary>
int getEditorValueRateTile1();
int getEditorValueRateTile2();
int getEditorValueRateTile3();
int getEditorValueRateTile4();
void setLightOrShadow(sfBool _val);
sfBool getLightOrShadow();

/// <summary>/// 
/// Take the mouse pos and set the selected wall according to the mouse pos
/// </summary>
/// <param name="_mouse"></param>
void pickWallsTiles(sfVector2f _mouse);