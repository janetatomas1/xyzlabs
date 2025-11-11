
#include <spdlog/spdlog.h>
#include <format>

#include "xyzlabs/widget.hpp"
#include "xyzlabs/randomgenerator.hpp"
#include "xyzlabs/globals.hpp"

namespace xyzlabs {

Widget::Widget(const std::string &title):
    title_(title), id_(random_generator().random()) {
    titleID_ = std::format("{}##{}", title_, id_);
}

void Widget::show(const ImVec2 &size, const ImVec2& position) {}

const std::string &Widget::title() const {
    return title_;
}

uint64_t Widget::id() const {
    return id_;
}

std::string &Widget::title_id() {
    return titleID_;
}


}
