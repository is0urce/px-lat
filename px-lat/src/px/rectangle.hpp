// name: rectangle.hpp
// type: c++ header
// desc: class with implementation
// auth: is0urce

// 2-d rectangle with integer components

#ifndef PX_RECTANGLE_H
#define PX_RECTANGLE_H

#include "point.hpp"

#include <functional>

namespace px
{
	struct rectangle
	{
	private:
		point m_start;
		point m_range;

	public:
		rectangle() : m_start{}, m_range{} {}
		rectangle(point range) : m_start(0, 0), m_range(range) {}
		rectangle(point start, point range) : m_start(start), m_range(range) {}

	public:

		point::component left() const
		{
			return m_start.X;
		}
		point::component right() const
		{
			return m_start.X + m_range.X - 1;
		}
		point::component bottom() const
		{
			return m_start.Y;
		}
		point::component top() const
		{
			return m_start.Y + m_range.Y - 1;
		}
		point::component width() const
		{
			return m_range.X;
		}
		point::component height() const
		{
			return m_range.Y;
		}
		point range() const
		{
			return m_range;
		}
		point start() const
		{
			return m_start;
		}

		bool contains(const point &point) const
		{
			return point.X >= m_start.X && point.Y >= m_start.Y && point.X < m_start.X + m_range.X && point.Y < m_start.Y + m_range.Y;
		}
		bool contains(const rectangle &rect)
		{
			return rect.left() >= left() && rect.bottom() >= bottom() && rect.top() <= top() && rect.right() <= right();
		}
		bool is_border(const point &point) const
		{
			return point.X == left() || point.X == right() || point.Y == top() || point.Y == bottom();
		}

		// enumerate rectangle from start to start + range
		void enumerate(std::function<void(const point&)> enum_fn) const
		{
			point corner = m_start.moved(m_range);
			point index = m_start;
			for (index.Y = m_start.Y; index.Y < corner.Y; ++index.Y)
			{
				for (index.X = m_start.X; index.X < corner.X; ++index.X)
				{
					enum_fn(index);
				}
			}
		}
		rectangle intersection(const rectangle &with) const
		{
			point start = with.m_start;
			point corner = with.m_start + with.m_range;
			if (start.X < m_start.X)
			{
				start.X = m_start.X;
			}
			if (start.Y < m_start.Y)
			{
				start.Y = m_start.Y;
			}
			point c = m_start + m_range;
			if (corner.X > c.X)
			{
				corner.X = c.X;
			}
			if (corner.Y > c.Y)
			{
				corner.Y = c.Y;
			}
			return rectangle(start, corner - start);
		}
	};
	inline bool operator==(const rectangle &a, const rectangle &b)
	{
		return a.range() == b.range() && a.start() == b.start();
	}
	inline bool operator!=(const rectangle &a, const rectangle &b)
	{
		return !operator==(a, b);
	}
}

#endif