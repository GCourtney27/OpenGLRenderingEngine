#pragma once
#include "core/scene.h"

class Scene07 : public Scene
{
public:
	Scene07(class Engine* engine) : Scene(engine) {}
	virtual ~Scene07() {}

	/*Known issues:
	- Possible malfunction of light ambience
	*/

	bool Initialize() override;
	void Shutdown() override;
	void Update() override;
	void Render() override;

	const char* Name() override { return "Scene 07"; }
	const char *Desc() override { return "Dynamic switching of directional and point light (Use 1 and 2 keys). Fog implementation."; }
	void* operator new(size_t i)
	{
		return _mm_malloc(i, 16);
	}

	void operator delete(void* p)
	{
		_mm_free(p);
	}

private:
	int m_lightIndex = 1;

	glm::vec2 m_uvOffset = glm::vec2(1.0f, 1.0f);
	float m_direction = 1.0f;
};