// name: scene.hp
// type: c++ header
// desc: 'scene' class definition
// auth: is0urce

#ifndef PX_SHELL_SCENE_H
#define PX_SHELL_SCENE_H

#include <px/rl/iscene.hpp>

#include <memory>

namespace px
{
	namespace es
	{
		class unit;
	}
	namespace rl
	{
		class scene : public iscene<std::shared_ptr<es::unit>>
		{
		public:
			scene();
			virtual ~scene();
		};
	}
}

#endif