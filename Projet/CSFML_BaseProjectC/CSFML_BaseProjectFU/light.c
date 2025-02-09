#include "light.h"
#include "shaderManager.h"
#include "viewManager.h"
#include "shaderManager.h"
#define  DATALIGHT STD_LIST_GETDATA(lightList, Light, i)

sfRectangleShape* rgl;

sfTexture* texWallpaper;

Light light;

void addLight(sfVector2f _pos, sfVector3f _color, sfVector2f _size, EditorLightForm _form, float _angle, float _rotate, float _lightStrength)
{
	if (lightList == NULL)
		return;

	light.pos = _pos;
	light.color = _color;
	light.size = _size;
	light.form = _form;
	light.angle = _angle;
	light.rotation = _rotate;
	light.strength = _lightStrength;

	lightList->push_back(&lightList, &light);
}

void initLights()
{
	lightList = STD_LIST_CREATE(Light, 0);
	rgl = sfRectangleShape_create();

	sfRectangleShape_setSize(rgl, vector2f(1920.f, 1080.f));

	sfRectangleShape_setFillColor(rgl, sfColor_fromRGBA(255, 255, 255, 255));
}

void updateLights()
{
}

void displayLights(Window* _window)
{
	for (int i = 0; i < lightList->size(lightList); i++)
	{
		sfVector2f sizes = addVectorsrf(mainView->size, DATALIGHT->size);
		if (DATALIGHT->pos.x > mainView->viewPos.x - (sizes.x / 2.f) && DATALIGHT->pos.x < mainView->viewPos.x + (sizes.x / 2.f) && DATALIGHT->pos.y > mainView->viewPos.y - (sizes.y / 2.f) && DATALIGHT->pos.y < mainView->viewPos.y + (sizes.y / 2.f))
		{
			sfShader* shaderLights = GetShader("light");
			if (shaderLights)
			{
				sfShader_setVec2Uniform(shaderLights, "u_pos", DATALIGHT->pos);
				sfShader_setVec2Uniform(shaderLights, "u_vpos", mainView->viewPos);
				//printf("posLight : %.2f / %.2f\n", posLightScreen.x, posLightScreen.y);
				sfShader_setVec2Uniform(shaderLights, "u_size", DATALIGHT->size);
				sfShader_setVec3Uniform(shaderLights, "u_color", DATALIGHT->color);
				sfShader_setTextureUniform(shaderLights, "u_texture", sfRenderTexture_getTexture(_window->rdrt));
				sfShader_setIntUniform(shaderLights, "u_lightForm", DATALIGHT->form);
				sfShader_setFloatUniform(shaderLights, "u_angle", DATALIGHT->angle);
				sfShader_setFloatUniform(shaderLights, "u_rotation", DATALIGHT->rotation);
				sfShader_setFloatUniform(shaderLights, "u_strength", DATALIGHT->strength);
				sfShader_setVec2Uniform(shaderLights, "u_vsize", mainView->size);
			}
			sfRenderTexture_drawRectangleShape(_window->rdrt, rgl, GetRenderState("light"));
		}
	} 
}

void light_Erase(int _id) {
	if (_id > lightList->size(lightList)) return;
	lightList->erase(&lightList, _id);
}

void light_Clear() {
	if (!lightList) return;
	for (int i = 0; i < lightList->size(lightList); i++) light_Erase(i);
}

Light* getLightListToSave()
{
	if (lightList == NULL)
		return NULL;

	int len = lightList->size(lightList);
	Light* tmp = calloc(len, sizeof(Light));

	if (tmp == NULL)
		return NULL;

	for (int i = 0; i < len; i++)
	{
		tmp[i] = *STD_LIST_GETDATA(lightList, Light, i);
	}
	return tmp;
}