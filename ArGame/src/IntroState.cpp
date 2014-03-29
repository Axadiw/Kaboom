
#include <stdio.h>

#include "SDL.h"
#include "SDL_opengl.h"
#include "GameEngine.h"
#include "GameState.h"
#include "IntroState.h"
#include "Resources\ConfigManager.h"
#include "MenuState.h"

#define SUBSTATES_COUNT 2

static const char* introConfFile = "Data\\ARintro.txt";

void IntroState::Init()
{
	char bufferName[255];
	substateNumber = 0;
	actualSubstateType = FADING;
	fade = true;

	// tekstury
	white = TextureManager::getSingleton().loadTexture("Data\\white.bmp");	
	logosTex = TextureManager::getSingleton().loadTexture("Data\\logos.png");
	fontVerdana = FontManager::getSingleton().loadFont("Data\\verdana.ttf");
		
	ConfigManager::getSingleton().loadFile(introConfFile);
	
	// sprite-y
	fadeCounter.load(introConfFile, "fadeCounter");
	showImagesTimeCounter.load(introConfFile, "showImagesTimeCounter");

	fullScreenPanel.load(introConfFile, "fullScreenPanel");		
	polygonLogo.load(introConfFile, "polygonLogo");
	fmodLogo.load(introConfFile, "FMODLogo");
	sdlLogo.load(introConfFile, "SDLLogo");

	ConfigManager::getSingleton().getValueInt(&textsCount, introConfFile, "license_texts_count");
	texts = new Text[textsCount];
	
	for(int i=0; i< textsCount; ++i)
	{
		sprintf(bufferName, "license_text_%d", i);
		texts[i].load(introConfFile, bufferName);
	}

	ConfigManager::getSingleton().releaseConfigFile(introConfFile);
}

void IntroState::Cleanup()
{
	TextureManager::getSingleton().releaseTexture(white);		
	TextureManager::getSingleton().releaseTexture(logosTex);	

	FontManager::getSingleton().releaseFont(fontVerdana);

	delete[] texts;
}

void IntroState::Pause()
{
	
}

void IntroState::Resume()
{
	
}

void IntroState::HandleEvents(GameEngine* game)
{
	SDL_Event event;

	if (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) {
					case SDLK_ESCAPE:
						game->Quit();
						break;
				}
				break;
		}
	}
}

void IntroState::Update(GameEngine* game) 
{	
	if(actualSubstateType == SHOWING_LOGOS)
	{
		showImagesTimeCounter.updateLinear(game->dt);

		if(showImagesTimeCounter.finished())
		{
			actualSubstateType = FADING;
			showImagesTimeCounter.progress = showImagesTimeCounter.minValue;
		}
	}

	if(actualSubstateType == FADING)
	{
		fadeCounter.updateLinear(game->dt);

		if(fadeCounter.finished())
		{
			if(fadeCounter.direction == DOWN)
			{
				actualSubstateType = SHOWING_LOGOS;
			}
			else
			{
				substateNumber++;
			}

			fadeCounter.direction = fadeCounter.direction==UP?DOWN:UP;
		}
	}

	fullScreenPanel.setColor(0,0,0, static_cast<u8>((fadeCounter.progress/fadeCounter.maxValue)*255.0f));

	if(substateNumber >= SUBSTATES_COUNT)
	{
		game->ChangeState( MenuState::getSingletonPtr() );
	}
}

void IntroState::Draw(GameEngine* game) 
{
	logosTex->bind();

	switch(substateNumber)
	{
	case 0:
		// pierwsza plansza
		polygonLogo.draw();
		break;
	case 1:
		// druga plansza
		sdlLogo.draw();
		fmodLogo.draw();

		white->bind();
		for(int i=0; i<textsCount; ++i)
		{
			texts[i].draw(fontVerdana);
		}

		break;
	}
	
	white->bind();
	fullScreenPanel.draw();
}
