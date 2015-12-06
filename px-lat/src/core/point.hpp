// name: point.hpp
// type: c++ header
// desc: class with implementation
// auth: is0urce

// 2-d point with integer components

#ifndef PX_POINT_H
#define PX_POINT_H

#include "coordinate.hpp"

#include <algorithm>
#include <functional>

namespace px
{
	class point : public coordinate<int>
	{
	public:
		point() : coordinate() {}
		point(component x, component y) : coordinate(x, y) {}
		point(component x, component y, component z) : coordinate(x, y, z) {}

		point moved(point move) const { move.multiply(*this); return move; }
		point multiplied(point stretch) const { stretch.multiply(*this); return stretch; }

		point operator-() const { return{ -X, -Y, -Z }; }

		point& operator+=(const point &rhs) { move(rhs); return *this; }
		point& operator-=(const point &rhs) { move(-rhs); return *this; }
		point& operator*=(const point &rhs) { multiply(rhs); return *this; };
		point& operator/=(const point &rhs) { multiply(1 / rhs.X, 1 / rhs.Y, 1 / rhs.Z); return *this; };

		point& operator*=(component c) { multiply(c); return *this; };
		point& operator/=(component c) { multiply(1 / c); return *this; };
	};

	inline point operator+(point lhs, const point &rhs) { lhs += rhs; return lhs; }
	inline point operator-(point lhs, const point &rhs) { lhs -= rhs; return lhs; }
	inline point operator*(point lhs, const point &rhs) { lhs *= rhs; return lhs; }
	inline point operator/(point lhs, const point &rhs) { lhs /= rhs; return lhs; }
	inline point operator*(point lhs, point::component c) { lhs *= c; return lhs; }
	inline point operator/(point lhs, point::component c) { lhs /= c; return lhs; }
}

#endif