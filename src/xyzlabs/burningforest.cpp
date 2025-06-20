
#include "xyzlabs/burningforest.hpp"
#include "xyzlabs/xyzlabs.hpp"

BurningForest::BurningForest(): Widget("Burning forest simulation") {
    auto &app = XYZLabs::instance();
    backend_.create_framebuffer(app.width(), app.height());
    t.draw_triangle();
    t.init();
}

void BurningForest::show(const ImVec2 &size, const ImVec2 &pos) {
    backend_.rescale_framebuffer(size.x, size.y);
    backend_.bind_framebuffer();
    t.render();
    backend_.unbind_framebuffer();

    ImGui::GetWindowDrawList()->AddImage(
			(void *)backend_.texture_id, 
			ImVec2(pos.x, pos.y), 
			ImVec2(pos.x + size.x, pos.y + size.y), 
			ImVec2(0, 1), 
			ImVec2(1, 0)
    );

}

BurningForest::~BurningForest() {
    backend_.destroy();   
}