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

#include <ControlPoints.hpp>

ControlPoints::ControlPoints() = default;

/*
** @brief Scale control points to [0, 1] range.
**
** Divide each coordinate by the corresponding maximum value.
**
** Complexity: O(2n)
**
** @param cpoints Vector of control points
*/

void ControlPoints::_rescale()
{
	float x_max, y_max, z_max;

	x_max = INT_MIN;
	y_max = INT_MIN;
	z_max = INT_MIN;

	for (glm::vec3 cpoint: this->_arr)
	{
		if (cpoint.x > x_max)
			x_max = cpoint.x;
		if (cpoint.y > y_max)
			y_max = cpoint.y;
		if (cpoint.z > z_max)
			z_max = cpoint.z;
	}
	for (auto &point : this->_arr)
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

void ControlPoints::_scale_back()
{
	if (this->_arr.size() == 1)
	{
		glm::vec3 &cp = this->_arr[0];
		cp -= 1; // all coordinates are always 1, since they are divided by themselves in rescale
		cp.y = (float)hf_sl / 3;
		return ;
	}
	for (glm::vec3 &cp: this->_arr)
	{
		cp *= (float)hf_sl; // scaling
		cp -= (float)hf_sl * 0.5; // centering
		cp.y += (float)hf_sl; // scale down height
		cp.y *= 0.3; // scale height more
	}
}


void	ControlPoints::show()
{
	for (glm::vec3 cp: this->_arr)
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

void 	ControlPoints::_add_borders()
{
	for (float i = 0; i < sl; i += 3.5f)
	{
		this->_arr.emplace_back(i - hf_sl, 0, -hf_sl);
		this->_arr.emplace_back(-hf_sl, 0, i - hf_sl);
		this->_arr.emplace_back(hf_sl, 0, i - hf_sl);
		this->_arr.emplace_back(i - hf_sl, 0, hf_sl);
	}
}

/*
** @brief Rescale control points and add border optionally
**
*/

void	ControlPoints::prepare(bool add_borders)
{
	this->_rescale();
	this->_scale_back();
	if (add_borders)
		this->_add_borders();
}

ControlPoints &ControlPoints::operator=(const std::vector<glm::vec3> &m)
{
	this->_arr = m;
	return (*this);
}

float ControlPoints::idw(float x, float z) const
{
	float num = 0;
	float denum = 0;
	for (const glm::vec3 &cp: _arr) {
		float w = 0;
		glm::vec3 diff(x - cp.x, -cp.y, z - cp.z);
		diff *= diff;
		float sqsum = diff.x + diff.z;
		w = 1.0f / (pow(sqsum, 1.3f) + 0.0001f);
		num += w * cp.y;
		denum += w;
	}
	return (num / denum);
}

float ControlPoints::idw(const glm::vec3 &point) const
{
	return idw(point.x, point.z);
}
