// name: iscene.hpp
// type: c++ header
// desc: events of scene class as template
// auth: is0urce

// class for registering two on_add and on_remove events

#ifndef PX_SHELL_I_SCENE_HPP
#define PX_SHELL_I_SCENE_HPP

#include <functional>
#include <list>

namespace
{
	template <typename _U>
	void broadcast(std::list<std::function<void(_U)>> list, _U unit)
	{
		for (auto f& : list)
		{
			f(unit);
		}
	}
}

namespace px
{
	namespace shell
	{
		template <typename _P>
		class iscene
		{
		public:
			typedef _P unit_ptr;
			typedef std::function<void(unit_ptr)> callback_fn;

			// attributes
		private:
			std::list<callback_fn> m_event_add;
			std::list<callback_fn> m_event_remove;

			// ctor & dtor
		public:
			virtual ~iscene()
			{
			}

		protected:
			void on_add(unit_ptr unit)
			{
				broadcast<unit_ptr>(m_event_add, unit);
			}
			void on_remove(unit_ptr unit)
			{
				broadcast<unit_ptr>(m_event_remove, unit);
			}
			
		public:
			void on_add(callback_fn fn)
			{
				m_event_add.push_back(fn);
			}
			void on_remove(callback_fn fn)
			{
				m_event_remove.push_back(fn);
			}
		};
	}
}

#endif