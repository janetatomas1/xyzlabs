
#ifndef BURNING_FOREST
#define BURNING_FOREST

#include "xyzlabs/openglwidget.hpp"
#include "xyzlabs/shape.hpp"

class BurningForest: public OpenGLWidget {
    Shape<4> t = {{
        0.5f,  0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        -0.5f,  0.5f, 0.0f
    }};
    
public:
    BurningForest();
    void update() override;
    void destroy() override;
};

#endif
