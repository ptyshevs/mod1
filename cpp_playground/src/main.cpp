/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ptyshevs <ptyshevs@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/09 16:40:21 by vpopovyc          #+#    #+#             */
/*   Updated: 2019/09/24 22:22:12 by ptyshevs         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <core.hpp>
#include <camera.hpp>
#include <key_bindings.hpp>
#include <ControlPoints.hpp>
#include <HeightMap.hpp>
#include <Water.hpp>
#include <sstream>
#include <iomanip>
#include "Emitter.hpp"
#include <parsing.hpp>


int main(int ac, char *av[]) {
	ControlPoints controlPoints;
	bool running = true;
	bool emitting = true;
	bool offline = false;

	ParticleSystemData data(MAX_PARTICLES);
	Emitter emitter(data);
	emitter.setScale(0.7);

	parse_arguments(ac, av, &controlPoints, running, emitting, offline, emitter);

	auto core = sdl_gl_init(offline);
	auto map = generate_map(controlPoints);
	data.hmap = &map;
	std::cout << "Initial # of particles = " << data.numOfParticles() << std::endl;

	auto water = instance_water(&map, &data, &emitter);
	water.running = running;
	water.emitting = emitting;
	auto camera = GLCamera();
	bool quit = false;
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
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
		glEnable(GL_BLEND);
		// glDepthMask(0);
		water.draw(camera.vp(), GL_POINTS);
		glDisable(GL_BLEND);
		camera.frameEnd();

		if (offline)
			save_image(std::string("render"));
		SDL_GL_SwapWindow(core.win);
	} while (!(core.event.type == SDL_QUIT || quit));
	deinit(core);
}
