// name: image.h
// type: c++ header
// desc: class declaration
// auth: is0urce

#ifndef PX_SHELL_IMAGE_H
#define PX_SHELL_IMAGE_H

#include <px/color.hpp>

namespace px
{
	namespace shell
	{
		struct image
		{
			unsigned int atlas;
			float left, right, bottom, top;
			float width, height;
			color tint;
			double transparency;
			unsigned int alternative_ascii;
		};
	}
}

#endif