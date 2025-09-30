
#include "burningforest.hpp"
#include "xyzlabs/xyzlabs.hpp"
#include "xyzlabs/constants.hpp"
#include "xyzlabs/xyzlabs.hpp"

#include "xyzlabs/periodictask.hpp"

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

BurningForest::BurningForest(): OpenGLWidget("Burning forest simulation") {
    tiles_.resize(height_);

    float x = begin_, y = begin_;
    float x_step = (end_ - begin_) / width_;
    float y_step = (end_ - begin_) / height_;

    for(uint32_t i=0;i < height_;i++) {
        tiles_[i].resize(width_);
        for(uint32_t j=0;j < height_;j++) {
            tiles_[i][j] = {{
                begin_ + j * x_step, begin_ + i * y_step, 0,
                begin_ + (j + 1) * x_step, begin_ + i * y_step, 0,
                begin_ + (j + 1) * x_step, begin_ + (i + 1) * y_step, 0,
                begin_ + j * x_step, begin_ + (i + 1) * y_step, 0
            }};
            tiles_[i][j].init();
            tiles_[i][j].draw();
        }
    }

    task_ = std::make_shared<BurningForestTask>();
    XYZLabs::instance().task_manager().execute_periodic_task(task_);
}

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
}

void BurningForest::destroy() {
    task_->stop();
    for(uint32_t i=0;i < height_;i++) {
        for(uint32_t j=0;j < height_;j++) {
            tiles_[i][j].destroy();
        }
    }
}

IntroWidget::IntroWidget(): Widget("Intro") {}

void IntroWidget::show(const ImVec2 &size, const ImVec2& position) {
    ImGui::SetWindowFontScale(1.3);

    ImGui::SetCursorPos({size.x * 0.37f, size.y * 0.45f});
    ImVec2 btnSize = {size.x * 0.15f, size.y * 0.035f};
}
