#pragma once

#include "ProgressCounter.h"

enum PROGRESS_DIRECTION { UP, DOWN, STABLE};

/*! 
 *  \brief     Licznik stopnia zaawansowania
 *  \details Wykorzystywanie podczas wy�wietlania animacji
 *  \author    Micha� Mizera
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

	/// warto�� maksymalna licznika
	float maxValue;

	/// warto�� minimalna licznika
	float minValue;

	/// pr�dko�� zmian licznika
	float speed;

	/// kierunek zmian licznika (czy wzrasta, czy maleje)
	int direction;


	/*! 
	wczytuje warto�� licznika z pliku (dla hierarchii 1 poziomowej)
	@param configFile adres pliku konfiguracyjnego
	@param name nazwa atrybutu
	*/
	void load(const char* configFile, const char* name);

	/*! 
	wczytuje warto�� licznika z pliku (dla hierarchii 2 poziomowej)
	@param configFile adres pliku konfiguracyjnego
	@param name nazwa atrybutu 
	@param name2 nazwa atrybutu (2 poziom w hierarchii)
	*/
	void load(const char* configFile, const char* name, const char* name2);

	/*!
	zmienia warto�� licznika (zgodnie z pr�dko�ci� i kierunkiem)
	@param change warto�� zmiany (cz�sto mo�e by� to delta czasu)
	*/
	void updateLinear(float change);

	/// zwraca true je��li licznik doko�czy� zliczanie (sprawdza kierunek zmian licznika)
	bool finished();
};