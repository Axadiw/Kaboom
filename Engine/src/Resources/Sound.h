#pragma once

#include <fmod.hpp>

#include "../includes.h"

enum SOUND_TYPE {SFX, MUSIC};

/*! 
 *  \brief     DŸwiêk
 *  \details Wykorzystywanie podczas wyœwietlania animacji
 *  \author    Micha³ Mizera
 *  \version   1.0
 *  \date      2011-2012
 *  \copyright GNU Public License.
 */

class Sound 
{
public:
	Sound():name(NULL), sound(NULL), channel(NULL), type(SFX){};

	/// œcie¿ka do dŸwiêku
	const char* name;
	
	/// rodzaj dŸwiêku (muzyka czy SFX)
	SOUND_TYPE type;	

	/// czy dŸwiêk zosta³ utworzony i poprawnie wczytany
	inline bool isDummy() { return sound == NULL || channel == NULL; }

	/// odtwarza dŸwiêk
	inline bool play() { return system->playSound(FMOD_CHANNEL_FREE, sound, false, &channel) == FMOD_OK; };

	/// pauzyje dŸwiêk
	inline bool pause() { return channel->setPaused(true) == FMOD_OK; };

	/// ustawia g³oœnoœæ dla danego dŸwiêku
	inline bool setVolume(float volume) { return channel->setVolume(volume) == FMOD_OK; };

	/// wraca true, je¿eli dŸwiêk jest odtwarzany (mo¿e dzia³aæ niepoprawnie dla zapêtlonych dŸwiêków)
	inline bool isPlaying() { bool tmp; channel->isPlaying(&tmp); return tmp; };

	friend class SoundManager;
private:
	int init(const char*, FMOD::System *, bool looped);	
	inline bool release() { return sound->release() == FMOD_OK; }

	FMOD::System *system;

	FMOD::Sound *sound;
	FMOD::Channel *channel;
};