#include "scene07.h"
#include "Objects/light.h"
#include "Objects/camera.h"
#include "Objects/sphere.h"
#include "Objects/cube.h"
#include "Objects/point_light.h"
#include "Objects/plane.h"
#include "Objects/directional_light.h"
#include <iostream>

//#define MULTI_TEXTURE
//#define SPECULAR_MAP

bool Scene07::Initialize()
{
	std::cout << "Current graphics driver: " << glGetString(GL_VERSION) << std::endl;

	m_engine->Get<Input>()->AddAction("point", SDL_SCANCODE_1, Input::eDevice::KEYBOARD);
	m_engine->Get<Input>()->AddAction("directional", SDL_SCANCODE_2, Input::eDevice::KEYBOARD);

	Camera* camera = this->CreateObject<Camera>();
	camera->Initialize();
	camera->SetView(glm::vec3(0.0f, 0.0f, 20.0f), glm::vec3(0.0f), glm::vec3(0.0f));

	// Light attributes
	PointLight* point_light = this->CreateObject<PointLight>("point_light");
	point_light->diffuse = glm::vec3(1.0f, 0.0f, 0.0f);
	point_light->specular = glm::vec3(1.0f, 1.0f, 1.0f);
	point_light->transform.translation = glm::vec3(5.0f, 0.0f, 0.0f);

	DirectionalLight* directional_light = this->CreateObject<DirectionalLight>("directional_light");
	directional_light->diffuse = glm::vec3(0.0f, 0.0f, 1.0f);
	directional_light->specular = glm::vec3(1.0f, 1.0f, 1.0f);
	directional_light->SetDirection(glm::vec3(5.0f, 5.0f, 5.0f), glm::vec3(0.0f));



	Model * model = this->CreateObject<Sphere>("model");
	((Sphere*)(model))->Initialize(1.0f, 40, 40);
	model->transform.scale = (glm::vec3(1.0f));

	Model * model2 = this->CreateObject<Plane>("model2");
	((Plane*)(model2))->Initialize(10.0f, 10.0f, 10, 10, 10.0f, 10.0f);
	model2->transform.scale = (glm::vec3(5.0f));

	// Shader load
	model->m_shader.CompileShader(m_engine->Get<FileSystem>()->GetPathname() + "Shaders\\Vertex Shaders\\phong_uv.vs", GL_VERTEX_SHADER);
	model->m_shader.CompileShader(m_engine->Get<FileSystem>()->GetPathname() + "Shaders\\Fragment Shaders\\phong_fog.fs", GL_FRAGMENT_SHADER);
	model->m_shader.Link();
	model->m_shader.Use();

	// Model attributes
	model->m_material.ambient = glm::vec3(0.2f, 0.2f, 0.2f); // Background light set on the object
	model->m_material.diffuse = glm::vec3(1.0f, 1.0f, 1.0f); // Color created when light shines on the boject
	model->m_material.specular = glm::vec3(1.0f, 1.0f, 1.0f); // Color of the shine on the object when light is pointed at it
	model->m_material.shininess = 10.0f; // Controls how much specularity the object has. Lower number = more shine
	
	// Load base texture
	model->m_material.AddTexture(m_engine->Get<FileSystem>()->GetPathname() + "Textures\\metal-diffuse.png", GL_TEXTURE0);
	
	// Choose render method
	model->m_shader.SetUniform("material.ambient", model->m_material.ambient);
	model->m_shader.SetUniform("material.diffuse", model->m_material.diffuse);
	model->m_shader.SetUniform("material.specular", model->m_material.specular);
	model->m_shader.SetUniform("material.shininess", model->m_material.shininess);

	model->m_shader.SetUniform("fog.distance_min", 10.0f);
	model->m_shader.SetUniform("fog.distance_max", 50.0f);
	model->m_shader.SetUniform("fog.color", glm::vec3(0.85f));

	model->transform.translation = glm::vec3(1.0f, 1.0f, 1.0f);

	// Shader load
	model2->m_shader.CompileShader(m_engine->Get<FileSystem>()->GetPathname() + "Shaders\\Vertex Shaders\\phong_uv.vs", GL_VERTEX_SHADER);
	model2->m_shader.CompileShader(m_engine->Get<FileSystem>()->GetPathname() + "Shaders\\Fragment Shaders\\phong.fs", GL_FRAGMENT_SHADER);
	model2->m_shader.Link();
	model2->m_shader.Use();

	// Model attributes
	model2->m_material.ambient = glm::vec3(0.2f, 0.2f, 0.2f); // Background light set on the object
	model2->m_material.diffuse = glm::vec3(1.0f, 1.0f, 1.0f); // Color created when light shines on the boject
	model2->m_material.specular = glm::vec3(1.0f, 1.0f, 1.0f); // Color of the shine on the object when light is pointed at it
	model2->m_material.shininess = 40.0f; // Controls how much specularity the object has. Lower number = more shine

	// Load base texture
	model2->m_material.AddTexture(m_engine->Get<FileSystem>()->GetPathname() + "Textures\\cobble-diffuse.tga", GL_TEXTURE0);

	model2->m_shader.SetUniform("material.ambient", model2->m_material.ambient);
	model2->m_shader.SetUniform("material.diffuse", model2->m_material.diffuse);
	model2->m_shader.SetUniform("material.specular", model2->m_material.specular);
	model2->m_shader.SetUniform("material.shininess", model2->m_material.shininess);

		 
	model2->transform.translation = glm::vec3(1.0f, -10.0f, 1.0f);



	return true;
}

void Scene07::Shutdown()
{
	
}

void Scene07::Update()
{
	float dt = m_engine->Get<Timer>()->DeltaTime();

	if (m_engine->Get<Input>()->GetActionButton("point") == Input::eButtonState::HELD) m_lightIndex = 0;
	if (m_engine->Get<Input>()->GetActionButton("directional") == Input::eButtonState::HELD) m_lightIndex = 1;

	// Update models
	//Model* model = this->GetObject<Model>("model");
	//model->transform.rotation = model->transform.rotation * glm::quat(glm::vec3(0.0f, glm::radians(45.0) * dt, 0.0f));

	// Update light
	Light* light = this->GetObject<PointLight>();
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
	glm::vec4 lightPosition;
	if (m_lightIndex == 0)
	{
		light = this->GetObject<PointLight>("point_light");
		lightPosition = ((PointLight*)light)->GetPositionFromView(camera->transform.matrix);
	}
	else if (m_lightIndex == 1)
	{
		light = this->GetObject<DirectionalLight>("directional_light");
		lightPosition = ((DirectionalLight*)light)->GetDirectionFromView(camera->transform.matrix);
	}

	// Update model lights
	std::vector<Model*> models = this->GetObjects<Model>();
	for (Model* model : models)
	{
		model->m_shader.Use();
		// Update light
		model->m_shader.SetUniform("light.position", lightPosition);
		model->m_shader.SetUniform("light.diffuse", light->diffuse);
		model->m_shader.SetUniform("light.specular", light->specular);
	}


}

void Scene07::Render()
{
	m_engine->Get<Renderer>()->ClearBuffer();

	std::vector<Renderable*> renderables = this->GetObjects<Renderable>();
	for (Renderable* renderable : renderables)
	{
		renderable->Draw();
	}

	m_engine->Get<Renderer>()->SwapBuffer();
}
