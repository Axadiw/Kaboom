/*! 
 *  \brief     Czcionka
 *  \author    Micha� Mizera
 *  \version   1.0
 *  \date      2011-2012
 *  \copyright GNU Public License.
 */

#pragma once

#include <cmath>
#include <FTGL/ftgl.h>

class Font
{
public:
	/// �cie�ka do czcionki
	const char* name;

	/// obiekt czcionki wykorzystywany przez FTGL
	FTPolygonFont* font;
	
	Font() { font = NULL; }
	Font(const char* path) { font = new FTPolygonFont(path); font->FaceSize(1);}
	~Font() { delete font; };

	/// sprawdza czy za�adowano czcionk�
	inline bool isDummy() { return font == NULL; }

	/*!
	zwraca d�ugo�� (we wsp�rz�dnych ekranu) zadanego tekstu
	@param text tekst dla kt�ego sprawdzana b�dzie d�ugo��
	\details pami�ta� o przemno�eniu wielko�ci przez skal�, gdy� ta liczona jest dla czcionki o wielko�ci == 1
	*/
	inline float getTextLength(const char* text) { FTBBox tmp = font->BBox(text); return tmp.Upper().Xf() - tmp.Lower().Xf(); }
};