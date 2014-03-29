#include "ConfigManager.h"
#include "../includes.h"
#include <fstream>
#include <iostream>
#include <string>

int ConfigManager::setValue(const char* path, const char* name, const char* name2, const char* name3, Json::Value value)
{
	if(data.find(path) == data.end())
	{
		// nie otworzono takiego pliku
		return -1;
	}

	if(!strcmp(name2, "") && !strcmp(name3, ""))
	{
		// bez zagnie¿d¿ania
		data[path][name] = value;
	}
	else if(!strcmp(name3, ""))
	{
		data[path][name][name2] = value;
	}
	else
	{
		data[path][name][name2][name3] = value;
	}

	return 0;
}


void ConfigManager::getValueInt(int* src, const char* path, const char* name, const char* name2, const char* name3, int default_value)
{
	if(!strcmp(name2, "") && !strcmp(name3, ""))
	{
		// bez zagnie¿d¿ania
		*src = (data[path][name] != Json::Value::null)?data[path][name].asInt():default_value;
	}
	else if(!strcmp(name3, ""))
	{
		// 2 level
		*src = (data[path][name][name2] != Json::Value::null)?data[path][name][name2].asInt():default_value;
	}
	else
	{
		// 3 levele
		*src = (data[path][name][name2][name3] != Json::Value::null)?data[path][name][name2][name3].asInt():default_value;
	}
}

void ConfigManager::getValueDouble(double* src, const char* path, const char* name, const char* name2, const char* name3, double default_value)
{
	if(!strcmp(name2, "") && !strcmp(name3, ""))
	{
		// bez zagnie¿d¿ania
		*src = (data[path][name] != Json::Value::null)?data[path][name].asDouble():default_value;
	}
	else if(!strcmp(name3, ""))
	{
		// 2 level
		*src = (data[path][name][name2] != Json::Value::null)?data[path][name][name2].asDouble():default_value;
	}
	else
	{
		// 3 levele
		*src = (data[path][name][name2][name3] != Json::Value::null)?data[path][name][name2][name3].asDouble():default_value;
	}
}

void ConfigManager::getValueFloat(float* src, const char* path, const char* name, const char* name2, const char* name3, float default_value)
{
	if(!strcmp(name2, "") && !strcmp(name3, ""))
	{
		// bez zagnie¿d¿ania
		*src = static_cast<float>((data[path][name] != Json::Value::null)?data[path][name].asDouble():default_value);
	}
	else if(!strcmp(name3, ""))
	{
		// 2 level
		*src = static_cast<float>((data[path][name][name2] != Json::Value::null)?data[path][name][name2].asDouble():default_value);
	}
	else
	{
		// 3 levele
		*src = static_cast<float>((data[path][name][name2][name3] != Json::Value::null)?data[path][name][name2][name3].asDouble():default_value);
	}
}

void ConfigManager::getValuePChar(char* src, const char* path, const char* name, const char* name2, const char* name3,const char* default_value)
{
	if(!strcmp(name2, "") && !strcmp(name3, ""))
	{
		// bez zagnie¿d¿ania
		strcpy(src, (data[path][name] != Json::Value::null)?data[path][name].asCString():default_value); 
	}
	else if(!strcmp(name3, ""))
	{
		// 2 level
		strcpy(src, (data[path][name][name2] != Json::Value::null)?data[path][name][name2].asCString():default_value); 
	}
	else
	{
		// 3 levele
		strcpy(src, (data[path][name][name2][name3] != Json::Value::null)?data[path][name][name2][name3].asCString():default_value); 
	}
}

void ConfigManager::getValueBool(bool* src, const char* path, const char* name, const char* name2,  const char* name3, bool default_value)
{
	if(!strcmp(name2, "") && !strcmp(name3, ""))
	{
		// bez zagnie¿d¿ania
		*src = (data[path][name] != Json::Value::null)?data[path][name].asBool():default_value;
	}
	else if(!strcmp(name3, ""))
	{
		// 2 level
		Json::Value tmp = data[path][name][name2];
		*src = (data[path][name][name2] != Json::Value::null)?data[path][name][name2].asBool():default_value;
	}
	else
	{
		// 3 levele
		*src = (data[path][name][name2][name3] != Json::Value::null)?data[path][name][name2][name3].asBool():default_value;
	}
}

int ConfigManager::createConfigFile(const char* path)
{	
	if(data.find(path) != data.end())
	{
		// ju¿ otworzono taki plik

		return -1;
	}

	data[path] = Json::Value();

	return 0;
}

int ConfigManager::releaseConfigFile(const char* path)
{
	std::map<const char*, Json::Value>::iterator it = data.find(path);

	if(it == data.end())
	{
		// nie ma otwartego takiego pliku
		return -1;
	}

	data.erase(it);

	return 0;
}

int ConfigManager::loadFile(const char* path)
{
	createConfigFile(path);

	std::string buf;
	std::string line;
	std::ifstream in(path);
	while(std::getline(in,line))
	{
		buf += line;
		buf += '\n';
	}

	in.close();

	bool parsingSuccessful = reader.parse( buf, data[path] );
	if ( !parsingSuccessful )
	{				
		std::cout  << "Failed to parse configuration\n"<< reader.getFormatedErrorMessages();

		return -2;
	}

	return 0;
}

int ConfigManager::saveFile(const char* path)
{
	if(!allowSavingToFile)
		return -2;

	if(data.find(path) == data.end())
	{
		// nie otwarto takiego pliku

		return -1;
	}

	std::string outputConfig = writer.write( data[path] );

	std::ofstream myfile;
	myfile.open (path);
	myfile << outputConfig;
	myfile.close();

	return 0;
}