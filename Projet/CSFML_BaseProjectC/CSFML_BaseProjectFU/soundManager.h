#pragma once
#include "tools.h"
#include "SFML/Audio.h"
#include "ressourcesManager.h"

typedef struct Sound Sound;
struct Sound
{
	Sound* pNext;
	char name[30];
	ressourceType type;
	State state;
	union MyUnion
	{
		sfMusic* music;
		sfSound* soundFX;
	};
	sfSoundBuffer* sBuff;
};

float SFXVolume;
float musicVolume;

Sound* soundBegin;

/// <summary>
/// load the sounds of a given state
/// </summary>
/// <param name="_state">the current state</param>
void soundsOnload(State _state);

/// <summary>
/// set the music and fx volume with the updated settings
/// </summary>
void setVolume();

/// <summary>
/// play a given sound
/// </summary>
void playSoundFX(char* _name);

/// <summary>
/// play a given music
/// </summary>
void playMusic(char* _name);

/// <summary>
/// stop a given music
/// </summary>
void stopMusic(char* _name);

/// <summary>
/// get a sound with a given name
/// </summary>
/// <param name="_name">the sound's name</param>
/// <returns>the sound</returns>
sfSound* GetSound(char* _name);

/// <summary>
/// get a music with a given name
/// </summary>
/// <param name="_name">the music's name</param>
/// <returns>the sound</returns>
sfMusic* GetMusic(char* _name);

/// <summary>
/// add a given sound
/// </summary>
/// <param name="_sound">the given sound</param>
void AddSound(Sound* _sound);

/// <summary>
/// remove a given sound
/// </summary>
/// <param name="_sound">the sound to delete</param>
/// <returns>the updated sound chained list</returns>
Sound* RemoveSound(Sound* _sound);

/// <summary>
/// deinit all the sounds
/// </summary>
void RemoveAllSounds();

/// <summary>
/// deinit all the sounds that are not needed in every states
/// </summary>
void RemoveAllSoundsNotNeededInEveryStates();

/// <summary>
/// pause every sfx
/// </summary>
void pauseAllSfx();

/// <summary>
/// play every paused sfx
/// </summary>
void playAllSfx();

/**
 * @brief Pause all sounds.
 */
void pauseAllSounds();

/**
 * @brief Resume all paused sounds.
 */
void resumeAllSounds();

/**
 * @brief Remove all sounds with a given state.
 * 
 * @param _state The state of the sounds to remove.
 */
void removeAllByState(State _state);


/**
 * @brief Play the specified music if it is not already playing.
 * 
 * @param _name The name of the music to play.
 */
void playMusicIfNotPlaying(char* _name);

/**
 * @brief Stop the specified music if it is playing.
 * 
 * @param _name The name of the music to stop.
 */
void stopMusicIfPlaying(char* _name);

/**
 * @brief Play a sound around a given position within a specified radius.
 * 
 * @param _pos The position of the sound.
 * @param radius The radius within which the sound should be played.
 * @param _name The name of the sound to play.
 */
void SoundAround(sfVector2f _pos, float radius, char* _name);

void StopALLMusic();