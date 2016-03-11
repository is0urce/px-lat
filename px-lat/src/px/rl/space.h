// name: space.h
// type: c++ header
// desc: class definition
// auth: is0urce

// space partition and manager

#ifndef PX_RL_SPACE_H
#define PX_RL_SPACE_H

#include <px/rl/location_component.hpp>

#include <px/point.hpp>
#include <px/qtree.hpp>

#include <memory>

namespace px
{
	namespace rl
	{
		class space
		{
		private:
			qtree<location_manager::element*> m_graph;
			location_manager m_locations;

		public:
			space();
			virtual ~space();

		public:
			bool transparent(const point &position) const;
			bool traversable(const point &position) const;
			std::shared_ptr<location_manager::element> make_location(point position);
		};
	}
}

#endif