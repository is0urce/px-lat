// name: traverse.hpp
// type: c++ header
// desc: enum
// auth: is0urce

// traverse layers indices

#ifndef PX_RL_TRAVERSE_HPP
#define PX_RL_TRAVERSE_HPP

namespace px
{
	enum class traverse : unsigned int
	{
		walk,
		swim,
		fly,
		teleport,

		max_value = teleport
	};
}

#endif