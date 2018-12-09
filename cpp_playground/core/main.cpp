/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpopovyc <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/09 16:40:21 by vpopovyc          #+#    #+#             */
/*   Updated: 2018/12/09 16:40:22 by vpopovyc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <core.hpp>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <glm/glm.hpp>
#include <regex>

bool isReadable(char *filePath) {
    std::ifstream fs(filePath);
    return fs.is_open();
}

bool isInputOK(int ac, char *av[]) {
    bool isInputOK = false;

    if (ac == 2) {
        isInputOK = isReadable(av[1]);
    }
    return isInputOK;
}

void readFile(char *filePath) {
    std::ifstream file(filePath);
    std::string line;
    bool isEmpty = true;

    const std::regex isVec3("\\s*(?:\\s*(\\d*)\\s*,\\s*(\\d*)\\s*,\\s*(\\d*)\\s*)\\s*"); 

    while (std::getline(file, line)) {
        isEmpty = false;
        std::cout << "New line: " +  line + "\n";
        std::smatch matches;
        

        while (std::regex_search(line, matches, isVec3)) {

        int match_iter = 0;
        for(auto submatch: matches) {
            // First match is a full string
            // All others is a values
            std::cout << "s: " << submatch << std::endl;
        }

        }
    }

    if (isEmpty) {
        throw std::invalid_argument("Get no data from read pass.");
    }
}

int main(int ac, char *av[]) {
    if (isInputOK(ac, av)) {
        try {
            readFile(av[1]);
        } catch (std::exception &e) {
            std::cout << e.what() << std::endl;
        }
        
    } else {
        std::cout << "File is not readable\n";
    }
}
