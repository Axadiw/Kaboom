#pragma once

#include "GraphicObject.h"

/*! 
 *  \brief     Sprite 2D
 *  \author    Micha³ Mizera
 *  \version   1.0
 *  \date      2011-2012
 *  \copyright GNU Public License.
 */

class Sprite: public GraphicObject
{
public:
	/// wspó³rzêdna X dla lewego rogu (dla obrazu nie-odwracanego) fragmentu tekstury dla obiektu
	float minU;
	/// wspó³rzêdna X dla prawego rogu (dla obrazu nie-odwracanego) fragmentu tekstury dla obiektu
	float maxU;
	/// wspó³rzêdna Y dla górnego rogu (dla obrazu nie-odwracanego) fragmentu tekstury dla obiektu
	float minV;
	/// wspó³rzêdna Y dla dolnego rogu (dla obrazu nie-odwracanego) fragmentu tekstury dla obiektu
	float maxV;

	/// czy Sprite jest aktywny
	bool isActive;

	/// lewy górny róg na teksturze dla obiektu
	Vector2 textureCorner;

	/// rozmiary tekstury ( w pikselach, odpowiednia dla rozmiarów ca³ej tekstury)
	Vector2 textureSize;

	/// rozmiary ca³ej tekstu z której wycinany jest fragment dla obiektu
	Vector2 textureDimensions;

	/// odwracaj teksturê po wspó³rzêdnej X
	bool flipX;

	/// odwracaj teksturê po wspó³rzêdnej Y
	bool flipY;

	/*! 
	wczytaj sprite z pliku
	@param objName nazwa obiektu w pliku konfuguracyjnym
	@param configFileName œcie¿ka do pliku konfiguracyjnego
	*/
	void load(const char* configFileName, const char* objName);

	/*! 
	zapisz sprite z pliku
	@param objName nazwa obiektu w pliku konfuguracyjnym
	@param configFileName œcie¿ka do pliku konfiguracyjnego
	*/
	void save(const char* configFileName, const char* objName);

	/// rysuj sprite na ekranie
	inline void draw() { if(isActive) draw(pos, halfSize, minU, maxU, minV, maxV, angle, color); };

	/*!
	rysuj sprite na ekranie
	@param c_ wartoœæ koloru przez który przemna¿amy sprite
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
	@param c_ wartoœæ koloru przez który przemna¿amy sprite
	*/
	inline void draw(Vector2 pos_, Color c_) { if(isActive) draw(pos_, halfSize, minU, maxU, minV, maxV, angle, c_); };

	/*!
	rysuj sprite na ekranie
	@param pos_ pozycja dla sprite
	@param angle_ k¹t obrotu sprite
	*/
	inline void draw(Vector2 pos_, float angle_) { if(isActive) draw(pos_, halfSize, minU, maxU, minV, maxV, angle_, color); };

	/*!
	rysuj sprite na ekranie
	@param pos_ pozycja dla sprite
	@param c_ wartoœæ koloru przez który przemna¿amy sprite
	@param angle_ k¹t obrotu sprite
	*/
	inline void draw(Vector2 pos_, float angle_, Color c_) { if(isActive) draw(pos_, halfSize, minU, maxU, minV, maxV, angle_, c_); };

	/*!
	rysuj sprite na ekranie
	@param pos_ pozycja dla sprite
	@param halfSize_ po³owa wielkoœci sprite (we wspó³rzêdnych ekranu)
	@param c_ wartoœæ koloru przez który przemna¿amy sprite
	@param angle_ k¹t obrotu sprite
	@param minU  wspó³rzêdna X dla lewego rogu (dla obrazu nie-odwracanego) fragmentu tekstury dla obiektu
	@param maxU  wspó³rzêdna X dla prawego rogu (dla obrazu nie-odwracanego) fragmentu tekstury dla obiektu
	@param minV  wspó³rzêdna Y dla górnego rogu (dla obrazu nie-odwracanego) fragmentu tekstury dla obiektu
	@param maxV  wspó³rzêdna Y dla dolnego rogu (dla obrazu nie-odwracanego) fragmentu tekstury dla obiektu
	*/
	static void draw(Vector2 pos, Vector2 halfSize_, float minU, float maxU, float minV, float maxV, float angle, Color c);
	
	/*!
	Ustawia wielkoœæ sprite
	@param size_ wielkoœæ
	*/
	void setSize(Vector2 size_);

	/*!
	Ustawia wspó³czynnik skalowania dla sprite
	@param size_ wielkoœæ
	*/
	void setScale(float scale_);
private:
	Vector2 halfSize;
};