
#pragma once

#include <GL/glew.h>
#include <spdlog/spdlog.h>
#include <cinttypes>

namespace xyzlabs {

struct Framebuffer {
	GLuint FBO;
	GLuint RBO;
	GLuint textureID;

	void create(int width, int height);
	void bind();
	void unbind();
	void rescale(float width, float height);
	void destroy();
	void render(float width, float height);
};

};
