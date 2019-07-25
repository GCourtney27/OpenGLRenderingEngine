#pragma once
#include "core/scene.h"

class Scene09 : public Scene
{
public:
	Scene09(class Engine* engine) : Scene(engine) {}
	virtual ~Scene09() {}

	bool Initialize() override;
	void Shutdown() override;
	void Update() override;
	void Render() override;

	const char* Name() override { return "Scene 09"; }
	const char* Desc() override { return "Multiple lights in one scene."; }

	void* operator new(size_t i)
	{
		return _mm_malloc(i, 16);
	}

	void operator delete(void* p)
	{
		_mm_free(p);
	}

private:

	glm::vec2 m_uvOffset = glm::vec2(1.0f, 1.0f);
	float m_direction = 1.0f;
};