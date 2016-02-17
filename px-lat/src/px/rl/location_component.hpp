// name: location_component.hpp
// type: c++ header
// desc: class definition
// auth: is0urce

#ifndef PX_ES_LOCATION_COMPONENT_H
#define PX_ES_LOCATION_COMPONENT_H

#include <px/es/component.hpp>
#include <px/es/component_manager.hpp>
#include <px/es/component_link.hpp>

#include <px/rl/location_component.hpp>
#include <px/qtree.hpp>

namespace px
{
	namespace rl
	{
		class location_component
			: public es::component
		{
		private:
			point m_position;
			bool m_transparent;
			bool m_blocking;
			qtree<location_component*> *m_space;

		public:
			location_component()
			{
			}
			virtual ~location_component()
			{
			}

		protected:
			virtual void enable_component() override
			{
				if (m_space)
				{
					m_space->add(m_position, this);
				}
			}
			virtual void disable_component() override
			{
				if (m_space)
				{
					m_space->remove(m_position, this);
				}
			}

		public:
			void move(const point &destination)
			{
				if (enabled() && m_space)
				{
					m_space->move(m_position, this, destination);
				}
				m_position = destination;
			}

			void space(qtree<location_component*> *scene)
			{
				m_space = scene;
			}
			const point& position() const
			{
				return m_position;
			}
			bool transparent() const
			{
				return m_transparent;
			}
			bool blocking() const
			{
				return m_blocking;
			}
			void transparent(bool v)
			{
				m_transparent = v;
			}
			void blocking(bool v)
			{
				m_blocking = v;;
			}
			int x() const
			{
				return m_position.X;
			}
			int y() const
			{
				return m_position.Y;
			}
		};

		class location_manager : public es::component_manager<location_component, 100>
		{
		public:
			location_manager()
			{
			}
			virtual ~location_manager()
			{
			}
		};
	}
}

#endif