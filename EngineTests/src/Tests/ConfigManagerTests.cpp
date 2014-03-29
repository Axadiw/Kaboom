#include "ConfigManagerTests.h"
#include "Math/Vector2.h"
#include "Resources/Sprite.h"
#include "Basic/ProgressCounter.h"
#include <cmath>
#include <assert.h>
#include "includes.h"

void ConfigManagerTests::test1()
{
	static const char* filePath = "TestData\\configTest.txt";

	ConfigManager::getSingleton().createConfigFile(filePath);

	// zapisywanie w pliku wartoœci

	ConfigManager::getSingleton().setValue(filePath, "BOOLexist1", true);
	ConfigManager::getSingleton().setValue(filePath, "BOOLexist2", "exist2", true);
	ConfigManager::getSingleton().setValue(filePath, "BOOLexist3", "exist3", "exists3", true);

	ConfigManager::getSingleton().setValue(filePath, "INTexist1", 1);
	ConfigManager::getSingleton().setValue(filePath, "INTexist2", "exist2", 2);
	ConfigManager::getSingleton().setValue(filePath, "INTexist3", "exist3", "exists3", 3);

	ConfigManager::getSingleton().setValue(filePath, "FLOATexist1", 32.0f);
	ConfigManager::getSingleton().setValue(filePath, "FLOATexist2", "exist2", 33.0f);
	ConfigManager::getSingleton().setValue(filePath, "FLOATexist3", "exist3", "exists3", 34.0f);

	ConfigManager::getSingleton().setValue(filePath, "DOUBLEexist1", 64.0);
	ConfigManager::getSingleton().setValue(filePath, "DOUBLEexist2", "exist2", 65.0);
	ConfigManager::getSingleton().setValue(filePath, "DOUBLEexist3", "exist3", "exists3", 66.0);

	ConfigManager::getSingleton().setValue(filePath, "PCHARexist1", "text1");
	ConfigManager::getSingleton().setValue(filePath, "PCHARexist2", "exist2", "text2");
	ConfigManager::getSingleton().setValue(filePath, "PCHARexist3", "exist3", "exists3", "text3");

	ConfigManager::getSingleton().saveFile(filePath);
	ConfigManager::getSingleton().releaseConfigFile(filePath);
	
	ConfigManager::getSingleton().loadFile(filePath);

	bool b1,b2,b3;
	int i1,i2,i3;
	float f1,f2,f3;
	double d1,d2,d3;
	char c1[255],c2[255],c3[255];

	/// testy odczytu istniej¹cych wartoœci
	ConfigManager::getSingleton().getValueBool(&b1, filePath, "BOOLexist1");
	ConfigManager::getSingleton().getValueBool(&b2, filePath, "BOOLexist2", "exist2");
	ConfigManager::getSingleton().getValueBool(&b3, filePath, "BOOLexist3", "exist3", "exists3");

	assert(b1 == true && "Exist b1");
	assert(b2 == true && "Exist b2");
	assert(b3 == true && "Exist b3");

	ConfigManager::getSingleton().getValueInt(&i1, filePath, "INTexist1");
	ConfigManager::getSingleton().getValueInt(&i2, filePath, "INTexist2", "exist2");
	ConfigManager::getSingleton().getValueInt(&i3, filePath, "INTexist3", "exist3", "exists3");

	assert(i1 == 1 && "Exist i1");
	assert(i2 == 2 && "Exist i2");
	assert(i3 == 3 && "Exist i3");

	ConfigManager::getSingleton().getValueFloat(&f1, filePath, "FLOATexist1");
	ConfigManager::getSingleton().getValueFloat(&f2, filePath, "FLOATexist2", "exist2");
	ConfigManager::getSingleton().getValueFloat(&f3, filePath, "FLOATexist3", "exist3", "exists3");

	assert(f1 == 32.0f && "Exist f1");
	assert(f2 == 33.0f && "Exist f2");
	assert(f3 == 34.0f && "Exist f3");

	ConfigManager::getSingleton().getValueDouble(&d1, filePath, "DOUBLEexist1");
	ConfigManager::getSingleton().getValueDouble(&d2, filePath, "DOUBLEexist2", "exist2");
	ConfigManager::getSingleton().getValueDouble(&d3, filePath, "DOUBLEexist3", "exist3", "exists3");

	assert(d1 == 64.0 && "Exist d1");
	assert(d2 == 65.0 && "Exist d2");
	assert(d3 == 66.0 && "Exist d3");

	ConfigManager::getSingleton().getValuePChar(c1, filePath, "PCHARexist1" , "no_text1");
	ConfigManager::getSingleton().getValuePChar(c2, filePath, "PCHARexist2", "exist2", "no_text2");
	ConfigManager::getSingleton().getValuePChar(c3, filePath, "PCHARexist3", "exist3", "exists3", "no_text3");

	assert(!strcmp(c1, "text1") && "Exist c1");
	assert(!strcmp(c2, "text2") && "Exist c2");
	assert(!strcmp(c3, "text3") && "Exist c3");

	/// testy dla wartoœci nieistniej¹cych
	bool nb1,nb2,nb3;
	int ni1,ni2,ni3;
	float nf1,nf2,nf3;
	double nd1,nd2,nd3;
	char nc1[255],nc2[255],nc3[255];

	ConfigManager::getSingleton().getValueBool(&nb1, filePath, "BOOLnonexist1");
	ConfigManager::getSingleton().getValueBool(&nb2, filePath, "BOOLnonexist2", "nonexist2");
	ConfigManager::getSingleton().getValueBool(&nb3, filePath, "BOOLnonexist3", "nonexist3", "nonexists3");

	assert(nb1 == false && "nonexist nb1");
	assert(nb2 == false && "nonexist nb2");
	assert(nb3 == false && "nonexist nb3");

	ConfigManager::getSingleton().getValueInt(&ni1, filePath, "INTnonexist1", -1);
	ConfigManager::getSingleton().getValueInt(&ni2, filePath, "INTnonexist2", "nonexist2", -2);
	ConfigManager::getSingleton().getValueInt(&ni3, filePath, "INTnonexist3", "nonexist3", "nonexists3", -3);

	assert(ni1 == -1 && "nonexist ni1");
	assert(ni2 == -2 && "nonexist ni2");
	assert(ni3 == -3 && "nonexist ni3");

	ConfigManager::getSingleton().getValueFloat(&nf1, filePath, "FLOATnonexist1" , -1.0f);
	ConfigManager::getSingleton().getValueFloat(&nf2, filePath, "FLOATnonexist2", "nonexist2", -2.0f);
	ConfigManager::getSingleton().getValueFloat(&nf3, filePath, "FLOATnonexist3", "nonexist3", "nonexists3", -3.0f);

	assert(nf1 == -1.0f && "nonexist nf1");
	assert(nf2 == -2.0f && "nonexist nf2");
	assert(nf3 == -3.0f && "nonexist nf3");

	ConfigManager::getSingleton().getValueDouble(&nd1, filePath, "DOUBLEnonexist1", -11.0);
	ConfigManager::getSingleton().getValueDouble(&nd2, filePath, "DOUBLEnonexist2", "nonexist2", -12.0);
	ConfigManager::getSingleton().getValueDouble(&nd3, filePath, "DOUBLEnonexist3", "nonexist3", "nonexists3", -13.0);

	assert(nd1 == -11.0 && "nonexist nd1");
	assert(nd2 == -12.0 && "nonexist nd2");
	assert(nd3 == -13.0 && "nonexist nd3");

	ConfigManager::getSingleton().getValuePChar(nc1, filePath, "PCHARnonexist1" , "no_text1");
	ConfigManager::getSingleton().getValuePChar(nc2, filePath, "PCHARnonexist2", "nonexist2", "no_text2");
	ConfigManager::getSingleton().getValuePChar(nc3, filePath, "PCHARnonexist3", "nonexist3", "nonexists3", "no_text3");

	assert(!strcmp(nc1, "no_text1") && "nonexist nc1");
	assert(!strcmp(nc2, "no_text2") && "nonexist nc2");
	assert(!strcmp(nc3, "no_text3") && "nonexist nc3");


	ConfigManager::getSingleton().releaseConfigFile(filePath);
	AX_PRINT("ConfigFile test1 OK!\n");
}

void ConfigManagerTests::vectorTest()
{
	static const char* filePath = "TestData\\vectorTest.txt";

	ConfigManager::getSingleton().createConfigFile(filePath);

	// zapisywanie w pliku wartoœci

	ConfigManager::getSingleton().setValue(filePath, "position", "x", 1.0f);
	ConfigManager::getSingleton().setValue(filePath, "position", "y", 2.0f);

	ConfigManager::getSingleton().setValue(filePath, "object", "position", "x", 3.0f);
	ConfigManager::getSingleton().setValue(filePath, "object", "position", "y", 4.0f);

	ConfigManager::getSingleton().saveFile(filePath);
	ConfigManager::getSingleton().releaseConfigFile(filePath);

	ConfigManager::getSingleton().loadFile(filePath);

	Vector2 vec1, vec2, vec3;

	vec1.load(filePath, "position");
	vec2.load(filePath, "object", "position");
	vec3.load(filePath, "abcd");

	assert(vec1 == Vector2(1.0f, 2.0f));
	assert(vec2 == Vector2(3.0f, 4.0f));
	assert(vec3 == Vector2(0.0f, 0.0f));

	AX_PRINT("Vector test OK!\n");
	ConfigManager::getSingleton().releaseConfigFile(filePath);
}

void ConfigManagerTests::colorTest()
{
	static const char* filePath = "TestData\\colorTest.txt";

	ConfigManager::getSingleton().createConfigFile(filePath);

	// zapisywanie w pliku wartoœci

	ConfigManager::getSingleton().setValue(filePath, "color", "r", 1);
	ConfigManager::getSingleton().setValue(filePath, "color", "g", 2);
	ConfigManager::getSingleton().setValue(filePath, "color", "b", 3);
	ConfigManager::getSingleton().setValue(filePath, "color", "a", 4);

	ConfigManager::getSingleton().setValue(filePath, "object", "color", "r", 5);
	ConfigManager::getSingleton().setValue(filePath, "object", "color", "g", 6);
	ConfigManager::getSingleton().setValue(filePath, "object", "color", "b", 7);
	ConfigManager::getSingleton().setValue(filePath, "object", "color", "a", 8);

	ConfigManager::getSingleton().saveFile(filePath);
	ConfigManager::getSingleton().releaseConfigFile(filePath);

	ConfigManager::getSingleton().loadFile(filePath);

	Color c1, c2, c3;

	c1.load(filePath, "color");
	c2.load(filePath, "object", "color");
	c3.load(filePath, "abcd");

	assert(c1 == Color(1,2,3,4));
	assert(c2 == Color(5,6,7,8));
	assert(c3 == Color(255,255,255,255));

	AX_PRINT("Color test OK!\n");
	ConfigManager::getSingleton().releaseConfigFile(filePath);
}

void ConfigManagerTests::spriteTest()
{
	static const char* filePath = "TestData\\spriteTest.txt";

	ConfigManager::getSingleton().createConfigFile(filePath);

	// zapisywanie w pliku wartoœci

	ConfigManager::getSingleton().setValue(filePath, "sprite", "position", "x", 1.0f);
	ConfigManager::getSingleton().setValue(filePath, "sprite", "position", "y", 2.0f);

	ConfigManager::getSingleton().setValue(filePath, "sprite", "size", "x", 2.0f);
	ConfigManager::getSingleton().setValue(filePath, "sprite", "size", "y", 3.0f);

	ConfigManager::getSingleton().setValue(filePath, "sprite", "angle", 123.0f);
	ConfigManager::getSingleton().setValue(filePath, "sprite", "scale", 1.5f);

	ConfigManager::getSingleton().setValue(filePath, "sprite", "color", "r", 5);
	ConfigManager::getSingleton().setValue(filePath, "sprite", "color", "g", 6);
	ConfigManager::getSingleton().setValue(filePath, "sprite", "color", "b", 7);
	ConfigManager::getSingleton().setValue(filePath, "sprite", "color", "a", 8);

	ConfigManager::getSingleton().setValue(filePath, "sprite", "texCorner", "x", 0.0f);
	ConfigManager::getSingleton().setValue(filePath, "sprite", "texCorner", "y", 0.0f);
	ConfigManager::getSingleton().setValue(filePath, "sprite", "texSize", "x", 256.0f);
	ConfigManager::getSingleton().setValue(filePath, "sprite", "texSize", "y", 256.0f);
	ConfigManager::getSingleton().setValue(filePath, "sprite", "texDimensions", "x", 1024.0f);
	ConfigManager::getSingleton().setValue(filePath, "sprite", "texDimensions", "y", 1024.0f);

	ConfigManager::getSingleton().setValue(filePath, "sprite", "flipX", true);
	ConfigManager::getSingleton().setValue(filePath, "sprite", "flipY", true);

	ConfigManager::getSingleton().saveFile(filePath);
	ConfigManager::getSingleton().releaseConfigFile(filePath);

	ConfigManager::getSingleton().loadFile(filePath);

	Sprite sprite, sprite2;

	sprite.load(filePath, "sprite");
	sprite2.load(filePath, "sprite2");

	assert(sprite.getPos() == Vector2(1.0f, 2.0f));
	assert(sprite.getPos() == sprite.getOriginalPos());
	assert(sprite.getSize() == Vector2(2.0f, 3.0f));
	assert(sprite.getSize() == sprite.getOriginalSize());
	assert(sprite.getAngle() == 123.0f);
	assert(sprite.getAngle() == sprite.getOriginalAngle());
	assert(sprite.getScale() == 1.5f);
	assert(sprite.getScale() == sprite.getOriginalScale());
	assert(sprite.getColor() == Color(5,6,7,8));
	assert(sprite.getColor() == sprite.getOriginalColor());
	assert(sprite.textureCorner == Vector2(0.0f, 0.0f));
	assert(sprite.textureSize == Vector2(256.0f, 256.0f));
	assert(sprite.textureDimensions == Vector2(1024.0f, 1024.0f));
	assert(sprite.minU == 0.0f);
	assert(sprite.minV == 0.0f);
	assert(sprite.maxU == 0.25f);
	assert(sprite.maxV == 0.25f);
	assert(sprite.flipX == true);
	assert(sprite.flipY == true);

	assert(sprite2.getPos() == Vector2(0.0f, 0.0f));
	assert(sprite2.getPos() == sprite2.getOriginalPos());
	assert(sprite2.getSize() == sprite2.getOriginalSize());
	assert(sprite2.getAngle() == 0.0f);
	assert(sprite2.getAngle() == sprite2.getOriginalAngle());
	assert(sprite2.getScale() == 1.0f);
	assert(sprite2.getScale() == sprite2.getOriginalScale());
	assert(sprite2.getColor() == Color(255,255,255,255));
	assert(sprite2.getColor() == sprite2.getOriginalColor());
	assert(sprite2.textureCorner == Vector2(-1.0f, -1.0f));
	assert(sprite2.textureSize == Vector2(-1.0f, -1.0f));
	assert(sprite2.textureDimensions == Vector2(512.0f, 512.0f));
	assert(sprite2.minU == (-1.0f / 512.0f));
	assert(sprite2.minV == (-1.0f / 512.0f));
	assert(sprite2.maxU == (-2.0f / 512.0f));
	assert(sprite2.maxV == (-2.0f / 512.0f));
	assert(sprite2.flipX == false);
	assert(sprite2.flipY == false);

	AX_PRINT("Sprite test OK!\n");
	ConfigManager::getSingleton().releaseConfigFile(filePath);
}

void ConfigManagerTests::progressCounterTest()
{
	static const char* filePath = "TestData\\counterTest.txt";

	ConfigManager::getSingleton().createConfigFile(filePath);

	// zapisywanie w pliku wartoœci

	ConfigManager::getSingleton().setValue(filePath, "counter", "maxValue", 8.0f);
	ConfigManager::getSingleton().setValue(filePath, "counter", "minValue", 0.0f);
	ConfigManager::getSingleton().setValue(filePath, "counter", "speed", 2.0f);
	ConfigManager::getSingleton().setValue(filePath, "counter", "init", 1.0f);
	ConfigManager::getSingleton().setValue(filePath, "counter", "direction", UP);

	ConfigManager::getSingleton().setValue(filePath, "test", "counter2", "maxValue", 9.0f);
	ConfigManager::getSingleton().setValue(filePath, "test", "counter2", "minValue", 1.0f);
	ConfigManager::getSingleton().setValue(filePath, "test", "counter2", "speed", 3.0f);
	ConfigManager::getSingleton().setValue(filePath, "test", "counter2", "init", 2.0f);
	ConfigManager::getSingleton().setValue(filePath, "test", "counter2", "direction", DOWN);

	ConfigManager::getSingleton().saveFile(filePath);
	ConfigManager::getSingleton().releaseConfigFile(filePath);

	ConfigManager::getSingleton().loadFile(filePath);

	ProgressCounter counter, counter2;

	counter.load(filePath, "counter");
	counter2.load(filePath, "test", "counter2");

	assert(counter.direction == UP);
	assert(counter.maxValue == 8.0f);
	assert(counter.minValue == 0.0f);
	assert(counter.progress == 1.0f);
	assert(counter.speed == 2.0f);
	assert(counter2.direction == DOWN);
	assert(counter2.maxValue == 9.0f);
	assert(counter2.minValue == 1.0f);
	assert(counter2.progress == 2.0f);
	assert(counter2.speed == 3.0f);

	counter.updateLinear(20.0f);
	counter.updateLinear(20.0f);

	counter2.updateLinear(20.0f);
	counter2.updateLinear(20.0f);

	ConfigManager::getSingleton().releaseConfigFile(filePath);
	AX_PRINT("Progress Counter test OK!\n");
}