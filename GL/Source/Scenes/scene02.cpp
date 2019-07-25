#include "scene02.h"
#include "Objects/camera.h"

bool Scene02::Initialize()
{
	// Shader load
	m_shader = new Shader();
	m_shader->CompileShader(m_engine->Get<FileSystem>()->GetPathname() + "Shaders\\Vertex Shaders\\basic2.vs", GL_VERTEX_SHADER);
	m_shader->CompileShader(m_engine->Get<FileSystem>()->GetPathname() + "Shaders\\Fragment Shaders\\basic2.fs", GL_FRAGMENT_SHADER);
	m_shader->Link();
	m_shader->Use();

	// Vertex buffer data
	static float cube_vertices[] = {
		// Front
		-1.0f, -1.0f,  1.0f, 0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f, 1.0f,
		-1.0f,  1.0f,  1.0f, 0.0f, 1.0f,
		// Right
		 1.0f, -1.0f,  1.0f, 0.0f, 0.0f,
		 1.0f, -1.0f, -1.0f, 1.0f, 0.0f,
		 1.0f,  1.0f, -1.0f, 1.0f, 1.0f,
		 1.0f,  1.0f,  1.0f, 0.0f, 1.0f,
		 // Back
		 -1.0f, -1.0f, -1.0f, 0.0f, 0.0f,
		 -1.0f,  1.0f, -1.0f, 1.0f, 0.0f,
		  1.0f,  1.0f, -1.0f, 1.0f, 1.0f,
		  1.0f, -1.0f, -1.0f, 0.0f, 1.0f,
		  // Left
		  -1.0f, -1.0f,  1.0f, 0.0f, 0.0f,
		  -1.0f,  1.0f,  1.0f, 1.0f, 0.0f,
		  -1.0f,  1.0f, -1.0f, 1.0f, 1.0f,
		  -1.0f, -1.0f, -1.0f, 0.0f, 1.0f,
		  // Bottom
		  -1.0f, -1.0f,  1.0f, 0.0f, 0.0f,
		  -1.0f, -1.0f, -1.0f, 1.0f, 0.0f,
		   1.0f, -1.0f, -1.0f, 1.0f, 1.0f,
		   1.0f, -1.0f,  1.0f, 0.0f, 1.0f,
		   // Top
		   -1.0f,  1.0f,  1.0f, 0.0f, 0.0f,
			1.0f,  1.0f,  1.0f, 1.0f, 0.0f,
			1.0f,  1.0f, -1.0f, 1.0f, 1.0f,
		   -1.0f,  1.0f, -1.0f, 0.0f, 1.0f
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


	//const GLushort indexBuffer[] =
	//{
	//	0, 1, 2,
	//	2, 3, 0
	//};

	m_vertexArray = new VertexArray(m_engine);
	m_vertexArray->CreateBuffer(5 * sizeof(GLfloat), sizeof(cube_vertices) / (5 * sizeof(GLfloat)), (void*)cube_vertices);
	m_vertexArray->CreateIndexBuffer(GL_UNSIGNED_SHORT, sizeof(cube_vertices) / (5 * sizeof(GLushort)), (void*)cube_indices);

	m_vertexArray->SetAttribute(0, 3, (5 * sizeof(GLfloat)), 0);
	m_vertexArray->SetAttribute(1, 2, (5 * sizeof(GLfloat)), (void*)(3 * sizeof(GLfloat)));
	//m_vertexArray->SetAttribute(2, 2, (8* sizeof(GLfloat)), (void*)(6 * sizeof(GLfloat)));

	//GLuint position = glGetAttribLocation(shaderProgramID, "position");
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

	//glEnableVertexAttribArray(2);
	//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));

	// Texture
	GLuint textureID = Material::LoadTexture(m_engine->Get<FileSystem>()->GetPathname() + "Textures\\face.png");
	glBindTexture(GL_TEXTURE_2D, textureID);

	glBindTexture(GL_TEXTURE_2D, textureID);

	// Input
	m_engine->Get<Input>()->AddAction("left", SDL_SCANCODE_LEFT, Input::KEYBOARD);
	m_engine->Get<Input>()->AddAction("right", SDL_SCANCODE_RIGHT, Input::KEYBOARD);
	m_engine->Get<Input>()->AddAction("up", SDL_SCANCODE_UP, Input::KEYBOARD);
	m_engine->Get<Input>()->AddAction("down", SDL_SCANCODE_DOWN, Input::KEYBOARD);

	m_engine->Get<Input>()->AddAction("rotateRight", SDL_SCANCODE_D, Input::KEYBOARD);
	m_engine->Get<Input>()->AddAction("rotateLeft", SDL_SCANCODE_A, Input::KEYBOARD);
	m_engine->Get<Input>()->AddAction("rotateUp", SDL_SCANCODE_W, Input::KEYBOARD);
	m_engine->Get<Input>()->AddAction("rotateDown", SDL_SCANCODE_S, Input::KEYBOARD);

	m_mxProjection = glm::perspective(glm::radians(45.0f), ((float)m_engine->Get<Renderer>()->GetWidth() / (float)m_engine->Get<Renderer>()->GetHeight()), 0.01f, 1000.0f);
	m_mxModel = glm::mat4(1.0f);
	m_transform.scale = glm::vec3(5.0f);

	m_camera = new Camera(this);
	

	return true;
}

void Scene02::Shutdown()
{
	
}

void Scene02::Update()
{
	float dt = m_engine->Get<Timer>()->DeltaTime();

	// Left Right Up Down
	if (m_engine->Get<Input>()->GetActionButton("left") == Input::HELD) m_transform.translation.x -= 25.0f * dt;
	if (m_engine->Get<Input>()->GetActionButton("right") == Input::HELD) m_transform.translation.x += 25.0f * dt;
	if (m_engine->Get<Input>()->GetActionButton("up") == Input::HELD) m_transform.translation.y += 25.0f * dt;
	if(m_engine->Get<Input>()->GetActionButton("down") == Input::HELD) m_transform.translation.y -= 25.0f * dt;

	// Rotation X and Y axis
	if (m_engine->Get<Input>()->GetActionButton("rotateRight") == Input::HELD) m_transform.rotation.y += 3.0f * dt;
	if (m_engine->Get<Input>()->GetActionButton("rotateLeft") == Input::HELD) m_transform.rotation.y -= 3.0f * dt;
	if (m_engine->Get<Input>()->GetActionButton("rotateUp") == Input::HELD) m_transform.rotation.x -= 3.0f * dt;
	if (m_engine->Get<Input>()->GetActionButton("rotateDown") == Input::HELD) m_transform.rotation.x += 3.0f * dt;


	m_camera->SetView(glm::vec3(0.0f, 0.0f, 25.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	//m_mxView = glm::lookAt(glm::vec3(0.0f, 0.0f, 25.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	m_transform.Update();
	m_mxModel = m_transform.matrix;
	//glm::mat4 mxMP = m_mxProjection * m_mxModel;
	glm::mat4 mxMVP = m_mxProjection * m_camera->transform.matrix * m_mxModel;

	m_shader->SetUniform("model", mxMVP);
}

void Scene02::Render()
{
	m_engine->Get<Renderer>()->ClearBuffer();
	
	m_vertexArray->Draw(GL_TRIANGLES);

	m_engine->Get<Renderer>()->SwapBuffer();
}
