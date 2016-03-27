// name: game.cpp
// type: c++ code
// desc: class implementation
// auth: is0urce

#include "game.h"

#include <px/rl/scene.h>
#include <px/rl/traverse.hpp>

namespace px
{
	namespace core
	{
		game::game(rl::library* lib, rl::scene* s, shell::perception* perception)
			: m_library(lib)
			, m_scene(s)
			, m_perception(perception)
		{
			if (!lib) throw std::runtime_error("game::game() library is null");
			if (!s) throw std::runtime_error("game::game() scene is null");
			if (!perception) throw std::runtime_error("game::game() perception is null");

			perception->outer(m_library->image("wall"));
		}
		game::~game()
		{
			// release before manager
			stop();
		}
		unsigned int game::distance(const point &a, const point &b) const
		{
			return a.king_distance(b);
		}

		// controls (used in crtp)
		bool game::step(const point &move)
		{
			auto destination = m_pos->position() + move;
			if (m_scene->traversable(destination, rl::traverse::walk))
			{
				m_scene->focus(destination);
				m_pos->move(destination);
				turn();
			}
			return true;
		}
		bool game::use(unsigned int slot, const point &position)
		{
			turn();
			return true;
		}
		bool game::activate(point position, unsigned int button)
		{
			turn();
			return true;
		}

		void game::turn()
		{
			if (m_player)
			{
				point start = m_pos->position() - point(m_perception->width() / 2, m_perception->height() / 2);
				m_perception->displace(start);

				rectangle({ 0,0 }, m_perception->range()).enumerate([&](const point &range_point)
				{
					const auto &tile = m_scene->select(start + range_point);
					m_perception->ground(range_point, tile.sprite());
				});
			}
		}

		void game::start()
		{
			point start(3, 3);
			auto u = std::make_shared<es::unit>();

			auto img = m_library->make_image("zombie_marksman");
			u->attach(img);
			m_scene->add(u, start);

			m_player = u.get(); 
			m_player->persistent(true);
			m_pos = m_player->component<rl::location_component>().get();

			u = std::make_shared<es::unit>();

			img = m_library->make_image("goblin_h");
			u->attach(img);
			m_scene->add(u, { 5, 5 });

			m_scene->focus(start, true);
			turn();
		}
		void game::stop()
		{
		}
	}
}