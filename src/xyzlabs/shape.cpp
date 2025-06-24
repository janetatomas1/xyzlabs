
#include "xyzlabs/shape.hpp"

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

void Triangle::draw() {
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

void Triangle::init() {
    program = glCreateProgram();
    if(!program) {
        spdlog::error("Error creating program:", log);
    }

    add_shader(program, vertex_shader_code.c_str(), GL_VERTEX_SHADER);
    add_shader(program, fragment_shader_code.c_str(), GL_FRAGMENT_SHADER);

    GLint result = 0;
    GLchar log[1024] = {0};

    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &result);
    if (!result) {
        glGetProgramInfoLog(program, sizeof(log), NULL, log);
        spdlog::error("Error linking program:", log);
    }

    glValidateProgram(program);
    glGetProgramiv(program, GL_VALIDATE_STATUS, &result);
    if (!result) {
        glGetProgramInfoLog(program, sizeof(log), NULL, log);
        spdlog::error("Error validating program:", log);
    }
}

void Triangle::add_shader(uint32_t program, const char* shader_code, GLenum type) {
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

void Triangle::render() {
    glUseProgram(program);
    glUseProgram(program);
    GLint colorLoc = glGetUniformLocation(program, "inColor");
    glUniform4f(colorLoc, 1.0f, 0.0f, 0.0f, 0.05f);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
}
