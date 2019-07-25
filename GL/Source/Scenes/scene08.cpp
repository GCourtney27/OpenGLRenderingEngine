#include "scene08.h"
#include "Objects/light.h"
#include "Objects/camera.h"
#include "Objects/sphere.h"
#include "Objects/cube.h"
#include "Objects/point_light.h"
#include "Objects/plane.h"
#include "Objects/directional_light.h"
#include "Objects/spot_light.h"
#include <iostream>

bool Scene08::Initialize()
{
	std::cout << "Current graphics driver: " << glGetString(GL_VERSION) << std::endl;

	Camera* camera = this->CreateObject<Camera>();
	camera->Initialize();
	camera->SetView(glm::vec3(0.0f, 0.0f, 20.0f), glm::vec3(0.0f));

	Light* light = this->CreateObject<PointLight>();
	light->transform.translation = glm::vec3(30.0f, 3.0f, 10.0f);
	light->diffuse = glm::vec3(0.0f, 1.0f, 0.0f);
	light->specular = glm::vec3(1.0f, 1.0f, 1.0f);

	// Light attributes
	SpotLight* spot_light = this->CreateObject<SpotLight>("spot_light");
	spot_light->diffuse = glm::vec3(0.0f, 0.0f, 1.0f);
	spot_light->specular = glm::vec3(1.0f, 1.0f, 1.0f);
	spot_light->cutoff = glm::radians(45.0f);
	spot_light->exponent = 10.0f;
	spot_light->transform.translation = glm::vec3(5.0f, 5.0f, 1.0f);

	spot_light->SetDirection(spot_light->transform.translation, glm::vec3(0.0f, 0.0f, 0.0f));


	Model * model = this->CreateObject<Model>("model");
	model->Import(m_engine->Get<FileSystem>()->GetPathname() + "Meshes\\dragon.obj");
	model->transform.scale = (glm::vec3(1.0f));

	Model * plane = this->CreateObject<Plane>("plane");
	((Plane*)(plane))->Initialize(10.0f, 10.0f, 10, 10, 10.0f, 10.0f);
	plane->transform.scale = (glm::vec3(5.0f));


	// Shader load for model
	model->m_shader.CompileShader(m_engine->Get<FileSystem>()->GetPathname() + "Shaders\\Vertex Shaders\\phong_uv.vs", GL_VERTEX_SHADER);
	model->m_shader.CompileShader(m_engine->Get<FileSystem>()->GetPathname() + "Shaders\\Fragment Shaders\\phong_spotlight.fs", GL_FRAGMENT_SHADER);
	model->m_shader.Link();
	model->m_shader.Use();

	// Model attributes
	model->m_material.ambient = glm::vec3(1.0, 1.0f, 1.0f); // Background light set on the object
	model->m_material.diffuse = glm::vec3(1.0f, 1.0f, 1.0f); // Color created when light shines on the boject
	model->m_material.specular = glm::vec3(1.0f, 1.0f, 1.0f); // Color of the shine on the object when light is pointed at it
	model->m_material.shininess = 10.0f; // Controls how much specularity the object has. Lower number = more shine
	
	// Load base texture
	model->m_material.AddTexture(m_engine->Get<FileSystem>()->GetPathname() + "Textures\\metal-diffuse.png", GL_TEXTURE0);
	
	model->m_shader.SetUniform("material.ambient", model->m_material.ambient);
	model->m_shader.SetUniform("material.diffuse", model->m_material.diffuse);
	model->m_shader.SetUniform("material.specular", model->m_material.specular);
	model->m_shader.SetUniform("material.shininess", model->m_material.shininess);

	model->m_shader.SetUniform("light.diffuse", light->diffuse);
	model->m_shader.SetUniform("light.specular", light->specular);
	model->transform.translation = glm::vec3(1.0f, 1.0f, 1.0f);

	//=======================

	// Shader load for plane
	plane->m_shader.CompileShader(m_engine->Get<FileSystem>()->GetPathname() + "Shaders\\Vertex Shaders\\phong_uv.vs", GL_VERTEX_SHADER);
	plane->m_shader.CompileShader(m_engine->Get<FileSystem>()->GetPathname() + "Shaders\\Fragment Shaders\\phong_spotlight.fs", GL_FRAGMENT_SHADER);
	plane->m_shader.Link();
	plane->m_shader.Use();

	// Model attributes
	plane->m_material.ambient = glm::vec3(1.0, 1.0f, 1.0f); // Background light set on the object
	plane->m_material.diffuse = glm::vec3(1.0f, 1.0f, 1.0f); // Color created when light shines on the boject
	plane->m_material.specular = glm::vec3(1.0f, 1.0f, 1.0f); // Color of the shine on the object when light is pointed at it
	plane->m_material.shininess = 40.0f; // Controls how much specularity the object has. Lower number = more shine

	// Load base texture
	plane->m_material.AddTexture(m_engine->Get<FileSystem>()->GetPathname() + "Textures\\cobble-diffuse.tga", GL_TEXTURE0);

	plane->m_shader.SetUniform("material.ambient", plane->m_material.ambient);
	plane->m_shader.SetUniform("material.diffuse", plane->m_material.diffuse);
	plane->m_shader.SetUniform("material.specular", plane->m_material.specular);
	plane->m_shader.SetUniform("material.shininess", plane->m_material.shininess);
		 
	plane->m_shader.SetUniform("light.diffuse", light->diffuse);
	plane->m_shader.SetUniform("light.specular", light->specular);
	plane->transform.translation = glm::vec3(1.0f, -10.0f, 1.0f);

	model->m_shader.SetUniform("light.cutoff", spot_light->cutoff);
	model->m_shader.SetUniform("light.exponent", spot_light->exponent);

	plane->m_shader.SetUniform("light.cutoff", spot_light->cutoff);
	plane->m_shader.SetUniform("light.exponent", spot_light->exponent);

	return true;
}


void Scene08::Update()
{
	float dt = m_engine->Get<Timer>()->DeltaTime();

	// Update light
	Light* light = this->GetObject<SpotLight>();
	light->transform.rotation = light->transform.rotation * glm::quat(glm::vec3(0.0f, glm::radians(-45.0) * dt, 0.0f));
	light->transform.translation = light->transform.rotation * glm::vec3(3.0f);

	// Update objects
	std::vector<Object*>objects = this->GetObjects<Object>();
	for (Object* object : objects)
	{
		object->Update();
	}

	// update lights    
	Camera* camera = this->GetObject<Camera>();
	SpotLight* spotlight = this->GetObject<SpotLight>("spot_light");
	light = this->GetObject<SpotLight>("spot_light");
	glm::vec3 lightPosition = ((SpotLight*)light)->GetDirectionFromView(camera->transform.matrix);


	// Update model lights
	std::vector<Model*> models = this->GetObjects<Model>();
	for (Model* model : models)
	{
		model->m_shader.Use();
		model->m_shader.SetUniform("light.position", spotlight->transform.translation);
		model->m_shader.SetUniform("light.direction", spotlight->GetDirectionFromView(camera->transform.matrix));
		model->m_shader.SetUniform("light.diffuse", spotlight->diffuse);
		model->m_shader.SetUniform("light.specular", spotlight->specular);
		model->m_shader.SetUniform("light.cutoff", spotlight->cutoff);
		model->m_shader.SetUniform("light.exponent", spotlight->exponent);

	}



}

void Scene08::Render()
{
	m_engine->Get<Renderer>()->ClearBuffer();

	std::vector<Renderable*> renderables = this->GetObjects<Renderable>();
	for (Renderable* renderable : renderables)
	{
		renderable->Draw();
	}

	m_engine->Get<Renderer>()->SwapBuffer();
}

void Scene08::Shutdown()
{

}
