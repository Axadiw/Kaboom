#include "Text.h"

#include "ConfigManager.h"

#define TEXT_DEFAULT_POS Vector2(0.0f, 0.0f)
#define TEXT_DEFAULT_COLOR Color(255, 255, 255, 255)
#define TEXT_DEFAULT_ANGLE 0.0f
#define TEXT_DEFAULT_SCALE 1.0f
#define TEXT_DEFAULT_IS_ACTIVE true
#define TEXT_DEFAULT_TEXT ""

void Text::drawCentered(Vector2 pos, float scale, float angle, Color c, const char* text, Font* font_)
{
	glPushMatrix();
		glColor4ub(c.r, c.g, c.b, c.a);
		glRotatef(angle, 0.0f, 0.0f, 1.0f);
		
		FTBBox tmp = font_->font->BBox(text);

		glTranslatef(pos.x - scale*(tmp.Upper().Xf() - tmp.Lower().Xf())/2.0f,  pos.y - scale*(tmp.Upper().Yf() - tmp.Lower().Yf())/2.0f, 0.0f);
		glScalef(scale,scale, 1.0f);		
		font_->font->Render(text);
	glPopMatrix();
}

void Text::drawLeftBottom(Vector2 pos, float scale, float angle, Color c, const char* text, Font* font_)
{
	glPushMatrix();
		glColor4ub(c.r, c.g, c.b, c.a);
		glRotatef(angle, 0.0f, 0.0f, 1.0f);
		
		glTranslatef(pos.x, pos.y, 0.0f);
		glScalef(scale,scale, 1.0f);		
		font_->font->Render(text);
	glPopMatrix();
}

void Text::load(const char* configFileName, const char* objName)
{	
	ConfigManager::getSingleton().getValueBool(&isActive, configFileName, objName, "is_active", TEXT_DEFAULT_IS_ACTIVE);

	pos.load(configFileName, objName, "position", TEXT_DEFAULT_POS);		
	color.load(configFileName, objName, "color", TEXT_DEFAULT_COLOR);
	ConfigManager::getSingleton().getValueFloat(&angle, configFileName, objName, "angle", TEXT_DEFAULT_ANGLE);
	ConfigManager::getSingleton().getValueFloat(&scale, configFileName, objName, "scale", TEXT_DEFAULT_SCALE);
	ConfigManager::getSingleton().getValuePChar(text, configFileName, objName, "text", TEXT_DEFAULT_TEXT);

	originalPos = pos;
	originalScale = scale;
	originalAngle = angle;
	originalSize = size;
	originalColor = color;
}