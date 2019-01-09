/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PolyKernel.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ptyshevs <ptyshevs@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/08 21:45:37 by ptyshevs          #+#    #+#             */
/*   Updated: 2019/01/08 21:45:37 by ptyshevs         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef POLYKERNEL_H
#define POLYKERNEL_H

#include <glm/glm.hpp>

class PolyKernel {
private:
	float _radius;
	float _radius2;
	float _constant;
	float _deriv_constant;
public:
	PolyKernel(float radius);
	inline float	weight(const float &distance) const;
	inline float	weight2(const float &distance2) const;
	float			first_derivative(const float &distance) const;
	float			second_derivative(const float &distance) const;

	glm::vec3		gradient(const float &distance, const glm::vec3 &direction) const;
};
#endif
