
#pragma once

#include "xyzlabs/widget.hpp"
#include "xyzlabs/openglbackend.hpp"
#include "xyzlabs/shape.hpp"

namespace xyzlabs {

class OpenGLWidget: public Widget {
	OpenGLBackend backend_;
    
public:
    OpenGLWidget(const std::string &title);
    void init();
    void show(const ImVec2 &size, const ImVec2 &pos) override;
    ~OpenGLWidget();
    virtual void update(){};
};

}
