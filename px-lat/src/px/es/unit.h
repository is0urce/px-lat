// name: unit.h
// type: c++ header
// desc: class definition
// auth: is0urce

#ifndef PX_ES_UNIT_H
#define PX_ES_UNIT_H

#include <list>

namespace px
{
	namespace es
	{
		class unit
		{
		public:
			typedef component_base* component_ptr;
			typedef std::list<component_ptr> component_container;
			typedef component_container::iterator component_it;
		private:
			std::list<component_ptr> m_components;

		public:
			unit() {}
			virtual ~unit()
			{
				for (component_it it = m_components.begin(), last = m_components.end(); it != last; ++it)
				{
					(*it)->destroy();
				}
			}

		public:
			void add(component_ptr c)
			{
				m_components.push_back(c);
				c->bind(this);
			}
			void enable()
			{
				for (auto &i : m_components)
				{
					i->enable();
				}
			}
			void disable()
			{
				for (auto &i : m_components)
				{
					i->disable();
				}
			}
			template<typename _C>
			_C* component() const
			{
				_C* cast = nullptr;
				for (auto &i : m_components)
				{
					cast = dynamic_cast<_C*>(i);
					if (cast != nullptr) break;
				}
				return cast;
			}
		};
	}
}

#endif