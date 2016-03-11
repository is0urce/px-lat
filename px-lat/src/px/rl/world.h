// name: world.h
// type: c++ header
// desc: class declaration
// auth: is0urce

#ifndef PX_RL_WORLD_H
#define PX_RL_WORLD_H

#include <px/es/unit.h>

#include <px/rl/tile.hpp>
#include <px/map.hpp>

#include <functional>
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

		private:
			map<bool> m_created;

		public:
			world() : m_created(10, 10, false)
			{}
			virtual ~world()
			{}
			world(const world&) = delete;

		public:
			tile_t default_tile() const
			{
				tile_t tile;
				tile.make_wall();

				return tile;
			}
			std::unique_ptr<map_t> generate(const point cell, std::function<void(es::unit::ptr)> fetch_fn)
			{

			}
		};
	}
}

#endif