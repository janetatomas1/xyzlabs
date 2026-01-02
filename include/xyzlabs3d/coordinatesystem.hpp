
#pragma once

#include "types.hpp"
#include "xyzlabs3d/types.hpp"

namespace xyzlabs3d {

struct CoordinateSystem: public Magnum::SceneGraph::Drawable3D {
    Mesh axisMesh_;
    Mesh gridMesh_;

    VertexColorGL3D axisShader_;
    VertexColorGL3D gridShader_;

    bool showAxis_ = true;
    bool showGrid_ = true;

public:
    CoordinateSystem(Object3D& object, DrawableGroup3D* drawables, float radius = 1.0f, float gridStep = 0.1f);
    void draw(const Matrix4& transformation, Camera3D& camera);
};

}
