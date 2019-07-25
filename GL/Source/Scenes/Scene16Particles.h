#pragma once
#include "core/scene.h"
#include "..\Objects\ParticleSystem.h"

class Scene16Particles : public Scene
{
public:
	Scene16Particles(class Engine* engine) : Scene(engine) {}
	virtual ~Scene16Particles() {}

	bool Initialize() override;
	void Shutdown() override;
	void Update() override;
	void Render() override;

	const char* Name() override { return "Scene16Particles"; }
	const char* Desc() override { return "Particle System"; }
	
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
	GLuint m_framebuffer = 0;
	float m_time = 0.0f;
};