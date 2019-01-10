/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HeightMap.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ptyshevs <ptyshevs@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/07 14:48:58 by ptyshevs          #+#    #+#             */
/*   Updated: 2019/01/07 14:48:59 by ptyshevs         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <HeightMap.hpp>

void	HeightMap::show() const
{
	for (int i = 0; i < sl; i++)
	{
		for (int j = 0; j < sl; j++)
		{
			for (int k = 0; k < sl; k++)
			{
				Cell c= hmap[hash(i, j, k)];

				std::cout << " 3D coords: [" << glm::to_string(c.pos);
				std::cout << "]: " << c.volume;
				std::cout << " is_solid: " << c.is_solid << std::endl;
			}
		}
	}
}
int		to1D(int x, int y, int z)
{
	x = x + hf_sl;
	z = z + hf_sl;
	int v = (sl * (sl / 4) * x) + sl * y + z;
	return (v);
}

glm::vec3		to3D(int n)
{
	int x = n / (sl * (sl / 4)) - hf_sl;
	n -= (x + hf_sl) * (sl * (sl / 4));
	int y = n / (sl);
	int z = n % (sl) - hf_sl;
	glm::vec3 f(x, y, z);
	return (f);
}

ssize_t HeightMap::hash(int i, int j, int k) const
{
	return ((ceil(i) + hf_sl) * (sl * (sl / 4.0)) + sl * ceil(j) + (ceil(k) + hf_sl));
}

ssize_t HeightMap::hash(glm::vec3 pos) const
{
	return static_cast<ssize_t>((ceil(pos.x) + hf_sl) * (sl * (sl / 4.0)) + sl * ceil(pos.y) + (ceil(pos.z) + hf_sl));
}

Cell& HeightMap::address(glm::vec3 pos)
{
	return (hmap[hash(pos)]);
}

Cell& HeightMap::address(int i, int j, int k)
{
	return (hmap[hash(i, j, k)]);
}

/*
 * Bound position to the grid world
 */
void HeightMap::bound(glm::vec3 &position)
{
	if (position.y < 0)
		position.y = 0;
	else if (position.y > hf_sl / 2.0f - 1)
		position.y = hf_sl / 2.0f - 1;

	if (position.z < -hf_sl)
		position.z = -hf_sl;
	else if (position.z > hf_sl - 1)
		position.z = hf_sl - 1;

	if (position.x < -hf_sl)
		position.x = -hf_sl;
	else if (position.x > hf_sl - 1)
		position.x = hf_sl - 1;
}

bool HeightMap::out_of_bound(const glm::vec3 &position) const
{
	// we handle below surface collision separatedly
//	if (position.y < 0 || position.y > hf_sl / 2.0f - 1)
//		return true;
	if (position.y > hf_sl / 2.0f - 1)
		return true;
	if (position.z < -hf_sl || position.z > hf_sl - 1)
		return (true);
	if (position.x < -hf_sl || position.x > hf_sl - 1)
		return true;
	return (false);
}

glm::vec3 HeightMap::normal(const glm::vec3 &position)
{
	Cell &c = address(position);
	glm::vec3 normal(-_dx(c.pos), 1.0f, -_dz(c.pos));
	glm::vec3 normed_normal = normal / glm::length(normal);
	return (normed_normal);
}

// This will fail on the boundaries, but is more straight-forward
float HeightMap::_dx(glm::vec3 &grid_position)
{
	float left_y = 0;
	float right_y = 0;
	bool  left_found, right_found;

	left_found = false;
	right_found = false;
	for (int i = hf_sl / 2 - 1; i >= 0; --i)
	{
		if (!left_found && address(grid_position.x - 1, i, grid_position.z).is_solid) {
			left_y = i;
			left_found = true;
		}
		if (!right_found && address(grid_position.x + 1, i, grid_position.z).is_solid) {
			right_y = i;
			right_found = true;
		}
		if (left_found && right_found)
			break ;
	}
	return (right_y - left_y) / 2.0f;
}

float HeightMap::_dz(glm::vec3 &grid_position)
{
	float left_y = 0;
	float right_y = 0;
	bool  left_found, right_found;

	left_found = false;
	right_found = false;
	for (int i = hf_sl / 2 - 1; i >= 0; --i)
	{
		if (!left_found && address(grid_position.x, i, grid_position.z - 1).is_solid) {
			left_y = i;
			left_found = true;
		}
		if (!right_found && address(grid_position.x, i, grid_position.z + 1).is_solid) {
			right_y = i;
			right_found = true;
		}
		if (left_found && right_found)
			break ;
	}
	return (right_y - left_y) / 2.0f;
}

///*
// * Find survace for the given y location
// */
//Cell& HeightMap::find_surface(glm::vec3 &position const)
//{
//	for (int i = 0; i < hf_sl / 2 - 2; ++i) {
//		if (address(position.x, i))
//	}
//}
