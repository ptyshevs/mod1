/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sdl_gl_init.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ptyshevs <ptyshevs@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/09 16:41:59 by vpopovyc          #+#    #+#             */
/*   Updated: 2018/12/24 20:47:23 by ptyshevs         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <core.hpp>

void	gl_attr_init(void) {
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
						SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
}

void	gl_version_info(void) {
	int major_version;
	int minor_version;

	major_version = 0;
	minor_version = 0;
	SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &major_version);
	SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &minor_version);

    std::cout << "OpenGL version: " << major_version << "." << minor_version << std::endl; 
}

SDLCore	sdl_gl_init(void) {
    SDLCore core = SDLCore();
	assert(SDL_Init(SDL_INIT_VIDEO) == 0);
	gl_attr_init();
	core.win = SDL_CreateWindow(WINNAME,SDL_WINDOWPOS_CENTERED,
										SDL_WINDOWPOS_CENTERED,
										WINX, WINY, SDL_WINDOW_OPENGL);
	assert(core.win != NULL);
	core.ctx = SDL_GL_CreateContext(core.win);
	glViewport(0, 0, WINX, WINY);
	glEnable(GL_DEPTH_TEST);
	glewExperimental = 1;
	glewInit();
	SDL_GL_SetSwapInterval(1);
	glClearColor(0.863, 0.863, 0.863, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	SDL_GL_SwapWindow(core.win);
	gl_version_info();
    return core;
}

void	deinit(SDLCore &core) {
	SDL_GL_DeleteContext(core.ctx);
	SDL_DestroyWindow(core.win);
	SDL_Quit();
}
