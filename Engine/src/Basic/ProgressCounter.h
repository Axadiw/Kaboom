#pragma once

#include "ProgressCounter.h"

enum PROGRESS_DIRECTION { UP, DOWN, STABLE};

/*! 
 *  \brief     Licznik stopnia zaawansowania
 *  \details Wykorzystywanie podczas wyœwietlania animacji
 *  \author    Micha³ Mizera
 *  \version   1.0
 *  \date      2011-2012
 *  \copyright GNU Public License.
 */

class ProgressCounter
{
public:
	ProgressCounter(): progress(0.0f), maxValue(0.0f), minValue(0.0f), speed(0.0f){};

	/// aktualny stan zaawansowania
	float progress;

	/// wartoœæ maksymalna licznika
	float maxValue;

	/// wartoœæ minimalna licznika
	float minValue;

	/// prêdkoœæ zmian licznika
	float speed;

	/// kierunek zmian licznika (czy wzrasta, czy maleje)
	int direction;


	/*! 
	wczytuje wartoœæ licznika z pliku (dla hierarchii 1 poziomowej)
	@param configFile adres pliku konfiguracyjnego
	@param name nazwa atrybutu
	*/
	void load(const char* configFile, const char* name);

	/*! 
	wczytuje wartoœæ licznika z pliku (dla hierarchii 2 poziomowej)
	@param configFile adres pliku konfiguracyjnego
	@param name nazwa atrybutu 
	@param name2 nazwa atrybutu (2 poziom w hierarchii)
	*/
	void load(const char* configFile, const char* name, const char* name2);

	/*!
	zmienia wartoœæ licznika (zgodnie z prêdkoœci¹ i kierunkiem)
	@param change wartoœæ zmiany (czêsto mo¿e byæ to delta czasu)
	*/
	void updateLinear(float change);

	/// zwraca true je¿êli licznik dokoñczy³ zliczanie (sprawdza kierunek zmian licznika)
	bool finished();
};