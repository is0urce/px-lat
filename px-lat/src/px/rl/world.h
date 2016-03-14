// name: world.h
// type: c++ header
// desc: class declaration
// auth: is0urce

#ifndef PX_RL_WORLD_H
#define PX_RL_WORLD_H

#include <px/es/unit.h>

#include <px/rl/library.h>
#include <px/rl/tile.hpp>
#include <px/map.hpp>

#include <functional>
#include <memory>

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
			library* m_library;
			tile_t m_default;

		public:
			world(library* lib)
				: m_created(10, 10, false)
				, m_library(lib)
			{
				if (!lib) throw std::runtime_error("world::world(..) library is null");

				m_default.assign(m_library->image("img/wall.png"));
			}
			virtual ~world()
			{
			}
			world(const world&) = delete;

		public:
			tile_t default_tile() const
			{
				return m_default;
			}
			std::unique_ptr<map_t> generate(const point cell, std::function<void(es::unit::ptr)> fetch_fn)
			{
				auto wall = m_library->image("img/stone0.png");

				auto ground = m_library->image("img/grass0.png");

				auto result = std::make_unique<map_t>(10, 10);
				rectangle({ 0,0 }, result->range()).enumerate([&](const point &position)
				{
					auto &t = result->at(position);
					t.make_ground();
					t.assign(position.X % 3 == 0 ? wall : ground);
				});
				return result;
			}
		};
	}
}

#endif