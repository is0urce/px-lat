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
			bool inside(int x0, int y0) const
			{
				return x == x0 && y == y0;
			}
		};

	private:
		int m_center_x;
		int m_center_y;
		unsigned int m_range;

		ptr nw, ne, se, sw;

		std::unique_ptr<bucket> m_bucket;

	public:
		qtree(int x, int y, unsigned int range)
		{
			m_center_x = x;
			m_center_y = y;
			m_range = range;
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
				auto result = std::make_unique<qtree<element>>(m_center_x + ((x >= m_center_x) ? range : -range), m_center_y + ((y >= m_center_y) ? range : -range), range);
				std::swap(branch, result);
			}
			return branch;
		}

	public:
		void add(int x, int y, element e)
		{
			if (m_bucket)
			{
				if (m_bucket->list.empty() || m_bucket->inside(x, y))
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

					add(x, y, e);
				}
			}
			else
			{
				auto &branch = access(x, y);
				branch->add(x, y, e);
			}
		}
		//remove(int x, int y, element e)
		//{
		//	if (m_bucket)
		//	{
		//		if (x == m_bucket->x && y == m_bucket->y)
		//		{
		//			m_bucket->list.remove(e);
		//		}
		//		else
		//		{
		//			throw std::runtime_error("qtree::remove - bucket not match");
		//		}
		//	}
		//	else
		//	{
		//		auto branch = select(x, y);
		//		if (!branch)
		//		{
		//			throw std::runtime_error("qtree::remove - no branch");
		//		}
		//		branch->remove(x, y, e);
		//	}
		//}
		//find(int x, int y, unsigned int radius, std::function<void(element&)> fn);
	};
}

#endif