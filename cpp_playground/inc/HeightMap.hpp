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
#include <unordered_set>

/*
 * Height map is a pseudo-3D data structure containing surface height at every point on
 * a rectangular grid. It uses hashing for accessing individual cells
 */
class HeightMap: public GLItem {
private:
	float	_dx(const glm::vec3 &grid_position);
	float	_dz(const glm::vec3 &grid_position);
public:
	const ControlPoints	&_cpoints;
	std::vector<Cell>	hmap;

	explicit		HeightMap(const ControlPoints &cpoints);
	void		show() const;
	ssize_t		hash(int i, int j, int k) const;
	ssize_t		hash(glm::vec3 xyz) const;
	Cell		&address(int i, int j, int k);
	Cell		&address(glm::vec3 pos);
	glm::vec3	pos_to_3D_idx(glm::vec3 const &pos) const;

	void		bound(glm::vec3 &position);
	bool		out_of_bound(const glm::vec3 &position) const;
	glm::vec3	normal(const glm::vec3 &position);

	float		interpolate(const glm::vec3 &point) const;
	float		interpolate(float x, float z) const;
	glm::vec3	closest_surface_point(glm::vec3 &point) const;
	float		surface_height(const glm::vec3 &point) const;
//	Cell		&find_surface(glm::vec3 &position const);
};

/*
 * Generate HeightMap from a set of control points. This is the main routine
 * that should be used for this purpose, as it also creates VBO and textures for
 * further simulation and rendering purposes
 */
HeightMap generate_map(const ControlPoints &control_points);

#endif
