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

#include <core.hpp>
#include <stdlib.h>
#include <time.h>


class Emitter: CLCore {
private:
	ParticleSystemData	&_data;
	glm::vec3			_velocity;
	glm::vec3			_force;
	float				_density;
	float				_pressure;
	float				_viscosity;
	unsigned int		_id;

	float				_step;
	float				_scale;
	float				_inflate;


public:
	explicit Emitter(ParticleSystemData &data);
	void	setVelocity(const glm::vec3 &velocity);
	void	setForce(const glm::vec3 &force);
	void	setDensity(float density);
	void	setPressure(float pressure);
	void	setViscosity(float viscosity);
	void	setId(unsigned int id);
	void	setStep(float step);
	void	setScale(float scale);
	void	setInflate(float inflate);
	void	setPointType(unsigned int type);
	void	cuboid(float x_start = 0, float x_end = 1,
				float y_start = 0, float y_end = 1,
				float z_start = 0, float z_end = 1);
	void	cube(const glm::vec3 &origin, float side);
	void	fromFile(std::string const &path);
	void	fromFile(glm::vec3 const &origin, std::string const &path);
	void	wall(float x_start, float x_end, float y_start, float y_end, float z_start, float z_end);
	void	pillow(glm::vec3 origin, float width=1, float radius=3, float height=20);
	void	sphere(glm::vec3 origin, float width=1, float radius=3);
	void	cache_solid_neighbors();

	void	emit();
	void 	prepare_emit(CLCore &core);
	unsigned int point_type;
	int		type; // rain, wave or boundaries
	int		prev_type;
	int 	pps; // particles per second
	cl_mem  cl_emitter;
	cl_mem  cl_vbo;
	cl_mem  cl_constants;
	};

#endif
