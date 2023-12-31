#include "easeings.h"

void easeings::Spring(float& x, float& v, float xt, float zeta, float omega, float h)
{
	const float f = 1.0f + 2.0f * h * zeta * omega;
	const float oo = omega * omega;
	const float hoo = h * oo;
	const float hhoo = h * hoo;
	const float detInv = 1.0f / (f + hhoo);
	const float detX = f * x + h * v + hhoo * xt;
	const float detV = v + hoo * (xt - x);
	x = detX * detInv;
	v = detV * detInv;
}



