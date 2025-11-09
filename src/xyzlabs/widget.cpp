
#include <spdlog/spdlog.h>
#include <format>

#include "xyzlabs/widget.hpp"
#include "xyzlabs/randomgenerator.hpp"
#include "xyzlabs/globals.hpp"

namespace xyzlabs {

Widget::Widget(const std::string &title):
    title_(title), id_(random_generator().random()), open_(true) {
    titleID_ = std::format("{}##{}", title_, id_);
}

void Widget::show(const ImVec2 &size, const ImVec2& position) {
    int x = 1;
}

void Widget::close() {
    open_ = false;
}

ImVec2 operator*(const ImVec2 &first, const ImVec2 &second) {
    return { first.x * second.x, first.y * second.y };
};

ImVec2 operator*(const ImVec2 &first, float x ) {
    return { first.x * x, first.y * x };
};

ImVec2 operator*(float x, const ImVec2 &first) {
    return { first.x * x, first.y * x };
};

ImVec2 operator+(const ImVec2 &first, const ImVec2 &second) {
    return { first.x + second.x, first.y + second.y };
};

ImVec2 operator-(const ImVec2 &first, const ImVec2 &second) {
    return { first.x - second.x, first.y - second.y };
};

}
