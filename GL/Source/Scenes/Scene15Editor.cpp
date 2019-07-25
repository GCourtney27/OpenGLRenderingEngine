#include "scene15Editor.h"
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


#define NUM_LIGHTS 2
#define GLM_ENABLE_EXPERIMENTAL

bool Scene15Editor::Initialize()
{
	Camera* camera = this->CreateObject<Camera>();
	camera->Initialize();
	camera->SetView(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	for (int i = 0; i < NUM_LIGHTS; i++)
	{
		Light* light = this->CreateObject<PointLight>();
		light->transform.translation = glm::sphericalRand(7.0f);
		glm::vec3 color = glm::rgbColor(glm::vec3(glm::linearRand(0.0f, 360.0f), 1.0f, 1.0f));
		light->diffuse = color;
		light->specular = glm::vec3(1.0f, 1.0f, 1.0f);
	}

	auto lights = GetObjects<PointLight>();

	// Model 1
	Model* model = this->CreateObject<Model>("main");
	model->Import(m_engine->Get<FileSystem>()->GetPathname() + "Meshes\\cube.obj");
	model->transform.translation = glm::vec3(0.0f, 0.0f, -2.0f);
	model->transform.scale = (glm::vec3(1.0f));
	model->transform.rotation = glm::quat(glm::vec3(glm::radians(glm::sin(90.0f) * 100.0f), 0.0f, 0.0f));

	// Shader load
	model->m_shader.CompileShader(m_engine->Get<FileSystem>()->GetPathname() + "Shaders\\Vertex Shaders\\development.vs", GL_VERTEX_SHADER);
	model->m_shader.CompileShader(m_engine->Get<FileSystem>()->GetPathname() + "Shaders\\Fragment Shaders\\development.fs", GL_FRAGMENT_SHADER);
	model->m_shader.Link();
	model->m_shader.Use();

	// Material attributes
	model->m_material.ambient = glm::vec3(0.0f);
	model->m_material.diffuse = glm::vec3(1.0f);
	model->m_material.specular = glm::vec3(1.0f);
	model->m_material.shininess = 50.0f;//Lower number is more shine

	// Load textures
	model->m_material.AddTexture(m_engine->Get<FileSystem>()->GetPathname() + "Textures\\metal-diffuse.png", GL_TEXTURE0);
	model->m_material.AddTexture(m_engine->Get<FileSystem>()->GetPathname() + "Textures\\metal-normal.png", GL_TEXTURE1);
	model->m_material.SetTextures();

	// Set shaders
	model->m_shader.SetUniform("material.ambient", model->m_material.ambient);
	model->m_shader.SetUniform("material.diffuse", model->m_material.diffuse);
	model->m_shader.SetUniform("material.specular", model->m_material.specular);
	model->m_shader.SetUniform("material.shininess", model->m_material.shininess);

	//================================

	// Model2
	Model * plane = this->CreateObject<Plane>("plane");
	plane->Import(m_engine->Get<FileSystem>()->GetPathname() + "Meshes\\plane.obj");
	//((Plane*)(plane))->Initialize(10.0f, 10.0f, 10, 10, 10.0f, 10.0f);
	plane->transform.translation = glm::vec3(1.0f, -10.0f, 1.0f);
	plane->transform.scale = (glm::vec3(5.0f));

	// Shader load
	plane->m_shader.CompileShader(m_engine->Get<FileSystem>()->GetPathname() + "Shaders\\Vertex Shaders\\development.vs", GL_VERTEX_SHADER);
	plane->m_shader.CompileShader(m_engine->Get<FileSystem>()->GetPathname() + "Shaders\\Fragment Shaders\\development.fs", GL_FRAGMENT_SHADER);
	plane->m_shader.Link();
	plane->m_shader.Use();

	// Material attributes
	plane->m_material.ambient = glm::vec3(0.2f); // Background light set on the object
	plane->m_material.diffuse = glm::vec3(0.5f); // Color created when light shines on the boject
	plane->m_material.specular = glm::vec3(1.0f, 1.0f, 1.0f); // Color of the shine on the object when light is pointed at it
	plane->m_material.shininess = 40.0f; // Controls how much specularity the object has. Lower number = more shine

	// Load textures
	plane->m_material.AddTexture(m_engine->Get<FileSystem>()->GetPathname() + "Textures\\cobble-diffuse.tga", GL_TEXTURE0);
	plane->m_material.AddTexture(m_engine->Get<FileSystem>()->GetPathname() + "Textures\\cobble-normal.tga", GL_TEXTURE1);
	plane->m_material.SetTextures();

	// Set shaders
	plane->m_shader.SetUniform("material.ambient", plane->m_material.ambient);
	plane->m_shader.SetUniform("material.diffuse", plane->m_material.diffuse);
	plane->m_shader.SetUniform("material.specular", plane->m_material.specular);
	plane->m_shader.SetUniform("material.shininess", plane->m_material.shininess);


	return true;
}

void Scene15Editor::Shutdown()
{

}


void Scene15Editor::Update()
{
	float dt = m_engine->Get<Timer>()->DeltaTime();

	Camera* camera = this->GetObject<Camera>();

	Model* model = this->GetObject<Model>("main");
	model->transform.rotation = model->transform.rotation * glm::quat(glm::vec3(0.0f, 0.0f, 0.05f) * dt);
	
	// Update objects
	std::vector<Object*>objects = this->GetObjects<Object>();
	for (Object* object : objects)
	{
		object->Update();
	}

	// update lights    
	auto lights = GetObjects<Light>();
	for (int i = 0; i < lights.size(); i++)
	{
		// calculate light position
		Light* light = lights[i];
		glm::vec4 lightPosition = ((PointLight*)light)->GetPositionFromView(camera->transform.matrix);

		// update models with light
		auto models = GetObjects<Model>();
		for (auto model : models)
		{
			model->m_shader.Use();
			char uniformName1[32]; sprintf_s(uniformName1, "lights[%d].position", i);
			model->m_shader.SetUniform(uniformName1, lightPosition);

			char uniformName2[32]; sprintf_s(uniformName2, "lights[%d].diffuse", i);
			model->m_shader.SetUniform(uniformName2, light->diffuse);

			char uniformName3[32]; sprintf_s(uniformName3, "lights[%d].specular", i);
			model->m_shader.SetUniform(uniformName3, light->specular);
		}
		auto planes = GetObjects<Plane>();
		for (auto plane : planes)
		{
			plane->m_shader.Use();
			char uniformName1[32]; sprintf_s(uniformName1, "lights[%d].position", i);
			plane->m_shader.SetUniform(uniformName1, lightPosition);

			char uniformName2[32]; sprintf_s(uniformName2, "lights[%d].diffuse", i);
			plane->m_shader.SetUniform(uniformName2, light->diffuse);

			char uniformName3[32]; sprintf_s(uniformName3, "lights[%d].specular", i);
			plane->m_shader.SetUniform(uniformName3, light->specular);
		}
	}


	//ImGui Options
	m_engine->Get<UI>()->Start();

	ImGui::Begin("Cube Translation");
	Transform::Edit(&model->transform);
	ImGui::End();
	
	ImGui::Begin("sphere transform");

	/*if (ImGui::Button("Create Point Light"))
	{
		PointLight * newLight = this->CreateObject<PointLight>();
		newLight->transform.translation = glm::vec3(0.0f, 0.0f, 0.0f);
		newLight->diffuse = glm::vec3(0.0f, 0.0f, 1.0f);
		newLight->specular = glm::vec3(1.0f, 1.0f, 1.0f);
		newLight->transform.translation = glm::vec3(0.0f, 0.0f, 3.0f);
	}*/
	if (ImGui::Button("Create Cube"))
	{
		Cube * newCube = this->CreateObject<Cube>();
		newCube->transform.translation = glm::vec3(0.0f, 0.0f, 0.0f);
	}
	ImGui::End();


}

void Scene15Editor::Render()
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

