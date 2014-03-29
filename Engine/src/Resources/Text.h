#pragma once

#include "GraphicObject.h"
#include "../Math/Vector2.h"
#include "Font.h"

#define TEXT_MAX_LENGTH 255

/*! 
 *  \brief     Tekst wy�wietlany na ekranie
 *  \author    Micha� Mizera
 *  \version   1.0
 *  \date      2011-2012
 *  \copyright GNU Public License.
 */

class Text : public GraphicObject
{
public:
	/// zwraca tekst w�a�ciwy
	const char* getText() { return text; };
	/*! 
	ustawia tekst
	@param text_ tekst do ustawienia
	*/
	void setText(const char* text_) { strcpy(text, text_); };

	/*!
	Rysuje tekst na ekranie (pozycja tekstu to �rodek tekstu)
	@param font czcionka wykorzystywana przy rysowaniu
	*/
	void draw(Font* font) { if(isActive) drawCentered(pos, scale, angle, color, text, font); };

	/*!
	Rysuje tekst na ekranie (pozycja tekstu to �rodek tekstu)
	@param pos_ pozycja tekstu
	@param font czcionka wykorzystywana przy rysowaniu
	*/
	void draw(Font* font, Vector2 pos_) { if(isActive) drawCentered(pos_, scale, angle, color, text, font); };

	/*!
	Rysuje tekst na ekranie (pozycja tekstu to lewy dolny r�g tekstu)
	@param font czcionka wykorzystywana przy rysowaniu
	*/
	void drawLB(Font* font) { if(isActive) drawLeftBottom(pos, scale, angle, color, text, font); };

	/*!
	Rysuje tekst na ekranie (pozycja tekstu to lewy dolny r�g tekstu)
	@param font czcionka wykorzystywana przy rysowaniu
	@param pos_ pozycja tekstu
	*/
	void drawLB(Font* font, Vector2 pos_) { if(isActive) drawLeftBottom(pos_, scale, angle, color, text, font); };

	/*!
	Rysuje tekst na ekranie (pozycja tekstu to �rodek tekstu)
	@param pos_ pozycja tekstu
	@param scale_ wielko�� czcionki
	@param angle_ k�t obrotu
	@param color_ kolor tekstu
	@param text_ tekst do wypisania
	@param font_ czcionka wykorzystywana przy rysowaniu
	*/
	static void drawCentered(Vector2 pos_, float scale_, float angle_, Color color_, const char* text_, Font* font_);

	/*!
	Rysuje tekst na ekranie (pozycja tekstu to lewy dolny r�g tekstu)
	@param pos_ pozycja tekstu
	@param scale_ wielko�� czcionki
	@param angle_ k�t obrotu
	@param color_ kolor tekstu
	@param text_ tekst do wypisania
	@param font_ czcionka wykorzystywana przy rysowaniu
	*/
	static void drawLeftBottom(Vector2 pos_, float scale_, float angle_, Color color_, const char* text_, Font* font_);

	/*!
	Wczytuje tekst z pliku
	@param objName nazwa parametru w pliku
	@Param configFileName �cie�ka do pliku konfiguracyjnego
	*/
	void load(const char* configFileName, const char* objName);

	/*!
	Zwraca d�ugo�� dla danego tekstu dla danej czcionki
	@param font czcionka
	*/
	float getTextLength(Font* font) { return scale*font->getTextLength(text); };

	/// tekst w�a�ciwy (nie jest prywatny, gdy� dzi�ki temu �atwo si� do niego dosta� przez sprintf)
	char* text;

	Text(){ text = new char[TEXT_MAX_LENGTH]; }
	~Text(){ delete[] text; }

	bool isActive;
};