#pragma once

#include "SDL.h"
#include "Singleton.h"
#include "Math\Vector2.h"
#include "includes.h"

#include <vector>
using namespace std;

enum QUALITY_LEVEL {LOW, MEDIUM, HIGH};

enum MAINMENU_SUBSTATE{INITIAL_SCREEN, MAIN_MENU, OPTIONS, EDITOR, LEVEL_SELECT};

/*! 
 *  \brief     Opcje gry, dostêpne w obrêbie ca³ej gry
 *  \author    Micha³ Mizera
 *  \version   1.0
 *  \date      2011-2012
 *  \copyright GNU Public License.
 */
class GameSetup: public Singleton<GameSetup>
{
public:
	Vector2 cameraResolution;
	float cameraFramerate;
	int screenFramerate;

	int screenQuality;
	int cameraQuality;

	int cameraID;
	int cursorMarkerID; 

	float physicScale;

	char levelName[255];

	MAINMENU_SUBSTATE actualMainMenuSubstate;

	// wartosci domyslne
	Vector2 lowScreenResolution;
	Vector2 mediumScreenResolution;
	Vector2 highScreenResolution;
	Vector2 lowCameraResolution;
	float lowCameraFramerate;
	Vector2 mediumCameraResolution;
	float mediumCameraFramerate;
	Vector2 highCameraResolution;
	float highCameraFramerate;

	float groundLevel;

	void init();

	void save();
};