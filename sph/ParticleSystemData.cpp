/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParticleSystemData.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ptyshevs <ptyshevs@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/07 10:55:22 by ptyshevs          #+#    #+#             */
/*   Updated: 2019/01/07 10:55:27 by ptyshevs         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ParticleSystemData.hpp"

static int offsets[27][3] = {
							{0, 0, 0},
							{-1, 0, 0},
							{0, -1, 0},
							{0, 0, -1},
							{0, 0, 1},
							{0, 1, 0},
							{1, 0, 0},
							{-1, -1, 0},
							{-1, -1, 1},
							{-1, 0, -1},
							{-1, 0, 1},
							{-1, 1, -1},
							{-1, 1, 0},
							{-1, 1, 1},
							{0, -1, -1},
							{0, -1, 1},
							{0, 1, -1},
							{0, 1, 1},
							{1, -1, -1},
							{1, -1, 0},
							{1, -1, 1},
							{1, 0, -1},
							{1, 0, 1},
							{1, 1, -1},
							{1, 1, 0},
							{1, 1, 1},
							{-1, -1, -1}};

ParticleSystemData::ParticleSystemData(size_t numOfParticles) : _mass(PARTICLE_MASS), _gravity(glm::vec3(0.0, -9.81, 0.0))
{
	if (numOfParticles > 0)
	{
		this->_particles.reserve(numOfParticles);

	}
}

size_t ParticleSystemData::numOfParticles() const {
	return (this->_particles.size());
}

void ParticleSystemData::addParticle(const glm::vec3 &position, const glm::vec3 &velocity, const glm::vec3 &force) {
	this->_particles.emplace_back(position, velocity, force);
}

void ParticleSystemData::addParticle(const glm::vec3 &position, const glm::vec3 &velocity, const glm::vec3 &force,
									  float density, float pressure)
{
	this->_particles.emplace_back(position, velocity, force, density, pressure);
}


void ParticleSystemData::addParticles(std::vector<glm::vec3> positions, std::vector<glm::vec3> velocities,
									  std::vector<glm::vec3> forces) {
	for (size_t i = 0; i < positions.size(); ++i) {
		this->_particles.emplace_back(positions[i], velocities[i], forces[i]);

	}
}

const float &ParticleSystemData::mass() const
{
	return (this->_mass);
}

const glm::vec3& ParticleSystemData::gravity() const
{
	return (this->_gravity);
}

Particle& ParticleSystemData::operator[](size_t i)
{
	return (this->_particles[i]);
}

void ParticleSystemData::show(ssize_t i)
{
	if (i >= 0)
		return (*this)[i].show();
	for (size_t j = 0; j < this->numOfParticles(); ++j)
	{
		Particle &particle = (*this)[j];
		std::cout << j << ": " << particle << std::endl;
	}
}


// Naive implementation of neighbors list building
// Spacial hashing should be here instead, probably with different grid
void ParticleSystemData::cacheNeighbors()
{
	fill_hmap();
	for (auto &p: _particles)
	{
		p.n_neighbors = 0;
//		std::cout << _particles[i] << std::endl;
//		std::cout << "hmap pos:" << glm::to_string(hmap->address(_particles[i].position).pos) << std::endl;
//		std::cout << "3d idx: " << glm::to_string(hmap->pos_to_3D_idx(_particles[i].position)) << std::endl;
//		auto const &cell = hmap->address(_particles[i].position);
		glm::vec3 cell_pos = hmap->pos_to_3D_idx(p.position);
		for (auto const &offset: offsets)
		{
			if (p.n_neighbors == MAX_NEIGHBORS)
				break ;
			glm::vec3 offset_pos(cell_pos[0] + offset[0], cell_pos[1] + offset[1], cell_pos[2] + offset[2]);
			if (hmap->out_of_bound(offset_pos))
				continue ;
			auto const &neigh_cell = hmap->address(offset_pos);
			for (size_t j = 0; j < neigh_cell.n_inside; ++j) {
				auto &np = neigh_cell.particles[j];
				if (&p != np) {
				// calculate distance, and if close enough, add to neighbor list
					if (distance(p.position, np->position) < NEIGHBOR_RADIUS && p.n_neighbors < MAX_NEIGHBORS) {
						p.neighbors[p.n_neighbors] = np;
						p.n_neighbors += 1;
						if (p.n_neighbors == MAX_NEIGHBORS) {
//							std::cout << "Max # of neighbors reached" << std::endl;
//							p.show();
							break ;
						}
					}
				}
			}
		}
	}
}

void	ParticleSystemData::fill_hmap()
{
	// step 1: clear non-empty cells
	for (auto const &idx: hmap->nempty_cells) {
		std::fill(hmap->hmap[idx].particles, hmap->hmap[idx].particles + MAX_PER_CELL, nullptr);
		hmap->hmap[idx].n_inside = 0;
	}
	hmap->nempty_cells.clear();
	// step 2: add particles to cells they occupy
	for (auto &p: _particles)
	{
		auto idx = hmap->hash(p.position);
		auto &cell = hmap->hmap[idx];
		if (cell.n_inside < MAX_PER_CELL)
		{
			cell.particles[cell.n_inside] = &p;
			cell.n_inside += 1;
			hmap->nempty_cells.insert(idx);
//			if (cell.n_inside > cnt_max)
//				cnt_max = cell.n_inside;
		}
	}
//	std::cout << "Max p in cell=" << cnt_max << std::endl;
}


void	ParticleSystemData::update_densities() {
	for (auto &p: _particles)
	{
		float sum = 0;
		for (size_t j=0, m=p.n_neighbors; j < m; ++j) {
			float dist = distance(p.position, p.neighbors[j]->position);
			sum += kernel.weight(dist);
		}
		p.density = _mass * (sum + kernel.weight(0));
	}
}

void 	ParticleSystemData::compute_pressure(bool clamp_negative) {
	for (auto &p: _particles) {
		p.pressure = PRESSURE_CONST * (p.density - TARGET_DENSITY);
		if (clamp_negative && p.pressure < 0)
			p.pressure *= -NEGATIVE_PRESSURE_SCALE;
	}
}

// Add negative pressure gradient to the force
void	ParticleSystemData::add_pressure() {
	for (auto &p: _particles) {
		for (size_t j=0, m=p.n_neighbors; j < m; ++j) {
			float dist = distance(p.position, p.neighbors[j]->position);
			if (dist > 0) {
				glm::vec3 dir = (p.position - p.neighbors[j]->position) / dist; // normed vector
				glm::vec3 val = _mass * (p.pressure + p.neighbors[j]->pressure) /
						(2 * p.density * p.neighbors[j]->density) * kernel.first_derivative(dist) * dir;
//				std::cerr << glm::to_string(val) << std::endl;
				p.force -= val;
			}
		}
	}
}

void	ParticleSystemData::add_viscosity() {
	for (auto &p: _particles) {
		for (size_t j=0, m=p.n_neighbors; j < m; ++j) {
			float dist = glm::distance(p.position, p.neighbors[j]->position);
			// maybe need to multiply on the unit vector of direction from one particle to it's neighbor
			p.force += VISCOSITY * (p.neighbors[j]->velocity - p.velocity) /
					p.neighbors[j]->density * kernel.second_derivative(dist);
		}
	}
}

float ParticleSystemData::distance(const glm::vec3 &a, const glm::vec3 &b) const
{
	return glm::distance(a, b);
}

