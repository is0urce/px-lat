// name: game.h
// type: c++ header
// desc: class declaration
// auth: is0urce

#ifndef PX_CORE_GAME_H
#define PX_CORE_GAME_H

#include <px/point.hpp>
#include <px/core/game_control.hpp>
#include <px/core/library.h>
#include <px/es/unit.h>

#include <list>
#include <memory>
#include <string>

namespace px
{
	namespace rl
	{
		class scene;
	}
	namespace core
	{
		class game : public game_control<game>
		{
		private:
			std::list<es::unit::ptr> m_units;
			rl::scene *m_scene;
			library *m_lib;
			shell::location_manager m_locations;

		public:
			game(rl::scene* scene, library* lib);
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