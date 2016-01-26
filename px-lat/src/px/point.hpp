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

		point moved(point move) const { move.move(*this); return move; }
		point moved(component x, component y) const { point result(x, y); result.move(*this); return result; }
		point moved(component x, component y, component z) const { point result(x, y, z); result.move(*this); return result; }
		point multiplied(point stretch) const { stretch.multiply(*this); return stretch; }
		point multiplied(component x, component y) const { point result(x, y); result.multiply(*this); return result; }
		point multiplied(component x, component y, component z) const { point result(x, y, z); result.multiply(*this); return result; }

		point operator-() const { return{ -X, -Y, -Z }; }

		point& operator+=(const point &rhs) { move(rhs); return *this; }
		point& operator-=(const point &rhs) { move(-rhs); return *this; }
		point& operator*=(const point &rhs) { multiply(rhs); return *this; };
		point& operator/=(const point &rhs) { divide(rhs.X); return *this; };

		point& operator*=(component c) { multiply(c); return *this; };
		point& operator/=(component c) { divide(c); return *this; };
	};

	inline point operator+(point lhs, const point &rhs) { lhs += rhs; return lhs; }
	inline point operator-(point lhs, const point &rhs) { lhs -= rhs; return lhs; }
	inline point operator*(point lhs, const point &rhs) { lhs *= rhs; return lhs; }
	inline point operator/(point lhs, const point &rhs) { lhs /= rhs; return lhs; }
	inline point operator*(point lhs, point::component c) { lhs *= c; return lhs; }
	inline point operator/(point lhs, point::component c) { lhs /= c; return lhs; }
}

#endif