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
	_velocity(glm::vec3(0.0f)), _force(glm::vec3(0.0f)), _density(0.0f), _pressure(0.0f), _step(0.5f), _scale(1.0f)
{
}

void Emitter::setVelocity(const glm::vec3 &velocity)
{
	_velocity = velocity;
}

void Emitter::setForce(const glm::vec3 &force)
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

void	Emitter::setStep(float step) {
	_step = step;
}

void	Emitter::setScale(float scale) {
	_scale = scale;
}

void	Emitter::cuboid(float x_start, float x_end,
		float y_start, float y_end, float z_start, float z_end)
{
	for (float i = x_start; i <= x_end; i += _step) {
		for (float j = y_start; j <= y_end; j += _step) {
			for (float k = z_start; k <= z_end; k += _step) {
				glm::vec3 pos(i, j, k);
				_data.addParticle(pos, _velocity, _force, _density, _pressure);
			}
		}
	}
}

void	Emitter::cube(const glm::vec3 &origin, float side) {
	for (float i = origin.x; i < origin.x + side; i += _step) {
		for (float j = origin.y; j < origin.y + side; j += _step) {
			for (float k = origin.z; k < origin.z + side; k += _step) {
				glm::vec3 pos(i, j, k);
				_data.addParticle(pos, _velocity, _force, _density, _pressure);
			}
		}
	}
}

void rescale(std::vector<glm::vec3> &points) {
	float x_max, y_max, z_max;

	x_max = INT_MIN;
	y_max = INT_MIN;
	z_max = INT_MIN;

	for (glm::vec3 cpoint: points)
	{
		if (cpoint.x > x_max)
			x_max = cpoint.x;
		if (cpoint.y > y_max)
			y_max = cpoint.y;
		if (cpoint.z > z_max)
			z_max = cpoint.z;
	}
	for (auto &point : points)
	{
		point.x /= x_max;
		point.y /= y_max;
		point.z /= z_max;
	}
}

void scale_back(std::vector<glm::vec3> &points) {
	for (glm::vec3 &cp: points)
	{
		cp *= (float)hf_sl; // scaling
		cp.x -= (float)hf_sl; // centering
		cp.z -= (float)hf_sl;
		cp.y -= (float)hf_sl - 25;
//		cp.y += (float)hf_sl; // scale down height
//		cp.y *= 0.3; // scale height more
	}
}

void Emitter::fromFile(std::string const &path)
{
	std::vector<glm::vec3> points = readFile((char *)path.c_str());
	rescale(points);
	scale_back(points);
	for (auto const &v: points) {
		_data.addParticle(_scale * v, _velocity, _force, _density, _pressure);
	}
}

void Emitter::fromFile(glm::vec3 const &origin, std::string const &path)
{
	std::vector<glm::vec3> points = readFile((char *)path.c_str());
	rescale(points);
	scale_back(points);
	for (auto const &v: points) {
		_data.addParticle(origin + _scale * v, _velocity, _force, _density, _pressure);
	}
}

