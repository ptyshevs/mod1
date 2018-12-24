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

void    process_input(GLCamera &camera, GLItem &map, GLItem &points, bool *quit)
{
    auto keystate = SDL_GetKeyboardState(NULL);
    
    if (keystate[SDL_SCANCODE_ESCAPE])
        *quit = true;
    else if (keystate[SDL_SCANCODE_W])
        camera.pos += camera.speed * camera.dir;
    else if (keystate[SDL_SCANCODE_S])
        camera.pos -= camera.speed * camera.dir;
    else if (keystate[SDL_SCANCODE_E])
	{
        map.model = glm::rotate(map.model, camera.speed * glm::radians(0.1f), glm::vec3(0.0f, 1.0f, 0.0f));
        points.model = glm::rotate(points.model, camera.speed * glm::radians(0.1f), glm::vec3(0.0f, 1.0f, 0.0f));
	}
    else if (keystate[SDL_SCANCODE_Q])
	{
        map.model = glm::rotate(map.model, camera.speed * glm::radians(-0.1f), glm::vec3(0.0f, 1.0f, 0.0f));
        points.model = glm::rotate(points.model, camera.speed * glm::radians(-0.1f), glm::vec3(0.0f, 1.0f, 0.0f));
	}
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
//	normalize_cp(controlPointsArray);
    auto core = sdl_gl_init();

    auto map = generate_map(controlPointsArray);
    auto points = generate_control_points(controlPointsArray);

    auto camera = GLCamera();
    glPointSize(3);
    while(!quit)
    {
        // Event handle
        SDL_PollEvent(&(core.event));
        process_input(camera, map, points, &quit);
        if (core.event.type == SDL_QUIT)
            quit = true;

        // Actual render

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        camera.frameStart();
        draw(map, camera.vp());
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
     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, item.ibo);
//    glDrawArrays(GL_TRIANGLES, 0, item.idx_num);
	glDrawArrays(GL_POINTS, 0, item.idx_num);
//     glDrawElements(GL_TRIANGLES, item.idx_num, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}