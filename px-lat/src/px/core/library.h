// name: library.h
// type: c++ header
// desc: class declaration
// auth: is0urce

#ifndef PX_CORE_LIBRARY_H
#define PX_CORE_LIBRARY_H

#include <px/es/unit.h>
#include <px/shell/sprite_manager.hpp>
#include <px/shell/image.h>

#include <memory>

namespace px
{
	namespace core
	{
		class library
		{
		public:
			//typedef shell::sprite_manager img_manager;
		private:
			shell::sprite_manager *m_sprites;
			shell::location_manager m_location;

		public:
			library(shell::sprite_manager *image_manager) : m_sprites(image_manager)
			{
			}
			virtual ~library() {}

		public:
			std::shared_ptr<es::unit> create()
			{
				// create
				auto sprite = m_sprites->create();
				auto location = m_location.create();

				// link
				sprite->link(location);

				// put in container
				auto result = std::make_shared<es::unit>();
				result->add(sprite);
				result->add(location);
				return result;
			}
			shell::sprite_manager::element* make_image(unsigned int u_plus)
			{
				auto s = m_sprites->create();
				s->alternative_ascii = u_plus;
				return s;
			}
		};
	}
}

#endif