#pragma once

#include <hash_map>
#include "../Singleton.h"
#include "Sound.h"
#include "SDL.h"
#include "SDL_opengl.h"

#define SOUND_CHANNELS 100

/*! 
 *  \brief     Menad�er d�wi�k�w
 *  \author    Micha� Mizera
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
	�aduje zadany d�wi�k z dysku
	@param path �cie�ka do d�wi�ku
	@param type rodzaj d�wi�ku (muzyka czy SFX)
	@param looped czy d�wi�k ma by� zap�tlony
	*/
	Sound* loadSound(const char*, SOUND_TYPE type, bool looped);

	/// zwalnia zasoby dla danego d�wi�ku
	void releaseSound(Sound*);

	/*! 
	ustawia g�o�no�� dla wszystkich d�wi�k�w oznaczonych jako SFX
	@param volume zadana g�o�no��
	*/
	bool setSFXVolume(float volume);
	/*! 
	ustawia g�o�no�� dla wszystkich d�wi�k�w oznaczonych jako muzyka
	@param volume zadana g�o�no��
	*/
	bool setMusicVolume(float volume);

	/// zwraca g�o�no�� SFX
	inline float getSFXVolume() { return SFXVolume; }

	///zwraca g�o�no�� muzyki
	inline float getMusicVolume() { return MusicVolume; }

private:
	stdext::hash_map<const char*, Sound*> sounds;
	stdext::hash_map<const char*, int> soundsCount;

	float SFXVolume;
	float MusicVolume;

	FMOD::System *system;
};

