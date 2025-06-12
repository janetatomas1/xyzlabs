
#include <GL/glew.h>

#include <spdlog/spdlog.h>

const std::string vertex_shader_code = R"*(
#version 330

layout (location = 0) in vec3 pos;

void main()
{
	gl_Position = vec4(0.9*pos.x, 0.9*pos.y, 0.5*pos.z, 1.0);
}
)*";

const std::string fragment_shader_code = R"*(
#version 330

out vec4 color;

void main()
{
	color = vec4(1.0, 1.0, 1.0, 1.0);
}
)*";


struct Triangle {
	uint32_t VAO;
	uint32_t VBO;
	uint32_t shader;
	GLfloat vertices[9] = {
		-0.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		0.0f, 0.5f, 0.5f
	};

	void draw_triangle() {
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	void init() {
		shader = glCreateProgram();
		if(!shader) {
			
		}

		add_shader(shader, vertex_shader_code.c_str(), GL_VERTEX_SHADER);
		add_shader(shader, fragment_shader_code.c_str(), GL_FRAGMENT_SHADER);

		GLint result = 0;
		GLchar log[1024] = {0};

		glLinkProgram(shader);
		glGetProgramiv(shader, GL_LINK_STATUS, &result);
		if (!result) {
			glGetProgramInfoLog(shader, sizeof(log), NULL, log);
			spdlog::error("Error linking program:", log);
		}

		glValidateProgram(shader);
		glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
		if (!result) {
			glGetProgramInfoLog(shader, sizeof(log), NULL, log);
			spdlog::error("Error validating program:", log);
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
			spdlog::error("Error compiling shader:", log);
		}

		glAttachShader(program, current_shader);
	}

	void render() {
		glUseProgram(shader);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);
	}
};
