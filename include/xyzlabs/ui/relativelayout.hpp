#pragma once

#include <imgui.h>
#include <utility>

#include "xyzlabs/utils/operators.hpp"

namespace xyzlabs {

/**
 * @brief Helper class for relative (normalized) UI layout.
 *
 * @details
 * RelativeLayout expresses widget size and position as **normalized values**
 * relative to a parent region (usually a window or container).
 *
 * - Size is expressed as a fraction of the available size
 *   (e.g. {1.0f, 1.0f} = fill entire parent).
 * - Position is expressed as a fraction of the parent size
 *   (e.g. {0.0f, 0.0f} = top-left).
 *
 * The class does not perform any ImGui rendering itself; it only computes
 * absolute pixel-space values from relative inputs.
 *
 * @note
 * This class assumes ImVec2 arithmetic operators are available
 * (see xyzlabs/utils/operators.hpp).
 *
 * @par Thread Safety
 * Not thread-safe. Intended to be used from the UI / render thread.
 */
class RelativeLayout {
    /// Normalized size (0..1 range, relative to parent size)
    ImVec2 size_ = {1.0f, 1.0f};

    /// Normalized position (0..1 range, relative to parent position)
    ImVec2 position_ = {0.0f, 0.0f};

public:
    /**
     * @brief Constructs a default relative layout.
     *
     * @details
     * Default layout fills the entire parent area and starts at the origin
     * (size = {1,1}, position = {0,0}).
     */
    RelativeLayout() = default;

    /**
     * @brief Constructs a relative layout with explicit size and position.
     *
     * @param size Normalized size relative to the parent.
     * @param position Normalized position relative to the parent.
     */
    RelativeLayout(const ImVec2 &size, const ImVec2 &position);

    /**
     * @brief Sets the relative size.
     *
     * @param size Normalized size (fractions of parent size).
     */
    void set_size_relative(const ImVec2 &size);

    /**
     * @brief Sets the relative position.
     *
     * @param position Normalized position (fractions of parent size).
     */
    void set_position_relative(const ImVec2 &position);

    /**
     * @brief Returns the normalized size.
     */
    ImVec2 size_relative();

    /**
     * @brief Returns the normalized position.
     */
    ImVec2 position_relative();

    /**
     * @brief Computes absolute size from a parent size.
     *
     * @param size Parent size in pixels.
     * @return Computed absolute size in pixels.
     */
    ImVec2 compute_size(const ImVec2 &size);

    /**
     * @brief Computes absolute position from parent size and position.
     *
     * @param size Parent size in pixels.
     * @param position Parent position in pixels.
     * @return Computed absolute position in pixels.
     */
    ImVec2 compute_position(
        const ImVec2 &size,
        const ImVec2 &position
    );

    /**
     * @brief Computes both absolute position and size.
     *
     * @param size Parent size in pixels.
     * @param position Parent position in pixels.
     *
     * @return Pair of (absolute size, absolute position).
     */
    std::pair<ImVec2, ImVec2> compute(
        const ImVec2 &size,
        const ImVec2 &position
    );
};

} // namespace xyzlabs
