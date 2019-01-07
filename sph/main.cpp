/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ptyshevs <ptyshevs@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/07 10:55:10 by ptyshevs          #+#    #+#             */
/*   Updated: 2019/01/07 10:55:12 by ptyshevs         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sph.hpp>




int main(int ac, char **av)
{
	ParticleSystemData data;
	data.addParticle(glm::vec3(0));
	data.addParticle(glm::vec3(1));
	std::cout << data.numOfParticles() << std::endl;

}