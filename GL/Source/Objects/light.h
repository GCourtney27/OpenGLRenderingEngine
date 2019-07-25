#pragma once

#include "renderable.h"

class Light : public Renderable
{
public:
	Light(Scene* scene, const std::string& name = "") : Renderable(scene, name) {}

	void Update();

	void* operator new(size_t i)
	{
		return _mm_malloc(i, 16);
	}

	void operator delete(void* p)
	{
		_mm_free(p);
	}

public:
	glm::vec3 diffuse;
	glm::vec3 specular;
};