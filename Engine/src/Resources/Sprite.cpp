#include "Sprite.h"
#include "ConfigManager.h"
#include "../GameEngine.h"
#include "../includes.h"

#pragma comment (lib, "SDL.lib")
#pragma comment (lib, "SDLmain.lib")
#pragma comment (lib, "opengl32.lib")

#define SPRITE_DEFAULT_SCALE 1.0f
#define SPRITE_DEFAULT_ANGLE 0.0f
#define SPRITE_DEFAULT_POS Vector2(0.0f, 0.0f)
#define SPRITE_DEFAULT_COLOR Color(255, 255, 255, 255)
#define SPRITE_DEFAULT_FLIPX false
#define SPRITE_DEFAULT_FLIPY false
#define SPRITE_DEFAULT_IS_ACTIVE true
#define SPRITE_DEFAULT_TEXCORNER Vector2(-1.0f, -1.0f)
#define SPRITE_DEFAULT_TEXSIZE Vector2(-1.0f, -1.0f)
#define SPRITE_DEFAULT_TEXDIMENSIONS Vector2(512.0f, 512.0f)

void Sprite::draw(Vector2 pos, Vector2 halfSize_, float minU, float maxU, float minV, float maxV, float angle, Color c)
{
	glPushMatrix();
		glTranslatef(pos.x, pos.y, 0.0f);
		glRotatef(-angle, 0.0f, 0.0f, 1.0f);		
		glColor4ub(c.r, c.g, c.b, c.a);
		glBegin( GL_QUADS );
			glTexCoord2d(minU, maxV);	glVertex2d(- halfSize_.x,- halfSize_.y);
			glTexCoord2d(maxU, maxV); 	glVertex2d(halfSize_.x,- halfSize_.y);
			glTexCoord2d(maxU, minV); 	glVertex2d(halfSize_.x,halfSize_.y);
			glTexCoord2d(minU, minV); 	glVertex2d(- halfSize_.x,halfSize_.y);
		glEnd();
	glPopMatrix();
}

void Sprite::setSize(Vector2 size_)
{
	size = size_;
	halfSize = size_ * 0.5f * scale;
}

void Sprite::setScale(float scale_)
{
	scale = scale_;
	halfSize = size * 0.5f * scale;
}

void Sprite::load(const char* configFileName, const char* objName)
{
	ConfigManager::getSingleton().getValueBool(&isActive, configFileName, objName, "is_active", SPRITE_DEFAULT_IS_ACTIVE);

	pos.load(configFileName, objName, "position", SPRITE_DEFAULT_POS);	

	color.load(configFileName, objName, "color", SPRITE_DEFAULT_COLOR);
	ConfigManager::getSingleton().getValueFloat(&angle, configFileName, objName, "angle", SPRITE_DEFAULT_ANGLE);
	ConfigManager::getSingleton().getValueFloat(&scale, configFileName, objName, "scale", SPRITE_DEFAULT_SCALE);

	
	ConfigManager::getSingleton().getValueBool(&flipX, configFileName, objName, "flipX", SPRITE_DEFAULT_FLIPX);
	ConfigManager::getSingleton().getValueBool(&flipY, configFileName, objName, "flipY", SPRITE_DEFAULT_FLIPY);

	textureCorner.load(configFileName, objName, "texCorner", SPRITE_DEFAULT_TEXCORNER);
	textureSize.load(configFileName, objName, "texSize", SPRITE_DEFAULT_TEXSIZE);
	textureDimensions.load(configFileName, objName, "texDimensions", SPRITE_DEFAULT_TEXDIMENSIONS);

	Vector2 tmp = Vector2(((textureSize.x)/GameEngine::getSingleton().screenDimensions.x)*SCREEN_WIDTH,((textureSize.y)/GameEngine::getSingleton().screenDimensions.y)*SCREEN_HEIGHT);

	float tmp2 = GameEngine::getSingleton().screenDimensions.x;

	size.load(configFileName, objName, "size", tmp);	
	halfSize = size * 0.5f * scale;

	minU = textureCorner.x / textureDimensions.x;
	maxU = (textureCorner.x + textureSize.x )/ textureDimensions.x;
	minV = textureCorner.y / textureDimensions.y;
	maxV = (textureCorner.y + textureSize.y )/ textureDimensions.y;

	float swapTmp;
	if(flipX)
	{
		swapTmp = minU;
		minU = maxU;
		maxU = swapTmp;
	}
	if(flipY)
	{
		swapTmp = minV;
		minV = maxV;
		maxV = swapTmp;
	}

	originalPos = pos;
	originalScale = scale;
	originalAngle = angle;
	originalSize = size;
	originalColor = color;
}

void Sprite::save(const char* configFileName, const char* objName)
{
	pos.save(configFileName, objName, "position");
	color.save(configFileName, objName, "color");
	ConfigManager::getSingleton().setValue(configFileName, objName, "angle", angle);
	ConfigManager::getSingleton().setValue(configFileName, objName, "scale", scale);

	ConfigManager::getSingleton().setValue(configFileName, objName, "flipX", flipX);
	ConfigManager::getSingleton().setValue(configFileName, objName, "flipY", flipY);

	ConfigManager::getSingleton().setValue(configFileName, objName, "is_active", isActive);
	
	textureCorner.save(configFileName, objName, "texCorner");
	textureSize.save(configFileName, objName, "texSize");
	textureDimensions.save(configFileName, objName, "texDimensions");
	
	size.save(configFileName, objName, "size");	

	ConfigManager::getSingleton().saveFile(configFileName);
}