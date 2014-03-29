#include "ParticleSystemManager.h"
#include "../includes.h"

ParticleSystem* ParticleSystemManager::loadSystem(const char* path)
{
	if(systems.find(path) != systems.end())
	{
		// font juz otwarty, zwieksz licznik odwo³añ
		(systemsCount[path])++;

		return systems[path];
	}

	ParticleSystem* retSystem = new ParticleSystem();
	retSystem->load(path);

	retSystem->name = path;

	systems[path] = retSystem;
	systemsCount[path] = 1;

	return retSystem;
}

void ParticleSystemManager::releaseSystem(ParticleSystem* system)
{
	stdext::hash_map<const char*, ParticleSystem*>::iterator itSystem = systems.find(system->name);
	stdext::hash_map<const char*, int>::iterator itCount = systemsCount.find(system->name);

	if(itSystem == systems.end() || itCount == systemsCount.end())
	{
		// nie ma takiego fonta w bazie, lub font jest pusty
		return;
	}

	(itCount->second)--;

	if(itCount->second <= 0)
	{
		delete itSystem->second;

		systems.erase(itSystem);
		systemsCount.erase(itCount);
	}
}