// name: game.cpp
// type: c++ code
// desc: class implementation
// auth: is0urce

#include "game.h"

#include <px/point.hpp>

#include <memory>
#include <string>

namespace px
{
	namespace core
	{
		game::game(rl::scene* scene, ui::stack_panel* ui)
			: m_scene(scene), m_ui(ui)
		{

		}
		game::~game()
		{

		}

		unsigned int game::distance(const point &a, const point &b) const
		{
			return a.king_distance(b);
		}

		// controls
		bool game::step(const point &move)
		{
			return false;
		}
		bool game::cast(unsigned int slot)
		{
			return false;
		}
		bool game::use()
		{
			return false;
		}
		bool game::hover(point position)
		{
			return false;
		}
		bool game::click(point position, unsigned int button)
		{
			return false;
		}
	}
}