#pragma once

#ifndef PRECOMPILED_H_
#define PRECOMPILED_H_
#define _USE_MATH_DEFINES

#include <cmath>
#include <limits>
#include <cstdio>
#include <cstdlib>
#include <cfloat>
#include <algorithm>
#include <array>

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Quat.h"

#include "Matrix2.h"
#include "Matrix3.h"
#include "Matrix4.h"
#include "Converter.h"


inline float lerp(float a, float b, float f)
{
	return a + f * (b - a);
}

#endif