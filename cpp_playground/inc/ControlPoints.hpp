/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ControlPoints.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ptyshevs <ptyshevs@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/07 16:00:20 by ptyshevs          #+#    #+#             */
/*   Updated: 2019/01/07 16:00:26 by ptyshevs         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONTROL_POINTS_HPP
#define CONTROL_POINTS_HPP

#include <core.hpp>

/*
 * Structure containing control points used for surface interpolation
 */
struct ControlPoints {
private:
	void	_rescale();
	void	_scale_back();
	void	_add_borders();
public:
	std::vector<glm::vec3> _arr;

	ControlPoints();
	/*
	 * Creates ControlPoints object from vector of 3D points.
	 * No transformation is applied
	 */
	ControlPoints &operator=(const std::vector<glm::vec3> &m);
	/*
	** @brief Rescale control points and add border optionally
	**
	*/
	void	prepare(bool borders);
	/*
	 * Display control points onto STDOUT
	 */
	void 	show();
	/*
	 * Interpolate surface height at arbitrary point (y axis), ignoring
	 * y coordinate of a <point>
	 */
	float	idw(const glm::vec3 &point) const;
	/*
	 * Interpolate surface height (y axis) at arbitrary point
	 */
	float	idw(float x, float z) const;
};

#endif
