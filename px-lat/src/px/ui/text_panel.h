// name: stack_panel.h
// type: c++
// desc: class declaration
// auth: is0urce

// generic container for panels

#ifndef PX_UI_TEXT_PANEL_H
#define PX_UI_TEXT_PANEL_H

#include <px/ui/stack_panel.h>
#include <px/ui/canvas.h>
#include <px/color.hpp>

#include <string>

namespace px
{
	namespace ui
	{
		class text_panel : public stack_panel
		{
		private:
			color m_front;
			std::string m_text;

		public:
			text_panel() {}
			text_panel(std::string text, color front) : m_text(text), m_front(front) {}
			virtual ~text_panel() {}

		protected:
			virtual void draw_panel(canvas& c)
			{
				c.write(bounds().start(), m_text, m_front);
			}

		public:
			void color_front(color front) { m_front = front; }
			color color_front() const { return m_front; }
			void text(std::string text) { m_text = text; }
			std::string text() const { return m_text; }
		};
	}
}

#endif