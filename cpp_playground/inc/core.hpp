/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ptyshevs <ptyshevs@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/09 16:41:59 by vpopovyc          #+#    #+#             */
/*   Updated: 2018/12/24 21:06:26 by ptyshevs         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

// Tmp
#include <iostream>
#define GLM_ENABLE_EXPERIMENTAL

// number of points per axis
# define sl 100
// half the number (used for centering in NDC
# define hf_sl 50

// Include all GLM core / GLSL features & extensions
// vec2, vec3, mat4, radians, perspective, translate, rotate
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "glm/gtx/string_cast.hpp"

// parsing.cpp 
// - STD
#include <fstream>
#include <stdexcept>
#include <regex>
#include <cstring>
// - Custom
bool inputIsGood(int ac, char *av[]);
std::vector<glm::vec3> readFile(char *filePath);

// sdl_gl_init.cpp
// OpenGL & SDL

# define GL3_PROTOTYPES 1
# include <GL/glew.h>
# include <SDL2/SDL.h>

# define WINY		720
# define WINX		1280
# define WINNAME	"mod1"

struct SDLCore {
	SDL_Window		*win;
	SDL_GLContext	ctx;
	SDL_Event		event;
};

struct GLItem {
    GLuint vao;
    GLuint vbo;
    GLuint ibo;
    GLuint tex;
    GLsizei idx_num;
    GLuint shader_program;
    glm::mat4 model;
    std::function<void (const glm::mat4 &vp)> fill_uniforms;
};

SDLCore	sdl_gl_init(void);
void	deinit(SDLCore &core);

// compileshaders.cpp
char	*reader(const char *path);
GLuint	compile_shaders(const char *vert_fpath, const char *frag_fpath);

// mapgen.cpp

#include "OpenCL/opencl.h"

struct CLCore {
    cl_device_id        device;
    cl_context          context;
    cl_command_queue    queue;
    cl_program          program;
    cl_kernel           kernel;
};

struct Cell
{
	int		id;
	bool	is_solid;
	float	volume;
public:
	Cell(int id, bool is_solid, float volume) : id(id), is_solid(is_solid), volume(volume) {};
};

struct HeightMap: GLItem {
public:
	std::vector<Cell>		height_map; // pseudo-3D height map, each cell contains volume of water
	std::vector<glm::vec3>	water_cells; // vector of cells with non-zero volume

	Cell	&address(int x, int y, int z) {
		return (this->height_map[x * sl * sl + y * sl + z]);
	}
	glm::vec3 to_coords(int x, int y, int z)
	{
		return (glm::vec3(x, y, z) - (float)hf_sl);
	}
	void	show_hmap(void);
};

HeightMap	generate_map(std::vector<glm::vec3> control_points);
GLItem	generate_control_points(std::vector<glm::vec3> control_points);
void	prepare_control_points(std::vector<glm::vec3> &cpoints);


// cl_init.cpp

void    cl_host_part(CLCore &cl_core);
void    cl_compile_kernel(CLCore &cl, const char *filepath, const char *program_name);