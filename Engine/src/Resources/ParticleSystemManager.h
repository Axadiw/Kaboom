#pragma once

#include "../Singleton.h"
#include "ParticleSystem.h"
#include <hash_map>

/*! 
 *  \brief     Menad¿er systemów efektów cz¹steczkowych
 *  \author    Micha³ Mizera
 *  \version   1.0
 *  \date      2011-2012
 *  \copyright GNU Public License.
 */

class ParticleSystemManager: public Singleton<ParticleSystemManager>
{
public: 
	/*!
	³aduje system cz¹teczkowy z zadanego adresu
	@param name adres pliku opisuj¹cego dany system cz¹steczkowy na dysku
	\return adres do za³adowanego systemu , w przypadku b³êdu NULL
	\details w przypadku próbu za³adowania systemu wielokrotnie, system jest ³adowany do pamiêci tylko raz, wzrasta jedynie wewnêtrzny licznik odwo³añ
	*/
	ParticleSystem* loadSystem(const char* name);

	/*! zwalnia zasoby dla zadanej czcionki
	\details zmniejsza licznik odwo³añ, i w przypadku braku odwo³añ do czcionki, nastêpuje usuniêcie zasobów z pamiêci
	*/
	void releaseSystem(ParticleSystem*);

private:
	stdext::hash_map<const char*, ParticleSystem*> systems;
	stdext::hash_map<const char*, int> systemsCount;

};