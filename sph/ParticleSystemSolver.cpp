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
	// todo: Add heightmap into consideration and bounding box
	size_t 			n = _data.numOfParticles();
	for (size_t i = 0; i < n; ++i){
		Particle &p = _data[i];
		auto new_position = _new_positions[i];
		auto new_velocity = _new_velocities[i];
		if (new_position.y < 0.0) // floor
		{
			_new_positions[i].y = 0;
			_new_velocities[i] *= -DAMPING;
		}
	}
}

void ParticleSystemSolver::beginAdvanceTimeStep()
{
	// here we should clear forces, but they're rewritten in accumulateExternalForces
	_new_positions.reserve(_data.numOfParticles());
	_new_velocities.reserve(_data.numOfParticles());
}

/*
 * Update particles position and velocity, corrected for collisions
 */
void ParticleSystemSolver::endAdvanceTimeStep()
{
	size_t n = _data.numOfParticles();
	for (size_t i = 0; i < n; ++i)
	{
		Particle &p = _data[i];
		p.position = _new_positions[i];
		p.velocity = _new_velocities[i];
	}
}

