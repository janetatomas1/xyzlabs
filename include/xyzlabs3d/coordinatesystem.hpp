
#pragma once

#include "types.hpp"
#include "xyzlabs3d/types.hpp"

namespace xyzlabs3d {

struct CoordinateSystem: public Magnum::SceneGraph::Drawable3D {
    Mesh mesh_;
    VertexColorGL3D shader_;
public:
    CoordinateSystem(Object3D& object, DrawableGroup3D* drawables);
    void draw(const Matrix4& transformation, Camera3D& camera);
};

}
