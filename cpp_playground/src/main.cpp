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
#include <sstream>
#include <iomanip>
#include "Emitter.hpp"
#include <parsing.hpp>

bool interop_enabled(bool verbose) {
	char data[2048];

	cl_platform_id platforms_available[10];
	cl_device_id device;
	unsigned int num_platforms = 0;
	cl_int err = clGetPlatformIDs(10, platforms_available, &num_platforms);
	if (verbose)
		std::cout << "# platforms:" << num_platforms << std::endl;
	err = clGetDeviceIDs(platforms_available[0], CL_DEVICE_TYPE_CPU, 1, &(device), NULL);
	if (verbose)
	{
		std::cout << "Device: " << device << std::endl;
		clGetDeviceInfo(device, CL_DRIVER_VERSION, sizeof(data), &data, NULL);
		printf("Version: %s\n", data);
		clGetDeviceInfo(device, CL_DEVICE_NAME, sizeof(data), &data, NULL);
		printf("Device name: %s\n", data);
	}
	clGetDeviceInfo(device, CL_DEVICE_EXTENSIONS, sizeof(data), &data, NULL);
	if (verbose)
		printf("Extensions: %s\n", data);
	std::stringstream ss(data);
	std::string item;
	while (ss >> item) {
		if (item == "cl_khr_gl_sharing")
			return (true);
	}
	return (false);
}

int main(int ac, char *av[]) {
	if (!interop_enabled(false))
	{
		std::cout << "GL/CL Interoperability doesn't work on this device" << std::endl;
		return (1);
	}
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
	std::cout << "Before instance water" << std::endl;
	auto water = instance_water(&map, &data, &emitter);
	std::cout << "After instance water" << std::endl;
	water.running = running;
	water.emitting = emitting;
	auto camera = GLCamera();
	bool quit = false;
	std::cout << "Here" << std::endl;
	do
	{
		// Event handle
		SDL_PollEvent(&(core.event));
		process_input(camera, static_cast<GLItem &>(map), water, &quit);
		std::cout << "Here" << std::endl;

		// Simulation step
		if (water.running)
			water.update_particles();

		std::cout << "Here" << std::endl;
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		camera.frameStart();
		map.draw(camera.vp(), GL_TRIANGLES);
		water.draw(camera.vp(), GL_POINTS);
		camera.frameEnd();

		if (offline)
			save_image(std::string("render"));
		SDL_GL_SwapWindow(core.win);
	} while (!(core.event.type == SDL_QUIT || quit));
	deinit(core);
}
