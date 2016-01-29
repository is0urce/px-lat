#include "board_panel.h"

#include <px/ui/canvas.h>

namespace px
{
	namespace ui
	{
		board_panel::board_panel()
		{
		}
		board_panel::board_panel(color bg)
			: m_background(bg)
		{
		}
		board_panel::~board_panel()
		{
		}

		void board_panel::draw_panel(canvas& c)
		{
			c.rectangle(bounds(), m_background);
		}
		color board_panel::background() const
		{
			return m_background;
		}
		void board_panel::background(color bg)
		{
			m_background = bg;
		}
	}
}