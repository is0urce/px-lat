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
			//m_graph.add(0, 0, nullptr);
			////m_graph.remove(0, 0, nullptr);
			//m_graph.add(8, 10, nullptr);
			////m_graph.add(-100, 100, nullptr);
			//m_graph.add(300,300, nullptr);
			//m_graph.find(9, 9, 3, [](int x, int y, es::unit*) { throw std::runtime_error(std::to_string(x) + std::string(":") + std::to_string(y)); });
			//throw std::runtime_error(m_graph.info());
		}

		scene::~scene()
		{
		}
	}
}