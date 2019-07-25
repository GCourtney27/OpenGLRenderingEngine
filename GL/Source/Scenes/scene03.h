#pragma once
#include "core/scene.h"

class Scene03 : public Scene
{
public:
	Scene03(class Engine* engine) : Scene(engine) {}
	virtual ~Scene03() {}

	/*Known issues:
	- Rotation on ny axis will caus madel to stretch and movetoward the camera
	- No texture added
	*/

	bool Initialize() override;
	void Shutdown() override;
	void Update() override;
	void Render() override;

	const char* Name() override { return "Scene 03"; }
	const char* Desc() override { return "Renders a 3D cube with texture. Does not load a file for vertecies or normals, relies on vertex buffer arrays in code. Up/Down arrows move on Z axis"; }

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
	class Camera * m_camera = nullptr;
	Material * m_material = nullptr;
	class Light * m_light = nullptr;
	
	glm::mat4 m_mxModel;
	glm::mat4 m_mxProjection;

	Transform m_transform;

	float m_direction = 1.0f;
};