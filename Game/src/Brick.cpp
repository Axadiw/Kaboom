#include "Brick.h"

#include <cmath>

#define DEFAULT_DENSITY 1.0f
#define DEFAULT_FRICTION 0.3f

void Brick::load(const char* configFileName, const char* objName)
{
	sprite.load(configFileName, objName);	

	float physicScale = GameSetup::getSingleton().physicScale;

	bodyDef->type = b2_dynamicBody;
	bodyDef->position.Set(sprite.getPos().x/physicScale, sprite.getPos().y/physicScale);
	body = world->CreateBody(bodyDef);		
	dynamicBox->SetAsBox((sprite.getScale()*sprite.getSize().x/2.0f)/physicScale, (sprite.getScale()*sprite.getSize().y/2.0f)/physicScale);	
	fixtureDef->shape = dynamicBox;	

	body->SetSleepingAllowed(false);

	float tmp;
	ConfigManager::getSingleton().getValueFloat(&tmp, configFileName, objName, "density", DEFAULT_DENSITY);
	fixtureDef->density = tmp;	

	ConfigManager::getSingleton().getValueFloat(&tmp, configFileName, objName, "friction", DEFAULT_FRICTION);
	fixtureDef->friction = tmp;
	body->CreateFixture(fixtureDef);	
}

void Brick::save(const char* configFileName, const char* objName)
{
	sprite.setAngle(body->GetAngle());
	sprite.setPos(Vector2(body->GetPosition().x, body->GetPosition().y));

	sprite.save(configFileName, objName);

	ConfigManager::getSingleton().setValue(configFileName, objName, "density", fixtureDef->density);
	ConfigManager::getSingleton().setValue(configFileName, objName, "friction", fixtureDef->friction);

	ConfigManager::getSingleton().saveFile(configFileName);
}