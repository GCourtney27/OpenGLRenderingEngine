#pragma once
#include "core/scene.h"

class Scene14 : public Scene
{
public:
	Scene14(class Engine* engine) : Scene(engine) {}
	virtual ~Scene14() {}

	bool Initialize() override;
	void Shutdown() override;
	void Update() override;
	void Render() override;

	const char* Name() override { return "Scene 14"; }
	const char* Desc() override { return "Shadow mapping"; }

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
private:
	GLuint m_depthBuffer = 0;
	Shader* m_depthShader = nullptr;

};