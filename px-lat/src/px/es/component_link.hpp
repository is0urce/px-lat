// name: component_link.hpp
// type: c++ header
// desc: class definition
// auth: is0urce

#ifndef PX_ES_COMPONENT_LINK_H
#define PX_ES_COMPONENT_LINK_H

#include <memory>

namespace px
{
	namespace es
	{
		template<typename _L>
		class component_link
		{
		public:
			typedef std::weak_ptr<_L> link_t;
		protected:
			link_t m_link;
		public:
			component_link() {}
			_L* link() const
			{
				auto l = m_link.lock();
				return l ? (l.get()) : nullptr;
			}
			void link(link_t link)
			{
				m_link = link;
			}
			explicit operator _L*() const
			{
				return link();
			}
		};
	}
}

#endif