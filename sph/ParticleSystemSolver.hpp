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
#include <HeightMap.hpp>

// like 60 FPS = 1/60 SPF
#define TIME_STEP 0.0166f
// increase to see how it would decrease relative velocity
#define DRAG_COEF 0.001f
#define RESTITUTION 0.98f
#define DAMPING 0.4f

class ParticleSystemSolver {
private:
	ParticleSystemData	&_data;
	std::vector<glm::vec3>	_new_velocities;
	std::vector<glm::vec3>	_new_positions;

	void	beginAdvanceTimeStep();
	void	endAdvanceTimeStep();

	void	accumulateForces(float dt);
	void	accumulateExternalForces();
	void	timeIntegration(float dt);
	void	resolveCollision();
public:
//	ParticleSystemSolver();
	ParticleSystemSolver(ParticleSystemData &data);

	void simulation_step(float dt = TIME_STEP);
};

#endif
