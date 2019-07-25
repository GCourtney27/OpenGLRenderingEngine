#include "scene12.h"
#include "Objects/light.h"
#include "Objects/camera.h"
#include "Objects/sphere.h"
#include "Objects/cube.h"
#include "Objects/point_light.h"
#include "Objects/plane.h"
#include "Objects/directional_light.h"
#include "Objects/spot_light.h"
#include <iostream>

#include <glm/gtc/random.hpp>
#include <glm/gtx/color_space.hpp>


bool Scene12::Initialize()
{
	Camera* camera = this->CreateObject<Camera>();
	camera->Initialize();
	camera->SetView(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	PointLight* light = this->CreateObject<PointLight>("point_light");
	light->diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	light->specular = glm::vec3(1.0f, 1.0f, 1.0f);
	light->transform.translation = glm::vec3(1.0f, 0.0f, 3.0f);

	// Model 1
	Model* model = this->CreateObject<Model>("model1");
	model->Import(m_engine->Get<FileSystem>()->GetPathname() + "Meshes\\plane.obj");
	model->transform.translation = glm::vec3(0.0f, 0.0f, 2.0f);
	model->transform.rotation = glm::quat(glm::vec3(glm::radians(glm::sin(90.0f) * 100.0f), 0.0f, 0.0f));

	model->m_shader.CompileShader(m_engine->Get<FileSystem>()->GetPathname() + "Shaders\\Vertex Shaders\\phong_nm.vs", GL_VERTEX_SHADER);
	model->m_shader.CompileShader(m_engine->Get<FileSystem>()->GetPathname() + "Shaders\\Fragment Shaders\\phong_nm.fs", GL_FRAGMENT_SHADER);

	model->m_shader.Link();
	model->m_shader.Use();

	model->m_material.ambient = glm::vec3(0.0f);
	model->m_material.diffuse = glm::vec3(1.0f);
	model->m_material.specular = glm::vec3(1.0f);
	model->m_material.shininess = 50.0f;//Lower number is more shine

	model->m_material.AddTexture(m_engine->Get<FileSystem>()->GetPathname() + "Textures\\rocks.jpg", GL_TEXTURE0);
	model->m_material.AddTexture(m_engine->Get<FileSystem>()->GetPathname() + "Textures\\rocks_normal.jpg", GL_TEXTURE1);
	model->m_material.SetTextures();

	model->m_shader.SetUniform("material.ambient", model->m_material.ambient);
	model->m_shader.SetUniform("material.diffuse", model->m_material.diffuse);
	model->m_shader.SetUniform("material.specular", model->m_material.specular);
	model->m_shader.SetUniform("material.shininess", model->m_material.shininess);

	return true;
}

void Scene12::Shutdown()
{

}


void Scene12::Update()
{
	float dt = m_engine->Get<Timer>()->DeltaTime();

	PointLight* light = this->GetObject<PointLight>("point_light");
	light->transform.translation = light->transform.translation * glm::quat(glm::vec3(0.0f, 0.0f, glm::radians(45.0f * dt)));

	std::vector<Object*> objects = this->GetObjects<Object>();
	for (Object* object : objects)
	{
		object->Update();
	}

	Camera* camera = this->GetObject<Camera>();
	glm::vec4 lightPosition = light->GetPositionFromView(camera->transform.matrix);

	std::vector<Model*> models = this->GetObjects<Model>();
	for (Model* model : models)
	{
		model->m_shader.Use();

		model->m_shader.SetUniform("light.position", lightPosition);
		model->m_shader.SetUniform("light_position", lightPosition);

		model->m_shader.SetUniform("light.diffuse", light->diffuse);
		model->m_shader.SetUniform("light.specular", light->specular);

		model->m_shader.SetUniform("material.ambient", model->m_material.ambient);
		model->m_shader.SetUniform("material.diffuse", model->m_material.diffuse);
		model->m_shader.SetUniform("material.specular", model->m_material.specular);
		model->m_shader.SetUniform("material.shininess", model->m_material.shininess);
	}

	m_engine->Get<UI>()->Start();

}
void Scene12::Render()
{
	m_engine->Get<Renderer>()->ClearBuffer();

	std::vector<Renderable*> renderables = this->GetObjects<Renderable>();
	for (Renderable* renderable : renderables)
	{
		renderable->Draw();
	}
	m_engine->Get<UI>()->Draw();

	m_engine->Get<Renderer>()->SwapBuffer();
}

