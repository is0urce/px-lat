// name: location.hpp
// type: c++ header
// desc: struct
// auth: is0urce

#ifndef PX_RL_LOCATION_H
#define PX_RL_LOCATION_H

#include <px/point.hpp>

namespace px
{
	namespace rl
	{
		struct location
		{
			point m_position;
			bool transparent;
			bool blocking;

			const point& position() const
			{
				return m_position;
			}
			int x() const
			{
				return m_position.X;
			}
			int y() const
			{
				return m_position.Y;
			}
		};
	}
}

#endif