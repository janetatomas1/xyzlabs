#pragma once

class ImVec2;

/**
 * @brief Component-wise multiplication of two vectors.
 */
ImVec2 operator*(const ImVec2 &first, const ImVec2 &second);

/**
 * @brief Multiplies both components of the vector by a scalar.
 */
ImVec2 operator*(const ImVec2 &first, float x);
ImVec2 operator*(float x, const ImVec2 &first);

/**
 * @brief Component-wise addition of two vectors.
 */
ImVec2 operator+(const ImVec2 &first, const ImVec2 &second);

/**
 * @brief Adds a scalar to both components of the vector.
 */
ImVec2 operator+(const ImVec2 &first, float x);

/**
 * @brief Component-wise subtraction of two vectors.
 */
ImVec2 operator-(const ImVec2 &first, const ImVec2 &second);

/**
 * @brief Component-wise multiplication assignment.
 */
ImVec2& operator*=(ImVec2 &first, const ImVec2 &second);

/**
 * @brief Scalar multiplication assignment.
 */
ImVec2& operator*=(ImVec2 &first, float x);

/**
 * @brief Component-wise addition assignment.
 */
ImVec2& operator+=(ImVec2 &first, const ImVec2 &second);

/**
 * @brief Scalar addition assignment.
 */
ImVec2& operator+=(ImVec2 &first, float x);

/**
 * @brief Component-wise subtraction assignment.
 */
ImVec2& operator-=(ImVec2 &first, const ImVec2 &second);
