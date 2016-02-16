// name: scene.cpp
// type: c++ source file
// desc: 'scene' class implementation
// auth: is0urce

#include "scene.h"

namespace px
{
	namespace rl
	{
		scene::scene() : m_graph(0, 0, 64), m_map(10, 10)
		{
			m_default.make_wall();

			tile t;
			t.make_ground();
			m_map.fill(t);
		}

		scene::~scene()
		{
		}
	}
}