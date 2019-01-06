//
// Created by Pavlo TYSHEVSKYI on 1/6/19.
//

#ifndef CONTROL_POINTS_HPP
#define CONTROL_POINTS_HPP

#include <core.hpp>

struct ControlPoints {
private:
	void	_rescale();
	void	_scale_back();
	void	_add_borders();
public:
	std::vector<glm::vec3> _arr;

	ControlPoints();
	ControlPoints &operator=(const std::vector<glm::vec3> &m);
	void	prepare(bool borders);
	void 	show();
};

#endif //CONTROL_POINTS_HPP
