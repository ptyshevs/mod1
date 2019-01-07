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
				Cell c= this->hash(i, j, k);

				std::cout << " 3D coords: [" << glm::to_string(c.pos);
				std::cout << "]: " << c.volume;
				std::cout << " is_solid: " << c.is_solid << std::endl;
			}
		}
	}
}

ssize_t HeightMap::hash(int i, int j, int k) const
{
	return (i * sl * sl + j * sl + k);
}