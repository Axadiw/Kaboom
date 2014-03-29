#include "TextureManagerTests.h"

/// engine
#include "Math/Vector2.h"
#include "Resources/Sprite.h"
#include "Resources/TextureManager.h"
#include "GameEngine.h"
#include "includes.h"

// reszta
#include <cmath>
#include <assert.h>

#define NUMBER_OF_TEXTURE_TESTS 10000

void TextureManagerTests::memoryLeakstTestStart()
{
	GameEngine* engine = GameEngine::getSingletonPtr();

	engine->Init("Texture tests");

	texturesBMP = new Texture*[NUMBER_OF_TEXTURE_TESTS];
	texturesPNG = new Texture*[NUMBER_OF_TEXTURE_TESTS];
	texturesJPG = new Texture*[NUMBER_OF_TEXTURE_TESTS];

	for(int i=0; i<NUMBER_OF_TEXTURE_TESTS; ++i)
	{
		texturesBMP[i] = TextureManager::getSingleton().loadTexture("TestData\\menu.bmp");
		texturesJPG[i] = TextureManager::getSingleton().loadTexture("TestData\\menu.jpg");
		texturesPNG[i] = TextureManager::getSingleton().loadTexture("TestData\\menu.png");
	}
}

void TextureManagerTests::memoryLeakstTestEnd()
{
	for(int i=0; i<NUMBER_OF_TEXTURE_TESTS; ++i)
	{
		TextureManager::getSingleton().releaseTexture(texturesBMP[i]);		
		TextureManager::getSingleton().releaseTexture(texturesJPG[i]);		
		TextureManager::getSingleton().releaseTexture(texturesPNG[i]);		
	}

	delete[] texturesBMP;
	delete[] texturesJPG;
	delete[] texturesPNG;
	
	GameEngine::getSingleton().Cleanup();
}

void TextureManagerTests::loadTexturesTest()
{
	Sprite testBMP, testPNG, testJPG;

	const char* path = "TestData\\textureLoadData.txt";

	Texture* BMP = TextureManager::getSingleton().loadTexture("TestData\\menu.bmp");
	Texture* JPG = TextureManager::getSingleton().loadTexture("TestData\\menu.jpg");
	Texture* PNG = TextureManager::getSingleton().loadTexture("TestData\\menu.png");
	
	ConfigManager::getSingleton().loadFile(path);

	testBMP.load(path, "testBMP");
	testPNG.load(path, "testPNG");
	testJPG.load(path, "testJPG");

	BMP->bind();
	testBMP.draw();

	PNG->bind();
	testPNG.setScale(1.5f);
	testPNG.draw();


	JPG->bind();
	testJPG.draw();

	SDL_GL_SwapBuffers();	

	system("PAUSE");

	ConfigManager::getSingleton().releaseConfigFile(path);
	TextureManager::getSingleton().releaseTexture(BMP);
	TextureManager::getSingleton().releaseTexture(PNG);
	TextureManager::getSingleton().releaseTexture(JPG);	
}
