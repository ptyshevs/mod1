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

ParticleSystemSolver::ParticleSystemSolver() : _data(NULL) {}

void ParticleSystemSolver::simulation_step(double dt)
{
	beginAdvanceTimeStep();

	accumulateForces(dt);
	timeIntegration(dt);
	resolveCollision();

	endAdvanceTimeStep();
}

void ParticleSystemSolver::accumulateForces(double dt)
{
	// This resets forces by rewriting them with gravity constant
	accumulateExternalForces(); // gravity
}

void ParticleSystemSolver::accumulateExternalForces()
{
	size_t			n = _data->numOfParticles();
	float			mass = _data->mass();
	glm::vec3		gravity = _data->gravity();

	for (size_t i = 0; i < n; ++i)
		(*_data)[i].force = mass * gravity;
}

void	ParticleSystemSolver::timeIntegration(double dt) {
	// todo
}

void	ParticleSystemSolver::resolveCollision() {
	// todo
}

void ParticleSystemSolver::beginAdvanceTimeStep()
{
	//TODO
}

void ParticleSystemSolver::endAdvanceTimeStep()
{
	// TODO
}

