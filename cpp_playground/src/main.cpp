/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ptyshevs <ptyshevs@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/09 16:40:21 by vpopovyc          #+#    #+#             */
/*   Updated: 2018/12/24 21:11:32 by ptyshevs         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <core.hpp>
#include <camera.hpp>
#include "limits.h"

static const size_t sl = 100;
static const size_t hf_sl = sl / 2;

void draw(GLItem item, const glm::mat4 &vp, GLenum type);

void	process_input(GLCamera &camera, GLItem &map, GLItem &points, bool *quit)
{
	auto keystate = SDL_GetKeyboardState(NULL);

	if (keystate[SDL_SCANCODE_ESCAPE])
		*quit = true;
	else if (keystate[SDL_SCANCODE_W])
		camera.pos += camera.speed * camera.dir;
	else if (keystate[SDL_SCANCODE_S])
		camera.pos -= camera.speed * camera.dir;
	else if (keystate[SDL_SCANCODE_E])
	{
		map.model = glm::rotate(map.model, camera.speed * glm::radians(0.1f), glm::vec3(0.0f, 1.0f, 0.0f));
		points.model = glm::rotate(points.model, camera.speed * glm::radians(0.1f), glm::vec3(0.0f, 1.0f, 0.0f));
	}
	else if (keystate[SDL_SCANCODE_Q])
	{
		map.model = glm::rotate(map.model, camera.speed * glm::radians(-0.1f), glm::vec3(0.0f, 1.0f, 0.0f));
		points.model = glm::rotate(points.model, camera.speed * glm::radians(-0.1f), glm::vec3(0.0f, 1.0f, 0.0f));
	}
}

void rescale(std::vector<glm::vec3> &cpoints)
{
	float x_max, y_max, z_max;

	x_max = INT_MIN;
	y_max = INT_MIN;
	z_max = INT_MIN;

	for (glm::vec3 cpoint: cpoints)
	{
		if (cpoint.x > x_max)
			x_max = cpoint.x;
		if (cpoint.y > y_max)
			y_max = cpoint.y;
		if (cpoint.z > z_max)
			z_max = cpoint.z;
	}
	for (int i = 0; i < cpoints.size(); ++i)
	{
		auto &point = cpoints[i];
		point.x /= x_max;
		point.y /= y_max;
		point.z /= z_max;
	}
}

/*
 * Change x and z, leaving y unchanged
 */
void center(std::vector<glm::vec3> &cpoints)
{
	if (cpoints.size() == 1)
		return ;
	for (glm::vec3 &cp: cpoints)
	{
		cp.x += 0.5f;
		cp.z += 0.5f;
	}
}

void scale_back(std::vector<glm::vec3> &cpoints)
{
	if (cpoints.size() == 1)
		return ;
	for (glm::vec3 &cp: cpoints)
	{
		cp *= sl;
		cp -= hf_sl;
	}
}


void	show_points(std::vector<glm::vec3> &cpoints)
{
	std::cout << "Control points:" << std::endl;
	for (glm::vec3 cp: cpoints)
	{
		std::cout << glm::to_string(cp) << std::endl;
	}
}

void 	add_borders(std::vector<glm::vec3> &cpoints)
{
	for (int i = 0; i < sl; i++)
	{
		cpoints.emplace_back(i - (int)hf_sl, 0, 0);
		cpoints.emplace_back(0, i - (int)hf_sl, 0);
		cpoints.emplace_back((int)hf_sl, i - (int)hf_sl, 0);
		cpoints.emplace_back(i - (int)hf_sl, (int)hf_sl, 0);
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
//	normalize_cp(controlPointsArray);
// add borders

	show_points(controlPointsArray);

	rescale(controlPointsArray);
	center(controlPointsArray);
	scale_back(controlPointsArray);
	std::cout << "After" << std::endl;
	show_points(controlPointsArray);
	std::cout << "After borders" << std::endl;
	add_borders(controlPointsArray);
//	controlPointsArray.emplace_back(50, 50, 50);
//	controlPointsArray.emplace_back(50, 100, 50);
//	controlPointsArray.emplace_back(50, 150, 50);
//	add_borders(controlPointsArray);
	show_points(controlPointsArray);
	auto core = sdl_gl_init();

	auto map = generate_map(controlPointsArray);
	auto points = generate_control_points(controlPointsArray);

	auto camera = GLCamera();
	glPointSize(3);
	while(!quit)
	{
		// Event handle
		SDL_PollEvent(&(core.event));
		process_input(camera, map, points, &quit);
		if (core.event.type == SDL_QUIT)
			quit = true;

		// Actual render

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		camera.frameStart();
		draw(map, camera.vp(), GL_TRIANGLES);
		draw(points, camera.vp(), GL_POINTS);
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
//	glDrawArrays(GL_TRIANGLES, 0, item.idx_num);
//	glDrawArrays(type, 0, item.idx_num);
	 glDrawElements(type, item.idx_num, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}