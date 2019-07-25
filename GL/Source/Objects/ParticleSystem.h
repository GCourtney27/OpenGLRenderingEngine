#pragma once
#include <vector>
#include "renderable.h"
#include "model.h"
#include <iostream>
#include "camera.h"

#define LOG(x) std::cout << x << std::endl;
#define ARRAY_SIZE 0;

struct Particle
{
	glm::vec3 position = glm::vec3(0.0f); //12
	glm::vec3 velocity = glm::vec3(0.0f); //12
	Model* model = nullptr; 
	float lifetime = 5.0f; //4
};

struct ParticleData
{
	std::vector<glm::vec3> positions; //12
	std::vector<glm::vec3> velocities; //12
	std::vector<Model*> models;
	std::vector<float> lifetimes; //4
	int particlesInSystem = 0; //4
};

class ParticleSystem : public Object
{
public:

	enum eSystemType
	{
		EXPLOSION, // Play once
		STREAM // Keep playing and streaming particles
	};
	enum eSystemModel
	{
		ECS,
		CLASSIC
	};
	
	ParticleSystem(Scene* scene, const std::string& name = "")
		: Object(scene, name) {}

	void Initialize(int particlesToSpawn, eSystemType systemType, eSystemModel modelType);
	void Update();
	void UpdateData();
	void AssignRandSpeed(Particle & _particle);
	void AssignRandLifetime(Particle & _particle);
	std::vector<Particle*>& GetParticles() { return m_particles; }
	ParticleData& GetParticleData() { return m_particleData; }


	void AssignRandLifetimePData(float & inData);
	void AssignRandSpeedPData(glm::vec3 & inData);

	float m_dt = 0.0f;
	Camera * m_mainCamera = nullptr;

private:
	int m_numParticles = 0;

	ParticleData m_particleData;
	size_t m_particleDataSize;

	std::vector<Particle*> m_particles;
	size_t m_particlesSize;

	eSystemType m_systemType = eSystemType::EXPLOSION;
};
