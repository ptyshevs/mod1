/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Water.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ptyshevs <ptyshevs@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/25 21:14:47 by ptyshevs          #+#    #+#             */
/*   Updated: 2018/12/25 21:14:48 by ptyshevs         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "core.hpp"

void	Water::show_hmap()
{
	for (int i = 0; i < sl; i++)
	{
		for (int j = 0; j < sl; j++)
		{
			for (int k = 0; k < sl; k++)
			{
				Cell c= this->address(i, j, k);

				std::cout << "id: " << c.id;
				std::cout << " 3D coords: [" << i << " " << j << " " << k << "] ";
				std::cout << " is_solid: " << c.is_solid << std::endl;
			}
		}

	}
}

glm::vec3	Water::to_coords(int x, int y, int z)
{
	return (glm::vec3(x, y, z) - (float)hf_sl);
}

Cell	&Water::address(int x, int y, int z)
{
	return (this->hmap[x * sl * sl + y * sl + z]);
}

void	Water::add_volume(int x, int y, int z, float volume)
{
	Cell &c = this->address(x, y, z);
	if (c.is_solid)
		return ;
	c.volume += volume;
}

void	Water::update_particles()
{
	for (glm::ivec3 &particle: this->particles)
	{
		if (particle.y > -hf_sl)
			particle.y -= 1; // gravity
	}
	this->update_buffer();
}

void	Water::update_buffer()
{
	glBindVertexArray(this->vao);
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
	glBufferData(GL_ARRAY_BUFFER, this->particles.size() * sizeof(glm::vec3),
			this->particles.data(), GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(glm::ivec3),
			this->indices.data(), GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, sizeof(glm::vec3), 0);
	glBindVertexArray(0);
}
