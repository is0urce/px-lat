// name: image.h
// type: c++ header
// desc: class declaration
// auth: is0urce

#ifndef PX_SHELL_IMAGE_H
#define PX_SHELL_IMAGE_H

#include <px/color.h>

namespace px
{
	namespace shell
	{
		struct image
		{
		public:
			typedef float real_t;

		public:
			unsigned int atlas;
			real_t x;
			real_t y;
			real_t dx;
			real_t dy;
		};
	}
}

#endif