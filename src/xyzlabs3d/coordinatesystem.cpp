
#include <Magnum/Magnum.h>
#include <spdlog/spdlog.h>

#include "xyzlabs3d/coordinatesystem.hpp"


using namespace Magnum;

namespace xyzlabs3d {
using namespace Math::Literals;

CoordinateSystem::CoordinateSystem() {
    GL::Buffer vertexBuffer;

    struct Vertex {
        Vector3 position;
        Color3 color;
    };

    Vertex vertices[] = {
        Vertex{{0.0f, 0.0f, 0.0f}, 0xff0000_rgbf},
        Vertex{{1.0f, 0.0f, 0.0f}, 0xff0000_rgbf},
        Vertex{{0.0f, 0.0f, 0.0f}, 0x00ff00_rgbf},
        Vertex{{0.0f, 1.0f, 0.0f}, 0x00ff00_rgbf},
        Vertex{{0.0f, 0.0f, 0.0f}, 0x0000ff_rgbf},
        Vertex{{0.0f, 0.0f, 1.0f}, 0x0000ff_rgbf}
    };

    vertexBuffer.setData(vertices, GL::BufferUsage::DynamicDraw);
    mesh_
        .setPrimitive(GL::MeshPrimitive::Lines)
        .setCount(6)
        .addVertexBuffer(
            vertexBuffer,
            0,
            Shaders::VertexColorGL3D::Position{},
            Shaders::VertexColorGL3D::Color3{}
        );
    auto x = mesh_.count();
    spdlog::info("count {}", x);
}

CoordinateSystem::~CoordinateSystem() {}

void CoordinateSystem::draw() {
    shader_.draw(mesh_);
}

}
