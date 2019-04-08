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
#include <ControlPoints.hpp>
#include <Emitter.hpp>


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

void parse_scene(Emitter &emitter, std::string const &scene_path) {
    std::ifstream file(scene_path);
    const std::regex scene_obj("\\s*([A-Z]+)\\s+([a-z]+)\\s+\\(\\s*(-?\\d+)\\s*,\\s*(-?\\d+)\\s*,\\s*(-?\\d+)\\)\\s+(.*)");
	const std::regex obj_prop("\\s*(-?\\d+)");

    std::string line;
    std::vector<glm::vec3> read_data;
    while (std::getline(file, line))
    {
    	glm::vec3 origin;
        std::smatch matches;
        while (std::regex_match(line, matches, scene_obj))
        {
        	std::smatch prop_matches;
        	std::string type = matches[1].str();
        	std::string obj = matches[2].str();
        	try {
				origin = glm::vec3(std::stof(matches[3]), std::stof(matches[4]), std::stof(matches[5]));
        	} catch (std::exception &e) {
        		panic("Failed to convert string numbers to float");
        	}
        	std::string properties_str = matches[6].str();
        	std::vector<float> properties;
        	while (std::regex_search(properties_str, prop_matches, obj_prop)) {
        		try {
	        		properties.push_back(std::stof(prop_matches[1]));
        		} catch (std::exception &e) {
        			panic("Failed to convert string number to float");
        		}
        		properties_str = prop_matches.suffix();
        	}

        	if (type == "STATIC")
        		emitter.setPointType(P_STATIC);
        	else if (type == "SOLID")
        		emitter.setPointType(P_SOLID);
        	else if (type == "DYNAMIC")
        		emitter.setPointType(P_DYNAMIC);
        	else
        		panic("Object type is unrecognized: " + type);

        	if (obj == "pillow") {
        		if (properties.size() == 3)
        			emitter.pillow(origin, properties[0], properties[1], properties[2]);
        		else
					panic("Invalid amount of options for pillow, should be 3: width, radius, and height");
        	}
        	else if (obj == "sphere") {
        		if (properties.size() == 1)
        			emitter.sphere(origin, 1, properties[0]);
        		else
	        		panic("Invalid amount of options for sphere, should be 1: radius");
        	}
        	else if (obj == "wall") {
        		if (properties.size() == 3)
        			emitter.wall(origin[0], properties[0], origin[1], properties[1], origin[2], properties[2]);
        		else
	        		panic("Invalid amount of options for wall, should be 3: x_end, y_end, z_end");
        	}
        	else
        		panic("Object name is unrecognized: " + obj);
        	line = matches.suffix();
        }
    }
}

void parse_arguments(int ac, char **av, ControlPoints *cp, bool &running, bool &emitting, bool &offline, Emitter &emitter)
{
	if (ac == 1)
		show_usage();
	try {
		if (!(hasValidExtenstion(av[1]) && isReadable(av[1])))
			panic("File is not valid");
		*cp = readFile(av[1]);
		cp->prepare(true);
		} catch (std::exception &e) {
		panic(e.what());
	}
	for (int i = 2; i < ac; ++i) {
		if (strcmp(av[i], "-s") == 0) {
			if (i + 1 < ac)
				parse_scene(emitter, av[i++ + 1]);
			else
				panic("-s should be followed by path to scene");
		}
		else if (strcmp(av[i], "--paused") == 0)
			running = false;
		else if (strcmp(av[i], "--no-emit") == 0)
			emitting = false;
		else if (strcmp(av[i], "--offline") == 0)
			offline = true;
		else
			panic(std::string(av[i]) + " is not a valid command");
	}
}
