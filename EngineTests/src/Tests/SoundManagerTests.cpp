#include "SoundManagerTests.h"

/// engine
#include "Math/Vector2.h"
#include "GameEngine.h"
#include "includes.h"

// reszta
#include <cmath>
#include <assert.h>
#include <iostream>

#define NUMBER_OF_SOUND_TESTS 10000

void SoundManagerTests::test1()
{	
	static const char* filePathMusic = "TestData\\muzyka.mp3";
	static const char* filePathMusic2 = "TestData\\muzyka2.mp3";
	static const char* filePathSFX = "TestData\\sfx.mp3";

	Sound * music, * sfx, * music2;

	music = SoundManager::getSingleton().loadSound(filePathMusic, MUSIC, true);
	music2 = SoundManager::getSingleton().loadSound(filePathMusic2, MUSIC, true);
	sfx = SoundManager::getSingleton().loadSound(filePathSFX, SFX, false);

	SoundManager::getSingleton().setMusicVolume(0.2f);
	SoundManager::getSingleton().setSFXVolume(0.5f);

	music->play();	
	sfx->play();	
	music2->play();	

	Sleep(5000);
	SoundManager::getSingleton().setMusicVolume(1.0f);

	assert(music->isPlaying() == true);
	assert(music2->isPlaying() == true);

	Sleep(5000);
	music->pause();

	assert(sfx->isPlaying() == false);	
	assert(music2->isPlaying() == true);

	AX_PRINT("Sounds OK!\n");

	system("PAUSE");
	
	SoundManager::getSingleton().releaseSound(music);
	SoundManager::getSingleton().releaseSound(music2);
	SoundManager::getSingleton().releaseSound(sfx);
}

void SoundManagerTests::memoryLeakstTestStart()
{
	musicTab = new Sound*[NUMBER_OF_SOUND_TESTS];
	sfxTab = new Sound*[NUMBER_OF_SOUND_TESTS];

	for(int i=0; i<NUMBER_OF_SOUND_TESTS; ++i)
	{
		musicTab[i] = SoundManager::getSingleton().loadSound("TestData\\muzyka.mp3", MUSIC, true);
		sfxTab[i] = SoundManager::getSingleton().loadSound("TestData\\sfx.mp3", SFX, false);
	}
}

void SoundManagerTests::memoryLeakstTestEnd()
{
	for(int i=0; i<NUMBER_OF_SOUND_TESTS; ++i)
	{
		SoundManager::getSingleton().releaseSound(musicTab[i]);
		SoundManager::getSingleton().releaseSound(sfxTab[i]);	
	}

	delete[] musicTab;
	delete[] sfxTab;
}