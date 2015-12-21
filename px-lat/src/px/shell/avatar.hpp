// name: avatar.h
// type: c++ header
// desc: class declaration
// auth: is0urce

// represents on-screen proprieties of a unit

#ifndef PX_SHELL_AVATAR_H
#define PX_SHELL_AVATAR_H

#include <px/vector.hpp>

namespace px
{
	namespace shell
	{
		template <typename _I>
		class avatar
		{
		public:
			typedef _I image_t;

		public:
			image_t img;
			vector position;
			double size;
		};
	}
}

#endif