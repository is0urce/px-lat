// name: qtree.h
// type: c++ header
// desc: class
// auth: is0urce

#ifndef PX_QTREE_H
#define PX_QTREE_H

#include <list>
#include <memory>
#include <functional>
#include <stdexcept>

#include <px/point.hpp>

namespace px
{
	template<typename _E>
	class qtree
	{
	public:
		typedef _E element;
		typedef std::unique_ptr<qtree> ptr;

		struct bucket
		{
			std::list<element> list;
			int x, y;
			bool match(int x0, int y0) const
			{
				return x == x0 && y == y0;
			}
			bool inside(int x0, int y0, unsigned int radius)
			{
				return (std::abs(x - x0) <= (int)radius) && (std::abs(y - y0) <= (int)radius);
			}
		};

	private:
		ptr nw, ne, se, sw; // leaves

		// partition data
		int m_center_x;
		int m_center_y;
		unsigned int m_range;

		std::unique_ptr<bucket> m_bucket; // value

	public:
		qtree(int x, int y, unsigned int range) : m_center_x(x), m_center_y(y), m_range(range)
		{
			m_bucket = std::make_unique<bucket>();
		}

	private:

		// select branch
		ptr& select(int x, int y)
		{
			if (x >= m_center_x)
			{
				if (y >= m_center_y)
				{
					return ne;
				}
				else
				{
					return se;
				}
			}
			else
			{
				if (y >= m_center_y)
				{
					return nw;
				}
				else
				{
					return sw;
				}
			}
		}

		// select branch, if none available, create one
		ptr& access(int x, int y)
		{
			auto &branch = select(x, y);
			if (!branch)
			{
				int range = m_range / 2;
				auto result = std::make_unique<qtree>(m_center_x + ((x >= m_center_x) ? range : -range), m_center_y + ((y >= m_center_y) ? range : -range), range);
				std::swap(branch, result);
			}
			return branch;
		}

		// partition contains point in range, there could be no elements
		bool contains(int x, int y) const
		{
			return (std::abs(x - m_center_x) <= (int)m_range) && (std::abs(y - m_center_x) <= (int)m_range);
		}

		// helper function for expansion
		// old - part for expanding (internal not null garantee)
		// expanded - bigger area, not null
		// update - area in bigger area for swap to old, is null
		void static expand(ptr& old, ptr& expanded, ptr& update)
		{
			expanded->m_bucket.reset();
			if (old->m_bucket)
			{
				std::swap(old->m_bucket, expanded->m_bucket);
				old->m_bucket.reset();
			}
			else
			{
				std::swap(update, old);
			}
			std::swap(old, expanded);
		}

		// expand range
		void expand()
		{
			int range = m_range;
			if (nw)
			{
				auto expand = std::make_unique<qtree>(m_center_x - range, m_center_y + range, range);
				qtree::expand(nw, expand, expand->se);
			}
			if (ne)
			{
				auto expand = std::make_unique<qtree>(m_center_x + range, m_center_y + range, range);
				qtree::expand(ne, expand, expand->sw);
			}
			if (sw)
			{
				auto expand = std::make_unique<qtree>(m_center_x - range, m_center_y - range, range);
				qtree::expand(sw, expand, expand->ne);
			}
			if (se)
			{
				auto expand = std::make_unique<qtree>(m_center_x + range, m_center_y - range, range);
				qtree::expand(se, expand, expand->nw);
			}
			m_range *= 2;
		}

		// adding element, internal guarantee x and y in range
		void insert(int x, int y, element e)
		{
			if (m_bucket)
			{
				if (m_bucket->list.empty() || m_bucket->match(x, y))
				{
					m_bucket->list.push_back(e);
					m_bucket->x = x;
					m_bucket->y = y;
				}
				else
				{
					// move bucket
					auto &branch = access(m_bucket->x, m_bucket->y);
					std::swap(m_bucket, branch->m_bucket);
					m_bucket.release();

					insert(x, y, e);
				}
			}
			else
			{
				auto &branch = access(x, y);
				branch->insert(x, y, e);
			}
		}

	public:
		void add(int x, int y, element e)
		{
			while (!contains(x, y))
			{
				expand();
			}
			insert(x, y, e);
		}

		void remove(int x, int y, element e)
		{
			if (m_bucket)
			{
				if (!m_bucket->match(x, y)) throw std::runtime_error("qtree::remove - bucket not match");

				auto it = m_bucket->list.begin();
				auto last = m_bucket->list.end();
				while (it != last)
				{
					if (*it == e)
					{
						it = m_bucket->list.erase(it);
						return;
					}
					++it;
				}

				throw std::runtime_error("qtree::remove - item not found");
			}
			else
			{
				auto &branch = select(x, y);

				if (!branch) throw std::runtime_error("qtree::remove - no branch");

				branch->remove(x, y, e);
			}
		}
		void find(int x, int y, unsigned int radius, std::function<bool(int, int, element)> fn) const
		{
			if (m_bucket && m_bucket->inside(x, y, radius))
			{
				for (auto it = m_bucket->list.begin(), last = m_bucket->list.end(); it != last; ++it)
				{
					if (!fn(m_bucket->x, m_bucket->y, *it)) return;
				}
			}
			else
			{
				bool w = x - (int)radius <= m_center_x;
				bool e = x + (int)radius >= m_center_x;
				bool n = y + (int)radius >= m_center_y;
				bool s = y - (int)radius <= m_center_y;
				if (n)
				{
					if (w && nw) nw->find(x, y, radius, fn);
					if (e && ne) ne->find(x, y, radius, fn);
				}
				if (s)
				{
					if (w && sw) sw->find(x, y, radius, fn);
					if (e && se) se->find(x, y, radius, fn);
				}
			}
		}
		void move(int sx, int sy, element e, int dx, int dy)
		{
			remove(sx, sy, e);
			add(dx, dy, e);
		}

		void move(point from, element e, point destination)
		{
			move(from.X, from.Y, e, destination.X, destination.Y);
		}
		void add(point position, element e)
		{
			add(position.X, position.Y, e);
		}
		void remove(point position, element e)
		{
			remove(position.X, position.Y, e);
		}

		std::string info() const
		{
			auto result = std::string("qT(") + std::to_string(m_center_x) + std::string(",") + std::to_string(m_center_y) + std::string(") R=") + std::to_string(m_range);
			if (m_bucket) result += std::string(" bucket size ") + std::to_string(m_bucket->list.size());
			if (nw) result += std::string(" nw=") + nw->info();
			if (ne) result += std::string(" ne=") + ne->info();
			if (sw) result += std::string(" sw=") + sw->info();
			if (se) result += std::string(" se=") + se->info();
			return result;
		}
	};
}

#endif