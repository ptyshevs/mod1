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


void	process_input(GLCamera &camera, GLItem &map, GLItem &points, Water &water, bool *quit)
{
	auto keystate = SDL_GetKeyboardState(NULL);

	if (keystate[SDL_SCANCODE_ESCAPE])
		*quit = true;
	if (keystate[SDL_SCANCODE_W])
		camera.pos += 0.1 * camera.speed * camera.dir;
	if (keystate[SDL_SCANCODE_S])
		camera.pos -= 0.1 * camera.speed * camera.dir;
	if (keystate[SDL_SCANCODE_E])
	{
		map.model = glm::rotate(map.model, camera.speed * glm::radians(0.1f), glm::vec3(0.0f, 1.0f, 0.0f));
		points.model = glm::rotate(points.model, camera.speed * glm::radians(0.1f), glm::vec3(0.0f, 1.0f, 0.0f));
		water.model = glm::rotate(water.model, camera.speed * glm::radians(0.1f), glm::vec3(0.0f, 1.0f, 0.0f));
	}
	if (keystate[SDL_SCANCODE_Q])
	{
		map.model = glm::rotate(map.model, camera.speed * glm::radians(-0.1f), glm::vec3(0.0f, 1.0f, 0.0f));
		points.model = glm::rotate(points.model, camera.speed * glm::radians(-0.1f), glm::vec3(0.0f, 1.0f, 0.0f));
		water.model = glm::rotate(water.model, camera.speed * glm::radians(-0.1f), glm::vec3(0.0f, 1.0f, 0.0f));
	}
	if (keystate[SDL_SCANCODE_UP])
	{
		map.model = glm::rotate(map.model, camera.speed * glm::radians(0.1f), glm::vec3(1.0f, 0.0f, 0.0f));
		points.model = glm::rotate(points.model, camera.speed * glm::radians(0.1f), glm::vec3(1.0f, 0.0f, 0.0f));
		water.model = glm::rotate(water.model, camera.speed * glm::radians(0.1f), glm::vec3(1.0f, 0.0f, 0.0f));
	}
	if (keystate[SDL_SCANCODE_DOWN])
	{
		map.model = glm::rotate(map.model, camera.speed * glm::radians(-0.1f), glm::vec3(1.0f, 0.0f, 0.0f));
		points.model = glm::rotate(points.model, camera.speed * glm::radians(-0.1f), glm::vec3(1.0f, 0.0f, 0.0f));
		water.model = glm::rotate(water.model, camera.speed * glm::radians(-0.1f), glm::vec3(1.0f, 0.0f, 0.0f));
	}
	if (keystate[SDL_SCANCODE_LEFT])
	{
		map.model = glm::rotate(map.model, camera.speed * glm::radians(0.1f), glm::vec3(0.0f, 0.0f, 1.0f));
		points.model = glm::rotate(points.model, camera.speed * glm::radians(0.1f), glm::vec3(0.0f, 0.0f, 1.0f));
		water.model = glm::rotate(water.model, camera.speed * glm::radians(0.1f), glm::vec3(0.0f, 0.0f, 1.0f));
	}
	if (keystate[SDL_SCANCODE_RIGHT])
	{
		map.model = glm::rotate(map.model, camera.speed * glm::radians(-0.1f), glm::vec3(0.0f, 0.0f, 1.0f));
		points.model = glm::rotate(points.model, camera.speed * glm::radians(-0.1f), glm::vec3(0.0f, 0.0f, 1.0f));
		water.model = glm::rotate(water.model, camera.speed * glm::radians(-0.1f), glm::vec3(0.0f, 0.0f, 1.0f));
	}
	if (keystate[SDL_SCANCODE_1])
	{
		water.emiter.type = EMITER_RAIN;
		water.emiter.pps = 1000;
	}
	if (keystate[SDL_SCANCODE_2])
	{
		water.emiter.type = EMITER_WAVE;
		water.emiter.pps = 1000;
	}
	if (keystate[SDL_SCANCODE_3])
	{
		water.emiter.type = EMITER_UNDERGROUND;
		water.emiter.pps = 1000;
	}
	if (keystate[SDL_SCANCODE_KP_MINUS])
		water.emiter.pps = abs(water.emiter.pps + 100);
	if (keystate[SDL_SCANCODE_KP_PLUS])
		water.emiter.pps = abs(water.emiter.pps - 100);
}

int		to1D(int x, int y, int z)
{
	x = x + hf_sl;
	z = z + hf_sl;
	int v = (sl * (sl / 4) * x) + sl * y + z;
	return (v);
}

glm::vec3		to3D(int n)
{
	int x = n / (sl * (sl / 4)) - hf_sl;
	n -= (x + hf_sl) * (sl * (sl / 4));
	int y = n / (sl);
	int z = n % (sl) - hf_sl;
	glm::vec3 f(x, y, z);
	return (f);
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
	hmap.reserve(sizeof(Cell) * sl * sl * (sl / 4));
	auto map = generate_map(controlPointsArray, hmap);

	hmap.shrink_to_fit();

	auto water = instance_water(hmap);

	auto points = generate_control_points(controlPointsArray);

	auto camera = GLCamera();

	 glPointSize(4);
//	glPointSize(3);
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
//		draw(points, camera.vp(), GL_POINTS);
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

	if (item.tex) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, item.tex);
	}

	if (item.tex_n) {
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, item.tex_n);
	}

	if (item.tex_ao) {
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, item.tex_ao);
	}

	if (item.tex_r) {
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, item.tex_r);
	}

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
