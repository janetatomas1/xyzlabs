
#ifndef OPENGLWIDGET_HPP
#define OPENGLWIDGET_HPP

#include "xyzlabs/widget.hpp"
#include "xyzlabs/openglbackend.hpp"
#include "xyzlabs/shape.hpp"

class OpenGLWidget: public Widget {
	OpenGLBackend backend_;
    
public:
    OpenGLWidget(const std::string &title);
    void show(const ImVec2 &size, const ImVec2 &pos) override;
    ~OpenGLWidget();
    virtual void update(){};
};

#endif
