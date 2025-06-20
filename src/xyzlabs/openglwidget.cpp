
#include "xyzlabs/openglwidget.hpp"
#include "xyzlabs/xyzlabs.hpp"

OpenGLWidget::OpenGLWidget(const std::string &title): Widget(title) {
    auto &app = XYZLabs::instance();
    backend_.create_framebuffer(app.width(), app.height());
}

void OpenGLWidget::show(const ImVec2 &size, const ImVec2 &pos) {
    backend_.rescale_framebuffer(size.x, size.y);
    backend_.bind_framebuffer();
    update();
    backend_.unbind_framebuffer();

    ImGui::GetWindowDrawList()->AddImage(
			(void *)backend_.texture_id, 
			ImVec2(pos.x, pos.y), 
			ImVec2(pos.x + size.x, pos.y + size.y), 
			ImVec2(0, 1), 
			ImVec2(1, 0)
    );

}

OpenGLWidget::~OpenGLWidget() {
    backend_.destroy();   
}
