/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HeightMap.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ptyshevs <ptyshevs@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/07 14:49:09 by ptyshevs          #+#    #+#             */
/*   Updated: 2019/01/07 14:49:16 by ptyshevs         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEIGHTMAP_H
#define HEIGHTMAP_H

#include <core.hpp>
#include <ControlPoints.hpp>

class HeightMap: public GLItem {
public:
	HeightMap() {};
	std::vector<Cell>	hmap;

	void show() const;
	ssize_t		hash(int i, int j, int k) const;
};

HeightMap generate_map(const ControlPoints &control_points);

#endif
