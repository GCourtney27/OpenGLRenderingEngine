#pragma once

#include "core/object.h"
#include "core/engine.h"

class Renderable : public Object
{
public:
	Renderable(Scene* scene, const std::string& name = "") : Object(scene, name) {}
	virtual ~Renderable() {}

	virtual void Draw() = 0;

	void* operator new(size_t i)
	{
		return _mm_malloc(i, 16);
	}

	void operator delete(void* p)
	{
		_mm_free(p);
	}
};