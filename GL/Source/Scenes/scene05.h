#pragma once
#include "core/scene.h"

class Scene05 : public Scene
{
public:
	Scene05(class Engine* engine) : Scene(engine) {}
	virtual ~Scene05() {}

	/*Known issues:
	- Nothing in the scene, possible shader issue
	*/

	bool Initialize() override;
	void Shutdown() override;
	void Update() override;
	void Render() override;

	const char* Name() override { return "Scene 05"; }
	const char* Desc() override { return "Empty scene, address known issues in Scene05.h"; }

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