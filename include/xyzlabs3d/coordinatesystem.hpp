
#pragma once

#include <Magnum/GL/Mesh.h>
#include <Magnum/Shaders/VertexColorGL.h>
#include <Magnum/GL/Renderer.h>
#include <Magnum/Shaders/Vector.h>
#include <Magnum/Math/Color.h>

namespace xyzlabs3d {

struct CoordinateSystem {
    Magnum::GL::Mesh mesh_;
    Magnum::Shaders::VertexColorGL3D shader_;

public:
    CoordinateSystem();
    ~CoordinateSystem();
    void draw();
};

}
