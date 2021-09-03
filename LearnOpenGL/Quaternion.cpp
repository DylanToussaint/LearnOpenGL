#include "Quaternion.h"

#include <cmath>

Quaternion::Quaternion(const glm::vec3 vector, float a) {
	//A is rotation angle
	//Vector is the angle axis
	//convet from degrees to radians
	a = glm::radians(a);

	w = cos(a / 2);
	v = vector * sin(a / 2);

}

const Quaternion Quaternion::Invert() const {
	Quaternion q;
	q.w = w;
	q.v = -v;
	return q;
}

const Quaternion Quaternion::operator*(const Quaternion& q) const {
	Quaternion r;
	return r;
}

//For this to be useful I would need a function that would transfer it to a matrix
//I would then need my very own vector and matrix classess