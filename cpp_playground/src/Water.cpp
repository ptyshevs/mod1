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

Water	instance_water(HeightMap *hmap, ParticleSystemData *data)
{
	Water w;

	w.hmap = hmap;
	w.data = data;
	w.state = true;

	w.constants.hmap_size = hmap->hmap.size();
	w.constants.n_control_points = hmap->_cpoints._arr.size();
	w.constants.n_particles = w.data->numOfParticles();
	w.constants.n_non_empty_cells = 0;
	w.cl = CLWaterCore();
	cl_host_part(w.cl, true);
	cl_compile_kernel(w.cl, "src/kernels/clear_caching.cl", "clear_caching");
	cl_compile_water_kernel(w.cl, w.cl.neighborCachingProgram, w.cl.neighborCaching, "src/kernels/neighbor_caching.cl", "neighbor_caching");
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
	glGenBuffers(1, &w.vbo2);
	glGenVertexArrays(1, &w.vao);
	glGenVertexArrays(1, &w.vao2);

	glBindVertexArray(w.vao);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);
	glEnableVertexAttribArray(5);
//	glEnableVertexAttribArray(6);

	glBindBuffer(GL_ARRAY_BUFFER, w.vbo);
	glBufferData(GL_ARRAY_BUFFER, w.data->_particles.size()* sizeof(Particle), w.data->_particles.data(), GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, sizeof(Particle), 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, sizeof(Particle), (void *)(sizeof(float) * 3));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_TRUE, sizeof(Particle), (void *)(sizeof(float) * 6));
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_TRUE, sizeof(Particle), (void *)(sizeof(float) * 9));
	glVertexAttribPointer(4, 1, GL_FLOAT, GL_TRUE, sizeof(Particle), (void *)(sizeof(float) * 10));
	glVertexAttribPointer(5, 1, GL_UNSIGNED_INT, GL_TRUE, sizeof(Particle), (void *)(sizeof(float) * 11));

	glBindVertexArray(w.vao2);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);
	glEnableVertexAttribArray(5);
	glBindBuffer(GL_ARRAY_BUFFER, w.vbo2);
	glBufferData(GL_ARRAY_BUFFER, w.data->_particles.size()* sizeof(Particle), w.data->_particles.data(), GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, sizeof(Particle), 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, sizeof(Particle), (void *)(sizeof(float) * 3));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_TRUE, sizeof(Particle), (void *)(sizeof(float) * 6));
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_TRUE, sizeof(Particle), (void *)(sizeof(float) * 9));
	glVertexAttribPointer(4, 1, GL_FLOAT, GL_TRUE, sizeof(Particle), (void *)(sizeof(float) * 10));
	glVertexAttribPointer(5, 1, GL_UNSIGNED_INT, GL_TRUE, sizeof(Particle), (void *)(sizeof(float) * 11));
	glBindVertexArray(0);
	w.idx_num = w.data->numOfParticles();
	int err = 0;

	w.cl_constants = clCreateBuffer(w.cl.context, CL_MEM_READ_WRITE, sizeof(WaterConstants), NULL, &err);
	w.cl_cp = clCreateBuffer(w.cl.context, CL_MEM_READ_ONLY, hmap->_cpoints._arr.size() * sizeof(glm::vec3), NULL, &err);
	w.cl_hmap = clCreateBuffer(w.cl.context, CL_MEM_READ_WRITE, hmap->hmap.size() * sizeof(Cell), NULL, &err);
	w.cl_vbo = clCreateFromGLBuffer(w.cl.context, CL_MEM_READ_WRITE, w.vbo, &err);
	w.cl_vbo2 = clCreateFromGLBuffer(w.cl.context, CL_MEM_READ_WRITE, w.vbo2, &err);


	err = 0;
	err = clEnqueueWriteBuffer(w.cl.queue, w.cl_cp, CL_TRUE, 0, hmap->_cpoints._arr.size() * sizeof(glm::vec3), hmap->_cpoints._arr.data(), 0, NULL, NULL);
	err |= clEnqueueWriteBuffer(w.cl.queue, w.cl_hmap, CL_TRUE, 0, hmap->hmap.size() * sizeof(Cell), hmap->hmap.data(), 0, NULL, NULL);
	err |= clEnqueueWriteBuffer(w.cl.queue, w.cl_constants, CL_TRUE, 0, sizeof(WaterConstants), &w.constants, 0, NULL, NULL);
	w.no_err(err, __LINE__);

	err = 0;
	err = clSetKernelArg(w.cl.kernel, 0, sizeof(w.cl_constants), &w.cl_constants);
	err |= clSetKernelArg(w.cl.kernel, 1, sizeof(w.cl_cp), &w.cl_cp);
	err |= clSetKernelArg(w.cl.kernel, 2, sizeof(w.cl_hmap), &w.cl_hmap);
	w.no_err(err, __LINE__);

	err = clSetKernelArg(w.cl.neighborCaching, 0, sizeof(w.cl_constants), &w.cl_constants);
	err |= clSetKernelArg(w.cl.neighborCaching, 1, sizeof(w.cl_cp), &w.cl_cp);
	err |= clSetKernelArg(w.cl.neighborCaching, 2, sizeof(w.cl_hmap), &w.cl_hmap);

	w.no_err(err, __LINE__);
	err = clSetKernelArg(w.cl.simUpdate, 0, sizeof(w.cl_constants), &w.cl_constants);
	err |= clSetKernelArg(w.cl.simUpdate, 1, sizeof(w.cl_cp), &w.cl_cp);
	err |= clSetKernelArg(w.cl.simUpdate, 2, sizeof(w.cl_hmap), &w.cl_hmap);
	w.no_err(err, __LINE__);

	err = clSetKernelArg(w.cl.accumForces, 0, sizeof(w.cl_constants), &w.cl_constants);
	err |= clSetKernelArg(w.cl.accumForces, 1, sizeof(w.cl_cp), &w.cl_cp);
	err |= clSetKernelArg(w.cl.accumForces, 2, sizeof(w.cl_hmap), &w.cl_hmap);
	w.no_err(err, __LINE__);

	err = clSetKernelArg(w.cl.integrateResolve, 0, sizeof(w.cl_constants), &w.cl_constants);
	err = clSetKernelArg(w.cl.integrateResolve, 1, sizeof(w.cl_cp), &w.cl_cp);
	err |= clSetKernelArg(w.cl.integrateResolve, 2, sizeof(w.cl_hmap), &w.cl_hmap);
	w.no_err(err, __LINE__);

	return (w);
}

Water::Water() : hmap(nullptr), solver(nullptr)
{
}

Water::~Water()
{
	clReleaseMemObject(cl_hmap);
	clReleaseMemObject(cl_cp);
	clReleaseMemObject(cl_vbo);
	clReleaseMemObject(cl_vbo2);
	clReleaseMemObject(cl_constants);
	clReleaseProgram(cl.program);
	clReleaseKernel(cl.kernel);
	clReleaseKernel(cl.accumForces);
	clReleaseKernel(cl.integrateResolve);
	clReleaseKernel(cl.simUpdate);
	clReleaseKernel(cl.neighborCaching);
	clReleaseCommandQueue(cl.queue);
	clReleaseDevice(cl.device);
	clReleaseContext(cl.context);
}


void Water::update_particles()
{
	static size_t global_neighbor_caching_jobs = data->hmap->hmap.size();
	static size_t global_work_size = data->numOfParticles() ;
	static size_t n_iter = 0;
	int err = 0;
//	solver->simulation_step();
//	_updateBuffer();
///////////////////////////////////////////////////////////////////////////////
	glFinish();

	if (state)
	{
		clEnqueueAcquireGLObjects(cl.queue, 1, &cl_vbo, 0, NULL, NULL);
		err = clSetKernelArg(cl.kernel, 3, sizeof(cl_vbo), &cl_vbo);
		err |= clSetKernelArg(cl.neighborCaching, 3, sizeof(cl_vbo), &cl_vbo);
		err |= clSetKernelArg(cl.simUpdate, 3, sizeof(cl_vbo), &cl_vbo);
		err |= clSetKernelArg(cl.accumForces, 3, sizeof(cl_vbo), &cl_vbo);
		err |= clSetKernelArg(cl.integrateResolve, 3, sizeof(cl_vbo), &cl_vbo);
	}
	else
	{
		clEnqueueAcquireGLObjects(cl.queue, 1, &cl_vbo2, 0, NULL, NULL);
		err = clSetKernelArg(cl.kernel, 3, sizeof(cl_vbo2), &cl_vbo2);
		err |= clSetKernelArg(cl.neighborCaching, 3, sizeof(cl_vbo2), &cl_vbo2);
		err |= clSetKernelArg(cl.simUpdate, 3, sizeof(cl_vbo2), &cl_vbo2);
		err |= clSetKernelArg(cl.accumForces, 3, sizeof(cl_vbo2), &cl_vbo2);
		err |= clSetKernelArg(cl.integrateResolve, 3, sizeof(cl_vbo2), &cl_vbo2);
	}
	no_err(err, __LINE__);
	// Emit some water
//	emit();
err = clEnqueueNDRangeKernel(cl.queue, cl.kernel, 1, NULL, &global_work_size, NULL, 0, NULL, NULL);
err |= clEnqueueNDRangeKernel(cl.queue, cl.neighborCaching, 1, NULL, &global_work_size, NULL, 0, NULL, NULL);
	err |= clEnqueueNDRangeKernel(cl.queue, cl.simUpdate, 1, NULL, &global_work_size, NULL, 0, NULL, NULL);
	err |= clEnqueueNDRangeKernel(cl.queue, cl.accumForces, 1, NULL, &global_work_size, NULL, 0, NULL, NULL);
	err |= clEnqueueNDRangeKernel(cl.queue, cl.integrateResolve, 1, NULL, &global_work_size, NULL, 0, NULL, NULL);
	no_err(err, __LINE__);
	if (state)
		clEnqueueReleaseGLObjects(cl.queue, 1, &cl_vbo, 0, NULL, NULL);
	else
		clEnqueueReleaseGLObjects(cl.queue, 1, &cl_vbo2, 0, NULL, NULL);

    clFinish(cl.queue);
    ++n_iter;
}

void Water::_updateBuffer()
{
//	static bool first_time = true;
//	glBindVertexArray(vao);
//	glBindBuffer(GL_ARRAY_BUFFER, vbo);
//	glBufferData(GL_ARRAY_BUFFER, data->numOfParticles() * sizeof(Particle), data->_particles.data(), GL_DYNAMIC_DRAW);
//	glBindVertexArray(0);
//	idx_num = data->numOfParticles();
}

void Water::draw(const glm::mat4 &vp, GLenum type)
{
	glUseProgram(this->shader_program);
	this->fill_uniforms(vp);
	if (state)
		glBindVertexArray(this->vao);
	else
		glBindVertexArray(this->vao2);

	if (this->tex) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, this->tex);
	}

	if (this->tex_n) {
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, this->tex_n);
	}

	if (this->tex_ao) {
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, this->tex_ao);
	}

	if (this->tex_r) {
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, this->tex_r);
	}

	if (type == GL_POINTS)
		glDrawArrays(GL_POINTS, 0, this->idx_num); // those are only particles
	else
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ibo);
		glDrawElements(type, this->idx_num, GL_UNSIGNED_INT, NULL);
	}
	glBindVertexArray(0);
}