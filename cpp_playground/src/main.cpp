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
	bool running = true;
	bool emitting = true;
	std::string scene_path;

	ParticleSystemData data(MAX_PARTICLES);
	Emitter emitter(data);
	emitter.setScale(0.7);

	parse_arguments(ac, av, &controlPoints, running, emitting, emitter);

	auto core = sdl_gl_init();
	auto map = generate_map(controlPoints);
	data.hmap = &map;

	//	emitter.setPointType(P_STATIC);
//	emitter.sphere(glm::vec3(-50, 17, 70), 2, 5);
//	emitter.fromFile(glm::vec3(-50, 11, -70), "res/ply/bun_zipper.mod1");
//	emitter.fromFile(glm::vec3(35, 3, 88), "res/ply/bun_zipper_res3.mod1");
//	emitter.setViscosity(3);
//	emitter.setId(1);
//	emitter.fromFile(glm::vec3(-35, 3, 88), "res/ply/bun_zipper_res3.mod1");
//	emitter.wall(-70, -69.5f, 1, 10, 10, 15);
//	emitter.pillow(glm::vec3(-75, 0, 70), 1, 3, 30);
//	emitter.pillow(glm::vec3(-75, 0, 50), 1, 3, 30);
//	emitter.pillow(glm::vec3(0, 0, 85), 1, 3, 30);
//	emitter.pillow(glm::vec3(0, 0, 94), 1, 3, 30);
//	emitter.cube(glm::vec3(10, 40, 10), 10);
//	emitter.cuboid(-10, 20, 47, 49, -10, 20);
//	emitter.cube(glm::vec3(5, 40, 5), 5);
//	emitter.cube(glm::vec3(10, 40, 10), 5);
//	emitter.cube(glm::vec3(20, 40, 20), 5);
//	emitter.cube(glm::vec3(30, 40, 30), 5);
//	emitter.cube(glm::vec3(0, 1, hf_sl - 5), 3);
//	emitter.cuboid(-5, 5, 0, 20, hf_sl - 7, hf_sl - 5);
//	emitter.cube(0.5,5, 7, 35, 49, 24, 25);
	std::cout << "Num of particles=" << data.numOfParticles() << std::endl;
	// right near the camera
//	data.addParticle(glm::vec3(0,25, hf_sl - 2));
//	data.addParticle(glm::vec3(0.5,25, hf_sl - 2));
//	data.addParticle(glm::vec3(1,25, hf_sl - 2));

	auto water = instance_water(&map, &data, &emitter);
	water.running = running;
	water.emitting = emitting;
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
		if (water.running)
			water.update_particles();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		camera.frameStart();
		map.draw(camera.vp(), GL_TRIANGLES);
		water.draw(camera.vp(), GL_POINTS);
		camera.frameEnd();

		SDL_GL_SwapWindow(core.win);
	} while (!(core.event.type == SDL_QUIT || quit));
	deinit(core);
}
