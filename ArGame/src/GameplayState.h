#pragma once

#include "includes.h"
#include "SDL.h"
#include "GameState.h"
#include "Resources/TextureManager.h"
#include "Resources/FontManager.h"
#include "Resources/Text.h"
#include "Resources/Sprite.h"
#include "Basic/ProgressCounter.h"

#include <vector>

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

private:	
	// zresetuj po³orzenie pi³ki
	void resetBall();

	// czy koliduje po³ka z paletk¹
	bool isColliding(Sprite box, Sprite ball);

	// tekstury
	Texture* white;	
	Texture* arGame;	
	Texture* cameraImageTex;

	// czcionka
	Font* fontTygodnik;

	// sprite-y
	Sprite cameraImage;
	Sprite paddle1;
	Sprite paddle2;
	Sprite crossingLine;
	Sprite ball;

	Vector2 ballVelocity;
	Vector2 defaultballVelocity;
	Vector2 paddle1Velocity;
	Vector2 paddle2Velocity;

	// teksty
	Text paddle1Points;
	Text paddle2Points;

	Text paddle1WonText;
	Text paddle2WonText;
	ProgressCounter victoryTextCounter;
	int pointLimit;

	/// -1 paddle1 0 - false 1 - paddle2
	int finished;

	std::vector<Vector2> paddle1History;
	std::vector<Vector2> paddle2History;
	Vector2 points;
	
	std::vector<Marker> markers;	
};