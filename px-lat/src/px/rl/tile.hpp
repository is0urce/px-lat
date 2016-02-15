// name: tile.hpp
// type: c++ header
// desc: tile class
// auth: is0urce

#ifndef PX_RL_TILE_HPP
#define PX_RL_TILE_HPP

#include <px/rl/tile_base.hpp>
#include <px/rl/traverse.hpp>

namespace px
{
	namespace rl
	{
		class tile : public tile_base<(unsigned int)traverse::max_value>
		{

		};
	}
}

#endif