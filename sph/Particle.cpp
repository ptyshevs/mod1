
#include "Particle.hpp"

Particle::Particle()  : position(cl_float3()), velocity(cl_float3()), force(cl_float3()), density(0.0f), pressure(0.0f), type(P_DYNAMIC), n_neighbors(0) {};

Particle::Particle(const glm::vec3 &position, const glm::vec3 &velocity, const glm::vec3 &force) :
		position({position.x, position.y, position.z}), velocity({velocity.x, velocity.y, velocity.z}),
		force({force.x, force.y, force.z})
{
	density = 0.0f;
	pressure = 0.0f;
};

Particle::Particle(const glm::vec3 &position, const glm::vec3 &velocity, const glm::vec3 &force, float density,
				   float pressure, float viscosity, unsigned int type) : Particle(position, velocity, force)
{
	this->density = density;
	this->pressure = pressure;
	this->viscosity = viscosity;
	this->type = type;
	n_neighbors = 0;
}

void	Particle::show() const {
	printf("pos [%0.2f, %0.2f, %0.2f] ", this->position.x, this->position.y,
			this->position.z);
	printf("vel [%0.2f, %0.2f, %0.2f] ", this->velocity.x, this->velocity.y,
			this->velocity.z);
	printf("F [%0.2f, %0.2f, %0.2f]\n", this->force.x, this->force.y, this->force.z);
}

std::ostream& operator<<(std::ostream &o, const Particle &particle)
{
//	o << "pos [" << glm::to_string(particle.position) << "]";
//	o << " vel [" << glm::to_string(particle.velocity) << "]";
//	o << " F [" << glm::to_string(particle.force) << "]";
//	o << " D: " << particle.density << " P: " << particle.pressure;
	return (o);
}