#pragma once

#include "../Math/Vector2.h"

/*! 
 *  \brief     Pojedyncza czastka
 *  \author    Micha� Mizera
 *  \version   1.0
 *  \date      2011-2012
 *  \copyright GNU Public License.
 */

class Particle
{
public:
	Particle(Vector2 position_, Vector2 velocity_, char color_): position(position_),velocity(velocity_), color(color_), age(0), isActive(true){};
	
	/// czy cz�sta jest aktywna
	bool isActive;

	/// kolor czastki (indeks w tablicy kolor�w)
	char color;

	/// wiek czastki
	float age;

	/// pozycja
	Vector2 position;

	/// przy�pieszenie
	Vector2 velocity;
};