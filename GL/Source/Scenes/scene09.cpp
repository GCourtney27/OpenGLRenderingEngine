#include "scene09.h"
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


#define NUM_LIGHTS 5
#define GLM_ENABLE_EXPERIMENTAL

bool Scene09::Initialize()
{
	std::cout << "Current graphics driver: " << glGetString(GL_VERSION) << std::endl;

	Camera* camera = this->CreateObject<Camera>();
	camera->Initialize();
	camera->SetView(glm::vec3(0.0f, 0.0f, 20.0f), glm::vec3(0.0f));

	for (int i = 0; i < NUM_LIGHTS; i++)
	{
		Light* light = this->CreateObject<PointLight>();
		light->transform.translation = glm::sphericalRand(7.0f);
		glm::vec3 color = glm::rgbColor(glm::vec3(glm::linearRand(0.0f, 360.0f), 1.0f, 1.0f));
		light->diffuse = color;
		light->specular = glm::vec3(1.0f, 1.0f, 1.0f);
	}

	auto lights = GetObjects<PointLight>();

	Model * model = this->CreateObject<Sphere>("model");
	((Sphere*)(model))->Initialize(1.0f, 40, 40);
	model->transform.scale = (glm::vec3(1.0f));
	
	// Shader load for model
	model->m_shader.CompileShader(m_engine->Get<FileSystem>()->GetPathname() + "Shaders\\Vertex Shaders\\phong.vs", GL_VERTEX_SHADER);
	model->m_shader.CompileShader(m_engine->Get<FileSystem>()->GetPathname() + "Shaders\\Fragment Shaders\\phong_multi.fs", GL_FRAGMENT_SHADER);
	model->m_shader.Link();
	model->m_shader.Use();

	// Model attributes
	model->m_material.ambient = glm::vec3(0.2f, 0.2f, 0.2f); // Background light set on the object
	model->m_material.diffuse = glm::vec3(1.0f, 1.0f, 1.0f); // Color created when light shines on the boject
	model->m_material.specular = glm::vec3(1.0f, 1.0f, 1.0f); // Color of the shine on the object when light is pointed at it
	model->m_material.shininess = 10.0f; // Controls how much specularity the object has. Lower number = more shine
	
	// Load base texture
	model->m_material.AddTexture(m_engine->Get<FileSystem>()->GetPathname() + "Textures\\metal-diffuse.png", GL_TEXTURE0);
	model->m_material.AddTexture(m_engine->Get<FileSystem>()->GetPathname() + "Textures\\metal-normal.png", GL_TEXTURE1);
	
	model->m_shader.SetUniform("material.ambient", model->m_material.ambient);
	model->m_shader.SetUniform("material.diffuse", model->m_material.diffuse);
	model->m_shader.SetUniform("material.specular", model->m_material.specular);
	model->m_shader.SetUniform("material.shininess", model->m_material.shininess);

	model->transform.translation = glm::vec3(1.0f, 1.0f, 1.0f);

	//=======================
	Model * plane = this->CreateObject<Plane>("plane");
	((Plane*)(plane))->Initialize(10.0f, 10.0f, 10, 10, 10.0f, 10.0f);
	plane->transform.scale = (glm::vec3(5.0f));

	// Shader load for plane
	plane->m_shader.CompileShader(m_engine->Get<FileSystem>()->GetPathname() + "Shaders\\Vertex Shaders\\phong.vs", GL_VERTEX_SHADER);
	plane->m_shader.CompileShader(m_engine->Get<FileSystem>()->GetPathname() + "Shaders\\Fragment Shaders\\phong_multi.fs", GL_FRAGMENT_SHADER);
	plane->m_shader.Link();
	plane->m_shader.Use();

	// Model attributes
	plane->m_material.ambient = glm::vec3(0.2f); // Background light set on the object
	plane->m_material.diffuse = glm::vec3(0.5f); // Color created when light shines on the boject
	plane->m_material.specular = glm::vec3(1.0f, 1.0f, 1.0f); // Color of the shine on the object when light is pointed at it
	plane->m_material.shininess = 40.0f; // Controls how much specularity the object has. Lower number = more shine

	// Load base texture
	plane->m_material.AddTexture(m_engine->Get<FileSystem>()->GetPathname() + "Textures\\cobble-diffuse.tga", GL_TEXTURE0);

	plane->m_shader.SetUniform("material.ambient", plane->m_material.ambient);
	plane->m_shader.SetUniform("material.diffuse", plane->m_material.diffuse);
	plane->m_shader.SetUniform("material.specular", plane->m_material.specular);
	plane->m_shader.SetUniform("material.shininess", plane->m_material.shininess);
	
	plane->transform.translation = glm::vec3(1.0f, -10.0f, 1.0f);

	return true;
}


void Scene09::Update()
{
	float dt = m_engine->Get<Timer>()->DeltaTime();
	
	// Update objects
	std::vector<Object*>objects = this->GetObjects<Object>();
	for (Object* object : objects)
	{
		object->Update();
	}

	Camera* camera = this->GetObject<Camera>();

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
	}
}

void Scene09::Render()
{
	m_engine->Get<Renderer>()->ClearBuffer();

	std::vector<Renderable*> renderables = this->GetObjects<Renderable>();
	for (Renderable* renderable : renderables)
	{
		renderable->Draw();
	}

	m_engine->Get<Renderer>()->SwapBuffer();
}

void Scene09::Shutdown()
{

}
