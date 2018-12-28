/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   textures.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpopovyc <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/25 20:42:09 by vpopovyc          #+#    #+#             */
/*   Updated: 2018/03/25 20:42:10 by vpopovyc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <core.hpp>

static unsigned char	*tex_loader(const char *tex_file_name, unsigned int *width, unsigned int *height)
{
	int				error;
	unsigned char	*png;

	*width = 0;
	*height = 0;
	png = NULL;
	error = lodepng_decode32_file(&png, width, height, tex_file_name);
	if (error)
		exit(-1);
	return (png);
}

static GLuint			load_to_opengl(int width, int height, unsigned char *png)
{
	GLuint	tex_id;

	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &tex_id);
	glBindTexture(GL_TEXTURE_2D, tex_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
											GL_BGRA, GL_UNSIGNED_BYTE, png);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
											GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	return (tex_id);
}

GLuint	load_texture(const char *tex_file_name)
{
	unsigned int	width;
	unsigned int	height;
	unsigned char	*png;

	png = tex_loader(tex_file_name, &width, &height);
	return load_to_opengl(width, height, png);
}