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
#include <key_bindings.hpp>
#include <ControlPoints.hpp>

int main(int ac, char *av[]) {
	ControlPoints controlPoints;

	if (!inputIsGood(ac, av))
		panic("File is not readable");
	try {
		controlPoints = readFile(av[1]);
		} catch (std::exception &e) {
		panic(e.what());
		}
	controlPoints.prepare(true);
	auto core = sdl_gl_init();

	std::vector<Cell> hmap;
	hmap.reserve(sizeof(Cell) * sl * sl * (sl / 4));
	auto map = generate_map(controlPoints._arr, hmap);

	hmap.shrink_to_fit();

	auto water = instance_water(hmap, false, false);

	auto camera = GLCamera();

	glPointSize(4);
	bool quit = false;
	do
	{
		// Event handle
		SDL_PollEvent(&(core.event));
		process_input(camera, map, water, &quit);
		// Simulation step
		water.update_particles();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		camera.frameStart();
		map.draw(camera.vp(), GL_TRIANGLES);
//		draw(points, camera.vp(), GL_POINTS);
		water.draw(camera.vp(), GL_POINTS);
		camera.frameEnd();

		SDL_GL_SwapWindow(core.win);
	} while (!(core.event.type == SDL_QUIT || quit));
	deinit(core);
}
