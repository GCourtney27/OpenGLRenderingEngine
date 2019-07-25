#include "scene03.h"
#include "Objects/light.h"
#include "Objects/camera.h"

#include "Objects/point_light.h"
#include <iostream>

bool Scene03::Initialize()
{
	// Vertex buffer data
	static float cube_vertices[] = {
		// Front
		// x, y, z, nx, ny, nz
		-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,
		// Right
		 1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,
		 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,
		 1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f,
		 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,
		 // Back
		 -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,
		 -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,
		  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,
		  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,
		  // Left
		  -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f,
		  -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f,
		  -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f,
		  -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f,
		  // Bottom
		  -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,
		  -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,
		   1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,
		   1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,
		   // Top
		   -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,
			1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,
			1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		   -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f
	};

	static GLushort cube_indices[] =
	{
		 0,  1,  2,  0,  2,  3,
		 4,  5,  6,  4,  6,  7,
		 8,  9, 10,  8, 10, 11,
		12, 13, 14, 12, 14, 15,
		16, 17, 18, 16, 18, 19,
		20, 21, 22, 20, 22, 23
	};

	std::cout << glGetString(GL_VERSION) << " Running using intel GPU" << std::endl;

	// Shader load
	m_shader = new Shader();
	m_shader->CompileShader(m_engine->Get<FileSystem>()->GetPathname() + "Shaders\\Vertex Shaders\\phong.vs", GL_VERTEX_SHADER);
	m_shader->CompileShader(m_engine->Get<FileSystem>()->GetPathname() + "Shaders\\Fragment Shaders\\phong.fs", GL_FRAGMENT_SHADER);
	m_shader->Link();
	m_shader->Use();

	PointLight* light = this->CreateObject<PointLight>();
	light->transform.translation = glm::vec3(10.0f, 10.0f, 10.0f);

	m_vertexArray = new VertexArray(m_engine);
	m_vertexArray->CreateBuffer(6 * sizeof(GLfloat), sizeof(cube_vertices) / (6 * sizeof(GLfloat)), (void*)cube_vertices);// Two arrays dont forget to change back if neeeded!
	m_vertexArray->CreateIndexBuffer(GL_UNSIGNED_SHORT, sizeof(cube_vertices) / (5 * sizeof(GLushort)), (void*)cube_indices);

	m_vertexArray->SetAttribute(0, 3, (6 * sizeof(GLfloat)), 0);
	m_vertexArray->SetAttribute(1, 3, (6 * sizeof(GLfloat)), (void*)(3 * sizeof(GLfloat)));
	//m_vertexArray->SetAttribute(2, 2, (6 * sizeof(GLfloat)), (void*)(3 * sizeof(GLfloat)));

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

	//glEnableVertexAttribArray(2);
	//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

	// Input
	m_engine->Get<Input>()->AddAction("left", SDL_SCANCODE_LEFT, Input::KEYBOARD);
	m_engine->Get<Input>()->AddAction("right", SDL_SCANCODE_RIGHT, Input::KEYBOARD);
	m_engine->Get<Input>()->AddAction("foreward", SDL_SCANCODE_UP, Input::KEYBOARD);
	m_engine->Get<Input>()->AddAction("backward", SDL_SCANCODE_DOWN, Input::KEYBOARD);

	m_engine->Get<Input>()->AddAction("rotateRight", SDL_SCANCODE_D, Input::KEYBOARD);
	m_engine->Get<Input>()->AddAction("rotateLeft", SDL_SCANCODE_A, Input::KEYBOARD);
	m_engine->Get<Input>()->AddAction("rotateUp", SDL_SCANCODE_W, Input::KEYBOARD);
	m_engine->Get<Input>()->AddAction("rotateDown", SDL_SCANCODE_S, Input::KEYBOARD);

	m_mxProjection = glm::perspective(glm::radians(45.0f), ((float)m_engine->Get<Renderer>()->GetWidth() / (float)m_engine->Get<Renderer>()->GetHeight()), 0.01f, 1000.0f);
	m_mxModel = glm::mat4(1.0f);
	m_transform.scale = glm::vec3(5.0f);

	m_camera = this->CreateObject<Camera>("camera");

	m_material = new Material();
	m_material->ambient = glm::vec3(0.2f);
	m_material->diffuse = glm::vec3(0.0f, 0.0f, 1.0f);
	m_material->specular = glm::vec3(1.0f);
	m_material->shininess = 1.0f;
	m_shader->SetUniform("material.ambient", m_material->ambient);
	m_shader->SetUniform("material.diffuse", m_material->diffuse);
	m_shader->SetUniform("material.specular", m_material->specular);
	m_shader->SetUniform("material.shininess", m_material->shininess);

	m_light = this->CreateObject<PointLight>("pointlight");
	m_light->diffuse = glm::vec3(0.0f, 0.0f, 1.0f);
	m_light->transform.translation = glm::vec3(5.0f, 10.0f, 10.0f);

	m_shader->SetUniform("light.diffuse", glm::vec3(0.0f, 0.0f, 1.0f));
	m_shader->SetUniform("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));

	return true;
}

void Scene03::Shutdown()
{
	
}

void Scene03::Update()
{
	float dt = m_engine->Get<Timer>()->DeltaTime();

	// Left Right Up Down
	if (m_engine->Get<Input>()->GetActionButton("left") == Input::HELD) m_transform.translation.x -= 50.0f * dt;
	if (m_engine->Get<Input>()->GetActionButton("right") == Input::HELD) m_transform.translation.x += 50.0f * dt;
	if (m_engine->Get<Input>()->GetActionButton("foreward") == Input::HELD) m_transform.translation.z -= 50.0f * dt;
	if(m_engine->Get<Input>()->GetActionButton("backward") == Input::HELD) m_transform.translation.z += 50.0f * dt;

	// Rotation X and Y axis
	if (m_engine->Get<Input>()->GetActionButton("rotateRight") == Input::HELD) m_transform.rotation.y += 5.0f * dt;
	if (m_engine->Get<Input>()->GetActionButton("rotateLeft") == Input::HELD) m_transform.rotation.y -= 5.0f * dt;
	if (m_engine->Get<Input>()->GetActionButton("rotateUp") == Input::HELD) m_transform.rotation.x -= 5.0f * dt;
	if (m_engine->Get<Input>()->GetActionButton("rotateDown") == Input::HELD) m_transform.rotation.x += 5.0f * dt;

	// Update light
	PointLight* light = this->GetObject<PointLight>();
	light->Update();

	// Update Camera
	m_camera->SetView(glm::vec3(0.0f, 0.0f, 25.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	//m_mxView = glm::lookAt(glm::vec3(0.0f, 0.0f, 25.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	// Update normals
	glm::mat3 mxNormal = glm::mat3(m_camera->transform.matrix * m_transform.matrix);
	mxNormal = glm::inverse(mxNormal);
	mxNormal = glm::transpose(mxNormal);
	m_shader->SetUniform("normal_matrix", mxNormal);

	// Update model view
	glm::mat4 mxMV = m_camera->transform.matrix * m_transform.matrix;
	m_shader->SetUniform("model_view_matrix", mxMV);

	// Update MVP
	m_transform.Update();
	//m_mxModel = m_transform.matrix;
	glm::mat4 mxMVP = m_mxProjection * m_camera->transform.matrix * m_transform.matrix;

	m_shader->SetUniform("model_view_projection_matrix", mxMVP);
}

void Scene03::Render()
{
	m_engine->Get<Renderer>()->ClearBuffer();
	
	m_vertexArray->Draw(GL_TRIANGLES);

	m_engine->Get<Renderer>()->SwapBuffer();
}
