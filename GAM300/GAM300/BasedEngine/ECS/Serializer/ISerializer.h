/**********************************************************************************
* \file ISerializer.h
* \brief File contains the function definitions for all function that are used
         to serialize and deserialize the data to the file.
* \author Zhang Xinyu, 100% Code Contribution
*
* \copyright Copyright (C) 2022 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma
#include "Settings.h"
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

#ifndef JsonWriter_HEADER
#define JsonWriter_HEADER

class ISerializer
{
public:
    /* virtual bool DeserializeFromFile(const std::string& file);
     virtual bool SerializeToFile(const std::string& file);*/
    virtual void Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) = 0;// const { (void)(writer); return false; };
    virtual void Deserialize(const rapidjson::Document& doc, std::string id) = 0;
    virtual void Deserialize(const rapidjson::Value& obj) { (void)obj; }
    virtual ~ISerializer() = default;
};

class JsonWriter
{
public:
    rapidjson::StringBuffer* _buffer;
    rapidjson::PrettyWriter<rapidjson::StringBuffer>* _writer;

    JsonWriter();
    void Set3Decimal();
    void StartObject();
    void EndObject();
    void StartArray();
    void EndArray();
    const char* GetString() const;
    bool ParseFile(std::ifstream& ifs, rapidjson::Document& old_doc, const std::string& file);
    ~JsonWriter();

    template<typename type>
    inline void SerializeComponent(type& componentType)
    {
        componentType.Serialize(_writer);
    };

    template<typename type>
    inline void SerializeComponentPrefab(type& componentType, std::string prefabName)
    {
        componentType.SerializePrefab(_writer, prefabName);
    };
};
class JsonReader
{
public:
    rapidjson::Document _document;

    //void ReadBool(const char*, bool&, rapidjson::Value& obj);
    //void ReadInt(const char*, int&, rapidjson::Value& obj);
    //void ReadFloat(const char*, float&, rapidjson::Value& obj);
    //void ReadUint(const char*, uint32_t&, rapidjson::Value& obj);
    //void ReadString(const char*, std::string&, rapidjson::Value& obj);
    //void ReadVecctor3(const char*, Vector3&, rapidjson::Value& obj);
    //void ReadVecctor4(const char*, Vector4&, rapidjson::Value& obj);
    //void ReadQuaternion(const char*, Quat&, rapidjson::Value& obj);

    JsonReader();
    bool IsError(rapidjson::IStreamWrapper*);
    void StartObject(std::ifstream&);
    ~JsonReader();

    template<typename type>
    inline void DeserializeComponent(type& componentType,std::string id)
    {
        componentType.Deserialize(_document,id);
    };
};
#endif

template<typename T>
T Read2DVector(const rapidjson::Value& val);
template<typename T>
T Read3DVector(const rapidjson::Value& val);
template<typename T>
T Read4DVector(const rapidjson::Value& val);

void ReadBool(const char*, bool&, const rapidjson::Value& obj);
void ReadInt(const char*, int&, const rapidjson::Value& obj);
void ReadFloat(const char*, float&, const rapidjson::Value& obj);
void ReadUint(const char*, uint32_t&, const rapidjson::Value& obj);
void ReadString(const char*, std::string&, const rapidjson::Value& obj);
void ReadVecctor2(const char*, Vector2&, const rapidjson::Value& obj);
void ReadVecctor3(const char*, Vector3&, const rapidjson::Value& obj);
void ReadVecctor4(const char*, Vector4&, const rapidjson::Value& obj);
void ReadQuaternion(const char*, Quat&, const rapidjson::Value& obj);

void WriteBool(const char*, bool&, rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer);
void WriteInt(const char*, int&, rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer);
void WriteFloat(const char*, float&, rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer);
void WriteUint(const char*, uint32_t&, rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer);
void WriteString(const char*, std::string&, rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer);
void WriteVecctor2(const char*, Vector2&, rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer);
void WriteVecctor3(const char*, Vector3&, rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer);
void WriteVecctor4(const char*, Vector4&, rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer);
void WriteQuaternion(const char*, Quat&, rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer);

void Write2DVector(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer, Vec2D v);
void Write2DVector(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer, PVec2D v);
void Write2DVector(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer, GVec2D v);
void Write3DVector(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer, Vec3D v);
void Write3DVector(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer, PVec3D v);
void Write3DVector(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer, GVec3D v);
void Write4DVector(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer, Vec4D v);
void Write4DVector(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer, const PVec4D& v);
void Write4DVector(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer, GVec4D v);
void WriteQuat(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer, Quat v);
void WriteQuat(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer, PQuat v);
void WriteQuat(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer, GQuat v);

// AddType to add in the value from a json file
bool AddType(const rapidjson::Value& obj, std::string name, bool val);
unsigned AddType(const rapidjson::Value& obj, std::string name, unsigned val);
int AddType(const rapidjson::Value& obj, std::string name, int val);
double AddType(const rapidjson::Value& obj, std::string name, double val);
float AddType(const rapidjson::Value& obj, std::string name, float  val);
std::string AddType(const rapidjson::Value& obj, std::string name, std::string val);

Vec2D AddType(const rapidjson::Value& obj, std::string name, Vec2D val);
Vec3D AddType(const rapidjson::Value& obj, std::string name, Vec3D val);
Vec4D AddType(const rapidjson::Value& obj, std::string name, Vec4D val);
PVec2D AddType(const rapidjson::Value& obj, std::string name, PVec2D val);
PVec3D AddType(const rapidjson::Value& obj, std::string name, PVec3D val);
PVec4D AddType(const rapidjson::Value& obj, std::string name, PVec4D val);
GVec2D AddType(const rapidjson::Value& obj, std::string name, GVec2D val);
GVec3D AddType(const rapidjson::Value& obj, std::string name, GVec3D val);
GVec4D AddType(const rapidjson::Value& obj, std::string name, GVec4D val);
Quat AddType(const rapidjson::Value& obj, std::string name, Quat val);
GQuat AddType(const rapidjson::Value& obj, std::string name, GQuat val);
physx::PxQuat AddType(const rapidjson::Value& obj, std::string name, physx::PxQuat val);

bool SerializeToFile(const std::string& file);
bool DeserializeFromFile(const std::string& file);
void ConfigSerializer(const std::string& file, int& width, int& height);
void ConfigDeserializer(const std::string file, std::string& windowTitle, int& width, int& height, bool& vsync, bool& editorMode, std::string& sceneFile);
bool SerializePrefabToFile(const std::string& file, std::string prefabName);

bool SerializeWayPoint(const std::string& file, std::string waypoint_name);
bool DeserializeWayPoint(const std::string& file);