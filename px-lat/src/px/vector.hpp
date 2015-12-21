// name: vector.hpp
// type: c++ header
// desc: class with implementation
// auth: is0urce

// 2-d point with real components

#ifndef PX_VECTOR_H
#define PX_VECTOR_H

#include <px/coordinate.hpp>
#include <px/point.hpp>

namespace px
{
	class vector : public coordinate<double>
	{
	public:
		vector() : coordinate() {}
		vector(component x, component y) : coordinate(x, y) {}
		vector(component x, component y, component z) : coordinate(x, y, z) {}
		vector(const vector &v) : coordinate(v) {}
		vector(const point &point) : coordinate((component)point.X, (component)point.Y, (component)point.Z) {}

		vector moved(vector move) const { move.move(*this); return move; }
		vector multiplied(vector c) const { c.multiply(*this); return c; }
		vector multiplied(component w, component h) const { return multiplied(w, h); }
		vector multiplied(component c) const { return multiplied(c); }

		vector operator-() const { return { -X, -Y, -Z }; }
		vector& operator+=(const vector &rhs) { move(rhs); return *this; }
		vector& operator-=(const vector &rhs) { move(-rhs); return *this; }
		vector& operator*=(const vector &v) { multiply(v); return *this; };
		vector& operator/=(const vector &v) { divide(v); return *this; };
		vector& operator*=(component c) { multiply(c); return *this; };
		vector& operator/=(component c) { divide(c); return *this; };

		void normalize()
		{
			auto len = magnitude();
			if (len > 0)
			{
				X /= len;
				Y /= len;
				Z /= len;
			}
			else
			{
				X = 0;
				Y = 0;
				Z = 0;
			}
		}
		vector normalized() { vector result(*this); result.normalize(); return result; }
		vector clamped(const vector &min, const vector &max) const
		{
			return vector((std::min)((std::max)(min.X, X), max.X), (std::min)((std::max)(min.Y, Y), max.Y), (std::min)((std::max)(min.Z, Z), max.Z));
		}
		vector lerp(const vector &b, component t) const
		{
			return multiplied(1 - t).moved(b.multiplied(t));
		}

		point floor() const
		{
			return point((point::component)std::floor(X), (point::component)std::floor(Y), (point::component)std::floor(Z));
		}
		point ceil() const
		{
			return point((point::component)std::ceil(X), (point::component)std::ceil(Y), (point::component)std::ceil(Z));
		}
		point round() const
		{
			return point((point::component)std::round(X), (point::component)std::round(Y), (point::component)std::round(Z));
		}
	};

	inline vector operator+(vector lhs, const vector &rhs) { lhs += rhs; return lhs; }
	inline vector operator-(vector lhs, const vector &rhs) { lhs -= rhs; return lhs; }
	inline vector operator*(vector lhs, const vector &rhs) { lhs *= rhs; return lhs; }
	inline vector operator/(vector lhs, const vector &rhs) { lhs /= rhs; return lhs; }
	inline vector operator*(vector lhs, point::component c) { lhs *= c; return lhs; }
	inline vector operator/(vector lhs, point::component c) { lhs /= c; return lhs; }
}

#endif