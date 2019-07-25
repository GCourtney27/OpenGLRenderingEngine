#pragma once
#include "core/scene.h"

class Scene01 : public Scene
{
public:
	Scene01(class Engine* engine) : Scene(engine) {}
	virtual ~Scene01() {}

	bool Initialize() override;
	void Shutdown() override;
	void Update() override;
	void Render() override;

	const char* Name() override { return "Scene 01"; }
	const char* Desc() override { return "Renders a 2D quad with texture. Does not load a file for vertecies or normals, relies on vertex buffer arrays in code"; }
	
	void* operator new(size_t i)
	{
		return _mm_malloc(i, 16);
	}

	void operator delete(void* p)
	{
		_mm_free(p);
	}
	
private:
	Shader * m_shader = nullptr;
	VertexArray * m_vertexArray = nullptr;
	
	glm::mat4 m_mxModel;
	glm::mat4 m_mxView;
	glm::mat4 m_mxProjection;

	Transform m_transform;

	float m_direction = 1.0f;
	glm::vec3 m_cameraPosition;

	
};