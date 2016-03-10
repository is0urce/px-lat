// name: scene.cpp
// type: c++ source file
// desc: 'scene' class implementation
// auth: is0urce

#include "scene.h"

namespace px
{
	namespace rl
	{
		namespace
		{
			const unsigned int stream_reach = 1;
			const unsigned int stream_size = stream_reach * 2 + 1;
			const point stream_range(stream_size, stream_size);
			const point stream_center(stream_reach, stream_reach);
		}

		scene::scene()
			: m_graph(0, 0, 64)
			, m_stream(stream_range)
		{
			m_default.make_wall();
			m_stream.at(stream_center) = std::make_unique<map_t>(10, 10);

			tile t;
			t.make_ground();
			m_stream.at(stream_center)->fill(t);
		}
		scene::~scene()
		{
		}

		void scene::focus(point position)
		{
			m_focus = position;
		}
		const tile& scene::select(const point &position) const
		{
			return m_stream.at(stream_center)->at(position, m_default);
		}

		bool scene::transparent(const point &point) const
		{
			bool block = false;
			m_graph.find(point.X, point.Y, 0, [&block](int x, int y, rl::location_component* l)
			{
				block &= l && l->enabled() && !l->transparent();
				return !block;
			});
			return !block && select(point).transparent();
		}
		bool scene::traversable(const point &point, traverse layer) const
		{
			bool block = false;
			m_graph.find(point.X, point.Y, 0, [&block](int x, int y, location_component* l)
			{
				block |= l && l->enabled() && l->blocking();
				return !block;
			});
			return !block && select(point).traversable(layer);
		}

		std::shared_ptr<location_manager::element> scene::make_location(point position)
		{
			auto result = m_locations.make_shared();
			result->move(position);
			result->space(&m_graph);

			return result;
		}
	}
}