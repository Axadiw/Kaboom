#pragma once

#include "Singleton.h"
#include "Resources/ConfigManager.h"

class ConfigManagerTests: public Singleton<ConfigManagerTests>
{
public:
	void test1();

	void spriteTest();
	void vectorTest();
	void colorTest();
	void progressCounterTest();
};