#include <Magnum/GL/AbstractFramebuffer.h>
#include <Magnum/GL/Buffer.h>
#include <Magnum/GL/Renderer.h>
#include <Magnum/GL/TextureFormat.h>
#include <Magnum/GL/RenderbufferFormat.h>

#include "xyzlabs3d/viewportwidget.hpp"

using namespace Magnum;
using namespace Magnum::Math::Literals;

namespace xyzlabs3d {

std::unique_ptr<GLContext>  ViewportWidget::ctx_ = nullptr;

ViewportWidget::ViewportWidget(const std::string& title) : Widget(title), camera_(cameraObject_) {}

void ViewportWidget::init() {

    if(!ctx_) {
        GL::Context::Configuration config;
        config.setFlags(GL::Context::Configuration::Flag::QuietLog);
        ctx_ = std::make_unique<GLContext>(config);
    }

    framebuffer_ = Framebuffer{{{0, 0}, {800, 600}}};

    texture_ = Texture2D();
    depthBuffer_ = Renderbuffer();

    texture_
        .setStorage(1, GL::TextureFormat::RGBA8, {800, 600})
        .setMinificationFilter(SamplerFilter::Linear);
    depthBuffer_.setStorage(
        GL::RenderbufferFormat::DepthComponent24, {800, 600});

    framebuffer_.attachTexture(GL::Framebuffer::ColorAttachment{0},
                       texture_, 0)
        .attachRenderbuffer(GL::Framebuffer::BufferAttachment::Depth,
                            depthBuffer_);
}

void ViewportWidget::destroy() {
}

void ViewportWidget::show(const ImVec2 &size, const ImVec2 &position) {
    ImGui::Image(texture_.id(), size);
}

Framebuffer& ViewportWidget::framebuffer() {
    return framebuffer_;
}

Texture2D& ViewportWidget::texture() {
    return texture_;
}

Renderbuffer& ViewportWidget::depth_buffer() {
    return depthBuffer_;
}

Camera3D& ViewportWidget::camera() {
    return camera_;
}

}
