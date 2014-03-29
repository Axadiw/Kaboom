#include "Vector2.h"
#include "../Resources/ConfigManager.h"
#include <cmath>

void Vector2::normalize() 
{
	float len = x*x + y*y;
	if(len == 0.0f || len == 1.0f)
	{
		/// wektor pusty, lub o d³ugœci 1, nie ma sensu normalizowaæ
		return;
	}

	/// jak juz wiemy, ze jest sens normalizowac, mozemy policzyc pierwiastek i przez niego podzielic
	len = sqrt(len);
	x /= len;
	y /= len;
}

void Vector2::load(const char* configFile, const char* name, Vector2 default_value)
{
	ConfigManager::getSingleton().getValueFloat(&x, configFile, name, "x", default_value.x);
	ConfigManager::getSingleton().getValueFloat(&y, configFile, name, "y",  default_value.y);
}

void Vector2::load(const char* configFile, const char* name, const char* name2,  Vector2 default_value)
{
	ConfigManager::getSingleton().getValueFloat(&x, configFile, name, name2, "x",  default_value.x);
	ConfigManager::getSingleton().getValueFloat(&y, configFile, name, name2, "y",  default_value.y);
}

void Vector2::save(const char* configFile, const char* name)
{
	ConfigManager::getSingleton().setValue(configFile, name, "x", x);
	ConfigManager::getSingleton().setValue(configFile, name, "y", y);

	ConfigManager::getSingleton().saveFile(configFile);
}

void Vector2::save(const char* configFile, const char* name, const char* name2)
{
	ConfigManager::getSingleton().setValue(configFile, name, name2, "x", x);
	ConfigManager::getSingleton().setValue(configFile, name, name2, "y", y);

	ConfigManager::getSingleton().saveFile(configFile);
}

void Vector2::rotate(float angle)
{
	float sinA = sin(angle);
	float cosA = cos(angle);

	float temp = x*cosA - y*sinA;
	y = x*sinA + y*cosA;
	x = temp;
}