/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Water.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ptyshevs <ptyshevs@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/25 21:14:47 by ptyshevs          #+#    #+#             */
/*   Updated: 2018/12/25 21:14:48 by ptyshevs         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "core.hpp"
#include <Water.hpp>

Water	instance_water(HeightMap *hmap, ParticleSystemData *data, Emitter *emitter)
{
	Water w;

	w.hmap = hmap;
	w.data = data;
	w.emitter = emitter;
	w.constants.hmap_size = hmap->hmap.size();
	w.constants.n_control_points = hmap->_cpoints._arr.size();
	w.constants.n_particles = w.data->n_particles;
	w.cl = CLWaterCore();
	cl_host_part(w.cl, true);
	cl_compile_kernel(w.cl, "src/kernels/clear_caching.cl", "clear_caching");
	cl_compile_water_kernel(w.cl, w.cl.neighborCachingProgram, w.cl.neighborCaching, "src/kernels/neighbor_caching.cl", "neighbor_caching");
	cl_compile_water_kernel(w.cl, w.cl.findNeighborsProgram, w.cl.findNeighbors, "src/kernels/find_neighbors.cl", "find_neighbors");
	cl_compile_water_kernel(w.cl, w.cl.simUpdateProgram, w.cl.simUpdate, "src/kernels/sim_update.cl", "sim_update");
	cl_compile_water_kernel(w.cl, w.cl.accumForcesProgram, w.cl.accumForces, "src/kernels/accum_forces.cl", "accum_forces");
	cl_compile_water_kernel(w.cl, w.cl.integrateResolveProgram, w.cl.integrateResolve, "src/kernels/integrate_resolve.cl", "integrate_resolve");

	w.model = glm::mat4(1.0f);
	w.shader_program = compile_shaders("src/shaders/water_vertex.glsl",
										"src/shaders/water_fragment.glsl");
	w.fill_uniforms = [&](const glm::mat4 &vp) {
		auto mvp_id = glGetUniformLocation(w.shader_program, "MVP");
		auto mvp = vp * w.model;
		glUniformMatrix4fv(mvp_id, 1, GL_FALSE, glm::value_ptr(mvp));
	};

	glGenBuffers(1, &w.vbo);
	glGenVertexArrays(1, &w.vao);

	glBindVertexArray(w.vao);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);
	glEnableVertexAttribArray(5);
	glEnableVertexAttribArray(6);
	glEnableVertexAttribArray(7);
	glEnableVertexAttribArray(8);

	glBindBuffer(GL_ARRAY_BUFFER, w.vbo);
	glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES * sizeof(Particle), w.data->_particles.data(), GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_TRUE, sizeof(Particle), 0); // position
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_TRUE, sizeof(Particle), (void *)(sizeof(float) * 4)); // velocity
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_TRUE, sizeof(Particle), (void *)(sizeof(float) * 8)); // force
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_TRUE, sizeof(Particle), (void *)(sizeof(float) * 12)); // density
	glVertexAttribPointer(4, 1, GL_FLOAT, GL_TRUE, sizeof(Particle), (void *)(sizeof(float) * 13)); // pressure
	glVertexAttribPointer(5, 1, GL_FLOAT, GL_TRUE, sizeof(Particle), (void *)(sizeof(float) * 14)); // viscosity
	glVertexAttribPointer(6, 1, GL_UNSIGNED_INT, GL_TRUE, sizeof(Particle), (void *)(sizeof(float) * 15)); // particle type
	glVertexAttribPointer(7, 1, GL_UNSIGNED_INT, GL_TRUE, sizeof(Particle), (void *)(sizeof(float) * 16)); // particle id
	glVertexAttribPointer(8, 1, GL_UNSIGNED_INT, GL_TRUE, sizeof(Particle), (void *)(sizeof(float) * 17)); // n_neighbors
	glBindVertexArray(0);
	w.idx_num = MAX_PARTICLES;
	int err = 0;

	w.cl_constants = clCreateBuffer(w.cl.context, CL_MEM_READ_WRITE, sizeof(WaterConstants), NULL, &err);
	w.cl_cp = clCreateBuffer(w.cl.context, CL_MEM_READ_ONLY, hmap->_cpoints._arr.size() * sizeof(glm::vec3), NULL, &err);
	w.cl_hmap = clCreateBuffer(w.cl.context, CL_MEM_READ_WRITE, hmap->hmap.size() * sizeof(Cell), NULL, &err);
	w.cl_vbo = clCreateFromGLBuffer(w.cl.context, CL_MEM_READ_WRITE, w.vbo, &err);


	err = 0;
	err = clEnqueueWriteBuffer(w.cl.queue, w.cl_cp, CL_TRUE, 0, hmap->_cpoints._arr.size() * sizeof(glm::vec3), hmap->_cpoints._arr.data(), 0, NULL, NULL);
	err |= clEnqueueWriteBuffer(w.cl.queue, w.cl_hmap, CL_TRUE, 0, hmap->hmap.size() * sizeof(Cell), hmap->hmap.data(), 0, NULL, NULL);
	err |= clEnqueueWriteBuffer(w.cl.queue, w.cl_constants, CL_TRUE, 0, sizeof(WaterConstants), &w.constants, 0, NULL, NULL);
	w.no_err(err, __LINE__);

	err = 0;
	err |= clSetKernelArg(w.cl.kernel, 0, sizeof(w.cl_hmap), &w.cl_hmap);
	w.no_err(err, __LINE__);

	err = clSetKernelArg(w.cl.neighborCaching, 0, sizeof(w.cl_hmap), &w.cl_hmap);
	err |= clSetKernelArg(w.cl.neighborCaching, 1, sizeof(w.cl_vbo), &w.cl_vbo);
	err |= clSetKernelArg(w.cl.neighborCaching, 2, sizeof(w.cl_constants), &w.cl_constants);
	w.no_err(err, __LINE__);

	err = clSetKernelArg(w.cl.findNeighbors, 0, sizeof(w.cl_hmap), &w.cl_hmap);
	err |= clSetKernelArg(w.cl.findNeighbors, 1, sizeof(w.cl_vbo), &w.cl_vbo);
	err |= clSetKernelArg(w.cl.findNeighbors, 2, sizeof(w.cl_constants), &w.cl_constants);
	w.no_err(err, __LINE__);

	err = clSetKernelArg(w.cl.simUpdate, 0, sizeof(w.cl_hmap), &w.cl_hmap);
	err |= clSetKernelArg(w.cl.simUpdate, 1, sizeof(w.cl_vbo), &w.cl_vbo);
	err |= clSetKernelArg(w.cl.simUpdate, 2, sizeof(w.cl_constants), &w.cl_constants);
	w.no_err(err, __LINE__);

	err = clSetKernelArg(w.cl.accumForces, 0, sizeof(w.cl_vbo), &w.cl_vbo);
	err |= clSetKernelArg(w.cl.accumForces, 1, sizeof(w.cl_constants), &w.cl_constants);
	w.no_err(err, __LINE__);

	err = clSetKernelArg(w.cl.integrateResolve, 0, sizeof(w.cl_constants), &w.cl_constants);
	err = clSetKernelArg(w.cl.integrateResolve, 1, sizeof(w.cl_cp), &w.cl_cp);
	err |= clSetKernelArg(w.cl.integrateResolve, 2, sizeof(w.cl_vbo), &w.cl_vbo);
	w.no_err(err, __LINE__);

	w.emitter->prepare_emit(w.cl);
	w.emitter->cl_vbo = w.cl_vbo;
	w.emitter->cl_constants = w.cl_constants;
	return (w);
}

Water::Water() : hmap(nullptr), solver(nullptr)
{
}

Water::~Water()
{
	clReleaseMemObject(cl_hmap);
	clReleaseMemObject(cl_cp);
	clReleaseProgram(cl.program);
	clReleaseKernel(cl.kernel);
	clReleaseKernel(cl.accumForces);
	clReleaseKernel(cl.integrateResolve);
	clReleaseKernel(cl.simUpdate);
	clReleaseKernel(cl.neighborCaching);
	clReleaseKernel(cl.findNeighbors);
	clReleaseCommandQueue(cl.queue);
	clReleaseDevice(cl.device);
	clReleaseContext(cl.context);
}

void 	Water::emit() {
	static uint last_emit_time = SDL_GetTicks();

	if (SDL_TICKS_PASSED(SDL_GetTicks(), last_emit_time + emitter->pps) || emitter->type == EMITER_FLUSH) {
		emitter->emit();
		last_emit_time = SDL_GetTicks();
		if (emitter->type == EMITER_FLUSH)
			emitter->type = emitter->prev_type;
	}
}

void Water::update_particles()
{
	static size_t global_neighbor_caching_jobs = data->hmap->hmap.size();
	static size_t global_work_size = MAX_PARTICLES;
	static size_t n_iter = 0;
	int err;
	glFinish();

	clEnqueueAcquireGLObjects(cl.queue, 1, &cl_vbo, 0, NULL, NULL);

	// Emit some water
	if (emitting)
		emit();
	if (n_iter % UPDATE_NEIGHBORS_EVERY_N_ITER == 0) {  // update neighbors
		err = clEnqueueNDRangeKernel(cl.queue, cl.kernel, 1, NULL, &global_neighbor_caching_jobs, NULL, 0, NULL, NULL);
		err |= clEnqueueNDRangeKernel(cl.queue, cl.neighborCaching, 1, NULL, &global_work_size, NULL, 0, NULL, NULL);
		err |= clEnqueueNDRangeKernel(cl.queue, cl.findNeighbors, 1, NULL, &global_work_size, NULL, 0, NULL, NULL);
	}
	else
		err = 0;
	no_err(err, __LINE__);
	err |= clEnqueueNDRangeKernel(cl.queue, cl.simUpdate, 1, NULL, &global_work_size, NULL, 0, NULL, NULL);
	err |= clEnqueueNDRangeKernel(cl.queue, cl.accumForces, 1, NULL, &global_work_size, NULL, 0, NULL, NULL);
	err |= clEnqueueNDRangeKernel(cl.queue, cl.integrateResolve, 1, NULL, &global_work_size, NULL, 0, NULL, NULL);
	no_err(err, __LINE__);
	clEnqueueReleaseGLObjects(cl.queue, 1, &cl_vbo, 0, NULL, NULL);

	clFinish(cl.queue);
	++n_iter;
}
