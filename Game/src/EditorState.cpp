
#include <stdio.h>
#include <cmath>

#include "SDL.h"
#include "SDL_opengl.h"
#include "GameEngine.h"
#include "EditorState.h"
#include "GameState.h"
#include "GameSetup.h"
#include "MenuState.h"
#include "Resources\ConfigManager.h"
#include "includes.h"

#define DESTROY_DISTANCE 10.0f

static const char* EditorConfFile = "Data\\Editor.txt";
static const char* GeneralConfFile = "Data\\generalConfig.txt";
static const char* editedLevelFile = "Data\\levels\\editedLevel.txt";

/// wykonuje odrzut spowownaany eksplozj¹ na pojedynczym klocku
class ExplodeElementEditor : public b2QueryCallback
{
public:
	b2World* world;
	float strength;
	b2Vec2 position;

	ExplodeElementEditor(b2World* world_, b2Vec2 position_, float strength_):world(world_), position(position_), strength(strength_){};

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

		if(distance < EditorState::getSingleton().destroyRadius)
		{			
			for(unsigned int i=0; i<EditorState::getSingleton().bricks.size(); i++)
				{					
					if(EditorState::getSingleton().bricks[i]->body == body)
					{
						delete EditorState::getSingleton().bricks[i];
						EditorState::getSingleton().bricks.erase(EditorState::getSingleton().bricks.begin() + i);

						break;
					}
				}


		}

		return true;
	}
};

void EditorState::save()
{
	std::remove(editedLevelFile);

	ConfigManager::getSingleton().loadFile(editedLevelFile);	

	char bufName[255];

	for(unsigned int i=0; i<bricks.size(); i++)
	{
		sprintf(bufName, "brick_%d", i);

		bricks[i]->save(editedLevelFile, bufName);
	}

	ConfigManager::getSingleton().setValue(editedLevelFile, "bricksNum", bricks.size());	
	ConfigManager::getSingleton().setValue(editedLevelFile, "dynamitesNum", dynamitesNum);	
	ConfigManager::getSingleton().setValue(editedLevelFile, "levelName", levelName);	
	ConfigManager::getSingleton().setValue(editedLevelFile, "secretVariable", SECRET_VARIABLE);	

	background.save(editedLevelFile, "background");
	victoryLine.save(editedLevelFile, "victoryLine");

	ConfigManager::getSingleton().saveFile(editedLevelFile);

	ConfigManager::getSingleton().releaseConfigFile(editedLevelFile);
}

void EditorState::clear()
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

void EditorState::load()
{
	// wyczysc liste klockow
	clear();

	ConfigManager::getSingleton().allowSavingToFile = false;
	ConfigManager::getSingleton().loadFile(editedLevelFile);

	ConfigManager::getSingleton().getValuePChar(levelName, editedLevelFile, "levelName", "Nowy poziom");	

	int bricksNum;
	char bufName[255];
	ConfigManager::getSingleton().getValueInt(&bricksNum, editedLevelFile, "bricksNum");	

	ConfigManager::getSingleton().getValueInt(&dynamitesNum, editedLevelFile, "dynamitesNum", 0);	

	background.load(editedLevelFile, "background");

	victoryLine.load(editedLevelFile, "victoryLine", Vector2(0.0f, -50.0f));

	Brick* tmpBrick;

	for(int i=0; i< bricksNum; ++i)
	{
		sprintf(bufName, "brick_%d", i);

		tmpBrick = new Brick(world);
		tmpBrick->load(editedLevelFile, bufName);
		tmpBrick->body->SetTransform(b2Vec2(tmpBrick->sprite.getPos().x, tmpBrick->sprite.getPos().y), tmpBrick->sprite.getAngle());

		bricks.push_back(tmpBrick);
	}

	ConfigManager::getSingleton().allowSavingToFile = true;
	ConfigManager::getSingleton().saveFile(editedLevelFile);
	ConfigManager::getSingleton().releaseConfigFile(editedLevelFile);
}

void EditorState::Init()
{
	white = TextureManager::getSingleton().loadTexture("Data\\white.bmp");	
	bricksTex = TextureManager::getSingleton().loadTexture("Data\\bricks.png");	
	backgroundsTex = TextureManager::getSingleton().loadTexture("Data\\backgrounds.png");	
	fontTygodnik = FontManager::getSingleton().loadFont("Data\\Tygodnik.ttf");	

	ConfigManager::getSingleton().loadFile(GeneralConfFile);	

	Vector2 tmpGravity;
	tmpGravity.load(GeneralConfFile, "gravity");
	gravity = new b2Vec2(tmpGravity.x, tmpGravity.y);	
	ConfigManager::getSingleton().getValueFloat(&destroyRadius, GeneralConfFile, "destroy_radius");
	ConfigManager::getSingleton().getValueFloat(&explodeRadius, GeneralConfFile, "explode_radius");
	ConfigManager::getSingleton().getValueFloat(&explodeStrength, GeneralConfFile, "explode_strength");
	physicsQuality.load(GeneralConfFile, "physics_quality");

	ConfigManager::getSingleton().releaseConfigFile(GeneralConfFile);

	ConfigManager::getSingleton().loadFile(EditorConfFile);	

	ConfigManager::getSingleton().getValueFloat(&brickSelectDistance, EditorConfFile, "brick_select_distance");
	physicsOnText.load(EditorConfFile, "PhysicOnText");
	ConfigManager::getSingleton().getValueInt(&backgroundsNum, EditorConfFile, "background_num");
	
	ConfigManager::getSingleton().getValueInt(&helpLinesNum, EditorConfFile, "help_text_lines");
	helpLines = new Text[helpLinesNum];
	showHelpText.load(EditorConfFile, "show_help");
	statusText.load(EditorConfFile, "status_text");
	
	dynamitesNumText.load(EditorConfFile, "dynamites_num_text");
	statusTextCounter.load(EditorConfFile, "status_text_counter");

	// teksty w HELPie
	char bufName[255];
	for(int i=0; i<helpLinesNum; ++i)
	{
		sprintf(bufName, "help_text_line_%d", i);
		helpLines[i].load(EditorConfFile, bufName);
	}
	
	victoryLineGraphic.load(EditorConfFile, "victoryLine");

	ConfigManager::getSingleton().getValueFloat(&rotateStep, EditorConfFile, "rotate_step");

	ConfigManager::getSingleton().releaseConfigFile(EditorConfFile);

	SDL_ShowCursor(SDL_ENABLE);
	showHelp = false;
	draggedBrickID = 0;
	dragBrick = false;
	simulatePhysics = false;	
	
	actualBackgroundChangeIndex = 0;

	// ustawienia BOX2D	
	world = new b2World(*gravity);
	
	float physicScale = GameSetup::getSingleton().physicScale;

	// ustaw pod³oge
	groundBodyDef = new b2BodyDef();
	groundBox = new b2PolygonShape();
	groundBodyDef->position.Set(0.0f, (-SCREEN_HEIGHT + GameSetup::getSingleton().groundLevel)/physicScale);
	groundBody = world->CreateBody(groundBodyDef);	
	groundBox->SetAsBox(2*SCREEN_WIDTH/physicScale, 0.1f);	
	groundBody->CreateFixture(groundBox, 0.0f);		

	load();
}

void EditorState::explode(b2Vec2 position, float radius, float strength)
{
	b2AABB aabb;
	ExplodeElementEditor explodeElement(world, position, strength);

	aabb.lowerBound = position - b2Vec2(radius, radius);
	aabb.upperBound = position + b2Vec2(radius, radius);

	world->QueryAABB(&explodeElement, aabb);	
}

void EditorState::Cleanup()
{
	// usun klocki
	clear();

	delete groundBodyDef;
	delete groundBox;
	delete world;
	delete gravity;
	delete[] helpLines;

	FontManager::getSingleton().releaseFont(fontTygodnik);
	TextureManager::getSingleton().releaseTexture(white);
	TextureManager::getSingleton().releaseTexture(backgroundsTex);
	TextureManager::getSingleton().releaseTexture(bricksTex);

	SDL_ShowCursor(SDL_DISABLE);
}

void EditorState::Pause()
{

}

void EditorState::Resume()
{

}

void EditorState::setStatus(char* text)
{
	statusTextCounter.progress = statusTextCounter.minValue;

	statusText.setText(text);
}

void EditorState::addBrick(int id)
{
	Brick* tmpBrick;

	char buffer[255];

	sprintf(buffer, "Dodano klocek %d", id);
	setStatus(buffer);

	ConfigManager::getSingleton().loadFile(EditorConfFile);	

	tmpBrick = new Brick(world);
	sprintf(buffer, "brick%d", id);
	tmpBrick->load(EditorConfFile, buffer);
	tmpBrick->body->SetTransform(b2Vec2(mouseCoords.x, mouseCoords.y), 0.0f);

	bricks.push_back(tmpBrick);

	ConfigManager::getSingleton().releaseConfigFile(EditorConfFile);
}

void EditorState::HandleEvents(GameEngine* game)
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
			case SDLK_UP:
				victoryLine.y += 1.0f;
				break;
			case SDLK_DOWN:
				victoryLine.y -= 1.0f;
				break;
			case SDLK_b:
				setStatus("Zmieniono tlo");
				if(backgroundsNum > 0)
				{
					ConfigManager::getSingleton().loadFile(EditorConfFile);	

					actualBackgroundChangeIndex = (actualBackgroundChangeIndex+1)%backgroundsNum;
					char bufName[255];
					sprintf(bufName, "background_%d", actualBackgroundChangeIndex);

					background.load(EditorConfFile, bufName);

					ConfigManager::getSingleton().releaseConfigFile(EditorConfFile);
				}
				break;
			case SDLK_l:
				load();
				setStatus("Wczytano z pliku");
				break;
			case SDLK_s:
				setStatus("Zapisano do pliku");
				save();
				break;
			case SDLK_c:
				setStatus("Usunieto wszystkie klocki");
				clear();
				break;
			case SDLK_F1:
				showHelp = !showHelp;
				break;
			case SDLK_KP_PLUS:
				setStatus("Zwiekszono ilosc dynamitu");
				dynamitesNum++;
				break;
			case SDLK_KP_MINUS:
				if(dynamitesNum>0)
				{
					dynamitesNum--;
					setStatus("Zmniejszono ilosc dynamitu");
				}				
				break;
			case SDLK_e:
				setStatus("Wybuch!");
				explode(b2Vec2(mouseCoords.x, mouseCoords.y), explodeRadius, explodeStrength);
				break;
			case SDLK_x:
				setStatus("Usunieto klocek");
				for(unsigned int i=0; i<bricks.size(); i++)
				{					
					if(mouseCoords.distance(bricks[i]->getPos()) < brickSelectDistance)
					{
						delete bricks[i];
						bricks.erase(bricks.begin() + i);

						break;
					}
				}
				break;
			case SDLK_SPACE:
				simulatePhysics = !simulatePhysics;
				break;	
			case SDLK_1:
				addBrick(1);
				break;
			case SDLK_2:
				addBrick(2);
				break;
			case SDLK_3:
				addBrick(3);
				break;
			case SDLK_4:
				addBrick(4);
				break;
			case SDLK_5:
				addBrick(5);
				break;
			case SDLK_6:
				addBrick(6);
				break;
			case SDLK_7:
				addBrick(7);
				break;
			case SDLK_8:
				addBrick(8);
				break;
			case SDLK_9:
				addBrick(9);
				break;
			case SDLK_0:
				addBrick(0);
				break;
			}
			break;
		case SDL_MOUSEMOTION :

			// przeliczenie wspó³rzêdnych kursora myszy na wspó³rzêdne ekranu
			mouseCoords = Vector2(event.motion.x, event.motion.y);
			mouseCoords.x = (mouseCoords.x/game->screenDimensions.x)*2.0f*SCREEN_WIDTH - SCREEN_WIDTH;
			mouseCoords.y = -(mouseCoords.y/game->screenDimensions.y)*2.0f*SCREEN_HEIGHT + SCREEN_HEIGHT;

			mouseCoords *= 1.0f/GameSetup::getSingleton().physicScale;

			if(dragBrick)
			{
				bricks[draggedBrickID]->body->SetTransform(b2Vec2(mouseCoords.x, mouseCoords.y), bricks[draggedBrickID]->body->GetAngle());
			}

			break;
		case SDL_MOUSEBUTTONDOWN:

			if(event.button.button == 4 || event.button.button == 5)
			{
				for(unsigned int i=0; i<bricks.size(); i++)
				{					
					if(mouseCoords.distance(bricks[i]->getPos()) < brickSelectDistance)
					{
						bricks[i]->body->SetTransform(bricks[i]->body->GetPosition(), bricks[i]->body->GetAngle() + (event.button.button == 4?rotateStep:-rotateStep));

						break;
					}
				}
			}

			for(unsigned int i=0; i<bricks.size(); i++)
			{					
				if(mouseCoords.distance(bricks[i]->getPos()) < brickSelectDistance)
				{
					draggedBrickID = i;
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
}

void EditorState::Update(GameEngine* game) 
{
	if(simulatePhysics)
	{
		world->Step(game->dt, static_cast<int32>(physicsQuality.x), static_cast<int32>(physicsQuality.y));	
	}

	statusTextCounter.updateLinear(game->dt);
}

void EditorState::Draw(GameEngine* game) 
{	
	// t³o
	backgroundsTex->bind();
	background.draw();

	// linia ponizej ktorej maja sie znalezc klocki
	white->bind();
	victoryLineGraphic.draw(victoryLine);

	// klocki
	bricksTex->bind();

	for(unsigned int i=0; i<bricks.size(); i++)
		bricks[i]->draw();	

	// teksty
	white->bind();
	showHelpText.drawLB(fontTygodnik);

	if(!statusTextCounter.finished())
	{
		statusText.drawLB(fontTygodnik);
	}	

	char bufname[255];
	sprintf(bufname, "%s%d", dynamitesNumText.text, dynamitesNum);

	dynamitesNumText.drawCentered(dynamitesNumText.getPos(), dynamitesNumText.getScale(), dynamitesNumText.getAngle(), dynamitesNumText.getColor(), bufname, fontTygodnik);

	if(showHelp)
	{
		for(int i=0; i<helpLinesNum; ++i)
		{
			helpLines[i].draw(fontTygodnik);
		}
	}

	if(simulatePhysics)
	{
		physicsOnText.draw(fontTygodnik);
	}
}