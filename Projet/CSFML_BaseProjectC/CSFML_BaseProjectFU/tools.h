#pragma once

#include "SFML/Graphics.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <windows.h>
#include "dirent.h"

#define TEXTURE_PATH "..\\Ressources\\Textures\\"
#define FONT_PATH "..\\Ressources\\Fonts\\"
#define MAP_PATH "..\\Ressources\\Maps\\"
#define TILESET_PATH "..\\Ressources\\Tilesets\\"

#define NULLVECTF vector2f(0.f, 0.f) /// Zero-magnitude vector (float)
#define NULLVECTI vector2i(0, 0) /// Zero-magnitude vector (int)

#pragma deprecated(printf) /// Clean those up when you're done with debugging
#pragma deprecated(sfKeyboard_isKeyPressed) /// Please use the focused version customKeyboardIsKeyPressed() instead
#pragma warning (disable: 4090 4244 4305 6031 26451)
#pragma warning (2: 4995)
#pragma warning (error: 4033 4715)

sfBool DEBUG_STATE;

typedef struct vector4f vector4f;
struct vector4f
{
	float r;
	float g;
	float b;
	float a;
};

typedef enum Direction Direction;
enum Direction {
	DIR_UP		= 1 << 0,
	DIR_DOWN	= 1 << 1,
	DIR_LEFT	= 1 << 2,
	DIR_RIGHT	= 1 << 3
};

//-----------------------------------------------------------
//	Fonction d'initialisation des tools
//	à mettre obligatoirement dans votre init 
//	pour utiliser les fonctions de cette bibliothèque
//-----------------------------------------------------------
void initTools();

/// <summary>
/// init the tools for the enemi thread
/// </summary>
void initToolsThread2();

/// <summary>
/// init the tools for the xp thread
/// </summary>
void initToolsThread3();

//-----------------------------------------------------------
//	Fonction vector2f ( float _x, float _y )
//  Renvoi un sfVector2f en partant de 2 float
//	_x float
//	_y float
//-----------------------------------------------------------
sfVector2f vector2f(float _x, float _y);

//-----------------------------------------------------------
//	Fonction vector2i ( int _x, int _y )
//  Renvoi un sfVector2i en partant de 2 int
//	_x int
//	_y int
//-----------------------------------------------------------
sfVector2i vector2i(int _x, int _y);

//-----------------------------------------------------------
//	Fonction vector3f ( float _x, float _y, float _z )
//  Renvoi un sfVector3f en partant de 3 float
//	_x float
//	_y float
//	_z float
//-----------------------------------------------------------
sfVector3f vector3f(float _x, float _y, float _z);

//------------------------------------------------------------------------------
//	Fonction FlRect ( float _left, float _top, float _width, float _height )
//  Renvoi un sfFloatRect en partant de 4 float
//	_left float
//	_top float
//	_width float
//	_height float
//------------------------------------------------------------------------------
sfFloatRect FlRect(float _left, float _top, float _width, float _height);

//-----------------------------------------------------------
//	Fonction vector2i ( int _left, int _top, int _width, int _height )
//  Renvoi un sfIntRect en partant de 4 int
//	_left int
//	_top int
// 	_width int
//	_height int
//-----------------------------------------------------------
sfIntRect IntRect(int _left, int _top, int _width, int _height);

//-----------------------------------------------------------
//	Fonction vector2i ( sfUint8 r, sfUint8 g, sfUint8 b, sfUint8 a )
//  Renvoi un sfColor en partant de 4 int
//	r rouge	: valeur entre 0 et 255
//	g vert	: valeur entre 0 et 255
// 	b bleu	: valeur entre 0 et 255
//	a alpha	: valeur entre 0 et 255
//-----------------------------------------------------------
sfColor color(sfUint8 r, sfUint8 g, sfUint8 b, sfUint8 a);


//-----------------------------------------------------------
//	Fonction lerp ( float )
//	v0 point de départ
//	v1 destination
//	t valeur entre 0.0f et 1.0f ( seul variable qui varie )
//-----------------------------------------------------------
float lerp(float v0, float v1, float t);
float lerp_smooth(float v0, float v1, float t);

//-----------------------------------------------------------
//	Fonction lerp ( vector2f )
//	v0 point de départ
//	v1 destination
//	t valeur entre 0.0f et 1.0f ( seul variable qui varie )
//-----------------------------------------------------------
sfVector2f vec2f_lerp(sfVector2f v0, sfVector2f v1, float t);

sfVector2f vec2f_lerp_smooth(sfVector2f v0, sfVector2f v1, float t);
sfVector2f vec2f_lerp_invSquare(sfVector2f v0, sfVector2f v1, float t);
sfVector2f vec2f_lerp_invCube(sfVector2f v0, sfVector2f v1, float t);

float float_lerp_invCube(float v0, float v1, float t);

//-----------------------------------------------------------
//	Fonction lerp ( vector3f )
//	v0 point de départ
//	v1 destination
//	t valeur entre 0.0f et 1.0f ( seul variable qui varie )
//-----------------------------------------------------------
sfVector3f vec3f_lerp(sfVector3f v0, sfVector3f v1, float t);

//-----------------------------------------------------------
//	Fonction lerp ( vector4f )
//	v0 point de départ
//	v1 destination
//	t valeur entre 0.0f et 1.0f ( seul variable qui varie )
//-----------------------------------------------------------
vector4f vec4f_lerp(vector4f v0, vector4f v1, float t);

//-----------------------------------------------------------
//	Fonction lerp ( sfColor )
//	v0 couleur de départ
//	v1 couleur de fin
//	t valeur entre 0.0f et 1.0f ( seul variable qui varie )
//-----------------------------------------------------------
sfColor color_lerp(sfColor v0, sfColor v1, float t);

sfColor color_lerp_no_opacity(sfColor v0, sfColor v1, float t);

//-----------------------------------------------------------
//	Fonction restartClock()
//	fonction à mettre dès le début de votre update
//	pour gèrer le temps
//-----------------------------------------------------------
void restartClock();

/// <summary>
/// restart the clock of the 2nd Thread
/// </summary>
void restartClockThread2();

/// <summary>
/// restart the clock of the 3rd Thread
/// </summary>
void restartClockThread3();

//-----------------------------------------------------------
//	Fonction GetDeltaTime()
//	Renvoi un float du temps passé entre 2 ticks
//-----------------------------------------------------------
float getDeltaTime();
float getDeltaTimeThread2();
float getDeltaTimeThread3();
float getUnscaledDeltaTime();
void setTimeScale(float _timeScale);
float getTimeScale();

//-----------------------------------------------------------
//	Fonction CreateSprite(char* _path)
//	Crée un sprite à partir d'une texture
//	_path le chemin vers la texture 
// 
//	renvoi un sfSprite*
//-----------------------------------------------------------
sfSprite* CreateSprite(char* _path);

//--------------------------------------------------------------------------------------------
//	Circle_Collision(sfVector2f _pos1, sfVector2f _pos2, float _rayon1, float _rayon2)
//	Vérifie si 2 cercles sont en collision
//	_pos1	position du 1er cercle
//	_pos2	position du 2eme cercle
//	_rayon1	rayon du 1er cercle
//	_rayon2	rayon du 2eme cercle
// 
// renvoi un sfBool, sfTrue si il y a collision, sfFalse si il n'y a pas collision
//--------------------------------------------------------------------------------------------
sfBool Circle_Collision(sfVector2f _pos1, sfVector2f _pos2, float _rayon1, float _rayon2);

//--------------------------------------------------------------------------------------------
//	Rectangle_Collision(sfFloatRect _box1, sfFloatRect _box2)
//	Vérifie si 2 rectangles sont en collision
//	_box1	position, largeur et hauteur du 1er rectangle
//	_box2	position, largeur et hauteur du 2eme rectangle
// 
// renvoi un sfBool, sfTrue si il y a collision, sfFalse si il n'y a pas collision
//--------------------------------------------------------------------------------------------
sfBool Rectangle_Collision(sfFloatRect _box1, sfFloatRect _box2);

//-----------------------------------------------------------
//	Fonction iRand ( int _min, int _max )
//	_min valeur minimum de random
//	_max valeur maximum de random
// 
//	si _max < _min, _min sera toujours renvoyé
// 
//	renvoi un int compris entre _min et _max
//-----------------------------------------------------------
int iRand(int _min, int _max);

//-----------------------------------------------------------
//	Fonction rand_int ( int _min, int _max )
//	_min valeur minimum de random
//	_max valeur maximum de random
// 
//	renvoi un int compris entre _min et _max
//-----------------------------------------------------------
int rand_int(int _min, int _max);

//-----------------------------------------------------------
//	Fonction rand_float ( float _min, float _max )
//	_min valeur minimum de random
//	_max valeur maximum de random
// 
//	renvoi un double compris entre _min et _max
//-----------------------------------------------------------
double rand_float(float _min, float _max);

/// <summary>
/// give you a random color with a given opacity
/// </summary>
/// <param name="a">the opacity between 0 and 255</param>
/// <returns></returns>
sfColor randColor(int a);

//-----------------------------------------------------------
//	Fonction FrameRate 
//-----------------------------------------------------------
float FrameRate();

//-----------------------------------------------------------
//	Fonction FrameRate thread 2
//-----------------------------------------------------------
float FrameRateThrd2();

//-----------------------------------------------------------
//	Fonction FrameRate thread 3
//-----------------------------------------------------------
float FrameRateThrd3();

//-----------------------------------------------------------
//	Fonction displayFrameRate
//----------------------------------------------------------- 
void displayFrameRate(sfRenderTexture* _window, sfText* _text);

//-----------------------------------------------------------
//	Fonction displayFrameRate thread 2
//----------------------------------------------------------- 
void displayFrameRateThrd2(sfRenderTexture* _window, sfText* _text);

//-----------------------------------------------------------
//	Fonction displayFrameRate thread 3
//----------------------------------------------------------- 
void displayFrameRateThrd3(sfRenderTexture* _window, sfText* _text);

//-----------------------------------------------------------
//	Fonction screenshot
//-----------------------------------------------------------
void screenshot(sfRenderWindow* _window);

/// <summary>
/// Check if an input is pressed while you have focus on the window
/// </summary>
/// <param name="key">the key pressed</param>
/// <param name="_window">the render window</param>
/// <returns>true if the key is pressed and the focus is on the window false if otherwise</returns>
sfBool customKeyboardIsKeyPressed(sfKeyCode key, sfRenderWindow* _window);

/// <summary>
/// Sets the text's origin to his middle
/// </summary>
void centralText(sfText* _txt);

/// <summary>
/// Sets the rect's origin to his middle
/// </summary>
void centralRect(sfRectangleShape* _rect); 

/// <summary>
/// Sets the sprite's origin to his middle
/// </summary>
void centralSprite(sfSprite* _spr);
/// <summary>
/// Sets the circle's origin to his middle
/// </summary>
void centralCircle(sfCircleShape* _circle);