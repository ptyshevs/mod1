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
	data.addParticle(glm::vec3(1));
//	data.addParticle(glm::vec3(1));
	std::cout << data.numOfParticles() << std::endl;
	ParticleSystemSolver solver(data);
	for (int i = 0; i < 100; ++i)
	{
		std::cout << "Time step: " << i << std::endl;
		solver.simulation_step();
		data.show();
	}
}