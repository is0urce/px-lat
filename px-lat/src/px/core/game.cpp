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
		game::game(library* lib, rl::scene* s, shell::perception* perception)
			: m_lib(lib)
			, m_scene(s)
			, m_perception(perception)
		{
			if (!lib) throw std::runtime_error("game::game() library is null");
			if (!s) throw std::runtime_error("game::game() scene is null");
			if (!perception) throw std::runtime_error("game::game() perception is null");
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
					const shell::image* img = tile.sprite();
					//bool tile = m_scene->transparent(range_point + start);
					//m_perception->ground(range_point, *img);
				});
			}
		}

		void game::start()
		{
			auto u = std::make_shared<es::unit>();

			auto img = m_lib->make_image('@');
			auto pos = m_scene->make_location({ 3,3 });
			img->tint = 0xffff00;
			pos->blocking(true);

			img->link(pos);
			u->add(img);
			u->add(pos);

			u->enable();
			m_units.push_back(u);

			m_pos = pos.get();
			m_player = u.get();

			u = std::make_shared<es::unit>();

			img = m_lib->make_image('g');
			pos = m_scene->make_location({ 5,5 });
			pos->blocking(true);
			img->tint = 0xff0000;

			img->link(pos);
			u->add(img);
			u->add(pos);

			u->enable();
			m_units.push_back(u);

			turn();
		}
		void game::stop()
		{
			m_units.clear();
		}
	}
}