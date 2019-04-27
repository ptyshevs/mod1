/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpopovyc <vpopovyc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/09 16:41:59 by vpopovyc          #+#    #+#             */
/*   Updated: 2018/12/17 19:24:08 by vpopovyc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CAMERA_HPP
# define CAMERA_HPP
#include <core.hpp>

/*
 * Camera object
 */
struct GLCamera {
    glm::vec3 pos;
    glm::vec3 front;
    glm::vec3 dir;
    glm::vec3 up;

    float near_z;
    float far_z;
    float aspect_ratio;
    float fov;

    float speed;
    float delta_time;
    float last_time;
    float current_time;

    GLCamera() {
        delta_time = 0.0f;
        last_time = 0.0f;
        current_time = 0.0f;
        speed = 0.0f;
        fov = 60.0f;
        near_z = 0.1f;
        far_z = 5000.0f;
        aspect_ratio = (float)WINX / (float)WINY;
        pos = glm::vec3(0.0f, 30.0f, 150.0f);
        front = glm::vec3(0.0f, 0.0f, 0.0f);
        up = glm::vec3(0.0f, 1.0f, 0.0f);
        dir = glm::normalize(front - pos);
    }

    const glm::mat4 view(void) const {
        return glm::lookAt(pos, front, up);
    }

    const glm::mat4 projection(void) const {
        return glm::perspective(glm::radians(fov), aspect_ratio, near_z, far_z);
    }

    const glm::mat4 vp(void) const {
        return projection() * view();
    }

    void frameStart() {
        current_time = SDL_GetTicks();
    }

    void frameEnd() {
        delta_time = current_time - last_time;
        last_time = current_time;
        speed = 0.5f * delta_time;
    }
};

#endif