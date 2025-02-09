#pragma once
#include "tools.h"
#include "windowManager.h"
#include "List.h"
#include "editorHUD.h"
#include "math.h"

typedef struct
{
	sfVector2f pos;
	sfVector3f color;
	sfVector2f size;
	EditorLightForm form;
	float angle;
	float rotation;
	float strength;
}Light;

stdList* lightList;

void addLight(sfVector2f _pos, sfVector3f _color, sfVector2f _size, EditorLightForm _form, float _angle, float _rotate, float _lightStrength);
void initLights();
void updateLights();
void displayLights(Window* _window);
void light_Erase(int _id);
void light_Clear();
Light* getLightListToSave();