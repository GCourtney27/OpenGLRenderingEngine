#pragma once

// Graphics
#include <glad\glad.h>
#include <sdl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

// Graphics Libraries
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"

// Resources
#include <string>
#include <vector>
#include <map>

// Files
#include "system.h"
#include "timer.h"
#include "input.h"
#include "file_system.h"
#include "Renderer/renderer.h"
#include "Math/transform.h"
#include "object.h"

class Engine
{
public:
	Engine();
	~Engine();

	bool Initialize();
	void Shutdown();
	void Update();
	
	// get engine system
	template <typename T>
	T* Get()
	{
		for (System* system : m_systems)
		{
			if (dynamic_cast<T*>(system) != nullptr)
			{
				return static_cast<T*>(system);
			}
		}
		return nullptr;
	}

	bool ForceQuit() { return m_quit; }
	SDL_Event& GetEvent() { return m_event; }

private:

	std::vector<class System*> m_systems;
	bool m_quit = false;
	SDL_Event m_event;
};