
#ifndef BURNING_FOREST
#define BURNING_FOREST

#include "xyzlabs/widget.hpp"

class BurningForest: public Widget {
public:
    BurningForest();
    void show(const ImVec2 &size, const ImVec2 &pos) override;
};

#endif
