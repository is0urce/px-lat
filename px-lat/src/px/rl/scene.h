// name: scene.hp
// type: c++ header
// desc: 'scene' class definition
// auth: is0urce

#ifndef PX_RL_SCENE_H
#define PX_RL_SCENE_H

#include <px/es/unit.h>
#include <px/rl/location_component.hpp>

#include <px/rl/tile.hpp>

#include <px/point.hpp>
#include <px/map.hpp>
#include <px/qtree.hpp>

#include <memory>

namespace px
{
	namespace rl
	{
		class world;
		class scene
		{
		public:
			typedef tile tile_t;
			typedef map<tile_t> map_t;

		private:
			world* m_world;
			// loading...
			point m_focus;
			tile_t m_default;
			map<std::unique_ptr<map_t>> m_stream;

			// space partition
			qtree<location_manager::element*> m_graph;
			location_manager m_locations;

		public:
			scene(world*);
			virtual ~scene();

		public:
			void focus(point position);
			const tile& select(const point &position) const;
			bool transparent(const point &point) const;
			bool traversable(const point &point, traverse layer) const;
			std::shared_ptr<location_manager::element> make_location(point position);
		};
	}
}

#endif