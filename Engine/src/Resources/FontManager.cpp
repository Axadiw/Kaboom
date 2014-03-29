#include <assert.h>

#include "FontManager.h"
#include "../includes.h"
#include "SDL_image.h"

Font* FontManager::loadFont(const char* path)
{
	if(fonts.find(path) != fonts.end())
	{
		// font juz otwarty, zwieksz licznik odwo³añ
		(fontsCount[path])++;

		return fonts[path];
	}

	Font* retFont = new Font(path);
	
	if(retFont->font->Error())
	{
		// b³¹d
		delete retFont;
		return NULL;
	}

	retFont->name = path;

	fonts[path] = retFont;
	fontsCount[path] = 1;

	return retFont;
}

void FontManager::releaseFont(Font* font)
{
	stdext::hash_map<const char*, Font*>::iterator itFont = fonts.find(font->name);
	stdext::hash_map<const char*, int>::iterator itCount = fontsCount.find(font->name);

	if(itFont == fonts.end() || itCount == fontsCount.end() || font->isDummy())
	{
		// nie ma takiego fonta w bazie, lub font jest pusty
		return;
	}

	(itCount->second)--;

	if(itCount->second <= 0)
	{
		delete itFont->second;

		fonts.erase(itFont);
		fontsCount.erase(itCount);
	}
}