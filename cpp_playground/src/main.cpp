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
#include "Emitter.hpp"

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
	data.hmap = &map;
	Emitter emitter(data);
	emitter.setStep(.93);
//	emitter.cube(glm::vec3(10, 40, 10), 10);
	emitter.cuboid(-80, 80, 45, 49, -80, 80);
//	emitter.cube(glm::vec3(5, 40, 5), 5);
//	emitter.cube(glm::vec3(10, 40, 10), 5);
//	emitter.cube(glm::vec3(20, 40, 20), 5);
//	emitter.cube(glm::vec3(30, 40, 30), 5);
//	emitter.cube(glm::vec3(0, 1, hf_sl - 5), 3);
//	emitter.cuboid(-5, 5, 0, 20, hf_sl - 7, hf_sl - 5);
//	emitter.cube(0.5,5, 7, 35, 49, 24, 25);
	std::cout << "Num of particles=" << data._particles.size() << std::endl;
	std::cout << "sizeof(Particle)=" << sizeof(Particle) << std::endl;
	// right near the camera
//	data.addParticle(glm::vec3(0,25, hf_sl - 2));
//	data.addParticle(glm::vec3(0.5,25, hf_sl - 2));
//	data.addParticle(glm::vec3(1,25, hf_sl - 2));

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
		map.draw(camera.vp(), GL_TRIANGLES);
//		draw(points, camera.vp(), GL_POINTS);
		water.draw(camera.vp(), GL_POINTS);
		camera.frameEnd();

		SDL_GL_SwapWindow(core.win);
	} while (!(core.event.type == SDL_QUIT || quit));
	deinit(core);
}
