#pragma once
#include <vector>
#include "renderable.h"

class ParticleSystem
{
public:
	ParticleSystem() {}



private:
	float m_particlesToSpawn = 0.0f;
	std::vector<Particle*> m_particles;
};

struct Particle
{
	glm::vec3 position;
};