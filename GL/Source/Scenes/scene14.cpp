#include "scene14.h"
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

#define SHADOW_BUFFER_WIDTH	1024
#define SHADOW_BUFFER_HEIGHT 1024


bool Scene14::Initialize()
{
	Camera* camera = this->CreateObject<Camera>();
	camera->Initialize();
	camera->SetView(glm::vec3(2.0f, 2.0f, 3.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	PointLight* light = this->CreateObject<PointLight>("point_light");
	light->diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	light->specular = glm::vec3(1.0f, 1.0f, 1.0f);
	light->transform.translation = glm::vec3(0.0f, 4.5f, 3.0f);

	m_depthShader = new Shader();
	m_depthShader->CompileShader(m_engine->Get<FileSystem>()->GetPathname() + "Shaders\\Vertex Shaders\\shadow_depth.vs", GL_VERTEX_SHADER);
	m_depthShader->CompileShader(m_engine->Get<FileSystem>()->GetPathname() + "Shaders\\Fragment Shaders\\shadow_depth.fs", GL_FRAGMENT_SHADER);
	m_depthShader->Link();

	GLuint depthTexture = Material::CreateDepthTexture(SHADOW_BUFFER_WIDTH, SHADOW_BUFFER_HEIGHT);


	// Sphere
	Model* model = this->CreateObject<Model>("sphere");
	model->Import(m_engine->Get<FileSystem>()->GetPathname() + "Meshes\\sphere.obj");
	model->transform.translation = glm::vec3(0.0f, 0.15f, 0.0f);

	model->m_shader.CompileShader(m_engine->Get<FileSystem>()->GetPathname() + "Shaders\\Vertex Shaders\\phong_shadow.vs", GL_VERTEX_SHADER);
	model->m_shader.CompileShader(m_engine->Get<FileSystem>()->GetPathname() + "Shaders\\Fragment Shaders\\phong_shadow.fs", GL_FRAGMENT_SHADER);

	model->m_shader.Link();
	model->m_shader.Use();

	model->m_material.ambient = glm::vec3(0.0f);
	model->m_material.diffuse = glm::vec3(1.0f);
	model->m_material.specular = glm::vec3(1.0f);
	model->m_material.shininess = 100.0f;//Lower number is more shine

	model->m_material.AddTexture(m_engine->Get<FileSystem>()->GetPathname() + "Textures\\metal-diffuse.png", GL_TEXTURE0);
	model->m_material.AddTexture(depthTexture, GL_TEXTURE1);
	
	model->m_material.SetTextures();

	model->m_shader.SetUniform("material.ambient", model->m_material.ambient);
	model->m_shader.SetUniform("material.diffuse", model->m_material.diffuse);
	model->m_shader.SetUniform("material.specular", model->m_material.specular);
	model->m_shader.SetUniform("material.shininess", model->m_material.shininess);

	// Plane
	Model* model2 = this->CreateObject<Model>("plane");
	model2->Import(m_engine->Get<FileSystem>()->GetPathname() + "Meshes\\plane.obj");
	model2->transform.translation = glm::vec3(0.0f, -4.2f, 0.0f);
	model2->transform.scale = glm::vec3(40.0f);

	model2->m_shader.CompileShader(m_engine->Get<FileSystem>()->GetPathname() + "Shaders\\Vertex Shaders\\phong_shadow.vs", GL_VERTEX_SHADER);
	model2->m_shader.CompileShader(m_engine->Get<FileSystem>()->GetPathname() + "Shaders\\Fragment Shaders\\phong_shadow.fs", GL_FRAGMENT_SHADER);

	model2->m_shader.Link();
	model2->m_shader.Use();

	model2->m_material.ambient = glm::vec3(0.0f);
	model2->m_material.diffuse = glm::vec3(1.0f);
	model2->m_material.specular = glm::vec3(1.0f);
	model2->m_material.shininess = 100.0f;//Lower number is more shine

	model2->m_material.AddTexture(m_engine->Get<FileSystem>()->GetPathname() + "Textures\\cobblestone.jpg", GL_TEXTURE0);
	model2->m_material.AddTexture(depthTexture, GL_TEXTURE1);
	model2->m_material.SetTextures();

	model2->m_shader.SetUniform("material.ambient", model->m_material.ambient);
	model2->m_shader.SetUniform("material.diffuse", model->m_material.diffuse);
	model2->m_shader.SetUniform("material.specular", model->m_material.specular);
	model2->m_shader.SetUniform("material.shininess", model->m_material.shininess);

	// Debug square
	Model* model3 = this->CreateObject<Model>("debug_cube");
	model3->Import(m_engine->Get<FileSystem>()->GetPathname() + "Meshes\\quad.obj");
	model3->transform.translation = glm::vec3(0.0f, 0.25f, 0.0f);

	model3->m_shader.CompileShader(m_engine->Get<FileSystem>()->GetPathname() + "Shaders\\Vertex Shaders\\shadow_depth_debug.vs", GL_VERTEX_SHADER);
	model3->m_shader.CompileShader(m_engine->Get<FileSystem>()->GetPathname() + "Shaders\\Fragment Shaders\\shadow_depth_debug.fs", GL_FRAGMENT_SHADER);

	model3->m_shader.Link();
	model3->m_shader.Use();

	model3->m_material.ambient = glm::vec3(0.0f);
	model3->m_material.diffuse = glm::vec3(1.0f);
	model3->m_material.specular = glm::vec3(1.0f);
	model3->m_material.shininess = 100.0f;//Lower number is more shine

	model3->m_material.AddTexture(depthTexture, GL_TEXTURE0);

	model3->m_material.SetTextures();

	model3->m_shader.SetUniform("material.ambient", model->m_material.ambient);
	model3->m_shader.SetUniform("material.diffuse", model->m_material.diffuse);
	model3->m_shader.SetUniform("material.specular", model->m_material.specular);
	model3->m_shader.SetUniform("material.shininess", model->m_material.shininess);

	m_depthBuffer = Material::CreateDepthbuffer(depthTexture, SHADOW_BUFFER_WIDTH, SHADOW_BUFFER_HEIGHT);

	return true;
}

void Scene14::Shutdown()
{

}


void Scene14::Update()
{
	float dt = m_engine->Get<Timer>()->DeltaTime();

	PointLight* light = this->GetObject<PointLight>("point_light");
	light->transform.translation = light->transform.translation * glm::quat(glm::vec3(0.0f, glm::radians(55.0f * dt), 0.0f));


	glm::mat4 mxProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, -10.0f, 10.0f);
	glm::mat4 mxLightView = glm::lookAt(light->transform.translation, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 mxLVP = mxProjection * mxLightView;

	m_depthShader->Use();
	m_depthShader->SetUniform("light_view_projection_matrix", mxLVP);

	glm::mat4 mxBias(
		0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f
	);


	/*Model* model = this->GetObject<Model>("sphere");
	m_depthShader->SetUniform("model_matrix", model->transform.matrix);

	Model* model2 = this->GetObject<Model>("plane");*/

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

		model->m_shader.SetUniform("light.diffuse", light->diffuse);
		model->m_shader.SetUniform("light.specular", light->specular);

		model->m_shader.SetUniform("material.ambient", model->m_material.ambient);
		model->m_shader.SetUniform("material.diffuse", model->m_material.diffuse);
		model->m_shader.SetUniform("material.specular", model->m_material.specular);
		model->m_shader.SetUniform("material.shininess", model->m_material.shininess);

		glm::mat4 mxMLVP = mxBias * mxLVP * model->transform.matrix;
		model->m_shader.SetUniform("light_view_projection_matrix", mxMLVP);

	}


}

void Scene14::Render()
{
	glViewport(0, 0, SHADOW_BUFFER_WIDTH, SHADOW_BUFFER_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, m_depthBuffer);
	glClear(GL_DEPTH_BUFFER_BIT);
	glCullFace(GL_FRONT);
	m_depthShader->Use();

	auto model = GetObject<Model>("sphere");
	m_depthShader->SetUniform("model_matrix", model->transform.matrix);
	model->m_vertexArrays.Draw();

	model = GetObject<Model>("plane");
	m_depthShader->SetUniform("model_matrix", model->transform.matrix);
	model->m_vertexArrays.Draw();

	model = GetObject<Model>("debug_cube"); 
	m_depthShader->SetUniform("model_matrix", model->transform.matrix);
	model->m_vertexArrays.Draw();

	glFlush();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glViewport(0, 0, m_engine->Get<Renderer>()->GetWidth(), m_engine->Get<Renderer>()->GetHeight());
	m_engine->Get<Renderer>()->ClearBuffer();
	glCullFace(GL_BACK);
	std::vector<Renderable*> renderables = this->GetObjects<Renderable>();

	for (Renderable* renderable : renderables)
	{
		renderable->Draw();
	}
	m_engine->Get<Renderer>()->SwapBuffer();
}

