#pragma once

#include "Singleton.h"
#include "Resources/SoundManager.h"

class SoundManagerTests: public Singleton<SoundManagerTests>
{
public:
	void memoryLeakstTestStart();
	void memoryLeakstTestEnd();
	void test1();
private:
	Sound** musicTab;
	Sound** sfxTab;
};