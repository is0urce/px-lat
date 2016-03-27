// name: sprite_manager.hpp
// type: c++ header
// desc: class definition
// auth: is0urce

#ifndef PX_SHELL_SPRITE_MANAGER_H
#define PX_SHELL_SPRITE_MANAGER_H

#include <px/es/component.hpp>
#include <px/es/component_manager.hpp>
#include <px/es/component_link.hpp>
#include <px/rl/location_component.hpp>
#include <px/shell/image.h>

namespace px
{
	namespace shell
	{
		class sprite_component
			: public shell::image
			, public es::component_link<rl::location_component>
			, public es::component_link<sprite_component>
			, public es::component
		{
			// lookup ambiguity define
		public:
			using es::component_link<rl::location_component>::link;
			using es::component_link<sprite_component>::link;

		public:
			sprite_component()
			{
			}
			virtual ~sprite_component()
			{
			}
		};

		class sprite_manager : public es::component_manager<sprite_component, 100>
		{
		public:
			sprite_manager() {}
			virtual ~sprite_manager() {}
		};
	}
}

#endif