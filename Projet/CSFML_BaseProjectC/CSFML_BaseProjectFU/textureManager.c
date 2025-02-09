#include "textureManager.h"
static sfBool firstPassTexture = sfFalse;


void Texture_Onload(State _state)
{
	if (!firstPassTexture) {
		firstPassTexture = sfTrue;
		textureBegin = 0;
	}

	ressources* tempRessources = ResBegin;
	while (tempRessources != NULL)
	{
		if (tempRessources->biome == BIOME_NO_BIOME) {
			if (tempRessources->type == SINGLE)
			{
				if (tempRessources->state == _state)
				{
					Texture* tempTexture = (Texture*)calloc(1, sizeof(Texture));
					strcpy(tempTexture->name, tempRessources->name);
					tempTexture->type = tempRessources->type;
					tempTexture->state = tempRessources->state;
					tempTexture->biome = tempRessources->biome;

					tempTexture->sftexture = sfTexture_createFromFile(tempRessources->path, NULL);
					tempTexture->compteur = 0;
					AddTexture(tempTexture);
				}
			}
			if (tempRessources->type == SPRITESHEET)
			{
				if (tempRessources->state == _state)
				{
					FILE* file;
					char tmpligne[200];
					char tmp[50];
					char tmpname[30], tmppath[50];
					sfIntRect tmprect = { 0 };
					StateTexture statetex = TEXNULL;
					sfTexture* tmptexture = NULL;
					sfBool firstPass = sfTrue;

					if (!(file = fopen(tempRessources->path, "r")))
					{
						printf("Unable to access file : %s", tempRessources->path);
						exit(1);
					}
					while (fgets(tmpligne, 200, file) != NULL) // On lit le fichier tant qu'on ne reçoit pas d'erreur (NULL)
					{
						sscanf(tmpligne, "%s ", tmp);

						if ((strcmp(tmp, "[end]")) == 0)
						{
							statetex = TEXNULL;
						}
						if (statetex == TEXNULL)
						{
							if ((strcmp(tmp, "[path]")) == 0)
							{
								statetex = PATH;
							}
							else if ((strcmp(tmp, "[config]")) == 0)
							{
								statetex = CONFIG;
							}
						}
						else if (statetex == PATH)
						{
							sscanf(tmpligne, "%s ", tmppath);
						}
						else if (statetex == CONFIG)
						{
							float tmptime;
							int tmpNbFrame, tmpRow;
							sscanf(tmpligne, "%s %d %d %d %d %d %f %d", tmpname, &tmprect.left, &tmprect.top, &tmprect.width, &tmprect.height, &tmpNbFrame, &tmptime, &tmpRow);

							Texture* tempTexture = (Texture*)calloc(1, sizeof(Texture));
							strcpy(tempTexture->name, tmpname);
							tempTexture->spSheetRec = tmprect;
							tempTexture->type = tempRessources->type;
							tempTexture->state = tempRessources->state;
							tempTexture->biome = tempRessources->biome;
							if (firstPass)
							{
								if (!(tempTexture->sftexture = sfTexture_createFromFile(tmppath, NULL)))
									exit(1);
								firstPass = sfFalse;
								tmptexture = tempTexture->sftexture;
							}
							else
							{
								tempTexture->sftexture = tmptexture;
							}
							tempTexture->compteur = 0;
							tempTexture->NbFrame = tmpNbFrame;
							tempTexture->frameTime = tmptime;
							tempTexture->row = tmpRow;
							AddTexture(tempTexture);
						}
					}
					firstPass = sfTrue;
					fclose(file);
				}
			}
			if (tempRessources->type == ANIMATION)
			{
				if (tempRessources->state == _state)
				{
					FILE* file;
					char tmpligne[200];
					char tmp[150];
					char tmpname[100], tmppath[150];
					sfIntRect tmprect = { 0 };
					StateTexture statetex = TEXNULL;
					sfTexture* tmptexture = NULL;
					sfBool firstPass = sfTrue;
					float tmptime;
					int tmpNbFrame, tmpRow;
					int cptNbTexture = 0;

					if (!(file = fopen(tempRessources->path, "r")))
					{
						printf("Unable to access file : %s", tempRessources->path);
						exit(1);
					}
					while (fgets(tmpligne, 200, file) != NULL) // On lit le fichier tant qu'on ne reçoit pas d'erreur (NULL)
					{
						sscanf(tmpligne, "%s ", tmp);
						tmp[49] = 0;

						if ((strcmp(tmp, "[end]")) == 0)
						{
							statetex = TEXNULL;
						}
						if (statetex == TEXNULL)
						{
							if ((strcmp(tmp, "[path]")) == 0)
							{
								statetex = PATH;
							}
							else if ((strcmp(tmp, "[config]")) == 0)
							{
								statetex = CONFIG;
							}
						}
						else if (statetex == PATH)
						{
							sscanf(tmpligne, "%s ", tmppath);
						}
						else if (statetex == CONFIG)
						{
							/// DANGEROUS, FIX THIS ONE
							sscanf(tmpligne, "%s %d %d %d %d %d %f %d", tmpname, &tmprect.left, &tmprect.top, &tmprect.width, &tmprect.height, &tmpNbFrame, &tmptime, &tmpRow);

							Texture* tempTexture = (Texture*)calloc(1, sizeof(Texture));
							if (!tempTexture) continue;
							tmpname[29] = 0;
							strcpy(tempTexture->name, tmpname);
							tempTexture->spSheetRec = tmprect;
							tempTexture->type = tempRessources->type;
							tempTexture->state = tempRessources->state;
							tempTexture->biome = tempRessources->biome;
							if (firstPass)
							{
								if (!(tempTexture->sftexture = sfTexture_createFromFile(tmppath, NULL)))
									exit(1);
								firstPass = sfFalse;
								tmptexture = tempTexture->sftexture;
							}
							else
							{
								tempTexture->sftexture = NULL;
							}
							tempTexture->compteur = cptNbTexture;
							tempTexture->NbFrame = tmpNbFrame;
							tempTexture->frameTime = tmptime;
							tempTexture->row = tmpRow;
							AddTexture(tempTexture);
							cptNbTexture++;
						}
					}
					firstPass = sfTrue;
					fclose(file);
				}
			}
		}
		
		tempRessources = tempRessources->pNext;
	}
}

void BiomeTexture_Onload(State _state, Biomes _biome)
{
	ressources* tempRessources = ResBegin;
	while (tempRessources != NULL)
	{
		if (tempRessources->biome == _biome) {
			if (tempRessources->type == SINGLE)
			{
				if (tempRessources->state == _state)
				{
					Texture* tempTexture = (Texture*)calloc(1, sizeof(Texture));
					strcpy(tempTexture->name, tempRessources->name);
					tempTexture->type = tempRessources->type;
					tempTexture->state = tempRessources->state;
					tempTexture->biome = tempRessources->biome;

					tempTexture->sftexture = sfTexture_createFromFile(tempRessources->path, NULL);
					tempTexture->compteur = 0;
					AddTexture(tempTexture);
				}
			}
			if (tempRessources->type == SPRITESHEET)
			{
				if (tempRessources->state == _state)
				{
					FILE* file;
					char tmpligne[200];
					char tmp[50];
					char tmpname[30], tmppath[50];
					sfIntRect tmprect = { 0 };
					StateTexture statetex = TEXNULL;
					sfTexture* tmptexture = NULL;
					sfBool firstPass = sfTrue;

					if (!(file = fopen(tempRessources->path, "r")))
					{
						printf("Unable to access file : %s", tempRessources->path);
						exit(1);
					}
					while (fgets(tmpligne, 200, file) != NULL) // On lit le fichier tant qu'on ne reçoit pas d'erreur (NULL)
					{
						sscanf(tmpligne, "%s ", tmp);

						if ((strcmp(tmp, "[end]")) == 0)
						{
							statetex = TEXNULL;
						}
						if (statetex == TEXNULL)
						{
							if ((strcmp(tmp, "[path]")) == 0)
							{
								statetex = PATH;
							}
							else if ((strcmp(tmp, "[config]")) == 0)
							{
								statetex = CONFIG;
							}
						}
						else if (statetex == PATH)
						{
							sscanf(tmpligne, "%s ", tmppath);
						}
						else if (statetex == CONFIG)
						{
							float tmptime;
							int tmpNbFrame, tmpRow;
							sscanf(tmpligne, "%s %d %d %d %d %d %f %d", tmpname, &tmprect.left, &tmprect.top, &tmprect.width, &tmprect.height, &tmpNbFrame, &tmptime, &tmpRow);

							Texture* tempTexture = (Texture*)calloc(1, sizeof(Texture));
							strcpy(tempTexture->name, tmpname);
							tempTexture->spSheetRec = tmprect;
							tempTexture->type = tempRessources->type;
							tempTexture->state = tempRessources->state;
							tempTexture->biome = tempRessources->biome;
							if (firstPass)
							{
								if (!(tempTexture->sftexture = sfTexture_createFromFile(tmppath, NULL)))
									exit(1);
								firstPass = sfFalse;
								tmptexture = tempTexture->sftexture;
							}
							else
							{
								tempTexture->sftexture = tmptexture;
							}
							tempTexture->compteur = 0;
							tempTexture->NbFrame = tmpNbFrame;
							tempTexture->frameTime = tmptime;
							tempTexture->row = tmpRow;
							AddTexture(tempTexture);
						}
					}
					firstPass = sfTrue;
					fclose(file);
				}
			}
			if (tempRessources->type == ANIMATION)
			{
				if (tempRessources->state == _state)
				{
					FILE* file;
					char tmpligne[200];
					char tmp[150];
					char tmpname[100], tmppath[150];
					sfIntRect tmprect = { 0 };
					StateTexture statetex = TEXNULL;
					sfTexture* tmptexture = NULL;
					sfBool firstPass = sfTrue;
					float tmptime;
					int tmpNbFrame, tmpRow;
					int cptNbTexture = 0;

					if (!(file = fopen(tempRessources->path, "r")))
					{
						printf("Unable to access file : %s", tempRessources->path);
						exit(1);
					}
					while (fgets(tmpligne, 200, file) != NULL) // On lit le fichier tant qu'on ne reçoit pas d'erreur (NULL)
					{
						sscanf(tmpligne, "%s ", tmp);
						tmp[49] = 0;

						if ((strcmp(tmp, "[end]")) == 0)
						{
							statetex = TEXNULL;
						}
						if (statetex == TEXNULL)
						{
							if ((strcmp(tmp, "[path]")) == 0)
							{
								statetex = PATH;
							}
							else if ((strcmp(tmp, "[config]")) == 0)
							{
								statetex = CONFIG;
							}
						}
						else if (statetex == PATH)
						{
							sscanf(tmpligne, "%s ", tmppath);
						}
						else if (statetex == CONFIG)
						{
							/// DANGEROUS, FIX THIS ONE
							sscanf(tmpligne, "%s %d %d %d %d %d %f %d", tmpname, &tmprect.left, &tmprect.top, &tmprect.width, &tmprect.height, &tmpNbFrame, &tmptime, &tmpRow);

							Texture* tempTexture = (Texture*)calloc(1, sizeof(Texture));
							if (!tempTexture) continue;
							tmpname[29] = 0;
							strcpy(tempTexture->name, tmpname);
							tempTexture->spSheetRec = tmprect;
							tempTexture->type = tempRessources->type;
							tempTexture->state = tempRessources->state;
							tempTexture->biome = tempRessources->biome;
							if (firstPass)
							{
								if (!(tempTexture->sftexture = sfTexture_createFromFile(tmppath, NULL)))
									exit(1);
								firstPass = sfFalse;
								tmptexture = tempTexture->sftexture;
							}
							else
							{
								tempTexture->sftexture = NULL;
							}
							tempTexture->compteur = cptNbTexture;
							tempTexture->NbFrame = tmpNbFrame;
							tempTexture->frameTime = tmptime;
							tempTexture->row = tmpRow;
							AddTexture(tempTexture);
							cptNbTexture++;
						}
					}
					firstPass = sfTrue;
					fclose(file);
				}
			}
		}		
		tempRessources = tempRessources->pNext;
	}
}


sfTexture* GetSfTexture(char* _name)
{
	Texture* tempTexture = textureBegin;
	while (tempTexture != NULL)
	{
		if ((strcmp(_name, tempTexture->name)) == 0)
		{
			if(tempTexture->sftexture != NULL)
				return tempTexture->sftexture;
		}
		tempTexture = tempTexture->pNext;
	}
	return NULL;
}

Texture* GetTexture(char* _name)
{
	Texture* tempTexture = textureBegin;
	while (tempTexture != NULL)
	{
		if ((strcmp(_name, tempTexture->name)) == 0)
		{
			return tempTexture;
		}
		tempTexture = tempTexture->pNext;
	}
	return NULL;
}

void GetAnimationDatas(char* _name, int* _frameX, float* _timerAnim, sfIntRect* _rectAnim, ressourceType* _resType)
{
	Texture* tempTexture = textureBegin;
	while (tempTexture != NULL)
	{
		if ((strcmp(_name, tempTexture->name)) == 0)
		{
			*_frameX = tempTexture->NbFrame;
			*_timerAnim = tempTexture->frameTime;
			*_rectAnim = tempTexture->spSheetRec;
			*_resType = tempTexture->type;
			return;
		}
		tempTexture = tempTexture->pNext;
	}
	
	*_frameX = 0;
	*_timerAnim = 0.f;
	*_rectAnim = IntRect(0, 0, 0, 0);
	*_resType = SINGLE;
}

void GetParticleAnimationDatas(char* _name, int* _frameXMax, sfIntRect* _rectAnim)
{
	Texture* tempTexture = textureBegin;
	while (tempTexture != NULL)
	{
		if ((strcmp(_name, tempTexture->name)) == 0)
		{
			*_frameXMax = tempTexture->NbFrame;
			*_rectAnim = tempTexture->spSheetRec;
			return;
		}
		tempTexture = tempTexture->pNext;
	}

	*_frameXMax = 0;
	*_rectAnim = IntRect(0, 0, 0, 0);
}

sfVector2f getAnimationSize(char* _name)
{
	Texture* tempTexture = textureBegin;
	while (tempTexture != NULL)
	{
		if ((strcmp(_name, tempTexture->name)) == 0)
		{
			return vector2f(tempTexture->spSheetRec.width, tempTexture->spSheetRec.height);
		}
		tempTexture = tempTexture->pNext;
	}
	return vector2f(0.f,0.f);
}

int GetNbFrame(char* _name)
{
	Texture* tempTexture = textureBegin;
	while (tempTexture != NULL)
	{
		if ((strcmp(_name, tempTexture->name)) == 0)
		{
			return tempTexture->NbFrame;
		}
		tempTexture = tempTexture->pNext;
	}
	return 0;
}

float GetFrameTime(char* _name)
{
	Texture* tempTexture = textureBegin;
	while (tempTexture != NULL)
	{
		if ((strcmp(_name, tempTexture->name)) == 0)
		{
			return tempTexture->frameTime;
		}
		tempTexture = tempTexture->pNext;
	}
	return 0.0f;
}

sfIntRect GetRect(char* _name)
{
	Texture* tempTexture = textureBegin;
	while (tempTexture != NULL)
	{
		if ((strcmp(_name, tempTexture->name)) == 0)
		{
			return tempTexture->spSheetRec;
		}
		tempTexture = tempTexture->pNext;
	}
	sfIntRect rectNULL = IntRect(0, 0, 0, 0);
	return rectNULL;
}

int getMaxFrameX(char* _name, int frameY)
{
	return 0;
}

void AddTexture(Texture* _texture)
{
	_texture->pNext = textureBegin;
	textureBegin = _texture;
}

Texture* RemoveTexture(Texture* _texture)
{
	if (_texture == textureBegin)
	{
		Texture* tempTexture = _texture->pNext;
		textureBegin = tempTexture;
		sfTexture_destroy(_texture->sftexture);
		free(_texture);
		return tempTexture;
	}
	else
	{
		Texture* tempTexture = textureBegin;
		while (tempTexture->pNext != _texture)
		{
			tempTexture = tempTexture->pNext;
		}
		tempTexture->pNext = _texture->pNext;
		sfTexture_destroy(_texture->sftexture);
		free(_texture);
		return tempTexture->pNext;
	}
}

// Enléve toutes les textures de la chaine
void RemoveAllTexture()
{
	Texture* tempTexture = textureBegin;
	while (tempTexture != NULL)
	{
		Texture* todelete = tempTexture;
		tempTexture = tempTexture->pNext;
		sfTexture_destroy(todelete->sftexture);
		free(todelete);
	}
	textureBegin = 0;
}

// Enléve toutes les textures de la chaine sauf celles qui sont indispensables dans tous les states
void RemoveAllTextureButALL()
{
	Texture* tempTexture = textureBegin;
	while (tempTexture != NULL)
	{
		if (tempTexture->state != ALL)
		{
			if (tempTexture == textureBegin)
			{
				Texture* tempTexture2 = tempTexture->pNext;
				textureBegin = tempTexture2;
 				sfTexture_destroy(tempTexture->sftexture);
				free(tempTexture);
				tempTexture = tempTexture2;
			}
			else
			{
				Texture* toDelete = tempTexture;
				tempTexture = tempTexture->pNext;
				sfTexture_destroy(toDelete->sftexture);
				free(toDelete);
			}
		}
		else
		{
			tempTexture = tempTexture->pNext;
		}
	}
}

void RemoveAllBiomeTexture(Biomes _biome)
{
	Texture* tempTexture = textureBegin;
	while (tempTexture != NULL)
	{
		if (tempTexture->biome == _biome)
		{
			if (tempTexture == textureBegin)
			{
				Texture* tempTexture2 = tempTexture->pNext;
				sfTexture_destroy(tempTexture->sftexture);
				free(tempTexture);
				tempTexture = tempTexture2;
				textureBegin = tempTexture;
			}
			else
			{
				Texture* toDelete = tempTexture;
				tempTexture = tempTexture->pNext;
				sfTexture_destroy(toDelete->sftexture);
				free(toDelete);
			}
		}
		else
		{
			tempTexture = tempTexture->pNext;
		}
	}
}

sfIntRect animatorAnim(char* _textureName, int* currentframex, float* _timer , int thread)
{
	return animator(_textureName, currentframex, _timer, 0, 0, thread);
}

sfIntRect animator(char* _textureName, int* currentframex, float* _timer, int _frameY, int _nbFrameX, int thread)
{
	int frameXStart = 0;
	int frameXEnd;
	sfIntRect rect;
	float frameDuration;
	ressourceType textureType;

	GetAnimationDatas(_textureName, &frameXEnd, &frameDuration, &rect, &textureType);

	sfBool hasFrameXChanged = sfFalse;
	int frameY = 0;

	if (textureType == SPRITESHEET) {
		frameY = _frameY;
		frameXEnd = _nbFrameX;
	}
	if (thread == 0)
		*_timer += getDeltaTime();
	else if (thread == 1)
		*_timer += getDeltaTimeThread2();

	if (*_timer > frameDuration) {
		*_timer = 0;
		(*currentframex)++;
		if (*currentframex > frameXEnd) {
			*currentframex = frameXStart;
		}
		hasFrameXChanged = sfTrue;
	}
	rect.left = (*currentframex) * rect.width + rect.left;
	if (frameY != 0)
		rect.top = frameY * rect.height;
	return rect;
}


sfIntRect animatorWithBeginFrame(char* _textureName, int* currentframex, float* _timer, int thread, int _frameXEnd)
{
	int frameXStart = _frameXEnd;
	int frameXEnd;
	sfIntRect rect;
	float frameDuration;
	ressourceType textureType;

	GetAnimationDatas(_textureName, &frameXEnd, &frameDuration, &rect, &textureType);

	sfBool hasFrameXChanged = sfFalse;
	
	if (thread == 0)
		*_timer += getDeltaTime();
	else if (thread == 1)
		*_timer += getDeltaTimeThread2();

	if (*_timer > frameDuration) {
		*_timer = 0;
		(*currentframex)++;
		if (*currentframex > frameXEnd) {
			*currentframex = frameXStart;
		}
		hasFrameXChanged = sfTrue;
	}
	rect.left = (*currentframex) * rect.width + rect.left;
	
	return rect;
}

sfIntRect animatorReverse(char* _textureName, int* currentframex, float* _timer, int thread, int _frameXReset)
{
	int frameXStart;
	int frameXEnd = _frameXReset;
	sfIntRect rect;
	float frameDuration;
	ressourceType textureType;

	GetAnimationDatas(_textureName, &frameXStart, &frameDuration, &rect, &textureType);

	sfBool hasFrameXChanged = sfFalse;

	if (thread == 0)
		*_timer += getDeltaTime();
	else if (thread == 1)
		*_timer += getDeltaTimeThread2();

	if (*_timer > frameDuration) {
		*_timer = 0;
		(*currentframex)--;
		if (*currentframex < frameXEnd) {
			*currentframex = frameXStart;
		}
		hasFrameXChanged = sfTrue;
	}
	rect.left = (*currentframex) * rect.width + rect.left;

	return rect;
}

sfIntRect animatorWithResize(char* _textureName, int* currentframex, float* _timer, int thread, sfRectangleShape* _rect)
{
	int frameXStart = 0;
	int frameXEnd;
	sfIntRect rect;
	float frameDuration;
	ressourceType textureType;

	GetAnimationDatas(_textureName, &frameXEnd, &frameDuration, &rect, &textureType);

	sfRectangleShape_setSize(_rect, vector2f(rect.width, rect.height));

	sfBool hasFrameXChanged = sfFalse;

	if (thread == 0)
		*_timer += getDeltaTime();
	else if (thread == 1)
		*_timer += getDeltaTimeThread2();

	if (*_timer > frameDuration) {
		*_timer = 0;
		(*currentframex)++;
		if (*currentframex > frameXEnd) {
			*currentframex = frameXStart;
		}
		hasFrameXChanged = sfTrue;
	}
	rect.left = (*currentframex) * rect.width + rect.left;

	return rect;
}

sfIntRect animatorWithResizeAndOrigin(char* _textureName, int* currentframex, float* _timer, int thread, sfRectangleShape* _rect, sfBool _isOriginMiddleDown, sfVector2f _origin)
{
	int frameXStart = 0;
	int frameXEnd;
	sfIntRect rect;
	float frameDuration;
	ressourceType textureType;

	GetAnimationDatas(_textureName, &frameXEnd, &frameDuration, &rect, &textureType);

	sfRectangleShape_setSize(_rect, vector2f(rect.width, rect.height));
	if (_isOriginMiddleDown)
		sfRectangleShape_setOrigin(_rect, vector2f(rect.width / 2.f, rect.height));
	else
		sfRectangleShape_setOrigin(_rect, _origin);

	sfBool hasFrameXChanged = sfFalse;

	if (thread == 0)
		*_timer += getDeltaTime();
	else if (thread == 1)
		*_timer += getDeltaTimeThread2();

	if (*_timer > frameDuration) {
		*_timer = 0;
		(*currentframex)++;
		if (*currentframex > frameXEnd) {
			*currentframex = frameXStart;
		}
		hasFrameXChanged = sfTrue;
	}
	rect.left = (*currentframex) * rect.width + rect.left;

	return rect;
}

sfIntRect animatorWithResizeAndOriginOffset(char* _textureName, int* currentframex, float* _timer, int thread, sfRectangleShape* _rect, sfBool _isOriginMiddleDown, sfVector2f _originOffset)
{
	int frameXStart = 0;
	int frameXEnd;
	sfIntRect rect;
	float frameDuration;
	ressourceType textureType;

	GetAnimationDatas(_textureName, &frameXEnd, &frameDuration, &rect, &textureType);

	sfRectangleShape_setSize(_rect, vector2f(rect.width, rect.height));
	if (_isOriginMiddleDown)
		sfRectangleShape_setOrigin(_rect, vector2f(rect.width / 2.f, rect.height));
	else
		sfRectangleShape_setOrigin(_rect, vector2f(rect.width / 2.f + _originOffset.x, rect.height + _originOffset.y));

	sfBool hasFrameXChanged = sfFalse;

	if (thread == 0)
		*_timer += getDeltaTime();
	else if (thread == 1)
		*_timer += getDeltaTimeThread2();

	if (*_timer > frameDuration) {
		*_timer = 0;
		(*currentframex)++;
		if (*currentframex > frameXEnd) {
			*currentframex = frameXStart;
		}
		hasFrameXChanged = sfTrue;
	}
	rect.left = (*currentframex) * rect.width + rect.left;

	return rect;
}

