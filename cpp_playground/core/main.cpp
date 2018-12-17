/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpopovyc <vpopovyc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/09 16:40:21 by vpopovyc          #+#    #+#             */
/*   Updated: 2018/12/17 14:17:48 by vpopovyc         ###   ########.fr       */
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
        } catch (std::exception &e) {
            std::cout << e.what() << std::endl;
        }
        
    } else {
        std::cout << "File is not readable\n";
    }
}
