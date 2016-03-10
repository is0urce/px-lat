// name: map_stream.hp
// type: c++ header
// desc: class definition
// auth: is0urce

#ifndef PX_RL_MAP_STREAM_HPP
#define PX_RL_MAP_STREAM_HPP

#include <px/es/unit.h>
#include <px/rl/tile.hpp>

#include <px/point.hpp>
#include <px/map.hpp>

#include <memory>
#include <thread>

namespace px
{
	namespace rl
	{
		class map_stream
		{
		public:
			typedef map<tile> map_t;
			typedef std::shared_ptr<es::unit> unit_ptr;
			typedef std::list<unit_ptr> unit_list;

		private:
			map<tile>* m_map;
			std::list<std::shared_ptr<es::unit>> m_units;

			bool m_ready;
			std::thread m_thread;

		public:
			map_stream() : m_ready(false)
			{
			}
			virtual ~map_stream()
			{
			}

		private:
			void process(std::function<void()> fn)
			{
				fn();
				m_ready = true;
			}
			void start(std::function<void(map_t*, unit_list&)> fn)
			{
				m_thread = std::thread(&map_stream::process, this, [&]() { fn(m_map, m_units); });
			}

		public:
			bool ready() const
			{
				return m_ready;
			}
			map_t* operator->()
			{
				return m_map;
			}
			unit_list& acquire()
			{
				m_thread.join();
				return m_units;
			}
		};
	}
}

#endif