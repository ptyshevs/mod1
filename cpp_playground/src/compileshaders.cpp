/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpopovyc <vpopovyc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/09 16:41:59 by vpopovyc          #+#    #+#             */
/*   Updated: 2018/12/17 19:24:08 by vpopovyc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <core.hpp>

char	*reader(const char *path)
{
	FILE	*fp;
	long	size;
	char	*data;

	assert((fp = fopen(path, "r")));
	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	rewind(fp);
	data = (char*)malloc(size + 1);
	data[size] = 0;
	fread(data, size, 1, fp);
	fclose(fp);
	return (data);
}

void	print_error_log(GLuint shader, GLchar *log)
{
	glGetShaderInfoLog(shader, 512, NULL, log);
	perror(log);
}

void	check_compile_errors(GLuint shader_program,
								GLuint fragment_shader, GLuint vertex_shader)
{
	GLint	success;
	GLchar	log[512];

	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
	if (!success)
		print_error_log(vertex_shader, log);
	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
	if (!success)
		print_error_log(fragment_shader, log);
	glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
	if (!success)
		print_error_log(fragment_shader, log);
}

GLuint	compile_shaders(const char *vert_fpath, const char *frag_fpath)
{
	const GLchar	*vert_shader;
	const GLchar	*frag_shader;
	GLuint			fragment_shader;
	GLuint			vertex_shader;
	GLuint			shader_program;

	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	vert_shader = reader(vert_fpath);
	glShaderSource(vertex_shader, 1, &vert_shader, NULL);
	glCompileShader(vertex_shader);
	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	frag_shader = reader(frag_fpath);
	glShaderSource(fragment_shader, 1, &frag_shader, NULL);
	glCompileShader(fragment_shader);
	shader_program = glCreateProgram();
	glAttachShader(shader_program, vertex_shader);
	glAttachShader(shader_program, fragment_shader);
	glLinkProgram(shader_program);
	check_compile_errors(shader_program, fragment_shader, vertex_shader);
	glDeleteShader(fragment_shader);
	glDeleteShader(vertex_shader);
	free(*(void**)&vert_shader);
	free(*(void**)&frag_shader);
	return (shader_program);
}