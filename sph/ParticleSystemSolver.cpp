/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParticleSystemSolver.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ptyshevs <ptyshevs@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/07 10:55:30 by ptyshevs          #+#    #+#             */
/*   Updated: 2019/01/07 10:55:31 by ptyshevs         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ParticleSystemSolver.hpp>
#include <iostream>

//ParticleSystemSolver::ParticleSystemSolver() : _data(ParticleSystemData()), _drag_coef(DRAG_COEF) {}

ParticleSystemSolver::ParticleSystemSolver(ParticleSystemData &data, bool interact) :_data(data), _interact(interact) {}

void ParticleSystemSolver::simulation_step(float dt)
{
	beginAdvanceTimeStep();

	accumulateForces(dt);
	timeIntegration(dt);
	resolveCollision();

	endAdvanceTimeStep();
}

void ParticleSystemSolver::accumulateForces(float dt)
{
	// This resets forces by rewriting them with gravity constant
	accumulateExternalForces(); // gravity and drag
	// Here should be force from pressure gradient
	if (_interact)
	{
		accumulatePressureForce();
		accumulateViscosityForce();
	}
}


void ParticleSystemSolver::accumulateExternalForces()
{
	size_t			n = _data.numOfParticles();
	float			mass = _data.mass();
	glm::vec3		gravity = _data.gravity();

	for (auto &p: _data._particles)
	{
		p.force = mass * gravity;
		p.force += - DRAG_COEF * p.velocity;
	}
}

void ParticleSystemSolver::accumulatePressureForce() {
	_data.compute_pressure();
	_data.add_pressure();
}

void	ParticleSystemSolver::accumulateViscosityForce() {
	_data.add_viscosity();
}

void	ParticleSystemSolver::timeIntegration(float dt) {
	const float 	mass = _data.mass();
	for (auto &p: _data._particles)
	{
		p.velocity += dt * p.force / mass; // F = ma -> a = F/m
		p.position +=  dt * p.velocity; // Use updated velocity to move particle position
	}
}

void	ParticleSystemSolver::resolveCollision() {
	float			h;

	for (auto &p: _data._particles){
		if (_data.hmap->out_of_bound(p.position))
		{
			// handle collision with boundary
			_data.hmap->bound(p.position);
			p.velocity *= -DAMPING;
			// ...
			continue ;
		}
		// No boundary crossing, maybe there's a surface?
		h = _data.hmap->surface_height(p.position);
		if (h > p.position.y)
		{
			surfaceCollision(p);
			p.position.y = h;
		}
	}
}

void 	ParticleSystemSolver::surfaceCollision(Particle &p) {
	glm::vec3 normal = _data.hmap->normal(p.position);
	float vel_mag = glm::length(p.velocity);
	glm::vec3 vel_normalized = p.velocity / vel_mag;
	glm::vec3 reflected = vel_normalized - (2 * glm::dot(vel_normalized, normal) * normal);
	glm::vec3 renormalized = reflected * (vel_mag);
	// Easy way
//	glm::vec3 frictioned = renormalized * RESTITUTION;
	// we prefer tangenial speed over
	glm::vec3 frictioned = renormalized - renormalized * 0.55 * fabs(glm::dot(vel_normalized, normal));
	p.velocity = frictioned;
}



void ParticleSystemSolver::beginAdvanceTimeStep()
{
	// here we should clear forces, but they're rewritten in accumulateExternalForces
	_data.cacheNeighbors();
	_data.update_densities();
}

/*
 * Update particles position and velocity, corrected for collisions
 */
void ParticleSystemSolver::endAdvanceTimeStep()
{
}



