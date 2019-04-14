//
// Created by Pavlo TYSHEVSKYI on 2019-04-14.
//

#ifndef PROJECT_SHADER_HPP
#define PROJECT_SHADER_HPP

#include <core.hpp>

/*
 * One step of water rendering pipeline
 */

	class WaterShader {
public:
	GLuint program;
	GLuint tex;
	GLuint fbo;
	GLuint vao;
	GLuint vbo;

	WaterShader(const GLchar *vertexPath, const GLchar *fragmentPath);

	virtual ~WaterShader();

	void setMatrix(const glm::mat4 &m, const GLchar *name);
	void bindVBO(GLuint &vbo, size_t offset);
};

#endif //PROJECT_SHADER_HPP
