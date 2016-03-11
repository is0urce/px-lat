// name: space.cpp
// type: c++ source file
// desc: class implementation
// auth: is0urce

#include "space.h"

namespace px
{
	namespace rl
	{
		space::space()
			: m_graph(0, 0, 64)
		{
		}
		space::~space()
		{
		}
		bool space::transparent(const point &point) const
		{
			bool block = false;
			m_graph.find(point.X, point.Y, 0, [&block](int x, int y, location_component* l)
			{
				block &= l && l->enabled() && !l->transparent();
				return !block;
			});
			return !block;
		}
		bool space::traversable(const point &point) const
		{
			bool block = false;
			m_graph.find(point.X, point.Y, 0, [&block](int x, int y, location_component* l)
			{
				block |= l && l->enabled() && l->blocking();
				return !block;
			});
			return !block;
		}
		std::shared_ptr<location_manager::element> space::make_location(point position)
		{
			auto result = m_locations.make_shared();
			result->move(position);
			result->space(&m_graph);

			return result;
		}
	}
}