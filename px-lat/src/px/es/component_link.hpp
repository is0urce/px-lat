// name: component_link.hpp
// type: c++ header
// desc: class definition
// auth: is0urce

#ifndef PX_ES_COMPONENT_LINK_H
#define PX_ES_COMPONENT_LINK_H

namespace px
{
	namespace es
	{
		template<typename _L>
		class component_link
		{
		protected:
			_L* m_link;
		public:
			component_link() {}
			_L* link() const { return m_link; }
			void link(_L* link) { m_link = link; }
			explicit operator _L*() const { return m_link; }
		};
	}
}

#endif