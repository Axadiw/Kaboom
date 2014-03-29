#include "Sprite.h"
#include "ConfigManager.h"
#include "ParticleSystem.h"
#include "PolyMTLib.h"

#pragma comment (lib, "SDL.lib")
#pragma comment (lib, "SDLmain.lib")
#pragma comment (lib, "opengl32.lib")

/// ile ró¿nych barw przejœciowych ma byæ wykorzystywane przez particle
#define COLORS_RESOLUTION 100

ParticleSystem::~ParticleSystem()
{
	for(int i=colorPairsNum-1; i>=0; --i)
	{
		for(int j=0; j<=COLORS_RESOLUTION; j++)
		{
			delete colors[i][j];
		}
		delete colors[i];
	}
}
	

void ParticleSystem::load(const char* path)
{
	char buffer[255];
	int reserveAmount;
	float middlePoint;
	float progress;	

	ConfigManager::getSingleton().loadFile(path);	

	ConfigManager::getSingleton().getValueInt(&colorPairsNum, path, "colorPairsNum", 0);
	ConfigManager::getSingleton().getValueInt(&reserveAmount, path, "reservreParticles", 1000);
	ConfigManager::getSingleton().getValueInt(&defaultAmount, path, "defaultAmount", 100);
	ConfigManager::getSingleton().getValueInt(&maximumInactiveParticles, path, "maximum_inactive_particles", 1000);
	ConfigManager::getSingleton().getValueFloat(&maximumAge, path, "maximumAge", 10.0f);
	ConfigManager::getSingleton().getValueFloat(&friction, path, "friction", 1.0f);

	angleRange.load(path, "angle_range", Vector2(0.0f, 1.0f));
	angleRange *= -M_PI/180.0f;

	gravity.load(path, "gravity", Vector2(0.0f, -1.0f));
	sprite.load(path, "sprite");
	startingVelocity.load(path, "startingVelocity", Vector2(0.0f, 1.0f));

	Color startColor, middleColor, endColor;
	Color* tmpColor;
	for(int i=0; i<colorPairsNum; ++i)
	{
		sprintf(buffer, "color_pair_%d", i);
		startColor.load(path, buffer, "startColor");
		middleColor.load(path, buffer, "middleColor");
		endColor.load(path, buffer, "endColor");
		ConfigManager::getSingleton().getValueFloat(&middlePoint, path , buffer, "middlePoint", 0.5f);
		
		Color** colorsTable = new Color*[COLORS_RESOLUTION+1];

		for(int j=0; j<=COLORS_RESOLUTION; j++)
		{
			tmpColor = new Color;
			progress = static_cast<float>(j)/static_cast<float>(COLORS_RESOLUTION);

			if(progress < middlePoint)
			{
				*tmpColor = lerp(startColor, middleColor, progress/middlePoint);				
			}
			else
			{
				*tmpColor = lerp(middleColor, endColor, (progress - middlePoint)/(1.0f - middlePoint));				
			}	

			colorsTable[j] = tmpColor; 
		}

		colors.push_back(colorsTable);
	}

	particles.reserve(reserveAmount);

	ConfigManager::getSingleton().releaseConfigFile(path);
}

void ParticleSystem::spawn(Vector2 position, int amount)
{
	Vector2 velocity;
	for(int i=0; i<amount; ++i)
	{
		velocity = Vector2(0.0f, fastrandFloat(startingVelocity.x, startingVelocity.y));
		velocity.rotate(fastrandFloat(angleRange.x, angleRange.y));

		particles.push_back(Particle(position, velocity, fastrandInt()%colorPairsNum));
	}
}

void ParticleSystem::updateAndDraw(float dt)
{
	inactiveParticles = 0;
	int particlesNum = particles.size();
	for(int i = 0 ; i<particlesNum; ++i)
	{
		if(!particles[i].isActive)
		{
			inactiveParticles++;
			continue;
		}

		particles[i].velocity = (particles[i].velocity*friction + gravity);
		particles[i].position += particles[i].velocity*dt;
		particles[i].age += dt;

		if(particles[i].age > maximumAge)
		{
			particles[i].isActive = false;
			continue;
		}

		sprite.draw(particles[i].position, *(colors[particles[i].color][static_cast<int>(COLORS_RESOLUTION*particles[i].age/maximumAge)]));
	}

	if(inactiveParticles > maximumInactiveParticles)
	{
		particles.erase(particles.begin(), particles.begin() + inactiveParticles);
	}
}