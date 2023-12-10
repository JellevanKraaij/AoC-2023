#pragma once
#include <array>

using Vec2 = std::array<int, 2>;

Vec2 operator+(const Vec2 &lhs, const Vec2 &rhs) {
    return {lhs[0] + rhs[0], lhs[1] + rhs[1]};
}

Vec2 inverse(const Vec2 &vec) {
	return {-vec[0], -vec[1]};
}