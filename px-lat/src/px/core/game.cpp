// name: game.cpp
// type: c++ code
// desc: class implementation
// auth: is0urce

#include "game.h"

namespace px
{
	namespace core
	{
		game::game(rl::scene* scene, library *l)
			: m_scene(scene), m_lib(l)
		{
		}
		game::~game()
		{
			// release before manager
			m_units.clear();
		}
		unsigned int game::distance(const point &a, const point &b) const
		{
			return a.king_distance(b);
		}

		// controls (used in crtp)
		bool game::step(const point &move)
		{
			m_pos->position += move;
			return true;
		}
		bool game::use(unsigned int slot, const point &position)
		{
			return true;
		}
		bool game::activate(point position, unsigned int button)
		{
			return true;
		}

		void game::start()
		{
			auto u = std::make_shared<es::unit>();

			auto img = m_lib->make_image('@');
			auto pos = m_locations.create();
			img->tint = 0xffff00;
			pos->position = { 0, 0 };

			img->link(pos);
			u->add(img);
			u->add(pos);

			u->enable();
			m_units.push_back(u);

			m_pos = pos;
			m_player = u.get();
		}
	}
}