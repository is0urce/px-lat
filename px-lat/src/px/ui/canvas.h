// name: canvas.h
// type: c++
// desc: class declaration
// auth: isource

#ifndef PX_UI_CANVAS_H
#define PX_UI_CANVAS_H

#include <px/point.hpp>
#include <px/map.hpp>
#include <px/rectangle.hpp>
#include <px/ui/symbol.h>

namespace px
{
	namespace ui
	{
		class canvas : public map<symbol>
		{
		private:
			color m_front;
			color m_back;

		public:
			canvas(point range);
			virtual ~canvas();

		public:
			void text_color(color front_color);
			void draw_color(color back_color);

			void cls();
			void cls(const symbol &fill);
			void rectangle(const px::rectangle &bounds);
			void rectangle(const px::rectangle &bounds, const color &back);
			void pset(const point &position);
			void pset(const point &position, const color &back);

			void write(const point &position, symbol::code_t code);
			void write(const point &position, symbol::code_t code, const color &front);
			void write(point position, const std::string &text);
			void write(point position, const std::string &text, const color &front);
		};
	}
}

#endif