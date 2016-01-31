// name: component.hp
// type: c++ header
// desc: class definition
// auth: is0urce

#ifndef PX_RL_COMPONENT_H
#define PX_RL_COMPONENT_H

#include <array>
#include <list>
#include <memory>
#include <vector>

namespace px
{
	namespace rl
	{
		class unit;
		class component_base
		{
		private:
			unit* m_unit = nullptr;
			bool m_enabled = false;

		public:
			component_base()
			{
			}
			virtual ~component_base()
			{
			}

		protected:
			virtual void destroy_component() = 0;

		public:
			bool enabled() const
			{
				return m_enabled;
			}
			void enable()
			{
				m_enabled = true;
			}
			void disable()
			{
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

		//template<typename _C, unsigned int _B>
		//class component_manager;

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

		public:
			void manage(_M* manager, key_t key)
			{
				m_manager = manager;
				m_key = key;
			}
			void manage(key_t key)
			{
				m_key = key;
			}
		};

		template<typename _C, unsigned int _B>
		class component_manager
		{
		public:
			typedef _C element;
			typedef std::array<element, _B> container_t;
			struct batch
			{
			public:
				container_t elements;
				std::list<unsigned int> recycle;
				unsigned int cursor = 0;
			};
			typedef std::list<batch> batch_t;
			typedef typename batch_t::iterator batch_it;
			struct key
			{
			public:
				batch_it it;
				unsigned int cursor;
				key()
				{

				}
				key(batch_it batch_iterator, unsigned int index) : it(batch_iterator), cursor(index)
				{

				}
			};

		private:
			batch_t m_batches;

		public:
			component_manager()
			{
				m_batches.emplace_back();
			}

		public:
			_C* create()
			{
				// select batch
				auto it = m_batches.begin();
				auto last = m_batches.end();
				while (it != last && it->recycle.empty() && it->cursor == _B)
				{
					++it;
				}
				if (it == last)
				{
					m_batches.emplace_back();
					it = m_batches.end();
				}
				batch &b = *it;
				unsigned int index = 0;
				_C* result = nullptr;
				if (!b.recycle.empty())
				{
					auto it = b.recycle.end();
					unsigned int index = *it;
					b.recycle.erase(it);

					// re-construct
					//new (result) _C;
				}
				else
				{
					index = b.cursor;

					// increment cursor
					++b.cursor;
				}

				result = &(b.elements[index]);
				result->manage(this, { it, index });
				return result;
			}
			void destroy(key k)
			{
				//batch &b = *(k.it);
				//b.elements[k.cursor].~_C();
			}
		};
		
		struct my_pov
		{
			std::string boolable;
			std::string get_custom_value() const { return boolable; }
			my_pov()
			{
				boolable = "zxcvb";
			}
		};

		class my_component
			: public my_pov
			, public component<component_manager<my_component, 10>>
		{
		public:
			virtual ~my_component()
			{

			}
		};
	}
}

#endif