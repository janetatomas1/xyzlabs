
#ifndef SHAPE_HPP
#define SHAPE_HPP

#include <GL/glew.h>

#include <spdlog/spdlog.h>

struct Shape {
	uint32_t VAO;
	uint32_t VBO;
	uint32_t EBO;

	uint32_t program;
	float vertices[12] = {
         0.5f,  0.5f, 0.0f,  // top right
         0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left 
    };
    unsigned int indices[6] = {  // note that we start from 0!
        0, 1, 3,  // first Triangle
        1, 2, 3   // second Triangle
    };

	void draw();
	void init();
	void add_shader(uint32_t program, const char* shader_code, GLenum type);
	void render();
};

#endif
