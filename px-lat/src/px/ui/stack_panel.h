// name: stack_panel.h
// type: c++
// desc: class declaration
// auth: is0urce

// generic container for panels

#ifndef PX_UI_STACK_PANEL_H
#define PX_UI_STACK_PANEL_H

#include <px/ui/panel.h>

#include <memory>
#include <map>
#include <string>

namespace px
{
	namespace ui
	{
		class stack_panel : public panel
		{
		public:
			typedef std::string panel_id;
			typedef std::shared_ptr<stack_panel> panel_ptr;
			struct stacked_panel
			{
				panel_ptr panel;
				alignment align;
				stacked_panel(panel_ptr panel, alignment align)
					: panel(panel), align(align)
				{
				}
			};
			typedef std::map<panel_id, stacked_panel> stack_t;

		private:
			stack_t m_stack;
			rectangle m_bounds;

		public:
			stack_panel();
			virtual ~stack_panel();

		protected:
			virtual bool key_control(key_t code) override;
			virtual bool hover_control(const point &position) override;
			virtual bool click_control(const point &position, unsigned int button) override;
			virtual bool scroll_control(int delta) override;
			virtual void draw_panel(canvas&) override;

		public:
			void add(panel_id name_tag, panel_ptr panel, alignment align);
			void remove(const panel_id &name_tag);
			void disable(const panel_id &name_tag);
			void enable(const panel_id &name_tag);
			void toggle(const panel_id &name_tag);
			bool enabled(const panel_id &name_tag) const;
			panel_ptr at(const panel_id &name_tag);
			void layout(rectangle bounds);
			void layout();
		};
	}
}

#endif