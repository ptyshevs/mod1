/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParticleSystemData.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ptyshevs <ptyshevs@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/07 10:55:22 by ptyshevs          #+#    #+#             */
/*   Updated: 2019/01/07 10:55:27 by ptyshevs         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ParticleSystemData.hpp>

Particle::Particle()  : position(glm::vec3(0.0f)), velocity(glm::vec3(0.0f)), force(glm::vec3(0.0f)) {};

Particle::Particle(const glm::vec3 &position, const glm::vec3 &velocity, const glm::vec3 &force) :
		position(position), velocity(velocity), force(force) {};

Particle::show() const {
	std::cout << "Position: " << glm::to_string(this->position) << std::endl;
}

ParticleSystemData::ParticleSystemData(size_t numOfParticles) : _mass(PARTICLE_MASS), _gravity(glm::vec3(0.0, -9.81, 0.0))
{
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

const float &ParticleSystemData::mass() const
{
	return (this->_mass);
}

const glm::vec3& ParticleSystemData::gravity() const
{
	return (this->_gravity);
}

Particle& ParticleSystemData::operator[](size_t i)
{
	return (this->_particles[i]);
}