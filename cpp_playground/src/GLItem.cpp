#include <core.hpp>

void GLItem::draw(const glm::mat4 &vp, GLenum type)
{
	glUseProgram(this->shader_program);
	this->fill_uniforms(vp);
	glBindVertexArray(this->vao);

	if (this->tex) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, this->tex);
	}

	if (this->tex_n) {
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, this->tex_n);
	}

	if (this->tex_ao) {
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, this->tex_ao);
	}

	if (this->tex_r) {
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, this->tex_r);
	}

	if (type == GL_POINTS)
	{
		try {
			auto dbitem = dynamic_cast<CLGLDoubleBufferedItem &>(*this);
			glBindBuffer(GL_ARRAY_BUFFER, dbitem.state ? dbitem.vbo : dbitem.vbo2);
		} catch (const std::bad_cast& e) { }
		glDrawArrays(GL_POINTS, 0, this->idx_num);
	} else
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ibo);
		glDrawElements(type, this->idx_num, GL_UNSIGNED_INT, NULL);
	}
	glBindVertexArray(0);
}