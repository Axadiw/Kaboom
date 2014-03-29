#pragma once

#include "GraphicObject.h"

/*! 
 *  \brief     Sprite 2D
 *  \author    Micha� Mizera
 *  \version   1.0
 *  \date      2011-2012
 *  \copyright GNU Public License.
 */

class Sprite: public GraphicObject
{
public:
	/// wsp�rz�dna X dla lewego rogu (dla obrazu nie-odwracanego) fragmentu tekstury dla obiektu
	float minU;
	/// wsp�rz�dna X dla prawego rogu (dla obrazu nie-odwracanego) fragmentu tekstury dla obiektu
	float maxU;
	/// wsp�rz�dna Y dla g�rnego rogu (dla obrazu nie-odwracanego) fragmentu tekstury dla obiektu
	float minV;
	/// wsp�rz�dna Y dla dolnego rogu (dla obrazu nie-odwracanego) fragmentu tekstury dla obiektu
	float maxV;

	/// czy Sprite jest aktywny
	bool isActive;

	/// lewy g�rny r�g na teksturze dla obiektu
	Vector2 textureCorner;

	/// rozmiary tekstury ( w pikselach, odpowiednia dla rozmiar�w ca�ej tekstury)
	Vector2 textureSize;

	/// rozmiary ca�ej tekstu z kt�rej wycinany jest fragment dla obiektu
	Vector2 textureDimensions;

	/// odwracaj tekstur� po wsp�rz�dnej X
	bool flipX;

	/// odwracaj tekstur� po wsp�rz�dnej Y
	bool flipY;

	/*! 
	wczytaj sprite z pliku
	@param objName nazwa obiektu w pliku konfuguracyjnym
	@param configFileName �cie�ka do pliku konfiguracyjnego
	*/
	void load(const char* configFileName, const char* objName);

	/*! 
	zapisz sprite z pliku
	@param objName nazwa obiektu w pliku konfuguracyjnym
	@param configFileName �cie�ka do pliku konfiguracyjnego
	*/
	void save(const char* configFileName, const char* objName);

	/// rysuj sprite na ekranie
	inline void draw() { if(isActive) draw(pos, halfSize, minU, maxU, minV, maxV, angle, color); };

	/*!
	rysuj sprite na ekranie
	@param c_ warto�� koloru przez kt�ry przemna�amy sprite
	*/
	inline void draw(Color c_) { if(isActive) draw(pos, halfSize, minU, maxU, minV, maxV, angle, c_); };

	/*!
	rysuj sprite na ekranie
	@param pos_ pozycja dla sprite
	*/
	inline void draw(Vector2 pos_) { if(isActive) draw(pos_, halfSize, minU, maxU, minV, maxV, angle, color); };

	/*!
	rysuj sprite na ekranie
	@param pos_ pozycja dla sprite
	@param c_ warto�� koloru przez kt�ry przemna�amy sprite
	*/
	inline void draw(Vector2 pos_, Color c_) { if(isActive) draw(pos_, halfSize, minU, maxU, minV, maxV, angle, c_); };

	/*!
	rysuj sprite na ekranie
	@param pos_ pozycja dla sprite
	@param angle_ k�t obrotu sprite
	*/
	inline void draw(Vector2 pos_, float angle_) { if(isActive) draw(pos_, halfSize, minU, maxU, minV, maxV, angle_, color); };

	/*!
	rysuj sprite na ekranie
	@param pos_ pozycja dla sprite
	@param c_ warto�� koloru przez kt�ry przemna�amy sprite
	@param angle_ k�t obrotu sprite
	*/
	inline void draw(Vector2 pos_, float angle_, Color c_) { if(isActive) draw(pos_, halfSize, minU, maxU, minV, maxV, angle_, c_); };

	/*!
	rysuj sprite na ekranie
	@param pos_ pozycja dla sprite
	@param halfSize_ po�owa wielko�ci sprite (we wsp�rz�dnych ekranu)
	@param c_ warto�� koloru przez kt�ry przemna�amy sprite
	@param angle_ k�t obrotu sprite
	@param minU  wsp�rz�dna X dla lewego rogu (dla obrazu nie-odwracanego) fragmentu tekstury dla obiektu
	@param maxU  wsp�rz�dna X dla prawego rogu (dla obrazu nie-odwracanego) fragmentu tekstury dla obiektu
	@param minV  wsp�rz�dna Y dla g�rnego rogu (dla obrazu nie-odwracanego) fragmentu tekstury dla obiektu
	@param maxV  wsp�rz�dna Y dla dolnego rogu (dla obrazu nie-odwracanego) fragmentu tekstury dla obiektu
	*/
	static void draw(Vector2 pos, Vector2 halfSize_, float minU, float maxU, float minV, float maxV, float angle, Color c);
	
	/*!
	Ustawia wielko�� sprite
	@param size_ wielko��
	*/
	void setSize(Vector2 size_);

	/*!
	Ustawia wsp�czynnik skalowania dla sprite
	@param size_ wielko��
	*/
	void setScale(float scale_);
private:
	Vector2 halfSize;
};