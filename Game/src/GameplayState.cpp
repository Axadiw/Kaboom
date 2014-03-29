
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

enum GAMEPLAY_MENU_ITEMS{
	LAUNCH_BUTTON,
	EXIT_BUTTON_COLLECTING_MARKERS,
	EXIT_BUTTON_GAMEPLAY
};

static const char* GameConfFile = "Data\\Game.txt";
static const char* GeneralConfFile = "Data\\generalConfig.txt";

/// wykonuje odrzut spowownaany eksplozj¹ na pojedynczym klocku
class ExplodeElement : public b2QueryCallback
{
public:
	b2World* world;
	float strength;
	b2Vec2 position;

	ExplodeElement(b2World* world_, b2Vec2 position_, float strength_):world(world_), position(position_), strength(strength_){};

	bool ReportFixture(b2Fixture* fixture)
	{
		b2Body* body = fixture->GetBody();
		body->SetAwake(true);
		b2Vec2 forceVec;

		forceVec = body->GetWorldCenter() - position;
		float distance = forceVec.Length();

		forceVec.Normalize();
		forceVec *= strength;		

		body->ApplyForceToCenter(forceVec);

		if(distance < GameplayState::getSingleton().destroyRadius)
		{			
			
			for(unsigned int i=0; i<GameplayState::getSingleton().bricks.size(); i++)
				{					
					if(GameplayState::getSingleton().bricks[i]->body == body)
					{
						delete GameplayState::getSingleton().bricks[i];
						GameplayState::getSingleton().bricks.erase(GameplayState::getSingleton().bricks.begin() + i);

						break;
					}
				}


		}

		return true;
	}
};

class DetectVictory : public b2QueryCallback
{
public:
	bool* gameOver;

	DetectVictory(bool* gameOver_):gameOver(gameOver_){};

	bool ReportFixture(b2Fixture* fixture)
	{
		*gameOver = true;

		return true;
	}
};

void GameplayState::clear()
{
	if(bricks.size() > 0)
	{
		for(unsigned int i=0; i<bricks.size(); i++)
		{
			delete bricks[i];
		}

		for(int i=bricks.size()-1; i>=0; --i)
		{
			bricks.erase(bricks.begin()+i);
		}
	}
}

void GameplayState::load()
{
	// wyczysc liste klockow
	clear();

	char levelPath[255];
	sprintf(levelPath, "%s", GameSetup::getSingleton().levelName);

	ConfigManager::getSingleton().loadFile(levelPath);

	ConfigManager::getSingleton().getValuePChar(levelName, levelPath, "levelName", "Nowy poziom");	
	collectMarkersLevelName.setText(levelName);

	int bricksNum;
	char bufName[255];
	ConfigManager::getSingleton().getValueInt(&bricksNum, levelPath, "bricksNum");	

	ConfigManager::getSingleton().getValueInt(&dynamitesNum, levelPath, "dynamitesNum", 0);	

	background.load(levelPath, "background");
	victoryLine.load(levelPath, "victoryLine", Vector2(0.0f, -50.0f));

	Brick* tmpBrick;

	for(int i=0; i< bricksNum; ++i)
	{
		sprintf(bufName, "brick_%d", i);

		tmpBrick = new Brick(world);
		tmpBrick->load(levelPath, bufName);
		tmpBrick->body->SetTransform(b2Vec2(tmpBrick->sprite.getPos().x, tmpBrick->sprite.getPos().y), tmpBrick->sprite.getAngle());

		bricks.push_back(tmpBrick);
	}

	ConfigManager::getSingleton().releaseConfigFile(levelPath);
}

void GameplayState::reset()
{
	// zresetuj stan dynamitów
	for(std::map<int, int>::iterator it = dynamiteAnimationIndexes.begin(); it!=dynamiteAnimationIndexes.end(); ++it)
	{
		it->second = 0;
	}
	for(std::map<int, bool>::iterator it = whichDynamitesExploded.begin(); it!=whichDynamitesExploded.end(); ++it)
	{
		it->second = false;
	}	

	if(simulatePhysics)
	{
		// reset
		load();
	}

	gameOver = false;
	simulatePhysics = !simulatePhysics;
}

void GameplayState::Init()
{
	int tmpX,tmpY;
	SDL_GetMouseState(&tmpX, &tmpY);
	mouseCoords= Vector2(static_cast<float>(tmpX), static_cast<float>(tmpY));
	mouseCoords.x = (mouseCoords.x/GameEngine::getSingleton().screenDimensions.x)*2.0f*SCREEN_WIDTH - SCREEN_WIDTH;
	mouseCoords.y = -(mouseCoords.y/GameEngine::getSingleton().screenDimensions.y)*2.0f*SCREEN_HEIGHT + SCREEN_HEIGHT;

	sprintf(levelName, "%s", GameSetup::getSingleton().levelName);

	// tekstury
	white = TextureManager::getSingleton().loadTexture("Data\\white.bmp");	
	bricksTex = TextureManager::getSingleton().loadTexture("Data\\bricks.png");	
	backgroundsTex = TextureManager::getSingleton().loadTexture("Data\\backgrounds.png");	
	fontTygodnik = FontManager::getSingleton().loadFont("Data\\Tygodnik.ttf");	
	mainMenuTex = TextureManager::getSingleton().loadTexture("Data\\mainMenuTex.png");
	GameTex = TextureManager::getSingleton().loadTexture("Data\\GameTex.png");
	particlesTex = TextureManager::getSingleton().loadTexture("Data\\particlesTex.png");

	// dŸwiêki
	explosionSound = SoundManager::getSingleton().loadSound("Data\\sounds\\explosion.wav", SFX, false);
	click = SoundManager::getSingleton().loadSound("Data\\sounds\\click.ogg", SFX, false);
	applause = SoundManager::getSingleton().loadSound("Data\\sounds\\applause.ogg", SFX, false);
	fanfare = SoundManager::getSingleton().loadSound("Data\\sounds\\fanfare.ogg", SFX, false);
	fireworks = SoundManager::getSingleton().loadSound("Data\\sounds\\fireworks.ogg", SFX, false);
	music = SoundManager::getSingleton().loadSound("Data\\sounds\\PlacingBombs.mp3", MUSIC, true);		
	music->play();

	// particle
	fireSystem = ParticleSystemManager::getSingleton().loadSystem("Data\\particles\\fire.txt");
	cursorParticle = ParticleSystemManager::getSingleton().loadSystem("Data\\particles\\mouseParticle.txt");
	fireworks1 = ParticleSystemManager::getSingleton().loadSystem("Data\\particles\\fireworks1.txt");
	fireworks2 = ParticleSystemManager::getSingleton().loadSystem("Data\\particles\\fireworks2.txt");
	fireworks3 = ParticleSystemManager::getSingleton().loadSystem("Data\\particles\\fireworks3.txt");
	
	// fizyka
	ConfigManager::getSingleton().loadFile(GeneralConfFile);	
	Vector2 tmpGravity;
	tmpGravity.load(GeneralConfFile, "gravity");
	gravity = new b2Vec2(tmpGravity.x, tmpGravity.y);	
	ConfigManager::getSingleton().getValueFloat(&destroyRadius, GeneralConfFile, "destroy_radius");
	ConfigManager::getSingleton().getValueFloat(&explodeRadius, GeneralConfFile, "explode_radius");
	ConfigManager::getSingleton().getValueFloat(&explodeStrength, GeneralConfFile, "explode_strength");
	physicsQuality.load(GeneralConfFile, "physics_quality");
	ConfigManager::getSingleton().releaseConfigFile(GeneralConfFile);


	ConfigManager::getSingleton().loadFile(GameConfFile);	

	//dynamity
	int dynamiteSpritesNum;
	ConfigManager::getSingleton().getValueInt(&dynamiteSpritesNum, GameConfFile, "dynamite_frames_num");
	dynamiteSpritesInterval.load(GameConfFile, "dynamite_sprites_interval");

	Sprite* dynamite;
	char buffer[255];

	for(int i=0; i<dynamiteSpritesNum; ++i)
	{
		dynamite = new Sprite;
		sprintf(buffer, "dynamite_%d",i);
		dynamite->load(GameConfFile, buffer);

		dynamiteSprite.push_back(dynamite);
	}

	
	ConfigManager::getSingleton().getValueFloat(&brickSelectDistance, GameConfFile, "brick_select_distance");

	// teksty i ekran przyciemniaj¹cy na pocz¹tku poziomu
	collectMarkersBackgroundFadeCounter.load(GameConfFile, "collectMarkersBackgroundFadeCounter");
	collectMarkersScreenFadeCounter.load(GameConfFile, "collectMarkersScreenFadeCounter");
	collectMarkersScreenFade.load(GameConfFile, "collectMarkersScreenFade");
	collectMarkersBackground.load(GameConfFile, "collectMarkersBackground");
	collectMarkersLevelName.load(GameConfFile, "collectMarkersLevelName");
	collectMarkersNeededMarkersText.load(GameConfFile, "collectMarkersNeededMarkersText");
	collectMarkersCursorText.load(GameConfFile, "collectMarkersCursorText");
	collectMarkersCollectedMarkers.load(GameConfFile, "collectMarkersCollectedMarkers");
	OKColor.load(GameConfFile, "OKColor");
	NotOKColor.load(GameConfFile, "NotOKColor");

	victoryLineGraphic.load(GameConfFile, "victoryLine");

	fireworksLaunchInterval.load(GameConfFile, "fireworksLaunchInterval");
	handSprite.load(GameConfFile, "handSprite");
	handRim.load(GameConfFile, "handRim");
	mouseHitCounter.load(GameConfFile, "mouseHitCounter");

	ConfigManager::getSingleton().getValueFloat(&cursorPosMultiplier, GameConfFile, "cursorPosMultiplier");

	intervalBeetweenExplosions.load(GameConfFile, "intervalBeetweenExplosions");	
	collectMarkersTimeoutCounter.load(GameConfFile, "collectMarkersTimeoutCounter");

	launchButton.load(GameConfFile,"launchButton");
	backButton.load(GameConfFile,"backButton");	
	collectMarkersProgressRim.load(GameConfFile,"collectMarkersProgressRim");	

	victoryText.load(GameConfFile, "victoryText");
	launchButtonText.load(GameConfFile, "launchButtonText");
	ResetButtonText.load(GameConfFile, "ResetButtonText");
	backButtonText.load(GameConfFile, "backButtonText");

	buttonEnabledColor.load(GameConfFile, "buttonEnabledColor");
	buttonSelectedColor.load(GameConfFile, "buttonSelectedColor");

	ConfigManager::getSingleton().releaseConfigFile(GameConfFile);

	gameOver = false;
	dynamitesGenerated = false;
	simulatePhysics = false;	
	substate = COLLECT_MARKERS;

	menuItems.push_back(std::make_pair(&launchButton, GAMEPLAY));
	menuItems.push_back(std::make_pair(&backButton, COLLECT_MARKERS));
	menuItems.push_back(std::make_pair(&backButton, GAMEPLAY));

	// ustawienia BOX2D	
	world = new b2World(*gravity);

	// ustaw pod³oge
	groundBodyDef = new b2BodyDef();
	groundBox = new b2PolygonShape();
	groundBodyDef->position.Set(0.0f, (-SCREEN_HEIGHT + GameSetup::getSingleton().groundLevel)/GameSetup::getSingleton().physicScale);
	groundBody = world->CreateBody(groundBodyDef);	
	groundBox->SetAsBox(2*SCREEN_WIDTH/GameSetup::getSingleton().physicScale, 0.1f);	
	groundBody->CreateFixture(groundBox, 0.0f);		

	load();
}

void GameplayState::explode(b2Vec2 position, float radius, float strength)
{
	b2AABB aabb;

	fireSystem->spawn(Vector2(position.x, position.y), fireSystem->defaultAmount);

	position *= 1.0f/GameSetup::getSingleton().physicScale;

	ExplodeElement explodeElement(world, position, strength);

	aabb.lowerBound = position - b2Vec2(radius, radius);
	aabb.upperBound = position + b2Vec2(radius, radius);

	world->QueryAABB(&explodeElement, aabb);		
}

void GameplayState::Cleanup()
{
	// usun klocki
	clear();

	delete groundBodyDef;
	delete groundBox;
	delete world;
	delete gravity;	
	

	for(std::vector<Sprite*>::iterator it = dynamiteSprite.begin(); it != dynamiteSprite.end(); ++it)
	{
		delete *it;
	}

	dynamiteSprite.clear();

	for(std::map<int, Marker*>::iterator it = dynamiteMarkers.begin(); it!=dynamiteMarkers.end(); ++it)
	{
		delete it->second;
	}

	ParticleSystemManager::getSingleton().releaseSystem(fireSystem);
	ParticleSystemManager::getSingleton().releaseSystem(cursorParticle);
	ParticleSystemManager::getSingleton().releaseSystem(fireworks1);
	ParticleSystemManager::getSingleton().releaseSystem(fireworks2);
	ParticleSystemManager::getSingleton().releaseSystem(fireworks3);
	dynamiteMarkers.clear();
	dynamiteAnimationIndexes.clear();
	whichDynamitesExploded.clear();

	FontManager::getSingleton().releaseFont(fontTygodnik);

	SoundManager::getSingleton().releaseSound(explosionSound);
	SoundManager::getSingleton().releaseSound(music);
	SoundManager::getSingleton().releaseSound(click);
	SoundManager::getSingleton().releaseSound(fanfare);
	SoundManager::getSingleton().releaseSound(applause);
	SoundManager::getSingleton().releaseSound(fireworks);

	TextureManager::getSingleton().releaseTexture(white);
	TextureManager::getSingleton().releaseTexture(backgroundsTex);
	TextureManager::getSingleton().releaseTexture(bricksTex);
	TextureManager::getSingleton().releaseTexture(particlesTex);
	TextureManager::getSingleton().releaseTexture(mainMenuTex);
	TextureManager::getSingleton().releaseTexture(GameTex);
}

void GameplayState::Pause()
{

}

void GameplayState::Resume()
{

}

void GameplayState::HandleEvents(GameEngine* game)
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
			case SDLK_q:
				GameEngine::getSingleton().ChangeState( MenuState::getSingletonPtr() );
				break;
			case SDLK_f:
				applause->play();
				fanfare->play();
				break;
			case SDLK_s:
				proceedToGameplay();

				for(int i=0; i<dynamitesNum; ++i)
				{
					dynamiteMarkers[-i] = new Marker();
					dynamiteAnimationIndexes[-i] = 0;
					whichDynamitesExploded[-i] = false;
				}

				break;
			}
			break;
		case SDL_MOUSEMOTION :

			// przeliczenie wspó³rzêdnych kursora myszy na wspó³rzêdne ekranu
			mouseCoords = Vector2(event.motion.x, event.motion.y);
			mouseCoords.x = (mouseCoords.x/game->screenDimensions.x)*2.0f*SCREEN_WIDTH - SCREEN_WIDTH;
			mouseCoords.y = -(mouseCoords.y/game->screenDimensions.y)*2.0f*SCREEN_HEIGHT + SCREEN_HEIGHT;

			if(dragBrick)
			{
				if(substate==GAMEPLAY && dynamiteMarkers.find(draggedBrickID) != dynamiteMarkers.end())
				{
					dynamiteMarkers[draggedBrickID]->center = cv::Point(static_cast<int>(mouseCoords.x), static_cast<int>(mouseCoords.y));
				}
			}

			break;
		case SDL_MOUSEBUTTONDOWN:

			for(std::map<int, Marker*>::iterator it = dynamiteMarkers.begin(); it!=dynamiteMarkers.end(); ++it)
			{					
				Vector2 tmp = Vector2(it->second->center.x, it->second->center.y);
				if(mouseCoords.distance(tmp) < brickSelectDistance)
				{
					draggedBrickID = it->first;
					dragBrick = true;

					break;
				}
			}						
			break;
		case SDL_MOUSEBUTTONUP:								
			dragBrick = false;
			break;
		
		}
	}

	game->arEngine->GetMarkers(markers);
	float cameraAspectRatio = gameSetup->cameraResolution.x/gameSetup->cameraResolution.y;

	for(unsigned int i=0; i<markers.size(); ++i)
	{
		markers[i].center.x = static_cast<int>((markers[i].center.x/gameSetup->cameraResolution.x)*2.0f*SCREEN_WIDTH - SCREEN_WIDTH);
		markers[i].center.y = static_cast<int>(-(markers[i].center.y/gameSetup->cameraResolution.y)*2.0f*SCREEN_WIDTH/cameraAspectRatio + SCREEN_WIDTH/cameraAspectRatio);
		markers[i].center *= cursorPosMultiplier;

		if(markers[i].id==gameSetup->cursorMarkerID)
		{
			mouseCoords.x = static_cast<float>(markers[i].center.x);
			mouseCoords.y = static_cast<float>(markers[i].center.y);
			handSprite.setAngle(markers[i].rotation*180.f/M_PI);
		}
		else
		{
			if(substate==GAMEPLAY && dynamiteMarkers.find(markers[i].id) != dynamiteMarkers.end())
			{
				dynamiteMarkers[markers[i].id]->center = markers[i].center;
				dynamiteMarkers[markers[i].id]->rotation = markers[i].rotation*180.f/M_PI;					
			}
		}
	}

	if(mouseCoords != oldMouseCoords)
	{
		cursorParticle->spawn(mouseCoords, cursorParticle->defaultAmount);
	}
}

bool GameplayState::mouseItemClicked(int id)
{
	GameSetup* gameSetup = GameSetup::getSingletonPtr();		
	switch(id)
	{
	case LAUNCH_BUTTON:
		reset();
		break;
	case EXIT_BUTTON_COLLECTING_MARKERS:
		GameEngine::getSingleton().ChangeState( MenuState::getSingletonPtr() );
		return true;
	case EXIT_BUTTON_GAMEPLAY:
		GameEngine::getSingleton().ChangeState( MenuState::getSingletonPtr() );
		return true;
	}

	return false;
}

void GameplayState::Update(GameEngine* game) 
{
	if(simulatePhysics)
	{
		world->Step(game->dt, static_cast<int32>(physicsQuality.x), static_cast<int32>(physicsQuality.y));	
	}

	// kolizje z elementami menu
	bool collisionDetedted = false;
	for(unsigned int i=0; i<menuItems.size(); ++i)
	{
		if( fabs(mouseCoords.x - menuItems[i].first->getPos().x) <= (menuItems[i].first->getScale()*menuItems[i].first->getSize().x/2.0f + handSprite.getScale()*handSprite.getSize().x/2.0f) &&
			fabs(mouseCoords.y - menuItems[i].first->getPos().y) <= (menuItems[i].first->getScale()*menuItems[i].first->getSize().y/2.0f + handSprite.getScale()*handSprite.getSize().y/2.0f)
			&& substate == menuItems[i].second && menuItems[i].first->isActive)
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

	// brak kolizji
	if(!collisionDetedted)
	{
		lastClickedItem = -1;
		selectedMenuItem = -1;
		mouseHitCounter.progress = mouseHitCounter.minValue;
	}	
	collectMarkersBackgroundFadeCounter.updateLinear(game->dt);
	collectMarkersScreenFadeCounter.updateLinear(game->dt);

	if(substate == COLLECT_MARKERS)
	{
		bool cursorFound = false;
		// sprawdzanei, czy mo¿na rozpocz¹æ grê
		int foundNotCursorMarkers = 0;
		for(unsigned int i=0; i<markers.size(); ++i)
		{
			if(markers[i].id==GameSetup::getSingleton().cursorMarkerID)
			{
				// znaleziono kursor!
				cursorFound = true;
			}
			else
			{
				foundNotCursorMarkers++;
			}
		}

		if(foundNotCursorMarkers == dynamitesNum && cursorFound)
		{
			collectMarkersTimeoutCounter.updateLinear(game->dt);
		}
		else
		{
			collectMarkersTimeoutCounter.progress = collectMarkersTimeoutCounter.minValue;
		}

		if(!dynamitesGenerated && collectMarkersTimeoutCounter.finished())
		{
			dynamitesGenerated = true;
			for(unsigned int i=0; i<markers.size(); ++i)
			{
				if(markers[i].id!=GameSetup::getSingleton().cursorMarkerID)
				{
					dynamiteMarkers[markers[i].id] = new Marker();
					dynamiteMarkers[markers[i].id]->center = markers[i].center;
					dynamiteMarkers[markers[i].id]->rotation = markers[i].rotation*180.f/M_PI;					

					dynamiteMarkers[markers[i].id]->center.x = static_cast<int>((dynamiteMarkers[markers[i].id]->center.x/game->screenDimensions.x)*2.0f*SCREEN_WIDTH - SCREEN_WIDTH);
					dynamiteMarkers[markers[i].id]->center.y = static_cast<int>(-(dynamiteMarkers[markers[i].id]->center.y/game->screenDimensions.y)*2.0f*SCREEN_HEIGHT + SCREEN_HEIGHT);

					dynamiteAnimationIndexes[markers[i].id] = 0;
					whichDynamitesExploded[markers[i].id] = false;
				}
			}

			proceedToGameplay();
		}
	}

	if(collectMarkersBackgroundFadeCounter.direction == DOWN && collectMarkersScreenFadeCounter.direction == DOWN && collectMarkersBackgroundFadeCounter.finished() && collectMarkersScreenFadeCounter.finished())
	{
		substate = GAMEPLAY;
		collectMarkersBackgroundFadeCounter.direction = STABLE;
		collectMarkersScreenFadeCounter.direction = STABLE;
		collectMarkersTimeoutCounter.direction = STABLE;
	}

	if(substate == GAMEPLAY && simulatePhysics)
	{
		intervalBeetweenExplosions.updateLinear(game->dt);		

		if(intervalBeetweenExplosions.finished())
		{
			for(std::map<int, bool>::iterator it = whichDynamitesExploded.begin(); it!=whichDynamitesExploded.end(); ++it)
			{
				if(!it->second)
				{
					explode(b2Vec2(static_cast<float32>(dynamiteMarkers[it->first]->center.x), static_cast<float32>(dynamiteMarkers[it->first]->center.y)), explodeRadius, explodeStrength);
					it->second = true;				

					explosionSound->play();

					break;
				}
			}

			intervalBeetweenExplosions.progress = intervalBeetweenExplosions.minValue;
		}

		dynamiteSpritesInterval.updateLinear(game->dt);
		if(dynamiteSpritesInterval.finished())
		{
			for(std::map<int, int>::iterator it = dynamiteAnimationIndexes.begin(); it!=dynamiteAnimationIndexes.end(); ++it)
			{
				if(whichDynamitesExploded[it->first] && it->second <= dynamiteSprite.size())
				{
					(it->second)++;
				}
			}

			dynamiteSpritesInterval.progress = dynamiteSpritesInterval.minValue;
		}


		// wykrywnaie koñca gry
		b2AABB aabb;
		bool areBricksAboveVictoryLevel = false;
		DetectVictory detectVic(&areBricksAboveVictoryLevel);

		aabb.lowerBound = b2Vec2(-200.0f, victoryLine.y/GameSetup::getSingleton().physicScale);
		aabb.upperBound = b2Vec2(200.0f, 200.0f);

		world->QueryAABB(&detectVic, aabb);	

		if(!areBricksAboveVictoryLevel)
		{
			if(!gameOver)
			{
				// zagraj dŸwiêki tylko raz
				applause->play();
				fanfare->play();
			}

			gameOver = true;			
		}

		if(gameOver)
		{
			fireworksLaunchInterval.updateLinear(game->dt);

			if(fireworksLaunchInterval.finished())
			{
				fireworksLaunchInterval.progress = fireworksLaunchInterval.minValue;

				switch(fireSystem->fastrandInt()%3)
				{
				case 0:
					fireworks1->spawn(Vector2(fireworks1->fastrandFloat(-1.0f, 1.0f)*SCREEN_WIDTH, fireworks1->fastrandFloat(-1.0f, 1.0f)*SCREEN_HEIGHT), fireworks1->defaultAmount);
					break;
				case 1:
					fireworks2->spawn(Vector2(fireworks2->fastrandFloat(-1.0f, 1.0f)*SCREEN_WIDTH, fireworks2->fastrandFloat(-1.0f, 1.0f)*SCREEN_HEIGHT), fireworks2->defaultAmount);
					break;
				case 2:
					fireworks3->spawn(Vector2(fireworks3->fastrandFloat(-1.0f, 1.0f)*SCREEN_WIDTH, fireworks3->fastrandFloat(-1.0f, 1.0f)*SCREEN_HEIGHT), fireworks3->defaultAmount);
					break;
				}

				
				fireworks->play();
			}
		}

	}
}

void GameplayState::proceedToGameplay()
{
	collectMarkersBackgroundFadeCounter.direction = DOWN;
	collectMarkersScreenFadeCounter.direction = DOWN;	
}

void GameplayState::Draw(GameEngine* game) 
{	
	backgroundsTex->bind();
	background.draw();

	white->bind();
	victoryLineGraphic.draw(victoryLine);

	bricksTex->bind();

	for(unsigned int i=0; i<bricks.size(); i++)
		bricks[i]->draw();	

	float frameProgress = (collectMarkersBackgroundFadeCounter.progress/collectMarkersBackgroundFadeCounter.maxValue);	
	char bufName[255];

	// kolorowanie tekstów zliczaj¹cych markery
	Color cursorTextColor = NotOKColor;
	Color collectedMarkersTextColor = NotOKColor;
	cursorTextColor.a = collectedMarkersTextColor.a = static_cast<u8>(NotOKColor.a * frameProgress);
	int foundNotCursorMarkers = 0;
	int slices;

	switch(substate)
	{
	case COLLECT_MARKERS:				
		// rysuj przycisk "KABOOM" ale zas³oniêty
		mainMenuTex->bind();
		launchButton.draw();
		white->bind();
		launchButtonText.draw(fontTygodnik);

		// wyciemnienie
		collectMarkersScreenFade.draw(Color(collectMarkersScreenFade.getColor().r,collectMarkersScreenFade.getColor().g,collectMarkersScreenFade.getColor().b,static_cast<u8>(collectMarkersScreenFade.getColor().a*(collectMarkersScreenFadeCounter.progress/collectMarkersScreenFadeCounter.maxValue))));	

		// jak przeŸroczyste powinny byc wszystkei teksty w ramce (wraz z ramk¹)
		collectMarkersBackground.draw(Color(collectMarkersBackground.getColor().r,collectMarkersBackground.getColor().g,collectMarkersBackground.getColor().b,static_cast<u8>(collectMarkersScreenFade.getColor().a*frameProgress)));

		// przeŸroczystoœæ nazwy levelu
		collectMarkersLevelName.setColor(collectMarkersLevelName.getOriginalColor().r,collectMarkersLevelName.getOriginalColor().g,collectMarkersLevelName.getOriginalColor().b,static_cast<u8>(collectMarkersLevelName.getOriginalColor().a*frameProgress));
		collectMarkersLevelName.draw(fontTygodnik);
		// przeŸroczystoœæ ilosci potrzebnych markerów
		collectMarkersNeededMarkersText.setColor(collectMarkersNeededMarkersText.getOriginalColor().r,collectMarkersNeededMarkersText.getOriginalColor().g,collectMarkersNeededMarkersText.getOriginalColor().b,static_cast<u8>((collectMarkersNeededMarkersText.getOriginalColor().a*frameProgress)));

		// "Ten poziom wymaga %d markerów."
		sprintf(bufName, collectMarkersNeededMarkersText.text, dynamitesNum);
		collectMarkersNeededMarkersText.drawCentered(collectMarkersNeededMarkersText.getPos(), collectMarkersNeededMarkersText.getScale(), collectMarkersNeededMarkersText.getAngle(), collectMarkersNeededMarkersText.getColor(), bufName, fontTygodnik);

		// "Kursor: BRAK"
		sprintf(bufName, collectMarkersCursorText.text, "Brak");

		for(unsigned int i=0; i<markers.size(); ++i)
		{
			if(markers[i].id==GameSetup::getSingleton().cursorMarkerID)
			{
				// znaleziono kursor!
				cursorTextColor = OKColor;
				cursorTextColor.a = static_cast<u8>(OKColor.a * frameProgress);
				sprintf(bufName, collectMarkersCursorText.text, "OK");
			}
			else
			{
				foundNotCursorMarkers++;
			}
		}

		if(foundNotCursorMarkers == dynamitesNum)
		{
			collectedMarkersTextColor = OKColor;
			collectedMarkersTextColor.a = static_cast<u8>(OKColor.a * frameProgress);
		}

		collectMarkersCursorText.drawCentered(collectMarkersCursorText.getPos(), collectMarkersCursorText.getScale(), collectMarkersCursorText.getAngle(), cursorTextColor, bufName, fontTygodnik);

		// Markery: %d
		sprintf(bufName, collectMarkersCollectedMarkers.text, foundNotCursorMarkers);
		collectMarkersCollectedMarkers.drawCentered(collectMarkersCollectedMarkers.getPos(), collectMarkersCollectedMarkers.getScale(), collectMarkersCollectedMarkers.getAngle(), collectedMarkersTextColor, bufName, fontTygodnik);


		mainMenuTex->bind();
		backButton.draw(selectedMenuItem==EXIT_BUTTON_COLLECTING_MARKERS?buttonSelectedColor:Color(255,255,255,255));


		slices = static_cast<int>(ceil(collectMarkersTimeoutCounter.progress/collectMarkersTimeoutCounter.maxValue/ (5.0f/360.0f)));
		for(int i=0; i<slices; ++i)
		{
			collectMarkersProgressRim.draw(collectMarkersProgressRim.getPos(), i*5.0f, Color(collectMarkersProgressRim.getColor().r,collectMarkersProgressRim.getColor().g,collectMarkersProgressRim.getColor().b,static_cast<u8>((collectMarkersProgressRim.getOriginalColor().a*frameProgress))));
		}

		white->bind();
		backButtonText.draw(fontTygodnik);	
		break;
	case GAMEPLAY:		
		mainMenuTex->bind();
		launchButton.draw(selectedMenuItem==LAUNCH_BUTTON?buttonSelectedColor:Color(255,255,255,255));
		backButton.draw(selectedMenuItem==EXIT_BUTTON_GAMEPLAY?buttonSelectedColor:Color(255,255,255,255));

		white->bind();
		backButtonText.draw(fontTygodnik);	
		if(!simulatePhysics)
		{
			launchButtonText.draw(fontTygodnik);	
		}
		else
		{
			ResetButtonText.draw(fontTygodnik);	
		}

		particlesTex->bind();
		fireSystem->updateAndDraw(game->dt);		
		fireworks1->updateAndDraw(game->dt);
		fireworks2->updateAndDraw(game->dt);
		fireworks3->updateAndDraw(game->dt);

		GameTex->bind();

		for(std::map<int, Marker*>::iterator it = dynamiteMarkers.begin(); it!=dynamiteMarkers.end(); ++it)
		{
			if(!whichDynamitesExploded[it->first] || (whichDynamitesExploded[it->first] && dynamiteAnimationIndexes[it->first] < dynamiteSprite.size()))
			{
				dynamiteSprite[dynamiteAnimationIndexes[it->first]]->draw(Vector2(static_cast<float>(it->second->center.x), static_cast<float>(it->second->center.y)), it->second->rotation);
			}
		}

		break;		
	}

	particlesTex->bind();
	cursorParticle->updateAndDraw(game->dt);

	if(gameOver)
	{
		white->bind();	
		victoryText.draw(fontTygodnik);	
	}

	mainMenuTex->bind();

	handSprite.draw(mouseCoords);

	// rysowanie obwiedni przedstawiaj¹cej progress klikania obiektów w menu
	slices = static_cast<int>(ceil(mouseHitCounter.progress/mouseHitCounter.maxValue/ (5.0f/360.0f)));
	for(int i=0; i<slices; ++i)
	{
		handRim.draw(mouseCoords, i*5.0f);
	}	
}