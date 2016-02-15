// name: scene.cpp
// type: c++ source file
// desc: 'scene' class implementation
// auth: is0urce

#include "scene.h"

namespace px
{
	namespace rl
	{
		scene::scene() : m_graph(0, 0, 64)
		{
			m_default.traversable(true);
		}

		scene::~scene()
		{
		}
	}
}