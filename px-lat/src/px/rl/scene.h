// name: scene.hp
// type: c++ header
// desc: 'scene' class definition
// auth: is0urce

#ifndef PX_RL_SCENE_H
#define PX_RL_SCENE_H

#include <px/rl/iscene.hpp>
#include <px/rl/tile_base.hpp>
#include <px/rl/traverse.hpp>
#include <px/point.hpp>

#include <memory>

namespace px
{
	namespace es
	{
		class unit;
	}
	namespace rl
	{
		class tile : public tile_base<(unsigned int)traverse::max_value>
		{

		};
		class scene : public iscene<std::shared_ptr<es::unit>>
		{
		private:
			tile m_default;

		public:
			scene();
			virtual ~scene();

		public:
			const tile& select(const point &position)
			{
				return m_default;
			}
			bool transparent(const point &point)
			{
				return select(point).transparent();
			}
			bool traversable(const point &point, traverse layer)
			{
				return select(point).traversable(layer);
			}
		};
	}
}

#endif