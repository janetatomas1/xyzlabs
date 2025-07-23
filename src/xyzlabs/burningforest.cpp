
#include "xyzlabs/burningforest.hpp"
#include "xyzlabs/xyzlabs.hpp"

#include "xyzlabs/periodictask.hpp"

const std::array<float, 3> RED = {1.0f, 0.0f, 0.0f};
const std::array<float, 3> GREEN = {0.0f, 1.0f, 0.0f};
const std::array<float, 3> BLUE = {0.0f, 0.0f, 1.0f};
const std::array<float, 3> GRAY = {0.5f, 0.5f, 0.5f};


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

    a = std::make_shared<MyTask>();
    XYZLabs::instance().task_manager().execute_periodic_task(a);
}

void BurningForest::update() {
    // auto get_color = [](){
    //     auto id = XYZLabs::instance().id_generator().get_id() % 3;
    //     if(id == 0) {
    //         return GREEN;
    //     } else if(id == 1) {
    //         return RED;
    //     } else {
    //         return GRAY;
    //     }
    // };
    // for(uint32_t i=0;i < height_;i++) {
    //     for(uint32_t j=0;j < height_;j++) {
    //         tiles_[i][j].render(get_color());
    //     }
    // }
    spdlog::info("state {}", a->get_value());
}

void BurningForest::destroy() {
    a->stop();
    for(uint32_t i=0;i < height_;i++) {
        for(uint32_t j=0;j < height_;j++) {
            tiles_[i][j].destroy();
        }
    }
}
