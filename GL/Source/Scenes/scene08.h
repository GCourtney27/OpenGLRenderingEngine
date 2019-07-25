#pragma once
#include "core/scene.h"

class Scene08 : public Scene
{
public:
	Scene08(class Engine* engine) : Scene(engine) {}
	virtual ~Scene08() {}

	/*Known issues:
	- Read sccess violation with model initialization
	*/

	bool Initialize() override;
	void Shutdown() override;
	void Update() override;
	void Render() override;

	const char* Name() override { return "Scene 08"; }
	const char* Desc() override { return "Model with dynamic light."; }

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