/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpopovyc <vpopovyc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/09 16:40:21 by vpopovyc          #+#    #+#             */
/*   Updated: 2018/12/17 17:37:38 by vpopovyc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <core.hpp>
#include <camera.hpp>

void draw(GLItem /*item*/, const glm::mat4 &/*vp*/);

int main(int ac, char *av[]) {
    if (inputIsGood(ac, av)) {
        try {
            auto controlPointsArray = readFile(av[1]);

            auto core = sdl_gl_init();

            auto map = generate_map(controlPointsArray);

            auto camera = GLCamera();

            while(1)
            {
                // Event handle

                SDL_PollEvent(&(core.event));
                auto keystate = SDL_GetKeyboardState(NULL);

                if ((core.event.type == SDL_QUIT) || keystate[SDL_SCANCODE_ESCAPE])
                    break;

                if (keystate[SDL_SCANCODE_W])
                    camera.pos += camera.speed * camera.dir;
                if (keystate[SDL_SCANCODE_S])
                    camera.pos -= camera.speed * camera.dir;
                if (keystate[SDL_SCANCODE_E]) {
                    map.model = glm::rotate(map.model, camera.speed * glm::radians(0.1f), glm::vec3(0.0f, 1.0f, 0.0f));
                }
                if (keystate[SDL_SCANCODE_Q]) {
                    map.model = glm::rotate(map.model, camera.speed * glm::radians(-0.1f), glm::vec3(0.0f, 1.0f, 0.0f));
                }

                // Actual render

                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                camera.frameStart();
                draw(map, camera.vp());
                camera.frameEnd();

                SDL_GL_SwapWindow(core.win);
            }

            deinit(core);

        } catch (std::exception &e) {
            std::cout << e.what() << std::endl;
        }
        
    } else {
        std::cout << "File is not readable\n";
    }
}

void draw(GLItem item, const glm::mat4 &vp)
{
    glUseProgram(item.shader_program);
    item.fill_uniforms(vp);
    glBindVertexArray(item.vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, item.ibo);
    glDrawElements(GL_TRIANGLES, item.idx_num, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}