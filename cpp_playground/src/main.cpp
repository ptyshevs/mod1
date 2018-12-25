/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ptyshevs <ptyshevs@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/09 16:40:21 by vpopovyc          #+#    #+#             */
/*   Updated: 2018/12/25 13:20:21 by ptyshevs         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <core.hpp>
#include <camera.hpp>
#include "limits.h"


struct Water
{
public:
	GLItem glitem;
	GLuint	n_particles;
	std::vector<glm::vec3> particles;
	std::vector<glm::vec3> indices;
	int		mode; // rain, wave of underground

	void	update_particles();
private:
	void	update_buffer();
};

void	Water::update_particles()
{
	for (glm::vec3 &particle: this->particles)
	{
		if (particle.y > -hf_sl)
			particle.y -= 0.1; // gravity
	}
	this->update_buffer();
}

void	Water::update_buffer()
{
	glBindVertexArray(this->glitem.vao);
	glBindBuffer(GL_ARRAY_BUFFER, this->glitem.vbo);
	glBufferData(GL_ARRAY_BUFFER, this->particles.size() * sizeof(glm::vec3), this->particles.data(), GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->glitem.ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(glm::ivec3), this->indices.data(), GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, sizeof(glm::vec3), 0);
	glBindVertexArray(0);
}

Water	instance_water(void)
{
	Water w;

	w.n_particles = 1;
	w.particles.emplace_back(10, hf_sl, 0);
	w.indices = {{0, 1, 2}, {1, 2, 3}, {2, 3, 4}, {3, 4, 5}, {4, 5, 6}, {5, 6, 7},
									   {6, 7, 8}, {7, 8, 9}, {8, 9, 10}, {9, 10, 11}, {10, 11, 12},
									   {11, 12, 13}, {12, 13, 14}, {13, 14, 15}, {14, 15, 16}, {15, 16, 17},
									   {16, 17, 18}, {17, 18, 19}, {18, 19, 20}, {19, 20, 21}};
	w.glitem.model = glm::mat4(1.0f);
	w.glitem.idx_num = w.indices.size() * 3;
	w.glitem.shader_program = compile_shaders("cpp_playground/src/shaders/water_vertex.glsl",
			"cpp_playground/src/shaders/water_fragment.glsl");
	w.glitem.fill_uniforms = [&](const glm::mat4 &vp) {
        auto mvp_id = glGetUniformLocation(w.glitem.shader_program, "MVP");
        auto mvp = vp * w.glitem.model;
        glUniformMatrix4fv(mvp_id, 1, GL_FALSE, glm::value_ptr(mvp));
    };

	glGenBuffers(1, &w.glitem.vbo);
	glGenBuffers(1, &w.glitem.ibo);
	glGenVertexArrays(1, &w.glitem.vao);
	glBindVertexArray(w.glitem.vao);
	glBindBuffer(GL_ARRAY_BUFFER, w.glitem.vbo);
	glBufferData(GL_ARRAY_BUFFER, w.particles.size() * sizeof(glm::vec3), w.particles.data(), GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, w.glitem.ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, w.indices.size() * sizeof(glm::ivec3), w.indices.data(), GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, sizeof(glm::vec3), 0);
	glBindVertexArray(0);
	return (w);
}

void draw(GLItem item, const glm::mat4 &vp, GLenum type);

void	process_input(GLCamera &camera, GLItem &map, GLItem &points, Water &water, bool *quit)
{
	auto keystate = SDL_GetKeyboardState(NULL);

	if (keystate[SDL_SCANCODE_ESCAPE])
		*quit = true;
	else if (keystate[SDL_SCANCODE_W])
		camera.pos += 0.1 * camera.speed * camera.dir;
	else if (keystate[SDL_SCANCODE_S])
		camera.pos -= 0.1 * camera.speed * camera.dir;
	else if (keystate[SDL_SCANCODE_E])
	{
		map.model = glm::rotate(map.model, camera.speed * glm::radians(0.1f), glm::vec3(0.0f, 1.0f, 0.0f));
		points.model = glm::rotate(points.model, camera.speed * glm::radians(0.1f), glm::vec3(0.0f, 1.0f, 0.0f));
		water.glitem.model = glm::rotate(water.glitem.model, camera.speed * glm::radians(0.1f), glm::vec3(0.0f, 1.0f, 0.0f));
	}
	else if (keystate[SDL_SCANCODE_Q])
	{
		map.model = glm::rotate(map.model, camera.speed * glm::radians(-0.1f), glm::vec3(0.0f, 1.0f, 0.0f));
		points.model = glm::rotate(points.model, camera.speed * glm::radians(-0.1f), glm::vec3(0.0f, 1.0f, 0.0f));
		water.glitem.model = glm::rotate(water.glitem.model, camera.speed * glm::radians(-0.1f), glm::vec3(0.0f, 1.0f, 0.0f));
	}
	else if (keystate[SDL_SCANCODE_UP])
	{
		map.model = glm::rotate(map.model, camera.speed * glm::radians(0.1f), glm::vec3(1.0f, 0.0f, 0.0f));
		points.model = glm::rotate(points.model, camera.speed * glm::radians(0.1f), glm::vec3(1.0f, 0.0f, 0.0f));
		water.glitem.model = glm::rotate(water.glitem.model, camera.speed * glm::radians(0.1f), glm::vec3(1.0f, 0.0f, 0.0f));
	}
	else if (keystate[SDL_SCANCODE_DOWN])
	{
		map.model = glm::rotate(map.model, camera.speed * glm::radians(-0.1f), glm::vec3(1.0f, 0.0f, 0.0f));
		points.model = glm::rotate(points.model, camera.speed * glm::radians(-0.1f), glm::vec3(1.0f, 0.0f, 0.0f));
		water.glitem.model = glm::rotate(water.glitem.model, camera.speed * glm::radians(-0.1f), glm::vec3(1.0f, 0.0f, 0.0f));
	}
}


int main(int ac, char *av[]) {
	bool quit = false;
	std::vector<glm::vec3> controlPointsArray;

	if (!inputIsGood(ac, av))
	{
		std::cout << "File is not readable\n";
		return (1);
	}
	try {
		controlPointsArray = readFile(av[1]);
		} catch (std::exception &e) {
			std::cout << e.what() << std::endl;
			return (1);
		}

	prepare_control_points(controlPointsArray);
	auto core = sdl_gl_init();

	auto map = generate_map(controlPointsArray);
	auto points = generate_control_points(controlPointsArray);
	auto water = instance_water();

	auto camera = GLCamera();
	glPointSize(3);
	while(!quit)
	{
		// Event handle
		SDL_PollEvent(&(core.event));
		process_input(camera, map, points, water, &quit);
		if (core.event.type == SDL_QUIT)
			quit = true;

		// Simulation step
		water.update_particles();

		// Actual render

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		camera.frameStart();
		draw(map, camera.vp(), GL_TRIANGLES);
		draw(points, camera.vp(), GL_POINTS);
		draw(water.glitem, camera.vp(), GL_POINTS);
		camera.frameEnd();

		SDL_GL_SwapWindow(core.win);
	}

	deinit(core);

}

void draw(GLItem item, const glm::mat4 &vp, GLenum type)
{
	glUseProgram(item.shader_program);
	item.fill_uniforms(vp);
	glBindVertexArray(item.vao);
	 glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, item.ibo);
	glDrawElements(type, item.idx_num, GL_UNSIGNED_INT, NULL);
	glBindVertexArray(0);
}