#include "ProgressCounter.h"
#include "../Resources/ConfigManager.h"

#define PROGRESS_COUNTER_DEFAULT_MAXVAL 1.0f
#define PROGRESS_COUNTER_DEFAULT_MINVAL 0.0f
#define PROGRESS_COUNTER_DEFAULT_SPEED 1.0f
#define PROGRESS_COUNTER_DEFAULT_INIT 0.0f
#define PROGRESS_COUNTER_DEFAULT_DIRECTION STABLE

void ProgressCounter::load(const char* configFile, const char* name)
{
	ConfigManager::getSingleton().getValueFloat(&maxValue, configFile, name, "maxValue", PROGRESS_COUNTER_DEFAULT_MAXVAL);
	ConfigManager::getSingleton().getValueFloat(&minValue, configFile, name, "minValue", PROGRESS_COUNTER_DEFAULT_MINVAL);
	ConfigManager::getSingleton().getValueFloat(&speed, configFile, name, "speed", PROGRESS_COUNTER_DEFAULT_SPEED);
	ConfigManager::getSingleton().getValueFloat(&progress, configFile, name, "init", PROGRESS_COUNTER_DEFAULT_INIT);
	ConfigManager::getSingleton().getValueInt(&direction, configFile, name, "direction", PROGRESS_COUNTER_DEFAULT_DIRECTION);
}
void ProgressCounter::load(const char* configFile, const char* name, const char* name2)
{
	ConfigManager::getSingleton().getValueFloat(&maxValue, configFile, name, name2, "maxValue", PROGRESS_COUNTER_DEFAULT_MAXVAL);
	ConfigManager::getSingleton().getValueFloat(&minValue, configFile, name, name2, "minValue", PROGRESS_COUNTER_DEFAULT_MINVAL);
	ConfigManager::getSingleton().getValueFloat(&speed, configFile, name, name2, "speed", PROGRESS_COUNTER_DEFAULT_SPEED);
	ConfigManager::getSingleton().getValueFloat(&progress, configFile, name, name2, "init", PROGRESS_COUNTER_DEFAULT_INIT);
	ConfigManager::getSingleton().getValueInt(&direction, configFile, name, name2, "direction", PROGRESS_COUNTER_DEFAULT_DIRECTION);
}

void ProgressCounter::updateLinear(float change)
{
	if(direction == UP && progress <= maxValue)
	{
		progress += speed*change;
		progress = (progress > maxValue)?maxValue:progress;
	} else if(direction == DOWN && progress >= minValue)
	{
		progress -= speed*change;
		progress = (progress < minValue)?minValue:progress;
	}
}

bool ProgressCounter::finished()
{
	switch(direction)
	{
	case STABLE:
		return true;
	case UP:
		return progress == maxValue;
		break;
	case DOWN:
		return progress == minValue;
		break;
	default:
		return false;
	}
}