#pragma once

#include "GraphicObject.h"
#include "Particle.h"
#include "Sprite.h"

#include <time.h>
#include <vector>

/*! 
 *  \brief     System efektów cz¹stek
 *  \author    Micha³ Mizera
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

	/// Nazwa pliku przechowuj¹cego dane opisuj¹ce systme cz¹stek
	const char* name;

	/*!
	wczytaj system z pliku
	@param configFile œcie¿ka do pliku konfiguracyjnego
	*/
	void load(const char* configFile);

	/*!
	dodaje odpowiedni¹ iloœæ cz¹stek do systemu
	@param position pozycja emitera cz¹stek
	@param amount iloœæ cz¹stek
	*/
	void spawn(Vector2 position, int amount);

	/*!
	uaktualnia pozycjê wszystkich cz¹tek, oraz rysuje je na ekranie
	*/
	void updateAndDraw(float dt);

	/// ile cz¹stek jest aktualnie widocznych
	inline int getVisibleParticlesNum() { return particles.size()-inactiveParticles; }

	/// domyœlna iloœæ cz¹stek wyrzucanych podczas generowania cz¹stek
	int defaultAmount;

	/// metoda generuj¹ca liczby losowe
	inline int fastrandInt() { 
		randSeed = (214013*randSeed+2531011); 
		return (randSeed>>16)&0x7FFF; 
	}

	/// metoda generuj¹ca liczby losowe
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

	/// ziarno dla algorytmu generuj¹cego liczby losowe
	int randSeed;

	/// iloœæ par kolorów
	int colorPairsNum;

	/// kolory wykorzystywane podczas obs³ugi 
	std::vector<Color**> colors;

	/// maksymalna d³ugoœæ ¿ycia cz¹stek
	float maximumAge;

	/// sprite wykorzystywany podczas wizualizacji cz¹stek
	Sprite sprite;

	/// grawitacja której podlegaj¹ cz¹stki
	Vector2 gravity;

	/// tarcie jakiemu podlegaj¹ cz¹stki
	float friction;

	/// rozrzut jakiemu podlegaj¹ cz¹stki podczas ich generowania
	Vector2 angleRange;

	/// pocz¹tkowa prêdkoœc dla cz¹stek
	Vector2 startingVelocity;

	/// wektor przechowuj¹cy dane dotycz¹ce cz¹stek
	std::vector<Particle> particles;

	/// ile cz¹stek jest aktualnie nieaktywnych
	int inactiveParticles;

	/// maksymalna iloœæ nieaktywnych cz¹stek
	int maximumInactiveParticles;
};