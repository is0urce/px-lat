// name: scene.cpp
// type: c++ source file
// desc: 'scene' class implementation
// auth: is0urce

#include "scene.h"

#include <px/rl/world.h>

#include <px/vector.hpp> // for division /w rounding

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
			const rectangle stream_rectangle({ 0, 0 }, stream_range);

			const unsigned int cell_width = 5;
			const unsigned int cell_height = cell_width;
			const point cell_range(cell_width, cell_height);
		}

		scene::scene(world* w)
			: m_stream(stream_range)
			, m_world(w)
			, m_focus(0, 0)
		{
			if (!w) throw std::runtime_error("px::rl::scene::scene(..) - world is null");

			m_default = m_world->default_tile();
		}
		scene::~scene()
		{
			clear();
		}

		// loading...
		point scene::cell(const point &absolute) const
		{
			return (vector(absolute) / cell_range).floor();
		}
		const tile& scene::select(const point &position) const
		{
			point c = cell(position);
			//point s = stream_center + c - m_focus;
			//if (m_stream.contains(s))
			//{
			//	return m_stream.at(s)->at(position - c * cell_range, m_default);
			//}
			//return m_default;
			const auto& map = m_stream.at(stream_center + c - m_focus, nullptr);
			return map ? map->at(position - c * cell_range, m_default) : m_default;
		}
		void scene::focus(point absolute)
		{
			focus(absolute, false);
		}
		void scene::focus(point absolute, bool force)
		{
			point c = cell(absolute);
			if (force || m_focus != c)
			{
				point offset = c - m_focus;
				stream_t shift(stream_range);
				stream_rectangle.enumerate([&](const point &cell)
				{
					if (m_stream.contains(cell - offset))
					{
						shift.at(cell).swap(m_stream.at(cell - offset));
					}
				});

				m_stream.swap(shift);
				
				stream_rectangle.enumerate([&](const point &cell)
				{
					if (!m_stream.at(cell))
					{
						m_stream.at(cell).swap(m_world->generate(c + cell, nullptr));
					}
				});

				m_focus = c;
			}
		}

		// space

		bool scene::transparent(const point &point) const
		{
			return m_space.transparent(point) && select(point).transparent();
		}
		bool scene::traversable(const point &point, traverse layer) const
		{
			return m_space.traversable(point) && select(point).traversable(layer);
		}

		// entity
		void scene::add(es::unit::ptr unit, const point &position)
		{
			auto location = m_space.make_location(position);

			location->blocking(true);
			unit->attach(location);

			unit->enable();
			m_units.push_back(unit);
		}
		void scene::clear()
		{
			m_units.clear();
		}
	}
}