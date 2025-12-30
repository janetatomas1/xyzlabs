
#pragma once

#include "tabwidget.hpp"
#include "xyzlabs/widget.hpp"
#include "xyzlabs/framebuffer.hpp"
#include "xyzlabs/shape.hpp"
#include "xyzlabs/tabwidget.hpp"

namespace xyzlabs {

class OpenGLWidget: public TabWidget {
	Framebuffer fb_;

public:
    OpenGLWidget(const std::string &title);
    void init();
    void show(const ImVec2 &size, const ImVec2 &pos) override;
    ~OpenGLWidget();
    virtual void update(){};
};

}
