#pragma once

#include "xyzlabs/ui/widget.hpp"

namespace xyzlabs {

/**
 * @brief Clickable button widget.
 *
 * Button is a lightweight wrapper around ImGui::Button that integrates with
 * the xyzlabs widget and layout system. It renders a clickable UI element
 * and reports activation state in an immediate-mode fashion.
 *
 * The button does not retain interaction state between frames; instead,
 * draw() returns true on the frame the button is clicked.
 */
class Button : public Widget {
public:
    /**
     * @brief Construct a button with an optional title.
     *
     * The title is used as the ImGui label. It may contain an ImGui ID suffix
     * (e.g. "Click me##unique_id") to decouple visible text from widget identity.
     *
     * @param title Button label (default: empty).
     */
    explicit Button(const std::string& title = "");

    /**
     * @brief Render the button.
     *
     * Computes layout using the associated layout object, positions the cursor,
     * and renders the button.
     *
     * @param size Available size for layout computation.
     * @param pos  Parent-relative position for layout computation.
     * @return true if the button was clicked during this frame.
     */
    bool draw(const ImVec2& size, const ImVec2& pos);

    /**
     * @brief Convenience call operator.
     *
     * Equivalent to calling draw(size, pos).
     *
     * @param size Available size for layout computation.
     * @param pos  Parent-relative position for layout computation.
     * @return true if the button was clicked during this frame.
     */
    bool operator()(const ImVec2& size, const ImVec2& pos);
};

} // namespace xyzlabs
