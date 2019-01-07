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

class ParticleSystemSolver {
private:
	ParticleSystemData	*_data;
	void	beginAdvanceTimeStep();
	void	endAdvanceTimeStep();

	void	accumulateForces(double dt);
	void	accumulateExternalForces();
	void	timeIntegration(double dt);
	void	resolveCollision();
public:
	ParticleSystemSolver();

	void simulation_step(double dt);
};

#endif
