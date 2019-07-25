#pragma once
#include "core/scene.h"

class Scene04 : public Scene
{
public:
	Scene04(class Engine* engine) : Scene(engine) {}
	virtual ~Scene04() {}

	/*Known issues:
	- Nothing in the scene, possible shader issue
	*/

	bool Initialize() override;
	void Shutdown() override;
	void Update() override;
	void Render() override;

	const char* Name() override { return "Scene 04"; }
	const char* Desc() override { return "Empty scene address known issues in Scene04.h"; }

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

	class Light * m_light = nullptr;

	float m_direction = 1.0f;
};