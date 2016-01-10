// name: panel.cpp
// type: c++
// desc: abstract class methods implementation
// auth: is0urce

#include "panel.h"
#include "canvas.h"

namespace px
{
	namespace ui
	{

		panel::panel() : m_visible(true)
		{
		}

		panel::~panel()
		{
		}

		void panel::draw(canvas& cnv)
		{
			draw_panel(cnv);
		}

		bool panel::visible() const
		{
			return m_visible;
		}

		void panel::visible(bool enabled)
		{
			m_visible = enabled;
		}

		void panel::enable()
		{
			visible(true);
		}

		void panel::disable()
		{
			visible(false);
		}

		bool panel::toggle()
		{
			return m_visible = !m_visible;
		}
	}
}