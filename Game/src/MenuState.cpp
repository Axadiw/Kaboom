
#include <stdio.h>
#include <cmath>

#include "dirent.h"
#include "SDL.h"
#include "SDL_opengl.h"
#include "GameEngine.h"
#include "GameState.h"
#include "MenuState.h"
#include "GameSetup.h"
#include "GameplayState.h"
#include "EditorState.h"
#include "Resources\ConfigManager.h"
#include "includes.h"

#define LEVELS_PER_PAGE 4

enum MAIN_MENU_ITEMS{
	START_BUTTON,
	PLAY_BUTTON,
	EDITOR_BUTTON,
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
	OPTIONS_FULLSCREEN_NO_BUTTON,
	LEVELSELECT_PREV_ARROW,
	LEVELSELECT_NEXT_ARROW,
	LEVELSELECT_LEVEL0,
	LEVELSELECT_LEVEL1,
	LEVELSELECT_LEVEL2,
	LEVELSELECT_LEVEL3
};

static const char* menuConfFile = "Data\\mainMenu.txt";

float RandomFloat(float min, float max)
{
	float r = (float)(rand()%100) / 100.0f;
	return min + r * (max - min);
}

void MenuState::Init()
{
	int tmpX,tmpY;
	SDL_GetMouseState(&tmpX, &tmpY);
	mouseCoords= Vector2(static_cast<float>(tmpX), static_cast<float>(tmpY));
	mouseCoords.x = (mouseCoords.x/GameEngine::getSingleton().screenDimensions.x)*2.0f*SCREEN_WIDTH - SCREEN_WIDTH;
	mouseCoords.y = -(mouseCoords.y/GameEngine::getSingleton().screenDimensions.y)*2.0f*SCREEN_HEIGHT + SCREEN_HEIGHT;

	white = TextureManager::getSingleton().loadTexture("Data\\white.bmp");
	mainMenuTex = TextureManager::getSingleton().loadTexture("Data\\mainMenuTex.png");
	particlesTex = TextureManager::getSingleton().loadTexture("Data\\particlesTex.png");
	
	music = SoundManager::getSingleton().loadSound("Data\\sounds\\MenuMusic.mp3", MUSIC, true);
	click = SoundManager::getSingleton().loadSound("Data\\sounds\\click.ogg", SFX, false);
	music->play();

	fontTygodnik = FontManager::getSingleton().loadFont("Data\\Tygodnik.ttf");

	ConfigManager::getSingleton().loadFile(menuConfFile);

	lastClickedItem = -1;

	cameraImageTex = new Texture;
	cameraImageTex->initOpenCVTexture();

	
	ConfigManager::getSingleton().getValueFloat(&cursorPosMultiplier, menuConfFile, "cursorPosMultiplier");

	background.load(menuConfFile, "fullScreenPanel");
	gameLogo.load(menuConfFile, "gameLogo");
	startButton.load(menuConfFile, "startButton");
	titleBar.load(menuConfFile, "titleBar");
	blob0.load(menuConfFile, "blob0");
	blob1.load(menuConfFile, "blob1");
	blob2.load(menuConfFile, "blob2");

	cursorParticle = ParticleSystemManager::getSingleton().loadSystem("Data\\particles\\mouseParticle.txt");

	blob0Counter.load(menuConfFile, "blob0_counter");
	blob1Counter.load(menuConfFile, "blob1_counter");
	blob2Counter.load(menuConfFile, "blob2_counter");

	handSprite.load(menuConfFile, "handSprite");
	handRim.load(menuConfFile, "handRim");
	nextText.load(menuConfFile, "nextText");

	playButton.load(menuConfFile, "playButton");
	editorButton.load(menuConfFile, "editorButton");
	optionsButton.load(menuConfFile, "optionsButton");
	backLevelSelectButton.load(menuConfFile, "backLevelSelectButton");
	backOptionsButton.load(menuConfFile, "backOptionsButton");
	backEditorButton.load(menuConfFile, "backEditorButton");
	playText.load(menuConfFile, "playText");
	editorText.load(menuConfFile, "editorText");
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

	mouseHitCounter.load(menuConfFile, "mouseHitCounter");

	blob0.setPos(RandomFloat(-SCREEN_WIDTH,SCREEN_WIDTH), SCREEN_HEIGHT + blob0.getSize().y);
	blob1.setPos(-SCREEN_WIDTH - blob1.getSize().x, RandomFloat(-SCREEN_HEIGHT,SCREEN_HEIGHT));
	blob2.setPos(-SCREEN_WIDTH - blob2.getSize().x, RandomFloat(-SCREEN_HEIGHT,SCREEN_HEIGHT));

	buttonEnabledColor.load(menuConfFile, "buttonEnabledColor");
	buttonSelectedColor.load(menuConfFile, "buttonSelectedColor");

	levelSelectPageIndex = 0;

	prevArrow.load(menuConfFile, "prev_arrow");
	nextArrow.load(menuConfFile, "next_arrow");

	levelSelectButton = new Sprite[LEVELS_PER_PAGE];
	levelSelectButtonText = new Text[LEVELS_PER_PAGE];
	char bufName[255];
	for(int i=0; i<LEVELS_PER_PAGE; i++)
	{
		sprintf(bufName, "levelSelect_level%d", i);
		levelSelectButton[i].load(menuConfFile, bufName);

		sprintf(bufName, "levelSelect_level_text%d", i);
		levelSelectButtonText[i].load(menuConfFile, bufName);
	}

	ConfigManager::getSingleton().releaseConfigFile(menuConfFile);	

	//KLIKANE ELEMENTY MENU
	menuItems.push_back(std::make_pair(&startButton, INITIAL_SCREEN));
	menuItems.push_back(std::make_pair(&playButton, MAIN_MENU));
	menuItems.push_back(std::make_pair(&editorButton, MAIN_MENU));
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
	menuItems.push_back(std::make_pair(&prevArrow, LEVEL_SELECT));	
	menuItems.push_back(std::make_pair(&nextArrow, LEVEL_SELECT));
	menuItems.push_back(std::make_pair(levelSelectButton, LEVEL_SELECT));
	menuItems.push_back(std::make_pair(levelSelectButton+1, LEVEL_SELECT));
	menuItems.push_back(std::make_pair(levelSelectButton+2, LEVEL_SELECT));
	menuItems.push_back(std::make_pair(levelSelectButton+3, LEVEL_SELECT));
}

void MenuState::Cleanup()
{
	cameraImageTex->releaseOpenCVTexture();
	delete cameraImageTex;

	delete[] levelSelectButton;
	delete[] levelSelectButtonText;

	ParticleSystemManager::getSingleton().releaseSystem(cursorParticle);

	TextureManager::getSingleton().releaseTexture(white);	
	TextureManager::getSingleton().releaseTexture(mainMenuTex);		
	TextureManager::getSingleton().releaseTexture(particlesTex);		

	SoundManager::getSingleton().releaseSound(music);
	SoundManager::getSingleton().releaseSound(click);

	FontManager::getSingleton().releaseFont(fontTygodnik);

	menuItems.clear();
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

	Vector2 oldMouseCoords = mouseCoords;

	if (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
			case SDLK_ESCAPE:
				game->Quit();
				break;
			case SDLK_e:
				GameEngine::getSingleton().ChangeState( EditorState::getSingletonPtr() );
				return;
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
			mouseCoords *= cursorPosMultiplier;
			handSprite.setAngle(markers[i].rotation*180.f/M_PI);
		}
	}

	if(mouseCoords != oldMouseCoords)
	{
		cursorParticle->spawn(mouseCoords, cursorParticle->defaultAmount);
	}
}

bool MenuState::mouseItemClicked(int id)
{
	GameSetup* gameSetup = GameSetup::getSingletonPtr();
	struct dirent* pozycja=0; 
	DIR* directory;
	char bufName[255];
	char bufName2[255];
	int magicNumber;

	click->play();

	switch(id)
	{
	case START_BUTTON:
		// przycisk "START"
		GameSetup::getSingleton().actualMainMenuSubstate = MAIN_MENU;
		break;
	case PLAY_BUTTON:
		// przycisk "GRAJ"		

		levelNames.clear();
		directory = opendir( "Data\\levels" ); //<sciezka> zamieniamy na œcie¿kê do katalogu 

		while (pozycja=readdir(directory)) 
		{ 		
			if(!strcmp(pozycja->d_name,".") || !strcmp(pozycja->d_name,".."))
			{
				// pomiñ katalogi "." i ".."
				continue;
			}

			sprintf(bufName, "Data\\levels\\%s", pozycja->d_name);

			ConfigManager::getSingleton().loadFile(bufName);

			ConfigManager::getSingleton().getValueInt(&magicNumber, bufName, "secretVariable");
			ConfigManager::getSingleton().getValuePChar(bufName2, bufName, "levelName", "ERROR");
			
			if(magicNumber == SECRET_VARIABLE)
			{
				levelNames.push_back(std::make_pair(bufName,bufName2));
			}

			ConfigManager::getSingleton().releaseConfigFile(bufName);
			
		} 

		closedir(directory);

		GameSetup::getSingleton().actualMainMenuSubstate = LEVEL_SELECT;
		break;
	case EDITOR_BUTTON:
		// przycisk "EDYTOR"
		GameEngine::getSingleton().ChangeState( EditorState::getSingletonPtr() );
		GameSetup::getSingleton().actualMainMenuSubstate = EDITOR;
		break;
	case OPTIONS_BUTTON:
		// przycisk "OPCJE"
		GameSetup::getSingleton().actualMainMenuSubstate = OPTIONS;
		break;
	case BACK_LEVELSELECT_BUTTON:
		// przycisk "WROC" z wyboru levelu
		GameSetup::getSingleton().actualMainMenuSubstate = MAIN_MENU;
		break;
	case BACK_OPTIONS_BUTTON:
		// przycisk "WRÓC" z opcji
		GameSetup::getSingleton().actualMainMenuSubstate = MAIN_MENU;
		break;
	case LEVELSELECT_NEXT_ARROW:
		// strza³ka "nastêpna strona" przy wyborze levelu
		levelSelectPageIndex++;
		break;
	case LEVELSELECT_PREV_ARROW:
		// strza³ka "nastêpna strona" przy wyborze levelu
		levelSelectPageIndex--;
		break;
	case LEVELSELECT_LEVEL0:
		sprintf(GameSetup::getSingleton().levelName, "%s", levelNames[levelSelectPageIndex*LEVELS_PER_PAGE].first.c_str());
		GameEngine::getSingleton().ChangeState(GameplayState::getSingletonPtr());
		return true;
		break;
	case LEVELSELECT_LEVEL1:
		sprintf(GameSetup::getSingleton().levelName, "%s", levelNames[levelSelectPageIndex*LEVELS_PER_PAGE+1].first.c_str());
		GameEngine::getSingleton().ChangeState(GameplayState::getSingletonPtr());
		return true;
		break;
	case LEVELSELECT_LEVEL2:
		sprintf(GameSetup::getSingleton().levelName, "%s", levelNames[levelSelectPageIndex*LEVELS_PER_PAGE+2].first.c_str());
		GameEngine::getSingleton().ChangeState(GameplayState::getSingletonPtr());
		return true;
		break;
	case LEVELSELECT_LEVEL3:
		sprintf(GameSetup::getSingleton().levelName, "%s", levelNames[levelSelectPageIndex*LEVELS_PER_PAGE+3].first.c_str());
		GameEngine::getSingleton().ChangeState(GameplayState::getSingletonPtr());
		return true;
		break;
	case BACK_EDITOR_BUTTON:
		// przycisk "WRÓC" z edytora
		GameSetup::getSingleton().actualMainMenuSubstate = MAIN_MENU;
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
			GameSetup::getSingleton().actualMainMenuSubstate = OPTIONS;
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
			GameSetup::getSingleton().actualMainMenuSubstate = OPTIONS;
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
			GameSetup::getSingleton().actualMainMenuSubstate = OPTIONS;
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
			GameSetup::getSingleton().actualMainMenuSubstate = OPTIONS;
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
			GameSetup::getSingleton().actualMainMenuSubstate = OPTIONS;
			gameSetup->save();
		}
		break;
	}

	return false;
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
			&& GameSetup::getSingleton().actualMainMenuSubstate == menuItems[i].second && menuItems[i].first->isActive)
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
				if(mouseItemClicked(i))
				{
					return;
				}
				lastClickedItem = i;
				break;
			}

			mouseHitCounter.updateLinear(game->dt);
			selectedMenuItem = i;			

			break;
		}
	}

	if(GameSetup::getSingleton().actualMainMenuSubstate == LEVEL_SELECT)
	{
		// znikaj strzalke w lewo, gdy grozi zejscie poni¿ej zera
		prevArrow.isActive = levelSelectPageIndex>0;
		nextArrow.isActive = levelSelectPageIndex<(ceil(levelNames.size()/static_cast<float>(LEVELS_PER_PAGE))-1);

		for(int i=0; i<LEVELS_PER_PAGE; ++i)
		{
			if(static_cast<unsigned int>(levelSelectPageIndex*LEVELS_PER_PAGE+i) < levelNames.size())
			{
				levelSelectButton[i].isActive = true;
				levelSelectButtonText[i].isActive = true;
				levelSelectButtonText[i].setText(levelNames[levelSelectPageIndex*LEVELS_PER_PAGE+i].second.c_str());
			}
			else
			{
				levelSelectButton[i].isActive = false;
				levelSelectButtonText[i].isActive = false;
			}
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

	switch(GameSetup::getSingleton().actualMainMenuSubstate)
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
		editorButton.draw(selectedMenuItem==EDITOR_BUTTON?buttonSelectedColor:Color(255,255,255,255));
		optionsButton.draw(selectedMenuItem==OPTIONS_BUTTON?buttonSelectedColor:Color(255,255,255,255));

		white->bind();	
		mainMenuTitleText.draw(fontTygodnik);
		playText.draw(fontTygodnik);
		editorText.draw(fontTygodnik);
		optionsText.draw(fontTygodnik);

		break;
	case LEVEL_SELECT:
		titleBar.draw();		

		backLevelSelectButton.draw(selectedMenuItem==BACK_LEVELSELECT_BUTTON?buttonSelectedColor:Color(255,255,255,255));		

		levelSelectButton[0].draw(selectedMenuItem==LEVELSELECT_LEVEL0?buttonSelectedColor:Color(255,255,255,255));
		levelSelectButton[1].draw(selectedMenuItem==LEVELSELECT_LEVEL1?buttonSelectedColor:Color(255,255,255,255));
		levelSelectButton[2].draw(selectedMenuItem==LEVELSELECT_LEVEL2?buttonSelectedColor:Color(255,255,255,255));
		levelSelectButton[3].draw(selectedMenuItem==LEVELSELECT_LEVEL3?buttonSelectedColor:Color(255,255,255,255));

		prevArrow.draw(selectedMenuItem==LEVELSELECT_PREV_ARROW?buttonSelectedColor:Color(255,255,255,255));
		nextArrow.draw(selectedMenuItem==LEVELSELECT_NEXT_ARROW?buttonSelectedColor:Color(255,255,255,255));

		white->bind();	

		char bufName[255];
		for(int i=0; i<LEVELS_PER_PAGE; i++)
		{
			sprintf(bufName, "levelSelect_level_text%d", i);
			levelSelectButtonText[i].draw(fontTygodnik);
		}

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

	
	particlesTex->bind();
	cursorParticle->updateAndDraw(game->dt);

	// kursor
	mainMenuTex->bind();
	handSprite.draw(mouseCoords);

	// rysowanie obwiedni przedstawiaj¹cej progress klikania obiektów w menu
	int slices = static_cast<int>(ceil(mouseHitCounter.progress/mouseHitCounter.maxValue/ (5.0f/360.0f)));
	for(int i=0; i<slices; ++i)
	{
		handRim.draw(mouseCoords, i*5.0f);
	}
}