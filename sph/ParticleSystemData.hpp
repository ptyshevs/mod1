/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParticleSystemData.hpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ptyshevs <ptyshevs@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/07 10:55:25 by ptyshevs          #+#    #+#             */
/*   Updated: 2019/01/07 10:56:09 by ptyshevs         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARTICLE_SYSTEM_DATA_HPP
#define PARTICLE_SYSTEM_DATA_HPP

#include <HeightMap.hpp>
#include <PolyMixedKernel.hpp>
#include <iostream>
#include <vector>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "glm/gtx/string_cast.hpp"

#define PARTICLE_MASS 1.0f
#define NEIGHBOR_RADIUS 1.5f

#define PRESSURE_CONST 7.5f
#define TARGET_DENSITY 0.01f
#define NEGATIVE_PRESSURE_SCALE 0.5f

struct Particle {
	glm::vec3	position;
	glm::vec3	velocity;
	glm::vec3	force;
	float		density;
	float		pressure;

	Particle();
	Particle(const glm::vec3 &position, const glm::vec3 &velocity, const glm::vec3 &force);
	void	show() const;
};

std::ostream &operator<<(std::ostream &o, const Particle &particle);


struct ParticleSystemData {
private:

	const float 			_mass;
	const glm::vec3			_gravity;
public:
	std::vector<Particle>	_particles;
	std::vector<std::vector<Particle *>> neighbors;
	std::vector<float>		densities;
	std::vector<float>		pressures;
	HeightMap				*hmap;
	PolyMixedKernel			kernel;

	explicit ParticleSystemData(size_t numOfParticles = 0);

	void addParticle(const glm::vec3 &position,
					 const glm::vec3 &velocity = glm::vec3(0.0f, 0.0f, 0.0f),
					 const glm::vec3 &force = glm::vec3(0.0f, 0.0f, 0.0f));
	void addParticles(std::vector<glm::vec3> positions,
					  std::vector<glm::vec3> velocities,
					  std::vector<glm::vec3> forces);

	size_t	numOfParticles() const;
	const float &mass() const;
	const glm::vec3	&gravity() const;

	Particle	&operator[](size_t i);

	void	show(ssize_t i = -1);

	float	distance(const glm::vec3 &a, const glm::vec3 &b) const;
	void	cacheNeighbors();
	void	update_densities();
	void	compute_pressure(bool clamp_negative = false);
	void	add_pressure();
};

#endif
