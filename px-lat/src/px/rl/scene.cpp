// name: scene.cpp
// type: c++ source file
// desc: 'scene' class implementation
// auth: is0urce

#include "scene.h"

#include <px/rl/world.h>

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

		scene::scene(world* w)
			: m_stream(stream_range)
			, m_world(w)
		{
			if (!w) throw std::runtime_error("px::rl::scene::scene(..) - world is null");

			m_default = m_world->default_tile();

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
			return m_space.transparent(point) && select(point).transparent();
		}
		bool scene::traversable(const point &point, traverse layer) const
		{
			return m_space.traversable(point, layer) && select(point).traversable(layer);
		}

		std::shared_ptr<location_manager::element> scene::make_location(point position)
		{
			return m_space.make_location(position);
		}
	}
}