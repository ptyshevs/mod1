#include "simulation.hpp"


__constant float3 gravity = (float3)(0.0f, -9.81f, 0.0f);
// pow(K_RADIUS, 9)
__constant float k_const = 315.0f / (64.0f * M_PI * 38.443359375);
__constant float k_dconst = 45.0f / (M_PI * 11.390625);


///////////////////////// PROTOTYPES
float k_distance(float3 a, float3 b);
float kernel_weight(float d);
bool  bound(__global float3 *pos);
float surface_height(__global t_constants *constants, __global t_cp *control_points, float3 pos);
float3 surface_normal(__global t_constants *c, __global t_cp *cp, float3 pos);
float3 surface_collision(__global t_constants *constants, __global t_cp *control_points, float3 pos, float3 vel);




float k_distance(float3 a, float3 b) {
	return length(a - b);
}



float kernel_weight(float d) {
	if (d >= K_RADIUS)
		return (0.0f);
	float x = K_RADIUS - d * d;
	return (k_const * x * x * x);
}

bool bound(__global float3 *pos) {
	bool out_of_bound = false;
	if (pos->y < 0) {
		pos->y = 0;
		out_of_bound = true;
	}
	else if (pos->y > hf_sl / 2.0f - 1) {
		pos->y = hf_sl / 2.0f - 1;
		out_of_bound = true;
	}

	if (pos->z < -hf_sl) {
		pos->z = -hf_sl;
		out_of_bound = true;
	}
	else if (pos->z > hf_sl - 1) {
		pos->z  = hf_sl - 1;
		out_of_bound = true;
	}

	if (pos->x < -hf_sl) {
		pos->x =  -hf_sl;
		out_of_bound = true;
	}
	else if (pos->x > hf_sl - 1) {
		pos->x = hf_sl - 1;
		out_of_bound = true;
	}
	return (out_of_bound);
}


// idw interpolation, same as what was used to generate surface, to find surface height at point accurately

float surface_height(__global t_constants *constants, __global t_cp *control_points, float3 pos) {
	float num = 0;
	float denum = 0;
	float w, sqsum;
	float3 diff;
	for (unsigned int i = 0, c = constants->n_cp; i < c; ++i) { // todo: how about using control points only on the neighbour cells in hmap? it has position pre-calculated already
		w = 0;
		float3 cppos = (float3)(control_points[i].x, control_points[i].y, control_points[i].z);
		diff = pos - cppos;
		diff *= diff;
		sqsum = diff.x + diff.z;
		w = 1.0f / (pow(sqsum, 1.3f) + 0.0001f);
		num += w * cppos.y;
		denum += w;
	}
	return (num / denum);
}

float3 surface_normal(__global t_constants *c, __global t_cp *cp, float3 pos) {
	float dx = surface_height(c, cp, (float3)(pos.x + 0.5f, pos.y, pos.z)) - surface_height(c, cp, (float3)(pos.x - 0.5f, pos.y, pos.z));
	float dz = surface_height(c, cp, (float3)(pos.x, pos.y, pos.z + 0.5f)) - surface_height(c, cp, (float3)(pos.x, pos.y, pos.z - 0.5f));
	return normalize((float3)(-dx, 1.0f, -dz));
}

float3 surface_collision(__global t_constants *constants, __global t_cp *control_points, float3 pos, float3 vel) {
	float3 normal = surface_normal(constants, control_points, pos);
	float vel_mag = length(vel);
	float3 vel_normalized = vel / vel_mag;
	float3 reflected = vel_normalized - (2 * dot(vel_normalized, normal) * normal);
	float3 renormalized = reflected * vel_mag;
	return renormalized - renormalized * RESTITUTION * fabs(dot(vel_normalized, normal));
}

// step 2: accumulate forces from pressure and viscosity
__kernel void integrate_resolve(__global t_constants *constants, __global t_cp *control_points, __global t_cell *hmap, __global t_particle *particles)
{
	size_t offset = get_global_id(0);
	__global t_particle *p = &particles[offset];
	float3 val = (float3)(p->force[0], p->force[1], p->force[2]);
	val = TIME_STEP * val / PARTICLE_MASS;

	p->vel.x += val.x;
	p->vel.y += val.y;
	p->vel.z += val.z;

	p->pos.x += TIME_STEP * p->vel.x;
	p->pos.y += TIME_STEP * p->vel.y;
	p->pos.z += TIME_STEP * p->vel.z;

	// Collision resolution
	if (bound(&p->pos)) {
		p->vel[0] *= -DAMPING;
		p->vel[1] *= -DAMPING;
		p->vel[2] *= -DAMPING;
	}
	// No boundary crossing, maybe there's a surface?
	float h = surface_height(constants, control_points, p->pos);
	if (h > p->pos.y)
	{
		float3 new_vel = surface_collision(constants, control_points, p->pos, p->vel);
		p->vel.x = new_vel.x;
		p->vel.y = new_vel.y;
		p->vel.z = new_vel.z;
		p->pos.y = h;
	}
}
