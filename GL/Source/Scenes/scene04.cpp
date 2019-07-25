#include "scene04.h"
#include "Objects/light.h"
#include "Objects/camera.h"
#include "Objects/sphere.h"

#include "Objects/point_light.h"
#include <iostream>

#define MULTI_TEXTURE
//#define SPECULAR_MAP

bool Scene04::Initialize()
{
	Camera* camera = this->CreateObject<Camera>();
	camera->Initialize();
	camera->SetView(glm::vec3(0.0f, 0.0f, 40.0f), glm::vec3(0.0f));

	PointLight* light = this->CreateObject<PointLight>();
	light->transform.translation = glm::vec3(10.0f, 10.0f, 10.0f);

	//m_mxProjection = glm::perspective(glm::radians(45.0f), ((float)m_engine->Get<Renderer>()->GetWidth() / (float)m_engine->Get<Renderer>()->GetHeight()), 0.01f, 1000.0f);

	Sphere * model = this->CreateObject<Sphere>();
	model->Initialize(1.0f, 40, 40);
	model->transform.scale = (glm::vec3(5.0f));
	std::cout << glGetString(GL_VERSION) << " Running using Intel GPU" << std::endl;

#ifdef SPECULAR_MAP
	model->m_shader.CompileShader(m_engine->Get<FileSystem>()->GetPathname() + "Shaders\\Fragment Shaders\\phong_specular_map.fs", GL_FRAGMENT_SHADER);
#elif defined MULTI_TEXTURE
	model->m_shader.CompileShader(m_engine->Get<FileSystem>()->GetPathname() + "Shaders\\Fragment Shaders\\phong_multi_texture.fs", GL_FRAGMENT_SHADER);
#endif


	// Shader load
	model->m_shader.CompileShader(m_engine->Get<FileSystem>()->GetPathname() + "Shaders\\Vertex Shaders\\phong_uv.vs", GL_VERTEX_SHADER);
	//model->m_shader.CompileShader(m_engine->Get<FileSystem>()->GetPathname() + "Shaders\\Vertex Shaders\\phong.fs", GL_FRAGMENT_SHADER);
	model->m_shader.Link();
	model->m_shader.Use();

	// Model attributes
	model->m_material.ambient = glm::vec3(1.0, 1.0f, 1.0f); // Background light set on the object
	model->m_material.diffuse = glm::vec3(0.0f, 0.0f, 1.0f); // Color created when light shines on the boject
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

	//model->m_shader.SetUniform("uv_scale", glm::vec2(1.0f, 1.0f));

	// Light attributes
	m_light = this->CreateObject<PointLight>("pointlight");
	m_light->diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	m_light->transform.translation = glm::vec3(10.0f, 10.0f, 10.0f);
	m_light->specular = glm::vec3(1.0f, 1.0f, 1.0f);

	model->m_shader.SetUniform("light.diffuse", m_light->diffuse);
	model->m_shader.SetUniform("light.specular", m_light->specular);

	model->transform.translation = glm::vec3(1.0f, 1.0f, 20.0f);

	return true;
}

void Scene04::Shutdown()
{
	
}

void Scene04::Update()
{
	float dt = m_engine->Get<Timer>()->DeltaTime();

	Model* model = this->GetObject<Sphere>();
	
	Camera * camera = this->GetObject<Camera>();
	camera->Update();

	model->transform.rotation = model->transform.rotation * glm::angleAxis(glm::radians(45.0f * dt), glm::vec3(0.0f, 1.0f, 0.0f));

	// UV offest/modification(Must use Phong.vs)
	//m_uvOffset.y = m_uvOffset.y + (0.1f * dt);
	//model->m_shader.SetUniform("uv_offset", m_uvOffset);

	// Update light
	PointLight* light = this->GetObject<PointLight>();
	light->Update();
	glm::vec4 lightPosition = light->GetPositionFromView(camera->transform.matrix);

	// Update normals
	glm::mat3 mxNormal = glm::mat3(camera->transform.matrix * model->transform.matrix);
	mxNormal = glm::inverse(mxNormal);
	mxNormal = glm::transpose(mxNormal);
	model->m_shader.SetUniform("normal_matrix", mxNormal);

	// Update model view
	glm::mat4 mxMV = camera->transform.matrix * model->transform.matrix;
	model->m_shader.SetUniform("model_view_matrix", mxMV);

	// Update MVP
	model->transform.Update();
	glm::mat4 mxMVP = camera->projection * camera->transform.matrix * model->transform.matrix;
	model->m_shader.SetUniform("model_view_projection_matrix", mxMVP);
}

void Scene04::Render()
{
	m_engine->Get<Renderer>()->ClearBuffer();
	
	Sphere* model = this->GetObject<Sphere>();
	model->Draw();

	m_engine->Get<Renderer>()->SwapBuffer();
}
