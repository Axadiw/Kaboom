
#include <stdio.h>
#include <cmath>

#include "SDL.h"
#include "SDL_opengl.h"
#include "GameEngine.h"
#include "GameplayState.h"
#include "GameState.h"
#include "GameSetup.h"
#include "MenuState.h"
#include "Resources\ConfigManager.h"
#include "includes.h"


static const char* arGameConfFile = "Data\\arGame.txt";

void GameplayState::Init()
{
	white = TextureManager::getSingleton().loadTexture("Data\\white.bmp");
	arGame = TextureManager::getSingleton().loadTexture("Data\\arGame.png");


	ConfigManager::getSingleton().loadFile(arGameConfFile);	

	fontTygodnik = FontManager::getSingleton().loadFont("Data\\Tygodnik.ttf");

	cameraImageTex = new Texture;
	cameraImageTex->initOpenCVTexture();

	cameraImage.load(arGameConfFile, "cameraImage");
	paddle1.load(arGameConfFile, "paddle1");
	paddle2.load(arGameConfFile, "paddle2");

	crossingLine.load(arGameConfFile, "crossingLine");

	paddle1Points.load(arGameConfFile, "paddle1Points");
	paddle2Points.load(arGameConfFile, "paddle2Points");
	ball.load(arGameConfFile, "ball");

	points = Vector2(0.0f);	
	defaultballVelocity.load(arGameConfFile, "defaultBallVelocity");
	ballVelocity = defaultballVelocity;
	paddle1Velocity = Vector2(0.0f);
	paddle2Velocity = Vector2(0.0f);
	paddle1.setAngle(90.0f);
	paddle2.setAngle(90.0f);
	finished = 0;

	
	paddle1WonText.load(arGameConfFile, "paddle1WonText");
	paddle2WonText.load(arGameConfFile, "paddle2WonText");
	victoryTextCounter.load(arGameConfFile, "victoryTextCounter");
	ConfigManager::getSingleton().getValueInt(&pointLimit, arGameConfFile, "pointLimit");

	ConfigManager::getSingleton().releaseConfigFile(arGameConfFile);

}

void GameplayState::Cleanup()
{
	cameraImageTex->releaseOpenCVTexture();
	delete cameraImageTex;

	FontManager::getSingleton().releaseFont(fontTygodnik);
	TextureManager::getSingleton().releaseTexture(white);	
	TextureManager::getSingleton().releaseTexture(arGame);
}

void GameplayState::Pause()
{

}

void GameplayState::Resume()
{

}

void GameplayState::resetBall()
{
	ball.setPos(0.0f, 0.0f);
	ballVelocity = defaultballVelocity;
}

void GameplayState::HandleEvents(GameEngine* game)
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
			case SDLK_q:
				GameEngine::getSingleton().ChangeState( MenuState::getSingletonPtr() );
				break;
			}
			break;
		case SDL_MOUSEMOTION :
			break;
		}
	}

	game->arEngine->GetMarkers(markers);
	float cameraAspectRatio = gameSetup->cameraResolution.x/gameSetup->cameraResolution.y;

	
	Sprite tmpPaddle1 = paddle1;
	Sprite tmpPaddle2 = paddle2;


	for(unsigned int i=0; i<markers.size(); ++i)
	{
		markers[i].center.x = static_cast<int>((markers[i].center.x/gameSetup->cameraResolution.x)*2.0f*SCREEN_WIDTH - SCREEN_WIDTH);
		markers[i].center.y = static_cast<int>(-(markers[i].center.y/gameSetup->cameraResolution.y)*2.0f*SCREEN_WIDTH/cameraAspectRatio + SCREEN_WIDTH/cameraAspectRatio);

		if(markers[i].id==gameSetup->paddle1MarkerID && markers[i].center.x > 0)
		{			
			tmpPaddle1.setPos(static_cast<float>(markers[i].center.x), static_cast<float>(markers[i].center.y));
			if(isColliding(tmpPaddle1,ball)) continue;

			paddle1.setPos(static_cast<float>(markers[i].center.x), static_cast<float>(markers[i].center.y));			

			paddle1History.push_back(paddle1.getPos());
			if(paddle1History.size() > static_cast<unsigned int>(GameSetup::getSingleton().screenFramerate/2))
			{
				paddle1History.erase(paddle1History.begin());
			}

			paddle1Velocity = Vector2(0.0f);
			for(unsigned int i=0; i<paddle1History.size(); ++i)
			{
				paddle1Velocity += paddle1History[i];
			}
			paddle1Velocity*= 1.0f/paddle1History.size();

		}

		if(markers[i].id==gameSetup->paddle2MarkerID && markers[i].center.x < 0)
		{			
			tmpPaddle2.setPos(static_cast<float>(markers[i].center.x), static_cast<float>(markers[i].center.y));
			if(isColliding(tmpPaddle2,ball)) continue;

			paddle2.setPos(static_cast<float>(markers[i].center.x), static_cast<float>(markers[i].center.y));		

			paddle2History.push_back(paddle1.getPos());
			if(paddle2History.size() > static_cast<unsigned int>(GameSetup::getSingleton().screenFramerate/2))
			{
				paddle2History.erase(paddle2History.begin());
			}

			paddle2Velocity = Vector2(0.0f);
			for(unsigned int i=0; i<paddle2History.size(); ++i)
			{
				paddle2Velocity += paddle2History[i];
			}
			paddle2Velocity*= 1.0f/paddle2History.size();
		}
	}


}

bool GameplayState::isColliding(Sprite box, Sprite ball)
{
	if( fabs(box.getPos().x - ball.getPos().x) <= (box.getScale()*box.getSize().y/2.0f + ball.getScale()*ball.getSize().x/2.0f) &&
		fabs(box.getPos().y - ball.getPos().y) <= (box.getScale()*box.getSize().x/2.0f + ball.getScale()*ball.getSize().y/2.0f))
	{
		return true;
	}

	return false;
}

void GameplayState::Update(GameEngine* game) 
{
	if(finished != 0)
	{
		victoryTextCounter.updateLinear(game->dt);

		if(victoryTextCounter.finished())
		{
			finished = 0;
			resetBall();
			points = Vector2(0.0f);
			victoryTextCounter.progress = victoryTextCounter.minValue;
		}

		return;
	}

	Vector2 prevBallPos = ball.getPos();

	// przesuwanie pi³ki
	ball.setPos(ball.getPos() + ballVelocity*game->dt);

	//odbijanie od œcianek
	if(ball.getPos().y + ball.getSize().y/2.0f > SCREEN_HEIGHT || ball.getPos().y - ball.getSize().y/2.0f < -SCREEN_HEIGHT)
	{
		ballVelocity.y *= -1;		
		ball.setPos(prevBallPos);
	}

	// wygrana tego po lewej
	if(ball.getPos().x > SCREEN_WIDTH)
	{
		points.x += 1;		

		if(points.x >= pointLimit)
		{
			finished = -1;
		}
		else
		{
			resetBall();
			ballVelocity.x *= -1;
		}		
	}

	// wygrana tego po prawej
	if(ball.getPos().x < -SCREEN_WIDTH)
	{
		points.y += 1;

		if(points.y >= pointLimit)
		{
			finished = 1;
		}
		else
		{
			resetBall();
		}	
	}

	// kolizje z paletkami
	if(isColliding(paddle1, ball))
	{
		if(ball.getPos().y - ball.getSize().y/2.0f > paddle1.getPos().y + paddle1.getSize().y/2.0f || 
			ball.getPos().y + ball.getSize().y/2.0f < paddle1.getPos().y - paddle1.getSize().y/2.0f)
		{
			ballVelocity.y *= -1;
		}
		else
		{
			ballVelocity.x *= -1;
		}

		ballVelocity -= paddle1Velocity*game->dt;
		ball.setPos(prevBallPos);
	}
	else if(isColliding(paddle2, ball))
	{
		if(ball.getPos().y - ball.getSize().y/2.0f > paddle2.getPos().y + paddle2.getSize().y/2.0f || 
			ball.getPos().y + ball.getSize().y/2.0f < paddle2.getPos().y - paddle2.getSize().y/2.0f)
		{
			ballVelocity.y *= -1;
		}
		else
		{
			ballVelocity.x *= -1;
		}		
		ballVelocity -= paddle2Velocity*game->dt;
		ball.setPos(prevBallPos);
	}

	float a =GameSetup::getSingleton().ballSpeedupFactor;
	float b = a*game->dt;

	ballVelocity *= 1.0f + b;

	// wyniki
	sprintf(paddle1Points.text, "%d", static_cast<int>(points.x));
	sprintf(paddle2Points.text, "%d", static_cast<int>(points.y));
}

void GameplayState::Draw(GameEngine* game) 
{	
	cameraImageTex->updateFromOpenCvmat(*(GameEngine::getSingleton().arEngine->image_));
	cameraImage.draw();	

	arGame->bind();
	crossingLine.draw();
	paddle1.draw();
	paddle2.draw();
	ball.draw();

	white->bind();

	if(finished >0)
	{
		paddle2WonText.draw(fontTygodnik);
	}
	else if(finished < 0)
	{
		paddle1WonText.draw(fontTygodnik);
	}

	paddle1Points.draw(fontTygodnik);
	paddle2Points.draw(fontTygodnik);	
}