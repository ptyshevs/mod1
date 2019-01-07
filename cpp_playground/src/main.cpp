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
#include <HeightMap.hpp>
#include <Water.hpp>

int main(int ac, char *av[]) {
	ControlPoints controlPoints;

	if (!inputIsGood(ac, av))
		panic("File is not readable");
	try {
		controlPoints = readFile(av[1]);
		controlPoints.prepare(true);
		} catch (std::exception &e) {
		panic(e.what());
	}

	auto core = sdl_gl_init();
	auto map = generate_map(controlPoints);

	ParticleSystemData data;
	data.addParticle(glm::vec3(0, hf_sl / 2 - 1, 0));
	data.addParticle(glm::vec3(hf_sl - 1, 1, hf_sl - 1));
	data.addParticle(glm::vec3(0, 1, hf_sl - 1));
	auto water = instance_water(&map, &data);
	water.solver = new ParticleSystemSolver(data);
	auto camera = GLCamera();
	bool quit = false;
	glPointSize(10);
	glEnable (GL_BLEND);
	glEnable(GL_DEPTH_TEST);

	do
	{
		// Event handle
		SDL_PollEvent(&(core.event));
		process_input(camera, static_cast<GLItem &>(map), water, &quit);
		// Simulation step
		water.update_particles();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		camera.frameStart();
//		map.draw(camera.vp(), GL_TRIANGLES);
//		draw(points, camera.vp(), GL_POINTS);
		water.draw(camera.vp(), GL_POINTS);
		camera.frameEnd();

		SDL_GL_SwapWindow(core.win);
	} while (!(core.event.type == SDL_QUIT || quit));
	deinit(core);
}
