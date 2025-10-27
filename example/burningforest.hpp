
#ifndef BURNING_FOREST
#define BURNING_FOREST

#include <vector>
#include <array>

#include "xyzlabs/openglwidget.hpp"
#include "xyzlabs/shape.hpp"
#include "xyzlabs/periodictask.hpp"
#include "xyzlabs/xyzlabs.hpp"


std::array<std::array<uint8_t, 50>, 50> zeroes();

class BurningForestTask: public PeriodicTask<std::array<std::array<uint8_t, 50>, 50>> {
public:
    BurningForestTask(): PeriodicTask<std::array<std::array<uint8_t, 50>, 50>>(zeroes(), 100) {}
    virtual void update_state(const  std::array<std::array<uint8_t, 50>, 50>& state1, std::array<std::array<uint8_t, 50>, 50> &state2) {
        for(uint32_t i=0;i < 50;i++) {
            for(uint32_t j=0;j < 50;j++) {
                if(state1[i][j] == 0) {
                    if(i > 0 && state1[i - 1][j] == 1) {
                        state2[i][j] = 1;
                    } else if(i < 50 && state1[i + 1][j] == 1) {
                        state2[i][j] = 1;
                    } if(j > 0 && state1[i][j - 1] == 1) {
                        state2[i][j + 1] = 1;
                    } if(j < 50 && state1[i][j + 1] == 1) {
                        state2[i][j] = 1;
                    } else {
                        if(XYZLabs::instance().random_generator().random() % 100000 < 2) {
                            state2[i][j] = 1;
                        }
                    }
                } else {
                    state2[i][j] = (state1[i][j] + 1) % 3;
                }
            }
        }   
    }
};

class BurningForest: public OpenGLWidget {
    const uint32_t height_ = 50;
    const uint32_t width_ = 50;
    std::vector<std::vector<Shape<4>>> tiles_;
    std::vector<std::vector<uint8_t>> colors_;
    float begin_ = -0.95f, end_ = 0.95f;
    std::shared_ptr<BurningForestTask> task_ = nullptr;

public:
    BurningForest();
    void update() override;
    void destroy() override;
    void init() override {
        OpenGLWidget::init();

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
        task_manager().execute_periodic_task(task_);
        uint64_t aa = 0;
        std::unique_ptr<SettingInterface> ptr = std::make_unique<Setting<uint64_t>>("aaaa", aa);
        settings_manager().add_setting("UI", std::move(ptr));
    }
};

#endif
