
#ifndef SHAPE_HPP
#define SHAPE_HPP

#include <GL/glew.h>

#include <spdlog/spdlog.h>

struct Triangle {
	uint32_t VAO;
	uint32_t VBO;
	uint32_t shader;
	GLfloat vertices[9] = {
		-0.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		0.0f, 0.5f, 0.5f
	};

	void draw_triangle();
	void init();
	void add_shader(uint32_t program, const char* shader_code, GLenum type);
	void render();
};

#endif
