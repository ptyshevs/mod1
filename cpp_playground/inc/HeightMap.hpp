//
// Created by Pavlo TYSHEVSKYI on 1/7/19.
//

#ifndef HEIGHTMAP_H
#define HEIGHTMAP_H

#include <core.hpp>
#include <ControlPoints.hpp>

class HeightMap: public GLItem {
public:
	HeightMap() {};
	std::vector<Cell>	hmap;

};

HeightMap generate_map(const ControlPoints &control_points);

#endif //HEIGHTMAP_H
