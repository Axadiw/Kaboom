#pragma once

/*! 
 *  \brief     Szablonowa klasa Singletona
 *  \details Wykorzystywanie podczas wy�wietlania animacji
 *  \author    Micha� Mizera
 *  \version   1.0
 *  \date      2011-2012
 *  \copyright GNU Public License.
 */

template <typename T>
class Singleton 
{
	Singleton(const Singleton&);
	Singleton& operator = (const Singleton&);
protected:
	static T* pSingleton;

	Singleton() {}
	virtual ~Singleton() {}
public:
	/// zwraca referencj� na obiekt
	static T& getSingleton() { return *pSingleton; }

	/// zwraca wska�nik do obiektu
	static T* getSingletonPtr() { return pSingleton; }

	/// zwraca true, je�eli singleton zosta� utworzony
	static bool isSingletonCreated() { return pSingleton != 0; }

	/// tworzy signleton
	static void createSingleton() 
	{
		if (!pSingleton) 
		{
			pSingleton = new T;
		}
	}

	/// zwalnia obiekt singletonu
	static void removeSingleton() 
	{
		delete pSingleton;
		pSingleton = 0;
	}	
};

template <typename T> T* Singleton<T>::pSingleton = 0;