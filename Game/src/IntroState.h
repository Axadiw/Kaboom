#pragma once

#include "SDL.h"
#include "GameState.h"
#include "Resources/TextureManager.h"
#include "Resources/Sprite.h"
#include "Basic/ProgressCounter.h"
#include "Resources/FontManager.h"
#include "Resources/Text.h"

enum SUBSTATE_TYPE { FADING, SHOWING_LOGOS};

class IntroState : public GameState, public Singleton<IntroState>
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
	Texture* logosTex;
	Texture* white;

	Font* fontVerdana;

	ProgressCounter fadeCounter;
	ProgressCounter showImagesTimeCounter;	
	Sprite fullScreenPanel;
	Sprite polygonLogo;
	Sprite fmodLogo;
	Sprite sdlLogo;
	Sprite box2DLogo;

	Text* texts;
	int textsCount;

	SUBSTATE_TYPE actualSubstateType;

	bool fade;
	u8 substateNumber;
};
