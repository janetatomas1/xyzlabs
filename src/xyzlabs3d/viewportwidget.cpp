#include <Magnum/GL/AbstractFramebuffer.h>
#include <Magnum/GL/Buffer.h>
#include <Magnum/GL/Renderer.h>
#include <Magnum/GL/TextureFormat.h>
#include <Magnum/GL/RenderbufferFormat.h>
#include <Magnum/Magnum.h>

#include "xyzlabs3d/viewportwidget.hpp"

using namespace Magnum;
using namespace Magnum::Math::Literals;

namespace xyzlabs3d {

std::unique_ptr<GLContext>  ViewportWidget::ctx_ = nullptr;

ViewportWidget::ViewportWidget(
    const std::string& title,
    int width,
    int height,
    Widget *parent
)
: Widget(title, parent), width_(width), height_(height) {}

void ViewportWidget::init() {
    if(!ctx_) {
        GL::Context::Configuration config;
        config.setFlags(GL::Context::Configuration::Flag::QuietLog);
        ctx_ = std::make_unique<GLContext>(config);
    }

    framebuffer_ = Framebuffer{{{0, 0}, {width_, height_}}};

    texture_ = Texture2D();
    depthBuffer_ = Renderbuffer();

    texture_
        .setStorage(1, GL::TextureFormat::RGBA8, {width_, height_})
        .setMinificationFilter(SamplerFilter::Linear);
    depthBuffer_.setStorage(
        GL::RenderbufferFormat::DepthComponent24, {width_, height_});

    framebuffer_.attachTexture(GL::Framebuffer::ColorAttachment{0},
                       texture_, 0)
        .attachRenderbuffer(GL::Framebuffer::BufferAttachment::Depth,
                            depthBuffer_);

    cameraObject_ = new Object3D{&scene_};
    camera_ = new Camera3D(*cameraObject_);
}

void ViewportWidget::show(const ImVec2 &size, const ImVec2 &) {
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

Camera3D* ViewportWidget::camera() {
    return camera_;
}

Object3D* ViewportWidget::camera_object() {
    return cameraObject_;
}

Scene3D& ViewportWidget::scene() {
    return scene_;
}

DrawableGroup3D& ViewportWidget::drawables() {
    return drawables_;
}

}
