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

/*
 * This is the main structure for water simulation, that contains:
 * 1) Particle System Solver
 * 2) Particle System Data
 */
struct Water: public GLItem
{
public:
	explicit Water(HeightMap &map) : hmap(map) {};
	HeightMap	&hmap;
	std::vector<glm::vec3>	indices;
	Emiter emiter;

	void	update_particles();
	glm::vec3 to_coords(int x, int y, int z);
	void	add_volume(int x, int y, int z, float volume);
	void	emit();
};


Water	instance_water(HeightMap &hmap);


#endif
