#include "GameEngine.h"
#include "Resources/TextureManager.h"
#include "Resources/ConfigManager.h"
#include "Resources/FontManager.h"
#include "Resources/SoundManager.h"
#include "Tests/ConfigManagerTests.h"
#include "Tests/TextureManagerTests.h"
#include "Tests/FontManagerTests.h"
#include "Tests/SoundManagerTests.h"
#include "Tests/ParticleManagerTests.h"

#include <iostream>

//#include "../include/vld.h"

int main(int argc, char* args[])
{	
	/// inicjacja modułów gry
	ConfigManager::createSingleton();
	FontManager::createSingleton();
	GameEngine::createSingleton();	
	TextureManager::createSingleton();
	SoundManager::createSingleton();
	ParticleSystemManager::createSingleton();

	/// inicjacja testów
	ConfigManagerTests::createSingleton();	
	TextureManagerTests::createSingleton();	
	FontManagerTests::createSingleton();
	SoundManagerTests::createSingleton();
	ParticleManagerTests::createSingleton();

	ParticleManagerTests::getSingleton().test1();

	//ConfigManagerTests::getSingleton().test1();
	//ConfigManagerTests::getSingleton().vectorTest();
	//ConfigManagerTests::getSingleton().spriteTest();
	//ConfigManagerTests::getSingleton().colorTest();
	//ConfigManagerTests::getSingleton().progressCounterTest();
	//
	//TextureManagerTests::getSingleton().memoryLeakstTestStart();
	//TextureManagerTests::getSingleton().loadTexturesTest();		
	//TextureManagerTests::getSingleton().memoryLeakstTestEnd();	

	//FontManagerTests::getSingleton().memoryLeakstTestStart();	
	//FontManagerTests::getSingleton().textTest();
	//FontManagerTests::getSingleton().memoryLeakstTestEnd();

	//SoundManagerTests::getSingleton().memoryLeakstTestStart();
	//SoundManagerTests::getSingleton().test1();
	//SoundManagerTests::getSingleton().memoryLeakstTestEnd();

	/// sprzątanie	
	
	ParticleSystemManager::removeSingleton();
	ParticleManagerTests::removeSingleton();
	GameEngine::removeSingleton();	
	TextureManager::removeSingleton();
	ConfigManager::removeSingleton();
	TextureManagerTests::removeSingleton();
	FontManagerTests::removeSingleton();	
	ConfigManagerTests::removeSingleton();	
	FontManager::removeSingleton();
	SoundManager::removeSingleton();
	SoundManagerTests::removeSingleton();

	//system("PAUSE");
	return 0;   
}