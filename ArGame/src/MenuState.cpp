
#include <stdio.h>
#include <cmath>

#include "SDL.h"
#include "SDL_opengl.h"
#include "GameEngine.h"
#include "GameplayState.h"
#include "MenuState.h"
#include "GameSetup.h"
#include "Resources\ConfigManager.h"
#include "includes.h"
#include "GameState.h"

enum MAIN_MENU_ITEMS{
	START_BUTTON,
	PLAY_BUTTON,
	OPTIONS_BUTTON,
	BACK_LEVELSELECT_BUTTON,
	BACK_OPTIONS_BUTTON, 
	BACK_EDITOR_BUTTON,
	OPTIONS_CAMERA_RESOLUTION_LOW_BUTTON, 
	OPTIONS_CAMERA_RESOLUTION_MEDIUM_BUTTON, 
	OPTIONS_CAMERA_RESOLUTION_HIGH_BUTTON, 
	OPTIONS_SCREEN_RESOLUTION_LOW_BUTTON, 
	OPTIONS_SCREEN_RESOLUTION_MEDIUM_BUTTON, 
	OPTIONS_SCREEN_RESOLUTION_HIGH_BUTTON, 
	OPTIONS_FULLSCREEN_YES_BUTTON, 
	OPTIONS_FULLSCREEN_NO_BUTTON
};

static const char* menuConfFile = "Data\\arMainMenu.txt";

float RandomFloat(float min, float max)
{
	float r = (float)(rand()%100) / 100.0f;
	return min + r * (max - min);
}

void MenuState::Init()
{
	white = TextureManager::getSingleton().loadTexture("Data\\white.bmp");
	mainMenuTex = TextureManager::getSingleton().loadTexture("Data\\arMainMenuTex.png");

	fontTygodnik = FontManager::getSingleton().loadFont("Data\\Tygodnik.ttf");

	ConfigManager::getSingleton().loadFile(menuConfFile);

	lastClickedItem = -1;

	cameraImageTex = new Texture;
	cameraImageTex->initOpenCVTexture();

	// t³o
	background.load(menuConfFile, "fullScreenPanel");
	gameLogo.load(menuConfFile, "gameLogo");
	startButton.load(menuConfFile, "startButton");
	titleBar.load(menuConfFile, "titleBar");
	blob0.load(menuConfFile, "blob0");
	blob1.load(menuConfFile, "blob1");
	blob2.load(menuConfFile, "blob2");
	blob0Counter.load(menuConfFile, "blob0_counter");
	blob1Counter.load(menuConfFile, "blob1_counter");
	blob2Counter.load(menuConfFile, "blob2_counter");

	// kursor
	handSprite.load(menuConfFile, "handSprite");
	handRim.load(menuConfFile, "handRim");
	mouseHitCounter.load(menuConfFile, "mouseHitCounter");
	
	// menu g³ówne
	nextText.load(menuConfFile, "nextText");
	playButton.load(menuConfFile, "playButton");
	optionsButton.load(menuConfFile, "optionsButton");
	backLevelSelectButton.load(menuConfFile, "backLevelSelectButton");
	backOptionsButton.load(menuConfFile, "backOptionsButton");
	backEditorButton.load(menuConfFile, "backEditorButton");
	playText.load(menuConfFile, "playText");
	optionsText.load(menuConfFile, "optionsText");
	backLevelSelectText.load(menuConfFile, "backLevelSelectText");
	backOptionsText.load(menuConfFile, "backOptionsText");
	backEditorText.load(menuConfFile, "backEditorText");
	mainMenuTitleText.load(menuConfFile, "mainMenuTitleText");
	optionsTitleText.load(menuConfFile, "optionsTitleText");
	editorTitleText.load(menuConfFile, "editorTitleText");
	levelSelectTitleText.load(menuConfFile, "levelSelectTitleText");

	//opcje
	optionsCameraResolutionText.load(menuConfFile,"optionsCameraResolutionText");
	optionsCameraResolutionLowText.load(menuConfFile,"optionsCameraResolutionLowText");
	optionsCameraResolutionMediumText.load(menuConfFile,"optionsCameraResolutionMediumText");
	optionsCameraResolutionHighText.load(menuConfFile,"optionsCameraResolutionHighText");
	optionsCameraImageText.load(menuConfFile,"optionsCameraImageText");
	optionsScreenResolutionText.load(menuConfFile,"optionsScreenResolutionText");
	optionsScreenResolutionLowText.load(menuConfFile,"optionsScreenResolutionLowText");
	optionsScreenResolutionMediumText.load(menuConfFile,"optionsScreenResolutionMediumText");
	optionsScreenResolutionHighText.load(menuConfFile,"optionsScreenResolutionHighText");
	optionsFullscreenText.load(menuConfFile,"optionsFullscreenText");
	optionsFullscreenYesText.load(menuConfFile,"optionsFullscreenYesText");
	optionsFullscreenNoText.load(menuConfFile,"optionsFullscreenNoText");
	optionsCameraResolutionLowButton.load(menuConfFile,"optionsCameraResolutionLowButton");
	optionsCameraResolutionMediumButton.load(menuConfFile,"optionsCameraResolutionMediumButton");
	optionsCameraResolutionHighButton.load(menuConfFile,"optionsCameraResolutionHighButton");
	optionsScreenResolutionLowButton.load(menuConfFile,"optionsScreenResolutionLowButton");
	optionsScreenResolutionMediumButton.load(menuConfFile,"optionsScreenResolutionMediumButton");
	optionsScreenResolutionHighButton.load(menuConfFile,"optionsScreenResolutionHighButton");
	optionsFullscreenYesButton.load(menuConfFile,"optionsFullscreenYesButton");
	optionsFullscreenNoButton.load(menuConfFile,"optionsFullscreenNoButton");
	cameraImage.load(menuConfFile, "cameraImage");

	blob0.setPos(RandomFloat(-SCREEN_WIDTH,SCREEN_WIDTH), SCREEN_HEIGHT + blob0.getSize().y);
	blob1.setPos(-SCREEN_WIDTH - blob1.getSize().x, RandomFloat(-SCREEN_HEIGHT,SCREEN_HEIGHT));
	blob2.setPos(-SCREEN_WIDTH - blob2.getSize().x, RandomFloat(-SCREEN_HEIGHT,SCREEN_HEIGHT));

	buttonEnabledColor.load(menuConfFile, "buttonEnabledColor");
	buttonSelectedColor.load(menuConfFile, "buttonSelectedColor");

	ConfigManager::getSingleton().releaseConfigFile(menuConfFile);

	actualSubstate = INITIAL_SCREEN;

	//KLIKANE ELEMENTY MENU
	menuItems.push_back(std::make_pair(&startButton, INITIAL_SCREEN));
	menuItems.push_back(std::make_pair(&playButton, MAIN_MENU));
	menuItems.push_back(std::make_pair(&optionsButton, MAIN_MENU));
	menuItems.push_back(std::make_pair(&backLevelSelectButton, LEVEL_SELECT));
	menuItems.push_back(std::make_pair(&backOptionsButton, OPTIONS));
	menuItems.push_back(std::make_pair(&backEditorButton, EDITOR));
	menuItems.push_back(std::make_pair(&optionsCameraResolutionLowButton, OPTIONS));
	menuItems.push_back(std::make_pair(&optionsCameraResolutionMediumButton, OPTIONS));
	menuItems.push_back(std::make_pair(&optionsCameraResolutionHighButton, OPTIONS));
	menuItems.push_back(std::make_pair(&optionsScreenResolutionLowButton, OPTIONS));
	menuItems.push_back(std::make_pair(&optionsScreenResolutionMediumButton, OPTIONS));
	menuItems.push_back(std::make_pair(&optionsScreenResolutionHighButton, OPTIONS));
	menuItems.push_back(std::make_pair(&optionsFullscreenYesButton, OPTIONS));
	menuItems.push_back(std::make_pair(&optionsFullscreenNoButton, OPTIONS));
}

void MenuState::Cleanup()
{
	cameraImageTex->releaseOpenCVTexture();
	delete cameraImageTex;

	TextureManager::getSingleton().releaseTexture(white);	
	TextureManager::getSingleton().releaseTexture(mainMenuTex);		

	FontManager::getSingleton().releaseFont(fontTygodnik);
}

void MenuState::Pause()
{

}

void MenuState::Resume()
{

}

void MenuState::HandleEvents(GameEngine* game)
{
	SDL_Event event;
	GameSetup* gameSetup = GameSetup::getSingletonPtr();

	if (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
			case SDLK_ESCAPE:
				game->Quit();
				break;
			}
			break;
		case SDL_MOUSEMOTION :

			// przeliczenie wspó³rzêdnych kursora myszy na wspó³rzêdne ekranu
			mouseCoords = Vector2(event.motion.x, event.motion.y);
			mouseCoords.x = (mouseCoords.x/game->screenDimensions.x)*2.0f*SCREEN_WIDTH - SCREEN_WIDTH;
			mouseCoords.y = -(mouseCoords.y/game->screenDimensions.y)*2.0f*SCREEN_HEIGHT + SCREEN_HEIGHT;

			break;
		}
	}

	// pobieranie markerów
	game->arEngine->GetMarkers(markers);
	float cameraAspectRatio = gameSetup->cameraResolution.x/gameSetup->cameraResolution.y;

	for(unsigned int i=0; i<markers.size(); ++i)
	{
		markers[i].center.x = static_cast<int>((markers[i].center.x/gameSetup->cameraResolution.x)*2.0f*SCREEN_WIDTH - SCREEN_WIDTH);
		markers[i].center.y = static_cast<int>(-(markers[i].center.y/gameSetup->cameraResolution.y)*2.0f*SCREEN_WIDTH/cameraAspectRatio + SCREEN_WIDTH/cameraAspectRatio);
		
		if(markers[i].id==gameSetup->cursorMarkerID)
		{
			mouseCoords.x = static_cast<float>(markers[i].center.x);
			mouseCoords.y = static_cast<float>(markers[i].center.y);
			handSprite.setAngle(markers[i].rotation*180.f/M_PI);
		}
	}
}

void MenuState::mouseItemClicked(int id)
{
	GameSetup* gameSetup = GameSetup::getSingletonPtr();

	switch(id)
	{
	case START_BUTTON:
		// przycisk "START"
		actualSubstate = MAIN_MENU;
		break;
	case PLAY_BUTTON:
		// przycisk "GRAJ"
		GameEngine::getSingleton().ChangeState( GameplayState::getSingletonPtr() );
		break;
	case OPTIONS_BUTTON:
		// przycisk "OPCJE"
		actualSubstate = OPTIONS;
		break;
	case BACK_LEVELSELECT_BUTTON:
		// przycisk "WROC" z wyboru levelu
		actualSubstate = MAIN_MENU;
		break;
	case BACK_OPTIONS_BUTTON:
		// przycisk "WRÓC" z opcji
		actualSubstate = MAIN_MENU;
		break;
	case BACK_EDITOR_BUTTON:
		// przycisk "WRÓC" z edytora
		actualSubstate = MAIN_MENU;
		break;
	case OPTIONS_CAMERA_RESOLUTION_LOW_BUTTON:
		// przycisk "niska" jakosc obrazu z kamery
		if(gameSetup->cameraQuality != LOW)
		{
			gameSetup->cameraQuality = LOW;
			gameSetup->cameraResolution = gameSetup->lowCameraResolution;
			gameSetup->cameraFramerate = gameSetup->lowCameraFramerate;
			gameSetup->save();
			GameEngine::getSingleton().arEngine->Reinit(gameSetup->cameraID, static_cast<int>(gameSetup->cameraResolution.x), static_cast<int>(gameSetup->cameraResolution.y), static_cast<int>(gameSetup->cameraFramerate));
		}
		break;
	case OPTIONS_CAMERA_RESOLUTION_MEDIUM_BUTTON:
		// przycisk "œrednia" jakosc obrazu z kamery
		if(gameSetup->cameraQuality != MEDIUM)
		{
			gameSetup->cameraQuality = MEDIUM;
			gameSetup->cameraResolution = gameSetup->mediumCameraResolution;
			gameSetup->cameraFramerate = gameSetup->mediumCameraFramerate;
			gameSetup->save();
			GameEngine::getSingleton().arEngine->Reinit(gameSetup->cameraID, static_cast<int>(gameSetup->cameraResolution.x), static_cast<int>(gameSetup->cameraResolution.y), static_cast<int>(gameSetup->cameraFramerate));
		}
		break;
	case OPTIONS_CAMERA_RESOLUTION_HIGH_BUTTON:
		// przycisk "wysoka" jakosc obrazu z kamery
		if(gameSetup->cameraQuality != HIGH)
		{
			gameSetup->cameraQuality = HIGH;
			gameSetup->cameraResolution = gameSetup->highCameraResolution;
			gameSetup->cameraFramerate = gameSetup->highCameraFramerate;
			gameSetup->save();
			GameEngine::getSingleton().arEngine->Reinit(gameSetup->cameraID, static_cast<int>(gameSetup->cameraResolution.x), static_cast<int>(gameSetup->cameraResolution.y), static_cast<int>(gameSetup->cameraFramerate));
		}
		break;
	case OPTIONS_SCREEN_RESOLUTION_LOW_BUTTON:
		// przycisk "niska" rozdzielczoœæ ekranu
		if(gameSetup->screenQuality != LOW)
		{
			gameSetup->screenQuality = LOW;
			GameEngine::getSingleton().screenDimensions = gameSetup->lowScreenResolution;
			GameEngine::getSingleton().Cleanup();
			GameEngine::getSingleton().Init(WINDOW_NAME);
			GameEngine::getSingleton().ChangeState(MenuState::getSingletonPtr());
			actualSubstate = OPTIONS;
			gameSetup->save();
		}
		break;
	case OPTIONS_SCREEN_RESOLUTION_MEDIUM_BUTTON:
		// przycisk "œrednia" rozdzielczoœæ ekranu
		if(gameSetup->screenQuality != MEDIUM)
		{
			gameSetup->screenQuality = MEDIUM;
			GameEngine::getSingleton().screenDimensions = gameSetup->mediumScreenResolution;
			GameEngine::getSingleton().Cleanup();
			GameEngine::getSingleton().Init(WINDOW_NAME);
			GameEngine::getSingleton().ChangeState(MenuState::getSingletonPtr());
			actualSubstate = OPTIONS;
			gameSetup->save();
		}
		break;
	case OPTIONS_SCREEN_RESOLUTION_HIGH_BUTTON:
		// przycisk "wysoka" rozdzielczoœæ ekranu
		if(gameSetup->screenQuality != HIGH)
		{
			gameSetup->screenQuality = HIGH;
			GameEngine::getSingleton().screenDimensions = gameSetup->highScreenResolution;
			GameEngine::getSingleton().Cleanup();
			GameEngine::getSingleton().Init(WINDOW_NAME);
			GameEngine::getSingleton().ChangeState(MenuState::getSingletonPtr());
			actualSubstate = OPTIONS;
			gameSetup->save();
		}
		break;
	case OPTIONS_FULLSCREEN_YES_BUTTON:
		// przycisk "TAK" dla fullscreena
		if(!(GameEngine::getSingleton().fullscreen))
		{
			GameEngine::getSingleton().fullscreen = true;
			GameEngine::getSingleton().Cleanup();
			GameEngine::getSingleton().Init(WINDOW_NAME);
			GameEngine::getSingleton().ChangeState(MenuState::getSingletonPtr());
			actualSubstate = OPTIONS;
			gameSetup->save();
		}
		break;
	case OPTIONS_FULLSCREEN_NO_BUTTON:
		// przycisk "NIE" dla fullscreena
		if(GameEngine::getSingleton().fullscreen)
		{
			GameEngine::getSingleton().fullscreen = false;
			GameEngine::getSingleton().Cleanup();
			GameEngine::getSingleton().Init(WINDOW_NAME);
			GameEngine::getSingleton().ChangeState(MenuState::getSingletonPtr());
			actualSubstate = OPTIONS;
			gameSetup->save();
		}
		break;
	}
}

void MenuState::Update(GameEngine* game) 
{
	// animowane t³o
	blob0Counter.updateLinear(game->dt);
	blob1Counter.updateLinear(game->dt);
	blob2Counter.updateLinear(game->dt);

	blob0.setPos(blob0.getPos().x,SCREEN_HEIGHT + blob0.getSize().y - blob0Counter.progress*(SCREEN_HEIGHT*2 + 2.0f*blob2.getSize().y));
	blob1.setPos(-SCREEN_WIDTH - blob1.getSize().x + blob1Counter.progress*(SCREEN_WIDTH*2 + 2.0f*blob2.getSize().x), blob1.getPos().y);
	blob2.setPos(-SCREEN_WIDTH - blob2.getSize().x + blob2Counter.progress*(SCREEN_WIDTH*2 + 2.0f*blob2.getSize().x), blob2.getPos().y);

	if(blob0Counter.finished()) 
	{
		blob0Counter.progress = blob0Counter.minValue; 
		blob0.setPos(RandomFloat(-SCREEN_WIDTH,SCREEN_WIDTH), SCREEN_HEIGHT + blob0.getSize().y);
	}

	if(blob1Counter.finished()) 
	{
		blob1Counter.progress = blob1Counter.minValue; 
		blob1.setPos(-SCREEN_WIDTH - blob1.getSize().x, RandomFloat(-SCREEN_HEIGHT,SCREEN_HEIGHT));
	}
	if(blob2Counter.finished()) 
	{
		blob2Counter.progress = blob2Counter.minValue; 
		blob2.setPos(-SCREEN_WIDTH - blob2.getSize().x, RandomFloat(-SCREEN_HEIGHT,SCREEN_HEIGHT));
	}

	// kolizje z elementami menu
	bool collisionDetedted = false;
	for(unsigned int i=0; i<menuItems.size(); ++i)
	{
		//if( mouseCoords.x >= (menuItems[i]->getPos().x - menuItems[i]->getSize().x/2.0f) && mouseCoords.x <= (menuItems[i]->getPos().x + menuItems[i]->getSize().x/2.0f) &&
		//	mouseCoords.y >= (menuItems[i]->getPos().y - menuItems[i]->getSize().y/2.0f) && mouseCoords.y <= (menuItems[i]->getPos().y + menuItems[i]->getSize().y/2.0f))
		if( fabs(mouseCoords.x - menuItems[i].first->getPos().x) <= (menuItems[i].first->getScale()*menuItems[i].first->getSize().x/2.0f + handSprite.getScale()*handSprite.getSize().x/2.0f) &&
			fabs(mouseCoords.y - menuItems[i].first->getPos().y) <= (menuItems[i].first->getScale()*menuItems[i].first->getSize().y/2.0f + handSprite.getScale()*handSprite.getSize().y/2.0f)
			&& actualSubstate == menuItems[i].second)
		{
			// kolizja z innym elementem menu niz ostatnio
			if(i != selectedMenuItem)
			{
				mouseHitCounter.progress = mouseHitCounter.minValue;
			}

			collisionDetedted = true;

			// zabiezpieczenie przed ci¹g³ym klikaniem jednej opcji w menu
			if(lastClickedItem == i)
				break;

			if(mouseHitCounter.progress == mouseHitCounter.maxValue)
			{
				mouseHitCounter.progress = mouseHitCounter.minValue;
				mouseItemClicked(i);
				lastClickedItem = i;
				break;
			}

			mouseHitCounter.updateLinear(game->dt);
			selectedMenuItem = i;			

			break;
		}
	}

	// brak kolizji
	if(!collisionDetedted)
	{
		lastClickedItem = -1;
		selectedMenuItem = -1;
		mouseHitCounter.progress = mouseHitCounter.minValue;
	}	
}

void MenuState::Draw(GameEngine* game) 
{
	// t³o
	white->bind();
	background.draw();

	mainMenuTex->bind();	

	// plamy na tle
	blob0.draw();
	blob1.draw();
	blob2.draw();

	switch(actualSubstate)
	{
	case INITIAL_SCREEN:
		// logo
		gameLogo.draw();
		startButton.draw(selectedMenuItem==START_BUTTON?buttonSelectedColor:Color(255,255,255,255));

		white->bind();	
		nextText.draw(fontTygodnik);
		break;
	case MAIN_MENU:
		titleBar.draw();

		playButton.draw(selectedMenuItem==PLAY_BUTTON?buttonSelectedColor:Color(255,255,255,255));
		optionsButton.draw(selectedMenuItem==OPTIONS_BUTTON?buttonSelectedColor:Color(255,255,255,255));

		white->bind();	
		mainMenuTitleText.draw(fontTygodnik);
		playText.draw(fontTygodnik);
		optionsText.draw(fontTygodnik);

		break;
	case LEVEL_SELECT:
		titleBar.draw();		

		backLevelSelectButton.draw(selectedMenuItem==BACK_LEVELSELECT_BUTTON?buttonSelectedColor:Color(255,255,255,255));

		white->bind();	
		levelSelectTitleText.draw(fontTygodnik);
		backLevelSelectText.draw(fontTygodnik);

		break;
	case EDITOR:
		titleBar.draw();
		backEditorButton.draw(selectedMenuItem==BACK_EDITOR_BUTTON?buttonSelectedColor:Color(255,255,255,255));


		white->bind();	
		editorTitleText.draw(fontTygodnik);
		backEditorText.draw(fontTygodnik);

		break;
	case OPTIONS:
		titleBar.draw();
		backOptionsButton.draw(selectedMenuItem==BACK_OPTIONS_BUTTON?buttonSelectedColor:Color(255,255,255,255));
		optionsCameraResolutionLowButton.draw(selectedMenuItem==OPTIONS_CAMERA_RESOLUTION_LOW_BUTTON?buttonSelectedColor:GameSetup::getSingleton().cameraQuality==LOW?buttonEnabledColor:Color(255,255,255,255));
		optionsCameraResolutionMediumButton.draw(selectedMenuItem==OPTIONS_CAMERA_RESOLUTION_MEDIUM_BUTTON?buttonSelectedColor:GameSetup::getSingleton().cameraQuality==MEDIUM?buttonEnabledColor:Color(255,255,255,255));
		optionsCameraResolutionHighButton.draw(selectedMenuItem==OPTIONS_CAMERA_RESOLUTION_HIGH_BUTTON?buttonSelectedColor:GameSetup::getSingleton().cameraQuality==HIGH?buttonEnabledColor:Color(255,255,255,255));
		optionsScreenResolutionLowButton.draw(selectedMenuItem==OPTIONS_SCREEN_RESOLUTION_LOW_BUTTON?buttonSelectedColor:GameSetup::getSingleton().screenQuality==LOW?buttonEnabledColor:Color(255,255,255,255));
		optionsScreenResolutionMediumButton.draw(selectedMenuItem==OPTIONS_SCREEN_RESOLUTION_MEDIUM_BUTTON?buttonSelectedColor:GameSetup::getSingleton().screenQuality==MEDIUM?buttonEnabledColor:Color(255,255,255,255));
		optionsScreenResolutionHighButton.draw(selectedMenuItem==OPTIONS_SCREEN_RESOLUTION_HIGH_BUTTON?buttonSelectedColor:GameSetup::getSingleton().screenQuality==HIGH?buttonEnabledColor:Color(255,255,255,255));
		optionsFullscreenYesButton.draw(selectedMenuItem==OPTIONS_FULLSCREEN_YES_BUTTON?buttonSelectedColor:GameEngine::getSingleton().fullscreen?buttonEnabledColor:Color(255,255,255,255));
		optionsFullscreenNoButton.draw(selectedMenuItem==OPTIONS_FULLSCREEN_NO_BUTTON?buttonSelectedColor:!(GameEngine::getSingleton().fullscreen)?buttonEnabledColor:Color(255,255,255,255));

		
		cameraImageTex->updateFromOpenCvmat(*(GameEngine::getSingleton().arEngine->image_));
		cameraImage.draw();

		white->bind();	
		optionsTitleText.draw(fontTygodnik);
		backOptionsText.draw(fontTygodnik);
		optionsCameraResolutionText.draw(fontTygodnik);
		optionsCameraResolutionLowText.draw(fontTygodnik);
		optionsCameraResolutionMediumText.draw(fontTygodnik);
		optionsCameraResolutionHighText.draw(fontTygodnik);
		optionsCameraImageText.draw(fontTygodnik);
		optionsScreenResolutionText.draw(fontTygodnik);
		optionsScreenResolutionLowText.draw(fontTygodnik);
		optionsScreenResolutionMediumText.draw(fontTygodnik);
		optionsScreenResolutionHighText.draw(fontTygodnik);
		optionsFullscreenText.draw(fontTygodnik);
		optionsFullscreenYesText.draw(fontTygodnik);
		optionsFullscreenNoText.draw(fontTygodnik);

		break;
	}

	// kursor
	mainMenuTex->bind();
	handSprite.draw(mouseCoords);

	// rysowanie obwiedni przedstawiaj¹cej progress klikania obiektów w menu
	int slices = static_cast<int>(mouseHitCounter.progress/mouseHitCounter.maxValue/ (5.0f/360.0f));
	for(int i=0; i<slices; ++i)
	{
		handRim.draw(mouseCoords, i*5.0f);
	}
}