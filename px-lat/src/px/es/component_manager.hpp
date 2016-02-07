// name: component.hp
// type: c++ header
// desc: class definition
// auth: is0urce

#ifndef PX_ES_COMPONENT_MANAGER_H
#define PX_ES_COMPONENT_MANAGER_H

#include <array>
#include <list>
#include <memory>
#include <mutex>
#include <vector>

namespace px
{
	namespace es
	{
		template<typename _C>
		class manager_base
		{
		public:
			manager_base() {}
			virtual ~manager_base() {}

		protected:
			virtual _C* create_component() = 0;
			virtual void update_manager() {}

		public:
			std::shared_ptr<_C> make_shared()
			{
				return std::shared_ptr<_C>(create_component(), [](_C* pointer) { pointer->destroy(); });
			}
			_C* create()
			{
				return create_component();
			}
		};

		template<typename _C, unsigned int _B>
		class component_manager : public manager_base<_C>
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

		protected:
			_C* create_component()
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
				auto &b = *it;

				// select index
				unsigned int index = (b.recycled() > 0) ? b.recycle() : b.increment();

				// component management setup
				_C &result = b[index];
				result.disable();
				result.bind(nullptr);
				result.manage([this, k = key(it, index)](){ destroy(k); });

				++m_count;

				return &result;
			}

		public:
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
	}
}

#endif