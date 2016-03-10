// name: game.h
// type: c++ header
// desc: class declaration
// auth: is0urce

#ifndef PX_RL_WORLD_H
#define PX_RL_WORLD_H

#include <px/es/unit.h>

#include <px/rl/tile.hpp>
#include <px/map.hpp>

#include <memory>
#include <list>

namespace px
{
	namespace rl
	{
		class world
		{
		public:
			typedef tile tile_t;
			typedef map<tile_t> map_t;
			typedef std::shared_ptr<map_t> map_ptr;
			typedef std::shared_ptr<es::unit> unit_ptr;

		private:
			map<bool> m_created;

		public:
			world();
			virtual ~world();
			world(const world&) = delete;

		public:
			// management
			//map_ptr generate(const point &cell, builder_t::fetch_op fetch);
			//void store(unit_ptr unit);
		};
	}
}

#endif