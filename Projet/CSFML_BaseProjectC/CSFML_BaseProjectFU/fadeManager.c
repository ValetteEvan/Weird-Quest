#include "fadeManager.h"
#include "viewManager.h"
#include "stateManager.h"

sfRectangleShape* fade_GameRect;
sfRectangleShape* fade_WalkRect;
sfVector2f fade_Size;
sfVector2f fade_WalkPos;
int fade_GameOpacity;
int fade_SpriteOpacity;
int fade_SpriteOpacity2;
int fade_SpriteOpacity3;
int fade_TextOpacity;
int fade_TextOpacity2;
int fade_RectangleOpacity;
int fade_CircleOpacity;
float fade_Timer;
float fade_SpriteTimer;
float fade_SpriteTimer2;
float fade_SpriteTimer3;
float fade_TextTimer;
float fade_TextTimer2;
float fade_RectangleTimer;
float fade_CircleTimer;

void fade_Init() {
	fade_GameRect = sfRectangleShape_create();
	sfRectangleShape_setFillColor(fade_GameRect, sfBlack);
	fade_GameOpacity, fade_SpriteOpacity, fade_TextOpacity, fade_RectangleOpacity, fade_CircleOpacity, fade_SpriteOpacity2, fade_TextOpacity2, fade_SpriteOpacity3 = 0;
	fade_Timer, fade_SpriteTimer, fade_TextTimer, fade_RectangleTimer, fade_CircleTimer, fade_SpriteTimer2, fade_TextTimer2, fade_SpriteTimer3 = 0.f;
	fade_Size = vector2f(1920.f, 1080.f);
}

void walkingFade_Init() {
	fade_WalkRect = sfRectangleShape_create();
	sfRectangleShape_setFillColor(fade_WalkRect, sfBlack);
	sfRectangleShape_setSize(fade_WalkRect, fade_Size);
}

void fade_Update() {
	if (!fade_GameRect) return;
	if (getState() == GAME) sfRectangleShape_setPosition(fade_GameRect, vector2f(getViewPosition(mainView).x - 960.f, getViewPosition(mainView).y - 540.f));
	sfRectangleShape_setFillColor(fade_GameRect, sfColor_fromRGBA(0, 0, 0, (char)fade_GameOpacity));
}

void walkingFade_Update() {
	if (!fade_WalkRect) return;
	sfRectangleShape_setPosition(fade_WalkRect, fade_WalkPos);
}

void fade_Display(Window* _window) {
	if (!fade_GameRect) return;
	sfRectangleShape_setSize(fade_GameRect, fade_Size);
	sfRenderTexture_drawRectangleShape(_window->rdrt, fade_GameRect, NULL);
}

void fade_Deinit() {
	sfRectangleShape_destroy(fade_GameRect);
}

void fade_SetPosition(sfVector2f _newPos) {
	sfRectangleShape_setPosition(fade_GameRect, _newPos);
}

void fadeIn(float _timer) {
	fade_Timer += getDeltaTime();
	if (fade_Timer >= (_timer / 255.f)) {
		if (fade_GameOpacity > 0) {
			fade_GameOpacity--;
			if (fade_GameRect != NULL) sfRectangleShape_setFillColor(fade_GameRect, sfColor_fromRGBA(0, 0, 0, fade_GameOpacity));
			fade_Timer = 0.f;
		}
	}
}

void fadeOut(float _timer) {
	fade_Timer += getDeltaTime();
	if (fade_Timer >= (_timer / 255.f)) {
		if (fade_GameOpacity < 255) {
			fade_GameOpacity++;
			if (fade_GameRect != NULL) sfRectangleShape_setFillColor(fade_GameRect, sfColor_fromRGBA(0, 0, 0, fade_GameOpacity));
			fade_Timer = 0.f;
		}
	}
}

void fadeOutSprite(float _timer, sfSprite* _spr) {
	fade_SpriteTimer += getDeltaTime();
	if (fade_SpriteTimer >= (_timer / 255.f)) {
		if (fade_SpriteOpacity > 0) {
			fade_SpriteOpacity--;
			if (_spr != NULL) sfSprite_setColor(_spr, sfColor_fromRGBA(sfSprite_getColor(_spr).r, sfSprite_getColor(_spr).g, sfSprite_getColor(_spr).b, fade_SpriteOpacity));
			fade_SpriteTimer = 0.f;
		}
	}
}

void fadeInSprite(float _timer, sfSprite* _spr) {
	fade_SpriteTimer += getDeltaTime();
	if (fade_SpriteTimer >= (_timer / 255.f)) {
		if (fade_SpriteOpacity < 255) {
			fade_SpriteOpacity++;
			if (_spr != NULL) sfSprite_setColor(_spr, sfColor_fromRGBA(sfSprite_getColor(_spr).r, sfSprite_getColor(_spr).g, sfSprite_getColor(_spr).b, fade_SpriteOpacity));
			fade_SpriteTimer = 0.f;
		}
	}
}

void fadeOutSprite2(float _timer, sfSprite* _spr) {
	fade_SpriteTimer2 += getDeltaTime();
	if (fade_SpriteTimer2 >= (_timer / 255.f)) {
		if (fade_SpriteOpacity2 > 0) {
			fade_SpriteOpacity2--;
			if (_spr != NULL) sfSprite_setColor(_spr, sfColor_fromRGBA(sfSprite_getColor(_spr).r, sfSprite_getColor(_spr).g, sfSprite_getColor(_spr).b, fade_SpriteOpacity2));
			fade_SpriteTimer2 = 0.f;
		}
	}
}

void fadeInSprite2(float _timer, sfSprite* _spr) {
	fade_SpriteTimer2 += getDeltaTime();
	if (fade_SpriteTimer2 >= (_timer / 255.f)) {
		if (fade_SpriteOpacity2 < 255) {
			fade_SpriteOpacity2++;
			if (_spr != NULL) sfSprite_setColor(_spr, sfColor_fromRGBA(sfSprite_getColor(_spr).r, sfSprite_getColor(_spr).g, sfSprite_getColor(_spr).b, fade_SpriteOpacity2));
			fade_SpriteTimer2 = 0.f;
		}
	}
}

void fadeOutSprite3(float _timer, sfSprite* _spr) {
	fade_SpriteTimer3 += getDeltaTime();
	if (fade_SpriteTimer3 >= (_timer / 255.f)) {
		if (fade_SpriteOpacity3 > 0) {
			fade_SpriteOpacity3--;
			if (_spr != NULL) sfSprite_setColor(_spr, sfColor_fromRGBA(sfSprite_getColor(_spr).r, sfSprite_getColor(_spr).g, sfSprite_getColor(_spr).b, fade_SpriteOpacity3));
			fade_SpriteTimer3 = 0.f;
		}
	}
}

void fadeInSprite3(float _timer, sfSprite* _spr) {
	fade_SpriteTimer3 += getDeltaTime();
	if (fade_SpriteTimer3 >= (_timer / 255.f)) {
		if (fade_SpriteOpacity3 < 255) {
			fade_SpriteOpacity3++;
			if (_spr != NULL) sfSprite_setColor(_spr, sfColor_fromRGBA(sfSprite_getColor(_spr).r, sfSprite_getColor(_spr).g, sfSprite_getColor(_spr).b, fade_SpriteOpacity3));
			fade_SpriteTimer = 0.f;
		}
	}
}

void fadeOutText(float _timer, sfText* _txt) {
	fade_TextTimer += getDeltaTime();
	if (fade_TextTimer >= (_timer / 255.f)) {
		if (fade_TextOpacity > 0) {
			fade_TextOpacity--;
			if (_txt != NULL) sfText_setColor(_txt, sfColor_fromRGBA(sfText_getColor(_txt).r, sfText_getColor(_txt).g, sfText_getColor(_txt).b, fade_TextOpacity));
			fade_TextTimer = 0.f;
		}
	}
}

void fadeInText(float _timer, sfText* _txt) {
	fade_TextTimer += getDeltaTime();
	if (fade_TextTimer >= (_timer / 255.f)) {
		if (fade_TextOpacity < 255) {
			fade_TextOpacity++;
			if (_txt != NULL) sfText_setColor(_txt, sfColor_fromRGBA(sfText_getColor(_txt).r, sfText_getColor(_txt).g, sfText_getColor(_txt).b, fade_TextOpacity));
			fade_TextTimer = 0.f;
		}
	}
}

void fadeOutText2(float _timer, sfText* _txt) {
	fade_TextTimer2 += getDeltaTime();
	if (fade_TextTimer2 >= (_timer / 255.f)) {
		if (fade_TextOpacity2 > 0) {
			fade_TextOpacity2--;
			if (_txt != NULL) sfText_setColor(_txt, sfColor_fromRGBA(sfText_getColor(_txt).r, sfText_getColor(_txt).g, sfText_getColor(_txt).b, fade_TextOpacity2));
			fade_TextTimer2 = 0.f;
		}
	}
}

void fadeInText2(float _timer, sfText* _txt) {
	fade_TextTimer2 += getDeltaTime();
	if (fade_TextTimer2 >= (_timer / 255.f)) {
		if (fade_TextOpacity2 < 255) {
			fade_TextOpacity2++;
			if (_txt != NULL) sfText_setColor(_txt, sfColor_fromRGBA(sfText_getColor(_txt).r, sfText_getColor(_txt).g, sfText_getColor(_txt).b, fade_TextOpacity2));
			fade_TextTimer2 = 0.f;
		}
	}
}

void fadeOutRectangle(float _timer, sfRectangleShape* _rectangle) {
	fade_RectangleTimer += getDeltaTime();
	if (fade_RectangleTimer >= (_timer / 255.f)) {
		if (fade_RectangleOpacity > 0) {
			fade_RectangleOpacity--; 
			if (_rectangle != NULL) sfRectangleShape_setFillColor(_rectangle, sfColor_fromRGBA(sfRectangleShape_getFillColor(_rectangle).r, sfRectangleShape_getFillColor(_rectangle).g, sfRectangleShape_getFillColor(_rectangle).b, fade_RectangleOpacity));
			fade_RectangleTimer = 0.f;
		}
	}
}

void fadeInRectangle(float _timer, sfRectangleShape* _rectangle) {
	fade_RectangleOpacity += getDeltaTime();
	if (fade_RectangleOpacity >= (_timer / 255.f)) {
		if (fade_RectangleOpacity < 255) {
			fade_RectangleOpacity++;
			if (_rectangle != NULL) sfRectangleShape_setFillColor(_rectangle, sfColor_fromRGBA(sfRectangleShape_getFillColor(_rectangle).r, sfRectangleShape_getFillColor(_rectangle).g, sfRectangleShape_getFillColor(_rectangle).b, fade_RectangleOpacity));
			fade_RectangleOpacity = 0.f;
		}
	}
}

void fadeOutCircle(float _timer, sfCircleShape* _circle) {
	fade_CircleTimer += getDeltaTime();
	if (fade_CircleTimer >= (_timer / 255.f)) {
		if (fade_CircleOpacity > 0) {
			fade_CircleOpacity--;
			if (_circle != NULL) sfCircleShape_setFillColor(_circle, sfColor_fromRGBA(sfCircleShape_getFillColor(_circle).r, sfCircleShape_getFillColor(_circle).g, sfCircleShape_getFillColor(_circle).b, fade_CircleOpacity));
			fade_CircleTimer = 0.f;
		}
	}
}

void fadeInCircle(float _timer, sfCircleShape* _circle) {
	fade_CircleTimer += getDeltaTime();
	if (fade_CircleTimer >= (_timer / 255.f)) {
		if (fade_CircleOpacity < 255) {
			fade_CircleOpacity++;
			if (_circle != NULL) sfCircleShape_setFillColor(_circle, sfColor_fromRGBA(sfCircleShape_getFillColor(_circle).r, sfCircleShape_getFillColor(_circle).g, sfCircleShape_getFillColor(_circle).b, fade_CircleOpacity));
			fade_CircleTimer = 0.f;
		}
	}
}

int fade_GetTextOpacity() {
	return fade_TextOpacity;
}

int fade_GetTextOpacity2() {
	return fade_TextOpacity2;
}

int fade_GetOpacity() {
	return fade_GameOpacity;
}

int fade_GetSpriteOpacity() {
	return fade_SpriteOpacity;
}

int fade_GetSpriteOpacity2() {
	return fade_SpriteOpacity2;
}

int fade_GetSpriteOpacity3() {
	return fade_SpriteOpacity3;
}

int fade_GetRectangleOpacity() {
	return fade_RectangleOpacity;
}

int fade_GetCircleOpacity() {
	return fade_CircleOpacity;
}

void fade_SetSize(sfVector2f _size) {
	fade_Size = _size;
}

void fade_SetOpacity(int _new_opacity) {
	fade_GameOpacity = _new_opacity;
}

void fade_SetTextOpacity(int _new_opacity) {
	fade_TextOpacity = _new_opacity;
}

void fade_SetTextOpacity2(int _new_opacity) {
	fade_TextOpacity2 = _new_opacity;
}

void fade_SetSpriteOpacity(int _new_opacity) {
	fade_SpriteOpacity = _new_opacity;
}

void fade_SetSprite2Opacity(int _new_opacity) {
	fade_SpriteOpacity2 = _new_opacity;
}

void fade_SetSprite3Opacity(int _new_opacity) {
	fade_SpriteOpacity3 = _new_opacity;
}

void fade_SetRectOpacity(int _new_opacity) {
	fade_RectangleOpacity = _new_opacity;
}

void fade_SetCircleOpacity(int _new_opacity) {
	fade_CircleOpacity = _new_opacity;
}

void walkingFade_SetPosition(sfVector2f _newPosition) {
	fade_WalkPos = _newPosition;
}

sfVector2f walkingFade_GetPosition() {
	return fade_WalkPos;
}

void walkingFade_Draw(Window* _window) {
	sfRenderTexture_drawRectangleShape(_window->rdrt, fade_WalkRect, NULL);
}