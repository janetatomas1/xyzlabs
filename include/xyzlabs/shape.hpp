
#ifndef SHAPE_HPP
#define SHAPE_HPP

#include <GL/glew.h>
#include <array>
#include <spdlog/spdlog.h>

const std::string vertex_shader_code = R"*(
#version 330

layout (location = 0) in vec3 pos;

void main()
{
	gl_Position = vec4(pos.x, pos.y, pos.z, 1.0);
}
)*";

const std::string fragment_shader_code = R"*(
// fragment_shader.glsl
#version 330 core

out vec4 FragColor;

uniform vec4 inColor;

void main() {
    FragColor = inColor;
}
)*";

template <size_t n>
struct Shape {
	static_assert(n > 2, "n must be at least 2");
	uint32_t VAO;
	uint32_t VBO;
	uint32_t EBO;

	uint32_t program;
	std::array<float, 3 * n> vertices;
	std::array<int32_t, 3 * n - 6> indices;
	Shape() = default;

	Shape(const std::array<float, 3 * n> &data) {
		vertices = data;
		for(size_t i=0;i < n-2;i++) {
			indices[3 * i] = 0;
			indices[3 * i + 1] = i + 1;
			indices[3 * i + 2] = i + 2;
		}
	}

	void draw() {
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.cbegin(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices.cbegin(), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	void init() {
		program = glCreateProgram();
		if(!program) {
		}

		add_shader(program, vertex_shader_code.c_str(), GL_VERTEX_SHADER);
		add_shader(program, fragment_shader_code.c_str(), GL_FRAGMENT_SHADER);

		GLint result = 0;
		GLchar log[1024] = {0};

		glLinkProgram(program);
		glGetProgramiv(program, GL_LINK_STATUS, &result);
		if (!result) {
			glGetProgramInfoLog(program, sizeof(log), NULL, log);
		}

		glValidateProgram(program);
		glGetProgramiv(program, GL_VALIDATE_STATUS, &result);
		if (!result) {
			glGetProgramInfoLog(program, sizeof(log), NULL, log);
		}
	}

	void add_shader(uint32_t program, const char* shader_code, GLenum type) {
		uint32_t current_shader = glCreateShader(type);

		const GLchar* code[1];
		code[0] = shader_code;

		GLint code_length[1];
		code_length[0] = strlen(shader_code);

		glShaderSource(current_shader, 1, code, code_length);
		glCompileShader(current_shader);

		GLint result = 0;
		GLchar log[1024] = {0};

		glGetShaderiv(current_shader, GL_COMPILE_STATUS, &result);
		if (!result) {
			glGetShaderInfoLog(current_shader, sizeof(log), NULL, log);
		}

		glAttachShader(program, current_shader);
	}

	void render(const std::array<float, 3> &color) {
		glUseProgram(program);
		GLint colorLoc = glGetUniformLocation(program, "inColor");
		glUniform4f(colorLoc, color[0], color[1], color[2], 1.0f);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 3 * n - 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	void destroy() {
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
		glDeleteProgram(program);
	}
};

#endif
