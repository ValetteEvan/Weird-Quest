#pragma once
#include "ressourcesManager.h"

typedef struct Texture Texture;
struct Texture
{
	Texture* pNext;
	char name[30];
	ressourceType type;
	State state;
	Biomes biome;
	sfTexture* sftexture;
	int compteur;
	sfIntRect spSheetRec;
	float frameTime;
	int NbFrame;
	int row;
};

 typedef enum 
{
	TEXNULL = -1, PATH, CONFIG
}StateTexture;

Texture* textureBegin;

/// <summary>
/// load the textures of a given state
/// </summary>
/// <param name="">the current state</param>
void Texture_Onload(State _state);

void BiomeTexture_Onload(State _state, Biomes _biome);

/// <summary>
/// get a texture with the texture's name
/// </summary>
/// <param name="_name">the texture name</param>
/// <returns>the texture</returns>
sfTexture* GetSfTexture(char* _name);

Texture* GetTexture(char* _name); 

void GetAnimationDatas(char* _name, int* _frameX, float* _timerAnim, sfIntRect* _rectAnim, ressourceType* _resType);

void GetParticleAnimationDatas(char* _name, int* _frameXMax, sfIntRect* _rectAnim);

sfVector2f getAnimationSize(char* _name);

/// <summary>
/// get the amount of frames in an animation
/// </summary>
/// <param name="_name">the texture name</param>
/// <returns>the amount of frames in an animation</returns>
int GetNbFrame(char* _name);

/// <summary>
/// get the animation time between each frame of a given texture
/// </summary>
/// <param name="_name">the texture name</param>
/// <returns></returns>
float GetFrameTime(char* _name);

/// <summary>
/// get the texture rect of a given texture
/// </summary>
/// <param name="_name">the texture name</param>
/// <returns>the rect of the texture</returns>
sfIntRect GetRect(char* _name);

int getMaxFrameX(char* _name, int frameY);

/// <summary>
/// add a givven texture
/// </summary>
/// <param name="_texture">texture to add</param>
void AddTexture(Texture* _texture);

/// <summary>
/// remove a given texture
/// </summary>
/// <param name="_texture">texture to delete</param>
/// <returns></returns>
Texture* RemoveTexture(Texture* _texture);

/// <summary>
/// deinit all the textures
/// </summary>
void RemoveAllTexture();

/// <summary>
/// deinit all the textures that are not needed in every states
/// </summary>
void RemoveAllTextureButALL();

void RemoveAllBiomeTexture(Biomes _biome);

/// <summary>
/// play an animation in an animation with 1 line
/// </summary>
/// <param name="_textureName">the texture name</param>
/// <param name="_spr">the current sprite</param>
/// <param name="currentframex">the current frame x</param>
/// <param name="_timer">the current timer</param>
sfIntRect animatorAnim(char* _textureName, int* currentframex, float* _timer, int thread);


/// <summary>
/// play an animation in a sprite sheet
/// </summary>
/// <param name="_textureName">the texture name</param>
/// <param name="currentframex">the current frame x</param>
/// <param name="_timer">the timer</param>
/// <param name="frameY">the current line number (needed only for sprite sheets)</param>
/// <param name="nbFrameX">the amount of sprites in the line (needed only for sprite sheets if you have less than the max amount of sprites in this line)</param>
sfIntRect animator(char* _textureName, int* currentframex, float* _timer, int _frameY, int _nbFrameX, int thread);

sfIntRect animatorWithBeginFrame(char* _textureName, int* currentframex, float* _timer, int thread, int _frameXEnd);

sfIntRect animatorReverse(char* _textureName, int* currentframex, float* _timer, int thread, int _frameXReset);

sfIntRect animatorWithResize(char* _textureName, int* currentframex, float* _timer, int thread, sfRectangleShape* _rect);

sfIntRect animatorWithResizeAndOrigin(char* _textureName, int* currentframex, float* _timer, int thread, sfRectangleShape* _rect, sfBool _isOriginMiddleDown, sfVector2f _origin);

sfIntRect animatorWithResizeAndOriginOffset(char* _textureName, int* currentframex, float* _timer, int thread, sfRectangleShape* _rect, sfBool _isOriginMiddleDown, sfVector2f _originOffset);

