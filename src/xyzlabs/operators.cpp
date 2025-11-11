
#include <imgui.h>

#include "xyzlabs/operators.hpp"

ImVec2 operator*(const ImVec2 &first, const ImVec2 &second) {
    return { first.x * second.x, first.y * second.y };
};

ImVec2 operator*(const ImVec2 &first, float x ) {
    return { first.x * x, first.y * x };
};

ImVec2 operator*(float x, const ImVec2 &first) {
    return { first.x * x, first.y * x };
};

ImVec2 operator+(const ImVec2 &first, const ImVec2 &second) {
    return { first.x + second.x, first.y + second.y };
};

ImVec2 operator-(const ImVec2 &first, const ImVec2 &second) {
    return { first.x - second.x, first.y - second.y };
};
