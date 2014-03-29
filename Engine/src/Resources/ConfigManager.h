#pragma once

#include "../Singleton.h"
#include <map>
#include <reader.h>
#include <writer.h>

#define DEFAULT_CONFIG_BOOL_VALUE false
#define DEFAULT_CONFIG_INT_VALUE -666
#define DEFAULT_CONFIG_FLOAT_VALUE -666.0f
#define DEFAULT_CONFIG_DOUBLE_VALUE -666.0

/*! 
 *  \brief     Menad¿er plików konfiguracyjncyh
 *  \author    Micha³ Mizera
 *  \version   1.0
 *  \date      2011-2012
 *  \copyright GNU Public License.
 */

class ConfigManager: public Singleton<ConfigManager>
{
public: 

	/*!
	Dodaje plik konfiguracyjny do wewnêtrznego spisu
	@param path œcie¿ka do pliku konfiguracyjnego
	*/
	int createConfigFile(const char* path);

	/// zwalnia  wczeœniej utworzony plik konfiguracyjny z pamiêci
	int releaseConfigFile(const char* path);

	/// czy mo¿na zapisywaæ do pliku (wykorzystywane podczas wielokrotnych zapisów)
	bool allowSavingToFile;

	/*!
	£aduje zadany plik konfiguracyjny z dysku
	@param path œcie¿ka do pliku konfiguracyjnego
	\details Funkcja sama sprawdza czy plik jest ju¿ w wewnêtrznym spisie plików (nie potrzeba wywy³ywaæ createConfigFile)
	*/
	int loadFile(const char* path);

	/*!
	Zapisuje zadany plik konfiguracyjny na dysk
	@param path œcie¿ka do pliku konfiguracyjnego	
	*/
	int saveFile(const char* path);	

	/// ustawia zadan¹ wartoœæ dla klucza (dla hierarchii 3 poziomowej)
	int setValue(const char* path, const char* name, const char* name2, const char* name3, Json::Value value);
	/// ustawia zadan¹ wartoœæ dla klucza (dla hierarchii 1 poziomowej)
	inline int setValue(const char* path, const char* name, Json::Value value) { return setValue(path, name, "", "", value); }; 
	/// ustawia zadan¹ wartoœæ dla klucza (dla hierarchii 2 poziomowej)
	inline int setValue(const char* path, const char* name, const char* name2, Json::Value value) { return setValue(path, name, name2, "", value); }; 	

	/// pobiera wartoœæ typu INT (dla hierarchii 3 poziomowej)
	void getValueInt(int* src, const char* path, const char* name, const char* name2, const char* name3, int default_value = DEFAULT_CONFIG_INT_VALUE);
	/// pobiera wartoœæ typu INT (dla hierarchii 1 poziomowej)
	inline void getValueInt(int* src, const char* path, const char* name, int default_value = DEFAULT_CONFIG_INT_VALUE) { getValueInt(src, path, name, "", "", default_value); }; 
	/// pobiera wartoœæ typu INT (dla hierarchii 2 poziomowej)
	inline void getValueInt(int* src, const char* path, const char* name, const char* name2, int default_value = DEFAULT_CONFIG_INT_VALUE) { getValueInt(src, path, name, name2, "", default_value); }; 
	
	/// pobiera wartoœæ typu DOUBLE (dla hierarchii 3 poziomowej)
	void getValueDouble(double* src, const char* path, const char* name, const char* name2, const char* name3, double default_value = DEFAULT_CONFIG_DOUBLE_VALUE);
	/// pobiera wartoœæ typu DOUBLE (dla hierarchii 1 poziomowej)
	inline void getValueDouble(double* src, const char* path, const char* name, double default_value = DEFAULT_CONFIG_DOUBLE_VALUE) { getValueDouble(src, path, name, "", "", default_value); }; 
	/// pobiera wartoœæ typu DOUBLE (dla hierarchii 2 poziomowej)
	inline void getValueDouble(double* src, const char* path, const char* name, const char* name2, double default_value = DEFAULT_CONFIG_DOUBLE_VALUE) { getValueDouble(src, path, name, name2, "", default_value); }; 	

	/// pobiera wartoœæ typu FLOAT (dla hierarchii 3 poziomowej)	
	void getValueFloat(float* src, const char* path, const char* name, const char* name2, const char* name3, float default_value = DEFAULT_CONFIG_FLOAT_VALUE);
	/// pobiera wartoœæ typu FLOAT (dla hierarchii 1 poziomowej)	
	inline void getValueFloat(float* src, const char* path, const char* name, float default_value = DEFAULT_CONFIG_FLOAT_VALUE) { getValueFloat(src, path, name, "", "", default_value); }; 
	/// pobiera wartoœæ typu FLOAT (dla hierarchii 2 poziomowej)	
	inline void getValueFloat(float* src, const char* path, const char* name, const char* name2, float default_value = DEFAULT_CONFIG_FLOAT_VALUE) { getValueFloat(src, path, name, name2, "", default_value); }; 	

	/// pobiera wartoœæ typu CHAR* (dla hierarchii 3 poziomowej)
	/// \details wymaga uprzedniego zaalokowania odpowiedniego obszaru pamiêci dla *src
	inline void getValuePChar(char* src, const char* path, const char* name, const char* name2, const char* default_value) { getValuePChar(src, path, name, name2, "", default_value); }; 
	/// pobiera wartoœæ typu CHAR* (dla hierarchii 1 poziomowej)
	/// \details wymaga uprzedniego zaalokowania odpowiedniego obszaru pamiêci dla *src
	inline void getValuePChar(char* src, const char* path, const char* name, const char* default_value) { getValuePChar(src, path, name, "", "", default_value); }; 	
	/// pobiera wartoœæ typu CHAR* (dla hierarchii 2 poziomowej)
	/// \details wymaga uprzedniego zaalokowania odpowiedniego obszaru pamiêci dla *src
	void getValuePChar(char* src, const char* path, const char* name, const char* name2, const char* name3, const char* default_value);

	/// pobiera wartoœæ typu BOOL (dla hierarchii 3 poziomowej)
	void getValueBool(bool* src, const char* path, const char* name, const char* name2, const char* name3, bool default_value = DEFAULT_CONFIG_BOOL_VALUE);
	/// pobiera wartoœæ typu BOOL (dla hierarchii 1 poziomowej)
	inline void getValueBool(bool* src, const char* path, const char* name, bool default_value = DEFAULT_CONFIG_BOOL_VALUE) { getValueBool(src, path, name, "", "", default_value); }; 
	/// pobiera wartoœæ typu BOOL (dla hierarchii 2 poziomowej)
	inline void getValueBool(bool* src, const char* path, const char* name, const char* name2, bool default_value = DEFAULT_CONFIG_BOOL_VALUE) { getValueBool(src, path, name, name2, "", default_value); }; 	
private:
	/// przechowuje zawartoœæ przetwarzanych plików konfiguracyjnych
	std::map<const char*, Json::Value> data;

	Json::Reader reader;
	Json::StyledWriter writer;
};