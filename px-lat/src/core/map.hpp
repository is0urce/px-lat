// name: map.h
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
		rectangle m_rectangle;
		std::unique_ptr<_T[]> m_tiles;

		// ctor & dtor

	public:
		map(point::component x, point::component y)
		{
			init(point(x, y));
		}
		map(point::component x, point::component y, const element &initial)
		{
			init(point(x, y));
			fill(initial);
		}
		map(point range)
		{
			init(range);
		};
		map(point range, const element &initial)
		{
			init(range);
			fill(initial);
		}
		map(point range, std::function<element(const point&)> generator_fn)
		{
			init(range);
			fill_indexed(generator_fn);
		};
		~map() {};
	private:
		map(const map&) = delete; // disable copy

	private:
		void init(point range) // constructor helper
		{
			m_rectangle = { {0, 0}, range };
			if (range.X <= 0 || range.Y <= 0) throw std::logic_error("map<_T>::init(point range) invalid range (x or y <= 0)");
			m_tiles.reset(new _T[range.X * range.Y]);
		}

		// methods

	public:
		void swap(map &that)
		{
			std::swap(m_range, that.m_range);
			std::swap(m_tiles, that.m_tiles);
		}

		point::component width() const { return m_rectangle.width(); };
		point::component height() const { return m_rectangle.height(); };
		point::component size() const { return m_rectangle.width() * m_rectangle.height(); };

		//const point& range() const { return m_range; };
		bool in_range(const point &position) const { return m_rectangle.contains(position); };
		void resize(int w, int h)
		{
			resize(point(w, h));
		}
		void resize(const point &range)
		{
			if (m_range == range) return;

			swap(map(range));
		}
		const _T& at(const point &position, const _T& outer) const
		{
			return in_range(position) ? m_tiles[m_range.X * position.Y + position.X] : outer;
		}
		_T& at(const point &position, _T& outer)
		{
			return in_range(position) ? m_tiles[m_range.X * position.Y + position.X] : outer;
		}
		const _T& at(const point& position) const
		{
			if (!in_range(position)) throw std::logic_error("const _T& map<_T>::at(point position) - argument out of range");
			return m_tiles[m_range.X * position.Y + position.X];
		}
		_T& at(const point& position)
		{
			if (!in_range(position)) throw std::logic_error("_T& map<_T>::at(point position) - argument out of range");
			return m_tiles[m_range.X * position.Y + position.X];
		}
		void fill(const _T &tile)
		{
			auto len = size();
			for (int i = 0; i < len; ++i)
			{
				m_tiles[i] = tile;
			}
		};
		void fill(const map &source)
		{
			if (source.m_range != m_range) throw std::logic_error("void fill(const map &source) different ranges");

			int len = size();
			for (int i = 0; i < len; ++i)
			{
				m_tiles[i] = source.m_tiles[i];
			}
		}
		template<typename _C>
		void fill(const map<_C>& map, std::function<element(const _C&)> converter_fn)
		{
			if (map.m_rectangle != m_rectangle) throw std::logic_error("void fill(const map<_C>& map, std::function<element(const _C&)> converter_fn) different ranges");
			if (!converter_fn) throw std::logic_error("void fill(const map<_C>& map, std::function<element(const _C&)> converter_fn) - converter empty");

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

			m_rectangle.enumerate([&](const point &position)
			{
				at(position) = generator_fn(position);
			});
		};
	};
}