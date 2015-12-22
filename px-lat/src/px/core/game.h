// name: game.h
// type: c++ header
// desc: class declaration
// auth: is0urce

#ifndef PX_CORE_GAME_H
#define PX_CORE_GAME_H

#include <px/point.hpp>

#include <memory>
#include <string>

namespace px
{
	namespace ui
	{
		class stack_panel;
	}
	namespace rl
	{
		class scene;
	}
	namespace core
	{
		class game
		{
		private:
			rl::scene* m_scene;
			ui::stack_panel* m_ui;

		public:
			game(rl::scene* scene, ui::stack_panel* ui);
			virtual ~game();

		public:
			// settings
			unsigned int distance(const point &a, const point &b) const;

			// controls
			bool step(const point &move);
			bool cast(unsigned int slot);
			bool use();
			bool hover(point position);
			bool click(point position, unsigned int button);
		};
	}
}

#endif