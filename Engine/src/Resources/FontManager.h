#pragma once

#include <hash_map>
#include "../Singleton.h"
#include "Font.h"
#include "SDL.h"
#include "SDL_opengl.h"

/*! 
 *  \brief     Menad�er czcionek
 *  \author    Micha� Mizera
 *  \version   1.0
 *  \date      2011-2012
 *  \copyright GNU Public License.
 */

class FontManager : public Singleton<FontManager>
{
public:
	/*!
	�aduje czcionke z zadanego adresu
	@param name adres czcionki na dysku
	\return adres do za�adowanej czcionki, w przypadku b��du NULL
	\details w przypadku pr�bu za�adowania czcionki wielokrotnie, czcionka jest �adowana do pami�ci tylko raz, wzrasta jedynie wewn�trzny licznik odwo�a�
	*/
	Font* loadFont(const char* name);

	/*! zwalnia zasoby dla zadanej czcionki
	\details zmniejsza licznik odwo�a�, i w przypadku braku odwo�a� do czcionki, nast�puje usuni�cie zasob�w z pami�ci
	*/
	void releaseFont(Font*);

private:
	stdext::hash_map<const char*, Font*> fonts;
	stdext::hash_map<const char*, int> fontsCount;
};

