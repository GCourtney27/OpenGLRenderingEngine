#include "ParticleSystem.h"
#include <iostream>
#include <ctime>
#include <chrono>


void ParticleSystem::Initialize(int particlesToSpawn, eSystemType systemType, eSystemModel modelType)
{
	std::srand((unsigned)std::time(0)); // Initialize clock

	m_particlesSize = particlesToSpawn;
	m_particleDataSize = particlesToSpawn;
	m_systemType = systemType;
	m_particleData.particlesInSystem = particlesToSpawn;

	for (int i = 0; i < particlesToSpawn; i++)
	{
		/*Classic particle system*/
		/*Particle* p = new Particle();
		AssignRandSpeed(*p);
		AssignRandLifetime(*p);
		m_particles.push_back(p);*/


		/*ECS particle system*/
		glm::vec3 speed = glm::vec3(0.0f);
		float lifetime = 0.0f;
		AssignRandSpeedPData(speed);
		AssignRandLifetimePData(lifetime);
		m_particleData.positions.push_back(glm::vec3(0.0f));
		m_particleData.velocities.push_back(speed);
		m_particleData.lifetimes.push_back(5.0f);

	}

}

float timer = 1.0f;
void ParticleSystem::Update()
{
	/*using namespace std::chrono;
	high_resolution_clock::time_point start = high_resolution_clock::now();
	high_resolution_clock::time_point end;*/

	/*ECS particle system*/
	UpdateData();

	/*Classic partical system*/
	//for (int i = 0; i < m_particlesSize; i++)
	//{
	//	Particle* p = m_particles[i];

	//	// Calculate position
	//	p->position += p->velocity * m_dt;
	//	p->model->transform.translation = p->position;
	//	
	//	// Rotate billboard for model
	//	p->model->transform.rotation = m_mainCamera->transform.rotation;

	//	// Calculate lifetime
	//	p->lifetime -= m_dt;
	//	if (p->lifetime <= 0.0f)
	//	{
	//		// Reset or delete particle based on system type
	//		if (m_systemType == eSystemType::EXPLOSION)
	//			p->model->enableDraw = false;
	//		if (m_systemType == eSystemType::STREAM)
	//		{
	//			AssignRandSpeed(*p);
	//			AssignRandLifetime(*p);
	//			p->position = glm::vec3(0.0f);
	//		}
	//	}
	//}

	/*end = high_resolution_clock::now();
	duration<double> time_span = duration_cast<duration<double>>(end - start);


	timer -= m_dt;
	if (timer <= 0.0f)
	{
		timer = 1.0f;
		LOG(time_span.count());
	}*/
}

void ParticleSystem::UpdateData()
{
	int particles = m_particleData.particlesInSystem;

	for (int i = 0; i < particles; i++) // Update positions
	{
		m_particleData.positions[i] += m_particleData.velocities[i] * m_dt;

		m_particleData.models[i]->transform.translation = m_particleData.positions[i];

		m_particleData.lifetimes[i] -= m_dt;
		if (m_particleData.lifetimes[i] <= 0.0f)
		{
			// Reset or delete particle based on system type
			if (m_systemType == eSystemType::EXPLOSION)
				m_particleData.models[i]->enableDraw = false;
			if (m_systemType == eSystemType::STREAM)
			{
				m_particleData.positions[i] = glm::vec3(0.0f);
				AssignRandLifetimePData(m_particleData.lifetimes[i]);
				AssignRandSpeedPData(m_particleData.velocities[i]);
			}
		}
		
	}

}

void ParticleSystem::AssignRandLifetimePData(float & inData)
{
	float lifetime = (float)(rand() % 20 + 1);
	inData = lifetime;
}

void ParticleSystem::AssignRandSpeedPData(glm::vec3 & inData)
{
	float velX = (float)(rand() % 20 + 1);
	if (velX > 10)
	{
		velX = -(velX - 10);
	}
	float velY = (float)(rand() % 20 + 1);
	if (velY > 10)
	{
		velY = -(velY - 10);
	}
	float velZ = (float)(rand() % 20 + 1);
	if (velZ > 10)
	{
		velZ = -(velZ - 10);
	}

	inData.r = velX;
	inData.g = velY;
	inData.b = velZ;
}

void ParticleSystem::AssignRandSpeed(Particle & _particle)
{
	float velX = (float)(rand() % 20 + 1);
	if (velX > 10)
	{
		velX = -(velX - 10);
	}
	float velY = (float)(rand() % 20 + 1);
	if (velY > 10)
	{
		velY = -(velY - 10);
	}
	float velZ = (float)(rand() % 20 + 1);
	if (velZ > 10)
	{
		velZ = -(velZ - 10);
	}
	_particle.velocity = glm::vec3(velX, velY, velZ);
	//LOG(velX << " , " << velY << " , " << velZ);

}

void ParticleSystem::AssignRandLifetime(Particle & _particle)
{
	float lifetime = (float)(rand() % 10 + 1);
	_particle.lifetime = lifetime;
	//LOG("Lifetime: " << lifetime);
}
