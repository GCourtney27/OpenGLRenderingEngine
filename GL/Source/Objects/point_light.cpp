#include "point_light.h"
#include "core/scene.h"
#include "cube.h"

PointLight::PointLight(Scene* scene, const std::string& name) : Light(scene, name)
{
	m_model = new Cube(scene);
	m_model->Initialize(0.5f);

	m_model->m_shader.CompileShader(scene->m_engine->Get<FileSystem>()->GetPathname() + "Shaders\\Vertex Shaders\\basic_color.vs", GL_VERTEX_SHADER);
	m_model->m_shader.CompileShader(scene->m_engine->Get<FileSystem>()->GetPathname() + "shaders\\Fragment Shaders\\basic_color.fs", GL_FRAGMENT_SHADER);
	m_model->m_shader.Link();
}

void PointLight::Update()
{
	transform.Update();
	m_model->transform = transform;
	m_model->Update();
	m_model->m_shader.SetUniform("material.diffuse", diffuse);
}

void PointLight::Draw()
{
	m_model->Draw();
}

glm::vec4 PointLight::GetPositionFromView(const glm::mat4& mxView)
{
	return mxView * transform.matrix * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
}
