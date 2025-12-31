
#pragma once

#include <Magnum/Platform/GLContext.h>
#include <xyzlabs/widget.hpp>
#include <Magnum/GL/Framebuffer.h>
#include <Magnum/GL/Renderbuffer.h>
#include <Magnum/GL/Texture.h>

#include <optional>
#include <memory>


namespace xyzlabs3d {

class ViewportWidget: public xyzlabs::Widget {
    std::unique_ptr<Magnum::Platform::GLContext> ctx_;
    std::optional<Magnum::GL::Framebuffer> framebuffer_;
    std::optional<Magnum::GL::Texture2D> texture_;
    std::optional<Magnum::GL::Renderbuffer> depthBuffer_;

public:
    ViewportWidget(const std::string& title = "");
    void init() override;
    void destroy() override;
    void show(const ImVec2 &size, const ImVec2 &position) override;
    Magnum::GL::Framebuffer& framebuffer();
    Magnum::GL::Texture2D& texture();
    Magnum::GL::Renderbuffer& depth_buffer();
};

}
