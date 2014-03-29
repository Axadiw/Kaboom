#pragma once

#include <hash_map>
#include "../Singleton.h"
#include "Texture.h"
#include "SDL.h"
#include "SDL_opengl.h"

/*! 
 *  \brief     Menad¿er tekstur
 *  \author    Micha³ Mizera
 *  \version   1.0
 *  \date      2011-2012
 *  \copyright GNU Public License.
 */

class TextureManager : public Singleton<TextureManager>
{
public:

	/*!
	£aduje zadan¹ teksturê z dysku
	@param path œcie¿ka do tekstury
	*/
	Texture* loadTexture(const char* path);

	/// zwalnia zasoby dla danej tekstury
	void releaseTexture(Texture*);

private:
	stdext::hash_map<const char*, Texture*> textures;
	stdext::hash_map<const char*, int> texturesCount;
};

