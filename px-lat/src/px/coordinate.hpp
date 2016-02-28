// name: coordinate.hpp
// type: c++ header
// desc: template class 
// auth: is0urce

// template coordinate in space with generic methods

#ifndef PX_COORDINATE_H
#define PX_COORDINATE_H

#include <algorithm>

namespace px
{
	template <typename _C>
	class coordinate
	{
	public:
		typedef _C component;

	public:
		component X;
		component Y;
		component Z;

	protected:
		coordinate() {}
		coordinate(component x, component y) : X(x), Y(y) {}
		coordinate(component x, component y, component z) : X(x), Y(y), Z(z) {}

	public:
		// accessors

		component x() const { return X; }
		component y() const { return Y; }
		component z() const { return Z; }

		// mutations

		void negate() { X = -X; Y = -Y; Z = -Z; }
		void move(const coordinate &move) { X += move.X; Y += move.Y; Z += move.Z; }
		void move(component x, component y) { X += x; Y += y; }
		void move(component x, component y, component z) { X += x; Y += y; Z += z; }
		void reverse_move(const coordinate &move) { X -= move.X; Y -= move.Y; Z -= move.Z; }
		void reverse_move(component x, component y) { X -= x; Y -= y; }
		void reverse_move(component x, component y, component z) { X -= x; Y -= y; Z -= z; }
		void multiply(const coordinate &c) { X *= c.X; Y *= c.Y; }
		void multiply(component w, component h) { X *= w, Y *= h; }
		void multiply(component x, component y, component z) { X *= x; Y *= y; Z *= z; }
		void multiply(component size) { X *= size; Y *= size; Z *= size; }
		void divide(component x, component y, component z) { X /= x; Y /= y; Z /= z; }
		void divide(component x, component y) { X /= x; Y /= y; }
		void divide(const coordinate &divisor) { X /= divisor.X; Y /= divisor.Y; Z /= divisor.Z; }
		void divide(component divisor) { X /= divisor; Y /= divisor; Z /= divisor; }

		// distances

		// chebyshev distance to specified coordinate
		component king_distance(const coordinate &target) const
		{
			coordinate::component dx = X - target.X;
			coordinate::component dy = Y - target.Y;
			coordinate::component dz = Z - target.Z;
			dx = (std::abs)(dx);
			dy = (std::abs)(dy);
			dz = (std::abs)(dz);
			return (std::max)(dx, (std::max)(dy, dz));
		}

		// manhattan distance to specified coordinate
		component block_distance(const coordinate &target) const
		{
			coordinate dx = X - target.X;
			coordinate dy = Y - target.Y;
			coordinate dz = Z - target.Z;
			dx = std::abs(dx);
			dy = std::abs(dy);
			dz = std::abs(dz);
			return dx + dy + dz;
		}

		// distance to to specified coordinate, squared for performance reasons
		component distance2(const coordinate &target) const // squared distance
		{
			component dx = X - target.X;
			component dy = Y - target.Y;
			component dz = Z - target.Z;
			return dx * dx + dy * dy + dz * dz;
		}

		// distance to to specified coordinate
		auto distance(const coordinate &target) const -> decltype(std::sqrt(distance2(target)))
		{
			return std::sqrt(distance2(target));
		}

		// size of vector, squared for performance reasons
		component magnitude2() const
		{
			return distance2({});
		}

		// size of vector
		auto magnitude() -> decltype(std::sqrt(magnitude2())) const
		{
			return distance({});
		}

		// misc

		template <typename _M>
		void write2(_M *memory) const
		{
			memory[0] = (_M)X;
			memory[1] = (_M)Y;
		}
		void write2(component *memory) const 
		{
			memory[0] = X;
			memory[1] = Y;
		}
		template <typename _M>
		void write3(_M *memory) const
		{
			memory[0] = (_M)X;
			memory[1] = (_M)Y;
			memory[2] = (_M)Z;
		}
		void write3(component *memory) const
		{
			memory[0] = X;
			memory[1] = Y;
			memory[2] = Z;
		}
	};

	// compare operators
	template <typename _C>
	inline bool operator==(const coordinate<_C> &a, const coordinate<_C> &b)
	{
		return a.X == b.X && a.Y == b.Y && a.Z == b.Z;
	}
	template <typename _C>
	inline bool operator!=(const coordinate<_C> &a, const coordinate<_C> &b)
	{
		return !operator==(a, b);
	}
}

#endif