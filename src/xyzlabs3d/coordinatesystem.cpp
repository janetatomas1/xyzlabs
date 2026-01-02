
#include "xyzlabs3d/coordinatesystem.hpp"
#include <Magnum/Magnum.h>

using namespace Magnum;

namespace xyzlabs3d {
using namespace Math::Literals;

CoordinateSystem::CoordinateSystem(
    Object3D& object,
    DrawableGroup3D* drawables,
    float radius,
    float gridStep
): Drawable3D(object, drawables) {
    GL::Buffer axisVertexBuffer;

    struct Vertex {
        Vector3 position;
        Color3 color;
    };

    Vertex vertices[] = {
        Vertex{{0.0f, 0.0f, 0.0f}, 0xff0000_rgbf},
        Vertex{{radius, 0.0f, 0.0f}, 0xff0000_rgbf},
        Vertex{{0.0f, 0.0f, 0.0f}, 0x00ff00_rgbf},
        Vertex{{0.0f, radius, 0.0f}, 0x00ff00_rgbf},
        Vertex{{0.0f, 0.0f, 0.0f}, 0x0000ff_rgbf},
        Vertex{{0.0f, 0.0f, radius}, 0x0000ff_rgbf}
    };

    axisVertexBuffer.setData(vertices, GL::BufferUsage::DynamicDraw);
    axisMesh_
        .setPrimitive(GL::MeshPrimitive::Lines)
        .setCount(sizeof(vertices))
        .addVertexBuffer(
            axisVertexBuffer,
            0,
            Shaders::VertexColorGL3D::Position{},
            Shaders::VertexColorGL3D::Color3{}
        );

    std::vector<Vertex> gridVertices;
    float current = -radius;
    while(current <= radius) {
        gridVertices.push_back({{current, -radius, 0.0f}, 0xffffff_rgbf});
        gridVertices.push_back({{current, radius, 0.0f}, 0xffffff_rgbf});
        gridVertices.push_back({{radius, current, 0.0f}, 0xffffff_rgbf});
        gridVertices.push_back({{-radius, current, 0.0f}, 0xffffff_rgbf});
        current += gridStep;
    }
    GL::Buffer gridVertexBuffer;

    Corrade::Containers::ArrayView<Vertex> vertexBufferView{&gridVertices[0], gridVertices.size()};
    gridVertexBuffer.setData(vertexBufferView, GL::BufferUsage::StaticDraw);
    gridMesh_
        .setPrimitive(GL::MeshPrimitive::Lines)
        .setCount(gridVertices.size())
        .addVertexBuffer(
            gridVertexBuffer,
            0,
            Shaders::VertexColorGL3D::Position{},
            Shaders::VertexColorGL3D::Color3{}
    );
}

void CoordinateSystem::draw(const Matrix4& transformation, SceneGraph::Camera3D& camera) {
    auto transform = camera.projectionMatrix() * transformation;

    gridShader_.setTransformationProjectionMatrix(transform);
    axisShader_.setTransformationProjectionMatrix(transform);

    if (showGrid_) {
        gridShader_.draw(gridMesh_);
    }

    if (showAxis_) {
        axisShader_.draw(axisMesh_);
    }
}

}
