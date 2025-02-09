#include "FXenemies.h"
#include "textureManager.h"

#define GD_FXE STD_LIST_GETDATA(FXe_ennemiesList, FXenemies_struct, i)

sfRectangleShape* FXe_rectangle;
sfTexture* FXe_textures[2];

void FXe_initFXenemies()
{
	FXe_rectangle = sfRectangleShape_create();
	FXe_ennemiesList = STD_LIST_CREATE(FXenemies_struct, 0);
	FXe_textures[FX_STARS] = GetSfTexture("FX_stars");
	FXe_textures[FX_EXPLOSION] = GetSfTexture("FX_dead");
}

void FXe_addFXenemies(sfVector2f _pos, FXenemies_enum _id)
{
	FXenemies_struct tmp;

	tmp.animTime = 0.0f;
	tmp.frameX = 0;
	tmp.pos = _pos;
	tmp.id = _id;

	FXe_ennemiesList->push_back(&FXe_ennemiesList, &tmp);
}

void FXe_updateFXenemies()
{
	for(int i = 0; i < FXe_ennemiesList->size(FXe_ennemiesList); i++)
	{
		if (GD_FXE->id == FX_STARS)
		{
			if(GD_FXE->frameX >=8)
			{
				FXe_ennemiesList->erase(&FXe_ennemiesList, i);
				continue;
			}
		}
		else
		{
			if (GD_FXE->frameX >= 7)
			{
				FXe_ennemiesList->erase(&FXe_ennemiesList, i);
				continue;
			}
		}
	}
}

void FXe_displayFXenemies(Window* _window)
{
	for (int i = 0; i < FXe_ennemiesList->size(FXe_ennemiesList); i++)
	{
		if (GD_FXE->id == FX_STARS)
		{
			sfRectangleShape_setTexture(FXe_rectangle, FXe_textures[FX_STARS], sfTrue);
			sfRectangleShape_setTextureRect(FXe_rectangle, animatorAnim("FX_stars", &GD_FXE->frameX, &GD_FXE->animTime, 0));
			sfRectangleShape_setSize(FXe_rectangle, vector2f(64.0f, 64.0f));
			sfRectangleShape_setOrigin(FXe_rectangle, vector2f(32.f, 32.f));
		}
		else
		{
			sfRectangleShape_setTexture(FXe_rectangle, FXe_textures[FX_EXPLOSION], sfTrue);
			sfRectangleShape_setTextureRect(FXe_rectangle, animatorAnim("FX_dead", &GD_FXE->frameX, &GD_FXE->animTime, 0));
			sfRectangleShape_setSize(FXe_rectangle, vector2f(192.0f, 164.0f));
			sfRectangleShape_setOrigin(FXe_rectangle, vector2f(96.f, 82.f));
		}
		sfRectangleShape_setPosition(FXe_rectangle, GD_FXE->pos);
		sfRenderTexture_drawRectangleShape(_window->rdrt, FXe_rectangle, NULL);
	}
}
