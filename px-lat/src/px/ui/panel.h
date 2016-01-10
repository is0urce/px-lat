// name: panel.h
// type: c++
// desc: base class declaration
// auth: is0urce

#ifndef PX_UI_PANEL_H
#define PX_UI_PANEL_H

#include <px/point.hpp>
#include <px/control.h>

#include <stdexcept>

namespace px
{
	namespace ui
	{
		class canvas;
		class panel : public control
		{
		protected:
			bool m_visible;

		public:
			panel();
			virtual ~panel();

		protected:
			virtual void draw_panel(canvas&) = 0;

		public:
			bool visible() const;
			void visible(bool enabled);
			void enable();
			void disable();
			bool toggle();
			void draw(canvas&);

			canvas* operator->();
		};
	}
}

#endif