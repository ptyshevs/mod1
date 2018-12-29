/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpopovyc <vpopovyc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/09 16:40:21 by vpopovyc          #+#    #+#             */
/*   Updated: 2018/12/17 14:22:23 by vpopovyc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <core.hpp>

static bool isReadable(char *filePath)
{
    std::ifstream fs(filePath);
    return fs.is_open();
}

static bool hasValidExtenstion(char *filePath)
{
    bool hasValidExtenstion = false;

    char *dot = strrchr(filePath, '.');
    if (dot != NULL)
    {
        if (strncmp(dot + 1, "mod1", 4) == 0)
        {
            hasValidExtenstion = true;
        }
    }
    return hasValidExtenstion;
}

bool inputIsGood(int ac, char *av[])
{
    bool inputIsGood = false;

    if (ac == 2 || ac == 3)
    {
        inputIsGood = hasValidExtenstion(av[1]) && isReadable(av[1]);
    }
    return inputIsGood;
}

std::vector<glm::vec3> readFile(char *filePath)
{
    std::ifstream file(filePath);
    const std::regex isVec3("\\s*(?:\\s*(\\d*)\\s*,\\s*(\\d*)\\s*,\\s*(\\d*)\\s*)\\s*");

    std::string line;
    std::vector<glm::vec3> read_data;
    read_data.reserve(50);
    while (std::getline(file, line))
    {
        std::smatch matches;
        while (std::regex_search(line, matches, isVec3))
        {
            read_data.emplace_back(std::stoi(matches[1]),
                                   std::stoi(matches[2]),
                                   std::stoi(matches[3]));
            line = matches.suffix();
        }
    }

    if (read_data.empty())
    {
        throw std::invalid_argument("Get no valid data from read pass.");
    }

    read_data.shrink_to_fit();
    return read_data;
}
