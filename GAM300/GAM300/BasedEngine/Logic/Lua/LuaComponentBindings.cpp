#include "Graphics/TransformCom.h"
#include "Graphics/LightCom.h"
#include "Graphics/MaterialCom.h"
#include "Camera/CameraCom.h"
#include "Physics/RigidBodyComponent.h"
#include "Particles/ParticleComponent.h"
#include "Particles/EmitterComponent.h"
#include "Graphics/SpriteCom.h"
#include "Audio/AudioSrcCom.h"
#include "LuaScriptCom.h"
#include "Animation/AnimationCom.h"
#include "Graphics/TextCom.h"
//Definitions for registering components

/// <summary>
/// Binding transform component's member functions and variable to use in LUA
/// </summary>
/// <param name="lua">The sol state</param>
void TransformCom::RegisterLua(sol::state& lua)
{
    lua.new_usertype<TransformCom>("TransformCom",
        //---------- Constructors ----------
        sol::constructors<
        TransformCom(),
        TransformCom(Vector3, Vector3, Vector3)
        >(),

        //---------- Data ----------
        "pos", &TransformCom::pos,
        "rotate", &TransformCom::rotate,
        "scale", &TransformCom::scale,


        //---------- Member Function ----------
        "StateToString", &TransformCom::StateToString,
        "Deserialize", &TransformCom::Deserialize,
        "Serialize", &TransformCom::Serialize
        ); //END
}

/// <summary>
/// Binding light component's member functions and variable to use in LUA
/// </summary>
/// <param name="lua">The sol state</param>
void LightCom::RegisterLua(sol::state& lua)
{
	lua.new_usertype<LightCom>("LightCom",
		//---------- Constructors ----------
		sol::constructors<
		LightCom(),
		LightCom(Type)
		>(),

		//---------- Data ----------
		"_stateMap", &LightCom::_stateMap,
		"type_light", &LightCom::type_light,
		"direction", &LightCom::direction,
		"ambient", &LightCom::ambient,
		"diffuse", &LightCom::diffuse,
		"specular", &LightCom::specular,
		"position", &LightCom::position,
		"constant", &LightCom::constant,
		"linear", &LightCom::linear,
		"quadratic", &LightCom::quadratic,
		"cutOff", &LightCom::cutOff,
		"outerCutOff", &LightCom::outerCutOff,

		//---------- Member Function ----------
		"returnDirLight", &LightCom::returnDirLight,
		"returnPointLight", &LightCom::returnPointLight,
		"returnSpotLight", &LightCom::returnSpotLight
		); //END
}

/// <summary>
/// Binding Directional light component's member functions and variable to use in LUA
/// </summary>
/// <param name="lua">The sol state</param>
void LightCom::DirLight::RegisterLua(sol::state& lua)
{
	lua.new_usertype<DirLight>("DirLight",
		//---------- Constructors ----------
		sol::constructors<
		DirLight(),
		DirLight(
			const Vector3&,
			const Vector3&,
			const Vector3&,
			const Vector3&)
		>(),

		//---------- Data ----------
		"direction", &DirLight::direction,
		"ambient", &DirLight::ambient,
		"diffuse", &DirLight::diffuse,
		"specular", &DirLight::specular,

		sol::base_classes, sol::bases<BaseLight>()
		); //END
}

/// <summary>
/// Binding Point Light component's member functions and variable to use in LUA
/// </summary>
/// <param name="lua">The sol state</param>
void LightCom::PointLight::RegisterLua(sol::state& lua)
{
	lua.new_usertype<PointLight>("PointLight",
		//---------- Constructors ----------
		sol::constructors<
		PointLight(),
		PointLight(
			const Vector3&,
			const Vector3&,
			const Vector3&,
			const Vector3&,
			const float&,
			const float&,
			const float&)
		>(),

		//---------- Data ----------
		"constant", &PointLight::constant,
		"linear", &PointLight::linear,
		"quadratic", &PointLight::quadratic,

		sol::base_classes, sol::bases<DirLight>()
		); //END
}

/// <summary>
/// Binding Spot light component's member functions and variable to use in LUA
/// </summary>
/// <param name="lua">The sol state</param>
void LightCom::SpotLight::RegisterLua(sol::state& lua)
{
	lua.new_usertype<SpotLight>("SpotLight",
		//---------- Constructors ----------
		sol::constructors<
		SpotLight(),
		SpotLight(
			const Vector3&,
			const Vector3&,
			const Vector3&,
			const Vector3&,
			const Vector3&,
			const float&,
			const float&,
			const float&,
			const float&,
			const float&)
		>(),

		//---------- Data ----------
		"position", &SpotLight::position,
		"cutOff", &SpotLight::cutOff,
		"cutOff", &SpotLight::outerCutOff,

		sol::base_classes, sol::bases<PointLight>()
		); //END
}

/// <summary>
/// Binding Material component's member functions and variable to use in LUA
/// </summary>
/// <param name="lua">The sol state</param>
void MaterialCom::RegisterLua(sol::state& lua)
{
	lua.new_usertype<MaterialCom>("MaterialCom",
		//---------- Constructors ----------
		sol::constructors<
		MaterialCom(),
		MaterialCom(std::string, std::string, std::string)
		>(),

		//---------- Data ----------
		"isInitialised", &MaterialCom::isInitialised,
		"m_model", &MaterialCom::m_model,
		"m_shader", &MaterialCom::m_shader,
		"m_texture", &MaterialCom::m_texture,
		"_alpha", &MaterialCom::_alpha,
		"colour", &MaterialCom::colour,
		"_saturation", &MaterialCom::_saturation,

		//---------- Member Function ----------
		"Deserialize", &MaterialCom::Deserialize,
		"Serialize", &MaterialCom::Serialize
		); //END
}

/// <summary>
/// Binding Camera component's member functions and variable to use in LUA
/// </summary>
/// <param name="lua">The sol state</param>
void CameraCom::RegisterLua(sol::state& lua)
{
	lua.new_usertype<CameraCom>("CameraCom",
		//---------- Constructors ----------
		sol::constructors<
		CameraCom(),
		CameraCom(glm::vec3, float, float, float, glm::mat4, glm::mat4)
		>(),

		//---------- Data ----------
		"_pos", &CameraCom::_pos,
		"_up", &CameraCom::_up,
		"_rotation", &CameraCom::_rotation,
		"_m_fov", &CameraCom::_m_fov,
		"_m_near", &CameraCom::_m_near,
		"_m_far", &CameraCom::_m_far,
		"_proj_mtx", &CameraCom::_proj_mtx,
		"_view_mtx", &CameraCom::_view_mtx,
		"_aggressive", &CameraCom::_aggressive,
		"_displaceRight", &CameraCom::_displaceRight,
		"_displaceUp", & CameraCom::_displaceUp,
		"_displaceFor", & CameraCom::_displaceFor,
		"_camEye", & CameraCom::_camEye,

		//---------- Member Function ----------
		"Deserialize", &CameraCom::Deserialize,
		"Serialize", &CameraCom::Serialize
		); //END
}

/// <summary>
/// Binding RigidStatic component's member functions and variable to use in LUA
/// </summary>
/// <param name="lua">The sol state</param>
void RigidStaticCom::RegisterLua(sol::state& lua)
{
	lua.new_usertype<RigidStaticCom>("RigidStaticCom",
		//---------- Constructors ----------
		sol::constructors<
		>(),

		//---------- Data ----------
		"pos", &RigidStaticCom::rigidPos,
		"scale", &RigidStaticCom::rigidScale,
		"rot", &RigidStaticCom::rigidRot,
		"body", &RigidStaticCom::body,
		"shape", &RigidStaticCom::shape,
		"material", &RigidStaticCom::material,
		"staticFriction", &RigidStaticCom::staticFriction,
		"dynamicFriction", &RigidStaticCom::dynamicFriction,
		"restitution", &RigidStaticCom::restitution,
		"getHit", &RigidStaticCom::getHit,


		//---------- Member Function ----------
		"Deserialize", &RigidStaticCom::Deserialize,
		"Serialize", &RigidStaticCom::Serialize,
		"GetAABB", &RigidStaticCom::GetAABB,
		"setActorName", &RigidStaticCom::setActorName,
		"getActorName", &RigidStaticCom::getActorName
		); //END

}

/// <summary>
/// Binding RigidDynamic component's member functions and variable to use in LUA
/// </summary>
/// <param name="lua">The sol state</param>
void RigidDynamicCom::RegisterLua(sol::state& lua)
{
	lua.new_usertype<RigidDynamicCom>("RigidDynamicCom",
		//---------- Constructors ----------
		sol::constructors<
		>(),

		//---------- Data ----------
		"rigidPos", &RigidDynamicCom::rigidPos,
		"rigidScale", &RigidDynamicCom::rigidScale,
		"rigidRot", &RigidDynamicCom::rigidRot,
		"mass", &RigidDynamicCom::mass,
		"body", &RigidDynamicCom::body,
		"shape", &RigidDynamicCom::shape,
		"m_Int", &RigidDynamicCom::m_Int,
		"material", &RigidDynamicCom::material,
		"staticFriction", &RigidDynamicCom::staticFriction,
		"dynamicFriction", &RigidDynamicCom::dynamicFriction,
		"restitution", &RigidDynamicCom::restitution,
		"linearDamp", &RigidDynamicCom::linearDamp,
		"angularDamp", &RigidDynamicCom::angularDamp,
		"getHit", & RigidDynamicCom::getHit,

		//---------- Member Function ----------
		"Deserialize", &RigidDynamicCom::Deserialize,
		"Serialize", &RigidDynamicCom::Serialize,
		"GetAABB", &RigidDynamicCom::GetAABB,
		"setActorName", & RigidDynamicCom::setActorName,
		"getActorName", & RigidDynamicCom::getActorName
		); //END
}

/// <summary>
/// Binding Emitter component's member functions and variable to use in LUA
/// </summary>
/// <param name="lua">The sol state</param>
void EmitterCom::RegisterLua(sol::state& lua)
{
	lua.new_usertype<EmitterCom>("EmitterCom",
		//---------- Constructors ----------
		sol::constructors<
		EmitterCom()
		>(),

		//---------- Data ----------
		"isInitialised", &EmitterCom::isInitialised,
		"timer", &EmitterCom::timer,
		"timeset", &EmitterCom::timeset,
		"poolIndex", &EmitterCom::poolIndex,
		"maxParticles", &EmitterCom::maxParticles,
		//"particles", &EmitterCom::particles,
		"position", &EmitterCom::position,
		"velocity", &EmitterCom::velocity,
		"velocityVariation", &EmitterCom::velocityVariation,
		"colorBegin", &EmitterCom::colorBegin,
		"colorEnd", &EmitterCom::colorEnd,
		"sizeBegin", &EmitterCom::sizeBegin,
		"sizeEnd", &EmitterCom::sizeEnd,
		"sizeVariation", &EmitterCom::sizeVariation,
		"lifeTime", &EmitterCom::lifeTime,
		"rotation", &EmitterCom::rotation,
		"emit", &EmitterCom::emit
		); //END
}

/// <summary>
/// Binding LuaScript component's member functions and variable to use in LUA
/// </summary>
/// <param name="lua">The sol state</param>
void LuaScriptCom::RegisterLua(sol::state& lua)
{
	lua.new_usertype<LuaScriptCom>("LuaScriptCom",
		//---------- Constructors ----------
		sol::constructors<
		LuaScriptCom(),
		LuaScriptCom(std::string)
		>(),

		//---------- Member Function ----------
		"GetEnvironment", &LuaScriptCom::GetEnvironment,
		"GetScriptVariable", &LuaScriptCom::GetScriptVariable,
		"Load", &LuaScriptCom::Load,
		"Awake", &LuaScriptCom::Awake,
		"Start", &LuaScriptCom::Start
		);
}

/// <summary>
/// Binding Audio Source component's member functions and variable to use in LUA
/// </summary>
/// <param name="lua">The sol state</param>
void AudioSrcCom::RegisterLua(sol::state& lua)
{
	lua.new_usertype<AudioSrcCom>("AudioSrcCom",
		
		//---------- Data ----------
		"volume", &AudioSrcCom::volume,
		"mute", &AudioSrcCom::mute
		); //END
}

/// <summary>
/// Binding Sprite component's member functions and variable to use in LUA
/// </summary>
/// <param name="lua">The sol state</param>
void TextCom::RegisterLua(sol::state& lua)
{
	lua.new_usertype<TextCom>("TextCom",

		"setName", & TextCom::setName,

		//---------- Data ----------
		"TextName", & TextCom::TextName,
		"_fontType", & TextCom::_fontType,
		"_pos", & TextCom::_pos,
		"_colour", & TextCom::_colour,
		"_size", & TextCom::_size
		); //END
}

/// <summary>
/// Binding Sprite component's member functions and variable to use in LUA
/// </summary>
/// <param name="lua">The sol state</param>
void SpriteCom::RegisterLua(sol::state& lua)
{
	lua.new_usertype<SpriteCom>("SpriteCom",

		"setName", &SpriteCom::setName,

		//---------- Data ----------
		"SpriteName", & SpriteCom::SpriteName,
		"_colour", & SpriteCom::_colour,
		"_alphaVal", & SpriteCom::_alphaVal,
		"_drawSprite", & SpriteCom::_drawSprite,
		"_brightVal", & SpriteCom::_brightVal,
		"_drawIn3D", & SpriteCom::_drawIn3D,
		"_useSprite", & SpriteCom::_useSprite,
		"_isBar", & SpriteCom::_isBar,
		"barPercentage", & SpriteCom::barPercentage,
		"barXAmount", & SpriteCom::barXAmount
  		); //END
}      

/// <summary>
/// Binding Animation component's member functions and variable to use in LUA
/// </summary>
/// <param name="lua">The sol state</param>
void AnimationCom::RegisterLua(sol::state& lua)
{
	lua.new_usertype<AnimationCom>("AnimationCom",
		//---------- Constructors ----------
		sol::constructors<
		AnimationCom()
		>(),

		//---------- Data ----------
		"speed", &AnimationCom::speed,
		"currentAnim", &AnimationCom::currentAnim,
		"pause",&AnimationCom::pause,
		"restart",&AnimationCom::restart
		); //END
}
