/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PolyMixedKernel.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ptyshevs <ptyshevs@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/09 15:05:22 by ptyshevs          #+#    #+#             */
/*   Updated: 2019/01/09 15:05:23 by ptyshevs         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <PolyMixedKernel.hpp>

PolyMixedKernel::PolyMixedKernel(float r) : _r(r), _r2(r * r)
{
	_const = 315.0f / (64.0f * M_PI * pow(r, 9));
	_dconst = 45.0f / (M_PI * pow(r, 6));
}

// Poly6 kernel for weighting
float PolyMixedKernel::weight(const float &d) const
{
	if (d >= _r)
		return (0.0f);
	float x = _r2 - d * d;
	return (_const * x * x * x);
}

// Poly6 kernel, but one can use dot product on difference vector, avoiding sqrt
float PolyMixedKernel::weight2(const float &d2) const
{
	if (d2 >= _r2)
		return (0.0f);
	float x = _r2 - d2;
	return (_const * x * x * x);
}

// Spiky kernel derivative
float PolyMixedKernel::first_derivative(const float &d) const
{
	if (d >= _r)
		return (0.0f);
	float x = _r - d;
	return (-_dconst * x * x);
}

// Another polynomial kernel second derivative
float PolyMixedKernel::second_derivative(const float &d) const
{
	if (d >= _r)
		return (0.0f);
	return (_dconst * (_r - d));
}

glm::vec3 PolyMixedKernel::gradient(const float &d, const glm::vec3 &dir) const
{
	return (-first_derivative(d) * dir);
}

