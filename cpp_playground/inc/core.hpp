/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpopovyc <vpopovyc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/09 16:41:59 by vpopovyc          #+#    #+#             */
/*   Updated: 2018/12/17 19:24:08 by vpopovyc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

// Tmp
#include <iostream>
#define GLM_ENABLE_EXPERIMENTAL

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
GLuint	compile_shaders(const char *vert_fpath, const char *frag_fpath);

// mapgen.cpp

GLItem generate_map(const std::vector<glm::vec3> control_points);
