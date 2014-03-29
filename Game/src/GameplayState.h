#pragma once

#include "includes.h"
#include "SDL.h"
#include "GameState.h"
#include "Brick.h"
#include "Resources/TextureManager.h"
#include "Resources/FontManager.h"
#include "Resources/SoundManager.h"
#include "Resources/ParticleSystemManager.h"
#include "Resources/Text.h"
#include "Resources/Sprite.h"
#include "Basic/ProgressCounter.h"

#include <Box2D/Box2D.h>

#include <vector>
#include <map>

enum GAMEPLAY_SUBSTATES { COLLECT_MARKERS, GAMEPLAY};

class GameplayState : public GameState, public Singleton<GameplayState>
{
public:
	void Init();
	void Cleanup();

	void Pause();
	void Resume();

	void HandleEvents(GameEngine* game);
	void Update(GameEngine* game);
	void Draw(GameEngine* game);

	float destroyRadius;
	vector<Brick*> bricks;
private:	
	void explode(b2Vec2 position, float radius, float strength);
	void load();
	void clear();
	void reset();
	bool mouseItemClicked(int id);
	void proceedToGameplay();

	Sound* explosionSound;
	Sound* music;
	Sound* click;
	Sound* fanfare;
	Sound* applause;
	Sound* fireworks;

	ParticleSystem* fireSystem;
	ParticleSystem* cursorParticle;
	ParticleSystem* fireworks1;
	ParticleSystem* fireworks2;
	ParticleSystem* fireworks3;

	Texture* white;	
	Texture* particlesTex;	
	Texture* bricksTex;	
	Texture* backgroundsTex;	
	Texture* mainMenuTex;
	Texture* GameTex;

	ProgressCounter fireworksLaunchInterval;
	ProgressCounter mouseHitCounter;
	Sprite handSprite;
	Sprite handRim;

	Font* fontTygodnik;	

	Vector2 mouseCoords;

	b2Vec2* gravity;
	b2World* world;

	b2BodyDef* groundBodyDef;
	b2PolygonShape* groundBox;
	b2Body* groundBody;

	b2BodyDef* leftWallBodyDef;
	b2PolygonShape* leftWallBox;
	b2Body* leftWallBody;

	b2BodyDef* rightWallBodyDef;
	b2PolygonShape* rightWallBox;
	b2Body* rightWallBody;	

	std::vector<Marker> markers;
	
	Sprite background;	

	
	bool gameOver;

	Vector2 victoryLine;
	Sprite victoryLineGraphic;

	Vector2 physicsQuality;
	
	std::map<int, bool> whichDynamitesExploded;
	std::map<int, int> dynamiteAnimationIndexes;
	std::map<int, Marker*> dynamiteMarkers;

	std::vector<Sprite*> dynamiteSprite;
	ProgressCounter dynamiteSpritesInterval;

	int dynamitesNum;
	float explodeRadius;
	float explodeStrength;

	char levelName[255];
	
	bool simulatePhysics;

	Sprite launchButton;
	Sprite backButton;

	Text launchButtonText;
	Text ResetButtonText;
	Text backButtonText;

	bool dynamitesGenerated;
	float cursorPosMultiplier;
	int draggedBrickID;
	bool dragBrick;
	float brickSelectDistance;

	ProgressCounter collectMarkersBackgroundFadeCounter;
	ProgressCounter collectMarkersScreenFadeCounter;
	ProgressCounter collectMarkersTimeoutCounter;
	ProgressCounter intervalBeetweenExplosions;

	Sprite collectMarkersProgressRim;
	Sprite collectMarkersScreenFade;
	Sprite collectMarkersBackground;
	Text collectMarkersLevelName;
	Text collectMarkersNeededMarkersText;
	Text collectMarkersCursorText;
	Text collectMarkersCollectedMarkers;
	Text victoryText;

	Color OKColor;
	Color NotOKColor;

	Color buttonEnabledColor;
	Color buttonSelectedColor;
	int selectedMenuItem;	
	// ostatnio klikniêty element w menu (wykorzystywane przy zabezpieczeniu, przed klikaniem wiele razy tego samego obiektu)
	int lastClickedItem;
	std::vector<std::pair<Sprite*, GAMEPLAY_SUBSTATES> > menuItems;
	GAMEPLAY_SUBSTATES substate;
};