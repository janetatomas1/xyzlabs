
#pragma once

#include <Corrade/Tags.h>
#include <xyzlabs/widget.hpp>

#include "types.hpp"
#include "xyzlabs3d/types.hpp"

namespace xyzlabs3d {

class ViewportWidget: public xyzlabs::Widget {
    static std::unique_ptr<GLContext> ctx_;

    Framebuffer framebuffer_{Corrade::NoCreate};
    Texture2D texture_{Corrade::NoCreate};
    Renderbuffer depthBuffer_{Corrade::NoCreate};

    Camera3D *camera_;
    Object3D *cameraObject_;

    Scene3D scene_;
    DrawableGroup3D drawables_;
    int width_;
    int height_;
public:
    ViewportWidget(
        const std::string& title = "",
        int width = 2000,
        int height = 2000,
        Widget *parent = nullptr
    );
    void init() override;
    void show(const ImVec2 &size, const ImVec2 &position) override;
    Framebuffer& framebuffer();
    Texture2D& texture();
    Renderbuffer& depth_buffer();
    Camera3D *camera();
    Object3D *camera_object();
    Scene3D& scene();
    DrawableGroup3D& drawables();
};

}
