// name: component.hp
// type: c++ header
// desc: class definition
// auth: is0urce

#ifndef PX_ES_COMPONENT_H
#define PX_ES_COMPONENT_H

#include <array>
#include <list>
#include <memory>
#include <mutex>
#include <vector>

namespace px
{
	namespace es
	{
		class unit;

		class component_base
		{
		private:
			unit* m_unit;
			bool m_enabled;

		public:
			component_base()
			{
			}
			virtual ~component_base()
			{
			}

		protected:
			virtual void destroy_component() = 0;
			virtual void disable_component() = 0;
			virtual void enable_component() = 0;

		public:
			bool enabled() const
			{
				return m_enabled;
			}
			void enable()
			{
				enable_component();
				m_enabled = true;
			}
			void disable()
			{
				disable_component();
				m_enabled = false;
			}
			void destroy()
			{
				destroy_component();
			}
			void bind(unit* entity)
			{
				m_unit = entity;
			}
			unit* entity()
			{
				return m_unit;
			}
		};

		template<typename _M>
		class component : public component_base
		{
		private:
			typedef typename _M::key key_t;
		private:
			_M* m_manager;
			key_t m_key;

		public:
			component()
			{
			}
			virtual ~component()
			{
			}

		protected:
			virtual void destroy_component() override
			{
				m_manager->destroy(m_key);
			}
			virtual void disable_component() override
			{
			}
			virtual void enable_component() override
			{
			}

		public:
			void manage(_M* manager, key_t key)
			{
				m_manager = manager;
				m_key = key;
			}
			void manage(_M* manager)
			{
				m_manager = manager;
			}
			void manage(key_t key)
			{
				m_key = key;
			}
		};
	}
}

#endif