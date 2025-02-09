#pragma once
#include "tools.h"
#include "windowManager.h"

/// <summary>
/// inits the fade manager in the game state
/// </summary>
void fade_Init();

/// <summary>
/// updates the fade manager in the game state
/// </summary>
void fade_Update();

/// <summary>
/// displays the fade manager in the game state
/// </summary>
/// <param name="_window">the current window</param>
void fade_Display(Window* _window);

/// <summary>
/// destroys the fade manager if the game state is left
/// </summary>
/// <param name="_window">the current window</param>
void fade_Deinit();

void fade_SetPosition(sfVector2f _newPos);

/// <summary>
/// sets a new opacity for the black rectangle shape above the game state
/// </summary>
/// <param name="_new_opacity">the new opacity value</param>
void fade_SetOpacity(int _new_opacity);

/// <summary>
/// sets a new opacity for a sprite that's fading
/// </summary>
/// <param name="_new_opacity">the new opacity value</param>
void fade_SetSpriteOpacity(int _new_opacity);

/// <summary>
/// sets a new opacity for a sprite that's fading
/// </summary>
/// <param name="_new_opacity">the new opacity value</param>
void fade_SetSprite2Opacity(int _new_opacity);

void fade_SetSprite3Opacity(int _new_opacity);

/// <summary>
/// sets a new opacity for a text that's fading
/// </summary>
/// <param name="_new_opacity">the new opacity value</param>
void fade_SetTextOpacity(int _new_opacity);

/// <summary>
/// sets a new opacity for a circle that's fading
/// </summary>
/// <param name="_new_opacity">the new opacity value</param>
void fade_SetCircleOpacity(int _new_opacity);

/// <summary>
/// sets a new opacity for a rectangle that's fading
/// </summary>
/// <param name="_new_opacity">the new opacity value</param>
void fade_SetRectOpacity(int _new_opacity);

/// <summary>
/// returns the opacity of the black rectangle shape above the game state
/// </summary>
int fade_GetOpacity();

/// <summary>
/// sets the size of the black rectangle shape above the game state
/// <param name="_size">the new size of the rectangle</param>
/// </summary>
void fade_SetSize(sfVector2f _size);

/// <summary>
/// returns the opacity of the text
/// </summary>
int fade_GetTextOpacity();

/// <summary>
/// returns the opacity of the sprite
/// </summary>
int fade_GetSpriteOpacity();

int fade_GetSpriteOpacity3();

/// <summary>
/// returns the opacity of the 2nd sprite
/// </summary>
int fade_GetSpriteOpacity2();

/// <summary>
/// returns the opacity of the rectangle
/// </summary>
int fade_GetRectangleOpacity();

/// <summary>
/// returns the opacity of the circle
/// </summary>
int fade_GetCircleOpacity();

/// <summary>
/// fades the entire screen in
/// </summary>
/// <param name="_timer">how much time for the screen to completely fade in</param>
void fadeIn(float _timer);

/// <summary>
/// fades the entire screen out
/// </summary>
/// <param name="_timer">how much time for the screen to completely fade out</param>
void fadeOut(float _timer);

/// <summary>
/// fades a singular sprite in
/// </summary>
/// <param name="_timer">how much time for the sprite to completely fade in</param>
/// <param name="_spr">the sprite you want to fade</param>
void fadeInSprite(float _timer, sfSprite* _spr);

/// <summary>
/// fades a singular sprite out
/// </summary>
/// <param name="_timer">how much time for the sprite to completely fade out</param>
/// <param name="_spr">the sprite you want to fade</param>
void fadeOutSprite(float _timer, sfSprite* _spr);

/// <summary>
/// fades a singular sprite in
/// </summary>
/// <param name="_timer">how much time for the sprite to completely fade in</param>
/// <param name="_spr">the sprite you want to fade</param>
void fadeInSprite3(float _timer, sfSprite* _spr);

/// <summary>
/// fades a singular sprite out
/// </summary>
/// <param name="_timer">how much time for the sprite to completely fade out</param>
/// <param name="_spr">the sprite you want to fade</param>
void fadeOutSprite3(float _timer, sfSprite* _spr);

/// <summary>
/// fades a singular sprite in
/// </summary>
/// <param name="_timer">how much time for the sprite to completely fade in</param>
/// <param name="_spr">the sprite you want to fade</param>
void fadeInSprite2(float _timer, sfSprite* _spr);

/// <summary>
/// fades a singular sprite out
/// </summary>
/// <param name="_timer">how much time for the sprite to completely fade out</param>
/// <param name="_spr">the sprite you want to fade</param>
void fadeOutSprite2(float _timer, sfSprite* _spr);

/// <summary>
/// fades a singular text in
/// </summary>
/// <param name="_timer">how much time for the text to completely fade in</param>
/// <param name="_txt">the text you want to fade</param>
void fadeInText(float _timer, sfText* _txt);

/// <summary>
/// fades a singular text out
/// </summary>
/// <param name="_timer">how much time for the text to completely fade out</param>
/// <param name="_text">the text you want to fade</param>
void fadeOutText(float _timer, sfText* _txt);

/// <summary>
/// fades a singular text in
/// </summary>
/// <param name="_timer">how much time for the 2nd text to completely fade in</param>
/// <param name="_txt">the text you want to fade</param>
void fadeInText2(float _timer, sfText* _txt);

/// <summary>
/// fades a singular text out
/// </summary>
/// <param name="_timer">how much time for the 2nd text to completely fade out</param>
/// <param name="_text">the text you want to fade</param>
void fadeOutText2(float _timer, sfText* _txt);

/// <summary>
/// fades a singular rectangle in
/// </summary>
/// <param name="_timer">how much time for the rectangle to completely fade in</param>
/// <param name="_rectangle">the rectangle you want to fade</param>
void fadeInRectangle(float _timer, sfRectangleShape* _rectangle);

/// <summary>
/// fades a singular rectangle out
/// </summary>
/// <param name="_timer">how much time for the rectangle to completely fade out</param>
/// <param name="_rectangle">the rectangle you want to fade</param>
void fadeOutRectangle(float _timer, sfRectangleShape* _rectangle);

/// <summary>
/// fades a singular circle in
/// </summary>
/// <param name="_timer">how much time for the circle to completely fade in</param>
/// <param name="_circle">the circle you want to fade</param>
void fadeInCircle(float _timer, sfCircleShape* _circle);

int fade_GetTextOpacity2();

void fade_SetTextOpacity2(int _new_opacity);
/// <summary>
/// fades a singular circle out
/// </summary>
/// <param name="_timer">how much time for the circle to completely fade out</param>
/// <param name="_circle">the circle you want to fade</param>
void fadeOutCircle(float _timer, sfCircleShape* _circle);

void walkingFade_SetPosition(sfVector2f _newPosition);

sfVector2f walkingFade_GetPosition();

void walkingFade_Draw(Window* _window);

void walkingFade_Init();

void walkingFade_Update();