#pragma once

#include "SDL.h"
#include "Singleton.h"
#include "Math\Vector2.h"
#include "includes.h"

#include <vector>
using namespace std;

enum QUALITY_LEVEL {LOW, MEDIUM, HIGH};

/*! 
 *  \brief     Opcje gry, dost�pne w obr�bie ca�ej gry
 *  \author    Micha� Mizera
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

	float ballSpeedupFactor;

	int levelNumber;

	int cameraID;
	int cursorMarkerID;

	int paddle1MarkerID;
	int paddle2MarkerID;

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

	void init();

	void save();
};