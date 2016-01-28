// name: stack_panel.cpp
// type: c++
// desc: class declaration
// auth: is0urce

// main container for panels

#include "stack_panel.h"

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

		bool stack_panel::key_control(key_t code)
		{
			for (auto p : m_stack)
			{
				if (p.second.panel->visible() && p.second.panel->key(code)) return true;
			}
			return false;
		}
		bool stack_panel::hover_control(const point &position)
		{
			for (auto p : m_stack)
			{
				if (p.second.panel->visible() && p.second.panel->hover(position)) return true;
			}
			return false;
		}
		bool stack_panel::click_control(const point &position, unsigned int button)
		{
			for (auto p : m_stack)
			{
				if (p.second.panel->visible() && p.second.panel->click(position, button)) return true;
			}
			return false;
		}
		bool stack_panel::scroll_control(int delta)
		{
			for (auto p : m_stack)
			{
				if (p.second.panel->visible() && p.second.panel->scroll(delta)) return true;
			}
			return false;
		}
		void stack_panel::draw_panel(canvas& table)
		{
			for (auto p : m_stack)
			{
				if (p.second.panel->visible())
				{
					p.second.panel->draw(table);
				}
			}
		}

		void stack_panel::add(panel_id name_tag, panel_ptr panel, alignment align)
		{
			remove(name_tag);
			panel->layout(m_bounds);
			m_stack.emplace(name_tag, stacked_panel(panel, align));
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
			for (auto p : m_stack)
			{
				p.second.panel->m_bounds = p.second.align.calculate(m_bounds);
				p.second.panel->layout(m_bounds);
			}
		}
	}
}