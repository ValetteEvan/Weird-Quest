#include "shaderManager.h"

static sfBool firstPassShader = sfFalse;

void shaderOnload(State _state)
{
	if (!firstPassShader) {
		firstPassShader = sfTrue;
		shdBegin = 0;
	}

	ressources* tempRessources = ResBegin;
	while (tempRessources != NULL)
	{
		if (tempRessources->type == SHADER)
		{
			if (tempRessources->state == _state)
			{
				Shader* tempShd = (Shader*)calloc(1, sizeof(Shader));
				if (!tempShd) return;

				strcpy(tempShd->name, tempRessources->name);
				tempShd->state = tempRessources->state;
				tempShd->rs.shader = sfShader_createFromFile(NULL, NULL, tempRessources->path);
				tempShd->rs.blendMode = sfBlendAlpha;
				tempShd->rs.transform = sfTransform_Identity;
				tempShd->rs.texture = NULL;
				AddShader(tempShd);
			}
		}
		tempRessources = tempRessources->pNext;
	}
}

sfShader* GetShader(char* _name)
{
	Shader* tempShd = shdBegin;
	while (tempShd != NULL)
	{
		if ((strcmp(_name, tempShd->name)) == 0)
		{
			return tempShd->rs.shader;
		}
		tempShd = tempShd->pNext;
	}
	return NULL;
}

sfRenderStates* GetRenderState(char* _name)
{
	Shader* tempShd = shdBegin;
	while (tempShd != NULL)
	{
		if ((strcmp(_name, tempShd->name)) == 0)
		{
			return &(tempShd->rs);
		}
		tempShd = tempShd->pNext;
	}
	return NULL;
}

void AddShader(Shader* _shd)
{
	_shd->pNext = shdBegin;
	shdBegin = _shd;
}

Shader* RemoveShader(Shader* _shd)
{
	if (_shd == shdBegin)
	{
		Shader* tempShd = _shd->pNext;
		shdBegin = tempShd;
		sfShader_destroy(_shd->rs.shader);
		free(_shd);
		return tempShd;
	}
	else
	{
		Shader* tempShd = shdBegin;
		while (tempShd->pNext != _shd)
		{
			tempShd = tempShd->pNext;
		}
		tempShd->pNext = _shd->pNext;
		sfShader_destroy(_shd->rs.shader);
		free(_shd);
		return tempShd->pNext;
	}
}

void RemoveAllShaders()
{
	Shader* tempShd = shdBegin;
	while (tempShd != NULL)
	{
		Shader* todelete = tempShd;
		tempShd = tempShd->pNext;
		sfShader_destroy(todelete->rs.shader);
		free(todelete);
	}
	shdBegin = 0;
}