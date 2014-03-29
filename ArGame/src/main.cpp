#include "PolyMTLib.h"
#include "GameEngine.h"
#include "Resources/TextureManager.h"
#include "Resources/ConfigManager.h"
#include "Resources/FontManager.h"
#include "Resources/SoundManager.h"
#include "GameSetup.h"
#include "GameplayState.h"
#include "MenuState.h"
#include "SDL.h"
#include "IntroState.h"
#include "SDL_opengl.h"
#include "includes.h"

#include "windows.h"

#pragma comment (lib, "SDL.lib")
#pragma comment (lib, "SDLmain.lib")
#pragma comment (lib, "opengl32.lib")
#pragma comment(lib,"winmm.lib") 

#define MAXIMUM_DT 0.2f

#include <iostream>

LARGE_INTEGER uFreq;
bool bUseQPC = (QueryPerformanceFrequency (&uFreq) != 0);

float GetSecs()
{
   if (!bUseQPC)   return GetTickCount() / 1000.0f;
   else
   {
      LARGE_INTEGER uTicks;
      QueryPerformanceCounter (&uTicks);
      return (float)(uTicks.QuadPart / (double)uFreq.QuadPart);
   }
}

int main(int argc, char* args[])
{		
	GameEngine::createSingleton();
	MenuState::createSingleton();
	GameplayState::createSingleton();
	TextureManager::createSingleton();
	ConfigManager::createSingleton();
	IntroState::createSingleton();
	FontManager::createSingleton();
	SoundManager::createSingleton();
	GameSetup::createSingleton();

	GameSetup::getSingleton().init();

	GameEngine* engine = GameEngine::getSingletonPtr();
	GameSetup* gameSetup = GameSetup::getSingletonPtr();
	engine->Init(WINDOW_NAME);

	engine->arEngine = new PolyMTLib();
	engine->arEngine->Init(gameSetup->cameraID, static_cast<int>(gameSetup->cameraResolution.x), static_cast<int>(gameSetup->cameraResolution.y), static_cast<int>(gameSetup->cameraFramerate));
	engine->arEngine->Start();

	#ifdef DEBUG
	engine->ChangeState( MenuState::getSingletonPtr() );
#else
	// intro
	engine->ChangeState( IntroState::getSingletonPtr() );
#endif

	float fTime = 0.0f, fNewTime = 0.0f, fFPSTime = 0.0f;
	unsigned uFrames = 0;
	engine->dt = 1.0f/GameSetup::getSingleton().screenFramerate;

	Uint32 before;
	int delay;

	while ( engine->Running() )
	{
		before = SDL_GetTicks();

		engine->HandleEvents();		
		engine->Update();		
		engine->Draw();
		
		fNewTime = GetSecs();
		engine->dt = ((fNewTime - fTime) < MAXIMUM_DT)?fNewTime - fTime:MAXIMUM_DT;
		fTime = fNewTime;

		// LICZENIE FPS
		++uFrames;
		fFPSTime += engine->dt;
		if (fFPSTime>= 1.0f)
		{
			engine->FPS = uFrames / fFPSTime;
			uFrames = 0;
			fFPSTime = 0.0f;
		}

		delay = before + 1000/GameSetup::getSingleton().screenFramerate - SDL_GetTicks();
		if(delay > 0) SDL_Delay(delay);
	}

	engine->Cleanup();

	delete engine->arEngine;
	IntroState::removeSingleton();
	GameEngine::removeSingleton();
	GameplayState::removeSingleton();
	MenuState::removeSingleton();	
	TextureManager::removeSingleton();
	ConfigManager::removeSingleton();
	FontManager::removeSingleton();
	SoundManager::removeSingleton();
	GameSetup::removeSingleton();

	return 0;   
}