/*! 
 *  \brief     Kolor
 *  \author    Micha³ Mizera
 *  \version   1.0
 *  \date      2011-2012
 *  \copyright GNU Public License.
 */

#pragma once
#include "../includes.h"
#include "ConfigManager.h"

struct Color {
	u8 r,g,b,a;

	Color() {}
	Color(const Color& c)
	{
		r = c.r;
		g = c.g;
		b = c.b;
		a = c.a;
	}
	Color(unsigned int c) {
		r = static_cast<u8>((c>>24) | 0x000F);
		g = static_cast<u8>((c>>16) | 0x000F);
		b = static_cast<u8>((c>>8)  | 0x000F);
		a = static_cast<u8>((c)		| 0x000F);
	}
	Color(u8 R, u8 G, u8 B, u8 A) {
		r = R;
		g = G;
		b = B;
		a = A;
	}

	int operator==(const Color &c) {return r == c.r && g == c.g && b == c.b && a == c.a; }; 
	
	/*!
	wczytuje kolor z pliku 
	@param configFile adres pliku konfiguracyjnego
	@param name nazwa atrybutu w pliku
	@param default_color kolor domyœlny
	*/
	void load(const char* configFile, const char* name, Color default_value = Color(255,255,255,255));

	/*!
	wczytuje kolor z pliku 
	@param configFile adres pliku konfiguracyjnego
	@param name nazwa atrybutu w pliku ( 1 poziom hierarchii )
	@param name2 nazwa atrybutu w pliku ( 2 poziom hierarchii )
	@param default_color kolor domyœlny
	*/
	void load(const char* configFile, const char* name, const char* name2, Color default_value = Color(255,255,255,255));

	/*!
	zapisz kolor do pliku 
	@param configFile adres pliku konfiguracyjnego
	@param name nazwa atrybutu w pliku
	@param default_color kolor domyœlny
	*/
	void save(const char* configFile, const char* name);

	/*!
	zapisz kolor do pliku 
	@param configFile adres pliku konfiguracyjnego
	@param name nazwa atrybutu w pliku ( 1 poziom hierarchii )
	@param name2 nazwa atrybutu w pliku ( 2 poziom hierarchii )
	*/
	void save(const char* configFile, const char* name, const char* name2);
};