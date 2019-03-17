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
	w.cl = CLCore();
	cl_host_part(w.cl, true);
	cl_compile_kernel(w.cl, "src/kernels/wsim_kernel.cl", "wsim_kernel");

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
//	glEnableVertexAttribArray(3);
//	glEnableVertexAttribArray(4);
//	glEnableVertexAttribArray(5);
//	glEnableVertexAttribArray(6);

	glBindBuffer(GL_ARRAY_BUFFER, w.vbo);
	glBufferData(GL_ARRAY_BUFFER, w.data->_particles.size()* sizeof(Particle), w.data->_particles.data(), GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, sizeof(Particle), 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, sizeof(Particle), (void *)(sizeof(float) * 3));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_TRUE, sizeof(Particle), (void *)(sizeof(float) * 6));
//	glVertexAttribPointer(3, 1, GL_FLOAT)
	glBindVertexArray(0);
	w.idx_num = w.data->numOfParticles();
	int err = 0;

	w.cl_cp = clCreateBuffer(w.cl.context, CL_MEM_READ_ONLY, hmap->_cpoints._arr.size() * sizeof(glm::vec3), NULL, &err);
	w.cl_hmap = clCreateBuffer(w.cl.context, CL_MEM_READ_WRITE, hmap->hmap.size() * sizeof(Cell), NULL, &err);
	w.cl_vbo = clCreateFromGLBuffer(w.cl.context, CL_MEM_READ_WRITE, w.vbo, &err);

	err = 0;
	err = clEnqueueWriteBuffer(w.cl.queue, w.cl_cp, CL_TRUE, 0, hmap->_cpoints._arr.size() * sizeof(glm::vec3), hmap->_cpoints._arr.data(), 0, NULL, NULL);
	err |= clEnqueueWriteBuffer(w.cl.queue, w.cl_hmap, CL_TRUE, 0, hmap->hmap.size() * sizeof(Cell), hmap->hmap.data(), 0, NULL, NULL);
	if (err != CL_SUCCESS) {
		std::cout << "Error: " << __LINE__ << "code: " << err << ".\n";
		exit(1);
	}
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
	clReleaseCommandQueue(cl.queue);
	clReleaseDevice(cl.device);
	clReleaseContext(cl.context);
}


void Water::update_particles()
{
//	solver->simulation_step();
//	_updateBuffer();
///////////////////////////////////////////////////////////////////////////////
	glFinish();

	clEnqueueAcquireGLObjects(cl.queue, 1, &cl_vbo, 0, NULL, NULL);

	// Emit some water
//	emit();

	int err = 0;
	err = clSetKernelArg(cl.kernel, 0, sizeof(cl_cp), &cl_cp);
	err |= clSetKernelArg(cl.kernel, 1, sizeof(cl_hmap), &cl_hmap);
	err |= clSetKernelArg(cl.kernel, 2, sizeof(cl_vbo), &cl_vbo);
	if (err != CL_SUCCESS) {
		std::cout << "Error: " << __LINE__ << "code: " << err << ".\n";
		exit(1);
	}
	size_t global_work_size = data->numOfParticles() ;
    err = clEnqueueNDRangeKernel(cl.queue, cl.kernel, 1, NULL, &global_work_size, NULL, 0, NULL, NULL);
    if (err != CL_SUCCESS) {
        std::cout << "Error: " << __LINE__ << "code: " << err << ".\n";
        exit(1);
    }

	clEnqueueReleaseGLObjects(cl.queue, 1, &cl_vbo, 0, NULL, NULL);

    clFinish(cl.queue);
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