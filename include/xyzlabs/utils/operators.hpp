
#pragma once

class ImVec2;

ImVec2 operator*(const ImVec2 &first, const ImVec2 &second);
ImVec2 operator*(const ImVec2 &first, float x );
ImVec2 operator*(float x, const ImVec2 &first);
ImVec2 operator+(const ImVec2 &first, const ImVec2 &second);
ImVec2 operator+(const ImVec2 &first, float x);
ImVec2 operator-(const ImVec2 &first, const ImVec2 &second);

ImVec2& operator*=(ImVec2 &first, const ImVec2 &second);
ImVec2& operator*=(ImVec2 &first, float x);
ImVec2& operator+=(ImVec2 &first, const ImVec2 &second);
ImVec2& operator+=(ImVec2 &first, float x);
ImVec2& operator-=(ImVec2 &first, const ImVec2 &second);
