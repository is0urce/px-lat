// name: scene.hp
// type: c++ header
// desc: 'scene' class definition
// auth: is0urce

#ifndef PX_RL_SCENE_H
#define PX_RL_SCENE_H

#include <px/es/unit.h>
#include <px/rl/space.h>
#include <px/rl/tile.hpp>

#include <px/point.hpp>
#include <px/map.hpp>

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
			space m_space;
			std::list<es::unit::ptr> m_units;

			// loading...
			point m_focus;
			tile_t m_default;
			map<std::unique_ptr<map_t>> m_stream;

		public:
			scene(world*);
			virtual ~scene();

		private:
			point cell(const point &absolute) const;

		public:
			void focus(point position);
			void focus(point position, bool force);
			const tile& select(const point &position) const;
			bool transparent(const point &point) const;
			bool traversable(const point &point, traverse layer) const;

			void add(es::unit::ptr unit, const point &position);
			void clear();
		};
	}
}

#endif