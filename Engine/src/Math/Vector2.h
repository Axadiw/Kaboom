#pragma once

#include <cmath>

/*! 
 *  \brief     Wektor dwuwymiarowy
 *  \author    Micha³ Mizera
 *  \version   1.0
 *  \date      2011-2012
 *  \copyright GNU Public License.
 */

class Vector2
{
public:
	/// wspó³rzêdna X
	float x;
	/// wspó³rzêdna Y
	float y;

	Vector2() {};
	Vector2(Vector2& vec) { x = vec.x; y = vec.y; };
	Vector2(double x_, double y_) : x(static_cast<float>(x_)), y(static_cast<float>(y_)) {};
	Vector2(int x_, int y_) : x(static_cast<float>(x_)), y(static_cast<float>(y_)) {};
	Vector2(float x_, float y_) : x(x_), y(y_) {};
	Vector2(int value) : x(static_cast<float>(value)), y(static_cast<float>(value)) {};
	Vector2(double value) : x(static_cast<float>(value)), y(static_cast<float>(value)) {};
	Vector2(float value) : x(value), y(value) {};

	inline void operator= (const Vector2& v) { x = v.x; y = v.y; }
	inline void operator+= (const Vector2& v) { x += v.x; y += v.y; }
	inline void operator-= (const Vector2& v) { x -= v.x; y -= v.y; }

	/// iloczyn skalarny
	inline void operator*= (float s) { x *= s; y *= s; }

	inline Vector2 operator+ (const Vector2& v) const { return Vector2(v.x + x, v.y + y); }
	inline Vector2 operator- (const Vector2& v) const {	return Vector2(x - v.x, y - v.y); }

	/// iloczyn skalarny
	inline Vector2 operator* (float s) const { return Vector2(x*s, y*s); }

	/// odwrotnoœæ iloczynu skalarnego
	inline Vector2 operator/ (float s) const { return Vector2(x/s, y/s); }
	inline Vector2 operator- () const { return Vector2(-x, -y); }

	void rotate(float angle);

	inline bool operator== (const Vector2& v) { return (this->x == v.x) && (this->y == v.y); }
	inline bool operator!= (const Vector2& v) { return !(*this == v); }

	/// odleg³oœæ od innego wektora
	inline float distance(Vector2 v){return sqrt(pow(x-v.x, 2.0f) + pow(y-v.y, 2.0f)); }

	/*!
	wczytaj wektor z pliku (dla hierarchii 1 poziomowej)
	@param configFile œcie¿ka do pliku konfiguracyjnego
	@param name nazwa wektora w pliku konfiguracyjnym
	@param default_value wartoœæ domyœlna
	*/
	void load(const char* configFile, const char* name, Vector2 default_value = Vector2(0.0f, 0.0f));

	/*!
	wczytaj wektor z pliku (dla hierarchii 2 poziomowej)
	@param configFile œcie¿ka do pliku konfiguracyjnego
	@param name nazwa wektora w pliku konfiguracyjnym  ( 1 poziom hierarchii )
	@param name2 nazwa wektora w pliku konfiguracyjnym ( 2 poziom hierarchii )
	@param default_value wartoœæ domyœlna
	*/
	void load(const char* configFile, const char* name, const char* name2, Vector2 default_value = Vector2(0.0f, 0.0f));

	/*!
	zapisz wektor do pliku (dla hierarchii 1 poziomowej)
	@param configFile œcie¿ka do pliku konfiguracyjnego
	@param name nazwa wektora w pliku konfiguracyjnym
	*/
	void save(const char* configFile, const char* name);

	/*!
	zapisz wektor do pliku (dla hierarchii 2 poziomowej)
	@param configFile œcie¿ka do pliku konfiguracyjnego
	@param name nazwa wektora w pliku konfiguracyjnym
	*/
	void save(const char* configFile, const char* name, const char* name2);

	void normalize();
};