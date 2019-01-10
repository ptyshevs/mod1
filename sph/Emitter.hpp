/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Emitter.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ptyshevs <ptyshevs@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/10 15:41:39 by ptyshevs          #+#    #+#             */
/*   Updated: 2019/01/10 15:41:40 by ptyshevs         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef EMITTER_HPP
#define EMITTER_HPP

#include "ParticleSystemData.hpp"

class Emitter {
private:
	ParticleSystemData	&_data;
	glm::vec3			_velocity;
	glm::vec3			_force;
	float				_density;
	float				_pressure;
public:
	Emitter(ParticleSystemData &data);
	void	setVelocity(glm::vec3 &velocity);
	void	setForce(glm::vec3 &force);
	void	setDensity(float density);
	void	setPressure(float pressure);
	void	cube(float step = 0.5f,
			float x_start = 0, float x_end = 1,
			float y_start = 0, float y_end = 1,
			float z_start = 0, float z_end = 1);
};

#endif
