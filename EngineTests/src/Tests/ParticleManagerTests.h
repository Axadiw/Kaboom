#pragma once

#include "Singleton.h"
#include "Resources/ParticleSystemManager.h"
#include "Resources/TextureManager.h" 
#include "Resources/FontManager.h"
#include "Resources/Text.h"

class ParticleManagerTests: public Singleton<ParticleManagerTests>
{
public:
	void test1();
private:
	Texture* white;	
	Texture* particlesTex;	

	Font* fontTygodnik;	
};
