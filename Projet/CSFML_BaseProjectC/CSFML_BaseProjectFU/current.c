#include "current.h"

typedef enum {
	CUR_LEFT,
	CUR_TOP,
	CUR_RIGHT,
	CUR_BOT
}cur_direction;

typedef struct {
	cur_direction d;
	sfRectangleShape* r;
	sfVector2f p;
	sfFloatRect c;
}Current;

sfVector2f current_ChangePlayerPos;
Current current;

void current_Init() {
	current.r = sfRectangleShape_create();
	current.p = vector2f(200.f, 100.f);
	current.d = 0;
	sfRectangleShape_setSize(current.r, vector2f(500.f, 500.f));
	sfRectangleShape_setFillColor(current.r, sfGreen);
	sfRectangleShape_setPosition(current.r, current.p);
	current.c = sfRectangleShape_getGlobalBounds(current.r);
}

void current_Update() {
	current_ChangePlayerPos = plr_GetPos();
	if (current.d == 0) current_ChangePlayerPos.x -= getDeltaTime() * 300.f;
	else if (current.d == 1) current_ChangePlayerPos.y -= getDeltaTime() * 300.f;
	else if (current.d == 2) current_ChangePlayerPos.x += getDeltaTime() * 300.f;
	else if (current.d == 3) current_ChangePlayerPos.y += getDeltaTime() * 300.f;
	if (sfFloatRect_intersects(getPointerPlayerBounds(), &current.c, NULL)) {
		if (current.d == 0 || current.d == 2) plr_SetPos(vector2f(current_ChangePlayerPos.x, current_ChangePlayerPos.y));
		else if (current.d == 1 || current.d == 3) plr_SetPos(vector2f(current_ChangePlayerPos.x, current_ChangePlayerPos.y));
	}
}

void current_Display(Window* _window) {
	sfRenderTexture_drawRectangleShape(_window->rdrt, current.r, NULL);
}

void current_Deinit() {
	sfRectangleShape_destroy(current.r);
}