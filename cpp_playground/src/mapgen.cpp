/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mapgen.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpopovyc <vpopovyc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/09 16:40:21 by vpopovyc          #+#    #+#             */
/*   Updated: 2018/12/17 19:25:56 by vpopovyc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <core.hpp>

static const size_t sl = 400;
static const size_t hf_sl = 200;

/*
** Generate map here
** It should be a function that renturn continius array of block pos 400x400x1
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
    return indexes;
}

GLItem generate_map(const std::vector<glm::vec3> control_points) {
    // Generate points
    // It should be done using OpenCL
    (void)control_points;
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
