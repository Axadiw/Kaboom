#include <assert.h>

#include "SoundManager.h"
#include "../includes.h"
#include "SDL_image.h"

SoundManager::SoundManager(): SFXVolume(1.0f), MusicVolume(1.0f)
{
	if(FMOD::System_Create(&system)!= FMOD_OK ) 
	{
		AX_PRINT_ERROR("Blad przy tworzeniu obiektu System");
	}

	if(system->init(SOUND_CHANNELS, FMOD_INIT_NORMAL,NULL)!=FMOD_OK)
	{
		AX_PRINT_ERROR("Inicjalizacja nie powiod³a siê");
	}	
}

SoundManager::~SoundManager()
{
	system->release();
}

Sound* SoundManager::loadSound(const char* path, SOUND_TYPE type, bool looped)
{
	if(sounds.find(path) != sounds.end())
	{
		/// font juz otwarty, zwieksz licznik odwo³añ
		(soundsCount[path])++;

		return sounds[path];
	}

	Sound* retSound = new Sound();
	/// inicjowanie dŸwieku	
	
	if(retSound->init(path, system, looped) != 0)
	{
		/// blad przy wczytywaniu dŸwiêku
		delete retSound;
		return NULL;
	}
	
	retSound->type = type;	
	retSound->setVolume(type==SFX?SFXVolume:MusicVolume);

	sounds[path] = retSound;
	soundsCount[path] = 1;

	return retSound;
}

void SoundManager::releaseSound(Sound* sound)
{
	stdext::hash_map<const char*, Sound*>::iterator itSound = sounds.find(sound->name);
	stdext::hash_map<const char*, int>::iterator itCount = soundsCount.find(sound->name);

	if(itSound == sounds.end() || itCount == soundsCount.end() || sound->isDummy())
	{
		/// nie ma takiego fonta w bazie, lub font jest pusty
		return;
	}

	(itCount->second)--;

	if(itCount->second <= 0)
	{
		/// kasowanie dŸwiêku
		itSound->second->release();
		delete itSound->second;

		sounds.erase(itSound);
		soundsCount.erase(itCount);
	}
}

bool SoundManager::setSFXVolume(float volume)
{
	SFXVolume = volume;

	for(stdext::hash_map<const char*, Sound*>::iterator itSound = sounds.begin(); itSound != sounds.end(); ++itSound)
	{
		if(itSound->second->type == SFX)
		{
			if(!itSound->second->setVolume(volume))
			{
				return false;
			}
		}
	}	

	return true;
}


bool SoundManager::setMusicVolume(float volume)
{
	MusicVolume = volume;

	for(stdext::hash_map<const char*, Sound*>::iterator itSound = sounds.begin(); itSound != sounds.end(); ++itSound)
	{
		if(itSound->second->type == MUSIC)
		{
			if(!itSound->second->setVolume(volume))
			{
				return false;
			}
		}
	}	

	return true;
}