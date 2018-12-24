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

static const size_t sl = 100;
static const size_t hf_sl = sl / 2;

/*
** Generate map here
** It should be a function that return continuous array of block pos 400x400x1
*/

std::vector<glm::ivec3> generate_triangulated_mesh_indices() {

    std::vector<glm::ivec3> indexes;

     indexes.reserve(sl*sl/8);

     for (size_t i = 0; i < sl - 1; i++) {
         for (size_t j = 0; j < sl - 1; j++) {
             int p = i * sl +j;
             indexes.emplace_back(p, p+1, p+sl);
             indexes.emplace_back(p+sl, p+1, p+sl+1);
         }
     }
//     indexes.emplace_back(0, 1, 2);
    return indexes;
}

/*
** Setup opencl
** Clamp controll points values on map dimensions -> todo
** Modify map
*/

void interpolate_using_controll_points(std::vector<glm::vec3> &cp, std::vector<glm::vec3> &map) {

    // auto cl = CLCore();

    // cl_host_part(cl);
    // cl_compile_kernel(cl, "src/kernels/idw_kernel.cl", "idw_kernel");

    // cl_mem cl_map = clCreateBuffer(cl.context, CL_MEM_READ_WRITE, map.size() * sizeof(glm::vec3), NULL, NULL);
    // cl_mem cl_cp = clCreateBuffer(cl.context, CL_MEM_READ_ONLY, cp.size() * sizeof(glm::vec3), NULL, NULL);
    // cl_mem cl_cp_size = clCreateBuffer(cl.context, CL_MEM_READ_ONLY, sizeof(size_t), NULL, NULL);
    // cl_mem cl_map_size = clCreateBuffer(cl.context, CL_MEM_READ_ONLY, sizeof(size_t), NULL, NULL);

    // int err;
    // err = clEnqueueWriteBuffer(cl.queue, cl_map, CL_TRUE, 0, map.size() * sizeof(glm::vec3), map.data(), 0, NULL, NULL);
    // err |= clEnqueueWriteBuffer(cl.queue, cl_cp, CL_TRUE, 0, cp.size() * sizeof(glm::vec3), cp.data(), 0, NULL, NULL);
    // auto cp_size = cp.size();
    // err |= clEnqueueWriteBuffer(cl.queue, cl_cp_size, CL_TRUE, 0, sizeof(size_t), &cp_size, 0, NULL, NULL);
    // auto map_size = map.size();
    // err |= clEnqueueWriteBuffer(cl.queue, cl_map_size, CL_TRUE, 0, sizeof(size_t), &map_size, 0, NULL, NULL);
    // if (err != CL_SUCCESS) {
    //     std::cout << "Error" << __LINE__ << "[" << err << "]\n";
    //     exit(1);
    // }

    // err = clSetKernelArg(cl.kernel, 0, sizeof(cl_cp), &cl_cp);
    // err |= clSetKernelArg(cl.kernel, 1, sizeof(cl_cp_size), &cl_cp_size);
    // err |= clSetKernelArg(cl.kernel, 2, sizeof(cl_map), &cl_map);
    // err |= clSetKernelArg(cl.kernel, 3, sizeof(cl_map_size), &cl_map_size);
    // if (err != CL_SUCCESS) {
    //     std::cout << "Error" << __LINE__ << "[" << err << "]\n";
    //     exit(1);
    // }

    // size_t  global_work_size = map.size();
    // err = clEnqueueNDRangeKernel(cl.queue, cl.kernel, 1, NULL, &global_work_size, NULL, 0, NULL, NULL);
    // if (err != CL_SUCCESS) {
    //     std::cout << "Error" << __LINE__ << "[" << err << "]\n";
    //     exit(1);
    // }

    // clFinish(cl.queue);

    // err = clEnqueueReadBuffer(cl.queue, cl_map, CL_TRUE, 0, map.size() * sizeof(glm::vec3), map.data(), 0, NULL, NULL);
    // if (err != CL_SUCCESS) {
    //     std::cout << "Error" << __LINE__ << "[" << err << "]\n";
    //     exit(1);
    // }
//    map = cp;
}

double	idw(std::vector<glm::vec3> control_points, glm::vec3 point)
{
	double d = 0;
	double w = 0;
	double num = 0;
	double denom = 0;
	double min_y = INT_MAX;

	for (glm::vec3 cp: control_points)
	{
		d = glm::pow(cp.x - point.x, 2) + glm::pow(cp.z - point.z, 2);
		d = glm::sqrt(d);
		d = glm::pow(d, 1.3); // sigma-smoothing
		w = glm::pow(d, -2);
		num += w * cp.y;
		if (cp.y > min_y)
			min_y = cp.y;
		denom += w;
	}
	return (denom > 0 ? num/denom : min_y);
}


GLItem generate_map(std::vector<glm::vec3> control_points) {
	// todo: generate border points
	// todo: normalize control points
    // Generate points
    // It should be done using OpenCL
    std::vector<glm::vec3> map(sl * sl, glm::vec3(0.0f));
    for (size_t i = 0; i < sl; i++) {
        for (size_t j = 0; j < sl; j++) {
            auto &point = map[i * sl + j];
            point.x = (float)i - hf_sl;
            point.z = (float)j - hf_sl;
            point.y = idw(control_points, point);
        }
    }

    // Generate triangulated indices for mesh opt rendering
    std::vector<glm::ivec3> map_render_indices = generate_triangulated_mesh_indices();
    // std::vector<glm::ivec3> map_render_indices = {0, 1, 2};

    // Interpolate stuff
    interpolate_using_controll_points(control_points, map);



    // Create object suitable for rendering
    GLItem map_item;

    map_item.model = glm::mat4(1.0f);
    map_item.idx_num = map_render_indices.size() * 3;
    map_item.shader_program = compile_shaders("cpp_playground/src/shaders/ground_vertex.glsl",
                                              "cpp_playground/src/shaders/ground_fragment.glsl");
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
    points_item.shader_program = compile_shaders("cpp_playground/src/shaders/ground_vertex.glsl",
                                                 "cpp_playground/src/shaders/ground_fragment.glsl");
    points_item.fill_uniforms = [&](const glm::mat4 &vp) {
        auto mvp_id = glGetUniformLocation(points_item.shader_program, "MVP");
        auto mvp = vp * points_item.model;
        glUniformMatrix4fv(mvp_id, 1, GL_FALSE, glm::value_ptr(mvp));
    };
    std::vector<glm::ivec3> indices = {{0, 1, 2}, {1, 2, 3}, {2, 3, 4}, {3, 4, 5}, {4, 5, 6}, {5, 6, 7}};
    glGenBuffers(1, &points_item.vbo);
    glGenBuffers(1, &points_item.ibo);
    glGenVertexArrays(1, &points_item.vao);

    glBindVertexArray(points_item.vao);
    glBindBuffer(GL_ARRAY_BUFFER, points_item.vbo);
    glBufferData(GL_ARRAY_BUFFER, control_points.size() * sizeof(glm::vec3), control_points.data(), GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, points_item.ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(glm::ivec3), indices.data(), GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, sizeof(glm::vec3), (GLvoid *)0);
    glBindVertexArray(0);
    return (points_item);
}