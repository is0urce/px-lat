// name: stack_panel.cpp
// type: c++
// desc: class declaration
// auth: is0urce

// main container for panels

#include "stack_panel.h"

#include <sstream>

namespace px
{
	namespace ui
	{
		stack_panel::stack_panel()
		{
		}

		stack_panel::~stack_panel()
		{
		}
		template<typename _O>
		bool stack_panel::panel_action(_O act)
		{
			for (auto p : m_stack)
			{
				if (p.second.panel && p.second.panel->visible() && act(p.second)) return true;
			}
			for (auto p : m_list)
			{
				if (p.panel && p.panel->visible() && act(p)) return true;
			}
			return false;
		}

		bool stack_panel::key_control(key_t code)
		{
			return panel_action([&](stacked_panel& p) { return p.panel->key(code); });
		}
		bool stack_panel::hover_control(const point &position)
		{
			return panel_action([&](stacked_panel& p) { return p.panel->hover(position); });
		}
		bool stack_panel::click_control(const point &position, unsigned int button)
		{
			return panel_action([&](stacked_panel& p) { return p.panel->click(position, button); });
		}
		bool stack_panel::scroll_control(int delta)
		{
			return panel_action([&](stacked_panel& p) { return p.panel->scroll(delta); });
		}
		void stack_panel::draw_panel(canvas& table)
		{
			panel_action([&](stacked_panel& p) { p.panel->draw(table); return true; }); // return false to not cancel loops
		}

		void stack_panel::add(panel_id name_tag, panel_ptr panel, alignment align)
		{
			remove(name_tag);
			panel->layout(m_bounds);
			m_stack.emplace(name_tag, stacked_panel(panel, align));
		}
		void stack_panel::add(panel_ptr panel, alignment align)
		{
			panel->layout(m_bounds);
			m_list.emplace_back(panel, align);
		}

		void stack_panel::remove(const panel_id &name_tag)
		{
			auto find = m_stack.find(name_tag);
			if (find != m_stack.end())
			{
				m_stack.erase(find);
			}
		}

		stack_panel::panel_ptr stack_panel::at(const panel_id &name_tag)
		{
			auto find = m_stack.find(name_tag);
			if (find == m_stack.end()) throw std::logic_error("px::ui::stack_panel::at(panel_id) panel not found");
			return find->second.panel;
		}

		void stack_panel::disable(const panel_id &name_tag)
		{
			auto find = m_stack.find(name_tag);
			if (find != m_stack.end())
			{
				panel &p = *find->second.panel;
				p.disable();
			}
		}

		void stack_panel::enable(const panel_id &name_tag)
		{
			auto find = m_stack.find(name_tag);
			if (find != m_stack.end())
			{
				panel &p = *find->second.panel;
				p.enable();
			}
		}
		void stack_panel::toggle(const panel_id &name_tag)
		{
			auto find = m_stack.find(name_tag);
			if (find != m_stack.end())
			{
				panel &p = *find->second.panel;
				p.toggle();
			}
		}
		bool stack_panel::enabled(const panel_id &name_tag) const
		{
			auto find = m_stack.find(name_tag);
			return find == m_stack.end() ? false : find->second.panel->visible();
		}
		void stack_panel::layout(rectangle bounds)
		{
			m_bounds = bounds;
			layout();
		}
		void stack_panel::layout()
		{
			panel_action([&](stacked_panel& p) { p.panel->m_bounds = p.align.calculate(m_bounds); p.panel->layout(); return false; });
		}
		rectangle stack_panel::bounds() const
		{
			return m_bounds;
		}
		void stack_panel::clear()
		{
			m_stack.clear();
		}

		std::string stack_panel::info() const
		{
			std::stringstream ss("");
			ss << "childs=" << m_stack.size() << " ";
			ss << "bounds=[start=[" << m_bounds.start().X << "," << m_bounds.start().Y << "], range=[" << m_bounds.range().X << ", " << m_bounds.range().Y << "]]";
			return ss.str();
		}
	}
}