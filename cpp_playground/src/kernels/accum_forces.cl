#include "simulation.hpp"

__constant float3 gravity = (float3)(0.0f, -9.81f, 0.0f);
// pow(K_RADIUS, 9)
__constant float k_const = 315.0f / (64.0f * M_PI * 38.443359375);
__constant float k_dconst = 45.0f / (M_PI * 11.390625);


///////////////////////// PROTOTYPES
float k_distance(float3 a, float3 b);
float k_weight(float d);
float k_first_derivative(float d);
float k_second_derivative(float d);



float k_distance(float3 a, float3 b) {
	return length(a - b);
}



float k_weight(float d) {
	if (d >= K_RADIUS)
		return (0.0f);
	float x = K_RADIUS - d * d;
	return (k_const * x * x * x);
}



float k_first_derivative(float d) {
	if (d >= K_RADIUS)
		return (0.0f);
	float x = K_RADIUS - d;
	return (-k_dconst * x * x);
}

float k_second_derivative(float d)
{
	if (d >= K_RADIUS)
		return (0.0f);
	return (k_dconst * (K_RADIUS - d));
}



// step 2: accumulate forces from pressure and viscosity
__kernel void accum_forces(__global t_constants *constants, __global t_cp *control_points, __global t_cell *hmap, __global t_particle *particles)
{
	size_t offset = get_global_id(0);
	__global t_particle *p = &particles[offset];
	for (unsigned int i=0; i < p->n_neighbors; ++i) {
		__global t_particle *np = &particles[p->neighbors[i]];

		float dist = k_distance(p->pos, np->pos);
		if (dist > 0) {
			float3 dir = (p->pos - np->pos) / dist; // normed vector
			float3 val = PARTICLE_MASS * (p->pressure + np->pressure) /
						(2 * p->density * np->density) * k_first_derivative(dist) * dir;

			p->force[0] -= val[0];
			p->force[1] -= val[1];
			p->force[2] -= val[2];

			float3 pvel = (float3)(p->vel[0], p->vel[1], p->vel[2]);
			float3 npvel = (float3)(np->vel[0], np->vel[1], np->vel[2]);
			val = VISCOSITY * (npvel - pvel) / np->density * k_second_derivative(dist);

			p->force[0] += val[0];
			p->force[1] += val[1];
			p->force[2] += val[2];
		}
	}
}
