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

std::vector<glm::vec3> generate_map(const std::vector<glm::vec3> control_points) {
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

    return map;
}
