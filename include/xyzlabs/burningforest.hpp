
#ifndef BURNING_FOREST
#define BURNING_FOREST

#include "xyzlabs/widget.hpp"
#include "xyzlabs/openglbackend.hpp"

class BurningForest: public Widget {
	OpenGLBackend backend_;
public:
    BurningForest();
    void show(const ImVec2 &size, const ImVec2 &pos) override;
    ~BurningForest();
};

#endif
