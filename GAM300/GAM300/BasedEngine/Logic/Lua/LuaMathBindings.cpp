#include "Physics/PhysicsSystem.h"
#include "MathLib/Random.h"
#include "MathLib/Vector3.h"
#include "MathLib/Vector4.h"
#include "MathLib/Quat.h"
#include "MathLib/Trigo.h"

//Math Library Bindings

/// <summary>
/// Binding Random functions to LUA
/// </summary>
/// <param name="lua">The sol state</param>
void Random::RegisterLua(sol::state& lua)
{
	lua.new_usertype<Random>("Random",

		//---------- Member Functions ----------
		"Init", &Random::Init,
		"Float", &Random::Float,
        "randInRange", &Random::randInRange,
        "randIntInRange", &Random::randIntInRange
		);
}

/// <summary>
/// Binding Trigo functions to LUA
/// </summary>
/// <param name="lua">The sol state</param>
void Trigo::RegisterLua(sol::state& lua)
{
    lua.new_usertype<Trigo>("Trigo",

        //---------- Member Functions ----------
        "sin", &Trigo::Sin,
        "cos", &Trigo::Cos,
        "tan", &Trigo::Tan,
        "asin", &Trigo::aSin,
        "acos", &Trigo::aCos,
        "atan", &Trigo::aTan,
        "atan2", & Trigo::aTan2,
        "pi", &Trigo::Pi,
        "floatToint", & Trigo::floatToint
        );
}

/// <summary>
/// Binding Vector3 functions to LUA
/// </summary>
/// <param name="lua">The sol state</param>
void Vector3::RegisterLua(sol::state& lua)
{
    lua.new_usertype<Vector3>("Vector3",
        //---------- Constructors ----------
        sol::constructors<
        Vector3(),
        //Vector3(float, float), // NC
        Vector3(float, float, float),
        Vector3(const Vector3&)
        >(),

        //---------- Data ----------
        "x", &Vector3::x,
        "y", &Vector3::y,
        "z", &Vector3::z,


        //---------- Member Function ----------
        "Set", &Vector3::Set,
        "Splat", &Vector3::Splat,
        "ScaleByVector", &Vector3::ScaleByVector,
        "ZeroOut", &Vector3::ZeroOut,
        "Reflect", &Vector3::Reflect,
        "Project",
        sol::overload(
            sol::resolve<Vector3(const Vector3&)>(&Vector3::Project),
            sol::resolve<Vector3(const Vector3&, const Vector3&)>(Vector3::Project) // static class function
        ),
        "AddScaledVector", &Vector3::AddScaledVector,
        "Dot", // There are function overloads for "Vector3::Project", so we need to specify them.
        sol::overload(
            //sol::resolve<float(Vec3Param)>(&Vector3::Dot),
            sol::resolve<float(Vec3Param, Vec3Param)>(Vector3::Dot) // static class function
        ),
        "Length", &Vector3::Length,
        "LengthSq", &Vector3::LengthSq,
        "Normalized", &Vector3::Normalized,
        "Normalize", &Vector3::Normalize,
        "Negate", &Vector3::Negate,
        "Cross", &Vector3::Cross,


        // sample on how to register overloaded func
        //"ScaleByVector", & Vector3::ScaleByVector,
        //sol::overload(
        //    sol::resolve<void(float)>(&Vector3::ScaleByVector),
        //    sol::resolve<void(const Vector3&)>(&Vector3::ScaleByVector)
        //),

        "Lerp", &Vector3::Lerp,
        "LerpUnclamped", &Vector3::LerpUnclamped,

        "ProjectOnPlane", &Vector3::ProjectOnPlane,
        "MoveTowards", &Vector3::MoveTowards,

        "SmoothDamp", &Vector3::SmoothDamp,

        "Min", &Vector3::Min,
        "Max", &Vector3::Max,
        "Angle", &Vector3::Angle,

        "Vec3toPVex3", &Vector3::Vec3toPVex3,

        "Distance", &Vector3::Distance,

        //---------- Operator Overloading ----------
        sol::meta_function::addition,
        sol::overload(
            sol::resolve<Vector3(const Vector3&, const Vector3&)>(operator+)
        ),

        sol::meta_function::subtraction,
        sol::overload(
            sol::resolve<Vector3(const Vector3&, const Vector3&)>(operator-)
        ),

        sol::meta_function::multiplication,
        sol::overload(
            sol::resolve<Vector3(const Vector3&, float)>(operator*),
            sol::resolve<Vector3(const Vector3&, const Vector3&)>(operator*)
        ),

        sol::meta_function::division,
        sol::overload(
            sol::resolve<Vector3(const Vector3&, float)>(operator/),
            sol::resolve<Vector3(const Vector3&, const Vector3&)>(operator/)
        ),

        sol::meta_function::unary_minus,
        sol::overload(
            sol::resolve<Vector3(const Vector3&)>(operator-)
        ),

        sol::meta_function::equal_to,
        sol::resolve<bool(const Vector3&, const Vector3&)>(operator==)
        );//END

}

/// <summary>
/// Binding Vector4 functions to LUA
/// </summary>
/// <param name="lua">The sol state</param>
void Vector4::RegisterLua(sol::state& lua)
{
    lua.new_usertype<Vector4>("Vector4",
        //---------- Constructors ----------
        sol::constructors<
        Vector4(),
        //Vector4(float, float), // NC
        Vector4(float, float, float, float),
        Vector4(const Vector4&)
        >(),

        //---------- Data ----------
        "x", &Vector4::x,
        "y", &Vector4::y,
        "z", &Vector4::z,
        "w", &Vector4::w,


        //---------- Member Function ----------
        "Set", &Vector4::Set,
        "Splat", &Vector4::Splat,
        "ScaleByVector", &Vector4::ScaleByVector,
        "ZeroOut", &Vector4::ZeroOut,
        "AddScaledVector", &Vector4::AddScaledVector,
        "Dot", &Vector4::Dot,
        "Length", &Vector4::Length,
        "LengthSq", &Vector4::LengthSq,
        "Normalized", &Vector4::Normalized,
        "Normalize", &Vector4::Normalize,
        "Negate", &Vector4::Negate,


        // sample on how to register overloaded func
        //"ScaleByVector", & Vector4::ScaleByVector,
        //sol::overload(
        //    sol::resolve<void(float)>(&Vector4::ScaleByVector),
        //    sol::resolve<void(const Vector4&)>(&Vector4::ScaleByVector)
        //),

        "Min", &Vector4::Min,
        "Max", &Vector4::Max,


        //---------- Operator Overloading ----------
        sol::meta_function::addition,
        sol::overload(
            sol::resolve<Vector4(const Vector4&, const Vector4&)>(operator+)
        ),

        sol::meta_function::subtraction,
        sol::overload(
            sol::resolve<Vector4(const Vector4&, const Vector4&)>(operator-)
        ),

        sol::meta_function::multiplication,
        sol::overload(
            sol::resolve<Vector4(const Vector4&, float)>(operator*),
            sol::resolve<Vector4(const Vector4&, const Vector4&)>(operator*)
        ),

        sol::meta_function::division,
        sol::overload(
            sol::resolve<Vector4(const Vector4&, float)>(operator/),
            sol::resolve<Vector4(const Vector4&, const Vector4&)>(operator/)
        ),

        sol::meta_function::unary_minus,
        sol::overload(
            sol::resolve<Vector4(const Vector4&)>(operator-)
        ),

        sol::meta_function::equal_to,
        sol::resolve<bool(const Vector4&, const Vector4&)>(operator==)
        );//END

}

/// <summary>
/// Binding Quat functions to LUA
/// </summary>
/// <param name="lua">The sol state</param>
void Quat::RegisterLua(sol::state& lua)
{
    lua.new_usertype<Quat>("Quat",
        //---------- Constructors ----------
        sol::constructors<
        Quat(),
        Quat(float, float, float, float),
        Quat(const Quat&)
        >(),

        //---------- Data ----------
        "x", &Quat::x,
        "y", &Quat::y,
        "z", &Quat::z,
        "w", &Quat::w,

        //---------- Member Function ----------
        "RotateTowards", &Quat::RotateTowards,
        "Slerp", &Quat::Slerp,
        "SlerpUnclamped", &Quat::SlerpUnclamped
        );//END
}