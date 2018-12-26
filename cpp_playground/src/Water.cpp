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

				std::cout << " 3D coords: [" << glm::to_string(c.pos);
				std::cout << "]: " << c.volume;
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
	Cell &c = this->address(x, z, y);
//	if (c.is_solid)
//	{
//		return ;
//	}
	c.volume += volume;
}

void	Water::update_particles()
{
	// To be 1000% sure that GL is finished it's work
	glFinish();
	// Swap buffers
	state = !state;

	clEnqueueAcquireGLObjects(cl.queue, 1, &cl_vbo, 0, NULL, NULL);
	clEnqueueAcquireGLObjects(cl.queue, 1, &cl_vbo2, 0, NULL, NULL);

	int err = 0;
	err = clSetKernelArg(cl.kernel, state ? 0 : 1, sizeof(cl_vbo), &cl_vbo);
	err = clSetKernelArg(cl.kernel, state ? 1 : 0, sizeof(cl_vbo2), &cl_vbo2);
	if (err != CL_SUCCESS) {
        std::cout << "Error: " << __LINE__ << "code: " << err << ".\n";
        exit(1);
    }

    size_t  global_work_size = sl * sl * sl;
    err = clEnqueueNDRangeKernel(cl.queue, cl.kernel, 1, NULL, &global_work_size, NULL, 0, NULL, NULL);
    if (err != CL_SUCCESS) {
        std::cout << "Error: " << __LINE__ << "code: " << err << ".\n";
        exit(1);
    }

	clEnqueueReleaseGLObjects(cl.queue, 1, &cl_vbo, 0, NULL, NULL);
	clEnqueueReleaseGLObjects(cl.queue, 1, &cl_vbo2, 0, NULL, NULL);

    clFinish(cl.queue);
}
