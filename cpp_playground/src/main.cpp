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

void draw(GLItem &item, const glm::mat4 &vp, GLenum type);

Water	instance_water(std::vector<Cell> &hmap)
{
	Water w;

    auto cl = CLCore();
    cl_host_part(cl, true);
    cl_compile_kernel(cl, "src/kernels/wsim_kernel.cl", "wsim_kernel");


	w.hmap = hmap;
	w.idx_num = sl * sl * sl;
	w.model = glm::mat4(1.0f);
	w.shader_program = compile_shaders("src/shaders/water_vertex.glsl",
									   "src/shaders/water_fragment.glsl");
	w.fill_uniforms = [&](const glm::mat4 &vp) {
        auto mvp_id = glGetUniformLocation(w.shader_program, "MVP");
        auto mvp = vp * w.model;
        glUniformMatrix4fv(mvp_id, 1, GL_FALSE, glm::value_ptr(mvp));
    };

	glGenBuffers(1, &w.vbo);
	glGenBuffers(1, &w.vbo2);
	glGenVertexArrays(1, &w.vao);

	glBindVertexArray(w.vao);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, w.vbo);
	glBufferData(GL_ARRAY_BUFFER, w.hmap.size() * sizeof(Cell), w.hmap.data(), GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, sizeof(Cell), 0);
	glVertexAttribPointer(1, 1, GL_INT, GL_TRUE, sizeof(Cell), (void *)(sizeof(float) * 3));
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_TRUE, sizeof(Cell), (void *)(sizeof(float) * 3 + sizeof(int)));

	glBindBuffer(GL_ARRAY_BUFFER, w.vbo2);
	glBufferData(GL_ARRAY_BUFFER, w.hmap.size() * sizeof(Cell), w.hmap.data(), GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, sizeof(Cell), 0);
	glVertexAttribPointer(1, 1, GL_INT, GL_TRUE, sizeof(Cell), (void *)(sizeof(float) * 3));
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_TRUE, sizeof(Cell), (void *)(sizeof(float) * 3 + sizeof(int)));
	
	glBindVertexArray(0);

	int err = 0;

	w.cl_vbo = clCreateFromGLBuffer(cl.context, CL_MEM_READ_WRITE, w.vbo, &err);
	w.cl_vbo2 = clCreateFromGLBuffer(cl.context, CL_MEM_READ_WRITE, w.vbo2, &err);
	if (err != CL_SUCCESS) {
        std::cout << "Error: " << __LINE__ << " code: " << err << ".\n";
        exit(1);
    }
	w.cl = cl;

	return (w);
}

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
		water.model = glm::rotate(water.model, camera.speed * glm::radians(0.1f), glm::vec3(0.0f, 1.0f, 0.0f));
	}
	else if (keystate[SDL_SCANCODE_Q])
	{
		map.model = glm::rotate(map.model, camera.speed * glm::radians(-0.1f), glm::vec3(0.0f, 1.0f, 0.0f));
		points.model = glm::rotate(points.model, camera.speed * glm::radians(-0.1f), glm::vec3(0.0f, 1.0f, 0.0f));
		water.model = glm::rotate(water.model, camera.speed * glm::radians(-0.1f), glm::vec3(0.0f, 1.0f, 0.0f));
	}
	else if (keystate[SDL_SCANCODE_UP])
	{
		map.model = glm::rotate(map.model, camera.speed * glm::radians(0.1f), glm::vec3(1.0f, 0.0f, 0.0f));
		points.model = glm::rotate(points.model, camera.speed * glm::radians(0.1f), glm::vec3(1.0f, 0.0f, 0.0f));
		water.model = glm::rotate(water.model, camera.speed * glm::radians(0.1f), glm::vec3(1.0f, 0.0f, 0.0f));
	}
	else if (keystate[SDL_SCANCODE_DOWN])
	{
		map.model = glm::rotate(map.model, camera.speed * glm::radians(-0.1f), glm::vec3(1.0f, 0.0f, 0.0f));
		points.model = glm::rotate(points.model, camera.speed * glm::radians(-0.1f), glm::vec3(1.0f, 0.0f, 0.0f));
		water.model = glm::rotate(water.model, camera.speed * glm::radians(-0.1f), glm::vec3(1.0f, 0.0f, 0.0f));
	}
	else if (keystate[SDL_SCANCODE_LEFT])
	{
		map.model = glm::rotate(map.model, camera.speed * glm::radians(0.1f), glm::vec3(0.0f, 0.0f, 1.0f));
		points.model = glm::rotate(points.model, camera.speed * glm::radians(0.1f), glm::vec3(0.0f, 0.0f, 1.0f));
		water.model = glm::rotate(water.model, camera.speed * glm::radians(0.1f), glm::vec3(0.0f, 0.0f, 1.0f));
	}
	else if (keystate[SDL_SCANCODE_RIGHT])
	{
		map.model = glm::rotate(map.model, camera.speed * glm::radians(-0.1f), glm::vec3(0.0f, 0.0f, 1.0f));
		points.model = glm::rotate(points.model, camera.speed * glm::radians(-0.1f), glm::vec3(0.0f, 0.0f, 1.0f));
		water.model = glm::rotate(water.model, camera.speed * glm::radians(-0.1f), glm::vec3(0.0f, 0.0f, 1.0f));
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

	std::vector<Cell> hmap;
	hmap.reserve(sizeof(Cell) * sl * sl * sl);

	auto map = generate_map(controlPointsArray, hmap);

	hmap.shrink_to_fit();

	auto water = instance_water(hmap);

	auto points = generate_control_points(controlPointsArray);

	auto camera = GLCamera();

	// glPointSize(3);
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
		draw(water, camera.vp(), GL_POINTS);
		camera.frameEnd();

		SDL_GL_SwapWindow(core.win);
	}

	deinit(core);

}

void draw(GLItem &item, const glm::mat4 &vp, GLenum type)
{
	glUseProgram(item.shader_program);
	item.fill_uniforms(vp);
	glBindVertexArray(item.vao);
	if (type == GL_POINTS)
	{
		try {
			auto dbitem = dynamic_cast<CLGLDoubleBufferedItem &>(item);
			glBindBuffer(GL_ARRAY_BUFFER, dbitem.state ? dbitem.vbo : dbitem.vbo2);
		} catch (const std::bad_cast& e) { }
		glDrawArrays(GL_POINTS, 0, item.idx_num);
	} else
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, item.ibo);
		glDrawElements(type, item.idx_num, GL_UNSIGNED_INT, NULL);
	}
	glBindVertexArray(0);
}
