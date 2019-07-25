#pragma once
#include "core/scene.h"

class Scene10 : public Scene
{
public:
	Scene10(class Engine* engine) : Scene(engine) {}
	virtual ~Scene10() {}

	bool Initialize() override;
	void Shutdown() override;
	void Update() override;
	void Render() override;

	const char* Name() override { return "Scene 10"; }
	const char* Desc() override { return "Cubemap reflection"; }

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