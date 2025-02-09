#pragma once
#include "tools.h"
#include "ressourcesManager.h"

typedef struct Shader Shader;
struct Shader
{
	Shader* pNext;
	char name[30];
	State state;
	sfRenderStates rs;
};

Shader* shdBegin;

/// <summary>
/// Load the shaders of a given state
/// </summary>
/// <param name="_state">the current state</param>
void shaderOnload(State _state);

/// <summary>
/// Give a shader with a given name
/// </summary>
/// <param name="_name">the shader name</param>
/// <returns>the shader</returns>
sfShader* GetShader(char* _name);

/// <summary>
/// Give a render state with a given name
/// </summary>
/// <param name="_name">the render state name</param>
/// <returns>the render state</returns>
sfRenderStates* GetRenderState(char* _name);

/// <summary>
/// Add a shader to the loaded ressources
/// </summary>
/// <param name="_shd">The font</param>
void AddShader(Shader* _shd);

/// <summary>
/// Remove a shader to the loaded ressources
/// </summary>
/// <param name="_shd"></param>
/// <returns></returns>
Shader* RemoveShader(Shader* _shd);

/// <summary>
/// Remove all the loaded shaders
/// </summary>
void RemoveAllShaders();