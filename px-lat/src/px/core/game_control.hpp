// name: game_CONTROL.hpp
// type: c++ header
// desc: class declaration
// auth: is0urce

// using crtp
// assuming _G has
//bool step(const point &move);
//bool use(unsigned int slot, const point &position);
//bool activate(point position, unsigned int button);

#ifndef PX_CORE_GAME_CONTROL_HPP
#define PX_CORE_GAME_CONTROL_HPP

#include <px/control.h>
#include <px/key.h>

namespace px
{
	namespace
	{
		unsigned int activate_vmb = 0; // (in general) 'e' to lmb 
	}
	namespace core
	{
		template<typename _G>
		class game_control : public control
		{
		private:
			point m_hover;

		public:
			virtual bool key_control(key_t code) override
			{
				_G* game = static_cast<_G*>(this);
				bool result = false;
				switch (code)
				{
				case key::move_none:
					result = game->step({ 0, 0 });
					break;
				case key::move_north:
					result = game->step({ 0, 1 });
					break;
				case key::move_south:
					result = game->step({ 0, -1 });
					break;
				case key::move_east:
					result = game->step({ 1, 0 });
					break;
				case key::move_west:
					result = game->step({ -1, 0 });
					break;
				case key::move_southwest:
					result = game->step({ -1, -1 });
					break;
				case key::move_southeast:
					result = game->step({ 1, -1 });
					break;
				case key::move_northwest:
					result = game->step({ -1, 1 });
					break;
				case key::move_northeast:
					result = game->step({ 1, 1 });
					break;
				case key::action_use:
					result = game->activate(m_hover, activate_vmb);
					break;
				case key::action0:
					result = game->use(0, m_hover);
					break;
				case key::action1:
					result = game->use(1, m_hover);
					break;
				case key::action2:
					result = game->use(2, m_hover);
					break;
				case key::action3:
					result = game->use(3, m_hover);
					break;
				case key::action4:
					result = game->use(4, m_hover);
					break;
				case key::action5:
					result = game->use(5, m_hover);
					break;
				default:
					result = false;
					break;
				}
				return result;
			}
			virtual bool hover_control(const point &position) override
			{
				m_hover = position;
				return true;
			}
			virtual bool click_control(const point &position, unsigned int button) override
			{
				m_hover = position;
				return static_cast<_G*>(this)->activate(m_hover, button);
			}
			virtual bool scroll_control(int delta) override
			{
				return true;
			}
		};
	}
}

#endif