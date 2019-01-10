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

struct ControlPoints {
private:
	void	_rescale();
	void	_scale_back();
	void	_add_borders();
public:
	std::vector<glm::vec3> _arr;

	ControlPoints();
	ControlPoints &operator=(const std::vector<glm::vec3> &m);
	void	prepare(bool borders);
	void 	show();
	float	idw(const glm::vec3 &point) const;
	float	idw(float x, float z) const;
};

#endif
