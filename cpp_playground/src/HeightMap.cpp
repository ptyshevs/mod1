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
	glm::vec3 normal(-_dx(position), 1.0f, -_dz(position));
	glm::vec3 normed_normal = normal / glm::length(normal);
	return (normed_normal);
}

float HeightMap::_dx(const glm::vec3 &pos)
{
	return (interpolate(pos.x + 0.5f, pos.z) - interpolate(pos.x - 0.5f, pos.z));
}

float HeightMap::_dz(const glm::vec3 &pos)
{
	return (interpolate(pos.x, pos.z + 0.5f) - interpolate(pos.x, pos.z - 0.5f));
}

/*
 * Find survace for the given y location
 */
glm::vec3 HeightMap::closest_surface_point(glm::vec3 &point) const
{
	float y = interpolate(point.x, point.z);
	return (glm::vec3(point.x, y, point.z));
}

float HeightMap::surface_height(const glm::vec3 &point) const {
	return (interpolate(point));
}


float HeightMap::interpolate(float x, float z) const
{

	return (_cpoints.idw(x, z));
}

float HeightMap::interpolate(const glm::vec3 &point) const {
	return _cpoints.idw(point);
}
