// name: game.h
// type: c++ header
// desc: class declaration
// auth: is0urce

#ifndef PX_SHELL_GAME_H
#define PX_SHELL_GAME_H


#include <px/shell/iscene.hpp>
#include <px/point.hpp>

#include <memory>
#include <string>

namespace px
{
	namespace ui
	{
		class stack_panel;
	}
	namespace es
	{
		class unit;
	}
	namespace shell
	{
		class game
		{
		public:
			typedef std::shared_ptr<es::unit> unit_ptr;
			typedef iscene<unit_ptr> scene_t;

		private:
			scene_t* m_scene;
			ui::stack_panel* m_ui;

		public:
			game(scene_t* scene, ui::stack_panel* ui);
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