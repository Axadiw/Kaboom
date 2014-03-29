#pragma once

#include <Box2D/Box2D.h>
#include "Resources/Sprite.h"
#include "PolyMTLib.h"
#include "GameSetup.h"

#include <cmath>

/// Klasa opisuj¹ca pojedynczy klocek
class Brick
{
public:
	Brick(b2World* world_)
	{
		bodyDef = new b2BodyDef();
		dynamicBox = new b2PolygonShape();
		fixtureDef = new b2FixtureDef();		
		world = world_;
	}

	~Brick()
	{
		world->DestroyBody(body);
		delete dynamicBox;
		delete bodyDef;
		delete fixtureDef;		
	}

	void load(const char* configFileName, const char* objName);
	void save(const char* configFileName, const char* objName);

	void draw()
	{
		sprite.draw(Vector2(body->GetPosition().x*GameSetup::getSingleton().physicScale, body->GetPosition().y*GameSetup::getSingleton().physicScale), -body->GetAngle()*180.0f/M_PI);
	}

	inline Vector2 getPos(){return Vector2(body->GetPosition().x, body->GetPosition().y); }

	b2Body* body;
	b2BodyDef* bodyDef;
	b2PolygonShape* dynamicBox;
	b2FixtureDef* fixtureDef;
	b2World* world;

	Sprite sprite;
};