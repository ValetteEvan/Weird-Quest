#pragma once
#include "tools.h"
#include "stateManager.h"

typedef enum {
	BIOME_NO_BIOME,
	BIOME_ALL,
	BIOME_FOREST,
	BIOME_CAVE,
	BIOME_MOUNTAIN,
	BIOME_BEACH,
	BIOME_GRASSLAND,
	BIOME_JOY_DUNGEON,
	BIOME_SADNESS_DUNGEON,
	BIOME_FEAR_DUNGEON,
	BIOME_ANGRY_DUNGEON,
	BIOME_LIGHTHOUSE
}Biomes;

typedef enum 
{
	SINGLE, SPRITESHEET, ANIMATION, SOUNDFX, MUSIC, FONT, VIDEO, SHADER
}ressourceType;

typedef struct ressources ressources;
struct ressources
{
	ressources* pNext;
	State state;
	ressourceType type;
	Biomes biome;
	char name[30];
	char path[200];
};

ressources* ResBegin;

/// <summary>
/// load ressources of a state
/// </summary>
/// <param name="_path">path of the file with all the ressources path in it</param>
void OnLoad(char* _path);

/// <summary>
/// add a ressource
/// </summary>
/// <param name="_ressources">ressource to add</param>
void AddRessources(ressources* _ressources);

/// <summary>
/// delete a ressource
/// </summary>
/// <param name="_ressources">ressource to delete</param>
/// <returns></returns>
ressources* RemoveRessources(ressources* _ressources);
