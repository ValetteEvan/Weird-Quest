#include "tools.h"
#include "game.h"

sfTime sftime;
sfTime sftimeThread2;
sfTime sftimeThread3;
sfClock* sfclock;
sfClock* sfclockEnemiThread;
sfClock* sfclockXPThread;
sfFont* defaultFont;

float timeScale;

void initTools()
{
	timeScale = 1.f;
	sfclock = sfClock_create();
	srand((unsigned int)time(NULL));
	DEBUG_STATE = sfFalse;
}

void initToolsThread2()
{
	sfclockEnemiThread = sfClock_create();
	srand((unsigned int)time(NULL));
}

void initToolsThread3()
{
	sfclockXPThread = sfClock_create();
	srand((unsigned int)time(NULL));
}


sfVector2f vector2f(float _x, float _y)
{
	sfVector2f tmp = { _x, _y };
	return tmp;
}

sfVector2i vector2i(int _x, int _y)
{
	sfVector2i tmp = { _x, _y };
	return tmp;
}

sfVector3f vector3f(float _x, float _y, float _z)
{
	sfVector3f tmp = { _x, _y, _z };
	return tmp;
}

sfFloatRect FlRect(float _left, float _top, float _width, float _height)
{
	sfFloatRect rectTmp = { _left, _top,  _width, _height };
	return rectTmp;
}

sfIntRect IntRect(int _left, int _top, int _width, int _height)
{
	sfIntRect rectTmp = { _left, _top,  _width, _height };
	return rectTmp;
}

sfColor color(sfUint8 r, sfUint8 g, sfUint8 b, sfUint8 a)
{
	return sfColor_fromRGBA(r, g, b, a);
}


float lerp(float v0, float v1, float t)
{
	return (1 - t) * v0 + t * v1;
}

float lerp_smooth(float v0, float v1, float t)
{
	return v0 + (v1 - v0) * (t * t * (3.f - 2.f * t));
}


sfVector2f vec2f_lerp(sfVector2f v0, sfVector2f v1, float t)
{
	sfVector2f tmp;
	tmp.x = (1 - t) * v0.x + t * v1.x;
	tmp.y = (1 - t) * v0.y + t * v1.y;
	return tmp;
}

sfVector2f vec2f_lerp_smooth(sfVector2f v0, sfVector2f v1, float t)
{
	sfVector2f tmp;
	t = fmaxf(0.0f, fminf(1.0f, t));
	tmp.x = v0.x + (v1.x - v0.x) * (t * t * (3.f - 2.f * t));
	tmp.y = v0.y + (v1.y - v0.y) * (t * t * (3.f - 2.f * t));
	return tmp;
}

sfVector2f vec2f_lerp_invSquare(sfVector2f v0, sfVector2f v1, float t)
{
	sfVector2f tmp;
	t = fmaxf(0.0f, fminf(1.0f, t));
	tmp.x = v0.x + (v1.x - v0.x) * (1.f - ((1.f - t) * (1.f - t)));
	tmp.y = v0.y + (v1.y - v0.y) * (1.f - ((1.f - t) * (1.f - t)));
	return tmp;
}

sfVector2f vec2f_lerp_invCube(sfVector2f v0, sfVector2f v1, float t)
{
	sfVector2f tmp;
	t = fmaxf(0.0f, fminf(1.0f, t));
	tmp.x = v0.x + (v1.x - v0.x) * (1.f - ((1.f - t) * (1.f - t) * (1.f - t)));
	tmp.y = v0.y + (v1.y - v0.y) * (1.f - ((1.f - t) * (1.f - t) * (1.f - t)));
	return tmp;
}

float float_lerp_invCube(float v0, float v1, float t) {
	return  v0 + (v1 - v0) * (1.f - ((1.f - t) * (1.f - t) * (1.f - t)));
}

sfVector3f vec3f_lerp(sfVector3f v0, sfVector3f v1, float t)
{
	sfVector3f tmp;
	tmp.x = (1 - t) * v0.x + t * v1.x;
	tmp.y = (1 - t) * v0.y + t * v1.y;
	tmp.z = (1 - t) * v0.z + t * v1.z;
	return tmp;
}


vector4f vec4f_lerp(vector4f v0, vector4f v1, float t)
{
	vector4f tmp;
	tmp.r = (1 - t) * v0.r + t * v1.r;
	tmp.g = (1 - t) * v0.g + t * v1.g;
	tmp.b = (1 - t) * v0.b + t * v1.b;
	tmp.a = (1 - t) * v0.a + t * v1.a;
	return tmp;
}


sfColor color_lerp(sfColor v0, sfColor v1, float t)
{
	sfColor tmp;
	tmp.r = (1 - t) * v0.r + t * v1.r;
	tmp.g = (1 - t) * v0.g + t * v1.g;
	tmp.b = (1 - t) * v0.b + t * v1.b;
	tmp.a = (1 - t) * v0.a + t * v1.a;
	return tmp;
}

sfColor color_lerp_no_opacity(sfColor v0, sfColor v1, float t)
{
	sfColor tmp;
	tmp.r = (1 - t) * v0.r + t * v1.r;
	tmp.g = (1 - t) * v0.g + t * v1.g;
	tmp.b = (1 - t) * v0.b + t * v1.b;
	tmp.a = (1 - t) * v0.a + t * 0;
	return tmp;
}

void restartClock()
{
	sftime = sfClock_restart(sfclock);
}

void restartClockThread2()
{
	sftimeThread2 = sfClock_restart(sfclockEnemiThread);
}

void restartClockThread3()
{
	sftimeThread3 = sfClock_restart(sfclockXPThread);
}

float getDeltaTime()
{
	float tmp = sfTime_asSeconds(sftime) * timeScale;
	if (tmp > 0.1 * timeScale) tmp = 0.1f * timeScale;
	return tmp;
}

float getDeltaTimeThread2()
{
	float tmp = sfTime_asSeconds(sftimeThread2) * timeScale;
	if (tmp > 0.1 * timeScale) tmp = 0.1f * timeScale;
	return tmp;
}

float getDeltaTimeThread3()
{
	float tmp = sfTime_asSeconds(sftimeThread3) * timeScale;
	if (tmp > 0.1 * timeScale) tmp = 0.1f * timeScale;
	return tmp;
}

float getUnscaledDeltaTime()
{
	float tmp = sfTime_asSeconds(sftime);
	if (tmp > 0.1) tmp = 0.1f;
	return tmp;
}

void setTimeScale(float _timeScale)
{
	timeScale = _timeScale;
}

float getTimeScale()
{
	return timeScale;
}

sfSprite* CreateSprite(char* _path)
{
	sfTexture* tmp = sfTexture_createFromFile(_path, NULL);
	sfSprite* spTmp = sfSprite_create();
	sfSprite_setTexture(spTmp, tmp, sfTrue);
	return spTmp;
}

sfBool Circle_Collision(sfVector2f _pos1, sfVector2f _pos2, float _rayon1, float _rayon2)
{
	if ((_pos1.x - _pos2.x) * (_pos1.x - _pos2.x) + (_pos1.y - _pos2.y) * (_pos1.y - _pos2.y) < (_rayon1 + _rayon2) * (_rayon1 + _rayon2))
	{
		return sfTrue;
	}
	else return sfFalse;
}

sfBool Rectangle_Collision(sfFloatRect _box1, sfFloatRect _box2)
{
	if ((_box2.left >= _box1.left + _box1.width)      // trop à droite
		|| (_box2.left + _box2.width <= _box1.left) // trop à gauche
		|| (_box2.top >= _box1.top + _box1.height) // trop en bas
		|| (_box2.top + _box2.height <= _box1.top))  // trop en haut
		return sfFalse;
	else
		return sfTrue;
}

int iRand(int _min, int _max)
{
	if (_max > _min)
		return rand() % (_max - _min) + _min;
	else
		return _min;
}

int rand_int(int _min, int _max)
{
	return rand() % (_max - _min) + _min;
}

double rand_float(float _min, float _max)
{
	return ((rand() / (double)RAND_MAX) * ((double)_max - (double)_min) + (double)_min);
}

sfColor randColor(int a)
{
	return sfColor_fromRGBA(rand_int(0, 255), rand_int(0, 255), rand_int(0, 255), a);
}

float FrameRate()
{
	return 1.0f / getDeltaTime();
}

float FrameRateThrd2()
{
	return 1.0f / getDeltaTimeThread2();
}

float FrameRateThrd3()
{
	return 1.0f / getDeltaTimeThread3();
}

void displayFrameRate(sfRenderTexture* _rdrt, sfText* _text)
{
	/*static float displayTimer = 0.0f;
	static char frameRate[25];

	displayTimer += getDeltaTime();

	if (displayTimer > 0.1f)
	{
		sprintf_s(frameRate, 25, "FPS : %.2f", FrameRate());
		displayTimer = 0.0f;
	}

	sfText_setString(_text, frameRate);
	sfRenderTexture_drawText(_rdrt, _text, NULL);*/
}

void displayFrameRateThrd2(sfRenderTexture* _rdrt, sfText* _text)
{
	/*static float displayTimer = 0.0f;
	static char frameRate[25];

	displayTimer += getDeltaTime();

	if (displayTimer > 0.1f)
	{
		sprintf_s(frameRate, 25, "THRD 2 FPS : %.2f", FrameRateThrd2());
		displayTimer = 0.0f;
	}

	sfText_setString(_text, frameRate);
	sfRenderTexture_drawText(_rdrt, _text, NULL);*/
}

void displayFrameRateThrd3(sfRenderTexture* _rdrt, sfText* _text)
{
	/*static float displayTimer = 0.0f;
	static char frameRate[25];

	displayTimer += getDeltaTime();

	if (displayTimer > 0.1f)
	{
		sprintf_s(frameRate, 25, "THRD 3 FPS : %.2f", FrameRateThrd3());
		displayTimer = 0.0f;
	}

	sfText_setString(_text, frameRate);
	sfRenderTexture_drawText(_rdrt, _text, NULL);*/
}

void screenshot(sfRenderWindow* _window)
{
	char hours[9] = { 0 }, date[9] = { 0 }, tmph[7] = { 0 }, tmpd[7] = { 0 };
	_strtime(hours);
	_strdate(date);
	sfVector2u windowSize;


	windowSize = sfRenderWindow_getSize(_window);
	sfTexture* texture = sfTexture_create(windowSize.x, windowSize.y);
	sfTexture_updateFromRenderWindow(texture, _window, 0, 0);
	sfImage* screenshot = sfTexture_copyToImage(texture);
	char filename[100];
	int j = 0;
	for (int i = 0; i < 9; i++) if (hours[i] != ':') tmph[j++] = hours[i];
	j = 0;
	for (int i = 0; i < 9; i++) if (date[i] != '/') tmpd[j++] = date[i];

	sprintf(filename, "../Ressources/Screenshots/Screenshot-%s-%s.jpg", tmpd, tmph);
	sfImage_saveToFile(screenshot, filename);
}

/// This preprocessor ballet allows to turn off the deprecation warning exclusively for this function
#pragma warning (push)
#pragma warning (disable: 4995)
sfBool customKeyboardIsKeyPressed(sfKeyCode key, sfRenderWindow* _window)
{
	if (fullscreen_IsDone) {
		if (_window && sfRenderWindow_hasFocus(_window)) {
			return sfKeyboard_isKeyPressed(key);
		}
		return sfFalse;
	}
	return sfFalse;
}
#pragma warning (pop)

void centralText(sfText* _txt) {
	if (!_txt) return;
	sfText_setOrigin(_txt, vector2f(sfText_getGlobalBounds(_txt).width / 2.0f, sfText_getGlobalBounds(_txt).height / 2.0f));
}

void centralRect(sfRectangleShape* _rect) {
	if (!_rect) return;
	sfRectangleShape_setOrigin(_rect, vector2f(sfRectangleShape_getGlobalBounds(_rect).width / 2.0f, sfRectangleShape_getGlobalBounds(_rect).height / 2.0f));
}

void centralSprite(sfSprite* _spr) {
	if (!_spr) return;
	sfSprite_setOrigin(_spr, vector2f(sfSprite_getGlobalBounds(_spr).width / 2.0f, sfSprite_getGlobalBounds(_spr).height / 2.0f));
}

void centralCircle(sfCircleShape* _circle) {
	if (!_circle) return;
	sfCircleShape_setOrigin(_circle, vector2f(sfCircleShape_getGlobalBounds(_circle).width / 2.0f, sfCircleShape_getGlobalBounds(_circle).height / 2.0f));
}