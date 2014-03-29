#pragma once

#include "../Math/Vector2.h"
#include "../includes.h"
#include "Color.h"

/*! 
 *  \brief     Klasa abstrakcyjna obiekt graficzny
 *  \author    Micha� Mizera
 *  \version   1.0
 *  \date      2011-2012
 *  \copyright GNU Public License.
 */

class GraphicObject
{
public:

	/// ustaw pozycj�
	inline void setPos(Vector2 pos_) { pos = pos_; };
	/// ustaw pozycj�
	inline void setPos(float x_, float y_) { pos.x = x_; pos.y = y_; };
	/// ustaw rozmiar
	inline void setSize(Vector2 size_) { size = size_; };
	/// ustaw rozmiar
	inline void setSize(float x_, float y_) { size.x = x_; size.y = y_; };
	/// ustaw kolor
	inline void setColor(Color c_) { color = c_; };
	/// ustaw kolor
	inline void setColor(u8 r, u8 g, u8 b, u8 a) { color.r = r; color.g = g; color.b = b; color.a = a; };
	/// ustaw k�t obrotu
	inline void setAngle(float angle_) { angle = angle_; };
	/// ustaw wsp�czynnik skalowania
	inline void setScale(float scale_) { scale = scale_; };

	/// pobierz pozycj�
	inline Vector2 getPos() { return pos; };
	/// pobierz rozmiar
	inline Vector2 getSize() { return size; };
	/// pobierz kolor
	inline Color getColor() { return color; };
	/// pobierz k�t obrotu
	inline float getAngle() { return angle; };
	/// pobierz wsp�czynnik skalowania
	inline float getScale() { return scale; };

	/// pobierz oryginaln� pozycj� (wczytan� z pliku)
	inline Vector2 getOriginalPos() { return originalPos; };
	/// pobierz oryginalny rozmiar (wczytany z pliku)
	inline Vector2 getOriginalSize() { return originalSize; };
	/// pobierz oryginalny kolor (wczytany z pliku)
	inline Color getOriginalColor() { return originalColor; };
	/// pobierz oryginalny k�t obrotu (wczytany z pliku)
	inline float getOriginalAngle() { return originalAngle; };
	/// pobierz oryginalny wsp�czynnik skalowania (wczytany z pliku)
	inline float getOriginalScale() { return originalScale; };
	
protected:
	Vector2 pos;
	Vector2 size;
	Color color;
	float angle;
	float scale;

	Vector2 originalPos;
	Vector2 originalSize;
	Color originalColor;
	float originalAngle;
	float originalScale;
};