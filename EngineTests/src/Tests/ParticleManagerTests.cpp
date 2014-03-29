#include "ParticleManagerTests.h"

/// engine
#include "Math/Vector2.h"
#include "GameEngine.h"
#include "includes.h"

#define MAXIMUM_DT 0.2f
#define FRAMERATE 60


LARGE_INTEGER uFreq;
bool bUseQPC = (QueryPerformanceFrequency (&uFreq) != 0);

float GetSecs()
{
   if (!bUseQPC)   return GetTickCount() / 1000.0f;
   else
   {
      LARGE_INTEGER uTicks;
      QueryPerformanceCounter (&uTicks);
      return (float)(uTicks.QuadPart / (double)uFreq.QuadPart);
   }
}

void ParticleManagerTests::test1()
{
	static const char* fileParticleSystem = "TestData\\testParticleSystem.txt";
	ParticleSystem* system;

	GameEngine* engine = GameEngine::getSingletonPtr();

	float fTime = 0.0f, fNewTime = 0.0f, fFPSTime = 0.0f;
	unsigned uFrames = 0;
	engine->dt = 1.0f/FRAMERATE;

	Uint32 before;
	int delay;
	bool running = true;

	engine->fullscreen = false;
	engine->screenDimensions = Vector2(800.0f, 600.0f);				
	engine->Init("Particle tests");

	fontTygodnik = FontManager::getSingleton().loadFont("Data\\Tygodnik.ttf");
	white = TextureManager::getSingleton().loadTexture("Data\\white.bmp");	
	particlesTex = TextureManager::getSingleton().loadTexture("Data\\particlesTex.png");	
	system = ParticleSystemManager::getSingleton().loadSystem(fileParticleSystem);

	Text particlesNum;
	particlesNum.setAngle(0.0f);
	particlesNum.setPos(100.0, -90.0f);
	particlesNum.setScale(10.0f);
	particlesNum.setColor(255,255,255,255);
	particlesNum.isActive = true;

	while(running)
	{
		before = SDL_GetTicks();
		SDL_Event event;

		if (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) {
				case SDLK_ESCAPE:
					running = false;
					break;
				case SDLK_r:
					ParticleSystemManager::getSingleton().releaseSystem(system);
					system = ParticleSystemManager::getSingleton().loadSystem(fileParticleSystem);
					break;
				}
			}
		}
		glClear( GL_COLOR_BUFFER_BIT );

		system->spawn(Vector2(0.0f, 0.0f), system->defaultAmount);

		particlesTex->bind();
		system->updateAndDraw(engine->dt);

		white->bind();
		sprintf(particlesNum.text, "%d", system->getVisibleParticlesNum());
		//particlesNum.draw(fontTygodnik);

		SDL_GL_SwapBuffers();
		
		fNewTime = GetSecs();
		engine->dt = ((fNewTime - fTime) < MAXIMUM_DT)?fNewTime - fTime:MAXIMUM_DT;
		fTime = fNewTime;

		/// LICZENIE FPS
		++uFrames;
		fFPSTime += engine->dt;
		if (fFPSTime>= 1.0f)
		{
			engine->FPS = uFrames / fFPSTime;
			uFrames = 0;
			fFPSTime = 0.0f;
		}

		delay = before + 1000/FRAMERATE - SDL_GetTicks();
		if(delay > 0) SDL_Delay(delay);

	}

	FontManager::getSingleton().releaseFont(fontTygodnik);
	TextureManager::getSingleton().releaseTexture(white);
	TextureManager::getSingleton().releaseTexture(particlesTex);
	ParticleSystemManager::getSingleton().releaseSystem(system);

	engine->Cleanup();
}
