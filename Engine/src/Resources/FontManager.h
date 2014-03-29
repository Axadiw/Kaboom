#pragma once

#include <hash_map>
#include "../Singleton.h"
#include "Font.h"
#include "SDL.h"
#include "SDL_opengl.h"

/*! 
 *  \brief     Menad¿er czcionek
 *  \author    Micha³ Mizera
 *  \version   1.0
 *  \date      2011-2012
 *  \copyright GNU Public License.
 */

class FontManager : public Singleton<FontManager>
{
public:
	/*!
	³aduje czcionke z zadanego adresu
	@param name adres czcionki na dysku
	\return adres do za³adowanej czcionki, w przypadku b³êdu NULL
	\details w przypadku próbu za³adowania czcionki wielokrotnie, czcionka jest ³adowana do pamiêci tylko raz, wzrasta jedynie wewnêtrzny licznik odwo³añ
	*/
	Font* loadFont(const char* name);

	/*! zwalnia zasoby dla zadanej czcionki
	\details zmniejsza licznik odwo³añ, i w przypadku braku odwo³añ do czcionki, nastêpuje usuniêcie zasobów z pamiêci
	*/
	void releaseFont(Font*);

private:
	stdext::hash_map<const char*, Font*> fonts;
	stdext::hash_map<const char*, int> fontsCount;
};

