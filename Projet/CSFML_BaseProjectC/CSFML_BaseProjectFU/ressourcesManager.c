#include "ressourcesManager.h"

void OnLoad(char* _path)
{
	FILE* file;
	char tmpligne[200];
	char tmp[50];
	char tmpname[30], tmppath[100];
	State state = RTNULL;
	Biomes biome = BIOME_NO_BIOME;
	ResBegin = 0;

	if (!(file = fopen(_path, "r")))
	{
	//	printf("Unable to access file : %s", _path);
		exit(1);
	}
	while (fgets(tmpligne, 200, file) != NULL) // On lit le fichier tant qu'on ne reçoit pas d'erreur (NULL)
	{
		sscanf(tmpligne, "%s ", tmp);

		if (strcmp(tmp, "[end]") == 0) state = RTNULL;

		if ((strcmp(tmp, "[biomeAll]")) == 0) biome = BIOME_ALL;
		else if ((strcmp(tmp, "[biomeForest]")) == 0) biome = BIOME_FOREST;
		else if ((strcmp(tmp, "[biomeCave]")) == 0) biome = BIOME_CAVE;
		else if ((strcmp(tmp, "[biomeMountain]")) == 0) biome = BIOME_MOUNTAIN;
		else if ((strcmp(tmp, "[biomeBeach]")) == 0) biome = BIOME_BEACH;
		else if ((strcmp(tmp, "[biomeGrassland]")) == 0) biome = BIOME_GRASSLAND;
		else if ((strcmp(tmp, "[biomeJoyDungeon]")) == 0) biome = BIOME_JOY_DUNGEON;
		else if ((strcmp(tmp, "[biomeSadnessDungeon]")) == 0) biome = BIOME_SADNESS_DUNGEON;
		else if ((strcmp(tmp, "[biomeFearDungeon]")) == 0) biome = BIOME_FEAR_DUNGEON;
		else if ((strcmp(tmp, "[biomeAngryDungeon]")) == 0) biome = BIOME_ANGRY_DUNGEON;
		else if ((strcmp(tmp, "[biomeLighthouse]")) == 0) biome = BIOME_LIGHTHOUSE; 
		else if ((strcmp(tmp, "[endBiome]")) == 0) biome = BIOME_NO_BIOME;

		if (state == RTNULL)
		{
			if ((strcmp(tmp, "[all]")) == 0) state = ALL;
			else if ((strcmp(tmp, "[intro]")) == 0) state = INTRO;
			else if ((strcmp(tmp, "[menu]")) == 0) state = MENU;
			else if ((strcmp(tmp, "[game]")) == 0) state = GAME;
			else if ((strcmp(tmp, "[ending]")) == 0) state = END;
		}
		else if (state != RTNULL)
		{ 
			strcpy(tmp, "");
			strcpy(tmpname, "");
			strcpy(tmppath, "");
			sscanf(tmpligne, "%s %s %s", tmp, tmpname, tmppath);

			ressources* tempRes = (ressources*)calloc(1, sizeof(ressources));
			tempRes->state = state;
			tempRes->biome = biome;
			strcpy(tempRes->name, tmpname);
			strcpy(tempRes->path, tmppath);
			if ((strcmp(tmp, "single")) == 0) tempRes->type = SINGLE;
			else if ((strcmp(tmp, "spritesheet")) == 0) tempRes->type = SPRITESHEET;
			else if ((strcmp(tmp, "animation")) == 0) tempRes->type = ANIMATION;
			else if ((strcmp(tmp, "soundfx")) == 0) tempRes->type = SOUNDFX;
			else if ((strcmp(tmp, "music")) == 0) tempRes->type = MUSIC;
			else if ((strcmp(tmp, "font")) == 0) tempRes->type = FONT;
			else if ((strcmp(tmp, "video")) == 0) tempRes->type = VIDEO;
			else if ((strcmp(tmp, "shader")) == 0) tempRes->type = SHADER;
			else continue;
			AddRessources(tempRes);
		}
	}
	fclose(file);
}

void AddRessources(ressources* _ressources)
{
	_ressources->pNext = ResBegin;
	ResBegin = _ressources;
}

ressources* RemoveRessources(ressources* _ressources)
{
	if (_ressources == ResBegin)
	{
		ressources* tempRes = _ressources->pNext;
		ResBegin = tempRes;
		free(_ressources);
		return tempRes;
	}
	else
	{
		ressources* tempRes = ResBegin;
		while (tempRes->pNext != _ressources)
		{
			tempRes = tempRes->pNext;
		}
		tempRes->pNext = _ressources->pNext;
		free(_ressources);
		return tempRes->pNext;
	}
}