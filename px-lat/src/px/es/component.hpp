// name: component.hp
// type: c++ header
// desc: class definition
// auth: is0urce

#ifndef PX_ES_COMPONENT_H
#define PX_ES_COMPONENT_H

#include <array>
#include <functional>
#include <list>
#include <memory>
#include <mutex>
#include <vector>

namespace px
{
	namespace es
	{
		class unit;

		class component
		{
		private:
			typedef std::function<void()> del_fn;

		private:
			del_fn m_del;
			unit* m_unit;
			bool m_enabled;

		public:
			component()
			{
			}
			virtual ~component()
			{
			}

		protected:
			virtual void enable_component()
			{
			}
			virtual void disable_component()
			{
			}

		public:
			bool enabled() const
			{
				return m_enabled;
			}
			void enable()
			{
				if (!m_enabled)
				{
					m_enabled = true;
					enable_component();
				}
			}
			void disable()
			{
				if (m_enabled)
				{
					m_enabled = false;
					disable_component();
				}
			}

			void destroy()
			{
				if (m_del)
				{
					m_del();
				}
			}
			void bind(unit* entity)
			{
				m_unit = entity;
			}
			unit* entity()
			{
				return m_unit;
			}
			void manage(del_fn del)
			{
				m_del = del;
			}
		};
	}
}

#endif