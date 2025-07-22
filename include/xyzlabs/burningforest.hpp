
#ifndef BURNING_FOREST
#define BURNING_FOREST

#include <vector>

#include "xyzlabs/openglwidget.hpp"
#include "xyzlabs/shape.hpp"
#include "xyzlabs/periodictask.hpp"

class MyTask: public PeriodicTask<int> {
public:
    MyTask(): PeriodicTask<int>() {
        spdlog::info("constructor");
    }
    void update() override {
        spdlog::info("abcd");
    }
};


class BurningForest: public OpenGLWidget {
    uint32_t height_ = 100;
    uint32_t width_ = 100;
    std::vector<std::vector<Shape<4>>> tiles_;
    std::vector<std::vector<uint8_t>> colors_;
    float begin_ = -0.95f, end_ = 0.95f;
    std::shared_ptr<MyTask> a;

public:
    BurningForest();
    void update() override;
    void destroy() override;
};

#endif
