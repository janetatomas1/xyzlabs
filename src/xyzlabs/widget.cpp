
#include "xyzlabs.hpp"

#include "widget.hpp"
#include <spdlog/spdlog.h>

Widget::Widget(const std::string &title): title_(title) {
    id_ = boost::uuids::to_string(XYZLabs::instance().id_generator()());
}

void Widget::show(ImVec2 &size, ImVec2& position) {
}
