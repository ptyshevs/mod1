/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mapgen.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ptyshevs <ptyshevs@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/09 16:40:21 by vpopovyc          #+#    #+#             */
/*   Updated: 2018/12/24 21:05:45 by ptyshevs         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <core.hpp>

/*
** Generate map here
** It should be a function that return continuous array of block pos 400x400x1
*/

std::vector<glm::ivec3> generate_triangulated_mesh_indices() {

	std::vector<glm::ivec3> indexes;

	indexes.reserve(sl*sl/8);

	for (size_t i = 0; i < sl - 1; i++) {
		for (size_t j = 0; j < sl - 1; j++) {
			int p = i * (int)sl + j;
			indexes.emplace_back(p, p+1, p+sl);
			indexes.emplace_back(p+sl, p+1, p+sl+1);
		}
	}
	return indexes;
}

/*
** Setup opencl
** Clamp controll points values on map dimensions -> todo
** Modify map
*/

void interpolate_using_controll_points(std::vector<glm::vec3> &cp, std::vector<glm::vec3> &map)
{
	auto cl = CLCore();

	cl_host_part(cl);
	cl_compile_kernel(cl, "src/kernels/idw_kernel.cl", "idw_kernel");

	int err = CL_SUCCESS;

	cl_mem cl_read_map = clCreateBuffer(cl.context, CL_MEM_READ_ONLY, map.size() * sizeof(glm::vec3), NULL, &err);
	cl_mem cl_write_map = clCreateBuffer(cl.context, CL_MEM_WRITE_ONLY, map.size() * sizeof(glm::vec3), NULL, &err);
	cl_mem cl_cp = clCreateBuffer(cl.context, CL_MEM_READ_ONLY, cp.size() * sizeof(glm::vec3), NULL, &err);
	cl_mem cl_constants = clCreateBuffer(cl.context, CL_MEM_READ_ONLY, sizeof(t_constants), NULL, &err);
	if (err != CL_SUCCESS) {
		std::cout << "Error: " << __LINE__ << "code: " << err << ".\n";
		exit(1);
	}

	t_constants constants = (t_constants){ (cl_int)map.size(), (cl_int)cp.size() };
	err = clEnqueueWriteBuffer(cl.queue, cl_read_map, CL_TRUE, 0, map.size() * sizeof(glm::vec3), map.data(), 0, NULL, NULL);
	err |= clEnqueueWriteBuffer(cl.queue, cl_write_map, CL_TRUE, 0, map.size() * sizeof(glm::vec3), map.data(), 0, NULL, NULL);
	err |= clEnqueueWriteBuffer(cl.queue, cl_cp, CL_TRUE, 0, cp.size() * sizeof(glm::vec3), cp.data(), 0, NULL, NULL);
	err |= clEnqueueWriteBuffer(cl.queue, cl_constants, CL_TRUE, 0, sizeof(t_constants), &constants, 0, NULL, NULL);
	if (err != CL_SUCCESS) {
		std::cout << "Error: " << __LINE__ << "code: " << err << ".\n";
		exit(1);
	}

	err = clSetKernelArg(cl.kernel, 0, sizeof(cl_cp), &cl_cp);
	err |= clSetKernelArg(cl.kernel, 1, sizeof(cl_read_map), &cl_read_map);
	err |= clSetKernelArg(cl.kernel, 2, sizeof(cl_write_map), &cl_write_map);
	err |= clSetKernelArg(cl.kernel, 3, sizeof(cl_constants), &cl_constants);
	if (err != CL_SUCCESS) {
		std::cout << "Error: " << __LINE__ << "code: " << err << ".\n";
		exit(1);
	}

	size_t  global_work_size = map.size();
	err = clEnqueueNDRangeKernel(cl.queue, cl.kernel, 1, NULL, &global_work_size, NULL, 0, NULL, NULL);
	if (err != CL_SUCCESS) {
		std::cout << "Error: " << __LINE__ << "code: " << err << ".\n";
		exit(1);
	}

	clFinish(cl.queue);

	err = clEnqueueReadBuffer(cl.queue, cl_write_map, CL_TRUE, 0, map.size() * sizeof(glm::vec3), map.data(), 0, NULL, NULL);
	if (err != CL_SUCCESS) {
		std::cout << "Error: " << __LINE__ << "code: " << err << ".\n";
		exit(1);
	}

	clReleaseMemObject(cl_read_map);
	clReleaseMemObject(cl_write_map);
	clReleaseMemObject(cl_cp);
	clReleaseMemObject(cl_constants);
	clReleaseProgram(cl.program);
	clReleaseKernel(cl.kernel);
	clReleaseCommandQueue(cl.queue);
	clReleaseDevice(cl.device);
	clReleaseContext(cl.context);
}

GLItem generate_map(std::vector<glm::vec3> control_points, std::vector<Cell> &hmap) {
	std::vector<glm::vec3> map(sl * sl, glm::vec3(0.0f));
	for (size_t i = 0; i < sl; i++) {
		for (size_t j = 0; j < sl; j++) {
			auto &point = map[i * sl + j];
			point.x = (float)i - hf_sl;
			point.z = (float)j - hf_sl;
		}
	}

	// Generate triangulated indices for mesh opt rendering
	std::vector<glm::ivec3> map_render_indices = generate_triangulated_mesh_indices();

	// Interpolate stuff
	interpolate_using_controll_points(control_points, map);

	// Remap interpolated map on height map
	for (size_t i = 0; i < sl; i++)
	{
		for (size_t j = 0; j < hf_sl / 2; j++)
		{
			for (size_t k = 0; k < sl; k++)
			{
				auto &point = map[i * sl + k];
				hmap.emplace_back(glm::vec3((float)i - (int)hf_sl, (float)j, (float)k - (int)hf_sl),
								  (float)j <= point.y,
								  0.0f);
			}
		}
	}
	// Create object suitable for rendering
	GLItem map_item;

	map_item.model = glm::mat4(1.0f);
	map_item.idx_num = map_render_indices.size() * 3;
	map_item.shader_program = compile_shaders("src/shaders/ground_vertex.glsl",
												 "src/shaders/ground_fragment.glsl");
	map_item.fill_uniforms = [&](const glm::mat4 &vp) {
		auto mvp_id = glGetUniformLocation(map_item.shader_program, "MVP");
		auto mvp = vp * map_item.model;
		glUniformMatrix4fv(mvp_id, 1, GL_FALSE, glm::value_ptr(mvp));
	};
	glGenBuffers(1, &map_item.vbo);
	glGenBuffers(1, &map_item.ibo);
	glGenVertexArrays(1, &map_item.vao);
	glBindVertexArray(map_item.vao);
	glBindBuffer(GL_ARRAY_BUFFER, map_item.vbo);
	glBufferData(GL_ARRAY_BUFFER, map.size() * sizeof(glm::vec3), map.data(), GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, map_item.ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, map_render_indices.size() * sizeof(glm::ivec3), map_render_indices.data(), GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, sizeof(glm::vec3), (GLvoid*)0);
	glBindVertexArray(0);
	return map_item;
}

GLItem generate_control_points(std::vector<glm::vec3> control_points)
{
	GLItem points_item;

	points_item.model = glm::mat4(1.0f);
	points_item.idx_num = control_points.size() * 3;
	points_item.shader_program = compile_shaders("src/shaders/ground_vertex.glsl",
												 "src/shaders/ground_fragment.glsl");
	points_item.fill_uniforms = [&](const glm::mat4 &vp) {
		auto mvp_id = glGetUniformLocation(points_item.shader_program, "MVP");
		auto mvp = vp * points_item.model;
		glUniformMatrix4fv(mvp_id, 1, GL_FALSE, glm::value_ptr(mvp));
	};
	glGenBuffers(1, &points_item.vbo);
	glGenBuffers(1, &points_item.ibo);
	glGenVertexArrays(1, &points_item.vao);

	glBindVertexArray(points_item.vao);
	glBindBuffer(GL_ARRAY_BUFFER, points_item.vbo);
	glBufferData(GL_ARRAY_BUFFER, control_points.size() * sizeof(glm::vec3), control_points.data(), GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, sizeof(glm::vec3), (GLvoid *)0);
	glBindVertexArray(0);
	return (points_item);
}