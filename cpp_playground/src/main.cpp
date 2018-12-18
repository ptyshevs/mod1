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

int main(int ac, char *av[]) {
    if (inputIsGood(ac, av)) {
        try {
            auto controlPointsArray = readFile(av[1]);

            for(auto &cp : controlPointsArray) {
                std::cout << "Control point: " << glm::to_string(cp) << std::endl;
            }

            auto map = generate_map(controlPointsArray);

            
            auto core = sdl_gl_init();

            while(1)
            {
                SDL_PollEvent(&(core.event));
                auto keystate = SDL_GetKeyboardState(NULL);

                if ((core.event.type == SDL_QUIT) || keystate[SDL_SCANCODE_ESCAPE])
                    break;
            }

            deinit(core);

        } catch (std::exception &e) {

            
            std::cout << e.what() << std::endl;
        }
        
    } else {
        std::cout << "File is not readable\n";
    }
}
