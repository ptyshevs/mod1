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

            for (size_t i = 0; i < 400; i++)
            {
                for (size_t j = 0; j < 400; j++)
                {
                    auto &point = map[i * 400 + j];
                    std::cout << point.x << " " << point.y << " " << point.z << " | ";
                }
                std::cout << std::endl;
            }
        } catch (std::exception &e) {
            std::cout << e.what() << std::endl;
        }
        
    } else {
        std::cout << "File is not readable\n";
    }
}
