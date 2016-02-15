// name: scene.hp
// type: c++ header
// desc: 'scene' class definition
// auth: is0urce

#ifndef PX_RL_SCENE_H
#define PX_RL_SCENE_H

#include <px/es/unit.h>
#include <px/es/location_component.hpp>

#include <px/rl/tile.hpp>

#include <px/point.hpp>
#include <px/qtree.hpp>

#include <memory>

namespace px
{
	namespace rl
	{
		class scene
		{
		private:
			tile m_default;
			qtree<es::location_manager::element*> m_graph;
			es::location_manager m_locations;

		public:
			scene();
			virtual ~scene();

		public:
			const tile& select(const point &position)
			{
				return m_default;
			}
			bool transparent(const point &point)
			{
				bool block = false;
				m_graph.find(point.X, point.Y, 0, [&block](int x, int y, es::location_component* l)
				{
					block &= l && !l->transparent;
					return !block;
				});
				return !block && select(point).transparent();
			}
			bool traversable(const point &point, traverse layer)
			{
				bool block = false;
				m_graph.find(point.X, point.Y, 0, [&block](int x, int y, es::location_component* l)
				{
					block &= l && l->blocking;
					return !block;
				});
				return !block && select(point).traversable(layer);
			}
			std::shared_ptr<es::location_manager::element> make_location(point position)
			{
				auto result = m_locations.make_shared();
				result->position = position;
				m_graph.add(position, result.get());
				return result;
			}
			void move(es::location_manager::element* e, point destination)
			{
				m_graph.move(e->position, e, destination);
				e->position = destination;
			}
			void remove(es::location_manager::element* e)
			{
				m_graph.remove(e->position, e);
			}
		};
	}
}

#endif