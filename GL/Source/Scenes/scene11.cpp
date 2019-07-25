#include "scene11.h"
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

#define GLM_ENABLE_EXPERIMENTAL

bool Scene11::Initialize()
{
	std::cout << "Current graphics driver: " << glGetString(GL_VERSION) << std::endl;

	Camera* camera = this->CreateObject<Camera>("camera");
	camera->Initialize();
	camera->SetView(glm::vec3(10.0f, 0.0f, 10.0f), glm::vec3(0.0f));
	camera->m_type = Camera::eType::EDITOR;

	Camera* camera_rtt = this->CreateObject<Camera>("camera_rtt");
	camera_rtt->Initialize();
	camera_rtt->SetView(glm::vec3(0.0f, 0.0f, 2.9f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	camera_rtt->SetProjection(45.0f, 1.0f, 0.01f, 1000.0f);
	camera_rtt->m_type = Camera::eType::LOOK_AT;

	PointLight* light = this->CreateObject<PointLight>("point_light");
	light->transform.translation = glm::vec3(5.0f, 42.0f, 72.0f);
	glm::vec3 color = glm::rgbColor(glm::vec3(glm::linearRand(0.0f, 360.0f), 1.0f, 1.0f));
	light->diffuse = color;
	light->specular = glm::vec3(1.0f, 1.0f, 1.0f);
	
	// Sphere
	Model * model = this->CreateObject<Sphere>("model");
	((Sphere*)(model))->Initialize(1.0f, 40, 40);
	model->transform.scale = (glm::vec3(1.0f));
	model->m_cameraName = "camera_rtt";

	// Shader load for model
	model->m_shader.CompileShader(m_engine->Get<FileSystem>()->GetPathname() + "Shaders\\Vertex Shaders\\phong_uv.vs", GL_VERTEX_SHADER);
	model->m_shader.CompileShader(m_engine->Get<FileSystem>()->GetPathname() + "Shaders\\Fragment Shaders\\phong.fs", GL_FRAGMENT_SHADER);
	model->m_shader.Link();
	model->m_shader.Use();

	// Model attributes
	model->m_material.ambient = glm::vec3(0.2f, 0.2f, 0.2f); // Background light set on the object
	model->m_material.diffuse = glm::vec3(1.0f, 1.0f, 1.0f); // Color created when light shines on the boject
	model->m_material.specular = glm::vec3(1.0f, 1.0f, 1.0f); // Color of the shine on the object when light is pointed at it
	model->m_material.shininess = 10.0f; // Controls how much specularity the object has. Lower number = more shine

	// Load base texture
	model->m_material.AddTexture(m_engine->Get<FileSystem>()->GetPathname() + "Textures\\metal-diffuse.png", GL_TEXTURE0);

	model->m_shader.SetUniform("material.ambient", model->m_material.ambient);
	model->m_shader.SetUniform("material.diffuse", model->m_material.diffuse);
	model->m_shader.SetUniform("material.specular", model->m_material.specular);
	model->m_shader.SetUniform("material.shininess", model->m_material.shininess);

	model->transform.translation = glm::vec3(0.0f, 0.0f, 0.0f);
	
	// model2(Camera model)=======================================
	Model * model2 = this->CreateObject<Model>("square");
	model2->Import(m_engine->Get<FileSystem>()->GetPathname() + "Meshes\\cube.obj");
	model2->transform.scale = (glm::vec3(2.0f));

	// Shader load for model
	model2->m_shader.CompileShader(m_engine->Get<FileSystem>()->GetPathname() + "Shaders\\Vertex Shaders\\phong_uv.vs", GL_VERTEX_SHADER);
	model2->m_shader.CompileShader(m_engine->Get<FileSystem>()->GetPathname() + "Shaders\\Fragment Shaders\\phong.fs", GL_FRAGMENT_SHADER);
	model2->m_shader.Link();
	model2->m_shader.Use();

	// Model attributes
	model2->m_material.ambient = glm::vec3(0.2f, 0.2f, 0.2f); // Background light set on the object
	model2->m_material.diffuse = glm::vec3(1.0f, 1.0f, 1.0f); // Color created when light shines on the boject
	model2->m_material.specular = glm::vec3(1.0f, 1.0f, 1.0f); // Color of the shine on the object when light is pointed at it
	model2->m_material.shininess = 10.0f; // Controls how much specularity the object has. Lower number = more shine

	// Load base texture
	glGenFramebuffers(1, &m_framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);

	GLuint textureID = Material::CreateTexture(512, 512);
	model2->m_material.AddTexture(textureID, GL_TEXTURE0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureID, 0);


	GLuint depthBufferID;
	glGenRenderbuffers(1, &depthBufferID);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBufferID);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 512, 512);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBufferID);

	GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, drawBuffers);

	GLenum result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	assert(result == GL_FRAMEBUFFER_COMPLETE);


	model2->m_shader.SetUniform("material.ambient", model2->m_material.ambient);
	model2->m_shader.SetUniform("material.diffuse", model2->m_material.diffuse);
	model2->m_shader.SetUniform("material.specular", model2->m_material.specular);
	model2->m_shader.SetUniform("material.shininess", model2->m_material.shininess);

	model2->transform.translation = glm::vec3(7.0f, 0.0f, 7.0f);

	return true;
}


void Scene11::Update()
{
	float dt = m_engine->Get<Timer>()->DeltaTime();

	// Update objects
	std::vector<Object*>objects = this->GetObjects<Object>();
	for (Object* object : objects)
	{
		object->Update();
	}
	
	PointLight* light = this->GetObject<PointLight>("point_light");

	Camera* camera_rtt = GetObject<Camera>("camera_rtt");
	glm::vec4 lightPosition = light->GetPositionFromView(camera_rtt->transform.matrix);
	Model* model2 = this->GetObject<Model>("square");
	model2->m_shader.Use();
	model2->m_shader.SetUniform("light.position", lightPosition);

	Camera* camera = GetObject<Camera>("camera");
	lightPosition = light->GetPositionFromView(camera->transform.matrix);

	Model* model = this->GetObject<Model>("model");
	model->m_shader.Use();
	model->m_shader.SetUniform("light.position", lightPosition);
	model->m_shader.SetUniform("light.diffuse", light->diffuse);
	model->m_shader.SetUniform("light.specular", light->specular);
	

	Model* square = this->GetObject<Model>("square");
	square->transform.rotation = square->transform.rotation * glm::quat(glm::vec3(glm::radians(25.0f * dt), glm::radians(25.0f * dt), 0.0f));
	square->m_shader.Use();
	square->m_shader.SetUniform("light.position", lightPosition);
	square->m_shader.SetUniform("light.diffuse", light->diffuse); 
	square->m_shader.SetUniform("light.specular", light->specular);

	// ImGui
	m_engine->Get<UI>()->Start();
	ImGui::Begin("square transform");
	Transform::Edit(&square->transform);
	ImGui::End();

	ImGui::Begin("camera_rtt transform");
	Transform::Edit(&camera_rtt->transform);
	ImGui::End();

	ImGui::Begin("point_light transform");
	Transform::Edit(&light->transform);
	ImGui::End();

	ImGui::Begin("sphere transform");
	Transform::Edit(&model->transform);
	ImGui::End();
}

void Scene11::Render()
{
	m_engine->Get<Renderer>()->ClearBuffer();
	
	glViewport(0, 0, 512, 512);
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f); // can use any color

	glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	auto renderable = GetObject<Renderable>("model");
	renderable->Draw();

	glFlush();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glViewport(0, 0, m_engine->Get<Renderer>()->GetWidth(), m_engine->Get<Renderer>()->GetHeight());
	glClearColor(0.8f, 0.8f, 0.8f, 0.8f);
	
	renderable = GetObject<Renderable>("square");
	renderable->Draw();

	m_engine->Get<UI>()->Draw();

	//std::vector<Renderable*> renderables = this->GetObjects<Renderable>();
	//for (Renderable* renderable : renderables)
	//{
	//	renderable->Draw();
	//}

	m_engine->Get<Renderer>()->SwapBuffer();
}

void Scene11::Shutdown()
{

}
