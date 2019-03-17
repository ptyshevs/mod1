//
// Created by Pavlo TYSHEVSKYI on 2019-03-16.
//

#ifndef PROJECT_PARTICLE_HPP
#define PROJECT_PARTICLE_HPP
#include <iostream>
#include <vector>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "glm/gtx/string_cast.hpp"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "glm/gtx/string_cast.hpp"
#include <OpenGL/OpenGL.h>
#include <OpenCL/cl.h>
#include <OpenCL/cl_gl.h>
#include <OpenCL/cl_gl_ext.h>
# define MAX_NEIGHBORS 26

struct Particle {
	cl_float3		position;
	cl_float3		velocity;
	cl_float3		force;
	cl_float		density;
	cl_float		pressure;
	cl_uint			n_neighbors; // 52 bytes
	cl_uint			neighbors_idx[MAX_NEIGHBORS];
//	Particle	*neighbors[MAX_NEIGHBORS];
	Particle();
	Particle(const glm::vec3 &position, const glm::vec3 &velocity, const glm::vec3 &force);
	Particle(const glm::vec3 &position, const glm::vec3 &velocity, const glm::vec3 &force,
			float density, float pressure);
	void	show() const;
};

std::ostream &operator<<(std::ostream &o, const Particle &particle);



#endif //PROJECT_PARTICLE_HPP
