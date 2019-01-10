/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HeightMap.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ptyshevs <ptyshevs@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/07 14:49:09 by ptyshevs          #+#    #+#             */
/*   Updated: 2019/01/07 14:49:16 by ptyshevs         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEIGHTMAP_H
#define HEIGHTMAP_H

#include <core.hpp>
#include <ControlPoints.hpp>

class HeightMap: public GLItem {
private:
	const ControlPoints &_cpoints;
	float	_dx(const glm::vec3 &grid_position);
	float	_dz(const glm::vec3 &grid_position);
public:
	HeightMap(const ControlPoints &cpoints) : _cpoints(cpoints) {};
	std::vector<Cell>	hmap;

	void show() const;
	ssize_t		hash(int i, int j, int k) const;
	ssize_t		hash(glm::vec3 xyz) const;
	Cell		&address(int i, int j, int k);
	Cell		&address(glm::vec3 pos);
	void		bound(glm::vec3 &position);
	bool		out_of_bound(const glm::vec3 &position) const;
	glm::vec3	normal(const glm::vec3 &position);

	float		interpolate(const glm::vec3 &point) const;
	float		interpolate(float x, float z) const;
	glm::vec3	closest_surface_point(glm::vec3 &point) const;
	float		surface_height(const glm::vec3 &point) const;
//	Cell		&find_surface(glm::vec3 &position const);
};

HeightMap generate_map(const ControlPoints &control_points);

#endif
