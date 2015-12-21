// name: control.h
// type: c++ header
// desc: interface declaration
// auth: is0urce

// intefcace for control in main mvc pattern
// functions return true if interaction handled

#ifndef PX_CONTROL_H
#define PX_CONTROL_H

#include <px/point.hpp>

namespace px
{
	enum class key : int;
	class control
	{
	public:
		typedef key key_t;

	protected:
		virtual bool key_control(key_t code) { return false; }
		virtual bool hover_control(const point &position) { return false; }
		virtual bool click_control(const point &position, unsigned int button) { return false; }
		virtual bool scroll_control(int delta) { return false; }

	public:
		bool key(key_t code) { return key_control(code); }
		bool hover(const point &position) { return hover_control(position); }
		bool click(const point &position, unsigned int button) { return click_control(position, button); }
		bool scroll(int delta) { return scroll_control(delta); }

		virtual ~control() {}
	};
}

#endif