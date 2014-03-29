#include <assert.h>

#include "TextureManager.h"
#include "../includes.h"
#include "SDL_image.h"

Texture* TextureManager::loadTexture(const char* path)
{
	if(textures.find(path) != textures.end())
	{
		(texturesCount[path])++;

		return textures[path];
	}

	Texture* retTex = new Texture;
	SDL_Surface* surface;

	retTex->name = path;

	if ( (surface = IMG_Load(path)) ) 
	{ 		
		// szerokosc to nie potêga 2
		assert((surface->w & (surface->w - 1)) == 0);

		// wysokosc to nie potêga 2
		assert((surface->h & (surface->h - 1)) == 0);

		if ( surface->format->BytesPerPixel == 4)     // contains an alpha channel
		{
			if (surface->format->Rmask == 0x000000ff)
				retTex->texture_format = GL_RGBA;
			else
				retTex->texture_format = GL_BGRA;

			SDL_SetColorKey(surface, SDL_RLEACCEL, surface->format->colorkey);
		} else if ( surface->format->BytesPerPixel == 3)     // no alpha channel
		{
			if (surface->format->Rmask == 0x000000ff)
				retTex->texture_format = GL_RGB;
			else
				retTex->texture_format = GL_BGR;
		} else {
			AX_PRINT_ERROR("tekstura nie jest w truecolor!");
		}

		// Have OpenGL generate a texture object handle for us
		glGenTextures( 1, &retTex->texture );

		// Bind the texture object
		glBindTexture( GL_TEXTURE_2D, retTex->texture );

		// Set the texture's stretching properties
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

		// Edit the texture object's image data using the information SDL_Surface gives us
		glTexImage2D( GL_TEXTURE_2D, 0,  surface->format->BytesPerPixel, surface->w, surface->h, 0, retTex->texture_format, GL_UNSIGNED_BYTE, surface->pixels );
		
		SDL_FreeSurface(surface);			

		textures[path] = retTex;
		texturesCount[path] = 1;

		return retTex;
	} 
	else 
	{
		delete retTex;
		return NULL;
	} 	
}

void TextureManager::releaseTexture(Texture* texture)
{
	stdext::hash_map<const char*, Texture*>::iterator itTex = textures.find(texture->name);
	stdext::hash_map<const char*, int>::iterator itCount = texturesCount.find(texture->name);

	bool b1 = itTex == textures.end();
	bool b2 = itCount == texturesCount.end();
	bool b3 = texture->isDummy();

	if(itTex == textures.end() || itCount == texturesCount.end() || texture->isDummy())
	{
		// nie ma takiej tekstury w bazie, lub tekstura jest pusta
		return;
	}

	(itCount->second)--;

	if(itCount->second <= 0)
	{
		glDeleteTextures(1, &(texture->texture));
		delete itTex->second;

		textures.erase(itTex);
		texturesCount.erase(itCount);
	}
}