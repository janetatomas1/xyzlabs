
#pragma once

#include <Magnum/Platform/GLContext.h>
#include <xyzlabs/widget.hpp>
#include <Magnum/GL/Framebuffer.h>
#include <Magnum/GL/Renderbuffer.h>
#include <Magnum/GL/Texture.h>

#include <optional>
#include <memory>

using namespace Magnum;

namespace xyzlabs3d {

class ViewportWidget: public xyzlabs::Widget {
    std::unique_ptr<Platform::GLContext> ctx_;
    std::optional<GL::Framebuffer> framebuffer_;
    std::optional<GL::Texture2D> texture_;
    std::optional<GL::Renderbuffer> depthBuffer_;

public:
    ViewportWidget(const std::string& title = "");
    void init() override;
    void destroy() override;
    void show(const ImVec2 &size, const ImVec2 &position) override;
    GL::Framebuffer& framebuffer();
    GL::Texture2D& texture();
    GL::Renderbuffer& depth_buffer();
};

}
