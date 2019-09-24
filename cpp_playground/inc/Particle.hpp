/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Particle.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ptyshevs <ptyshevs@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/27 14:06:43 by ptyshevs          #+#    #+#             */
/*   Updated: 2019/04/27 14:06:51 by ptyshevs         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARTICLE_HPP
#define PARTICLE_HPP
#include <iostream>
#include <vector>

#define GLM_ENABLE_EXPERIMENTAL
#include "algo_defines.hpp"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "glm/gtx/string_cast.hpp"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "glm/gtx/string_cast.hpp"
#include <GL/gl.h>
#include <CL/cl.h>
#include <CL/cl_gl.h>
#include <CL/cl_gl_ext.h>
// #include <OpenGL/OpenGL.h>
// #include <OpenCL/opencl.h>
// #include <OpenCL/opencl.h>
// #include <OpenCL/opencl.h>

struct Particle {
	cl_float3		position;
	cl_float3		velocity;
	cl_float3		force;
	cl_float		density;
	cl_float		pressure;
	cl_float		viscosity;
	cl_uint			type;
	cl_uint			id; // id of the object particle belongs to
	cl_uint			n_neighbors; // 52 bytes
	cl_uint			neighbors_idx[MAX_NEIGHBORS];
	Particle();
	Particle(const glm::vec3 &position, const glm::vec3 &velocity, const glm::vec3 &force);
	Particle(const glm::vec3 &position, const glm::vec3 &velocity, const glm::vec3 &force,
			float density, float pressure, float viscosity, unsigned int type = P_DYNAMIC,
			unsigned int id = 0);
	void	show() const;
};

std::ostream &operator<<(std::ostream &o, const Particle &particle);

#endif
