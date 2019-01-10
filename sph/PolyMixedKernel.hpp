/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PolyMixedKernel.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ptyshevs <ptyshevs@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/09 15:05:16 by ptyshevs          #+#    #+#             */
/*   Updated: 2019/01/09 15:05:18 by ptyshevs         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef POLY_MIXED_KERNEL_HPP
#define POLY_MIXED_KERNEL_HPP

#include <glm/glm.hpp>

#define KERNEL_RADIUS 3.95f

class PolyMixedKernel {
private:
	float _r;
	float _r2;
	float _const;
	float _dconst;
public:
	explicit PolyMixedKernel(float r = KERNEL_RADIUS);
	float	weight(const float &d) const;
	float	weight2(const float &d2) const;
	float	first_derivative(const float &d) const;
	float	second_derivative(const float &d) const;

	glm::vec3		gradient(const float &d, const glm::vec3 &dir) const;
};
#endif