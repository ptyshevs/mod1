/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   control_points.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ptyshevs <ptyshevs@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/25 13:50:11 by ptyshevs          #+#    #+#             */
/*   Updated: 2018/12/25 13:51:18 by ptyshevs         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "core.hpp"


/*
** @brief Scale control points to [0, 1] range.
**
** Divide each coordinate by the corresponding maximum value.
**
** Complexity: O(2n)
**
** @param cpoints Vector of control points
*/

void rescale(std::vector<glm::vec3> &cpoints)
{
	float x_max, y_max, z_max;

	x_max = INT_MIN;
	y_max = INT_MIN;
	z_max = INT_MIN;

	for (glm::vec3 cpoint: cpoints)
	{
		if (cpoint.x > x_max)
			x_max = cpoint.x;
		if (cpoint.y > y_max)
			y_max = cpoint.y;
		if (cpoint.z > z_max)
			z_max = cpoint.z;
	}
	for (auto &point : cpoints)
	{
		point.x /= x_max;
		point.y /= y_max;
		point.z /= z_max;
	}
}

/*
** @brief Scale normalized [0, 1] control points to map range [-hf_sl, hf_sl]
**
** If there is a single control point, center it at the origin and give height = hf_sl / 2
** (since cp.y is always 1).
**
** if more than one points:
** 1) scale [0, 1] to [0, hf_sl]
** 2) center [0, hf_sl] to [-hf_sl * 0.6, 0.4 * hf_sl]
** 3) restore height (y coordinate) from [-hf_sl * 0.6, 0.4 * hf_sl] to [-hf_sl * 0.2, 0.8 * hf_sl]
**
** @param cpoints Control points vector
*/

void scale_back(std::vector<glm::vec3> &cpoints)
{
	if (cpoints.size() == 1)
	{
		glm::vec3 &cp = cpoints[0];
		cp -= 1; // all coordinates are always 1, since they are divided by themselves in rescale
		cp.y = (float)hf_sl / 8;
		return ;
	}
	for (glm::vec3 &cp: cpoints)
	{
		cp *= (float)hf_sl; // scaling
		cp -= (float)hf_sl * 0.5; // centering
		cp.y += (float)hf_sl; // scale down height
		cp.y *= 0.2; // scale height more
	}
}


void	show_points(std::vector<glm::vec3> &cpoints)
{
	for (glm::vec3 cp: cpoints)
	{
		std::cout << glm::to_string(cp) << std::endl;
	}
}

/*
** @brief Add borders to control points (needed for proper interpolation)
**
** You can control border points density by varying the step
** @param cpoints 
*/

void 	add_borders(std::vector<glm::vec3> &cpoints)
{
	for (int i = 0; i < sl; i += 3)
	{
		cpoints.emplace_back(i - hf_sl, 0, -hf_sl);
		cpoints.emplace_back(-hf_sl, 0, i - hf_sl);
		cpoints.emplace_back(hf_sl, 0, i - hf_sl);
		cpoints.emplace_back(i - hf_sl, 0, hf_sl);
	}
}

/*
** @brief Rescale control points and add border
** 
** @param cpoints 
*/

void	prepare_control_points(std::vector<glm::vec3> &cpoints)
{
	rescale(cpoints);
	scale_back(cpoints);
	add_borders(cpoints);
}
