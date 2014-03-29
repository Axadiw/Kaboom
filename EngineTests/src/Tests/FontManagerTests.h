#pragma once

#include "Singleton.h"
#include "Resources/ConfigManager.h"
#include "Resources/Text.h"

class FontManagerTests: public Singleton<FontManagerTests>
{
public:
	void textTest();
	void memoryLeakstTestStart();
	void memoryLeakstTestEnd();
private:
	Font** fonts;	
};