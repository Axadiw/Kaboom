#pragma once

#include <hash_map>
#include "../Singleton.h"
#include "Sound.h"
#include "SDL.h"
#include "SDL_opengl.h"

#define SOUND_CHANNELS 100

/*! 
 *  \brief     Menad¿er dŸwiêków
 *  \author    Micha³ Mizera
 *  \version   1.0
 *  \date      2011-2012
 *  \copyright GNU Public License.
 */

class SoundManager : public Singleton<SoundManager>
{
public:
	SoundManager();
	~SoundManager();

	/*!
	£aduje zadany dŸwiêk z dysku
	@param path œcie¿ka do dŸwiêku
	@param type rodzaj dŸwiêku (muzyka czy SFX)
	@param looped czy dŸwiêk ma byæ zapêtlony
	*/
	Sound* loadSound(const char*, SOUND_TYPE type, bool looped);

	/// zwalnia zasoby dla danego dŸwiêku
	void releaseSound(Sound*);

	/*! 
	ustawia g³oœnoœæ dla wszystkich dŸwiêków oznaczonych jako SFX
	@param volume zadana g³oœnoœæ
	*/
	bool setSFXVolume(float volume);
	/*! 
	ustawia g³oœnoœæ dla wszystkich dŸwiêków oznaczonych jako muzyka
	@param volume zadana g³oœnoœæ
	*/
	bool setMusicVolume(float volume);

	/// zwraca g³oœnoœæ SFX
	inline float getSFXVolume() { return SFXVolume; }

	///zwraca g³oœnoœæ muzyki
	inline float getMusicVolume() { return MusicVolume; }

private:
	stdext::hash_map<const char*, Sound*> sounds;
	stdext::hash_map<const char*, int> soundsCount;

	float SFXVolume;
	float MusicVolume;

	FMOD::System *system;
};

