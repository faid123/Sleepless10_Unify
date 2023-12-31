/**********************************************************************************
* \file EntitySerializer.cpp
* \brief File contains the function definitions for all function that are used
         to serialize and deserialize entity data.
* \author Zhang Xinyu, 100% Code Contribution
*
* \copyright Copyright (C) 2022 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#include "Entity/Entity.h"
#include "ISerializer.h"
#include "Engine.h"

void Entity::Deserialize(const rapidjson::Value& obj)
{
    _entityName = obj["Name"].GetString();
    _parentID = (EntityID)obj["ParentID"].GetInt();
    _isActive = obj["isActive"].GetBool();

    for (rapidjson::SizeType i = 0; i < obj["Tags"].Size(); ++i)
    {
        _tags.insert(obj["Tags"][i].GetString());
        Engine::_ecs.createNewTag(obj["Tags"][i].GetString());
    }
    /* std::cout << "EntityID: " << _entityID << " successfully deserialized "
         << std::endl;*/
}

void Entity::SerializePrefab(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer)
{
    std::string s = std::to_string(_entityID);
    char const* pchar = s.c_str();
    std::string name = "Prefab_";
    writer->String(pchar);

    writer->StartObject();
    writer->String("ID");
    writer->Int(_entityID);
    writer->String("Name");
    name += _entityName;
    writer->String(name.c_str());
    writer->String("ParentID");
    writer->Int(_parentID);
    writer->String("isActive");
    writer->Bool(_isActive);

    writer->String("Tags");
    writer->StartArray();
    writer->SetFormatOptions(rapidjson::kFormatSingleLineArray);
    for (std::set<std::string>::iterator it = _tags.begin(); it != _tags.end(); ++it)
        writer->String((*it).c_str());
    writer->EndArray();
    writer->SetFormatOptions(rapidjson::kFormatDefault);

    //writer->EndObject();
}

void Entity::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer)
{

    std::string s = std::to_string(_entityID);
    char const* pchar = s.c_str();
    writer->String(pchar);

    writer->StartObject();
    writer->String("Name");
    writer->String(_entityName.c_str());
    writer->String("ParentID");
    writer->Int(_parentID);
    writer->String("isActive");
    writer->Bool(_isActive);

    writer->String("Tags");
    writer->StartArray();
    writer->SetFormatOptions(rapidjson::kFormatSingleLineArray);
    for (std::set<std::string>::iterator it = _tags.begin(); it != _tags.end(); ++it)
        writer->String((*it).c_str());
    writer->EndArray();
    writer->SetFormatOptions(rapidjson::kFormatDefault);

    //writer->EndObject();
}
void EntityManager::Deserialize(const rapidjson::Document& doc, std::string id)
{
    //UNREFERENCED_PARAMETER(id);

    if (doc.HasMember("EntityManager") && doc["EntityManager"].IsArray())
    {
        for (rapidjson::SizeType i = 0; i < doc["EntityManager"].Size(); ++i)
        {
            const rapidjson::Value& em = doc["EntityManager"][i];
            //create a tamp entity
            EntityID e = Engine::_ecs.createNewEntity("Temp");
            //load the saved data from json
            Engine::_ecs.getEntity(e).Deserialize(em);
        }
    }
    else
    {
        std::cout << "EntityManager: no input for it!"
            << std::endl;
    }
}


void EntityManager::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer)
{

    writer->Key("EntityManager");
    writer->StartArray();

    auto allEntities = getActiveEntities();
    for (auto it = allEntities.begin();
        it != allEntities.end(); it++)
    {
        (*it).Serialize(writer);

    }
    writer->EndArray();

}
