#include "Color.h"

void Color::load(const char* configFile, const char* name, Color default_value)
{
	int r_,g_,b_,a_;

	ConfigManager::getSingleton().getValueInt(&r_, configFile, name, "r", default_value.r);
	ConfigManager::getSingleton().getValueInt(&g_, configFile, name, "g", default_value.g);
	ConfigManager::getSingleton().getValueInt(&b_, configFile, name, "b", default_value.b);
	ConfigManager::getSingleton().getValueInt(&a_, configFile, name, "a", default_value.a);

	r = r_;
	g = g_;
	b = b_;
	a = a_;
}

void Color::load(const char* configFile, const char* name, const char* name2, Color default_value)
{
	int r_,g_,b_,a_;

	ConfigManager::getSingleton().getValueInt(&r_, configFile, name, name2, "r", default_value.r);
	ConfigManager::getSingleton().getValueInt(&g_, configFile, name, name2, "g", default_value.g);
	ConfigManager::getSingleton().getValueInt(&b_, configFile, name, name2, "b", default_value.b);
	ConfigManager::getSingleton().getValueInt(&a_, configFile, name, name2, "a", default_value.a);

	r = r_;
	g = g_;
	b = b_;
	a = a_;
}

void Color::save(const char* configFile, const char* name)
{
	ConfigManager::getSingleton().setValue(configFile, name, "r", r);
	ConfigManager::getSingleton().setValue(configFile, name, "g", g);
	ConfigManager::getSingleton().setValue(configFile, name, "b", b);
	ConfigManager::getSingleton().setValue(configFile, name, "a", a);

	ConfigManager::getSingleton().saveFile(configFile);
}

void Color::save(const char* configFile, const char* name, const char* name2)
{
	ConfigManager::getSingleton().setValue(configFile, name, name2, "r", r);
	ConfigManager::getSingleton().setValue(configFile, name, name2, "g", g);
	ConfigManager::getSingleton().setValue(configFile, name, name2, "b", b);
	ConfigManager::getSingleton().setValue(configFile, name, name2, "a", a);

	ConfigManager::getSingleton().saveFile(configFile);
}