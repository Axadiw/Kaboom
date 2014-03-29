#pragma once

#include "includes.h"
#include "SDL.h"
#include "GameState.h"
#include "Brick.h"
#include "Resources/TextureManager.h"
#include "Resources/FontManager.h"
#include "Resources/Text.h"
#include "Resources/Sprite.h"
#include "Basic/ProgressCounter.h"

#include <Box2D/Box2D.h>

#include <vector>

class EditorState : public GameState, public Singleton<EditorState>
{
public:
	void Init();
	void Cleanup();

	void Pause();
	void Resume();

	void HandleEvents(GameEngine* game);
	void Update(GameEngine* game);
	void Draw(GameEngine* game);

	vector<Brick*> bricks;
	float destroyRadius;
private:	
	void explode(b2Vec2 position, float radius, float strength);
	void save();
	void load();
	void clear();

	// ustaw status
	void setStatus(char* text);

	//dodaj klocek
	void addBrick(int id);

	Texture* white;	
	Texture* bricksTex;	
	Texture* backgroundsTex;	

	Font* fontTygodnik;

	Text physicsOnText;

	Vector2 mouseCoords;

	// pod³oga
	b2Vec2* gravity;
	b2World* world;	
	b2BodyDef* groundBodyDef;
	b2PolygonShape* groundBox;
	b2Body* groundBody;

	// t³a
	int backgroundsNum;
	Sprite background;
	int actualBackgroundChangeIndex;

	// teksty
	Text showHelpText;
	Text statusText;
	Text* helpLines;
	int helpLinesNum;
	bool showHelp;

	char levelName[255];

	Vector2 victoryLine;
	Sprite victoryLineGraphic;

	float rotateStep;
	float brickSelectDistance;

	Vector2 physicsQuality;

	Text dynamitesNumText;
	int dynamitesNum;
	float explodeRadius;
	float explodeStrength;

	ProgressCounter statusTextCounter;

	int draggedBrickID;
	bool dragBrick;
	bool simulatePhysics;
};