#pragma once
class easeings
{
	/*
	x     - value             (input/output)
	v     - velocity          (input/output)
	xt    - target value      (input)
	zeta  - damping ratio     (input)
	omega - angular frequency (input)
	h     - time step         (input)
	*/
	void Spring(float& x, float& v, float xt, float zeta, float omega, float h);



};

