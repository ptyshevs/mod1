/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ptyshevs <ptyshevs@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/09 16:40:21 by vpopovyc          #+#    #+#             */
/*   Updated: 2018/12/24 21:11:32 by ptyshevs         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <core.hpp>
#include <camera.hpp>

void draw(GLItem /*item*/, const glm::mat4 &/*vp*/);

void    process_input(GLCamera camera, GLItem map, bool *quit)
{
    auto keystate = SDL_GetKeyboardState(NULL);
    
    if (keystate[SDL_SCANCODE_ESCAPE])
        *quit = true;
    else if (keystate[SDL_SCANCODE_W])
        camera.pos += camera.speed * camera.dir;
    else if (keystate[SDL_SCANCODE_S])
        camera.pos -= camera.speed * camera.dir;
    else if (keystate[SDL_SCANCODE_E])
        map.model = glm::rotate(map.model, camera.speed * glm::radians(0.1f), glm::vec3(0.0f, 1.0f, 0.0f));
    else if (keystate[SDL_SCANCODE_Q])
        map.model = glm::rotate(map.model, camera.speed * glm::radians(-0.1f), glm::vec3(0.0f, 1.0f, 0.0f));
}

int main(int ac, char *av[]) {
    bool quit = false;
    std::vector<glm::vec3> controlPointsArray;

    if (!inputIsGood(ac, av))
    {
        std::cout << "File is not readable\n";
        return (1);
    }
    try {
        controlPointsArray = readFile(av[1]);
        } catch (std::exception &e) {
            std::cout << e.what() << std::endl;
            return (1);
        }

    auto core = sdl_gl_init();

    auto map = generate_map(controlPointsArray);
    auto points = generate_control_points(controlPointsArray);

    auto camera = GLCamera();

    while(1)
    {
        // Event handle
        SDL_PollEvent(&(core.event));
        process_input(camera, map, &quit);
        if ((core.event.type == SDL_QUIT) || quit)
            break;

        // Actual render

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        camera.frameStart();
        // draw(map, camera.vp());
        draw(points, camera.vp());
        camera.frameEnd();

        SDL_GL_SwapWindow(core.win);
    }

    deinit(core);

}

void draw(GLItem item, const glm::mat4 &vp)
{
    glUseProgram(item.shader_program);
    item.fill_uniforms(vp);
    glBindVertexArray(item.vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, item.ibo);
    // glDrawElements(GL_TRIANGLES, item.idx_num, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}