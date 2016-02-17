// name: game.h
// type: c++ header
// desc: class declaration
// auth: is0urce

#ifndef PX_CORE_GAME_H
#define PX_CORE_GAME_H

#include <px/core/game_control.hpp>
#include <px/core/library.h>

#include <px/es/unit.h>
#include <px/shell/perception.h>
#include <px/rl/scene.h>

#include <px/point.hpp>

#include <list>
#include <memory>
#include <string>

namespace px
{
	namespace core
	{
		class game : public game_control<game>
		{
		private:
			std::list<es::unit::ptr> m_units;
			std::unique_ptr<rl::scene> m_scene;
			shell::perception* m_perception;
			library *m_lib;

			es::unit *m_player;
			rl::location_manager::element *m_pos;

		public:
			game(shell::perception* perception, library* lib);
			virtual ~game();

		public:
			// settings
			unsigned int distance(const point &a, const point &b) const;

			// controls
			bool step(const point &move);
			bool use(unsigned int slot, const point &position);
			bool activate(point position, unsigned int button);

			void start();
		};
	}
}

#endif