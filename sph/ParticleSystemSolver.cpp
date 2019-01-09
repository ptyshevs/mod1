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

ParticleSystemSolver::ParticleSystemSolver(ParticleSystemData &data) : _data(data) {}

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
	accumulatePressureForce();
}


void ParticleSystemSolver::accumulateExternalForces()
{
	size_t			n = _data.numOfParticles();
	float			mass = _data.mass();
	glm::vec3		gravity = _data.gravity();

	for (size_t i = 0; i < n; ++i)
	{
		Particle &p = _data[i];
		p.force = mass * gravity;
		p.force += - DRAG_COEF * p.velocity;
	}
}

void ParticleSystemSolver::accumulatePressureForce() {
	_data.compute_pressure();

	//	exit(1);
}

void	ParticleSystemSolver::timeIntegration(float dt) {
	size_t			n = _data.numOfParticles();
	const float 	mass = _data.mass();
	for (size_t i = 0; i < n; ++i)
	{
		Particle &p = _data[i];
		_new_velocities[i] = p.velocity + dt * p.force / mass; // F = ma -> a = F/m
		_new_positions[i] = p.position +  dt * p.velocity; // Use updated velocity to move particle position
	}
}

void	ParticleSystemSolver::resolveCollision() {
	size_t 			n = _data.numOfParticles();
	for (size_t i = 0; i < n; ++i){
		Particle &p = _data[i];
//		auto &new_position = _new_positions[i];
		auto velocity = _new_velocities[i];
			if (_data.hmap->out_of_bound(_new_positions[i]))
			{
				// handle collision with boundary
				_data.hmap->bound(_new_positions[i]);
				_new_velocities[i] *= -DAMPING;
				// ...
				continue ;
			}
			// No boundary crossing, maybe there's a surface?
			if (_data.hmap->address(_new_positions[i]).is_solid)
			{
				// assume that this cell is right at the surface. If anything strange happens,
				// especially on high velicities, this will probably fail.
				glm::vec3 normal = _data.hmap->normal(_new_positions[i]);
				float vel_mag = glm::length(velocity);
				glm::vec3 vel_normalized = velocity / vel_mag;
				glm::vec3 reflected = vel_normalized - (2 * glm::dot(vel_normalized, normal) * normal);
				glm::vec3 renormalized = reflected * (vel_mag);
				// Easy way
//				glm::vec3 frictioned = renormalized * RESTITUTION;
				glm::vec3 frictioned = renormalized - renormalized * 0.05 * fabs(glm::dot(vel_normalized, normal));
				_new_velocities[i] = frictioned;
			}
	}
}

void ParticleSystemSolver::beginAdvanceTimeStep()
{
	// here we should clear forces, but they're rewritten in accumulateExternalForces
	_new_positions.reserve(_data.numOfParticles());
	_new_velocities.reserve(_data.numOfParticles());
	_data.cacheNeighbors();
	_data.update_densities();
}

/*
 * Update particles position and velocity, corrected for collisions
 */
void ParticleSystemSolver::endAdvanceTimeStep()
{
	// TODO: replace with range-based loop for efficiency
	size_t n = _data.numOfParticles();
	for (size_t i = 0; i < n; ++i)
	{
		Particle &p = _data[i];
		p.position = _new_positions[i];
		p.velocity = _new_velocities[i];
	}
}



