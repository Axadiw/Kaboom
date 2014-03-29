/*! 
 *  \brief     Czcionka
 *  \author    Micha³ Mizera
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
	/// œcie¿ka do czcionki
	const char* name;

	/// obiekt czcionki wykorzystywany przez FTGL
	FTPolygonFont* font;
	
	Font() { font = NULL; }
	Font(const char* path) { font = new FTPolygonFont(path); font->FaceSize(1);}
	~Font() { delete font; };

	/// sprawdza czy za³adowano czcionkê
	inline bool isDummy() { return font == NULL; }

	/*!
	zwraca d³ugoœæ (we wspó³rzêdnych ekranu) zadanego tekstu
	@param text tekst dla któego sprawdzana bêdzie d³ugoœæ
	\details pamiêtaæ o przemno¿eniu wielkoœci przez skalê, gdy¿ ta liczona jest dla czcionki o wielkoœci == 1
	*/
	inline float getTextLength(const char* text) { FTBBox tmp = font->BBox(text); return tmp.Upper().Xf() - tmp.Lower().Xf(); }
};