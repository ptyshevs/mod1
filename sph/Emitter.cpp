/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Emitter.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ptyshevs <ptyshevs@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/10 15:41:36 by ptyshevs          #+#    #+#             */
/*   Updated: 2019/01/10 15:41:37 by ptyshevs         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Emitter.hpp"

Emitter::Emitter(ParticleSystemData &data) : _data(data),
	_velocity(glm::vec3(0.0f)), _force(glm::vec3(0.0f)), _density(0.0f), _pressure(0.0f)
{
}

void Emitter::setVelocity(glm::vec3 &velocity)
{
	_velocity = velocity;
}

void Emitter::setForce(glm::vec3 &force)
{
	_force = force;
}

void Emitter::setDensity(float density)
{
	_density = density;
}

void Emitter::setPressure(float pressure)
{
	_pressure = pressure;
}

void	Emitter::cube(float step, float x_start, float x_end,
		float y_start, float y_end, float z_start, float z_end)
{
	for (float i = x_start; i <= x_end; i += step) {
		for (float j = y_start; j <= y_end; j += step) {
			for (float k = z_start; k <= z_end; k += step) {
				glm::vec3 pos(i, j, k);
				_data.addParticle(pos, _velocity, _force);
			}
		}
	}
}
