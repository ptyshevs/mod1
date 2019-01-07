/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Water.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ptyshevs <ptyshevs@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/25 21:14:47 by ptyshevs          #+#    #+#             */
/*   Updated: 2018/12/25 21:14:48 by ptyshevs         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "core.hpp"
#include <Water.hpp>

Water	instance_water(HeightMap *hmap, ParticleSystemData *data)
{
	Water w;

	w.hmap = hmap;
	w.data = data;

	w.model = glm::mat4(1.0f);
	w.shader_program = compile_shaders("src/shaders/water_vertex.glsl",
										"src/shaders/water_fragment.glsl");
	w.fill_uniforms = [&](const glm::mat4 &vp) {
		auto mvp_id = glGetUniformLocation(w.shader_program, "MVP");
		auto mvp = vp * w.model;
		glUniformMatrix4fv(mvp_id, 1, GL_FALSE, glm::value_ptr(mvp));
	};

	glGenBuffers(1, &w.vbo);
	glGenVertexArrays(1, &w.vao);

	glBindVertexArray(w.vao);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, w.vbo);
	glBufferData(GL_ARRAY_BUFFER, w.data->numOfParticles() * sizeof(Particle), w.data->_particles.data(), GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, sizeof(Particle), 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, sizeof(Particle), (void *)(sizeof(float) * 3));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_TRUE, sizeof(Particle), (void *)(sizeof(float) * 6));
	glBindVertexArray(0);

	return (w);
}

Water::Water() : hmap(nullptr), solver(nullptr)
{
}

void Water::update_particles()
{
	solver->simulation_step();
	_updateBuffer();
}

void Water::_updateBuffer()
{
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, data->numOfParticles() * sizeof(Particle), data->_particles.data(), GL_DYNAMIC_DRAW);
	glBindVertexArray(0);
	idx_num = data->numOfParticles();
}