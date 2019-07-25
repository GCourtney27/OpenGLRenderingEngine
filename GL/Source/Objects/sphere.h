#pragma once

#include "objects/model.h"

class Sphere : public Model
{
public:
	Sphere(Scene* scene, const std::string& name = "") : Model(scene, name) {}

	void Initialize(GLfloat radius, GLuint slices, GLuint stacks);

	void* operator new(size_t i)
	{
		return _mm_malloc(i, 16);
	}

	void operator delete(void* p)
	{
		_mm_free(p);
	}

};