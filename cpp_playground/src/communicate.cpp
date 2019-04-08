#include <core.hpp>

void	show_usage(void)
{
	std::cout << "usage: ./mod1 file.mod1 [-s scene_path] [--paused] [--no-emit] [--offline]" << std::endl;
	exit(1);
}

void	panic(const std::string &message)
{
	std::cout << message << std::endl;
	exit(1);
}
