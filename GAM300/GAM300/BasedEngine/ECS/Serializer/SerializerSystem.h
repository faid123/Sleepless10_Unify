/**********************************************************************************
* \file SerializerSystem.h
* \brief File contains the function definitions for all function that are used
		 to serialize and deserialize the data to the file.
* \author Zhang Xinyu, 100% Code Contribution
*
* \copyright Copyright (C) 2022 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once

#include <System/System.h>
#include <iostream>
#include <fstream>  
#include <cstdio>
#include "Serializer/ISerializer.h"
//#include <GL/glew.h>
//#include <GLFW/glfw3.h>
#define SOL_CHECK_ARGUMENTS 1
#include "sol/sol.hpp"

class SerializerSystem : public ISystem
{
	std::string filePath;
	std::string fileName = "Final_Level3.scene";
	std::string _SystemName;
	bool isLoaded = false;
	bool isSaved = false;
public:
	
	void init();
	void load();
	void update(float delta_time);
	void unload();
	void release();

	std::string getFilePath();
	void setFileName(std::string name);
	std::string getFileName();
	void setIsLoaded(bool loaded);
	bool getIsLoaded();
	void setIsSaved(bool saved);
	bool getIsSaved();
	void createFile(const char*);
	bool renameFile(const char* _old, const char* _new);
	void callReset();
	static void RegisterLua(sol::state& lua);
};