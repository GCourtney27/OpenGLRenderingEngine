#include "scene05.h"
#include "Objects/light.h"
#include "Objects/camera.h"
#include "Objects/sphere.h"
#include "Objects/cube.h"
#include "Objects/point_light.h"
#include <iostream>

//#define MULTI_TEXTURE
//#define SPECULAR_MAP

bool Scene05::Initialize()
{
	std::cout << "Current graphics driver: " << glGetString(GL_VERSION) << std::endl;

	Camera* camera = this->CreateObject<Camera>();
	camera->Initialize();
	camera->SetView(glm::vec3(0.0f, 0.0f, 20.0f), glm::vec3(0.0f));

	PointLight* light = this->CreateObject<PointLight>();
	light->transform.translation = glm::vec3(10.0f, 10.0f, 10.0f);
	light->diffuse = glm::vec3(0.0f, 1.0f, 0.0f);
	light->specular = glm::vec3(1.0f, 1.0f, 1.0f);

	// Light attributes
	//m_light = this->CreateObject<PointLight>("pointlight");
	//m_light->diffuse = glm::vec3(0.0f, 1.0f, 0.0f);
	//m_light->transform.translation = glm::vec3(10.0f, 10.0f, 10.0f);
	//m_light->specular = glm::vec3(1.0f, 1.0f, 1.0f);

	Model * model = this->CreateObject<Sphere>();
	((Sphere*)(model))->Initialize(1.0f, 40, 40);
	model->transform.scale = (glm::vec3(5.0f));

	Model * model2 = this->CreateObject<Cube>();
	((Cube*)(model2))->Initialize(1.0f);
	model2->transform.scale = (glm::vec3(1.0f));

	// Model load
#ifdef SPECULAR_MAP
	model->m_shader.CompileShader(m_engine->Get<FileSystem>()->GetPathname() + "Shaders\\Fragment Shaders\\phong_specular_map.fs", GL_FRAGMENT_SHADER);
#elif defined MULTI_TEXTURE
	model->m_shader.CompileShader(m_engine->Get<FileSystem>()->GetPathname() + "Shaders\\Fragment Shaders\\phong_multi_texture.fs", GL_FRAGMENT_SHADER);
#endif


	// Shader load
	model->m_shader.CompileShader(m_engine->Get<FileSystem>()->GetPathname() + "Shaders\\Vertex Shaders\\phong_uv.vs", GL_VERTEX_SHADER);
	model->m_shader.CompileShader(m_engine->Get<FileSystem>()->GetPathname() + "Shaders\\Fragment Shaders\\phong_fog.fs", GL_FRAGMENT_SHADER);
	model->m_shader.Link();
	model->m_shader.Use();

	// Model attributes
	model->m_material.ambient = glm::vec3(1.0, 1.0f, 1.0f); // Background light set on the object
	model->m_material.diffuse = glm::vec3(1.0f, 1.0f, 1.0f); // Color created when light shines on the boject
	model->m_material.specular = glm::vec3(1.0f, 1.0f, 1.0f); // Color of the shine on the object when light is pointed at it
	model->m_material.shininess = 10.0f; // Controls how much specularity the object has. Lower number = more shine
	
	// Load base texture
	//model->m_material.AddTexture(m_engine->Get<FileSystem>()->GetPathname() + "textures\\flower.png", GL_TEXTURE0);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	model->m_material.AddTexture(m_engine->Get<FileSystem>()->GetPathname() + "Textures\\metal-diffuse.png", GL_TEXTURE0);
	
	// Choose render method
#ifdef SPECULAR_MAP
	model->m_material.AddTexture(m_engine->Get<FileSystem>()->GetPathname() + "Textures\\metal-specular.png", GL_TEXTURE1);
#elif defined MULTI_TEXTURE
	model->m_material.AddTexture(m_engine->Get<FileSystem>()->GetPathname() + "Textures\\moss.png", GL_TEXTURE1);
#endif
	model->m_shader.SetUniform("material.ambient", model->m_material.ambient);
	model->m_shader.SetUniform("material.diffuse", model->m_material.diffuse);
	model->m_shader.SetUniform("material.specular", model->m_material.specular);
	model->m_shader.SetUniform("material.shininess", model->m_material.shininess);

	model->m_shader.SetUniform("fog.distance_min", 10.0f);
	model->m_shader.SetUniform("fog.distance_max", 50.0f);
	model->m_shader.SetUniform("fog.color", glm::vec3(0.85f));

	model->m_shader.SetUniform("light.diffuse", light->diffuse);
	model->m_shader.SetUniform("light.specular", light->specular);
	model->transform.translation = glm::vec3(1.0f, 1.0f, 20.0f);

	//model->m_shader.SetUniform("uv_scale", glm::vec2(1.0f, 1.0f));

	// Model2 Load
#ifdef SPECULAR_MAP
	model->m_shader.CompileShader(m_engine->Get<FileSystem>()->GetPathname() + "Shaders\\Fragment Shaders\\phong_specular_map.fs", GL_FRAGMENT_SHADER);
#elif defined MULTI_TEXTURE
	model->m_shader.CompileShader(m_engine->Get<FileSystem>()->GetPathname() + "Shaders\\Fragment Shaders\\phong_multi_texture.fs", GL_FRAGMENT_SHADER);
#endif


	// Shader load
	model2->m_shader.CompileShader(m_engine->Get<FileSystem>()->GetPathname() + "Shaders\\Vertex Shaders\\phong_uv.vs", GL_VERTEX_SHADER);
	model2->m_shader.CompileShader(m_engine->Get<FileSystem>()->GetPathname() + "Shaders\\Fragment Shaders\\phong_fog.fs", GL_FRAGMENT_SHADER);
	model2->m_shader.Link();
	model2->m_shader.Use();

	// Model attributes
	model2->m_material.ambient = glm::vec3(1.0, 1.0f, 1.0f); // Background light set on the object
	model2->m_material.diffuse = glm::vec3(1.0f, 1.0f, 1.0f); // Color created when light shines on the boject
	model2->m_material.specular = glm::vec3(1.0f, 1.0f, 1.0f); // Color of the shine on the object when light is pointed at it
	model2->m_material.shininess = 10.0f; // Controls how much specularity the object has. Lower number = more shine

	// Load base texture
	//model->m_material.AddTexture(m_engine->Get<FileSystem>()->GetPathname() + "textures\\flower.png", GL_TEXTURE0);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	model2->m_material.AddTexture(m_engine->Get<FileSystem>()->GetPathname() + "Textures\\metal-diffuse.png", GL_TEXTURE0);

	// Choose render method
#ifdef SPECULAR_MAP
	model->m_material.AddTexture(m_engine->Get<FileSystem>()->GetPathname() + "Textures\\metal-specular.png", GL_TEXTURE1);
#elif defined MULTI_TEXTURE
	model->m_material.AddTexture(m_engine->Get<FileSystem>()->GetPathname() + "Textures\\moss.png", GL_TEXTURE1);
#endif
	model2->m_shader.SetUniform("material.ambient", model2->m_material.ambient);
	model2->m_shader.SetUniform("material.diffuse", model2->m_material.diffuse);
	model2->m_shader.SetUniform("material.specular", model2->m_material.specular);
	model2->m_shader.SetUniform("material.shininess", model2->m_material.shininess);
		 
	model2->m_shader.SetUniform("fog.distance_min", 10.0f);
	model2->m_shader.SetUniform("fog.distance_max", 50.0f);
	model2->m_shader.SetUniform("fog.color", glm::vec3(0.85f));
		 
	model2->m_shader.SetUniform("light.diffuse", light->diffuse);
	model2->m_shader.SetUniform("light.specular", light->specular);
	model2->transform.translation = light->transform.translation;



	return true;
}

void Scene05::Shutdown()
{
	
}

void Scene05::Update()
{
	float dt = m_engine->Get<Timer>()->DeltaTime();

	std::vector<Object*>objects = this->GetObjects<Object>();
	for (Object* object : objects)
	{
		object->Update();
	}

	Camera * camera = this->GetObject<Camera>();

	// Update light
	PointLight* light = this->GetObject<PointLight>();
	glm::vec4 lightPosition = light->GetPositionFromView(camera->transform.matrix);

	// Update model lights
	std::vector<Model*> models = this->GetObjects<Model>();
	for (Model* model : models)
	{
		model->m_shader.Use();
		// Update light
		model->m_shader.SetUniform("light.position", lightPosition);
		
	}

}

void Scene05::Render()
{
	m_engine->Get<Renderer>()->ClearBuffer();

	std::vector<Renderable*> renderables = this->GetObjects<Renderable>();
	for (Renderable* renderable : renderables)
	{
		renderable->Draw();
	}

	m_engine->Get<Renderer>()->SwapBuffer();
}
