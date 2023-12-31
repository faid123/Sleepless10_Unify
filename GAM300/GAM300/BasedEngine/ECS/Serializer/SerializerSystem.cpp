#include "SerializerSystem.h"
#include "Engine.h"
#include <Camera/CameraSystem/CameraSystem.h>
#include <UserIntered/UI_System.h>
#include <Physics/PhysicsSystem.h>

std::string SerializerSystem::getFilePath()
{
	return filePath;
}

void SerializerSystem::setFileName(std::string name)
{
	fileName = name;
}

std::string SerializerSystem::getFileName()
{
	return fileName;
}

void SerializerSystem::setIsLoaded(bool loaded)
{
	isLoaded = loaded;
}

bool SerializerSystem::getIsLoaded()
{
	return isLoaded;
}

void SerializerSystem::setIsSaved(bool saved)
{
	isSaved = saved;
}

bool SerializerSystem::getIsSaved()
{
	return isSaved;
}

void SerializerSystem::createFile(const char* file)
{
	std::ofstream outfile(file);
	outfile.close();
}

bool SerializerSystem::renameFile(const char* _old, const char* _new)
{
	if (rename(_old, _new) != 0)
	{
		perror("Error moving file");
		return false;
	}
	else
		return true;
}

void SerializerSystem::callReset()
{
	setFileName("reset.scene");
	setIsLoaded(true);
	Engine::_ecs.getSystem<CameraSystem>()->_camType = CameraSystem::CameraType::EDITOR;
	Engine::_isPlay = false;
	Engine::_ecs.getSystem<UISystem>()->_isPaused = false;
	Engine::_ecs.getSystem<WindowSystem>()->setViewableCustor();	
	Engine::_ecs.getSystem<UISystem>()->activateAfterPause();
}

void SerializerSystem::init()
{
	
}
void SerializerSystem::load()
{
	filePath = "Resources/Scene/";
	DeserializeFromFile(filePath + fileName);
	setIsLoaded(false);
}
void SerializerSystem::update(float delta_time)
{
	(void)delta_time;

	// saving to file
	if (isSaved)
	{
		SerializeToFile(filePath + fileName);
		isSaved = false;
	}
	if (isLoaded)
	{
		unload();
		load();
	}

	
}

void SerializerSystem::unload()
{
	auto temp = Engine::_ecs.getActiveEntities();
	for (auto entity : temp)
	{
		Engine::_ecs.destroyEntity(entity._entityID);
	}
	temp = Engine::_ecs.getActiveEntities();
}
void SerializerSystem::release()
{

}