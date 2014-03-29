#pragma once

#include "../Singleton.h"
#include "ParticleSystem.h"
#include <hash_map>

/*! 
 *  \brief     Menad�er system�w efekt�w cz�steczkowych
 *  \author    Micha� Mizera
 *  \version   1.0
 *  \date      2011-2012
 *  \copyright GNU Public License.
 */

class ParticleSystemManager: public Singleton<ParticleSystemManager>
{
public: 
	/*!
	�aduje system cz�teczkowy z zadanego adresu
	@param name adres pliku opisuj�cego dany system cz�steczkowy na dysku
	\return adres do za�adowanego systemu , w przypadku b��du NULL
	\details w przypadku pr�bu za�adowania systemu wielokrotnie, system jest �adowany do pami�ci tylko raz, wzrasta jedynie wewn�trzny licznik odwo�a�
	*/
	ParticleSystem* loadSystem(const char* name);

	/*! zwalnia zasoby dla zadanej czcionki
	\details zmniejsza licznik odwo�a�, i w przypadku braku odwo�a� do czcionki, nast�puje usuni�cie zasob�w z pami�ci
	*/
	void releaseSystem(ParticleSystem*);

private:
	stdext::hash_map<const char*, ParticleSystem*> systems;
	stdext::hash_map<const char*, int> systemsCount;

};