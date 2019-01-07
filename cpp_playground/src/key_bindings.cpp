#include <core.hpp>
#include <camera.hpp>

void	process_input(GLCamera &camera, GLItem &map,  Water &water, bool *quit)
{
	auto keystate = SDL_GetKeyboardState(NULL);

	if (keystate[SDL_SCANCODE_ESCAPE])
		*quit = true;
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
//		points.model = glm::rotate(points.model, camera.speed * glm::radians(0.1f), glm::vec3(0.0f, 1.0f, 0.0f));
		water.model = glm::rotate(water.model, camera.speed * glm::radians(0.1f), glm::vec3(0.0f, 1.0f, 0.0f));
	}
	if (keystate[SDL_SCANCODE_Q])
	{
		map.model = glm::rotate(map.model, camera.speed * glm::radians(-0.1f), glm::vec3(0.0f, 1.0f, 0.0f));
//		points.model = glm::rotate(points.model, camera.speed * glm::radians(-0.1f), glm::vec3(0.0f, 1.0f, 0.0f));
		water.model = glm::rotate(water.model, camera.speed * glm::radians(-0.1f), glm::vec3(0.0f, 1.0f, 0.0f));
	}
	if (keystate[SDL_SCANCODE_UP])
	{
		map.model = glm::rotate(map.model, camera.speed * glm::radians(0.1f), glm::vec3(1.0f, 0.0f, 0.0f));
//		points.model = glm::rotate(points.model, camera.speed * glm::radians(0.1f), glm::vec3(1.0f, 0.0f, 0.0f));
		water.model = glm::rotate(water.model, camera.speed * glm::radians(0.1f), glm::vec3(1.0f, 0.0f, 0.0f));
	}
	if (keystate[SDL_SCANCODE_DOWN])
	{
		map.model = glm::rotate(map.model, camera.speed * glm::radians(-0.1f), glm::vec3(1.0f, 0.0f, 0.0f));
//		points.model = glm::rotate(points.model, camera.speed * glm::radians(-0.1f), glm::vec3(1.0f, 0.0f, 0.0f));
		water.model = glm::rotate(water.model, camera.speed * glm::radians(-0.1f), glm::vec3(1.0f, 0.0f, 0.0f));
	}
	if (keystate[SDL_SCANCODE_LEFT])
	{
		map.model = glm::rotate(map.model, camera.speed * glm::radians(0.1f), glm::vec3(0.0f, 0.0f, 1.0f));
//		points.model = glm::rotate(points.model, camera.speed * glm::radians(0.1f), glm::vec3(0.0f, 0.0f, 1.0f));
		water.model = glm::rotate(water.model, camera.speed * glm::radians(0.1f), glm::vec3(0.0f, 0.0f, 1.0f));
	}
	if (keystate[SDL_SCANCODE_RIGHT])
	{
		map.model = glm::rotate(map.model, camera.speed * glm::radians(-0.1f), glm::vec3(0.0f, 0.0f, 1.0f));
//		points.model = glm::rotate(points.model, camera.speed * glm::radians(-0.1f), glm::vec3(0.0f, 0.0f, 1.0f));
		water.model = glm::rotate(water.model, camera.speed * glm::radians(-0.1f), glm::vec3(0.0f, 0.0f, 1.0f));
	}
	if (keystate[SDL_SCANCODE_1])
	{
		water.emiter.type = EMITER_RAIN;
		water.emiter.pps = 1000;
	}
	if (keystate[SDL_SCANCODE_2])
	{
		water.emiter.type = EMITER_WAVE;
		water.emiter.pps = 1000;
	}
	if (keystate[SDL_SCANCODE_3])
	{
		water.emiter.type = EMITER_UNDERGROUND;
		water.emiter.pps = 1000;
	}
	if (keystate[SDL_SCANCODE_4])
	{
		water.emiter.type = EMITER_BOUNDARIES;
		water.emiter.pps = 1000;
	}
	if (keystate[SDL_SCANCODE_KP_MINUS])
		water.emiter.pps = abs(water.emiter.pps + 100);
	if (keystate[SDL_SCANCODE_KP_PLUS])
		water.emiter.pps = abs(water.emiter.pps - 100);
}
