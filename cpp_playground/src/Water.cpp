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

Water	instance_water(std::vector<Cell> &hmap, bool snow, bool explode)
{
	Water w;

    auto cl = CLCore();
    cl_host_part(cl, true);
    if (snow)
    	cl_compile_kernel(cl, "src/kernels/wsim_snow.cl", "wsim_kernel");
    else if (explode)
    	cl_compile_kernel(cl, "src/kernels/wsim_explode.cl", "wsim_kernel");
    else
    	cl_compile_kernel(cl, "src/kernels/wsim_kernel.cl", "wsim_kernel");

	w.hmap = hmap;
	w.idx_num = sl * sl * sl / 4;
	w.model = glm::mat4(1.0f);
	if (snow)
		w.shader_program = compile_shaders("src/shaders/water_vertex.glsl",
											"src/shaders/snow_fragment.glsl");
	else if (explode)
		w.shader_program = compile_shaders("src/shaders/water_vertex.glsl",
											"src/shaders/explode_fragment.glsl");
	else
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

	glBindVertexArray(w.vao);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, w.vbo);
	glBufferData(GL_ARRAY_BUFFER, w.hmap.size() * sizeof(Cell), w.hmap.data(), GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, sizeof(Cell), 0);
	glVertexAttribPointer(1, 1, GL_INT, GL_TRUE, sizeof(Cell), (void *)(sizeof(float) * 3));
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_TRUE, sizeof(Cell), (void *)(sizeof(float) * 3 + sizeof(int)));

	glBindBuffer(GL_ARRAY_BUFFER, w.vbo2);
	glBufferData(GL_ARRAY_BUFFER, w.hmap.size() * sizeof(Cell), w.hmap.data(), GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, sizeof(Cell), 0);
	glVertexAttribPointer(1, 1, GL_INT, GL_TRUE, sizeof(Cell), (void *)(sizeof(float) * 3));
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_TRUE, sizeof(Cell), (void *)(sizeof(float) * 3 + sizeof(int)));

	glBindVertexArray(0);

	int err = 0;

	w.cl_vbo = clCreateFromGLBuffer(cl.context, CL_MEM_READ_WRITE, w.vbo, &err);
	w.cl_vbo2 = clCreateFromGLBuffer(cl.context, CL_MEM_READ_WRITE, w.vbo2, &err);
	if (err != CL_SUCCESS) {
        std::cout << "Error: " << __LINE__ << " code: " << err << ".\n";
        exit(1);
    }
	w.cl = cl;

	auto emiter = Emiter();
	emiter.prepare_emit(cl);

	w.emiter = emiter;

	return (w);
}

void 	HeightMap::emit(void) {
	static uint last_emit_time = SDL_GetTicks();

	if (SDL_TICKS_PASSED(SDL_GetTicks(), last_emit_time + emiter.pps)) {
		emiter.emit(state ? cl_vbo : cl_vbo2);
		last_emit_time = SDL_GetTicks();
	}
}

static const size_t  global_work_size = (sl * sl);

void	HeightMap::simulation_step()
{
	// To be 1000% sure that GL is finished it's work
	glFinish();
	// Swap buffers
	state = !state;

	clEnqueueAcquireGLObjects(cl.queue, 1, &cl_vbo, 0, NULL, NULL);
	clEnqueueAcquireGLObjects(cl.queue, 1, &cl_vbo2, 0, NULL, NULL);

	// Emit some water
	emit();

	int err = 0;
	err = clSetKernelArg(cl.kernel, state ? 0 : 1, sizeof(cl_vbo), &cl_vbo);
	err |= clSetKernelArg(cl.kernel, state ? 1 : 0, sizeof(cl_vbo2), &cl_vbo2);
	if (err != CL_SUCCESS) {
        std::cout << "Error: " << __LINE__ << "code: " << err << ".\n";
        exit(1);
    }

    err = clEnqueueNDRangeKernel(cl.queue, cl.kernel, 1, NULL, &global_work_size, NULL, 0, NULL, NULL);
    if (err != CL_SUCCESS) {
        std::cout << "Error: " << __LINE__ << "code: " << err << ".\n";
        exit(1);
    }

	clEnqueueReleaseGLObjects(cl.queue, 1, &cl_vbo, 0, NULL, NULL);
	clEnqueueReleaseGLObjects(cl.queue, 1, &cl_vbo2, 0, NULL, NULL);

    clFinish(cl.queue);
}
