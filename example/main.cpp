
#include "xyzlabs/globals.hpp"
#include "burningforest.hpp"

struct W: public xyzlabs::Widget {
    void show(const ImVec2 &size, const ImVec2 &pos) {
        ImGui::Button("abcddd", {200.0, 100.0});
    }
};

int main() {
    app().init<W>("Burning forest");
    return app().exec();
}
