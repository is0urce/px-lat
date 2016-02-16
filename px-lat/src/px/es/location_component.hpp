// name: location_component.hpp
// type: c++ header
// desc: class definition
// auth: is0urce

#ifndef PX_ES_LOCATION_COMPONENT_H
#define PX_ES_LOCATION_COMPONENT_H

#include <px/es/component.hpp>
#include <px/es/component_manager.hpp>
#include <px/es/component_link.hpp>

#include <px/rl/location.hpp>

namespace px
{
	namespace es
	{
		class location_component
			: public rl::location
			, public es::component
		{
		public:
			location_component()
			{
			}
			virtual ~location_component()
			{
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