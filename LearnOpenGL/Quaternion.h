#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Quaternion
{
public:
	Quaternion() {};
	Quaternion(const glm::vec3 vector, float a);

public:
	const Quaternion Invert() const;
	const Quaternion operator*(const Quaternion& q) const;

public:
	float w;
	glm::vec3 v;
};

