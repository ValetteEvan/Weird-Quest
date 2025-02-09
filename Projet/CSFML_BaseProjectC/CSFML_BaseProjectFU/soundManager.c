#include "soundManager.h"
#include "options.h"

static sfBool firstPassSound = sfFalse;
void soundsOnload(State _state)
{
	if (!firstPassSound) {
		firstPassSound = sfTrue;
		soundBegin = 0;
	}

	ressources* tempRessources = ResBegin;
	while (tempRessources != NULL)
	{
		if (tempRessources->type == SOUNDFX)
		{
			if (tempRessources->state == _state)
			{
				Sound* tempSound = (Sound*)calloc(1, sizeof(Sound));
				strcpy(tempSound->name, tempRessources->name);
				tempSound->type = tempRessources->type;
				tempSound->state = tempRessources->state;

				tempSound->soundFX = sfSound_create();
				tempSound->sBuff = sfSoundBuffer_createFromFile(tempRessources->path);
				sfSound_setBuffer(tempSound->soundFX, tempSound->sBuff);
				sfSound_setVolume(tempSound->soundFX, SFXVolume);
				AddSound(tempSound);
			}
		}
		else if (tempRessources->type == MUSIC)
		{
			if (tempRessources->state == _state)
			{
				if (tempRessources != NULL)
				{
					Sound* tempSound = (Sound*)calloc(1, sizeof(Sound));
					strcpy(tempSound->name, tempRessources->name);
					tempSound->type = tempRessources->type;
					tempSound->state = tempRessources->state;

					tempSound->music = sfMusic_createFromFile(tempRessources->path);
					sfMusic_setLoop(tempSound->music, sfTrue);
					sfMusic_setVolume(tempSound->music, musicVolume);
					AddSound(tempSound);
				}
			}
		}
		tempRessources = tempRessources->pNext;
	}
}

void setVolume() {
	Sound* tempSound = soundBegin;
	seting.musicVol = musicVolume;
	seting.sfxVol = SFXVolume;
	FILE* fichier = fopen("..\\Ressources\\Save\\settings.txt", "w");
	fprintf(fichier, "%d %f %f", seting.fullScreen, seting.musicVol, seting.sfxVol);
	fclose(fichier);
	while (tempSound != NULL)
	{
		if (tempSound->type == MUSIC) {
			sfMusic_setVolume(tempSound->music, musicVolume);
		}
		else {
			sfSound_setVolume(tempSound->soundFX, SFXVolume);
		}
		tempSound = tempSound->pNext;
	}
}

void playSoundFX(char* _name)
{
	Sound* tempSound = soundBegin;
	while (tempSound != NULL)
	{
		if ((strcmp(_name, tempSound->name)) == 0)
		{
			if (tempSound->type == SOUNDFX)
				sfSound_play(tempSound->soundFX);
		}
		tempSound = tempSound->pNext;
	}
}

void playMusic(char* _name)
{
	Sound* tempSound = soundBegin;
	while (tempSound != NULL)
	{
		if ((strcmp(_name, tempSound->name)) == 0)
		{
			if (tempSound->type == MUSIC)
			{
				sfMusic_play(tempSound->music);
				sfMusic_setLoop(tempSound->music, sfTrue);
			}
		
		}
		tempSound = tempSound->pNext;
	}
}

void StopALLMusic()
{
	Sound* tempSound = soundBegin;
	while (tempSound != NULL)
	{
		if (tempSound->type == MUSIC)
		{
			stopMusicIfPlaying(tempSound->name);
		}
		tempSound = tempSound->pNext;
	}
}

void playMusicIfNotPlaying(char* _name)
{
	Sound* tempSound = soundBegin;
	while (tempSound != NULL)
	{
		if ((strcmp(_name, tempSound->name)) == 0)
		{
			if (tempSound->type == MUSIC)
			{
				if (sfMusic_getStatus(tempSound->music) != sfPlaying)
				{
					sfMusic_play(tempSound->music);
					sfMusic_setLoop(tempSound->music, sfTrue);
				}
				if (_name == "Music_River")
				{
					sfMusic_setVolume(tempSound->music, 20);
				}
			}
		}
		tempSound = tempSound->pNext;
	}
}

void stopMusicIfPlaying(char* _name)
{
	Sound* tempSound = soundBegin;
	while (tempSound != NULL)
	{
		if ((strcmp(_name, tempSound->name)) == 0)
		{
			if (tempSound->type == MUSIC)
			{
				if ((sfMusic_getStatus(tempSound->music) != sfStopped))
				{
					sfMusic_stop(tempSound->music);
				}
			}
		}
		tempSound = tempSound->pNext;
	}
}

void stopMusic(char* _name)
{
	Sound* tempSound = soundBegin;
	while (tempSound != NULL)
	{
		if ((strcmp(_name, tempSound->name)) == 0)
		{
			if (tempSound->type == MUSIC)
				sfMusic_stop(tempSound->music);
		}
		tempSound = tempSound->pNext;
	}
}

sfSound* GetSound(char* _name)
{
	Sound* tempSound = soundBegin;
	while (tempSound != NULL)
	{
		if ((strcmp(_name, tempSound->name)) == 0)
		{
			if (tempSound->type == SOUNDFX)
				return tempSound->soundFX;
		}
		tempSound = tempSound->pNext;
	}
	return NULL;
}

sfMusic* GetMusic(char* _name)
{
	Sound* tempSound = soundBegin;
	while (tempSound != NULL)
	{
		if ((strcmp(_name, tempSound->name)) == 0)
		{
			if (tempSound->type == MUSIC)
				return tempSound->music;
		}
		tempSound = tempSound->pNext;
	}
	return NULL;
}

void AddSound(Sound* _sound)
{
	_sound->pNext = soundBegin;
	soundBegin = _sound;
}

Sound* RemoveSound(Sound* _sound) {
	if (_sound == soundBegin)
	{
		Sound* tempSound = _sound->pNext;
		soundBegin = tempSound;
		if (tempSound->type == MUSIC) {
			sfMusic_destroy(tempSound->music);
		}
		else {
			sfSoundBuffer_destroy(tempSound->sBuff);
			sfSound_destroy(tempSound->soundFX);
		}
		free(_sound);
		return tempSound;
	}
	else
	{
		Sound* tempSound = soundBegin;
		while (tempSound->pNext != _sound)
		{
			tempSound = tempSound->pNext;
		}
		tempSound->pNext = _sound->pNext;
		free(_sound);
		return tempSound->pNext;
	}
}

void RemoveAllSounds() {
	Sound* tempSound = soundBegin;
	while (tempSound != NULL)
	{
		Sound* todelete = tempSound;
		tempSound = tempSound->pNext;
		if (todelete->type == MUSIC) {
			sfMusic_destroy(todelete->music);
		}
		else {
			sfSoundBuffer_destroy(todelete->sBuff);
			sfSound_destroy(todelete->soundFX);
		}
		free(todelete);
	}
	soundBegin = 0;
}

void RemoveAllSoundsNotNeededInEveryStates() {
	Sound* tempSound = soundBegin;
	while (tempSound != NULL)
	{
		if (tempSound->state != ALL)
		{
			if (tempSound == soundBegin)
			{
				Sound* tempSound2 = tempSound->pNext;
				soundBegin = tempSound2;
				if (tempSound->type == MUSIC) {
					sfMusic_destroy(tempSound->music);
				}
				else {
					sfSoundBuffer_destroy(tempSound->sBuff);
					sfSound_destroy(tempSound->soundFX);
				}
				free(tempSound);
				tempSound = tempSound2;
			}
			else
			{
				Sound* toDelete = tempSound;
				tempSound = tempSound->pNext;
				if (tempSound->type == MUSIC) {
					sfMusic_destroy(tempSound->music);
				}
				else {
					sfSoundBuffer_destroy(tempSound->sBuff);
					sfSound_destroy(tempSound->soundFX);
				}
				free(toDelete);
			}
		}
		else
		{
			tempSound = tempSound->pNext;
		}
	}
}

void pauseAllSfx()
{
	Sound* tempSound = soundBegin;
	while (tempSound != NULL) {
		if (tempSound->type == SOUNDFX) {
			sfSound_pause(tempSound->soundFX);
		}
		tempSound = tempSound->pNext;
	}
}

void playAllSfx()
{
	Sound* tempSound = soundBegin;
	while (tempSound != NULL) {
		if (tempSound->type == SOUNDFX) {
			if (sfSound_getStatus(tempSound->soundFX) == sfPaused)
				sfSound_play(tempSound->soundFX);
		}
		tempSound = tempSound->pNext;
	}
}

void pauseAllSounds()
{
	Sound* tempSound = soundBegin;
	while (tempSound != NULL) {
		if (tempSound->type == MUSIC) {
			sfMusic_pause(tempSound->music);
		}
		if (tempSound->type == SOUNDFX) {
			sfSound_pause(tempSound->soundFX);
		}
		tempSound = tempSound->pNext;
	}
}

void resumeAllSounds()
{
	Sound* tempSound = soundBegin;
	while (tempSound != NULL) {
		if (tempSound->type == MUSIC) {
			sfMusic_play(tempSound->music);
		}
		if (tempSound->type == SOUNDFX) {
			sfSound_play(tempSound->soundFX);
		}
		tempSound = tempSound->pNext;
	}
}

void removeAllByState(State _state)
{
	Sound* tempSound = soundBegin;
	while (tempSound != NULL) {
		if (tempSound->state == _state) {
			if (tempSound == soundBegin)
			{
				Sound* tempSound2 = tempSound->pNext;
				soundBegin = tempSound2;
				if (tempSound->type == MUSIC) {
					sfMusic_destroy(tempSound->music);
				}
				else {
					sfSoundBuffer_destroy(tempSound->sBuff);
					sfSound_destroy(tempSound->soundFX);
				}
				free(tempSound);
				tempSound = tempSound2;
			}
			else
			{
				Sound* toDelete = tempSound;
				tempSound = tempSound->pNext;
				if (tempSound->type == MUSIC) {
					sfMusic_destroy(tempSound->music);
				}
				else {
					sfSoundBuffer_destroy(tempSound->sBuff);
					sfSound_destroy(tempSound->soundFX);
				}
				free(toDelete);
			}
		}
	}
	if (tempSound != NULL)
		tempSound = tempSound->pNext;	
}

void SoundAround(sfVector2f _pos, float radius, char* _name)
{
	Sound* tempSound = soundBegin;
	while (tempSound != NULL)
	{
		if ((strcmp(_name, tempSound->name)) == 0)
		{
			if (tempSound->type == SOUNDFX)
			{
				sfVector2f soundPos = vector2f(sfSound_getPosition(tempSound->soundFX).x, sfSound_getPosition(tempSound->soundFX).y);
				float distance = sqrt(pow(soundPos.x - _pos.x, 2) + pow(soundPos.y - _pos.y, 2));
				if (distance < radius)
				{
					sfSound_play(tempSound->soundFX);
				}
				sfSound_setVolume(tempSound->soundFX, SFXVolume * (1 - distance / radius));
			}
		}
		else if (tempSound->type == MUSIC)
		{
			sfVector2f soundPos = vector2f(sfMusic_getPosition(tempSound->music).x, sfMusic_getPosition(tempSound->music).y);
			float distance = sqrt(pow(soundPos.x - _pos.x, 2) + pow(soundPos.y - _pos.y, 2));
			if (distance < radius)
			{
				sfMusic_play(tempSound->music);
			}
			sfMusic_setVolume(tempSound->music, musicVolume * (1 - distance / radius));
		}
		tempSound = tempSound->pNext;
	}
}