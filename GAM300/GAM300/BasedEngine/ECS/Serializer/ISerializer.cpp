#include "ISerializer.h"
#include <glm/gtc/quaternion.hpp>
#include <foundation/PxVec4.h>
#include "../Engine.h"
#include <ResourceManager/ResourceManager.h>

using namespace rapidjson;
JsonWriter::JsonWriter()
{

   // StringBuffer _buffer;
    //PrettyWriter<StringBuffer> writer(buffer);
      _buffer = new rapidjson::StringBuffer;
      _writer = new rapidjson::PrettyWriter<rapidjson::StringBuffer> (*_buffer);
}
void JsonWriter::Set3Decimal()
{
    _writer->SetMaxDecimalPlaces(3);
}
void JsonWriter::StartObject()
{
    _writer->StartObject();
//    return *this;
}
void JsonWriter::EndObject()
{
    _writer->EndObject();
   // return *this;
}
void JsonWriter::StartArray()
{
    _writer->StartArray();
    _writer->SetFormatOptions(kFormatSingleLineArray);
}
void JsonWriter::EndArray()
{
    _writer->EndArray();
    _writer->SetFormatOptions(kFormatDefault);
}
const char* JsonWriter::GetString() const
{
    return _buffer->GetString();
}
bool JsonWriter::ParseFile(std::ifstream& ifs, Document& old_doc, const std::string& file)
{
    if (!ifs)
    {
        std::cout << "File " << file << "is created for serialiser" << std::endl;
    }
    else if (ifs.peek() == std::ifstream::traits_type::eof())
    {
        std::cout << "Empty file: " << file << std::endl;
    }
    else if (!ifs.is_open())
    {
        std::cout << "Could not open file " << file << " for reading!" << std::endl;
        return false;
    }
    else
    {
        //Save the original data to DOM
        IStreamWrapper isw(ifs);
        bool IsError = old_doc.ParseStream(isw).HasParseError() ? true : false;
        (void)IsError;
#ifdef _DEBUG           
        if (IsError)
        {
            std::cout << "Parse Error occured for file: " << file << std::endl;
        }
#endif        
    }
    return true;
}

void WriteBool(const char* name, bool& b, rapidjson::PrettyWriter<rapidjson::StringBuffer>* _writer)
{
    _writer->StartObject();
    _writer->String("Name");  _writer->String(name);
    _writer->String("Type"); _writer->String("bool");
    _writer->String(name); _writer->Bool(b);
    _writer->EndObject();
}
void WriteInt(const char*name, int& i, rapidjson::PrettyWriter<rapidjson::StringBuffer>* _writer)
{
    _writer->StartObject();
    _writer->String("Name");  _writer->String(name);
    _writer->String("Type"); _writer->String("int");
    _writer->String(name); _writer->Int(i);
    _writer->EndObject();
}
void WriteFloat(const char*name, float& f, rapidjson::PrettyWriter<rapidjson::StringBuffer>* _writer)
{
    _writer->StartObject();
    _writer->String("Name");  _writer->String(name);
    _writer->String("Type"); _writer->String("float");
    _writer->String(name); _writer->Double(f);
    _writer->EndObject();
}
void WriteUint(const char*name, uint32_t& u, rapidjson::PrettyWriter<rapidjson::StringBuffer>* _writer)
{
    _writer->StartObject();
    _writer->String("Name");  _writer->String(name);
    _writer->String("Type"); _writer->String("Uint32");
    _writer->String(name); _writer->Uint(u);
    _writer->EndObject();
}
void WriteString(const char*name, std::string& s, rapidjson::PrettyWriter<rapidjson::StringBuffer>* _writer)
{
    _writer->StartObject();
    _writer->String("Name");  _writer->String(name);
    _writer->String("Type"); _writer->String("string");
    _writer->String(name); _writer->String(s.c_str());
    _writer->EndObject();
}
void WriteVecctor2(const char* name, Vector2& v, rapidjson::PrettyWriter<rapidjson::StringBuffer>* _writer)
{
    _writer->StartObject();
    _writer->String("Name");  _writer->String(name);
    _writer->String("Type"); _writer->String("vectore2");
    _writer->String(name); Write2DVector(_writer, v);
    _writer->EndObject();
}
void WriteVecctor3(const char*name, Vector3& v, rapidjson::PrettyWriter<rapidjson::StringBuffer>* _writer)
{
    _writer->StartObject();
    _writer->String("Name");  _writer->String(name);
    _writer->String("Type"); _writer->String("vectore3");
    _writer->String(name); Write3DVector(_writer,v);
    _writer->EndObject();
}
void WriteVecctor4(const char*name, Vector4& v, rapidjson::PrettyWriter<rapidjson::StringBuffer>* _writer)
{
    _writer->StartObject();
    _writer->String("Name");  _writer->String(name);
    _writer->String("Type"); _writer->String("vectore4");
    _writer->String(name); Write4DVector(_writer, v);
    _writer->EndObject();
}

void WriteQuaternion(const char*name, Quat& q, rapidjson::PrettyWriter<rapidjson::StringBuffer>* _writer)
{
    _writer->StartObject();
    _writer->String("Name");  _writer->String(name);
    _writer->String("Type"); _writer->String("quat");
    _writer->String(name); WriteQuat(_writer, q);
    _writer->EndObject();
}

JsonWriter::~JsonWriter()
{
    delete _writer;
    delete _buffer;
}

JsonReader::JsonReader()
{
   // _document = new rapidjson::Document;
}

bool JsonReader::IsError(rapidjson::IStreamWrapper* isw)
{
    bool isError = _document.ParseStream(*isw).HasParseError() ? true : false;
    return isError;
}

void JsonReader::StartObject(std::ifstream& f)
{
    if (!f.is_open())
    {
        std::cout << "Could not open or find scene file!" << std::endl;
        return;
    }
    if (f.peek() == std::ifstream::traits_type::eof())
    {
        std::cout << "File is empty!" << std::endl;
        return;
    }
}

void ReadBool(const char* name, bool& b, const rapidjson::Value& obj)
{
    b = obj[name].GetBool();
}
void ReadInt(const char* name, int& i, const rapidjson::Value& obj)
{
    i = obj[name].GetInt();
}
void ReadFloat(const char name, float& f, const rapidjson::Value& obj)
{
    f = obj[name].GetFloat();
}
void ReadUint(const char* name, uint32_t& u, const rapidjson::Value& obj)
{
    u = obj[name].GetUint();
}
void ReadString(const char* name, std::string& s, const rapidjson::Value& obj)
{
    s = obj[name].GetString();
}
void ReadVecctor2(const char* name, Vector2& v, const rapidjson::Value& obj)
{
    v = Read2DVector<Vector2>(obj[name]);
}
void ReadVecctor3(const char* name, Vector3& v, const rapidjson::Value& obj)
{
    v = Read3DVector<Vector3>(obj[name]);
}
void ReadVecctor4(const char* name, Vector4& v, const rapidjson::Value& obj)
{
    v = Read4DVector<Vector4>(obj[name]);
}
void ReadQuaternion(const char* name, Quat& q, const rapidjson::Value& obj)
{
    q = Read4DVector<Quat>(obj[name]);
}

JsonReader::~JsonReader()
{
    //delete _document;
}
template<typename T>
T Read3DVector(const rapidjson::Value& val)
{
    T v{0};

    if (val.IsArray())
    {
        v.x = val[0].GetFloat();
        v.y = val[1].GetFloat();
        if (val.Size()==3)
            v.z = val[2].GetFloat();
        return v;
    }
    return v;
}
template<typename T>
T Read4DVector(const rapidjson::Value& val)
{
    T v{0,0,0,0};

    if (val.IsArray())
    {
        v.x = val[0].GetFloat();
        v.y = val[1].GetFloat();
        v.z = val[2].GetFloat();
        v.w = val[3].GetFloat();
    }
   
    return v;
}

template<typename T>
T Read2DVector(const rapidjson::Value& val)
{
    T v{ 0,0 };

    if (val.IsArray())
    {
        v.x = val[0].GetFloat();
        v.y = val[1].GetFloat();
    }

    return v;
}

void Write2DVector(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer, Vec2D v)
{
    writer->SetFormatOptions(kFormatSingleLineArray);
    writer->StartArray();
    writer->Double(v.x);
    writer->Double(v.y);
    writer->EndArray();
    writer->SetFormatOptions(kFormatDefault);
}

void Write2DVector(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer, GVec2D v)
{
    Vec2D tmp{ v.x,v.y };
    Write2DVector(writer, tmp);
}
void Write3DVector(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer, Vec3D v)
{
    writer->SetFormatOptions(kFormatSingleLineArray);
    writer->StartArray();
    writer->Double(v.x);
    writer->Double(v.y);
    writer->Double(v.z);
    writer->EndArray();
    writer->SetFormatOptions(kFormatDefault);
}
void Write3DVector(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer, PVec3D v)
{
    Vec3D tmp{ v.x,v.y,v.z };
    Write3DVector(writer, tmp);
}
void Write3DVector(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer, GVec3D v)
{
    Vec3D tmp{ v.x,v.y,v.z };
    Write3DVector(writer, tmp);
}
void Write4DVector(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer, Vec4D v)
{
    writer->SetFormatOptions(kFormatSingleLineArray);
    writer->StartArray();
    writer->Double(v.x);
    writer->Double(v.y);
    writer->Double(v.z);
    writer->Double(v.w);
    writer->EndArray();
    writer->SetFormatOptions(kFormatDefault);
}
void Write4DVector(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer, const PVec4D& v)
{
    Vec4D tmp{ v.x,v.y,v.z,v.w };
    Write4DVector(writer, tmp);
}
void Write4DVector(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer, GVec4D v)
{
    Vec4D tmp{ v.x,v.y,v.z,v.w };
    Write4DVector(writer, tmp);
}
void WriteQuat(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer, Quat v)
{
    writer->SetFormatOptions(kFormatSingleLineArray);
    writer->StartArray();
    writer->Double(v.x);
    writer->Double(v.y);
    writer->Double(v.z);
    writer->Double(v.w);
    writer->EndArray();
    writer->SetFormatOptions(kFormatDefault);
}

void WriteQuat(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer, GQuat v)
{
    Quat tmp{ v.x,v.y,v.z,v.w };
    WriteQuat(writer, tmp);
}
void WriteQuat(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer, PQuat v)
{
    Quat tmp{ v.x,v.y,v.z,v.w };
    WriteQuat(writer, tmp);
}

bool AddType(const rapidjson::Value& obj, std::string name, bool val)
{
    if (obj.HasMember(name.c_str()))
    {
        if (obj[name.c_str()].IsBool())
        {
            return obj[name.c_str()].GetBool();
        }
    }
    std::cout << name << ": Wrong Type, using default." << std::endl;
    return val;
}

unsigned AddType(const rapidjson::Value& obj, std::string name, unsigned val)
{
    if (obj.HasMember(name.c_str()))
    {
        if (obj[name.c_str()].IsUint())
        {
            return obj[name.c_str()].GetUint();
        }
    }
    std::cout << name << ": Wrong Type, using default." << std::endl;
    return val;
}

int AddType(const rapidjson::Value& obj, std::string name, int val)
{
    if (obj.HasMember(name.c_str()))
    {
        if (obj[name.c_str()].IsInt())
        {
            return obj[name.c_str()].GetInt();
        }
    }
    std::cout << name << ": Wrong Type, using default." << std::endl;
    return val;
}

double AddType(const rapidjson::Value& obj, std::string name, double val)
{
    if (obj.HasMember(name.c_str()))
    {
        if (obj[name.c_str()].IsDouble())
        {
            return obj[name.c_str()].GetDouble();
        }
    }
    std::cout << name << ": Wrong Type, using default." << std::endl;
    return val;
}

float AddType(const rapidjson::Value& obj, std::string name, float  val)
{
    if (obj.HasMember(name.c_str()))
    {
        if (obj[name.c_str()].IsFloat())
        {
            return obj[name.c_str()].GetFloat();
        }
    }
    std::cout << name << ": Wrong Type, using default." << std::endl;
    return val;
}
std::string AddType(const rapidjson::Value& obj, std::string name, std::string val)
{
    if (obj.HasMember(name.c_str()))
    {
        if (obj[name.c_str()].IsString())
        {
            return obj[name.c_str()].GetString();
        }
    }
    std::cout << name << ": Wrong Type, using default." << std::endl;
    return val;
}

Vec2D AddType(const rapidjson::Value& obj, std::string name, Vec2D val)
{
    (void)val;
    if (obj.HasMember(name.c_str()))
    {
        return Read2DVector<Vec2D>(obj[name.c_str()]);
    }
    std::cout << name << ": Wrong Type, using default." << std::endl;
    return val;
}

Vec3D AddType(const rapidjson::Value& obj, std::string name, Vec3D val)
{
    (void)val;
    if (obj.HasMember(name.c_str()))
    {
        return Read3DVector<Vec3D>(obj[name.c_str()]);
    }
    std::cout << name << ": Wrong Type, using default." << std::endl;
    return val;
}

Vec4D AddType(const rapidjson::Value& obj, std::string name, Vec4D val)
{
    (void)val;
    return Read4DVector<Vec4D>(obj[name.c_str()]);
}


PVec3D AddType(const rapidjson::Value& obj, std::string name, PVec3D val)
{
    (void)val;
    return Read3DVector<PVec3D>(obj[name.c_str()]);
}
PVec4D AddType(const rapidjson::Value& obj, std::string name, PVec4D val)
{
    (void)val;
    return Read4DVector<PVec4D>(obj[name.c_str()]);
}

GVec2D AddType(const rapidjson::Value& obj, std::string name, GVec2D val)
{
    (void)val;
    return Read2DVector<GVec2D>(obj[name.c_str()]);
}
GVec3D AddType(const rapidjson::Value& obj, std::string name, GVec3D val)
{
    (void)val;
    return Read3DVector<GVec3D>(obj[name.c_str()]);
}
GVec4D AddType(const rapidjson::Value& obj, std::string name, GVec4D val)
{
    (void)val;
    return Read4DVector<GVec4D>(obj[name.c_str()]);
}

physx::PxQuat AddType(const rapidjson::Value& obj, std::string name, physx::PxQuat val)
{
    (void)val;
    return Read4DVector<physx::PxQuat>(obj[name.c_str()]);
}
Quat AddType(const rapidjson::Value& obj, std::string name, Quat val)
{
    (void)val;
    return Read4DVector<Quat>(obj[name.c_str()]);
}
GQuat AddType(const rapidjson::Value& obj, std::string name, GQuat val)
{
    (void)val;
    return Read4DVector<GQuat>(obj[name.c_str()]);
}


bool SerializeToFile(const std::string& file)
{
    std::ofstream f(file);//overwrite the file
    JsonWriter writer;
    writer.StartObject();
    auto cm = *Engine::_ecs.getComponentManager();
    writer.SerializeComponent<ComponentManager>(cm);
    //Write all components realted to that entity
    writer.EndObject();
    std::ofstream ofs(file);
    ofs << writer.GetString();
    ofs.close();
    return true;
}

bool DeserializeFromFile(const std::string& file)
{
    std::ifstream f(file);//overwrite the file
    if (f.is_open())
    {
        JsonReader reader;
        reader.StartObject(f);
        //Document i = *reader._document;
        IStreamWrapper isw(f);
        if (!reader.IsError(&isw))
        {
            reader.DeserializeComponent<ComponentManager>(*Engine::_ecs.getComponentManager(),"0");
            f.close();
        }return true;
    }
    else
        return false;
   
}

bool SerializeWayPoint(const std::string& file, std::string waypoint_name)
{
    StringBuffer buffer;
    PrettyWriter<StringBuffer> writer(buffer);
    writer.StartObject();
    writer.String("WayPoints");
    writer.String(waypoint_name.c_str());
    // get the map vector from the waypoint_name
    auto vec = RESOURCE->GetWayPoint(waypoint_name);
    int order = 0;
    for (auto value : vec)
    {
        std::string point_name = "Point_";
        point_name += std::to_string(order);
        writer.String(point_name.c_str());
        writer.StartArray();
        // fix this part causing error
        writer.Double(value.x);
        writer.Double(value.y);
        writer.Double(value.z);
        // fix this part causing error
        writer.EndArray();
        ++order;
    }
    writer.EndObject();
    std::string str = "./Resources/WayPoints/";
    str.append(file);
    std::ofstream ofs(str);

    std::string st = buffer.GetString();
    ofs << st;
    ofs.flush();
    ofs.close();

    return true;
}

bool DeserializeWayPoint(const std::string& file)
{
    std::string str = "./Resources/WayPoints/";
    str.append(file);
    std::ifstream ifs(str);
    if (ifs)
    {
        IStreamWrapper isw(ifs);
        Document document;
        document.ParseStream(isw);


        std::vector<Vector3> elements;
        std::string name = document["WayPoints"].GetString();

        for (Value::ConstMemberIterator itr = ++document.MemberBegin(); itr != document.MemberEnd(); ++itr)
        {
            Vector3 vec;
            vec.x = static_cast<float>(document[itr->name][0].GetDouble());
            vec.y = static_cast<float>(document[itr->name][1].GetDouble());
            vec.z = static_cast<float>(document[itr->name][2].GetDouble());
            RESOURCE->AddWayPoint(name, vec);
        }

        return true;
    }
    return false;
}

void ConfigSerializer(const std::string& file, int& width, int& height)
{
    rapidjson::Document doc;
    doc.SetObject();
    rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();

    doc.AddMember("Width", width, allocator);
    doc.AddMember("Height", height, allocator);

    rapidjson::StringBuffer strbuf;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(strbuf);
    doc.Accept(writer);

    std::string st = strbuf.GetString();

    std::ofstream f(file, std::ios_base::trunc);

    f << st;
    f.close();
}

void ConfigDeserializer(const std::string file, std::string& windowTitle, int& width, int& height, bool& vsync, bool& editorMode, std::string& sceneFile)
{
    std::ifstream ifs{ file.c_str() };
    if (!ifs.is_open())
    {
        std::cout << "Could not open file for reading!" << std::endl;
    }

    IStreamWrapper isw(ifs);
    Document d;
    d.ParseStream(isw);

    windowTitle = d["Title"].GetString();
    width = d["Width"].GetInt();
    height = d["Height"].GetInt();
    vsync = d["VSync"].GetBool();
    editorMode = d["Editor"].GetBool();
    sceneFile = d["Scene"].GetString();

    ifs.close();
}

bool SerializePrefabToFile(const std::string& file, std::string prefabName)
{
    std::ofstream f(file);//overwrite the file
    JsonWriter writer;
    writer.StartObject();
    auto cm = *Engine::_ecs.getComponentManager();
    writer.SerializeComponentPrefab<ComponentManager>(cm, prefabName);
    //Write all components realted to that entity
    writer.EndObject();
    std::ofstream ofs(file);
    ofs << writer.GetString();
    ofs.close();
    return true;
}
