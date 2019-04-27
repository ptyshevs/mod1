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
#include <Emitter.hpp>

struct WaterConstants {
	unsigned int	n_control_points;
	unsigned int 	hmap_size;
	unsigned int	n_particles;
};

/*
 * This is the main structure for water simulation, that contains:
 * 1) Particle System Solver
 * 2) Particle System Data
 */
struct Water: public GLItem
{
public:
	explicit Water();

	WaterConstants			constants;
	GLuint					static_program;

	CLWaterCore				cl;
	cl_mem					cl_cp;
	cl_mem					cl_hmap;
	cl_mem					cl_vbo; // particles
	cl_mem					cl_constants;

	HeightMap				*hmap;
	ParticleSystemData		*data;

	bool					running;
	bool					emitting;
	Emitter					*emitter;

	~Water() override;
	/*
	 * simulation step update
	 */
	void					update_particles();
	/*
	 * Main emit routine
	 */
	void					emit();
	/*
	 * Overridden function to render water
	 */
	void					draw(const glm::mat4 &vp, GLenum type) override;
};

/*
 * Main routine to instantiate Water object properly. It also set-ups all necessary
 * fields for GPU simulation and rendering
 */
Water	instance_water(HeightMap *hmap, ParticleSystemData *data, Emitter *emitter);

#endif