// name: map.hpp
// type: c++ header
// desc: class declaration
// auth: is0ruce

// generic 2-d table container

#pragma once

#include "rectangle.hpp"

#include <functional>
#include <memory>
#include <stdexcept>

namespace px
{
	template <typename _T>
	class map
	{
		// friend with maps with different element type
		template<typename _C>
		friend class map;

	public:
		typedef _T element;

		// attributes

	private:
		point::component m_width;
		point::component m_height;
		std::unique_ptr<_T[]> m_tiles;

		// ctor & dtor

	public:
		map(point::component width, point::component height)
		{
			init(width, height);
		}
		map(point::component x, point::component y, const element &initial)
		{
			init(x, y);
			fill(initial);
		}
		map(point range)
		{
			init(range.X, range.Y);
		};
		map(point range, const element &initial)
		{
			init(range.X, range.Y);
			fill(initial);
		}
		map(point range, std::function<element(const point&)> generator_fn)
		{
			init(range.X, range.Y);
			fill_indexed(generator_fn);
		};
		~map()
		{
		};
	private:
		map(const map&) = delete; // disable copy

	private:
		void init(point::component w, point::component h) // constructor helper
		{
			if (w <= 0 || h <= 0) throw std::logic_error("map<_T>::init(point range) invalid range (x or y <= 0)");

			m_width = w;
			m_height = h;
			m_tiles.reset(new _T[w * h]);
		}

		// methods

	public:
		void swap(map &that)
		{
			std::swap(m_width, that.m_width);
			std::swap(m_height, that.m_height);
			std::swap(m_tiles, that.m_tiles);
		}

		point::component width() const { return m_width; };
		point::component height() const { return m_height; };
		point::component size() const { return m_width * m_height; };
		point range() const { return point(m_width, m_height); }

		bool contains(const point &position) const
		{
			return position.X >= 0 && position.Y >= 0 && position.X < m_width && position.Y < m_height;
		};
		void resize(const point &range)
		{
			resize(range.X, range.Y);
		}
		void resize(int w, int h)
		{
			if (m_width == w && m_height == h) return;

			swap(map(w, h));
		}

		const _T& at(const point &position, const _T& outer) const
		{
			return contains(position) ? m_tiles[m_width * position.Y + position.X] : outer;
		}
		_T& at(const point &position, _T& outer)
		{
			return contains(position) ? m_tiles[m_width * position.Y + position.X] : outer;
		}
		const _T& at(const point& position) const
		{
			if (!contains(position)) throw std::logic_error("const _T& map<_T>::at(point position) - argument out of range");
			return m_tiles[m_width * position.Y + position.X];
		}
		_T& at(const point& position)
		{
			if (!contains(position)) throw std::logic_error("_T& map<_T>::at(point position) - argument out of range");
			return m_tiles[m_width * position.Y + position.X];
		}

		void fill(const _T &tile)
		{
			int len = m_width * m_height;
			for (int i = 0; i < len; ++i)
			{
				m_tiles[i] = tile;
			}
		};
		void fill(const map &source)
		{
			if (source.m_width != w || source.m_height != m_height) throw std::logic_error("void fill(const map &source) different ranges");

			int len = m_width * m_height;
			for (int i = 0; i < len; ++i)
			{
				m_tiles[i] = source.m_tiles[i];
			}
		}
		template<typename _C>
		void fill(const map<_C>& map, std::function<element(const _C&)> converter_fn)
		{
			if (source.m_width != w || source.m_height != m_height) throw std::logic_error("fill(const map<_C>& map, std::function<element(const _C&)> converter_fn) - different ranges");
			if (!converter_fn) throw std::logic_error("fill(const map<_C>& map, std::function<element(const _C&)> converter_fn) - converter empty");

			int len = size();
			for (int i = 0; i < len; ++i)
			{
				m_tiles[i] = converter_fn(map.m_tiles[i]);
			}
		}
		void fill(std::function<element()> generator_fn)
		{
			if (!generator_fn) throw std::logic_error("void fill(std::function<element()> generator_fn) - generator empty");

			int len = size();
			for (int i = 0; i < len; ++i)
			{
				m_tiles[i] = generator_fn();
			}
		};
		void fill_indexed(std::function<element(const point&)> generator_fn)
		{
			if (!generator_fn) throw std::logic_error("void fill_indexed(std::function<element()> generator_fn) - generator empty");

			point index;
			unsigned int offset = 0;
			for (index.Y = 0; index.Y < m_height; ++index.Y)
			{
				for (index.X = 0; index.X < m_width; ++index.X)
				{
					m_tiles[offset] = generator_fn(index);
					++offset;
				}
			}
		};
	};
}