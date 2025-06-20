
#ifndef BURNING_FOREST
#define BURNING_FOREST

#include "xyzlabs/widget.hpp"
#include "xyzlabs/openglbackend.hpp"
#include "xyzlabs/shape.hpp"

class BurningForest: public Widget {
	OpenGLBackend backend_;
    Triangle t;
    
public:
    BurningForest();
    void show(const ImVec2 &size, const ImVec2 &pos) override;
    ~BurningForest();
};

#endif
