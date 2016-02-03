// name: game.cpp
// type: c++ code
// desc: class implementation
// auth: is0urce

#include "game.h"

namespace px
{
	namespace core
	{
		game::game(rl::scene* scene)
			: m_scene(scene)
		{
		}
		game::~game()
		{
		}
		unsigned int game::distance(const point &a, const point &b) const
		{
			return a.king_distance(b);
		}

		// controls (used in crtp)
		bool game::step(const point &move)
		{
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
	}
}