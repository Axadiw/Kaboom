#pragma once

#include "Singleton.h"
#include "Resources/ConfigManager.h"
#include "Resources/TextureManager.h"

class TextureManagerTests: public Singleton<TextureManagerTests>
{
public:
	void memoryLeakstTestStart();
	void memoryLeakstTestEnd();
	void loadTexturesTest();

private:
	Texture** texturesBMP;
	Texture** texturesJPG;
	Texture** texturesPNG;
};