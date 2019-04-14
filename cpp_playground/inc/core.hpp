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
#include "algo_defines.hpp"

struct ControlPoints;
struct Emitter;

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
std::vector<glm::vec3> readFile(char *filePath);
void parse_arguments(int ac, char **av, ControlPoints *cp, bool &running, bool &emitting, bool &offline, Emitter &emitter);
void save_image(std::string &&dirname);


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

struct GLItem  {
    GLuint vao;
    GLuint vbo;
    GLuint ibo;
    GLuint tex;
	GLuint tex_n;
	GLuint tex_ao;
	GLuint tex_r;
    GLsizei idx_num;
    GLuint shader_program;
    glm::mat4 model;
    std::function<void (const glm::mat4 &vp)> fill_uniforms;
	virtual ~GLItem() = default;
	virtual void	draw(const glm::mat4 &vp, GLenum type);
	void	no_err(int err, int line) noexcept(false);
};

SDLCore	sdl_gl_init(bool offline);
void	deinit(SDLCore &core);

// compileshaders.cpp
char	*reader(const char *path);
GLuint	compile_shaders(const char *vert_fpath, const char *frag_fpath);
void	show_usage(void);
void	panic(const std::string &message);

// mapgen.cpp

#include <OpenGL/OpenGL.h>
#include <OpenCL/cl.h>
#include <OpenCL/cl_gl.h>
#include <OpenCL/cl_gl_ext.h>
#include <Particle.hpp>

struct CLCore {
    cl_device_id        device;
    cl_context          context;
    cl_command_queue    queue;
    cl_program          program;
    cl_kernel           kernel;
};

struct CLWaterCore: CLCore {
	// kernel is used for neighbors search and density/pressure update
	cl_kernel           accumForces;
	cl_program          accumForcesProgram;
	cl_kernel           integrateResolve;
	cl_program          integrateResolveProgram;
	cl_kernel           simUpdate;
	cl_program          simUpdateProgram;
	cl_kernel           neighborCaching;
	cl_program          neighborCachingProgram;
	cl_kernel           findNeighbors;
	cl_program          findNeighborsProgram;
};

struct CLGLDoubleBufferedItem: GLItem {
	// CL ctx, queue, kernel
	CLCore cl;
	// GL second vbo buffer
	GLuint vbo2;
	// CL shared vbo's
	cl_mem cl_vbo;
	cl_mem cl_vbo2;
	// Determine buffer that be next
	bool state = false;
};

struct Cell
{
	cl_uint      n_inside;
	cl_uint      particles[MAX_PER_CELL];
public:
	Cell() : n_inside(0) {};
};

/*
 * Constants used for map interpolation
 */
typedef struct  s_constants {
    cl_int      map_size;
    cl_int      cp_size;
}               t_constants;


/*
 * Emitter parameters passed to kernel
 */
typedef struct {
    cl_int type;
    cl_uint pps;
	cl_uint seed;
} t_emiter;

// cl_init.cpp

void	cl_host_part(CLCore &cl_core, bool wGLInterop = false);
void	cl_compile_kernel(CLCore &cl, const char *filepath, const char *program_name);
void	cl_compile_water_kernel(CLWaterCore &cl, cl_program &program_field, cl_kernel &kernel_field,
		const char *filepath, const char *program_name);

// textures.cpp

#include <lodepng.h>

GLuint	load_texture(const char *tex_file_name);
