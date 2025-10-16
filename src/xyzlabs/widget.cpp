
#include <spdlog/spdlog.h>
#include <format>

#include "xyzlabs/widget.hpp"
#include "xyzlabs/randomgenerator.hpp"
#include "xyzlabs/globals.hpp"


Widget::Widget(const std::string &title):
    title_(title), id_(random_generator().random()), open_(true) {
    titleID_ = std::format("{}##{}", title_, id_);
}

void Widget::show(const ImVec2 &size, const ImVec2& position) {
    int x = 1;
}

void Widget::show_toolbar(const ImVec2 &size, const ImVec2& position) {}

void Widget::close() {
    open_ = false;
}
