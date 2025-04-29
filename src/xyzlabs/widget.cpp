
#include <spdlog/spdlog.h>
#include <format>

#include "xyzlabs.hpp"
#include "widget.hpp"


Widget::Widget(const std::string &title): title_(title), id_(""), open_(true), openPtr_(&open_) {
    id_ = boost::uuids::to_string(XYZLabs::instance().id_generator()());
    titleID_ = std::format("{}##{}", title_, id_);
}

void Widget::show(ImVec2 &size, ImVec2& position) {}

void Widget::disable_closing() {
    openPtr_ = nullptr;
}
