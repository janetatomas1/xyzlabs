
#ifndef BURNING_FOREST
#define BURNING_FOREST

#include "xyzlabs/openglwidget.hpp"
#include "xyzlabs/shape.hpp"

class BurningForest: public OpenGLWidget {
    Triangle t;
    
public:
    BurningForest();
    void update() override;
    // ~BurningForest();
};

#endif
