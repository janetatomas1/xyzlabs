
#include "xyzlabs/openglwidget.hpp"
#include "xyzlabs/globals.hpp"
#include "xyzlabs/windowmanager.hpp"

namespace xyzlabs {

OpenGLWidget::OpenGLWidget(const std::string &title): TabWidget(title) {}

void OpenGLWidget::init() {
    fb_.create(
        window_manager().get_main_window()->width(),
        window_manager().get_main_window()->height()
    );
}

void OpenGLWidget::show(const ImVec2 &size, const ImVec2 &pos) {
    fb_.rescale(size.x, size.y);
    fb_.bind();
    update();
    fb_.unbind();
    fb_.render(size.x, size.y);
}

OpenGLWidget::~OpenGLWidget() {
    fb_.destroy();
}

}
