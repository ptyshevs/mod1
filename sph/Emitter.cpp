/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Emitter.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ptyshevs <ptyshevs@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/10 15:41:36 by ptyshevs          #+#    #+#             */
/*   Updated: 2019/01/10 15:41:37 by ptyshevs         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Emitter.hpp"

Emitter::Emitter(ParticleSystemData &data) : _data(data),
	_velocity(glm::vec3(0.0f)), _force(glm::vec3(0.0f)), _density(0.0f), _pressure(0.0f), _viscosity(VISCOSITY),
	_step(0.5f), _scale(0.3f), _inflate(1.4f), point_type(P_DYNAMIC), _id(0)
{
}

void Emitter::setVelocity(const glm::vec3 &velocity)
{
	_velocity = velocity;
}

void Emitter::setForce(const glm::vec3 &force)
{
	_force = force;
}

void Emitter::setDensity(float density)
{
	_density = density;
}

void Emitter::setPressure(float pressure)
{
	_pressure = pressure;
}

void Emitter::setViscosity(float viscosity)
{
	_viscosity = viscosity;
}

void Emitter::setId(unsigned int id)
{
	_id = id;
}

void	Emitter::setStep(float step) {
	_step = step;
}

void	Emitter::setScale(float scale) {
	_scale = scale;
}

void	Emitter::setInflate(float inflate) {
	_inflate = inflate;
}

void	Emitter::setPointType(unsigned int type) {
	point_type = type;
}

void	Emitter::cuboid(float x_start, float x_end,
		float y_start, float y_end, float z_start, float z_end)
{
	for (float i = x_start; i <= x_end; i += _step) {
		for (float j = y_start; j <= y_end; j += _step) {
			for (float k = z_start; k <= z_end; k += _step) {
				glm::vec3 pos(i, j, k);
				_data.addParticle(pos, _velocity, _force, _density, _pressure, _viscosity, point_type, _id);
			}
		}
	}
}

void	Emitter::cube(const glm::vec3 &origin, float side) {
	for (float i = origin.x; i < origin.x + side; i += _step) {
		for (float j = origin.y; j < origin.y + side; j += _step) {
			for (float k = origin.z; k < origin.z + side; k += _step) {
				glm::vec3 pos(i, j, k);
				_data.addParticle(pos, _velocity, _force, _density, _pressure, _viscosity, point_type, _id);
			}
		}
	}
}

/*
 * Wall is a dense static cuboid
 */
void Emitter::wall(float x_start, float x_end, float y_start, float y_end, float z_start, float z_end)
{
	auto save_step = _step;
	auto save_ptype = point_type;
	auto save_viscosity = _viscosity;
	setStep(.25);
	setPointType(P_STATIC);
	setViscosity(0.4);
	cuboid(x_start, x_end, y_start, y_end, z_start, z_end);
	setStep(save_step);
	setPointType(save_ptype);
	setViscosity(save_viscosity);
}

/*
 * Pillow is a static cilinder
 */
void Emitter::pillow(glm::vec3 origin, float width, float radius, float height)
{
	for (float angle = 0; angle < 2 * M_PI; angle += 0.1) {
		for (float j = 0; j < width; j += _step) {
			glm::vec3 pos(origin.x + (radius - j) * cos(angle), 0, origin.z + (radius - j) * sin(angle));
			for (float i = 0; i < height; i += _step) {
				pos.y = i;
				_data.addParticle(pos, _velocity, _force, _density, _pressure, _viscosity, point_type, _id);
			}
		}
	}
}

/*
 * Sphere is ... well, a sphere
 */
void Emitter::sphere(glm::vec3 origin, float width, float radius)
{
	for (float t = -M_PI; t < M_PI; t += _step) {
		for (float p = -M_PI; p < M_PI; p += _step) {
			glm::vec3 pos(origin.x + _inflate * radius * cos(t) * cos(p), origin.y + _inflate * radius * sin(t), origin.z + _inflate * radius * cos(t) * sin(p));
			_data.addParticle(pos, _velocity, _force, _density, _pressure, _viscosity, point_type, _id);
		}
	}
}

void rescale(std::vector<glm::vec3> &points) {
	float x_max, y_max, z_max;

	x_max = INT_MIN;
	y_max = INT_MIN;
	z_max = INT_MIN;

	for (glm::vec3 cpoint: points)
	{
		if (cpoint.x > x_max)
			x_max = cpoint.x;
		if (cpoint.y > y_max)
			y_max = cpoint.y;
		if (cpoint.z > z_max)
			z_max = cpoint.z;
	}
	for (auto &point : points)
	{
		point.x /= x_max;
		point.y /= y_max;
		point.z /= z_max;
	}
}

void scale_back(std::vector<glm::vec3> &points) {
	for (glm::vec3 &cp: points)
	{
		cp *= (float)hf_sl; // scaling
		cp.x -= (float)hf_sl; // centering
		cp.z -= (float)hf_sl;
		cp.y -= (float)hf_sl - 25;
//		cp.y += (float)hf_sl; // scale down height
//		cp.y *= 0.3; // scale height more
	}
}

glm::vec3	centroid(std::vector<glm::vec3> const &points) {
	glm::vec3 center(0);
	for (auto const &point: points) {
		center += point;
	}
	return (center / points.size());
}

void Emitter::fromFile(std::string const &path)
{
	std::vector<glm::vec3> points = readFile((char *)path.c_str());
	rescale(points);
	scale_back(points);
	for (auto const &v: points) {
		_data.addParticle(_scale * v, _velocity, _force, _density, _pressure, point_type);
	}
}

void Emitter::fromFile(glm::vec3 const &origin, std::string const &path)
{
	std::vector<glm::vec3> points = readFile((char *)path.c_str());
	rescale(points);
	scale_back(points);
	glm::vec3 center = centroid(points);
	for (auto const &v: points) {
		_data.addParticle(origin + _scale * v + _inflate * (v - center), _velocity, _force, _density, _pressure, _viscosity,
				point_type, _id);
	}
}


void Emitter::prepare_emit(CLCore &core) {
    device = core.device;
    context = core.context;
    queue = core.queue;

    cl_compile_kernel(*this, "src/kernels/emit_kernel.cl", "emit_kernel");

    int err = 0;
    cl_emitter = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(t_emiter), NULL, &err);
    if (err != CL_SUCCESS) {
        std::cout << "Error: " << __FILE__ << " " << __LINE__ << "code: " << err << ".\n";
        exit(1);
    }

    pps = 100; // Once in a sec
    type = EMITER_RAIN;

    srand(time(NULL));
}

static const size_t  global_work_size = 1;

void Emitter::emit()
{
	static int emit_calls = 0;
    int err = 0;

    t_emiter emiter = (t_emiter){ (cl_int)type, (cl_uint)pps, (cl_uint)rand() };
    err = clEnqueueWriteBuffer(queue, cl_emitter, CL_TRUE, 0, sizeof(t_emiter), &emiter, 0, NULL, NULL);
    if (err != CL_SUCCESS) {
        std::cout << "Error: " << __FILE__ << " " << __LINE__ << "code: " << err << ".\n";
        exit(1);
    }

	err = clSetKernelArg(kernel, 0, sizeof(cl_vbo), &cl_vbo);
    err |= clSetKernelArg(kernel, 1, sizeof(cl_emitter), &cl_emitter);
    err |= clSetKernelArg(kernel, 2, sizeof(cl_constants), &cl_constants);
	if (err != CL_SUCCESS) {
        std::cout << "Error: " << __FILE__ << " " << __LINE__ << "code: " << err << ".\n";
        exit(1);
    }

    err = clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &global_work_size, NULL, 0, NULL, NULL);
    if (err != CL_SUCCESS) {
        std::cout << "Error: " << __LINE__ << "code: " << err << ".\n";
        exit(1);
    }
    emit_calls++;
    if (emit_calls % 300 == 0 && emit_calls < MAX_PARTICLES)
    	std::cout << "# of emission calls: " << emit_calls << std::endl;
}