#pragma once

#include "includes.h"
#include "SDL.h"
#include "GameState.h"
#include "GameSetup.h"
#include "Resources/TextureManager.h"
#include "Resources/FontManager.h"
#include "Resources/SoundManager.h"
#include "Resources/ParticleSystemManager.h"
#include "Resources/Text.h"
#include "Resources/Sprite.h"
#include "Basic/ProgressCounter.h"

#include <vector>
#include <string>

class MenuState : public GameState, public Singleton<MenuState>
{
public:
	void Init();
	void Cleanup();

	void Pause();
	void Resume();

	void HandleEvents(GameEngine* game);
	void Update(GameEngine* game);
	void Draw(GameEngine* game);

private:	
	bool mouseItemClicked(int id);

	Texture* white;
	Texture* mainMenuTex;
	Texture* cameraImageTex;
	Texture* particlesTex;	

	Sound* music;
	Sound* click;

	Font* fontTygodnik;
	
	ParticleSystem* cursorParticle;

	Text nextText;

	Sprite background;
	Sprite gameLogo;
	Sprite startButton;
	Sprite titleBar;
	Sprite blob0,blob1,blob2;
	Sprite handSprite;
	Sprite handRim;
	// ostatnio klikniêty element w menu (wykorzystywane przy zabezpieczeniu, przed klikaniem wiele razy tego samego obiektu)
	int lastClickedItem;

	Sprite playButton;
	Sprite editorButton;
	Sprite optionsButton;
	Sprite backLevelSelectButton;
	Sprite backOptionsButton;
	Sprite backEditorButton;
	Text playText;
	Text editorText;
	Text optionsText;
	Text backLevelSelectText;
	Text backOptionsText;
	Text backEditorText;
	Text mainMenuTitleText;
	Text optionsTitleText;
	Text editorTitleText;
	Text levelSelectTitleText;

	//opcje	
	Text optionsCameraResolutionText;
	Text optionsCameraResolutionLowText;
	Text optionsCameraResolutionMediumText;
	Text optionsCameraResolutionHighText;
	Text optionsCameraImageText;
	Text optionsScreenResolutionText;
	Text optionsScreenResolutionLowText;
	Text optionsScreenResolutionMediumText;
	Text optionsScreenResolutionHighText;
	Text optionsFullscreenText;
	Text optionsFullscreenYesText;
	Text optionsFullscreenNoText;
	Sprite optionsCameraResolutionLowButton;
	Sprite optionsCameraResolutionMediumButton;
	Sprite optionsCameraResolutionHighButton;
	Sprite optionsScreenResolutionLowButton;
	Sprite optionsScreenResolutionMediumButton;
	Sprite optionsScreenResolutionHighButton;
	Sprite optionsFullscreenYesButton;
	Sprite optionsFullscreenNoButton;
	Sprite cameraImage;

	float cursorPosMultiplier;

	int levelSelectPageIndex;
	Sprite prevArrow,nextArrow;
	Sprite* levelSelectButton;
	Text* levelSelectButtonText;

	ProgressCounter blob0Counter,blob1Counter,blob2Counter;

	Color buttonEnabledColor;
	Color buttonSelectedColor;

	ProgressCounter mouseHitCounter;
	Vector2 mouseCoords;
	
	std::vector<std::pair<std::string,std::string> > levelNames;
	std::vector<Marker> markers;	

	std::vector<std::pair<Sprite*, MAINMENU_SUBSTATE> > menuItems;
	int selectedMenuItem;	
};