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
		//qtree(int x, int y, unsigned int range, ptr ne, ptr nw, ptr se, ptr sw) : m_center_x(x), m_center_y(y), m_range(range)
		//{
		//	this->ne = ne;
		//	this->nw = nw;
		//	this->se = se;
		//	this->sw = sw;
		//}

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
				auto result = std::make_unique<qtree<element>>(m_center_x + ((x >= m_center_x) ? range : -range), m_center_y + ((y >= m_center_y) ? range : -range), range);
				std::swap(branch, result);
			}
			return branch;
		}

	public:
		// partition contains point in range, there could be no elements
		bool contains(int x, int y)
		{
			return (std::abs(x - m_center_x) <= (int)m_radius) && (std::abs(y - m_center_x) <= (int)m_radius);
		}
		void add(int x, int y, element e)
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

					add(x, y, e);
				}
			}
			else
			{
				auto &branch = access(x, y);
				branch->add(x, y, e);
			}
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
		void find(int x, int y, unsigned int radius, std::function<void(int, int, element&)> fn) const
		{
			if (m_bucket && m_bucket->inside(x, y, radius))
			{
				for (auto it = m_bucket->list.begin(), last = m_bucket->list.end(); it != last; ++it)
				{
					fn(m_bucket->x, m_bucket->y, *it);
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
		void expand()
		{
			int range = m_range;
			if (nw)
			{
				auto nw_expand = std::make_unique<qtree<element>>(m_center_x - range, m_center_y + range, range);
				if (nw->m_bucket)
				{
					std::swap(nw->m_bucket, nw_expand->m_bucket);
					m_bucket.reset();
				}
				else
				{
					std::swap(ne_expand.se, ne);
				}
				std::swap(ne, ne_expand);
			}
			//qtree(int x, int y, unsigned int range, ptr ne, ptr nw, ptr se, ptr sw) : m_center_x(x), m_center_y(y), m_range(range)
			//{
			//	this->ne = ne;
			//	this->nw = nw;
			//	this->se = se;
			//	this->sw = sw;
			//}
		}
		std::string info() const
		{
			auto result = std::string("qtree (") + std::to_string(m_center_x) + std::string(",") + std::to_string(m_center_y) + std::string(") radius=") + std::to_string(m_range);
			if (m_bucket) result += std::string(" has bucket size") + std::to_string(m_bucket->list.size());
			if (nw) result += std::string(" nw=") + nw->info();
			if (ne) result += std::string(" ne=") + ne->info();
			if (sw) result += std::string(" sw=") + sw->info();
			if (se) result += std::string(" se=") + se->info();
			return result;
		}
	};
}

#endif