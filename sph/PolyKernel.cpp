/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PolyKernel.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ptyshevs <ptyshevs@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/08 21:45:32 by ptyshevs          #+#    #+#             */
/*   Updated: 2019/01/08 21:45:35 by ptyshevs         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <PolyKernel.hpp>

PolyKernel::PolyKernel(float radius) : _radius(radius), _radius2 (_radius * _radius)
{
	_constant = 315.0f / (64.0f * M_PI * pow(radius, 9));
	_deriv_constant = _constant * (-6.0f);
}

float PolyKernel::weight(const float &distance) const
{
	if (distance >= _radius)
		return (0.0f);
	else
	{
		float x = _radius2 - distance * distance;
		return (_constant * x * x * x);
	}
}

// Weight using squared distance
float PolyKernel::weight2(const float &distance2) const
{
	if (distance2 >= _radius2)
		return (0.0f);
	else
	{
		float x = _radius2 - distance2;
		return (_constant * x * x * x);
	}
}

float PolyKernel::first_derivative(const float &distance) const
{
	if (distance >= _radius)
		return (0.0f);
	else {
		float x = _radius2 - distance * distance;
		return (_deriv_constant * x * x);
	}
}

float PolyKernel::second_derivative(const float &distance) const
{
	if (distance >= _radius)
		return (0.0f);
	else {
		float x = _radius2 - distance * distance;
		float y = 5 * _radius2 - distance * distance;
		return (_deriv_constant * x * y);
	}
}

glm::vec3 PolyKernel::gradient(const float &distance, const glm::vec3 &direction) const
{
	return (-first_derivative(distance) * direction);
}
