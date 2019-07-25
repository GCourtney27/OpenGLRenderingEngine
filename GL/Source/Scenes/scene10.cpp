#include "scene10.h"
#include "Objects/light.h"
#include "Objects/camera.h"
#include "Objects/sphere.h"
#include "Objects/cube.h"
#include "Objects/point_light.h"
#include "Objects/plane.h"
#include "Objects/directional_light.h"
#include "Objects/spot_light.h"
#include <iostream>

bool Scene10::Initialize()
{
	std::cout << "Current graphics driver: " << glGetString(GL_VERSION) << std::endl;

	Camera* camera = this->CreateObject<Camera>();
	camera->Initialize();
	camera->SetView(glm::vec3(0.0f, 3.0f, 10.0f), glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

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
	spot_light->transform.translation = glm::vec3(5.0f, 0.0f, 0.0f);

	spot_light->SetDirection(spot_light->transform.translation, glm::vec3(0.0f, 0.0f, 0.0f));


	Model * skybox = this->CreateObject<Model>("skybox");
	skybox->Import(m_engine->Get<FileSystem>()->GetPathname() + "Meshes\\cube.obj");
	skybox->transform.translation = glm::vec3(0.0f, 0.0f, 0.0f);
	skybox->transform.scale = (glm::vec3(40.0f));
	   

	// Shader load for model
	skybox->m_shader.CompileShader(m_engine->Get<FileSystem>()->GetPathname() + "Shaders\\Vertex Shaders\\cubemap.vs", GL_VERTEX_SHADER);
	skybox->m_shader.CompileShader(m_engine->Get<FileSystem>()->GetPathname() + "Shaders\\Fragment Shaders\\cubemap.fs", GL_FRAGMENT_SHADER);
	skybox->m_shader.Link();
	skybox->m_shader.Use();

	std::vector<std::string> suffixes = { "_posx", "_negx", "_posy", "_negy", "_posz", "_negz" };
	skybox->m_material.AddTextureCube(m_engine->Get<FileSystem>()->GetPathname() + "Textures\\lancellotti", suffixes, "jpg", GL_TEXTURE);
	skybox->m_shader.AddSubroutine(GL_VERTEX_SHADER, "skybox");

	//=====================================

	Model * plane = this->CreateObject<Model>("plane");
	plane->Import(m_engine->Get<FileSystem>()->GetPathname() + "Meshes\\plane.obj");
	plane->transform.translation = glm::vec3(0.0f, 0.0f, 0.0f);
	plane->transform.scale = (glm::vec3(20.0f));

	// Shader load for plane
	plane->m_shader.CompileShader(m_engine->Get<FileSystem>()->GetPathname() + "Shaders\\Vertex Shaders\\phong_uv.vs", GL_VERTEX_SHADER);
	plane->m_shader.CompileShader(m_engine->Get<FileSystem>()->GetPathname() + "Shaders\\Fragment Shaders\\phong.fs", GL_FRAGMENT_SHADER);
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
	
	plane->m_shader.SetUniform("light.cutoff", spot_light->cutoff);
	plane->m_shader.SetUniform("light.exponent", spot_light->exponent);

	//=======================

	Model * sphere = this->CreateObject<Model>("sphere");
	sphere->Import(m_engine->Get<FileSystem>()->GetPathname() + "Meshes\\sphere.obj");
	sphere->transform.translation = glm::vec3(0.0f, 3.0f, 0.0f);
	sphere->transform.scale = (glm::vec3(1.0f));
	   
	// Shader load for sphere
	sphere->m_shader.CompileShader(m_engine->Get<FileSystem>()->GetPathname() + "Shaders\\Vertex Shaders\\cubemap.vs", GL_VERTEX_SHADER);
	sphere->m_shader.CompileShader(m_engine->Get<FileSystem>()->GetPathname() + "Shaders\\Fragment Shaders\\cubemap.fs", GL_FRAGMENT_SHADER);
	sphere->m_shader.Link();
	sphere->m_shader.Use();

	// Load base texture
	sphere->m_material.AddTexture(m_engine->Get<FileSystem>()->GetPathname() + "Textures\\cobble-diffuse.tga", GL_TEXTURE0);
	sphere->m_material.AddTextureCube(m_engine->Get<FileSystem>()->GetPathname() + "Textures\\lancellotti", suffixes, "jpg", GL_TEXTURE);
	sphere->m_shader.AddSubroutine(GL_VERTEX_SHADER, "reflection");
	
	

	return true;
}


void Scene10::Update()
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

		model->m_shader.SetUniform("model_matrix", model->transform.matrix);
		model->m_shader.SetUniform("camera_position", camera->transform.translation);
	}



}

void Scene10::Render()
{
	glDisable(GL_CULL_FACE);
	m_engine->Get<Renderer>()->ClearBuffer();

	std::vector<Renderable*> renderables = this->GetObjects<Renderable>();
	for (Renderable* renderable : renderables)
	{
		renderable->Draw();
	}

	m_engine->Get<Renderer>()->SwapBuffer();
}

void Scene10::Shutdown()
{

}
