/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Water.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ptyshevs <ptyshevs@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/07 14:49:36 by ptyshevs          #+#    #+#             */
/*   Updated: 2019/01/07 14:49:43 by ptyshevs         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WATER_H
#define WATER_H

#include <core.hpp>
#include <HeightMap.hpp>
#include <ParticleSystemData.hpp>
#include <ParticleSystemSolver.hpp>

/*
 * This is the main structure for water simulation, that contains:
 * 1) Particle System Solver
 * 2) Particle System Data
 */
struct Water: public GLItem
{
private:
	void					_updateBuffer();
public:
	explicit Water();

	HeightMap				*hmap;
	ParticleSystemData		*data;
	ParticleSystemSolver	*solver;
	std::vector<glm::vec3>	indices;
//	Emiter emiter;

	void	update_particles();
//	void	emit();
};


Water	instance_water(HeightMap *hmap, ParticleSystemData *data);

#endif