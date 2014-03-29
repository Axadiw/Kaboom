#pragma once

#include <fmod.hpp>

#include "../includes.h"

enum SOUND_TYPE {SFX, MUSIC};

/*! 
 *  \brief     D�wi�k
 *  \details Wykorzystywanie podczas wy�wietlania animacji
 *  \author    Micha� Mizera
 *  \version   1.0
 *  \date      2011-2012
 *  \copyright GNU Public License.
 */

class Sound 
{
public:
	Sound():name(NULL), sound(NULL), channel(NULL), type(SFX){};

	/// �cie�ka do d�wi�ku
	const char* name;
	
	/// rodzaj d�wi�ku (muzyka czy SFX)
	SOUND_TYPE type;	

	/// czy d�wi�k zosta� utworzony i poprawnie wczytany
	inline bool isDummy() { return sound == NULL || channel == NULL; }

	/// odtwarza d�wi�k
	inline bool play() { return system->playSound(FMOD_CHANNEL_FREE, sound, false, &channel) == FMOD_OK; };

	/// pauzyje d�wi�k
	inline bool pause() { return channel->setPaused(true) == FMOD_OK; };

	/// ustawia g�o�no�� dla danego d�wi�ku
	inline bool setVolume(float volume) { return channel->setVolume(volume) == FMOD_OK; };

	/// wraca true, je�eli d�wi�k jest odtwarzany (mo�e dzia�a� niepoprawnie dla zap�tlonych d�wi�k�w)
	inline bool isPlaying() { bool tmp; channel->isPlaying(&tmp); return tmp; };

	friend class SoundManager;
private:
	int init(const char*, FMOD::System *, bool looped);	
	inline bool release() { return sound->release() == FMOD_OK; }

	FMOD::System *system;

	FMOD::Sound *sound;
	FMOD::Channel *channel;
};