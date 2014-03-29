#include "FontManagerTests.h"

/// engine
#include "Math/Vector2.h"
#include "Resources/Sprite.h"
#include "Resources/FontManager.h"
#include "Resources/TextureManager.h"
#include "GameEngine.h"
#include "includes.h"
#include "SDL_opengl.h"
#include "Resources/Font.h"

// reszta
#include <cmath>
#include <assert.h>

#define NUMBER_OF_FONT_TESTS 10000

void FontManagerTests::textTest()
{
	Font* font = FontManager::getSingleton().loadFont("TestData\\verdana.ttf");
	Font* font2 = FontManager::getSingleton().loadFont("TestData\\verdana.ttf");
	Font* font3 = FontManager::getSingleton().loadFont("TestData\\LHANDW.TTF");
	
	static const char* filePath = "TestData\\fontTest.txt";
	const char* path = "TestData\\textureLoadData.txt";

	ConfigManager::getSingleton().createConfigFile(filePath);

	// zapisywanie w pliku wartoœci

	ConfigManager::getSingleton().setValue(filePath, "font", "position", "x", 1.0f);
	ConfigManager::getSingleton().setValue(filePath, "font", "position", "y", 2.0f);

	ConfigManager::getSingleton().setValue(filePath, "font", "angle", 0.0f);
	ConfigManager::getSingleton().setValue(filePath, "font", "scale", 10.0f);

	ConfigManager::getSingleton().setValue(filePath, "font", "color", "r", 255);
	ConfigManager::getSingleton().setValue(filePath, "font", "color", "g", 0);
	ConfigManager::getSingleton().setValue(filePath, "font", "color", "b", 0);
	ConfigManager::getSingleton().setValue(filePath, "font", "color", "a", 255);

	ConfigManager::getSingleton().setValue(filePath, "font", "text", "Tekst do wypisania");

	ConfigManager::getSingleton().saveFile(filePath);
	ConfigManager::getSingleton().releaseConfigFile(filePath);

	ConfigManager::getSingleton().loadFile(filePath);
	ConfigManager::getSingleton().loadFile(path);
	/// narysuj teksturê
	Sprite testBMP;

	Texture* BMP = TextureManager::getSingleton().loadTexture("TestData\\menu.bmp");

	testBMP.load(path, "testBMP");

	testBMP.setPos(Vector2(0.0f, 0.0f));
	testBMP.setScale(1.0f);
	BMP->bind();
	testBMP.draw();

	TextureManager::getSingleton().releaseTexture(BMP);

	Text t1, t2, t3, t4;

	t1.load(filePath, "font");
	t3.load(filePath, "font");
	t4.load(filePath, "font");
	t3.setPos(t3.getPos() + Vector2(0.0f, 50.0f));
	t4.setPos(t4.getPos() + Vector2(0.0f, -50.0f));

	assert(t1.getAngle() == 0.0f);
	assert(t1.getScale() == 10.0f);
	assert(t1.getPos() == Vector2(1.0f, 2.0f));
	assert(t1.getColor() == Color(255,0,0,255));
	assert(!strcmp(t1.getText(), "Tekst do wypisania"));

	t1.setText("t1 Tekst do wypisania");
	t3.setText("t3 Tekst do wypisania");
	t4.setText("t4 Tekst do wypisania");

	t2.setPos(Vector2(-170.0f, -20.0f));
	t2.setAngle(0.0f);
	t2.setScale(3.0f);
	t2.setColor(Color(255,255,255,255));
	sprintf(t2.text, "t1 len: %.2f, t3 len: %.2f, t4 len: %.2f, obrazek ma %.2f", t1.getTextLength(font), t3.getTextLength(font2), t4.getTextLength(font3), testBMP.getSize().x);

	t1.draw(font);
	t2.drawLB(font);
	t3.draw(font2);
	t4.draw(font3);

	FontManager::getSingleton().releaseFont(font);
	FontManager::getSingleton().releaseFont(font2);
	FontManager::getSingleton().releaseFont(font3);
	ConfigManager::getSingleton().releaseConfigFile(filePath);
	ConfigManager::getSingleton().releaseConfigFile(path);
	AX_PRINT("Font test OK!\n");	
}

void FontManagerTests::memoryLeakstTestStart()
{
	GameEngine* engine = GameEngine::getSingletonPtr();

	engine->Init("Font tests");

	fonts = new Font*[NUMBER_OF_FONT_TESTS];

	for(int i=0; i<NUMBER_OF_FONT_TESTS; ++i)
	{
		fonts[i] = FontManager::getSingleton().loadFont("TestData\\verdana.ttf");		
	}

}
void FontManagerTests::memoryLeakstTestEnd()
{
	SDL_GL_SwapBuffers();
	system("PAUSE");

	for(int i=0; i<NUMBER_OF_FONT_TESTS; ++i)
	{
		FontManager::getSingleton().releaseFont(fonts[i]);			
	}

	delete[] fonts;

	GameEngine::getSingleton().Cleanup();
}