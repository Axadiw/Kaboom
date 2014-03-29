#include "Sound.h"

#include "ConfigManager.h"


int Sound::init(const char* name_, FMOD::System * system_, bool looped)
{
	system = system_;
	name = name_;

	if(system->createSound(name, looped?FMOD_LOOP_NORMAL:FMOD_DEFAULT, 0, &sound)!=FMOD_OK)
	{
		AX_PRINT_ERROR("nie uda�o si� utworzy� d�wi�ku");
		return -1;
	}

	if(system->playSound(FMOD_CHANNEL_FREE, sound, true, &channel)!=FMOD_OK)
	{
		AX_PRINT_ERROR("nie uda�o si� odtworzy� d�wi�ku");
		return -2;
	}

	return 0;
}