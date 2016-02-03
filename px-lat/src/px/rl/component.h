// name: component.hp
// type: c++ header
// desc: class definition
// auth: is0urce

#ifndef PX_RL_COMPONENT_H
#define PX_RL_COMPONENT_H

#include <array>
#include <list>
#include <memory>
#include <mutex>
#include <vector>

namespace px
{
	namespace rl
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

		template<typename _C, unsigned int _B>
		class component_manager
		{
		public:
			typedef _C element;
			typedef std::array<element, _B> container_t;
			struct batch
			{
			private:
				container_t m_elements;
				std::list<unsigned int> m_recycle;
				unsigned int m_cursor = 0;

			public:
				unsigned int recycled() const
				{
					return m_recycle.size();
				}
				unsigned int count() const
				{
					return m_cursor - recycled();
				}
				unsigned int cursor() const
				{
					return m_cursor;
				}
				bool empty() const
				{
					return count() == 0;
				}
				bool full() const
				{
					return count() == _B;
				}
				void clear()
				{
					m_recycle.clear();
					m_cursor = 0;
				}
				void optimise()
				{
					if (count() == 0)
					{
						clear();
					}
				}
				const _C& operator[](unsigned int index) const
				{
					return m_elements[index];
				}
				_C& operator[](unsigned int index)
				{
					return m_elements[index];
				}
				unsigned int increment()
				{
					unsigned int index = m_cursor;
					++m_cursor;
					return index;
				}
				void recycle(unsigned int index)
				{
					m_elements[index].disable();
					m_recycle.push_back(index);

					// optimise batch
					optimise();
				}
				unsigned int recycle()
				{
					// use position of recycled component
					auto it = m_recycle.end();
					unsigned int index = *it;
					m_recycle.erase(it);
					return index;
				}
			};
			typedef std::list<batch> batch_t;
			typedef typename batch_t::iterator batch_it;
			struct key
			{
			public:
				batch_it batch;
				unsigned int cursor;
				key()
				{
				}
				key(batch_it batch_iterator, unsigned int index) : batch(batch_iterator), cursor(index)
				{
				}
			};

		private:
			batch_t m_batches;
			std::mutex m_mutex;
			unsigned int m_count = 0;

		public:
			component_manager()
			{
			}
			component_manager(const component_manager&) = delete;

		public:
			_C* create()
			{
				std::lock_guard<std::mutex> lock(m_mutex);

				// select batch
				auto it = m_batches.begin();
				auto last = m_batches.end();
				while (it != last && it->full())
				{
					++it;
				}
				// create new batch if no place available
				if (it == last)
				{
					it = m_batches.emplace(it);
				}
				batch &b = *it;

				// select index
				unsigned int index = (b.recycled() > 0) ? b.recycle() : b.increment();

				// component management setup
				_C &result = b[index];
				result.disable();
				result.bind(nullptr);
				result.manage(this, { it, index });

				++m_count;
				return &result;
			}
			void destroy(key k)
			{
				std::lock_guard<std::mutex> lock(m_mutex);

				k.batch->recycle(k.cursor);

				--m_count;
			}

			template<typename _O>
			void update(_O op)
			{
				for (auto it = m_batches.begin(), last = m_batches.end(); it != last; ++it)
				{
					batch &b = *it;
					for (unsigned int i = 0; i < b.cursor(); ++i)
					{
						auto &component = (*it)[i];
						if (component.enabled())
						{
							op(component);
						}
					}
				}
			}

			// clear unused batches
			void optimise()
			{
				auto it = m_batches.begin();
				auto last = m_batches.last();
				//++it; // skip first (i.e. 'root')
				while (it != last)
				{
					it->optimise();
					if (it->count() == 0)
					{
						++it;
						m_batches.erase(it);
					}
					else
					{
						++it;
					}
				}
			};

			// count created (including not enabled) components
			unsigned int count() const
			{
				return m_count;
			};
		};

		template<typename _L>
		class component_link
		{
		protected:
			_L* m_link;
		public:
			component_link() {}
			_L* link() const { return m_link; }
			void link(_L* link) { m_link = link; }
			explicit operator _L*() const { return m_link; }
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

		struct sprite
		{
			unsigned int atlas;
			float left, right, bottom, top;
			void* color;
			double transparency;
		};
		struct location
		{
			point position;
		};

		class sprite_component
			: public sprite
			, public component_link<location>
			, public component<component_manager<sprite_component, 100>>
		{
		public:
			sprite_component()
			{
			}
			virtual ~sprite_component()
			{

			}
		};

		class sprite_manager : public component_manager<sprite_component, 100>
		{
		private:
			unsigned int m_length;
			std::vector<float> colors;
			std::vector<float> textcoords;
			std::vector<float> vertice;
			std::vector<unsigned int> index;

			unsigned int m_version = 0;

		public:
			void construct()
			{
				m_length = 0;
				update([this](sprite_component &sc)
				{
					++m_length;
					location* l = (location*)sc;

					vertice[m_length + 0] = (float)(l->position.X);
					vertice[m_length + 1] = (float)(l->position.Y);
				});
			}
		};
	}
}

#endif