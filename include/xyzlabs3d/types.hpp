

#pragma once

#include <Magnum/Platform/GLContext.h>

#include <Magnum/GL/Framebuffer.h>
#include <Magnum/GL/Renderbuffer.h>
#include <Magnum/GL/Texture.h>

#include <Magnum/SceneGraph/Camera.h>
#include <Magnum/SceneGraph/MatrixTransformation3D.h>
#include <Magnum/SceneGraph/Object.h>
#include <Magnum/SceneGraph/Scene.h>
#include <Magnum/SceneGraph/Drawable.h>

#include <Magnum/Shaders/VertexColorGL.h>
#include <Magnum/Shaders/FlatGL.h>

#include <Magnum/GL/Mesh.h>
#include <Magnum/Math/Matrix4.h>
#include <Magnum/Math/Color.h>

namespace xyzlabs3d {

using GLContext = Magnum::Platform::GLContext;

using Framebuffer = Magnum::GL::Framebuffer;
using Texture2D = Magnum::GL::Texture2D;
using Renderbuffer = Magnum::GL::Renderbuffer;

using Camera3D = Magnum::SceneGraph::Camera3D;
using Object3D = Magnum::SceneGraph::Object<Magnum::SceneGraph::MatrixTransformation3D>;
using Scene3D = Magnum::SceneGraph::Scene<Magnum::SceneGraph::MatrixTransformation3D>;
using DrawableGroup3D = Magnum::SceneGraph::DrawableGroup3D;
using Drawable3D = Magnum::SceneGraph::Drawable3D;

using VertexColorGL3D = Magnum::Shaders::VertexColorGL3D;
using FlatGL3D = Magnum::Shaders::FlatGL3D;

using Mesh = Magnum::GL::Mesh;
using Matrix4 = Magnum::Math::Matrix4<float>;

using Color3 = Magnum::Math::Color3<float>;
using Color4 = Magnum::Math::Color4<float>;

}
