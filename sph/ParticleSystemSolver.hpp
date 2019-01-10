/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParticleSystemSolver.hpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ptyshevs <ptyshevs@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/07 10:55:32 by ptyshevs          #+#    #+#             */
/*   Updated: 2019/01/07 10:55:44 by ptyshevs         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARTICLESYSTEMSOLVER_H
#define PARTICLESYSTEMSOLVER_H

#include <ParticleSystemData.hpp>
#include <PolyMixedKernel.hpp>
#include <HeightMap.hpp>

// like 60 FPS = 1/60 SPF
#define TIME_STEP 0.0166f
// increase to see how it would decrease relative velocity
#define DRAG_COEF 0.004f
#define RESTITUTION 0.6f
#define DAMPING 0.15f

class ParticleSystemSolver {
private:
	bool	_interact;
	ParticleSystemData	&_data;

	// Temporary arrays for storing new velocity and positions, s.t. collision resolution

	void	beginAdvanceTimeStep();
	void	endAdvanceTimeStep();

	void	accumulateForces(float dt);
	void	accumulateExternalForces();

	void	accumulatePressureForce();

	void	timeIntegration(float dt);
	void	resolveCollision();
	void	surfaceCollision(Particle &p);
public:
//	ParticleSystemSolver();
	explicit ParticleSystemSolver(ParticleSystemData &data, bool interact = true);

	void simulation_step(float dt = TIME_STEP);
};

#endif
