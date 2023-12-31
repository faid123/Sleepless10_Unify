#pragma once

#include <glm/gtc/type_ptr.hpp>
#define SOL_CHECK_ARGUMENTS 1
#include "sol/sol.hpp"
#include <Properties.h>
#include "Serializer/ISerializer.h"

struct LightCom : Component , public ISerializer
{
	//action type to perform
	enum  Type
	{
		None = 0, //don't render anything
		directional,
		point,
		spot = 3
	};
	std::map <std::string, Type> _stateMap = { 
		{"None",Type::None},
		{"directional",Type::directional},
		{"point",Type::point},
		{"spot",Type::spot} };

	// Data Members
	Type type_light;

	//NOTE TO FUTURE:
	/*
		if possible, 
		if type_light == Dir_Light, only show direction, ambient, diffuse, specular

		if type_light == Point_Light, only show position, ambient, diffuse, specular,
												constant, linear, quadratic

		if type_light == SpotLight, show all
	*/
	int _type_light = 2;
	//Directional Light data members
	Vector3  direction{ -0.2f, -1.0f, -0.3f };
	Vector3  ambient{0.5f};
	Vector3  diffuse{ 0.4f };
	Vector3  specular{ 0.5f };


	//Point Light data members
	Vector3 position{ 0.0f }; //grab from transform component
	float constant{1.0f};		//dont modify!
	float linear{ 0.09f };
	float quadratic{ 0.032f };


	//Spot Light data members
	float cutOff{ glm::cos(glm::radians(12.5f)) };
	float outerCutOff{ glm::cos(glm::radians(15.0f)) };
	/// till here
	struct BaseLight {};

	struct DirLight : BaseLight{
		Vector3  direction;
		
		Vector3  ambient;
		Vector3  diffuse;
		Vector3  specular;
		//shouldnt exist actually
		DirLight() = default;

		DirLight(
			const Vector3& dir,
			const Vector3& amb,
			const Vector3& dif,
			const Vector3& spe) :
			direction{dir},
			ambient{amb},
			diffuse{dif},
			specular{ spe }
		{}

		static void RegisterLua(sol::state& lua);

	};

	struct PointLight : DirLight{
		//point light is supposed to have glm::vec3 position here,
		//direction in DirLight will replace position as direction isn't
		// needed in pointlight and i can make use of inherited glm::vec3 diretion instead

		float constant;
		float linear;
		float quadratic;

		PointLight() = default;
		PointLight(
			const Vector3& pos,
			const Vector3& amb,
			const Vector3& dif,
			const Vector3& spe,
			const float& cons,
			const float& lin,
			const float& quad) :
			DirLight{pos,amb,dif,spe},
			constant{cons},
			linear{lin},
			quadratic{quad}
		{}

		static void RegisterLua(sol::state& lua);
	};

	struct SpotLight : PointLight{
		Vector3 position;
		float cutOff;
		float outerCutOff;
		SpotLight() = default;
		SpotLight(
			const Vector3& dir,
			const Vector3& amb,
			const Vector3& dif,
			const Vector3& spe,
			const Vector3& pos,
			const float& cons,
			const float& lin,
			const float& quad,
			const float& cOff,
			const float& ocOff) :
			PointLight{ dir,amb,dif,spe,cons,lin,quad},
			position{ pos },
			cutOff{ cOff },
			outerCutOff{ ocOff }
		{}

		static void RegisterLua(sol::state& lua);
	};

	template<typename T>
	T return_as_Light()
	{
		return T{};
	}

	template<>
	DirLight return_as_Light<DirLight>()
	{
		return DirLight{ direction,ambient,diffuse,specular };
	}

	template<>
	PointLight return_as_Light<PointLight>()
	{
		return PointLight{ position,ambient,diffuse,specular,constant,linear,quadratic };
	}
	template<>
	SpotLight return_as_Light<SpotLight>()
	{
		return SpotLight{ direction,ambient,diffuse,specular,position,constant,linear,quadratic,cutOff,outerCutOff };
	}

	DirLight returnDirLight()
	{
		return return_as_Light<DirLight>();
	}

	PointLight returnPointLight()
	{
		return return_as_Light<PointLight>();
	}

	SpotLight returnSpotLight()
	{
		return return_as_Light<SpotLight>();
	}

	LightCom() : type_light{ None }
	{
	}
	LightCom(const Type& type) : type_light{ type }
	{
	}
	void destroyEntity(EntityID entity) override
	{
		(void)entity;
	}
	void cloneComponent(EntityID entity1, EntityID entity2) override
	{
		(void)entity1;
		(void)entity2;
	}
	void Deserialize(const rapidjson::Document& doc, std::string id);
	void Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer);
	const char* StateToString(Type T);

	static void RegisterLua(sol::state& lua);
	property_vtable()
	
#pragma region deferred_shading
	/*  Pre-defined light & material properties.
	We assume all objects in the scene have the same material.
	*/
	//glm::vec3 position;	//will grab this value from transform
	//glm::vec4 intensity = { 0.5f, 0.5f, 1.0f, 1.0f };     /*  intensity of each light source */
	//glm::vec4 ambientAlbedo = { 0.4f, 0.4f, 0.4f, 1.0f };     /*  ambient albedo of material */
	//glm::vec4 diffuseAlbedo = { 0.18f, 0.18f, 0.36f, 1.0f };  /*  diffuse albedo of material */
	//glm::vec4 specularAlbedo = { 0.05f, 0.05f, 0.05f, 1.0f };  /*  specular albedo of material */
	//int  specularPower = 2;                              /*  specular power of material */
	/*LightCom(const Type& _Render, const glm::vec4& _intensity, const glm::vec4& ambient, 
		const glm::vec4 & diffuse, const glm::vec4& specular, const int& specularPOW = 2)
		: type_light{ _Render },
		intensity		{_intensity},
		ambientAlbedo	{ambient},
		diffuseAlbedo	{diffuse},
		specularAlbedo	{specular},
		specularPower	{specularPOW}
	{
	}*/
#pragma endregion
};

//property_begin_name(LightCom, "Light Component")
property_begin(LightCom)
{
	property_var(_type_light).Name("Light Type").Help("0 - no light\n1 - Directional Light\n2 - Point Light\n3 - Spot Light")
	,property_var(direction)
	, property_var(ambient)
	, property_var(diffuse)
	, property_var(specular)
	, property_var(position).Flags(property::flags::DONTSHOW)
	, property_var(constant)
	, property_var(linear)
	, property_var(quadratic)
	, property_var(cutOff)
	, property_var(outerCutOff)

} property_vend_h(LightCom)