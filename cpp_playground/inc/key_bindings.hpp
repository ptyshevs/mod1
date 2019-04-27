/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_bindings.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ptyshevs <ptyshevs@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/07 14:49:19 by ptyshevs          #+#    #+#             */
/*   Updated: 2019/01/07 14:49:33 by ptyshevs         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef KEY_BINDINGS_HPP
#define KEY_BINDINGS_HPP

#include <core.hpp>
#include <camera.hpp>
#include <Water.hpp>

/*
 * Process key press event on the SDL event queue
 */
void	process_input(GLCamera &camera, GLItem &map,  Water &water, bool *quit);


#endif
