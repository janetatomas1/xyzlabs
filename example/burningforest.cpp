
#include "burningforest.hpp"
#include "xyzlabs/periodictask.hpp"
#include "xyzlabs/globals.hpp"
#include "xyzlabs/settingsmanager.hpp"
#include "xyzlabs/floatingwidget.hpp"


const std::array<float, 3> RED = {1.0f, 0.0f, 0.0f};
const std::array<float, 3> GREEN = {0.0f, 1.0f, 0.0f};
const std::array<float, 3> BLUE = {0.0f, 0.0f, 1.0f};
const std::array<float, 3> GRAY = {0.5f, 0.5f, 0.5f};

std::array<std::array<uint8_t, 50>, 50> zeroes() {
    std::array<std::array<uint8_t, 50>, 50> x;
    for(uint32_t i=0;i < 50;i++) {
        for(uint32_t j=0;j < 50;j++) {
            x[i][j] = 0;
        }
    }

    return x;
}

BurningForest::BurningForest(): OpenGLWidget("Burning forest simulation") {}

void BurningForest::update() {
    auto get_color = [](uint8_t c){
        if(c == 0) {
            return GREEN;
        } else if(c == 1) {
            return RED;
        } else {
            return GRAY;
        }
    };
    
    auto state = task_->get_value();

    for(uint32_t i=0;i < height_;i++) {
        for(uint32_t j=0;j < height_;j++) {
            tiles_[i][j].render(get_color(state[i][j]));
        }
    }

    if(ImGui::IsKeyDown(ImGuiKey_N)) {
        static FloatingWidget widget = FloatingWidget("abcd");
        widget.show();
    }
}

void BurningForest::destroy() {
    task_->stop();
    for(uint32_t i=0;i < height_;i++) {
        for(uint32_t j=0;j < height_;j++) {
            tiles_[i][j].destroy();
        }
    }
}
