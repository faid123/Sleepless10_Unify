/**********************************************************************************
* \file ComponentsSerializer.cpp
* \brief File contains the function definitions for all function that are used
         to serialize and deserialize all the component data.
* \author Zhang Xinyu, 100% Code Contribution
*
* \copyright Copyright (C) 2022 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#include "Graphics/TransformCom.h"
#include "Camera/CameraCom.h"
#include "Physics/RigidBodyComponent.h"
#include "ISerializer.h"
#include "Component/ComponentManager.h"
#include "Graphics/LightCom.h"
#include "Graphics/MaterialCom.h"
#include "Particles/ParticleComponent.h"
#include "Particles/EmitterComponent.h"
#include "Logic/Lua/LuaScriptCom.h"
#include "Engine.h"
#include "Audio/AudioSrcCom.h"
#include "../../Graphics/FreeType/text.h"

///////////////////////// Component Manager /////////////////////////////////////
//void ComponentManager::Deserialize(const rapidjson::Document& doc, std::string id)
//{
//#ifdef USE_VALUE
//    useValue(id);
//#endif
//    const char* key = "";
//    for (auto m = doc.MemberBegin(); m != doc.MemberEnd(); ++m)
//    {
//        key = m->name.GetString();
//        std::string entityName = doc[key]["Name"].GetString();
//        EntityID eid = Engine::_ecs.createNewEntity(entityName); //(EntityID)std::stoi(key);
//        Engine::_ecs.getEntity(eid).Deserialize(doc[key]);
//        if (doc[key].HasMember("Transform"))
//        {
//            TransformCom c;
//            c.Deserialize(doc, key);
//            Engine::_ecs.addComponentToEntity(eid, c);
//        }
//        if (doc[key].HasMember("Camera"))
//        {
//            CameraCom c;
//            c.Deserialize(doc, key);
//            Engine::_ecs.addComponentToEntity(eid, c);
//        }
//        if (doc[key].HasMember("RigidBodyDynamic"))
//        {
//            RigidDynamicCom c;
//            c.Deserialize(doc, key);
//            Engine::_ecs.addComponentToEntity(eid, c);
//        }
//        if (doc[key].HasMember("RigidBodyStatic"))
//        {
//            RigidStaticCom c;
//            c.Deserialize(doc, key);
//            Engine::_ecs.addComponentToEntity(eid, c);
//        }
//        if (doc[key].HasMember("Light"))
//        {
//            LightCom c;
//            c.Deserialize(doc, key);
//            Engine::_ecs.addComponentToEntity(eid, c);
//        }
//        if (doc[key].HasMember("Material"))
//        {
//            MaterialCom c;
//            c.Deserialize(doc, key);
//            Engine::_ecs.addComponentToEntity(eid, c);
//        }
//    }
//}

/// <summary>
/// reading from file and putting into engine
/// </summary>
/// <param name="doc"></param>
/// <param name="id"></param>
void ComponentManager::Deserialize(const rapidjson::Document& doc, std::string id)
{
#ifdef USE_VALUE
    useValue(id);
#endif
    const char* key = "";
    for (auto m = doc.MemberBegin(); m != doc.MemberEnd(); ++m)
    {
        bool isPrefab = false;
        EntityID eid = 0; PrefabID pid = 0;
        key = m->name.GetString();
        std::string entityName = doc[key]["Name"].GetString();
        if (!entityName.find("Prefab"))
        {
            pid = Engine::_ecs.createNewEntity(entityName, false, false, true);
            Engine::_ecs.getPrefab(pid).Deserialize(doc[key]);
            std::set<std::string> getTag = Engine::_ecs.getPrefabTag(pid);
            for (auto tag : getTag)
            {
                Engine::_ecs.addToPrefabTagsMap(pid, tag);
                std::cout << "added prefab tag: " + tag << std::endl;
            }
            isPrefab = true;
        }
        else
        {
            eid = Engine::_ecs.createNewEntity(entityName); //(EntityID)std::stoi(key);
            Engine::_ecs.getEntity(eid).Deserialize(doc[key]);
            std::set<std::string> getTag = Engine::_ecs.getEntityTag(eid);
            for (auto tag : getTag)
            {
                Engine::_ecs.addToEntityTagsMap(eid, tag);
            } 
            Entity entity = Engine::_ecs.getEntity(eid);
            if (entity._parentID != -1)
            {
                Engine::_ecs.setEntityParent(eid, entity._parentID);
            }
        }

        property::entry PropEntry;

        const rapidjson::Value& arr = doc[key]["Components"]; // component array
        for (rapidjson::SizeType i = 0; i < arr.Size(); ++i)
        {

            const rapidjson::Value& cam = arr[i]; // camera com array
            const char* comp_name = cam["ComponentType"].GetString();
            
            //std::cout << comp_name << std::endl;
            std::shared_ptr<Component> temp;
            if (isPrefab)
            {
                temp = Engine::_ecs.addComponentPrefab(pid, comp_name);
            }
            else
            {
                temp = Engine::_ecs.addComponent(eid, comp_name);
            }

            for (rapidjson::SizeType j = 0; j < cam["Properties"].Size(); ++j)
            {
                const char* name = cam["Properties"][j]["Name"].GetString();
                const char* type = cam["Properties"][j]["Type"].GetString();

                PropEntry.first = name;
                std::string test;
                test = type;

                if (0 == std::strcmp(test.data(), "string"))
                {
                    PropEntry.second.emplace<std::string>() = cam["Properties"][j][PropEntry.first.c_str()].GetString();
                }
                else if (0 == std::strcmp(test.data(), "Uint"))
                {
                    PropEntry.second.emplace<unsigned int>() = cam["Properties"][j][PropEntry.first.c_str()].GetUint();
                }
                else if (0 == std::strcmp(test.data(), "int"))
                {
                    PropEntry.second.emplace<int>() = cam["Properties"][j][PropEntry.first.c_str()].GetInt();
                }
                else if (0 == std::strcmp(test.data(), "float"))
                {
                    PropEntry.second.emplace<float>() = cam["Properties"][j][PropEntry.first.c_str()].GetFloat();
                }
                else if (0 == std::strcmp(test.data(), "bool"))
                {
                    PropEntry.second.emplace<bool>() = cam["Properties"][j][PropEntry.first.c_str()].GetBool();
                }
                else if (0 == std::strcmp(test.data(), "vectore3"))
                {
                    Vector3 vector;
                    ReadVecctor3(name, vector, cam["Properties"][j]);
                    PropEntry.second.emplace<Vector3>() = vector;
                }
                else if (0 == std::strcmp(test.data(), "vectore4"))
                {
                    Vector4 vector;
                    ReadVecctor4(name, vector, cam["Properties"][j]);
                    PropEntry.second.emplace<Vector4>() = vector;
                }
                else if (0 == std::strcmp(test.data(), "quat"))
                {
                    Quat quat;
                    ReadQuaternion(name, quat, cam["Properties"][j]);
                    PropEntry.second.emplace<Quat>() = quat;
                }
                else if (0 == std::strcmp(test.data(), "vectore2"))
                {
                    Vector2 vector;
                    ReadVecctor2(name, vector, cam["Properties"][j]);
                    PropEntry.second.emplace<Vector2>() = vector;
                }
                property::set(*temp, PropEntry.first.c_str(), PropEntry.second);

            }
        }
    }
}


//void ComponentManager::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer)
//{
//
//    //writer->Key("ComponentManager");
//    //writer->StartArray();
//
//    EntityManager em = *Engine::_ecs.getEntityManager();
//    auto allEntities = em.getActiveEntities();
//    for (auto it = allEntities.begin(); it != allEntities.end(); it++)
//    {
//        (*it).Serialize(writer);
//        EntityID id = (*it)._entityID;
//        //Get all the components related to this entityID
//        //auto listComponents = Engine::_ecs.getEntityComponentNames(id);
//        if (getComponent<TransformCom>(id) != nullptr)
//            (*(getComponent<TransformCom>(id))).Serialize(writer);
//        if (getComponent<CameraCom>(id) != nullptr)
//            (*(getComponent<CameraCom>(id))).Serialize(writer);
//        if (getComponent<RigidDynamicCom>(id) != nullptr)
//            (*(getComponent<RigidDynamicCom>(id))).Serialize(writer);
//        if (getComponent<RigidStaticCom>(id) != nullptr)
//            (*(getComponent<RigidStaticCom>(id))).Serialize(writer);
//        if (getComponent<LightCom>(id) != nullptr)
//            (*(getComponent<LightCom>(id))).Serialize(writer);
//        if (getComponent<MaterialCom>(id) != nullptr)
//            (*(getComponent<MaterialCom>(id))).Serialize(writer);
//        writer->EndObject();
//    }
//    //writer->EndArray();
//
//}

/// <summary>
/// saving to a file
/// </summary>
/// <param name="writer"></param>
void ComponentManager::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer)
{

    //writer->Key("ComponentManager");
    //writer->StartArray();

    EntityManager em = *Engine::_ecs.getEntityManager();
    auto allEntities = em.getActiveEntities();
    std::vector<std::string> listofcomnames;
    std::vector<std::shared_ptr<Component>> listofcom;

    for (auto it = allEntities.begin(); it != allEntities.end(); it++)
    {
        auto tag = (*it)._tags.find("Particles");
        if (tag == (*it)._tags.end())
        {
            (*it).Serialize(writer);
            EntityID id = (*it)._entityID;

            Engine::_ecs.getEntityComponentPtrAndName(id, listofcomnames, listofcom);
            writer->String("Components");
            writer->StartArray();
            int i = 0;
            for (auto& E : listofcom)
            {
                writer->StartObject();
                writer->String("ComponentType");
                writer->String(listofcomnames[i++].c_str());
                writer->String("Properties");
                writer->StartArray();

                property::SerializeEnum(*E, [&](std::string_view PropertyName, property::data&& Data, const property::table&, std::size_t, property::flags::type Flags)
                    {
                        (void)Flags;
                        std::visit([&](auto&& Value)
                            {
                                using T = std::decay_t<decltype(Value)>;

                                if constexpr (std::is_same_v<T, int>)
                                {
                                    WriteInt(PropertyName.data(), Value, writer);
                                }
                                else if constexpr (std::is_same_v<T, float>)
                                {
                                    WriteFloat(PropertyName.data(), Value, writer);
                                }
                                else if constexpr (std::is_same_v<T, uint32_t>)
                                {
                                    WriteUint(PropertyName.data(), Value, writer);
                                }
                                else if constexpr (std::is_same_v<T, bool>)
                                {
                                    WriteBool(PropertyName.data(), Value, writer);
                                }
                                else if constexpr (std::is_same_v<T, string_t>)
                                {
                                    WriteString(PropertyName.data(), Value, writer);
                                }
                                else if constexpr (std::is_same_v<T, oobb>)
                                {

                                }
                                else if constexpr (std::is_same_v<T, Vector2>)
                                {
                                    WriteVecctor2(PropertyName.data(), Value, writer);
                                }
                                else if constexpr (std::is_same_v<T, Vector3>)
                                {
                                    WriteVecctor3(PropertyName.data(), Value, writer);
                                }
                                else if constexpr (std::is_same_v<T, Vector4>)
                                {
                                    WriteVecctor4(PropertyName.data(), Value, writer);
                                }
                                else if constexpr (std::is_same_v<T, Matrix4>)
                                {

                                }
                                else if constexpr (std::is_same_v<T, Quat>)
                                {
                                    WriteQuaternion(PropertyName.data(), Value, writer);
                                }
                                else if constexpr (std::is_same_v<T, glm::vec3>)
                                {

                                }
                                else assert(always_false<T>::value);

                            }
                        , Data);

                    });
                writer->EndArray();
                writer->EndObject();
            }
            writer->EndArray();
            listofcomnames.clear();
            listofcom.clear();
            writer->EndObject();
        }
    }
    //writer->EndArray();

}

void ComponentManager::SerializePrefab(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer, std::string prefabName)
{
    EntityManager em = *Engine::_ecs.getEntityManager();
    EntityID entityID = em.getEntityID(prefabName);
    auto entitytoprefab = em.getEntity(entityID);

    std::vector<std::string> listofcomnames;
    std::vector<std::shared_ptr<Component>> listofcom;

    entitytoprefab.SerializePrefab(writer);
    EntityID id = entitytoprefab._entityID;

    Engine::_ecs.getEntityComponentPtrAndName(id, listofcomnames, listofcom);
    writer->String("Components");
    writer->StartArray();
    int i = 0;
    for (auto& E : listofcom)
    {
        writer->StartObject();
        writer->String("ComponentType");
        writer->String(listofcomnames[i++].c_str());
        writer->String("Properties");
        writer->StartArray();

        property::SerializeEnum(*E, [&](std::string_view PropertyName, property::data&& Data, const property::table&, std::size_t, property::flags::type Flags)
        {
            (void)Flags;
            std::visit([&](auto&& Value)
            {
                using T = std::decay_t<decltype(Value)>;

                if constexpr (std::is_same_v<T, int>)
                {
                    WriteInt(PropertyName.data(), Value, writer);
                }
                else if constexpr (std::is_same_v<T, float>)
                {
                    WriteFloat(PropertyName.data(), Value, writer);
                }
                else if constexpr (std::is_same_v<T, uint32_t>)
                {
                    WriteUint(PropertyName.data(), Value, writer);
                }
                else if constexpr (std::is_same_v<T, bool>)
                {
                    WriteBool(PropertyName.data(), Value, writer);
                }
                else if constexpr (std::is_same_v<T, string_t>)
                {
                    WriteString(PropertyName.data(), Value, writer);
                }
                else if constexpr (std::is_same_v<T, oobb>)
                {

                }
                else if constexpr (std::is_same_v<T, Vector3>)
                {
                    WriteVecctor3(PropertyName.data(), Value, writer);
                }
                else if constexpr (std::is_same_v<T, Vector4>)
                {
                    WriteVecctor4(PropertyName.data(), Value, writer);
                }
                else if constexpr (std::is_same_v<T, Matrix4>)
                {

                }
                else if constexpr (std::is_same_v<T, Quat>)
                {
                    WriteQuaternion(PropertyName.data(), Value, writer);
                }
                else if constexpr (std::is_same_v<T, glm::vec3>)
                {

                }
                else assert(always_false<T>::value);

            }
            , Data);

        });
        writer->EndArray();
        writer->EndObject();
    }
    writer->EndArray();
    listofcomnames.clear();
    listofcom.clear();
    writer->EndObject();
//writer->EndArray();

}


///////////////////////// Transform /////////////////////////////////////
const char* TransformCom::StateToString(int s)
{
    switch (s)
    {
        case 1:  return "Static";
        case 2:  return "Dynamic";
        default: return "None";
    }
}


void TransformCom::Deserialize(const rapidjson::Document& doc, std::string id)
{
    if (doc.HasMember(id.c_str()))
    {
        if (doc[id.c_str()].HasMember("Transform"))
        {
            const rapidjson::Value& obj = doc[id.c_str()]["Transform"];
            TransformCom t;
            pos = AddType(obj, "Pos", t.pos); //std::cout << "t.pos: " << t.pos.z << std::endl;
            scale = AddType(obj, "Scale", t.scale);
            rotate = AddType(obj, "Rotate", t.rotate);
        }
    }
}

void TransformCom::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer)
{
    writer->String("Transform");
    writer->StartObject();

    writer->String("Pos");
    Write3DVector(writer, pos);
    writer->String("Scale");
    Write3DVector(writer, scale);
    writer->String("Rotate");
    Write3DVector(writer, rotate);

    writer->EndObject();
}

///////////////////////// Camera Component /////////////////////////////////////
void CameraCom::Deserialize(const rapidjson::Document& doc, std::string id)
{
    if (doc.HasMember(id.c_str()))
    {
        if (doc[id.c_str()].HasMember("Camera"))
        {
            const rapidjson::Value& obj = doc[id.c_str()]["Camera"];
            CameraCom t;
            _pos = AddType(obj, "Pos", t._pos); //std::cout << "t.pos: " << t.pos.z << std::endl;
            _up = AddType(obj, "Up", t._up);
            _rotation = AddType(obj, "Rot", t._rotation);
            

            _m_fov = AddType(obj, "FOV", t._m_fov);
            _m_near = AddType(obj, "Near", t._m_near);
            _m_far = AddType(obj, "Far", t._m_far);

        }
    }
    else
    {
        std::cout << "EntityID " << id <<
            " : cannot find the entity and its Camera." << std::endl;
        return;
    }
}

void CameraCom::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer)
{
    writer->String("Camera");
    writer->StartObject();

    writer->String("Pos");
    Write3DVector(writer,  _pos );
    writer->String("Up");
    Write3DVector(writer, _up);
    writer->String("Rot");
    Write3DVector(writer, _rotation);

    writer->String("FOV");
    writer->Double(_m_fov);
    writer->String("Near");
    writer->Double(_m_near);
    writer->String("Far");
    writer->Double(_m_far);


    writer->EndObject();
}


///////////////////////// RigidBody /////////////////////////////////////////
void RigidStaticCom::Deserialize(const rapidjson::Document& doc, std::string id)
{
    if (doc.HasMember(id.c_str()))
    {
        if (doc[id.c_str()].HasMember("RigidBodyStatic"))
        {
            const rapidjson::Value& obj = doc[id.c_str()]["RigidBodyStatic"];
            RigidStaticCom t;
            rigidPos = AddType(obj, "Pos", t.rigidPos); //std::cout << "t.pos: " << t.pos.z << std::endl;
            rigidScale = AddType(obj, "Scale", t.rigidScale);
            rigidRot = AddType(obj, "Rot", t.rigidRot);
        }
    }
}
void RigidStaticCom::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer)
{
    writer->String("RigidBodyStatic");
    writer->StartObject();

    writer->String("Pos");
    Write3DVector(writer, rigidPos);
    writer->String("Scale");
    Write3DVector(writer, rigidScale);
    writer->String("Rot"); WriteQuat(writer, rigidRot);
    writer->EndObject();
}
void RigidDynamicCom::Deserialize(const rapidjson::Document& doc, std::string id)
{
    if (doc.HasMember(id.c_str()))
    {
        if (doc[id.c_str()].HasMember("RigidBodyDynamic"))
        {
            const rapidjson::Value& obj = doc[id.c_str()]["RigidBodyDynamic"];
            RigidDynamicCom t;
            rigidPos = AddType(obj, "Pos", t.rigidPos); //std::cout << "t.pos: " << t.pos.z << std::endl;
            rigidScale = AddType(obj, "Scale", t.rigidScale);
            mass = AddType(obj, "Mass", t.mass);
            rigidRot = AddType(obj, "Rot", t.rigidRot);
        }
    }
}
void RigidDynamicCom::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer)
{
    writer->String("RigidBodyDynamic");
    writer->StartObject();

    writer->String("Pos");
    Write3DVector(writer, rigidPos);
    writer->String("Scale");
    Write3DVector(writer, rigidScale);
    writer->String("Rot"); WriteQuat(writer, rigidRot);
    writer->String("Mass");
    writer->Double(mass);

    writer->EndObject();
}

void LightCom::Deserialize(const rapidjson::Document& doc, std::string id)
{
    if (doc.HasMember(id.c_str()))
    {
        if (doc[id.c_str()].HasMember("Light"))
        {
            const rapidjson::Value& obj = doc[id.c_str()]["Light"];
            LightCom t;
            type_light = _stateMap[obj["type_light"].GetString()];
            direction = AddType(obj, "direction", t.direction); //std::cout << "t.pos: " << t.pos.z << std::endl;
            ambient = AddType(obj, "ambient", t.ambient);
            diffuse = AddType(obj, "diffuse", t.diffuse);
            specular = AddType(obj, "specular", t.specular);
            position = AddType(obj, "position", t.position);
            constant = AddType(obj, "constant", t.constant);
            linear = AddType(obj, "linear", t.linear);
            quadratic = AddType(obj, "quadratic", t.quadratic);
            cutOff = AddType(obj, "cutOff", t.cutOff);
            outerCutOff = AddType(obj, "outerCutOff", t.outerCutOff);
        }
    }
}

void LightCom::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer)
{
    writer->String("Light");
    writer->StartObject();

    writer->String("type_light");
    writer->String(StateToString(type_light));

    writer->String("direction");
    Write3DVector(writer, direction);

    writer->String("ambient");
    Write3DVector(writer, ambient);

    writer->String("diffuse");
    Write3DVector(writer, diffuse);

    writer->String("specular");
    Write3DVector(writer, specular);

    writer->String("position");
    Write3DVector(writer, position);

    writer->String("constant");
    writer->Double(constant);

    writer->String("linear");
    writer->Double(linear);

    writer->String("quadratic");
    writer->Double(quadratic);

    writer->String("cutOff");
    writer->Double(cutOff);

    writer->String("outerCutOff");
    writer->Double(outerCutOff);

    writer->EndObject();
}
const char* LightCom::StateToString(Type t)
{
    switch (t)
    {
    case LightCom::Type::None:
        return "None";
        break;
    case LightCom::Type::directional:
        return "directional";
        break;
    case LightCom::Type::point:
        return "point";
        break;
    case LightCom::Type::spot:
        return "spot";
        break;
    default:
        break;
    }
    return nullptr;
}
void MaterialCom::Deserialize(const rapidjson::Document& doc, std::string id)
{
    if (doc.HasMember(id.c_str()))
    {
        if (doc[id.c_str()].HasMember("Material"))
        {
            const rapidjson::Value& obj = doc[id.c_str()]["Material"];
            MaterialCom t;
            m_model = AddType(obj, "Model", t.m_model);
            m_shader = AddType(obj, "Shader", t.m_shader);
            m_texture = AddType(obj, "Texture", t.m_texture);
            _alpha = AddType(obj, "alpha", t._alpha);
            _saturation = AddType(obj, "saturation", t._saturation);
        }
    }
}

void MaterialCom::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer)
{
    writer->String("Material");
    writer->StartObject();

    writer->String("Model");
    writer->String(m_model.c_str());
    writer->String("Shader");
    writer->String(m_shader.c_str());
    writer->String("Texture");
    writer->String(m_texture.c_str());
    writer->String("alpha");
    writer->Double(_alpha);
    writer->String("saturation");
    Write3DVector(writer, _saturation);

    writer->EndObject();
}

void ParticleCom::Deserialize(const rapidjson::Document& doc, std::string id)
{
    (void)doc;
    (void)id;
}

void ParticleCom::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer)
{
    (void)writer;
}

void EmitterCom::Deserialize(const rapidjson::Document& doc, std::string id)
{
    if (doc.HasMember(id.c_str()))
    {
        if (doc[id.c_str()].HasMember("Emitter"))
        {
            const rapidjson::Value& obj = doc[id.c_str()]["Emitter"];
            EmitterCom t;
            //particles.clear();
            velocityVariation = AddType(obj, "velocityVariation", t.velocityVariation);
            colorBegin = AddType(obj, "ColorBegin", t.colorBegin);
            colorEnd = AddType(obj, "ColorEnd", t.colorEnd);
            sizeBegin = AddType(obj, "SizeBegin", t.sizeBegin);
            sizeEnd = AddType(obj, "SizeEnd", t.sizeEnd);
            sizeVariation = AddType(obj, "SizeVariation", t.sizeVariation);
            lifeTime = AddType(obj, "LifeTime", t.lifeTime);
            emit = AddType(obj, "Emit", t.emit);
        }
    }
}

void EmitterCom::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer)
{
    writer->String("Emitter");
    writer->StartObject();
    
    writer->String("velocityVariation");
    Write3DVector(writer, velocityVariation);

    writer->String("ColorBegin");
    Write4DVector(writer, colorBegin);

    writer->String("ColorEnd");
    Write4DVector(writer, colorEnd);

    writer->String("SizeBegin");
    writer->Double(sizeBegin);

    writer->String("SizeEnd");
    writer->Double(sizeEnd);

    writer->String("SizeVariation");
    writer->Double(sizeVariation);

    writer->String("LifeTime");
    writer->Double(lifeTime);

    writer->String("Emit");
    writer->Bool(emit);

}

///////////////////////// Audio /////////////////////////////////////////
void AudioSrcCom::Deserialize(const rapidjson::Document& doc, std::string id)
{
    if (doc.HasMember(id.c_str()))
    {
        if (doc[id.c_str()].HasMember("AudioSrcCom"))
        {
            const rapidjson::Value& obj = doc[id.c_str()]["AudioSrcCom"];
            AudioSrcCom t;
            volume = AddType(obj, "Vol", t.volume);
            mute = AddType(obj, "Mute", t.mute);
        }
    }
}
void AudioSrcCom::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer)
{
    writer->String("AudioSrcCom");
    writer->StartObject();

    writer->String("Vol");
    Write3DVector(writer, volume);
    writer->String("Mute");
    writer->Bool(mute);
    writer->EndObject();
}

//void LuaScriptCom::Deserialize(const rapidjson::Document& doc, std::string id)
//{
//    /*if (doc.HasMember(id.c_str()))
//    {
//        if (doc[id.c_str()].HasMember("Lua"))
//        {
//            const rapidjson::Value& obj = doc[id.c_str()]["Lua"];
//            LuaScriptCom t;
//        }
//    }*/
//}
//
//void LuaScriptCom::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) 
//{
//    /*writer->String("Emitter");
//    writer->StartObject();
//
//    writer->String("velocityVariation");
//    Write3DVector(writer, velocityVariation);*/
//}