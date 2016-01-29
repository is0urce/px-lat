// name: alignment.h
// type: c++
// desc: struct declaration
// auth: is0urce

#ifndef PX_UI_ALIGNMENT_H
#define PX_UI_ALIGNMENT_H

#include <px/point.hpp>
#include <px/vector.hpp>
#include <px/rectangle.hpp>

namespace px
{
	namespace ui
	{
		struct alignment
		{
		public:
			vector anchor_percent;
			point anchor_offset;
			point size_absolute;
			vector size_relative;

			alignment()
			{
			}
			alignment(vector a_percent, point a_offset, point s_absolute, vector s_relative)
				: anchor_percent(a_percent)
				, anchor_offset(a_offset)
				, size_absolute(s_absolute)
				, size_relative(s_relative)
			{
			}

			rectangle calculate(const rectangle &parent) const
			{
				point start = anchor_offset + parent.start() + (parent.range() * anchor_percent).ceil();
				point range = size_absolute + (parent.range() * size_relative).ceil();
				return{ start, range };
			}
			rectangle calculate() const
			{
				return{ anchor_offset, size_absolute };
			}
		};
	}
}

#endif