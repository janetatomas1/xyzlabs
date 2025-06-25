
#ifndef SHAPE_HPP
#define SHAPE_HPP

#include <GL/glew.h>
#include <array>
#include <spdlog/spdlog.h>

struct Shape {
	uint32_t VAO;
	uint32_t VBO;
	uint32_t EBO;

	uint32_t program;
	std::array<float, 12> vertices = {
         0.5f,  0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        -0.5f,  0.5f, 0.0f 
    };
	std::array<int32_t, 6> indices = {
        0, 1, 3,
        1, 2, 3
    };

	void draw();
	void init();
	void add_shader(uint32_t program, const char* shader_code, GLenum type);
	void render();
};

#endif
