
#include "xyzlabs/openglwidget.hpp"
#include "xyzlabs/globals.hpp"
#include "xyzlabs/windowmanager.hpp"
#include "xyzlabs/dialog.hpp"

namespace xyzlabs {

OpenGLWidget::OpenGLWidget(const std::string &title): Widget(title) {}

void OpenGLWidget::init() {
    backend_.create_framebuffer(
        window_manager().get_main_window()->width(),
        window_manager().get_main_window()->height()
    );
}

void OpenGLWidget::show(const ImVec2 &size, const ImVec2 &pos) {
    backend_.rescale_framebuffer(size.x, size.y);
    backend_.bind_framebuffer();
    update();
    backend_.unbind_framebuffer();

    ImGui::GetWindowDrawList()->AddImage(
			(void*)backend_.texture_id, 
			ImVec2(pos.x, pos.y), 
			ImVec2(pos.x + size.x, pos.y + size.y), 
			ImVec2(0, 1), 
			ImVec2(1, 0)
    );
    static auto dialog = new Dialog("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.");
    dialog->display(size, pos);
}

OpenGLWidget::~OpenGLWidget() {
    backend_.destroy();   
}

}