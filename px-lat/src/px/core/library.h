// name: library.h
// type: c++ header
// desc: class declaration
// auth: is0urce

#ifndef PX_CORE_LIBRARY_H
#define PX_CORE_LIBRARY_H

#include <px/shell/sprite_manager.h>
#include <px/es/unit.h>

#include <memory>

namespace px
{
	namespace core
	{
		class library
		{
		private:
			shell::sprite_manager m_sprites;
			shell::location_manager m_location;

		public:
			library() {}
			virtual ~library() {}

		public:
			std::shared_ptr<es::unit> create()
			{
				// create
				auto sprite = m_sprites.create();
				auto location = m_location.create();

				// link
				sprite->link(location);

				// put in container
				auto result = std::make_shared<es::unit>();
				result->add(sprite);
				result->add(location);
				return result;
			}
		};
	}
}

#endif