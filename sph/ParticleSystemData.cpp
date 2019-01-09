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

Particle::Particle()  : position(glm::vec3(0.0f)), velocity(glm::vec3(0.0f)), force(glm::vec3(0.0f)), density(0.0f), pressure(0.0f) {};

Particle::Particle(const glm::vec3 &position, const glm::vec3 &velocity, const glm::vec3 &force) :
		position(position), velocity(velocity), force(force), density(0.0f), pressure(0.0f) {};

void	Particle::show() const {
	printf("pos [%0.2f, %0.2f, %0.2f] ", this->position.x, this->position.y,
			this->position.z);
	printf("vel [%0.2f, %0.2f, %0.2f] ", this->velocity.x, this->velocity.y,
			this->velocity.z);
	printf("F [%0.2f, %0.2f, %0.2f]\n", this->force.x, this->force.y, this->force.z);
}

std::ostream& operator<<(std::ostream &o, const Particle &particle)
{
	o << "pos [" << glm::to_string(particle.position) << "]";
	o << " vel [" << glm::to_string(particle.velocity) << "]";
	o << " F [" << glm::to_string(particle.force) << "]";
	o << " D: " << particle.density << " P: " << particle.pressure;
	return (o);
}

ParticleSystemData::ParticleSystemData(size_t numOfParticles) : _mass(PARTICLE_MASS), _gravity(glm::vec3(0.0, -9.81, 0.0))
{
	if (numOfParticles > 0)
	{
		this->_particles.reserve(numOfParticles);

	}
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

void ParticleSystemData::show(ssize_t i)
{
	if (i >= 0)
		return (*this)[i].show();
	for (size_t j = 0; j < this->numOfParticles(); ++j)
	{
		Particle &particle = (*this)[j];
		std::cout << j << ": " << particle << std::endl;
	}
}


// Naive implementation of neighbors list building
// Spacial hashing should be here instead, probably with different grid
void ParticleSystemData::cacheNeighbors()
{
	size_t n = numOfParticles();
	if (neighbors.size() < n)
	{
		for (size_t i = neighbors.size(); i < n; ++i)
			neighbors.push_back(std::vector<Particle *>());
	}
	for (size_t i = 0; i < n; ++i)
	{
		neighbors[i].clear();
		for (size_t j = 0; j < n; ++j)
		{
			if (i != j) {
				// calculate distance, and if close enough, add to neighbor list
				if (distance(_particles[i].position, _particles[j].position) < NEIGHBOR_RADIUS) {
					neighbors[i].push_back(&_particles[j]);
				}
			}
		}
	}
}

void	ParticleSystemData::update_densities() {
	size_t	n = numOfParticles();
	for (size_t i = 0; i < n; ++i)
	{
		float sum = 0;
		for (Particle *neighbor: neighbors[i]) {
			float dist = distance(_particles[i].position, neighbor->position);
			sum += kernel.weight(dist);
		}
//		_particles[i].density = _mass * sum;
		// this will add mass of the particle itself to calculations
		_particles[i].density = _mass * (sum + kernel.weight(0));
		if (_particles[i].density < 0) {
			std::cerr << "Density is negative! : " << _particles[i] << std::endl;
		}
	}
}

void 	ParticleSystemData::compute_pressure(bool clamp_negative) {
	size_t n = numOfParticles();
	for (size_t i = 0; i < n; ++i) {
		Particle &p = _particles[i];
		p.pressure = PRESSURE_CONST * (p.density - TARGET_DENSITY);
		if (clamp_negative && p.pressure < 0)
			p.pressure *= -NEGATIVE_PRESSURE_SCALE;
	}
}

// Add negative pressure gradient to the force
void	ParticleSystemData::add_pressure() {
	size_t n = numOfParticles();
	for (size_t i = 0; i < n; ++i) {
		Particle &p = _particles[i];
		for (Particle *neighbor: neighbors[i]) {
			float dist = distance(p.position, neighbor->position);
			if (dist > 0) {
				glm::vec3 dir = (p.position - neighbor->position) / dist; // normed vector
				glm::vec3 val = _mass * (p.pressure + neighbor->pressure) /
						(2 * p.density * neighbor->density) * kernel.first_derivative(dist) * dir;
//				std::cerr << glm::to_string(val) << std::endl;
				p.force -= val;
			}
		}
	}
}

float ParticleSystemData::distance(const glm::vec3 &a, const glm::vec3 &b) const
{
	return glm::distance(a, b);
}

