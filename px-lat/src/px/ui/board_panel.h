// name: stack_panel.h
// type: c++
// desc: class declaration
// auth: is0urce

// generic container for panels

#ifndef PX_UI_BOARD_PANEL_H
#define PX_UI_BOARD_PANEL_H

#include <px/ui/stack_panel.h>
#include <px/color.h>

namespace px
{
	namespace ui
	{
		class board_panel : public stack_panel
		{
		private:
			color m_background;

		public:
			board_panel();
			board_panel(color bg);
			virtual ~board_panel();

		protected:
			virtual void draw_panel(canvas&);

		public:
			void background(color bg);
			color background() const;
		};
	}
}

#endif