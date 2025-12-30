#include <Magnum/GL/AbstractFramebuffer.h>
#include <Magnum/GL/Buffer.h>
#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/GL/Mesh.h>
#include <Magnum/Math/Color.h>
#include <Magnum/Shaders/VertexColorGL.h>
#include <Magnum/GL/TextureFormat.h>
#include <Magnum/GL/RenderbufferFormat.h>
#include <Magnum/GL/Renderer.h>

#include "xyzlabs3d/viewportwidget.hpp"

using namespace Magnum::Math::Literals;

namespace xyzlabs3d {

ViewportWidget::ViewportWidget(const std::string& title) : Widget(title) {}

void ViewportWidget::init() {
    ctx_ = std::make_unique<Platform::GLContext>(0, nullptr);

    framebuffer_ = std::make_optional(GL::Framebuffer{{{0, 0}, {800, 600}}});

    texture_ = std::make_optional<GL::Texture2D>();
    depthBuffer_ = std::make_optional<GL::Renderbuffer>();

    texture_
         ->setStorage(1, GL::TextureFormat::RGBA8, {800, 600})
        .setMinificationFilter(SamplerFilter::Linear);
    depthBuffer_->setStorage(
        GL::RenderbufferFormat::DepthComponent24, {800, 600});

    framebuffer_->attachTexture(GL::Framebuffer::ColorAttachment{0},
                       texture_.value(), 0)
        .attachRenderbuffer(GL::Framebuffer::BufferAttachment::Depth,
                            depthBuffer_.value());
}

void ViewportWidget::destroy() {
    framebuffer_->release();
    ctx_.release();
}

void ViewportWidget::show(const ImVec2 &size, const ImVec2 &position) {
    ImGui::Image(texture_->id(), size);
}

}
