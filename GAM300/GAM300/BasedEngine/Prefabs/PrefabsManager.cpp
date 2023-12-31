/**********************************************************************************
* \file PrefabsManager.cpp
* \brief File contains the function definitions for all functions that are used
		 to serialize & deserialize prefabs.
* \author Shaikh Faid Bin Omar, 100% Code Contribution
*
* \copyright Copyright (C) 2022 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#include "PrefabsManager.h"
#include <Serializer/SerializerSystem.h>

void Prefab::Deserialize(const rapidjson::Value& obj)
{
	//_prefabID = (PrefabID)obj["ID"].GetInt();
	_prefabName = obj["Name"].GetString();

	for (rapidjson::SizeType i = 0; i < obj["Tags"].Size(); ++i)
	{
		_tags.insert(obj["Tags"][i].GetString());
	}
	/* std::cout << "PrefabID: " << _prefabID << " successfully deserialized "
		 << std::endl;*/

}

void Prefab::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer)
{
	std::string s = std::to_string(_prefabID);
	char const* pchar = s.c_str();
	writer->String(pchar);

	writer->StartObject();
	writer->String("ID");
	writer->Int(_prefabID);
	writer->String("Name");
	writer->String(_prefabName.c_str());
	//writer->String("ParentID");
	//writer->Int(_parentID);
	//writer->String("isActive");
	//writer->Bool(_isActive);

	writer->String("Tags");
	writer->StartArray();
	writer->SetFormatOptions(rapidjson::kFormatSingleLineArray);
	for (std::set<std::string>::iterator it = _tags.begin(); it != _tags.end(); ++it)
		writer->String((*it).c_str());
	writer->EndArray();
	writer->SetFormatOptions(rapidjson::kFormatDefault);

	//writer->EndObject();

}
