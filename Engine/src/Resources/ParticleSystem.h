#pragma once

#include "GraphicObject.h"
#include "Particle.h"
#include "Sprite.h"

#include <time.h>
#include <vector>

/*! 
 *  \brief     System efekt�w cz�stek
 *  \author    Micha� Mizera
 *  \version   1.0
 *  \date      2011-2012
 *  \copyright GNU Public License.
 */

class ParticleSystem
{
public:
	/// standardowy konstruktor
	ParticleSystem():randSeed(static_cast<int>(time(NULL))){};

	/// destruktor
	~ParticleSystem();

	/// Nazwa pliku przechowuj�cego dane opisuj�ce systme cz�stek
	const char* name;

	/*!
	wczytaj system z pliku
	@param configFile �cie�ka do pliku konfiguracyjnego
	*/
	void load(const char* configFile);

	/*!
	dodaje odpowiedni� ilo�� cz�stek do systemu
	@param position pozycja emitera cz�stek
	@param amount ilo�� cz�stek
	*/
	void spawn(Vector2 position, int amount);

	/*!
	uaktualnia pozycj� wszystkich cz�tek, oraz rysuje je na ekranie
	*/
	void updateAndDraw(float dt);

	/// ile cz�stek jest aktualnie widocznych
	inline int getVisibleParticlesNum() { return particles.size()-inactiveParticles; }

	/// domy�lna ilo�� cz�stek wyrzucanych podczas generowania cz�stek
	int defaultAmount;

	/// metoda generuj�ca liczby losowe
	inline int fastrandInt() { 
		randSeed = (214013*randSeed+2531011); 
		return (randSeed>>16)&0x7FFF; 
	}

	/// metoda generuj�ca liczby losowe
	inline float fastrandFloat(float start, float end) { 		
		return (end - start)*static_cast<float>(fastrandInt())/static_cast<float>(32000) + start; 
	}
private:

	inline u8 lerp(u8 value1, u8 value2, float percent) { return static_cast<u8>(value1 + percent*(value2-value1)); }

	inline Color lerp(Color color1, Color color2, float percent)
	{
		Color ret;
		ret.r = lerp(color1.r, color2.r, percent);
		ret.g = lerp(color1.g, color2.g, percent);
		ret.b = lerp(color1.b, color2.b, percent);
		ret.a = lerp(color1.a, color2.a, percent);

		return ret;
	}

	/// ziarno dla algorytmu generuj�cego liczby losowe
	int randSeed;

	/// ilo�� par kolor�w
	int colorPairsNum;

	/// kolory wykorzystywane podczas obs�ugi 
	std::vector<Color**> colors;

	/// maksymalna d�ugo�� �ycia cz�stek
	float maximumAge;

	/// sprite wykorzystywany podczas wizualizacji cz�stek
	Sprite sprite;

	/// grawitacja kt�rej podlegaj� cz�stki
	Vector2 gravity;

	/// tarcie jakiemu podlegaj� cz�stki
	float friction;

	/// rozrzut jakiemu podlegaj� cz�stki podczas ich generowania
	Vector2 angleRange;

	/// pocz�tkowa pr�dko�c dla cz�stek
	Vector2 startingVelocity;

	/// wektor przechowuj�cy dane dotycz�ce cz�stek
	std::vector<Particle> particles;

	/// ile cz�stek jest aktualnie nieaktywnych
	int inactiveParticles;

	/// maksymalna ilo�� nieaktywnych cz�stek
	int maximumInactiveParticles;
};