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
# define MAX_NEIGHBORS 10

struct Particle {
	glm::vec3	position;
	glm::vec3	velocity;
	glm::vec3	force;
	float		density;
	float		pressure;
	size_t		n_neighbors;
	Particle	*neighbors[MAX_NEIGHBORS];

	Particle();
	Particle(const glm::vec3 &position, const glm::vec3 &velocity, const glm::vec3 &force);
	Particle(const glm::vec3 &position, const glm::vec3 &velocity, const glm::vec3 &force,
			float density, float pressure);
	void	show() const;
};

std::ostream &operator<<(std::ostream &o, const Particle &particle);



#endif //PROJECT_PARTICLE_HPP
