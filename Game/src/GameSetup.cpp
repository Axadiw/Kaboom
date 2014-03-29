#include "GameSetup.h"
#include "Resources\ConfigManager.h"
#include "GameEngine.h"

static const char* generalConfFile = "Data\\generalConfig.txt";
static const char* saveFile = "Data\\saveFile.txt";

void GameSetup::init()
{
	ConfigManager::getSingleton().loadFile(generalConfFile);
	ConfigManager::getSingleton().loadFile(saveFile);


	actualMainMenuSubstate = INITIAL_SCREEN;

	ConfigManager::getSingleton().getValueInt(&cameraID, generalConfFile, "cameraID");
	ConfigManager::getSingleton().getValueInt(&cursorMarkerID, generalConfFile, "cursorMarkerID");
	
	ConfigManager::getSingleton().getValueFloat(&physicScale, generalConfFile, "physicScale");
	
	ConfigManager::getSingleton().getValueFloat(&groundLevel, generalConfFile, "ground_level");

	// domyœlne wartoœæi save
	int defaultScreenResolution, defaultCameraQuality;
	ConfigManager::getSingleton().getValueInt(&defaultScreenResolution, generalConfFile, "defaultScreenResolution");
	ConfigManager::getSingleton().getValueInt(&defaultCameraQuality, generalConfFile, "defaultCameraQuality");

	bool defaultFullscreen;
	ConfigManager::getSingleton().getValueBool(&defaultFullscreen, generalConfFile, "defaultFullscreen");

	lowScreenResolution.load(generalConfFile, "lowScreenResolution");
	mediumScreenResolution.load(generalConfFile, "mediumScreenResolution");
	highScreenResolution.load(generalConfFile, "highScreenResolution");
	lowCameraResolution.load(generalConfFile, "lowCameraResolution");
	ConfigManager::getSingleton().getValueFloat(&lowCameraFramerate, generalConfFile, "lowCameraFramerate");
	mediumCameraResolution.load(generalConfFile, "mediumCameraResolution");
	ConfigManager::getSingleton().getValueFloat(&mediumCameraFramerate, generalConfFile, "mediumCameraFramerate");
	highCameraResolution.load(generalConfFile, "highCameraResolution");
	ConfigManager::getSingleton().getValueFloat(&highCameraFramerate, generalConfFile, "highCameraFramerate");
	ConfigManager::getSingleton().getValueInt(&screenFramerate, generalConfFile, "screenFramerate");

	// wczytywanie save
	ConfigManager::getSingleton().getValueInt(&screenQuality, saveFile, "screenQuality", defaultScreenResolution);
	ConfigManager::getSingleton().getValueInt(&cameraQuality, saveFile, "cameraQuality", defaultCameraQuality);
	ConfigManager::getSingleton().getValueBool(&(GameEngine::getSingleton().fullscreen), saveFile, "fullscreen", defaultFullscreen);

	

	switch(cameraQuality)
	{
	case LOW:
		cameraResolution = lowCameraResolution;
		cameraFramerate = lowCameraFramerate;
		break;
	case MEDIUM:
		cameraResolution = mediumCameraResolution;
		cameraFramerate = mediumCameraFramerate;
		break;
	case HIGH:
		cameraResolution = highCameraResolution;
		cameraFramerate = highCameraFramerate;
		break;
	}

	switch(screenQuality)
	{
	case LOW:
		GameEngine::getSingleton().screenDimensions = lowScreenResolution;
		break;
	case MEDIUM:
		GameEngine::getSingleton().screenDimensions = mediumScreenResolution;
		break;
	case HIGH:
		GameEngine::getSingleton().screenDimensions = highScreenResolution;
		break;
	}

	ConfigManager::getSingleton().releaseConfigFile(generalConfFile);
	ConfigManager::getSingleton().releaseConfigFile(saveFile);
}

void GameSetup::save()
{
	ConfigManager::getSingleton().createConfigFile(saveFile);

	ConfigManager::getSingleton().setValue(saveFile, "screenQuality", screenQuality);
	ConfigManager::getSingleton().setValue(saveFile, "cameraQuality", cameraQuality);
	ConfigManager::getSingleton().setValue(saveFile, "fullscreen", GameEngine::getSingleton().fullscreen);

	ConfigManager::getSingleton().saveFile(saveFile);
	ConfigManager::getSingleton().releaseConfigFile(saveFile);
}