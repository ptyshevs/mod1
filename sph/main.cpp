#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "glm/gtx/string_cast.hpp"

#include <vector>
#include <iostream>

struct Particle {
	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec3 force;

	Particle();
	Particle(const glm::vec3 &position, const glm::vec3 &velocity, const glm::vec3 &force);
};

Particle::Particle()  : position(glm::vec3(0.0f)), velocity(glm::vec3(0.0f)), force(glm::vec3(0.0f)) {};

Particle::Particle(const glm::vec3 &position, const glm::vec3 &velocity, const glm::vec3 &force) :
		position(position), velocity(velocity), force(force) {};

struct ParticleSystemData {
private:
	std::vector<Particle> _particles;
	std::vector<glm::vec3>	_positions;
	std::vector<glm::vec3>	_velocities;
	std::vector<glm::vec3>	_forces;
public:
	size_t	numOfParticles() const;
	explicit ParticleSystemData(size_t numOfParticles = 0);
	void addParticle(const glm::vec3 &position,
					 const glm::vec3 &velocity = glm::vec3(0.0f, 0.0f, 0.0f),
					 const glm::vec3 &force = glm::vec3(0.0f, 0.0f, 0.0f));
	void addParticles(std::vector<glm::vec3> positions,
					  std::vector<glm::vec3> velocities,
					  std::vector<glm::vec3> forces);
};

ParticleSystemData::ParticleSystemData(size_t numOfParticles) {
	if (numOfParticles > 0)
		this->_particles.reserve(numOfParticles);
}

size_t ParticleSystemData::numOfParticles() const {
	return (this->_particles.size());
}

void ParticleSystemData::addParticle(const glm::vec3 &position, const glm::vec3 &velocity, const glm::vec3 &force) {
	this->_particles.emplace_back(position, velocity, force);
}

void ParticleSystemData::addParticles(std::vector<glm::vec3> positions, std::vector<glm::vec3> velocities,
									  std::vector<glm::vec3> forces) {
	for (size_t i = 0; i < positions.size(); ++i) {
		this->_particles.emplace_back(positions[i], velocities[i], forces[i]);
	}
}

class ParticleSystemSolver {
private:
	ParticleSystemData	*data;
public:
	ParticleSystemSolver();
};

int main(int ac, char **av)
{
	ParticleSystemData data;
	data.addParticle(glm::vec3(0));
	data.addParticle(glm::vec3(1));
	std::cout << data.numOfParticles() << std::endl;
}