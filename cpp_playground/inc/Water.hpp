/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Water.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ptyshevs <ptyshevs@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/07 14:49:36 by ptyshevs          #+#    #+#             */
/*   Updated: 2019/01/07 14:49:43 by ptyshevs         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WATER_H
#define WATER_H


#include <core.hpp>
#include <HeightMap.hpp>
#include <ParticleSystemData.hpp>
#include <ParticleSystemSolver.hpp>

struct WaterConstants {
	unsigned int	n_control_points;
	unsigned int 	hmap_size;
	unsigned int	n_particles;
	unsigned int	n_non_empty_cells;
	unsigned int	non_empty_cells[MAX_NONEMPTY_CELLS];
};

/*
 * This is the main structure for water simulation, that contains:
 * 1) Particle System Solver
 * 2) Particle System Data
 */
struct Water: public GLItem
{
private:
	void					_updateBuffer();
public:
	explicit Water();
	GLuint					vao2;
	GLuint					vbo2; // second VBO used for ping-pong technique
	bool					state;
	cl_mem 					cl_zidx;

	CLWaterCore				cl;
	cl_mem					cl_cp;
	cl_mem					cl_hmap;
	cl_mem					cl_vbo; // particles
	cl_mem					cl_vbo2; // second buffer for particles
	cl_mem					cl_constants;
	cl_mem					cl_indices;
	WaterConstants			constants;

	HeightMap				*hmap;
	ParticleSystemData		*data;
	ParticleSystemSolver	*solver;
	std::vector<glm::vec3>	indices;
//	Emiter emiter;
	virtual void	draw(const glm::mat4 &vp, GLenum type) override;
	virtual ~Water() override;
	void	update_particles();
//	void	emit();
};


Water	instance_water(HeightMap *hmap, ParticleSystemData *data);

#endif