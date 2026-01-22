
#include <xyzlabs3d/viewportwidget.hpp>
#include <xyzlabs/xyzlabs.hpp>

#include <Magnum/GL/GL.h>
#include <Magnum/GL/Mesh.h>
#include <Magnum/Math/Color.h>
#include <Magnum/Shaders/VertexColorGL.h>
#include <Magnum/GL/Renderer.h>
#include <Magnum/GL/DefaultFramebuffer.h>
#include <xyzlabs/ui/tabwidget.hpp>

using namespace xyzlabs;
using namespace Magnum;

int main(int argc, char** argv) {
    struct TriangleWidget: public xyzlabs3d::ViewportWidget {
        TriangleWidget(): xyzlabs3d::ViewportWidget("Triangle") {}

        void init() override {
            xyzlabs3d::ViewportWidget::init();

            struct TriangleVertex {
                Vector2 position;
                Color3 color;
            };
            using namespace Math::Literals;
            const TriangleVertex vertices[]{
                {{-0.5f, -0.5f}, 0xff0000_rgbf},    /* Left vertex, red color */
                {{ 0.5f, -0.5f}, 0x00ff00_rgbf},    /* Right vertex, green color */
                {{ 0.0f,  0.5f}, 0x0000ff_rgbf}     /* Top vertex, blue color */
            };

            GL::Mesh mesh;
            mesh.setCount(Containers::arraySize(vertices))
                .addVertexBuffer(GL::Buffer{vertices}, 0,
                    Shaders::VertexColorGL2D::Position{},
                    Shaders::VertexColorGL2D::Color3{});

            Shaders::VertexColorGL2D shader;
            framebuffer().bind();
            framebuffer().clear(GL::FramebufferClear::Color);
            shader.draw(mesh);
            GL::defaultFramebuffer.bind();
        }

        void destroy() override {
            xyzlabs3d::ViewportWidget::destroy();
        }
    };

    auto app_ = XYZLabs();
    app_.init<Window, TriangleWidget>("Triangle");
    return app_.exec();
}
