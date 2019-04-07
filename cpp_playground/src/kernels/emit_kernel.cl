#include "simulation.hpp"

#define RAIN_PPI 4

#define hf_hf_sl 50

/*
*/
typedef struct {
	int type;
	uint pps;
	uint seed;
} t_emiter;


/*
*/
void it_s_raining_man(__global t_particle *particles, size_t seed, __global t_constants *constants);

void havaji(__global t_particle *particles, __global t_constants *constants);

void boundaries(__global t_particle *particles, __global t_constants *constants);

/*
*/

size_t place_in_corner_with_offset(__global t_particle *particles, size_t offset, size_t y_offset);

size_t to_address(float x, float y, float z);

size_t rand(size_t seed);

/*
*/
__kernel void emit_kernel(__global t_particle *particles,
						  __global t_emiter *emiter,
						  __global t_constants *constants)
{
	if (constants->n_particles >= MAX_PARTICLES)
		return ;
	const t_emiter _emiter = emiter[0];


	if (_emiter.type == EMITER_RAIN)
		it_s_raining_man(particles, (size_t)_emiter.seed, constants);
	else if (_emiter.type == EMITER_WAVE)
		havaji(particles, constants);
	else if (_emiter.type == EMITER_BOUNDARIES)
		boundaries(particles, constants);
}

void it_s_raining_man(__global t_particle *particles, size_t seed, __global t_constants *constants)
{
	for (size_t i = 0; i < RAIN_PPI; i++) {
		float foo = ((float)(seed % sl)) - hf_sl;
		float foo2 = ((float)(rand(seed) % sl)) - hf_sl;

		size_t offset = to_address((float)foo,
								(float)hf_hf_sl - 1,
								(float)foo2);
		t_particle p;
		p.pos.x = foo;
		p.pos.z = foo2;
		p.pos.y = hf_hf_sl - 1;
		particles[constants->n_particles] = p;
		constants->n_particles += 1;
	}
}

void havaji(__global t_particle *particles, __global t_constants *constants)
{
	int cnt = 0;
	int store_n = constants->n_particles;
	for (int i = -hf_sl; i < hf_sl; i++) {
		if (store_n + cnt >= MAX_PARTICLES)
			break ;
		t_particle p;
		p.pos.x = -hf_sl;
		p.pos.z = i;
		p.pos.y = 1;
		particles[store_n + cnt] = p;
		cnt += 1;
	}
	constants->n_particles += cnt;
}

void boundaries(__global t_particle *particles, __global t_constants *constants)
{
	int cnt = 0;
	int store_n = constants->n_particles;
	for (int i = 0; i < sl; i ++)
	{
		if (store_n + cnt >= MAX_PARTICLES)
			break ;
		t_particle p;
		p.pos = (float3)(i - (float)hf_sl, 1, -(float)hf_sl);
		particles[store_n + cnt] = p;
		cnt++;
		if (store_n + cnt >= MAX_PARTICLES)
			break ;
		p.pos = (float3)(-(float)hf_sl, 1, i - (float)hf_sl);
		particles[store_n + cnt] = p;
		cnt++;
		if (store_n + cnt >= MAX_PARTICLES)
			break ;
		p.pos = (float3)((float)hf_sl - 1, 1, i - (float)hf_sl);
		particles[store_n + cnt] = p;
		cnt++;
		if (store_n + cnt >= MAX_PARTICLES)
			break ;
		p.pos = (float3)(i - (float)hf_sl, 1, (float)hf_sl - 1);
		particles[store_n + cnt] = p;
		cnt++;
	}
	constants->n_particles += cnt;
}



size_t to_address(float x, float y, float z)
{
	return ((x + hf_sl) * sl * (sl / 4) + y * sl + (z + hf_sl));
}

size_t rand(size_t seed)
{
	uint t = seed ^ (seed << 11);
	return (seed ^ (seed >> 19) ^ (t ^ (t >> 8)));
}
