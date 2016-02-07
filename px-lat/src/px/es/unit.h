// name: unit.h
// type: c++ header
// desc: class definition
// auth: is0urce

#ifndef PX_ES_UNIT_H
#define PX_ES_UNIT_H

#include <px/es/component.hpp>

#include <list>
#include <memory>

namespace px
{
	namespace es
	{
		class unit : public std::enable_shared_from_this<unit>
		{
		public:
			typedef std::shared_ptr<component> component_ptr;
			typedef std::list<component_ptr> component_container;
			typedef component_container::iterator component_it;
			typedef std::shared_ptr<unit> ptr;
		private:
			std::list<component_ptr> m_components;

		public:
			unit() {}
			virtual ~unit()
			{
				clear();
			}

		public:
			void add(component_ptr c)
			{
				m_components.push_back(c);
				c->bind(this);
			}

			// remove specified component (O=n)
			void remove(component_ptr c)
			{
				m_components.remove(c);
				c->bind(nullptr);
			}

			// remove all components
			void clear()
			{
				m_components.clear();
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
			std::shared_ptr<_C> component() const
			{
				std::shared_ptr<_C> cast;
				for (auto &i : m_components)
				{
					cast = std::dynamic_pointer_cast<_C>(i);
					if (cast) break;
				}
				return cast;
			}
			template<typename _C>
			bool remove()
			{
				for (auto it = m_components.begin(), last = m_components.end(); it != last; ++it)
				{
					auto cast = std::dynamic_pointer_cast<_C>(*it);
					if (cast)
					{
						m_components.erase(it);
						return true;
					}
				}
				return false;
			}
		};
	}
}

#endif