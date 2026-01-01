
#pragma once

#include <Corrade/Tags.h>
#include <Magnum/Platform/GLContext.h>
#include <xyzlabs/widget.hpp>

#include <Magnum/GL/Framebuffer.h>
#include <Magnum/GL/Renderbuffer.h>
#include <Magnum/GL/Texture.h>
#include <Magnum/SceneGraph/Camera.h>
#include <Magnum/SceneGraph/MatrixTransformation3D.h>
#include <Magnum/SceneGraph/Object.h>

#include <optional>
#include <memory>


namespace xyzlabs3d {

using GLContext = Magnum::Platform::GLContext;
using Framebuffer = Magnum::GL::Framebuffer;
using Texture2D = Magnum::GL::Texture2D;
using Renderbuffer = Magnum::GL::Renderbuffer;
using Camera3D = Magnum::SceneGraph::Camera3D;
using Object3D = Magnum::SceneGraph::Object<Magnum::SceneGraph::MatrixTransformation3D>;

class ViewportWidget: public xyzlabs::Widget {
    static std::unique_ptr<GLContext> ctx_;

    Framebuffer framebuffer_{Corrade::NoCreate};
    Texture2D texture_{Corrade::NoCreate};
    Renderbuffer depthBuffer_{Corrade::NoCreate};
    Object3D cameraObject_;
    Camera3D camera_;

public:
    ViewportWidget(const std::string& title = "");
    void init() override;
    void destroy() override;
    void show(const ImVec2 &size, const ImVec2 &position) override;
    Framebuffer& framebuffer();
    Texture2D& texture();
    Renderbuffer& depth_buffer();
    Camera3D& camera();
};

}
