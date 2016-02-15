// name: tile_base.hpp
// type: c++ header
// desc: tile class
// auth: is0urce

#ifndef PX_RL_TILE_BASE_HPP
#define PX_RL_TILE_BASE_HPP

#include <bitset>

namespace px
{
	template <unsigned int layers = 1>
	class tile_base
	{
	public:
		typedef std::bitset<layers> bitset;

		// attributes
	private:
		bool m_transparent;
		std::bitset<layers> m_traversable;

		// ctor & dtor
	public:
		tile_base() {}
		tile_base(bool transparent) : m_transparent(transparent) {}
		tile_base(bool is_transparent, bool is_traversable) : m_transparent(is_transparent) { traversable(is_traversable); }
		tile_base(bool is_transparent, bitset traversable) : m_transparent(is_transparent), m_traversable(traversable) {}

	public:

		// transparency accessors
		bool transparent() const { return m_transparent; }
		void transparent(bool val) { m_transparent = val; }

		// traversability props accessors
		bool traversable(unsigned int layer) const
		{
			return m_traversable.test(layer);
		}
		bool traversable() const
		{
			return traversable(0);
		}
		template <typename _E>
		bool traversable(_E enum_layer) const
		{
			return traversable((unsigned int)enum_layer);
		}
		bool traversable(bitset l) const
		{
			return (m_traversable & l).any();
		}
		void traversable(unsigned int layer, bool val)
		{
			m_traversable.set(layer, val);
		}
		template <typename _E>
		void traversable(_E enum_layer, bool val)
		{
			traversable((unsigned int)enum_layer, val);
		}
		void traversable(bool val)
		{
			traversable(0, val);
		}
		void traversable(bitset l, bool val)
		{
			if (val)
			{
				m_traversable |= l;
			}
			else
			{
				m_traversable &= ~l;
			}
		}

		// utility
		void make_wall() { traversable(false); transparent(false); }
		void make_ground() { traversable(false); transparent(false); }
	};
}

#endif