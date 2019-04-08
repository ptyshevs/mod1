/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_bindings.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ptyshevs <ptyshevs@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/07 14:48:55 by ptyshevs          #+#    #+#             */
/*   Updated: 2019/01/07 14:48:56 by ptyshevs         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "key_bindings.hpp"

void	process_input(GLCamera &camera, GLItem &map,  Water &water, bool *quit)
{
	auto keystate = SDL_GetKeyboardState(NULL);

	if (keystate[SDL_SCANCODE_ESCAPE])
		*quit = true;
	if (keystate[SDL_SCANCODE_SPACE])
		dynamic_cast<Water *>(&water)->running = !dynamic_cast<Water *>(&water)->running;
	if (keystate[SDL_SCANCODE_W])
		camera.pos += 0.1 * camera.speed * camera.dir;
	if (keystate[SDL_SCANCODE_S])
		camera.pos -= 0.1 * camera.speed * camera.dir;
	if (keystate[SDL_SCANCODE_A])
	{
		map.model = glm::translate(map.model, 0.1 * camera.speed * glm::vec3(1.0f, 0.0f, 0.0f));
		water.model = glm::translate(water.model, 0.1 * camera.speed * glm::vec3(1.0f, 0.0f, 0.0f));
	}
	if (keystate[SDL_SCANCODE_D])
	{
		map.model = glm::translate(map.model, 0.1 * camera.speed * glm::vec3(-1.0f, 0.0f, 0.0f));
		water.model = glm::translate(water.model, 0.1 * camera.speed * glm::vec3(-1.0f, 0.0f, 0.0f));
	}
	if (keystate[SDL_SCANCODE_E])
	{
		map.model = glm::rotate(map.model, camera.speed * glm::radians(0.1f), glm::vec3(0.0f, 1.0f, 0.0f));
		water.model = glm::rotate(water.model, camera.speed * glm::radians(0.1f), glm::vec3(0.0f, 1.0f, 0.0f));
	}
	if (keystate[SDL_SCANCODE_Q])
	{
		map.model = glm::rotate(map.model, camera.speed * glm::radians(-0.1f), glm::vec3(0.0f, 1.0f, 0.0f));
		water.model = glm::rotate(water.model, camera.speed * glm::radians(-0.1f), glm::vec3(0.0f, 1.0f, 0.0f));
	}
	if (keystate[SDL_SCANCODE_UP])
	{
		map.model = glm::rotate(map.model, camera.speed * glm::radians(0.1f), glm::vec3(1.0f, 0.0f, 0.0f));
		water.model = glm::rotate(water.model, camera.speed * glm::radians(0.1f), glm::vec3(1.0f, 0.0f, 0.0f));
	}
	if (keystate[SDL_SCANCODE_DOWN])
	{
		map.model = glm::rotate(map.model, camera.speed * glm::radians(-0.1f), glm::vec3(1.0f, 0.0f, 0.0f));
		water.model = glm::rotate(water.model, camera.speed * glm::radians(-0.1f), glm::vec3(1.0f, 0.0f, 0.0f));
	}
	if (keystate[SDL_SCANCODE_LEFT])
	{
		map.model = glm::rotate(map.model, camera.speed * glm::radians(0.1f), glm::vec3(0.0f, 0.0f, 1.0f));
		water.model = glm::rotate(water.model, camera.speed * glm::radians(0.1f), glm::vec3(0.0f, 0.0f, 1.0f));
	}
	if (keystate[SDL_SCANCODE_RIGHT])
	{
		map.model = glm::rotate(map.model, camera.speed * glm::radians(-0.1f), glm::vec3(0.0f, 0.0f, 1.0f));
		water.model = glm::rotate(water.model, camera.speed * glm::radians(-0.1f), glm::vec3(0.0f, 0.0f, 1.0f));
	}
	if (keystate[SDL_SCANCODE_1])
	{
		water.emitter->type = EMITER_RAIN;
		water.emitter->pps = 1000;
	}
	if (keystate[SDL_SCANCODE_2])
	{
		water.emitter->type = EMITER_WAVE;
		water.emitter->pps = 1000;
	}
	if (keystate[SDL_SCANCODE_3])
	{
		water.emitter->type = EMITER_BOUNDARIES;
		water.emitter->pps = 1000;
	}
	if (keystate[SDL_SCANCODE_KP_MINUS])
		water.emitter->pps = abs(water.emitter->pps + 100);
	if (keystate[SDL_SCANCODE_KP_PLUS])
		water.emitter->pps = abs(water.emitter->pps - 100);
	if (keystate[SDL_SCANCODE_R])
		water.emitting = !water.emitting;
	if (keystate[SDL_SCANCODE_F]) {
		if (water.emitter->type != EMITER_FLUSH)
			water.emitter->prev_type = water.emitter->type;
		water.emitter->type = EMITER_FLUSH;
	}
}
