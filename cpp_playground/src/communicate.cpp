#include <core.hpp>
// unused functions

void	show_usage(void)
{
	std::cout << "usage: ./mod1 file.mod1 [--expode|--snow]" << std::endl;
}

void	panic(const std::string &message)
{
	std::cout << message << std::endl;
	exit(1);
}

bool	is_snowing(int ac, char **av)
{
	return (ac == 3 && !strcmp(av[2], "--snow"));
}

bool	exploding(int ac, char **av)
{
	return (ac == 3 && !strcmp(av[2], "--explode"));
}
//
//
//int		to1D(int x, int y, int z)
//{
//	x = x + hf_sl;
//	z = z + hf_sl;
//	int v = (sl * (sl / 4) * (x + hf_sl)) + sl * y + (z + hf_sl);
//	return (v);
//}
//
//glm::vec3		to3D(int n)
//{
//	int x = n / (sl * (sl / 4)) - hf_sl;
//	n -= (x + hf_sl) * (sl * (sl / 4));
//	int y = n / (sl);
//	int z = n % (sl) - hf_sl;
//	glm::vec3 f(x, y, z);
//	return (f);
//}
